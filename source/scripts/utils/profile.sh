#!/bin/bash
set -ex
source "$(dirname "${0}")/../build/init.sh" relwithdebinfo
timestamp() {
    date +"%Y%m%d%H%M%S"
}
TS=$(timestamp)
TARGET="${2}-bin"
OUT_FOLDER="${BUILD_FOLDER}/bin/${TARGET}"
BINARY="${OUT_FOLDER}/${TARGET}"
OUT_NAME="${OUT_FOLDER}/profile_${TS}.out"
valgrind --tool=callgrind --callgrind-out-file="${OUT_NAME}" "${BINARY}" bench
kcachegrind "${OUT_NAME}" >/dev/null 2>&1 &
