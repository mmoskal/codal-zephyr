#!/bin/sh

export MSYS_NO_PATHCONV=1
set -x
docker run --rm -it --mount "type=bind,src=`pwd -W`,dst=/zephyr" --cap-add SYS_PTRACE --security-opt seccomp:unconfined \
 -u build -w /zephyr pext/zephyr /bin/bash "$@"
