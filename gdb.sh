#!/bin/sh

arm-none-eabi-gdb --command=scripts/debug.gdb build/zephyr/zephyr.elf
