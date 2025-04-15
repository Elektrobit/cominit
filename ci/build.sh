#!/bin/bash -e
# SPDX-License-Identifier: MIT
#
# project build script
#
CMDPATH=$(cd $(dirname $0) && pwd)
BASEDIR=${CMDPATH%/*}

# architecture name amd64, arm64, ...
ARCH=$(dpkg --print-architecture)

BUILDDIR="$BASEDIR/build/$ARCH"
RESULTDIR="$BASEDIR/result/$ARCH"

# prepare result dir
rm -rf "$RESULTDIR"
mkdir -p "$RESULTDIR"/bin

# build
cd "$BASEDIR"

# build aarch64 cominit, statically linked with musl
mkdir -p "$BUILDDIR"
cmake -B "$BUILDDIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_VERBOSE_MAKEFILE=On \
    -DUNIT_TESTS=On \
    -DFAKE_HSM=On \
    "$BASEDIR"
make -C "$BUILDDIR"

# copy binaries
cp "$BUILDDIR"/src/cominit "$RESULTDIR"/bin/

# build and copy documentation
make -C "$BUILDDIR" doxygen
cp -a $BASEDIR/doc "$RESULTDIR"/
