# SPDX-License-Identifier: MIT
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_C_COMPILER /usr/local/aarch64-musl/bin/musl-gcc)
set(CMAKE_C_FLAGS_INIT "-march=armv8-a+crc -mcpu=cortex-a53 -isystem /usr/local/aarch64-musl/include")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-L/usr/local/aarch64-musl/lib")
set(CMAKE_AR aarch64-linux-gnu-ar)

set(CMAKE_FIND_ROOT_PATH "/usr/local/aarch64-musl")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(UNIT_TEST_INTERPRETER "qemu-aarch64-static")
