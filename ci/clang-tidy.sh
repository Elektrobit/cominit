#!/bin/bash -e
# SPDX-License-Identifier: MIT
#
# script to prepare and run clang-tidy on the project
#
# Dependency: ci/build.sh needs to be run before
#
CMDPATH=$(cd $(dirname $0) && pwd)
BASEDIR=${CMDPATH%/*}
BUILDDIR=build

if [ ! -f "${BUILDDIR}"/compile_commands.json ]; then
    echo "Build environment not set up. Please run ci/build.sh first!" >&2
    exit 1
fi

rm -rf $BASEDIR/result/clang-tidy
mkdir $BASEDIR/result/clang-tidy
cd $BASEDIR

CLANG_TIDY_FLAGS=( -p "${BUILDDIR}" --extra-arg-before='--target=aarch64-linux-gnu' )

# run clang-tidy for cominit
clang-tidy "${CLANG_TIDY_FLAGS[@]}" -dump-config -header-filter='inc\/*.h' inc/*.h src/*.c "$BUILDDIR"/src/version.c > result/clang-tidy/config
# catch errors even though we use a pipe to tee
set -o pipefail
clang-tidy "${CLANG_TIDY_FLAGS[@]}" -header-filter='inc\/*.h' inc/*.h src/*.c "$BUILDDIR"/src/version.c 2>&1 | tee result/clang-tidy/report-cominit

# run clang-tidy for unit tests
for d in $BASEDIR/test/utest-*/ ; do
    SUBDIR=$d
    clang-tidy "${CLANG_TIDY_FLAGS[@]}" -header-filter='inc\/*.h' $SUBDIR/*.c 2>&1 | tee result/clang-tidy/report-$(basename $SUBDIR)
done

# run clang-tidy for mocks
clang-tidy "${CLANG_TIDY_FLAGS[@]}" -header-filter='inc\/*.h' $BASEDIR/test/mocks/*.c 2>&1 | tee result/clang-tidy/report-mocks
