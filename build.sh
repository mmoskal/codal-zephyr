#!/bin/sh

set -ex

. ./scripts/env.sh "$0" "$@"

cd build
make -j10
cd ..
