#!/bin/bash -i

# Description start kernel debug session
qemu-system-i386 -drive format=raw,file=boon.img -s -S &
gdb --nx kernel/bin/kernel.elf \
	-ex 'target remote localhost:1234' \
	-ex 'b kmain' \
	-ex 'c'

