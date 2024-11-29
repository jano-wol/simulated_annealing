#!/bin/bash
set -ex
BUILD_TYPE="relwithdebinfo"
source "$(dirname "${0}")/../build/init.sh" ${BUILD_TYPE}
timestamp() {
    date +"%Y%m%d%H%M%S"
}
if [[ -z "$1" || ! " ${TARGETS} " =~ " $1 " ]]; then
    echo "'$1' is not a correct target. Targets=${TARGETS}."
    exit 1
fi
TS=$(timestamp)
TARGET="${1}-bin"
OUT_FOLDER="${BUILD_FOLDER}/bin/${TARGET}"
BINARY="${OUT_FOLDER}/${TARGET}"
OUT_NAME="${OUT_FOLDER}/profile_${TS}.out"

${SCRIPT_FOLDER}/configure.sh ${BUILD_TYPE}
${SCRIPT_FOLDER}/build.sh ${BUILD_TYPE}
valgrind --tool=callgrind --callgrind-out-file="${OUT_NAME}" "${BINARY}" profile
kcachegrind "${OUT_NAME}" >/dev/null 2>&1 &
