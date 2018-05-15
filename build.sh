#!/bin/sh

set -x

. ./scripts/env.sh "$0" "$@"

cd build
#ln -s `pwd`/cmake-cache ~/.cache/zephyr
make "$@"
ex=$?

# do something?

exit $ex
