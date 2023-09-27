#!/bin/bash
set -ex
if [[ $1 != "debug" ]] && [[ $1 != "release" ]]; then
    echo "First command line argument should be debug or release"
    exit 1
fi	
CMAKE_BUILD_TYPE=$1
CMAKE_BUILD_ARCH=default
if [[ $2 == "avx2" ]]; then
    CMAKE_BUILD_ARCH="avx2"
fi	
SCRIPT_FOLDER=$(dirname "${0}") 
WORKSPACE_FOLDER=$(readlink -e "${SCRIPT_FOLDER}/../../")
BUILD_FOLDER="${WORKSPACE_FOLDER}/build/${CMAKE_BUILD_TYPE}"
BUILD_DATA_FOLDER="${WORKSPACE_FOLDER}/build/${CMAKE_BUILD_TYPE}/data"
BUILD_TEST_DATA_FOLDER="${WORKSPACE_FOLDER}/build/${CMAKE_BUILD_TYPE}/test/data"
SOURCE_FOLDER="${WORKSPACE_FOLDER}/source"
PYTHON_FOLDER="${SOURCE_FOLDER}/python"
CMAKE_CXX_COMPILER=g++
CMAKE_MAKE_PROGRAM=ninja
