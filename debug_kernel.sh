#!/bin/bash -i

# Description start kernel debug session
qemu-system-i386 -cdrom boon.iso -s -S &
gdb --nx kernel/bin/kernel.elf \
	-ex 'target remote localhost:1234' \
	-ex 'b _start' \
	-ex 'c'

