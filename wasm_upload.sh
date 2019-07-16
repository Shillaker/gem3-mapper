#!/bin/bash

set -e

THIS_DIR=$(dirname $(readlink -f $0))
FILE_PATH=${THIS_DIR}/wasm_bin/gem-mapper

FAASM_HOST=localhost
URL=http://${FAASM_HOST}:8002/f/gene/mapper

# Do the actual upload
echo "Uploading ${FILE_PATH} to ${URL}"
curl -X PUT ${URL} -T ${FILE_PATH}
