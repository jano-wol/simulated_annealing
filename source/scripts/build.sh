#!/bin/bash
set -ex
source "$(dirname "${0}")/build/init.sh"
source "$(dirname "${0}")/build/init_venv.sh"
cmake --build "${BUILD_FOLDER}" -- -j 4 -v
