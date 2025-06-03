// SPDX-License-Identifier: MIT
/**
 * @file dmctl.h
 * @brief Header related to device-mapper setup.
 */
#ifndef __DMCTL_H__
#define __DMCTL_H__

#include <stddef.h>

#include "meta.h"
#include "tpm.h"

/**
 * The name of the new device mapper node. Path to dm volume will be `/dev/<DM_DIR>/<COMINIT_ROOTFS_DM_NAME>`. `DM_DIR`
 * is usually `mapper`.
 */
#define COMINIT_ROOTFS_DM_NAME "rootfs"

/**
 * Set up a dm-verity or dm-integrity rootfs according to given metadata.
 *
 * The \a rfsMeta structure must have been initialised/loaded by cominitLoadVerifyMetadata() and contain a configuration
 * requiring either dm-verity or dm-integrity. The function will set up a new device mapper node according to
 * #COMINIT_ROOTFS_DM_NAME. The full path to the new device node will be written to cominitRfsMetaData_t::devicePath in
 * \a rfsMeta. This path can then be used to mount the filesystem.
 *
 * If the parameter cominitRfsMetaData_t::crypt in rfsMeta contains something different from either
 * #COMINIT_CRYPTOPT_VERITY or #COMINIT_CRYPTOPT_INTEGRITY, an error will be returned.
 *
 * @param rfsMeta  The rootfs configuration metadata. See rfs_meta_data.
 *
 * @return  0 on success, -1 otherwise
 */
int cominitSetupDmDevice(cominitRfsMetaData_t *rfsMeta);

/**
 * Set up a dm-crypt mapping for a given block device using a raw key.
 *
 * @param device  The path to the raw block device to encrypt.
 * @param name  The name of the created device mapper node.
 * @param key  The key to encrypt the device.
 * @param offsetSectors  The number of 512-byte sectors to skip at the start of the device. That sectors are not mapped
 * and therefor not encrypted.
 *
 * @return  0 on success, -1 otherwise
 */
int cominitSetupDmDeviceCrypt(char *device, const char *name, const TPM2B_DIGEST *key, uint64_t offsetSectors);

#endif /* __DMCTL_H__ */
