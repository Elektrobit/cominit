// SPDX-License-Identifier: MIT
/**
 * @file automount.h
 * @brief Header related to automatically mounting according to Discoverable Partitions Specification
 * (see https://uapi-group.org/specifications/specs/discoverable_partitions_specification/)
 */
#include <stddef.h>

#include "meta.h"

/**
 * Structure holding the GPT as defined by the UEFI specification.
 */
typedef struct {
    uint8_t signature[8];            ///< "EFI PART"
    uint32_t revision;               ///< GPT specification revision
    uint32_t headerSize;             ///< size of this header in bytes
    uint32_t headerCrc32;            ///< CRC32 of the header
    uint32_t reserved;               ///< must be zero
    uint64_t currentLba;             ///< LBA of this header copy
    uint64_t backupLba;              ///< LBA of the backup header
    uint64_t firstUsableLba;         ///< first usable LBA for partitions
    uint64_t lastUsableLba;          ///< last usable LBA for partitions
    uint8_t diskGuid[16];            ///< GUID for the whole disk
    uint64_t partitionEntriesLba;    ///< starting LBA of partition entries array
    uint32_t partitionEntryCount;    ///< number of partition entries
    uint32_t partitionEntrySize;     ///< size of each partition entry in bytes
    uint32_t partitionEntriesCrc32;  ///< CRC32 of the partition entries array
} __attribute__((packed)) cominitGPTHeader_t;

/**
 * Structure holding metadata for a disk that uses GPT.
 */
typedef struct {
    char diskName[COMINIT_ROOTFS_DEV_PATH_MAX];  ///< Device node path of the whole disk
    int blockSize;                               ///< Logical block size in bytes.
    cominitGPTHeader_t hdr;                      ///< Parsed GPT header structure for this disk.
} cominitGPTDisk_t;

#define COMINIT_ROOTFS_GUID_TYPE "b921b045-1df0-41c3-af44-4c6f280d3fae"          ///< THE GUID of the rootfs.
#define COMINIT_SECURE_STORAGE_GUID_TYPE "CA7D7CCB-63ED-4C53-861C-1742536059CC"  ///< THE GUID of the secure storage.
#define GPT_HEADER_DEFAULT_ENTRY_SIZE \
    128  ///< The default entry size within a GPT header as defined in UEFI specification.
/**
 * Find a partition of a given type GUID.
 *
 * This function searches for a partition whose type GUID matches @p guidType.
 *
 * All block devices under /dev are scanned until a matching partition is found.
 *
 * When a matching partition is found, the corresponding device node
 * is written to @p partitionName. In addition, @p gptDisk is updated to describe
 * the disk on which the partition resides.
 * This allows cominitAutomountFindPartitionOnDisk() to be called with the same
 * @p gptDisk to search for additional partitions of other GUID types without
 *  re-scaning all block devices under /dev.
 *
 * @param[out] gptDisk   Pointer to a cominitGPTDisk_t struct. On success, it will
 *                       contain the GPT disk where the partition was found.
 * @param[in] guidType      The GUID type that should be looked for in the GPT header.
 * @param[out] partitionName    Pointer to a buffer that receives device node of the partition.
 * @param[in] partitionNameSize The size of the buffer.
 *
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
int cominitAutomountFindPartition(cominitGPTDisk_t *gptDisk, const char *guidType, char *partitionName,
                                  size_t partitionNameSize);

/**
 * Tries to find the GUID type inside the GPT header on a given not empty disk @p gptDisk.
 * If found the partition device is copied to @p partitionName.
 *
 * @param[in] gptDisk       Pointer to a cominitGPTDisk_t struct containing a valid disk with GPT header.
 * @param[in] guidType      The GUID type that should be looked for in the GPT header.
 * @param[out] partitionName    Pointer to a buffer that receives device node of the partition.
 * @param[in] partitionNameSize The size of the buffer.
 *
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
int cominitAutomountFindPartitionOnDisk(cominitGPTDisk_t *gptDisk, const char *guidType, char *partitionName,
                                        size_t partitionNameSize);
