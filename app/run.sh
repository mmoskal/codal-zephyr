#!/bin/sh

set -e
cd build
make -j10
openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg -c "program zephyr/zephyr.bin verify reset exit 0x08000000"
