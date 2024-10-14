#!/bin/bash -e
# SPDX-License-Identifier: MIT
#
# project build script
#
CMDPATH=$(cd $(dirname $0) && pwd)
BASEDIR=${CMDPATH%/*}

# prepare result dir
rm -rf $BASEDIR/result
mkdir -p $BASEDIR/result/bin/aarch64

# build
cd $BASEDIR

# build aarch64 cominit, statically linked with musl
mkdir -p $BASEDIR/build
cmake -B $BASEDIR/build \
    -DCMAKE_TOOLCHAIN_FILE=$BASEDIR/ci/aarch64-toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_VERBOSE_MAKEFILE=On \
    -DUNIT_TESTS=On \
    -DFAKE_HSM=On \
    -DCMAKE_EXE_LINKER_FLAGS="-static -Wl,--gc-sections" \
    $BASEDIR
make -C $BASEDIR/build
cp $BASEDIR/build/src/cominit $BASEDIR/result/bin/aarch64

# build and copy documentation
make -C $BASEDIR/build doxygen
cp -a $BASEDIR/doc $BASEDIR/result
