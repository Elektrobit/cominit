#!/bin/bash
# SPDX-License-Identifier: MIT
#
# Table of contents generation and lint script for merge request
# branches.
#
# Usage: ./ci/doc-toc.sh
#
# This will update the Table of Contents automatically. If git detects
# changes afterwards, the script will return 1, otherwise 0.
#
# Set DOCTOC_CMD env variable if your doctoc binary/script is not in
# your path or if you want to run using npx.
#

set -euo pipefail

DOCTOC_CMD=${DOCTOC_CMD:-doctoc}
CMDPATH=$(cd "$(dirname "$0")" && pwd)
BASEDIR=${CMDPATH%/*}

# Update ToC.
${DOCTOC_CMD} --github "${BASEDIR}/README.md"
${DOCTOC_CMD} --github "${BASEDIR}/doc/measured_boot/tpm-library.md"

# Check if the Update changed anything and report via exit status (useful for
# the pipeline check). This will be false positive if there are other unstaged
# changes in the updated files but that does not matter for development and should
# not happen in the pipeline.
git diff --quiet
