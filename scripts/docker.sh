#!/bin/sh

export MSYS_NO_PATHCONV=1
set -x
# --cap-add SYS_PTRACE --security-opt seccomp:unconfined
docker run --rm -it --mount "type=bind,src=`pwd -W`,dst=/zephyr" -u build -w /zephyr pext/zephyr /bin/bash "$@"
