#!/bin/sh

set -ex

. ./scripts/env.sh "$0" "$@"

rm -rf build
mkdir build
cd build
cmake -DBOARD=nucleo_f411re ../app
