#!/bin/bash

set -e

# -----------------------------
# To give Faasm access to data directory locally we can create 
# a symlink to the real directory.
# -----------------------------

THIS_DIR=$(dirname $(readlink -f $0))
RUNTIME_ROOT=/usr/local/faasm/runtime_root

# Create the fake path at the runtime root
RUNTIME_PATH=${RUNTIME_ROOT}/${THIS_DIR}
mkdir -p ${RUNTIME_PATH}

pushd ${RUNTIME_PATH} > /dev/null

# Remove symlink if it already exists
rm -f data

# Create the symlink
ln -s ${THIS_DIR}/data data

popd > /dev/null

