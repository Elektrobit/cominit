#!/bin/bash -e
# SPDX-License-Identifier: MIT

###############################################################################
print_info() {
    SCRIPT_NAME="${0##*/}"
    echo "
    Build cominit sources. Specify build type to utilize compiler features for release or test build.

    Usage: ${SCRIPT_NAME} [--ci] [-h|--help] [BUILD_TYPE]

    BUILD_TYPE      [Release|Analyzer|Asan|Ubsan], may be empty as well (use standard compile options)
    --clean         remove build directory before build
    -h|--help:      print this help

    Examples:
    ${0} # normal build with no extra compile options (suitable for development iterations)
    ${0} --ci Release # trigger release build
    ${0} Asan # build with address analyzer enabled
    "
}
###############################################################################

CMDPATH=$(cd $(dirname $0) && pwd)
BASEDIR=${CMDPATH%/*}
CMAKE_PARAM=${CMAKE_PARAM:-""}

OPTION_CLEAN=0

while [ $# -gt 0 ]; do
    case ${1} in
        --clean | -c) OPTION_CLEAN=1 ;;
        --help | -h)
            print_info
            exit 0
            ;;
        -*)
            echo "error: unknown option: ${1}"
            print_info
            exit 1
            ;;
        *)
            PARAM="${PARAM} ${1}"
            ;;
    esac
    shift
done

set -- $PARAM

if [ $# -gt 1 ]; then
    echo "error: only one build-type allowed"
    exit 1
fi

BUILD_TYPE="${1}"
if [ $# -eq 0 ]; then
    BUILD_TYPE="Debug"
fi

BUILDDIR="${BASEDIR}/build/${BUILD_TYPE}"
CMAKE_BUILD_DIR="${BUILDDIR}/cmake"
RESULTDIR="${BUILDDIR}/result/"
DIST_DIR="${DIST_DIR:-"${BUILDDIR}/dist"}"
CMAKE_PARAM="${CMAKE_PARAM} -DCMAKE_INSTALL_PREFIX:PATH=${DIST_DIR}/usr/local/ -DDOC_BUILD_DIR=${BUILDDIR}"

if [ $OPTION_CLEAN -eq 1 ]; then
    if [ -e "$BUILD_DIR" ]; then
        echo "Removing $BUILD_DIR ..."
        rm -rf "$BUILD_DIR"
    fi
fi

mkdir -p ${RESULTDIR}

cd "$BASEDIR"

cmake -B "$CMAKE_BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    -DCMAKE_VERBOSE_MAKEFILE=On \
    -DUNIT_TESTS=On \
    -DFAKE_HSM=On \
    -DUSE_TPM=On \
    ${CMAKE_PARAM} \
    "$BASEDIR"
make -C "$CMAKE_BUILD_DIR" all install

make -C "$CMAKE_BUILD_DIR" doxygen
