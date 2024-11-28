#!/bin/bash
set -ex
source "$(dirname "${0}")/../build/init.sh"
TARGET="${2}-bin"
OUT_FOLDER="${BUILD_FOLDER}/bin/${TARGET}"
BINARY="${OUT_FOLDER}/${TARGET}"
valgrind --leak-check=full --track-origins=yes "${BINARY}" bench

