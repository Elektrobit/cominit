// SPDX-License-Identifier: MIT
/**
 * @file minsetup.h
 * @brief Header related to minimal system setup.
 */
#ifndef __MINSETUP_H__
#define __MINSETUP_H__

#include <stdbool.h>

#include "meta.h"

/**
 * Setup a minimal environment.
 *
 * Sets up a minimal environment to do what we need to do in initramfs. Currently this is limited to mounting a devtmpfs
 * on /dev but may include further steps in the future as needed. If a devtmpfs is already mounted to `/dev`, this will
 * do nothing.
 *
 * @return 0 on success, -1 on error
 */
int cominitSetupSysfiles(void);

/**
 * Cleanup initramfs environment.
 *
 * Meant to clean the environment (mounts, temporary files) before changing the root directory to rootfs and exec-ing
 * into rootfs init. For now, it will perform a 'lazy' unmount of /dev using MNT_DETACH.
 *
 * @return 0 on success, -1 on error
 */
int cominitCleanupSysfiles(void);

/**
 * Mount rootfs at /newroot.
 *
 * Will mount the rootfs partition according to the options set in \a rfsMeta. Will call cominitSetupDmDevice) if
 * \a rfsMeta specifies a rootfs using device mapper features.
 *
 * @param rfsMeta  Pointer to an cominitRfsMetaData_t struct specifying which kind of rootfs to mount and where
 *                 to find it. See cominitRfsMetaData_t definition for details.
 *
 * @return 0 on success, -1 on error
 */
int cominitSetupRootfs(cominitRfsMetaData_t *rfsMeta);

/**
 * Switch into the rootfs mounted at `/newroot`.
 *
 * Will free up memory in the initramfs and move the root mount.
 *
 * @return 0 on success, -1 on error
 */
int cominitSwitchIntoRootfs(void);

#endif /* __MINSETUP_H__ */
