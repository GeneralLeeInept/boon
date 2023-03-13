#!/bin/bash -i

# Description start bootloader debug session
qemu-system-i386 -drive format=raw,file=boon.img -s -S &
gdb --nx -ix gdb.txt bootloader/bin/bootloader.elf \
	-ex 'target remote localhost:1234' \
	-ex 'b _start' \
	-ex 'c'

