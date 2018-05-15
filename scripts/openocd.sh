#!/bin/sh

set -e
BOARD_DIR=$(grep "^BOARD_DIR:" build/CMakeCache.txt | sed -e 's/.*boards/boards/')
openocd -f ${BOARD_DIR}/support/openocd.cfg "$@"
