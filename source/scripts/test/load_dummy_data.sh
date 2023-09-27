#!/bin/bash
set -ex
${BUILD_FOLDER}/bin/data_loader_eval
echo "data_loader_eval ready"
${BUILD_FOLDER}/bin/data_loader_move_candidate
echo "data_loader_move_canidate ready"
echo "load_dummy_data test OK"

