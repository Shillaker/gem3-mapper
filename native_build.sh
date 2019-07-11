#!/bin/bash

set -e

./configure --enable-cuda=no
make
