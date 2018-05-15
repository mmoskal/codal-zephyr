#!/bin/sh

set -ex

. ./scripts/env.sh "$0" "$@"

rm -rf build
mkdir build
mkdir -p $XDG_CACHE_HOME
cd build
cmake -DBOARD=nucleo_f411re ../app
