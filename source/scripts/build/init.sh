#!/bin/bash
set -ex
if [[ $1 != "debug" ]] && [[ $1 != "release" ]] && [[ $1 != "relwithdebinfo" ]]; then
    echo "First command line argument should be debug or release or relwithdebinfo"
    exit 1
fi	
CMAKE_BUILD_TYPE=$1
SCRIPT_FOLDER=$(readlink -f "$(dirname "${BASH_SOURCE[0]}")/..")
WORKSPACE_FOLDER=$(readlink -e "${SCRIPT_FOLDER}/../../")
BUILD_FOLDER="${WORKSPACE_FOLDER}/build/${CMAKE_BUILD_TYPE}"
SOURCE_FOLDER="${WORKSPACE_FOLDER}/source"
TARGETS_FOLDER="${SOURCE_FOLDER}/targets"
TARGETS=$(find "$TARGETS_FOLDER" -mindepth 1 -maxdepth 1 -type d -exec basename {} \; | tr '\n' ' ')
CMAKE_CXX_COMPILER=g++
CMAKE_MAKE_PROGRAM=ninja
