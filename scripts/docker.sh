#!/bin/sh

export MSYS_NO_PATHCONV=1
set -x
docker run --rm -it --mount "type=bind,src=`pwd -W`,dst=/zephyr" -w /zephyr pext/zephyr /bin/bash "$@"