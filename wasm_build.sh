#!/bin/bash

set -e

EMCC_WASM_BACKEND=1

FAASM_ROOT=/usr/local/faasm
EMCC_ROOT=${FAASM_ROOT}/emsdk/upstream/latest
EMCC_BIN=${EMCC_ROOT}/bin

WASM_TRIPLE=wasm32-unknown-wasm
WASM_BUILD_TYPE=wasm32
WASM_SYSROOT=${EMCC_ROOT}/sysroot

export CFLAGS="--sysroot=${WASM_SYSROOT}  \
    -Xlinker --no-entry \
    -Xlinker --export=main \
    -Xlinker --max-memory=1073741824 \
    -Xlinker --stack-first \
    -Xlinker --threads \
    -Wl,-z,stack-size=4194304 -Wl, \
"

export CPPFLAGS="${CFLAGS} -DHAVE_UNSETENV=1 -DHAVE_PUTENV=1 -DHAVE_TIMEGM=1 -DHAVE_FORK=1"

export CC="${EMCC_BIN}/wasm32-clang ${CFLAGS}"
export CXX="${EMCC_BIN}/wasm32-clang++ ${CPPFLAGS}"
export AR=${EMCC_BIN}/llvm-ar
export RANLIB=${EMCC_BIN}/llvm-ranlib
export SYSROOT=${EMCC_ROOT}/sysroot
export LDSHARED="${EMCC_BIN}/wasm-ld"

export WASM_BUILD=1

./configure --enable-cuda=no --disable-lto --without-tests --host=${WASM_TRIPLE} --build=${WASM_BUILD_TYPE}

make
