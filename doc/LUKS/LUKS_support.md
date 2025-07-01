

# ADR LUKS support to cominit

## Table of Contents

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [Problem](#problem)
- [Assumptions](#assumptions)
- [Considered alternatives](#considered-alternatives)
  - [1) home grown LUKS functionality](#1-home-grown-luks-functionality)
    - [Binary header:](#binary-header)
    - [JSON metadata:](#json-metadata)
  - [2) cryptsetup](#2-cryptsetup)
    - [cryptsetup - further details](#cryptsetup---further-details)
- [Decision](#decision)
  - [Review Team](#review-team)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Problem

According to the Linux kernel documentation, LUKS (Linux Unified Key Setup) is the de facto standard for encrypted volumes
and is the recommended way to configure disk encryption with dm-crypt:

> LUKS (Linux Unified Key Setup) is now the preferred way to set up disk
> encryption with dm-crypt using the 'cryptsetup' utility

see [kernel.org/doc](https://www.kernel.org/doc/Documentation/admin-guide/device-mapper/dm-crypt.rst)

Moreover, cryptsetup is recommended and remains the only tool capable of natively creating LUKS containers.

## Assumptions

Cryptsetup is provided under GPL-2.0 license. We assume that ist not an issue and can be handled i.e. by forking a child process
that calls the cryptsetup utility. It can also be build against musl libc.
As cryptsetup is the only option out of the box, the only other way is to implement LUKS header functionality oneself.

## Considered alternatives


### 1) home grown LUKS functionality

To estimate the complexity of this undertaking let start with header layout:

LUKS header layout:

```
┌──────────────────┬─────────────────┬───────────────────────┐
│  Binary header   │   JSON metadata │   Keyslots area       │
│  (verification,  │   (keyslot,     │   (Argon2 blobs)      │
│   offsets,       │    tokens)      │                       │
│volume encryption)│                 │                       │
│  ──────────────  │  Metadata area  │                       │
│ bytes  0…16383   │ bytes  0…16383  │ bytes 16384…16777215  │
└──────────────────┴─────────────────┴───────────────────────┘
```

see [LUKS2-docs] (https://gitlab.com/cryptsetup/LUKS2-docs/blob/main/luks2_doc_wip.pdf)

Areas in a nutshell:

* Binary header: verification (magic value, version, CRC) of a LUKS container, locating data by offsets, general volume encryption information
* JSON metadata: metadata of each keyslot. With this metadata (salt, cypher, keysize, etc) and a passphrase the volume key from the keyslots area can be decrypted.
    * Additional parts in JSON metadata
        * config: ciphers, key sizes, sector sizes
        * segments: which byte ranges of the device are encrypted and how
        * tokens: retrieve the passphrase automatically (TPM, Kernel key ring, FIDO as YubiKey).
        * digests: hash for verification
* Keyslots area: each entry is the same single volume key encrypted under a different wrapping key (passphrase, TPM2 token, FIDO device, etc.).

So everything is about verification of a LUKS container and retrieving the volume key and its cypher.

#### Binary header:

```
struct luks2_hdr_disk {
	char		magic[LUKS2_MAGIC_L];
	uint16_t	version;	/* Version 2 */
	uint64_t	hdr_size;	/* in bytes, including JSON area */
	uint64_t	seqid;		/* increased on every update */
	char		label[LUKS2_LABEL_L];
	char		checksum_alg[LUKS2_CHECKSUM_ALG_L];
	uint8_t		salt[LUKS2_SALT_L]; /* unique for every header/offset */
	char		uuid[LUKS2_UUID_L];
	char		subsystem[LUKS2_LABEL_L]; /* owner subsystem label */
	uint64_t	hdr_offset;	/* offset from device start in bytes */
	char		_padding[184];
	uint8_t		csum[LUKS2_CHECKSUM_L];
	char		_padding4096[7*512];
	/* JSON area starts here */
} __attribute__ ((packed));
```

The JSON area in the binary header contains information of the volume key (cypher, key length).

#### JSON metadata:

Example can be retrieved via

Pseudo code

```
cryptsetup luksDump
  --dump-json-metadata
  --header luks-header.img
  /dev/crypt > header.json

jq . header.json
```

from an existing LUKS volume.

Example:

```
{
  "keyslots": {
    "0": {
      "type": "luks2",
      "key_size": 64,
      "af": {
        "type": "luks1",
        "stripes": 4000,
        "hash": "sha256"
      },
      "area": {
        "type": "raw",
        "offset": "32768",
        "size": "258048",
        "encryption": "aes-xts-plain64",
        "key_size": 64
      },
      "kdf": {
        "type": "argon2id",
        "time": 7,
        "memory": 1048576,
        "cpus": 4,
        "salt": "WQy3X7HHXjKdaWeZQxvodn4OjIPlgYfF1SvI1lu/RiE="
      }
    },
    "1": {
      "type": "luks2",
      "key_size": 64,
      "af": {
        "type": "luks1",
        "stripes": 4000,
        "hash": "sha256"
      },
      "area": {
        "type": "raw",
        "offset": "290816",
        "size": "258048",
        "encryption": "aes-xts-plain64",
        "key_size": 64
      },
      "kdf": {
        "type": "argon2id",
        "time": 7,
        "memory": 1048576,
        "cpus": 4,
        "salt": "Ow3+gzLXu5mcQApVwguSXcPCNfiWFQhBmRoV8TIzBTs="
      }
    },
  },
  "tokens": {},
  "segments": {
    "0": {
      "type": "crypt",
      "offset": "16777216",
      "size": "dynamic",
      "iv_tweak": "0",
      "encryption": "aes-xts-plain64",
      "sector_size": 512
    }
  },
  "digests": {
    "0": {
      "type": "pbkdf2",
      "keyslots": [
        "0",
        "1",
      ],
      "segments": [
        "0"
      ],
      "hash": "sha256",
      "iterations": 378820,
      "salt": "WpVD+h5e9L7RJ9hqwP7eM0wn2/QEMaAi66l5W0EA0uo=",
      "digest": "RFxnNEmB6rP0Zbvn/5yU1pLLHWI6UkN7UH35tsprxuM="
    }
  },
  "config": {
    "json_size": "12288",
    "keyslots_size": "16744448"
  }
}
```

Example token for keyring:

```
"tokens":{
  "0":{
    "type": "luks2-keyring,
    "keyslots":["1"],
    "key_description":"myKey"
  }
}
```

*cons*
- high complexity: re-implementing all header-parsing, keyslot handling, integrity checks, version-migrations, JSON parsing
- Security risk: cryptographic bugs are easy to introduce
- maintenance burden

### 2) cryptsetup

Let cryptsetup do all the magic by

Pseudo code:
```
cryptsetup luksFormat \
        --type luks2 \
        --cipher aes-xts-plain64 \
        --key-size 256 \
        --hash sha256 \
        --key-file - \
        /dev/crypto
```

*pros*
- benefit from ongoing maintenance, audit, and kernel support
- well tested
- standard

*cons*
- adding new external dependencies
- no TPM support

#### cryptsetup - further details

1. Are external dependencies an issue?

Overview over new dependencies:

cryptsetup:


| Package            | Size | Security Concerns                                                     | License           |
|--------------------|------|-----------------------------------------------------------------------|------------------ |
| **cryptsetup**     |  985 | High: Manages on-disk encryption (privileged, crypto parsing).        | LGPL-2.1 & GPL-2.0|
| **eudev**          | 1272 | Normal: Handles device events (moderate privileges).                  | GPL-2             |
| **libjson-c5**     |   68 | Low: JSON parsing library (small, memory-safe code path).             | MIT               |
| **libkmod2**       |   88 | High: Kernel module loader interface (can load arbitrary modules).    | LGPL-2.1          |
| **liblzma5**       |  158 | Low: DEFLATE/XZ compression (mature, minimal attack surface).         | Public Domain     |
| **libpopt0**       |   52 | Low: CLI-option parsing (no network, tiny codebase).                  | MIT/X11           |
| **libssh4**        |  522 | High: SSH protocol implementation (authentication, crypto).           | LGPL-2.1          |
| **libssl3**        |  676 | High: TLS/crypto library (frequent CVEs, high-value).                 | Apache-2.0        |
| **libdevmapper**   |  539 | High: Device-mapper API (block-device manipulation, privileged).      | GPL-2             |
| **libudev1**       |  162 | Normal: Hardware enumeration (runtime rules, moderate privileges).    | LGPL-2.1          |
| **libz1**          |   92 | Low: DEFLATE compression (very mature, widely used).                  | zlib              |
| **lvm2-udevrules** |   18 | Normal: Udev rules for LVM activation (static rules, low complexity). | LGPL-2.1 & GPL-2.0|

--> Additional Package Size (4632 KB) is deemed to not be an issue. Hence this is not a contra.

Additional security issues by adding libkmod2, libssh4, libssl3 and libdevmapper should be evaluated.
libkmod2, libssh4: installing can avoided by using libcryptsetup instead of cryptsetup utility


2. Variant "reduced cryptsetup-helper-tool": Build a purpose-built helper binary (libcryptsetup) or fork a child process that invokes cryptsetup utility?

*pros*

- If libcryptsetup is build into a separate binary any license issue injection to `cominit` can be avoided.
- Also it can only offer the functioniality from cryptsetup that is needed (decrease attack surface) and additional external dependencies can be reduced (i.e. libkmod2, libssh4).

*cons*

- extra effort



3. Cryptsetup throttles passphrase checks to thwart brute-force attacks by inserting a delay between attempts.
 Can this be configured if boot time is too long?
Delay can be configured via iter-time option --> not an issue. Hence no contra.

4. Which system component manages and invokes TPM commands? How should the key returned by the TPM be handled
to maintain maximum security? Can the kernel handle TPM commands?
 Will be answered in another ADR.

## Decision

Option 2, without reduced "cryptsetup-helper-tool"

### Review Team

Following members and teams have reviewed and checked the decision:

| *Team* | *Status* | *Date*   |
| -------| -------- | -------- |
| emlix  | OK       | 16.07.25 |


*Status*

- OK
- not OK
- not yet checked
- don't care
