#!/bin/sh

./scripts/openocd.sh -c "program build/zephyr/zephyr.bin verify reset exit 0x08000000"
