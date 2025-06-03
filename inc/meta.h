// SPDX-License-Identifier: MIT
/**
 * @file meta.h
 * @brief Header related to partition metadata handling.
 */
#ifndef __META_H__
#define __META_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** The location of the public key to verify the rootfs partition metadata. **/
#define COMINIT_ROOTFS_KEY_LOCATION "/etc/rootfs_key_pub.pem"

/** Version of the partition metadata. This is incremented if parsing changes.  **/
#define COMINIT_PART_META_DATA_VERSION "1"

/** Maximum size of the device mapper tables. **/
#define COMINIT_DM_TABLE_SIZE_MAX 1024
/** Maximum size of the path to the rootfs block device. May additionally be limited by #COMINIT_DM_TABLE_SIZE_MAX. **/
#define COMINIT_ROOTFS_DEV_PATH_MAX 256
/** Maximum length of the filesystem type identifier. **/
#define COMINIT_FSTYPE_STR_MAX_LEN 32

/** Size (in Bytes) of the metadata region at the end of the rootfs patition. **/
#define COMINIT_PART_META_DATA_SIZE 4096
/** Size (in Bytes) of the signature within the metadata region **/
#define COMINIT_PART_META_SIG_LENGTH 512

/** Bitmask specifiying which dm-crypt/verity/integrity features to use, if any. **/
typedef int8_t cominitCryptOpt_t;
/** No dm-integrity/verity/crypt **/
#define COMINIT_CRYPTOPT_NONE 0
/** dm-verity, partition must be read-only, mutually exclusive with #COMINIT_CRYPTOPT_INTEGRITY **/
#define COMINIT_CRYPTOPT_VERITY (1 << 0)
/** dm-integrity, mutually exclusive with #COMINIT_CRYPTOPT_INTEGRITY **/
#define COMINIT_CRYPTOPT_INTEGRITY (1 << 1)
/** dm-crypt, can be combined with either #COMINIT_CRYPTOPT_VERITY or #COMINIT_CRYPTOPT_INTEGRITY **/
#define COMINIT_CRYPTOPT_CRYPT (1 << 2)

/**
 * Structure holding rootfs partition metadata necessary to set it up correctly. cominitRfsMetaData_t::devicePath is
 * read from the boot command line. Everything else is read from the partition metadata region on disk by
 * cominitLoadVerifyMetadata().
 */
typedef struct cominitRfsMetaData_t {
    char devicePath[COMINIT_ROOTFS_DEV_PATH_MAX];  ///< Path to the partition block device.
    char fsType[COMINIT_FSTYPE_STR_MAX_LEN];       ///< Filesystem type, either "ext4" or "squashfs"
    bool ro;                                       ///< If the filesystem shall be mounted read-only
                                                   ///< (ro == true => read-only).

    cominitCryptOpt_t crypt;  ///< Device mapper cryptographic features to use. See #COMINIT_CRYPTOPT_NONE,
                              ///< #COMINIT_CRYPTOPT_VERITY, #COMINIT_CRYPTOPT_INTEGRITY, #COMINIT_CRYPTOPT_CRYPT.

    uint64_t dmVerintDataSizeBytes;                 ///< Size in Bytes of the resulting volume for dm-verity or
                                                    ///< dm-integrity.
    uint64_t dmCryptDataSizeBytes;                  ///< Size in Bytes of the resulting volume for dm-crypt.
    char dmTableVerint[COMINIT_DM_TABLE_SIZE_MAX];  ///< Space to hold device mapper table for dm-verity or
                                                    ///< dm-integrity.
    char dmTableCrypt[COMINIT_DM_TABLE_SIZE_MAX];   ///< Space to hold device mapper table for dm-crypt
} cominitRfsMetaData_t;

/**
 * Loads and verifies the rootfs partition metadata.
 *
 * Expects a valid metadata region at the end of partition \a meta->devicePath. For details on how this region must
 * look, see README.md (the doxygen main page). All other fields of \a meta will be filled according to the found
 * metadata if signature verification (using the RSASSA-PSS implementation of libmbedcrypto) succeeds.
 *
 * @param meta      The metadata structure to fill. Field \a .devicePath needs to contain the rootfs device path.
 * @param keyfile   Path to the RSA public key for signature verification in PEM-format.
 *
 * @return  0 on success, -1 otherwise
 */
int cominitLoadVerifyMetadata(cominitRfsMetaData_t *meta, const char *keyfile);
/**
 * Get the size of a partition.
 *
 * Uses the BLKGETSIZE64 ioctl() to return the size in Bytes. The given file descriptor must be opened and associated
 * with a partition block device.
 *
 * @param partSize  Return pointer for the size in Bytes.
 * @param fd         The partition file descriptor.
 *
 * @return  0 on success, -1 otherwise
 */
int cominitGetPartSize(uint64_t *partSize, int fd);
/**
 * Convert a series of Bytes to a hexadecimal string representation.
 *
 * Will read \a n Bytes from src and write \f$ 2n+1 \f$ (including the null-Byte) characters to dest. Hexadecimal digits
 * `[a-f]` wil be written in lower-case.
 *
 * @param dest  The output string, needs to have space for at least \f$ 2n+1 \f$ characters.
 * @param src   The array of bytes to convert.
 * @param n     The amount of Bytes in src t convert.
 *
 * @return  0 on success, -1 otherwise
 */
int cominitBytesToHex(char *dest, const uint8_t *src, size_t n);

#endif /* __META_H__ */
