#!/bin/bash -e
# SPDX-License-Identifier: MIT
#
# script to prepare and run clang-tidy on the project
#
# Dependency: ci/build.sh needs to be run before
#
CMDPATH=$(cd $(dirname $0) && pwd)
BASEDIR=${CMDPATH%/*}

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

if [ ! -f "${CMAKE_BUILD_DIR}"/compile_commands.json ]; then
    echo "Build environment \"${CMAKE_BUILD_DIR}\" not set up. Please run ci/build.sh first!" >&2
    exit 1
fi

# remove -fanalyzer as it's unknown to clang
sed -i -e 's/\-fanalyzer//g' "${CMAKE_BUILD_DIR}"/compile_commands.json

rm -rf $RESULTDIR/clang-tidy
mkdir $RESULTDIR/clang-tidy

cd $BASEDIR

CLANG_TIDY_FLAGS=(-p "${CMAKE_BUILD_DIR}" --extra-arg-before='--target=amd64-linux-gnu')

# run clang-tidy for cominit
clang-tidy "${CLANG_TIDY_FLAGS[@]}" -dump-config -header-filter='inc\/*.h' inc/*.h src/*.c "$CMAKE_BUILD_DIR"/src/version.c >$RESULTDIR/clang-tidy/config
# catch errors even though we use a pipe to tee
set -o pipefail
clang-tidy "${CLANG_TIDY_FLAGS[@]}" -header-filter='inc\/*.h' inc/*.h src/*.c "$CMAKE_BUILD_DIR"/src/version.c 2>&1 | tee $RESULTDIR/clang-tidy/report-cominit

# run clang-tidy for unit tests
for d in $BASEDIR/test/utest/*/utest-*/; do
    SUBDIR=$d
    clang-tidy "${CLANG_TIDY_FLAGS[@]}" -header-filter='inc\/*.h' $SUBDIR/*.c 2>&1 | tee $RESULTDIR/clang-tidy/report-$(basename $SUBDIR)
done

# run clang-tidy for mocks
clang-tidy "${CLANG_TIDY_FLAGS[@]}" -header-filter='inc\/*.h' $BASEDIR/test/mocks/*/*.c 2>&1 | tee $RESULTDIR/clang-tidy/report-mocks
