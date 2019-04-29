#!/bin/bash

set -e

EMCC_WASM_BACKEND=1

FAASM_ROOT=/usr/local/code/faasm
EMCC_ROOT=${FAASM_ROOT}/pyodide/emsdk/emsdk/upstream/latest
EMCC_BIN=${EMCC_ROOT}/bin

WASM_TRIPLE=wasm32-unknown-wasm
WASM_BUILD=wasm32
WASM_SYSROOT=${EMCC_ROOT}/sysroot

export CFLAGS="--sysroot=${WASM_SYSROOT}"
export CPPFLAGS="${CFLAGS} -DHAVE_UNSETENV=1 -DHAVE_PUTENV=1 -DHAVE_TIMEGM=1 -DHAVE_FORK=1"

export CC="${EMCC_BIN}/wasm32-clang ${CFLAGS}"
export CXX=${EMCC_BIN}/wasm32-clang++
export AR=${EMCC_BIN}/llvm-ar
export RANLIB=${EMCC_BIN}/llvm-ranlib
export SYSROOT=${EMCC_ROOT}/sysroot
export LDSHARED=${EMCC_BIN}/wasm-ld

./configure --enable-lto=no --enable-cuda=no --host=${WASM_TRIPLE} --build=${WASM_BUILD}

make


