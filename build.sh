#!/bin/sh

set -x

. ./scripts/env.sh "$0" "$@"

cd build
make "$@"
ex=$?

# do something?

exit $ex
