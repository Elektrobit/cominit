// SPDX-License-Identifier: MIT
/**
 * @file dmctl.h
 * @brief Header related to device-mapper setup.
 */
#ifndef __DMCTL_H__
#define __DMCTL_H__

#include "meta.h"

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

#endif /* __DMCTL_H__ */
