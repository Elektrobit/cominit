
# Compact Init

## Table of Contents

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [Local build using Docker container (including html documentation)](#local-build-using-docker-container-including-html-documentation)
- [Functional Documentation](#functional-documentation)
  - [General Description](#general-description)
  - [Startup](#startup)
  - [Rootfs Partition Metadata](#rootfs-partition-metadata)
    - [Settings Fields](#settings-fields)
    - [DM\_TABLE data](#dm%5C_table-data)
    - [Signature](#signature)
  - [HSM Emulation](#hsm-emulation)
  - [Design decision regarding static linkage and musl](#design-decision-regarding-static-linkage-and-musl)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Local build using Docker container (including html documentation)

from the root directory run:
```
ci/docker-run.sh
```
The docker script will start a Docker container for the native host architecture with all necessary programs to build Cominit and its
Doxygen documentation and to run the tests.
It is possible to run the Docker container for a foreign architecture such as arm64 with the help of qemu-user-static
and binfmt-support. Make sure these packages are installed on your host system if you want to use this functionality.
All following commands to be run inside the container will be the same regardless of the architecture.
```sh
ci/docker-run.sh arm64

Inside the container, it is sufficient to run
```sh
ci/build.sh
```

All further steps (i.e. `clang-tidy` and unit testing) assume the build has been run with `ci/build.sh` and
will fail otherwise.

A `clang-tidy` analysis of the source can be performed using
```
ci/clang-tidy.sh
```
This will also generate a `compile_commands.json`. The output will be saved to `result/clang-tidy`.

To build and run the unit tests, run
```
ci/run-utests.sh
```
The test results will be saved to `result/utest_report.txt`.

## Functional Documentation

### General Description
The resulting binary (`cominit`) expects to be run as the system init process out of an initramfs.
Its purpose is to mount the rootfs partition, switch the root and start the rootfs init. The rootfs filesystem may
be read-only or read-write using any filesystem supported by the Kernel in use.
Continuous integrity checking is supported through dm-verity for read-only and dm-integrity for writable variants.

If successful, `cominit`will clean up after itself and exec into the rootfs init
(`[rootfs]/sbin/init`). On any fatal error, `cominit` will try to exec into a rescue shell. This
will be either `[initramfs]/bin/sh` if the error occured before switching the root or `[rootfs]/bin/sh` if
it occured after.

### Startup
To start `cominit` as the init process, one may copy/symlink `cominit` to `/sbin/init` and/or
specify `rdinit=/path/to/cominit` on the Kernel command line. It is possible to give arguments
to the init process via Kernel command line as well, like so:
```
rdinit=/path/to/cominit [OTHER_KERNEL_PARAMETERS] -- [COMINIT_ARGV1] [COMINIT_ARGV2] [...]
```

`cominit` currently either looks at its last argument (`argv[argc - 1]`) for the location of
the rootfs to mount and switch into. All other settings are read from the partition's metadata.

If the rootfs is not immediately available or accessible, cominit will wait a pre-set interval and try again for a
pre-set number of times. These values are currently set via preprocessor defines but need to made configurable in a
later version.

### Rootfs Partition Metadata
As suggested above, a rootfs partition needs to contain a valid metadata region containing settings
for `cominit` as well as a signature.

The metadata region is defined as the last 4 Kilobytes of a partition. Unused space is padded with zeros. It has a data
and a signature block separated by a single zero-Byte (`\0`). The data field contains three sub-blocks separated by
`\xFF` with ASCII-Strings separated by spaces:

```
>>>The metadata region<<<

================================================= data (ASCII) ======================================++++++++++signature++++++++++++
<meta_ver> <fstype> <mode> <crypt>\xFF<DM_TABLE_VALUES_VERITY_INTEGRITY>\xFF<DM_TABLE_VALUES_CRYPT>\0<512-Byte RSASSA-PSS signature>

```
#### Settings Fields
* **meta_ver** - The version of the metadata format, currently `1`.
* **fstype** - The filesystem type of the rootfs, same format as for the mount() syscall.
* **mode** - Read-only (`ro`) or read-write (`rw`) mount option.
* **crypt** - The device mapper cryptographic features to set up for the rootfs.
    - `plain` - None, `DM_TABLE_VALUES_VERITY_INTEGRITY` and `DM_TABLE_VALUES_CRYPT` can be left empty.
    - `verity` - Activate dm-verity. `DM_TABLE_VALUES_VERITY_INTEGRITY` must contain valid dm-verity data (see below)
      while `DM_TABLE_VALUES_CRYPT` may be left empty.
    - `integrity` - Activate dm-integrity. `DM_TABLE_VALUES_VERITY_INTEGRITY` will need to contain valid dm-integrity
      data (see below) while `DM_TABLE_VALUES_CRYPT` may be left empty.
    - `crypt` - Activate dm-crypt. `DM_TABLE_VALUES_CRYPT` will need to contain valid dm-crypt data (format TBD) while
      `DM_TABLE_VALUES_VERITY_INTEGRITY` may be left empty. Currently unimplemented and will cause boot to fail.
    - `crypt-verity` - Activate dm-verity and dm-crypt. `DM_TABLE_VALUES_VERITY_INTEGRITY` will need to contain valid
      dm-verity data (format TBD) and `DM_TABLE_VALUES_CRYPT` will need to contain valid dm-crypt data (format TBD).
      Currently unimplemented and will cause boot to fail.
    - `crypt-integrity` - Activate dm-integrity and dm-crypt. `DM_TABLE_VALUES_VERITY_INTEGRITY` will need to contain
      valid dm-integrity data (format TBD) and `DM_TABLE_VALUES_CRYPT` will need to contain valid dm-crypt data (format
      TBD). Currently unimplemented and will cause boot to fail.

#### DM\_TABLE data
As shown above, the data block contains two sub-blocks for `DM_TABLE` data if needed. These are settings for
dm-verity/integrity and dm-crypt, respectively. All values are in ASCII text.

Currently dm-verity and (hash-based) dm-integrity are supported. The format for dm-verity (written to
`DM_TABLE_VALUES_VERITY_INTEGRITY`) is
```
<version> <data_block_size> <hash_block_size> <num_data_blocks> <hash_start_block> <algorithm> <digest> <salt>
```
For an explanation of each option, see the [dm-verity Linux Kernel
documentation](https://www.kernel.org/doc/html/latest/admin-guide/device-mapper/verity.html).

For dm-integrity, use the following format:
```
<num_data_blocks> <data_block_size> <num_additional_args> [<additional> <arguments> ... ]
```
The additional arguments are documented in the [dm-integrity Linux Kernel
documentation](https://www.kernel.org/doc/html/latest/admin-guide/device-mapper/dm-integrity.html). Note, that the
option `data_blocks:<number>` is already generated by `cominit` and does not need to be specified in the additional
arguments.

An example for a 512MiB partition formatted with sha256-based dm-integrity (i.e. with less than 512 MiB available for
actual data), 512 Bytes block size, and using the newer padding format would look like
```
978936 512 2 internal_hash:sha256 fix_padding
```
If a parameter with an optional key is specified, cominit can fetch the key from the Kernel user keyring. For that, use
the keyring key description string prefixed with a `:`.

An example for the above partition using sha256 with HMAC and using the key with description `dm-integrity-hmac-secret`
from the user keyring looks like
```
978936 512 2 internal_hash:hmac(sha256)::dm-integrity-hmac-secret fix_padding
```

#### Signature
The signature block beginning after the delimiting zero-byte contains an RSASSA-PSS signature over all bytes from the
beginning of the data block up to and including the delimiting zero. The used hash function is SHA-256. The resulting
signature length is 512 Bytes.

Assuming we have generated the data block including the delimiting Bytes as the file `data.meta` and we have our private
RSA key as `rootfs.key`, it is possible to use openssl to generate a compatible signature like so
```
openssl dgst -sha256 -sigopt rsa_padding_mode:pss -sigopt rsa_pss_saltlen:-1 -sigopt rsa_mgf1_md:sha256 -sign rootfs.key -out sig.meta data.meta
```
This yields a 512 Byte long file `sig.meta` which can then be appended to `data.meta`. The resulting combined file must
then be padded to 4KiB and written to the end of the partition. Possibly, the partition needs to be enlarged/padded
before doing that to provide enough empty space at the end (4kiB, possibly more for correct alignment if necessary).
Finally, the partition table must reflect the total size of the partition including the metadata region and any
additional padding.

`cominit` will expect the fitting public key for verification to be at `/etc/rootfs_key_pub.pem` in the initramfs. To
generate from the private key, one may use
```
openssl rsa -pubout < rootfs.key > rootfs_key_pub.pem
```

### HSM Emulation
If compiled with the optional `-DFAKE_HSM=On` flag, cominit will enroll private keys in the user keyring during early
bootup. This is meant for development purposes in case a real hardware-security module with key storage is unavailable
or not functional. The source file `keyring.h` shows how to define which keys to enroll. The names of the keys need to
be defined at compile-time and the key files themselves need to be accessible to cominit in the initramfs.

### TPM Usage
If compiled with the optional `-DUSE_TPM=On` flag, cominit will add TPM functionality. This flag is currently intended
for development use only, but it will later enable a measured‑boot feature.

### Design decision regarding static linkage and musl

`cominit` is to be statically linked. A decision was made to use the musl libc over the standard glibc for two reasons:

1. Using musl results in considerably smaller binaries than glibc if linking statically.
2. glibc is released under LGPL which does not permit static linking with closed-source code while
   musl is released under the more permissive MIT license without these restrictions.
