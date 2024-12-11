#!/bin/bash
set -ex
source "$(dirname "${0}")/../build/init.sh"
${SCRIPT_FOLDER}/configure.sh ${1}
${SCRIPT_FOLDER}/build.sh ${1}
for target in $TARGETS; do
    TARGET="${target}-bin"
    OUT_FOLDER="${BUILD_FOLDER}/bin/${TARGET}"
    BINARY="${OUT_FOLDER}/${TARGET}"
    valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all "${BINARY}" mem_check
done

