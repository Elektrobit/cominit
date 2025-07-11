

# ADR Key handling with LUKS

## Table of Contents

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [Problem](#problem)
- [Assumptions](#assumptions)
- [Considered alternatives](#considered-alternatives)
  - [Definitions](#definitions)
  - [1) key handling in kernel space](#1-key-handling-in-kernel-space)
    - [Go upstream](#go-upstream)
  - [2) key handling in user space](#2-key-handling-in-user-space)
    - [2a) systemd cryptsetup + cryptenroll](#2a-systemd-cryptsetup--cryptenroll)
    - [2b) cryptsetup + own TPM implementation](#2b-cryptsetup--own-tpm-implementation)
      - [Keyring:](#keyring)
      - [stdin:](#stdin)
    - [2c) Forward a raw volume key from TPM to cryptsetup](#2c-forward-a-raw-volume-key-from-tpm-to-cryptsetup)
  - [3) key handling in user and kernel space](#3-key-handling-in-user-and-kernel-space)
- [Decision](#decision)
  - [Review Team](#review-team)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Problem

No matter how robust the encryption algorithm, its security ultimately hinges on effective key management:
Encryption is only as strong as the secrecy and integrity of its keys. Choosing whether to perform key operations
in user-space or kernel-space is a critical decision, as it directly impacts the confidentiality and integrity
of the cryptographic keys.

## Assumptions

A fix requirement is to use LUKS header to store and handle keys as it is the de-facto linux standard. Along with that
requirement the use of cryptsetup is set: Only cryptsetup can create LUKS header and handle the metadata within.
Beyond employing LUKS for volume encryption, the use of a TPM is mandated to protect and hand over the passphrase
for volume key encryption solely when the system is in a trusted state.


## Considered alternatives

### Definitions

key: A piece of secret data that an encryption algorithm uses to transform plaintext into ciphertext (and vice versa).

volume key: The symmetric key that actually encrypts
and decrypts every sector of the LUKS volume.

passphrase: Secret data to unlock a LUKS-key‑slot. It can be a human‑memorable string or a raw key.
The passphrase is needed to indirectly decrypt the volume key.

### 1) key handling in kernel space

The basic idea of a LUKS volume is to encrypt a single volume key with multiple passphrases.
Decryption of the volume key using a passphrase is currently performed by cryptsetup in userspace,
via whichever crypto backend it was built with (libgcrypt, OpenSSL, mbedTLS, etc.).

Because that wrapping and unwrapping happens in userspace, cryptsetup must receive the passphrase in plaintext.

The kernel however can handle TPM operations and can load a TPM‑sealed passphrase into the keyring
without exposing it i.e. by:

```
keyctl add trusted PASSPHRASE "new 32 keyhandle=0x81000000 pcrlock=7" @u
```
That would add a key of type "trusted" to the key ring. But:

> Trusted and Encrypted Keys are two new key types added to the existing kernel key ring service.
> Both of these new types are variable length symmetric keys, and in both cases all keys are created in the kernel,
> and user space sees, stores, and loads only encrypted blobs.

from [kernel.org/doc](https://www.kernel.org/doc/html/v4.13/security/keys/trusted-encrypted.html)

By design, user space cannot retrieve "trusted" (or "encrypted") keys in plaintext—only as encrypted blobs.
That’s why cryptsetup currently only supports the "user" key type from the kernel keyring:

> The key type [...] must always be 'user' since we need to read the actual key
> data from user space [...].

From [cryptsetup/keyring](https://gitlab.com/cryptsetup/cryptsetup/-/blob/main/docs/Keyring.txt)

So to keep key handling in kernel solely, cryptsetup must be extended and the kernel be patched:

#### Go upstream

Rather than performing all header key wrapping/unwrapping in userspace, cryptsetup would:

1. Use a pre‑loaded kernel key

  * The passphrase is sealed into the kernel keyring as "trusted" key via TPM
  * cryptsetup is passed a kernel key ID (no plaintext key ever appears in its memory).

2. Parse and manage the LUKS header and metadata
  (cipher, IV mode, keyslots) as it does today.

3. Call a new kernel API

  * Supply only the encrypted keyslot blob plus the kernel key ID.

  * The kernel unwraps the volume key under that sealed passphrase, entirely in kernel space

4. cryptsetup then sets up the dm‑crypt device by passing the handle to the volume key; it never sees raw keys.

cryptsetup would never see the plaintext volume key; it would only handle encrypted blobs and key references.

*pros*
- Volume keys and passphrases never touch user memory—they remain protected in the kernel keyring and TPM.
- Leverages kernel TPM support, secure elements, and hardware crypto accelerators.

*cons*
- Requires upstream changes to both the Linux kernel (new keyring unwrap/wrap API) and cryptsetup.
  (strong contra, ruling out this option)

### 2) key handling in user space

#### 2a) systemd cryptsetup + cryptenroll

Standard way of combining LUKS and TPM support.
Token can be configured to get passphrase from TPM automatically:

Pseudo code:
```
systemd-cryptenroll \
  --tpm2-device=auto \
  --tpm2-pcrs="0+1+2" \
  /dev/crypto
```

*pros*
- benefit from ongoing maintenance, audit, and kernel support
- well tested
- standard

*cons*
- adding new external dependencies
- no builtin update mechanism
- systemd dependency (strong contra, ruling out this option)

#### 2b) cryptsetup + own TPM implementation

The basic concept of this approach:

1. Seal a passphrase by TPM in user space
1. Feed the unsealed passphrase to cryptsetup

The seal and unseal mechanic with TPM has already been implemented in cominit.

After unsealing the passphrase can be forwarded via stdin or by token from keyring:

Pseudo code:

##### Keyring:

Add key(=Passphrase) to keyring:
```
syscall(SYS_add_key, "user", keyDesc, key, keyLen, KEY_SPEC_USER_KEYRING)
```
Add keyring token to cryptsetup:
```
cryptsetup token add --key-description my:key --token-type keyring /dev/crypt
```
Open LUKS volume:
```
cryptsetup open /dev/crypt secureStorage
```
##### stdin:


create child:
```
pid_t pid = fork();
```
execute cryptsetup in child with "-" as value of "key-file" option:
```
if (pid == 0)
char *const argv[] = {COMINIT_CRYPTSETUP_DIR,
                      "luksOpen",
                      "--key-file",
                      "-",
                      devCrypt,
                      secureStorage,
                    }

execve(path, argv, env);
```
connect pipe and then write to STDIN of child
```
if (pid > 0)
write(pipefd[1], passphrase, passphraseSize);
```

*pros*
- working solution: Already implemented as proof of concept

*cons*
- exposure risk: Passing the passphrase via stdin increases the attack surface
- Less standardized:  maintenance burden when kernels or TPM tools evolve
- Security risk: cryptographic bugs can be introduced

#### 2c) Forward a raw volume key from TPM to cryptsetup

Pseudo code:

```
tpm2_unseal \
  --object-context=seal.ctx \
  --auth=pcr:sha256:0,1,2,3 \
  --output=keyfile

cryptsetup luksFormat \
  --type luks2 \
  --cipher aes-xts-plain64 \
  --key-size 512 \
  --hash sha256 \
  --volume-key-file keyfile
  /dev/crypto

cryptsetup open \
  --type luks2 \
  --volume-key-file keyfile \
  /dev/crypto   secureStorage
```

From [cryptsetup/FAQ] (https://github.com/mhfan/cryptsetup/blob/master/FAQ)

> 2.9 How do I read the LUKS master key from file?
>
>  The only legitimate reason I can think of is if you want
>  to have two LUKS devices with the same master key. Even then, I
>  think it would be preferable to just use key-slots with the same
>  passphrase, or to use plain dm-crypt instead.

--> doable but not recommended

### 3) key handling in user and kernel space

The basic concept of this approach:

1. Seal a passphrase by TPM handling in kernel space
1. Feed the unsealed passphrase to cryptsetup

Not possible:

The kernel can create a TPM sealed key ("trusted" key) that can be used as passphrase but
the kernel never exposes a "trusted" key to user space.

*pros*
- TPM handling in kernel: well tested and no maintenance burden

*cons*
- by design not possible (strong contra, ruling out this option)


## Decision

Option 1 is ideal but not currently feasible; Option 2b will be used.

### Review Team

Following members and teams have reviewed and checked the decision:

| *Team* | *Status* | *Date*   |
| -------| -------- | -------- |
| emlix  | OK       | 29.07.25 |


*Status*

- OK
- not OK
- not yet checked
- don't care
