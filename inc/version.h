// SPDX-License-Identifier: MIT
/**
 * @file version.h
 * @brief Header definitions related to component versioning.
 */
#ifndef __VERSION_H__
#define __VERSION_H__

/**
 * Returns a const char pointer to a formatted human-readable version string for this software component.
 *
 * Generated via CMake in version.c.in. Format is `MAJOR.MINOR.MICRO.GIT` if a git commit hash exists or
 * `MAJOR.MINOR.MICRO` if the git hash is an empty string.
 *
 * @return  Pointer to a version string constant.
 */
const char *cominitGetVersionString(void);

#endif /* __VERSION_H__ */
