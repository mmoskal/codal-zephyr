#!/bin/sh

arm-none-eabi-gdb --command=debug.gdb build/zephyr/zephyr.elf
