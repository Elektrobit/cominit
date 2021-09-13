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
    $BASEDIR
make -C $BASEDIR/build
cp $BASEDIR/build/src/cominit $BASEDIR/result/bin/aarch64

# build and copy aarch64 rpm
CMAKE_TOOLCHAIN_FILE=$BASEDIR/ci/aarch64-toolchain.cmake \
    make -C $BASEDIR/build rpmbuild
cp -a packaging/rpmbuild/RPMS $BASEDIR/result
cp -a packaging/rpmbuild/SRPMS $BASEDIR/result

# build and copy documentation
make -C $BASEDIR/build doxygen
cp -a $BASEDIR/doc $BASEDIR/result
