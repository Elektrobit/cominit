// SPDX-License-Identifier: MIT
/**
 * @file automount.c
 * @brief Implementations related to automatically mounting according to Discoverable Partitions Specification
 * (see https://uapi-group.org/specifications/specs/discoverable_partitions_specification/)
 */

#include "automount.h"

#include <ctype.h>
#include <dirent.h>
#include <endian.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/fs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common.h"
#include "meta.h"
#include "output.h"

/**
 * Gets the block size of a block device with ioctl().
 *
 * @param fd            The open file descriptor of the block device.
 * @param blockSize     Pointer that receives the block size.
 *
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
static int cominitAutomountGetBlockSize(int fd, int *blockSize) {
    int result = EXIT_FAILURE;

    if (ioctl(fd, BLKSSZGET, blockSize) < 0) {
        cominitErrnoPrint("ioctl failed to get block size.");
    } else {
        result = EXIT_SUCCESS;
    }

    return result;
}

/**
 * Checks wether the given name is black listed.
 *
 * @param name  The name to check.
 *
 * @return  1 name is black listed, 0 otherwise
 */
static bool cominitAutomountIsBlacklistedName(const char *name) {
    static const char *const blacklist[] = {
        "loop", "zram", "dm-", "md", "sr", "ram",
    };

    for (size_t i = 0; i < ARRAY_SIZE(blacklist); i++) {
        size_t len = strlen(blacklist[i]);
        if (strncmp(name, blacklist[i], len) == 0) {
            cominitDebugPrint("%s is black listed", name);
            return true;
        }
    }
    return false;
}

/**
 * Tries to find a valid GPT header on a given block device.
 *
 * @param blockDevice   The block device to probe.
 * @param gptDisk       The pointer to a cominitGPTDisk_t struct that receives the disk information.
 *
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
static int cominitAutomountFindGpt(const char *blockDevice, cominitGPTDisk_t *gptDisk) {
    int result = EXIT_FAILURE;
    int fd = open(blockDevice, O_RDONLY | O_CLOEXEC);

    if (fd < 0) {
        cominitErrnoPrint("could not open disk %s.", blockDevice);
    } else {
        if (cominitAutomountGetBlockSize(fd, &gptDisk->blockSize) == EXIT_FAILURE) {
            cominitErrPrint("Could not get block size of disk %s.", blockDevice);
        } else {
            cominitDebugPrint("blockSize: %ld", gptDisk->blockSize);
            cominitGPTHeader_t *hdr = &(gptDisk->hdr);
            if (pread(fd, hdr, sizeof(*hdr), gptDisk->blockSize) != (ssize_t)sizeof(*hdr)) {
                cominitErrnoPrint("Could not read from device %s.", blockDevice);
            } else {
                if (memcmp(hdr->signature, "EFI PART", sizeof(hdr->signature)) != 0) {
                    cominitDebugPrint("No GPT signature found in disk %s.", blockDevice);
                } else {
                    cominitDebugPrint("Disk %s contains GPT signature.", blockDevice);
                    memcpy(gptDisk->diskName, blockDevice, sizeof(gptDisk->diskName));
                    result = EXIT_SUCCESS;
                }
            }
        }
        close(fd);
    }

    return result;
}

/**
 * Reads the raw 16-byte GUID field and formats it into the
 * canonical text form
 *
 * @param[in] rawGuid   Pointer to a 16-byte buffer containing the raw GUID data.
 * @param[out] guid     Pointer to a buffer of at least 37 bytes, where the
 *                      resulting null-terminated string will be written.
 *
 */
static void cominitAutomountFormatGuid(const uint8_t rawGuid[16], char guid[37]) {
    uint32_t a = le32toh(*(const uint32_t *)(rawGuid + 0));
    uint16_t b = le16toh(*(const uint16_t *)(rawGuid + 4));
    uint16_t c = le16toh(*(const uint16_t *)(rawGuid + 6));
    snprintf(guid, 37, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", a, b, c, rawGuid[8], rawGuid[9], rawGuid[10],
             rawGuid[11], rawGuid[12], rawGuid[13], rawGuid[14], rawGuid[15]);
}

/**
 * Builds the partition device node from index in the partition table.
 *
 * @param[in] diskName     Pointer to a buffer containing a valid disk name.
 * @param[in] idx           The index of the partition within the partition table.
 * @param[out] partitionName    Pointer to a buffer that receives device node of the partition.
 * @param[in] partitionNameSize The size of the buffer.
 *
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
static int cominitAutomountBuildPartitionNode(const char *diskName, unsigned idx, char *partitionName,
                                              size_t partitionNameSize) {
    int result = EXIT_FAILURE;
    size_t len = strlen(diskName);
    bool endsWithDigit = (len > 0 && isdigit((unsigned char)diskName[len - 1]));

    int n = -1;
    if (endsWithDigit)
        n = snprintf(partitionName, partitionNameSize, "%sp%u", diskName, idx);
    else
        n = snprintf(partitionName, partitionNameSize, "%s%u", diskName, idx);

    if (n < 0 || (size_t)n >= partitionNameSize) {
        cominitErrnoPrint("buffer too small  %s.", partitionName);
    } else {
        result = EXIT_SUCCESS;
    }

    return result;
}

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
                                        size_t partitionNameSize) {
    int result = EXIT_FAILURE;

    if (gptDisk == NULL || gptDisk->diskName[0] == '\0' || guidType == NULL || partitionName == NULL ||
        partitionNameSize == 0) {
        cominitErrPrint("Invalid parameters");
    } else {
        int fd = open(gptDisk->diskName, O_RDONLY | O_CLOEXEC);
        if (fd < 0) {
            cominitErrnoPrint("Could not open disk %s.", gptDisk->diskName);
        } else {
            cominitGPTHeader_t *hdr = &(gptDisk->hdr);
            uint64_t diskSize = 0;
            if (cominitCommonGetPartSize(&diskSize, fd) == -1) {
                cominitErrPrint("Could not get size of disk %s.", gptDisk->diskName);
            } else {
                uint32_t partitionEntrySize = hdr->partitionEntrySize;
                if (partitionEntrySize < GPT_HEADER_DEFAULT_ENTRY_SIZE || partitionEntrySize > diskSize) {
                    cominitErrPrint("Entry size of gpt header invalid.");
                } else {
                    uint8_t *entryBuffer = malloc(partitionEntrySize);
                    if (!entryBuffer) {
                        cominitErrnoPrint("Allocation of entry buffer failed");
                    } else {
                        char typeGuidString[37] = {0};
                        for (uint32_t entryIndex = 0; entryIndex < hdr->partitionEntryCount; ++entryIndex) {
                            uint64_t tableBaseBytes = hdr->partitionEntriesLba * (uint64_t)gptDisk->blockSize;
                            uint64_t entryOffsetBytes = tableBaseBytes + (uint64_t)entryIndex * partitionEntrySize;
                            if (entryOffsetBytes > diskSize || entryOffsetBytes > UINT64_MAX) {
                                cominitErrPrint("Entry offset to large.");
                                break;
                            }
                            off_t entryOffset = (off_t)entryOffsetBytes;
                            ssize_t bytesRead = pread(fd, entryBuffer, partitionEntrySize, entryOffset);
                            if (bytesRead != (ssize_t)partitionEntrySize) {
                                cominitErrnoPrint("Could only read %zd bytes from partition entry of %zd byte size)",
                                                  bytesRead, partitionEntrySize);
                                break;
                            }
                            bool typeGuidAllZero = true;
                            for (int b = 0; b < 16; ++b) {
                                if (entryBuffer[b] != 0) {
                                    typeGuidAllZero = false;
                                    break;
                                }
                            }
                            if (typeGuidAllZero) {
                                continue;
                            }
                            cominitAutomountFormatGuid(entryBuffer, typeGuidString);
                            if (strcasecmp(typeGuidString, guidType) == 0) {
                                result = cominitAutomountBuildPartitionNode(gptDisk->diskName, entryIndex + 1,
                                                                            partitionName, partitionNameSize);
                                break;
                            }
                        }
                        free(entryBuffer);
                    }
                }
            }
            close(fd);
        }
    }

    return result;
}

int cominitAutomountFindPartition(cominitGPTDisk_t *gptDisk, const char *guidType, char *partitionName,
                                  size_t partitionNameSize) {
    int result = EXIT_FAILURE;
    if (gptDisk == NULL || gptDisk->diskName == NULL || guidType == NULL || partitionName == NULL ||
        partitionNameSize == 0) {
        cominitErrPrint("Invalid parameters");
    } else {
        DIR *d = opendir("/dev");
        if (!d) {
            cominitErrnoPrint("Could not open /dev for gpt disk scan.");
        } else {
            struct dirent *deviceEntry = NULL;
            char device[2 * COMINIT_ROOTFS_DEV_PATH_MAX] = {0};
            struct stat st = {0};

            while ((deviceEntry = readdir(d))) {
                const char *name = deviceEntry->d_name;
                if (name[0] == '.') continue;
                if (cominitAutomountIsBlacklistedName(name)) continue;

                snprintf(device, sizeof(device), "/dev/%s", name);
                if (lstat(device, &st) != 0) continue;
                if (S_ISLNK(st.st_mode)) continue;
                if (!S_ISBLK(st.st_mode)) continue;

                cominitGPTDisk_t diskToProbe = {0};
                result = cominitAutomountFindGpt(device, &diskToProbe);
                if (result == EXIT_SUCCESS) {
                    result =
                        cominitAutomountFindPartitionOnDisk(&diskToProbe, guidType, partitionName, partitionNameSize);
                    if (result == EXIT_SUCCESS) {
                        memcpy(gptDisk, &diskToProbe, sizeof(*gptDisk));
                        break;
                    }
                }
            }
            closedir(d);
        }
    }

    return result;
}