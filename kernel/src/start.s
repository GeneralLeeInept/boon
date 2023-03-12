.section .text.start
.global _start
.type _start,@function
_start:
	/*
	The bootloader has loaded us into 32-bit protected mode on a x86
	machine. Interrupts are disabled. Paging is disabled. The processor
	state is as defined in the multiboot standard. The kernel has full
	control of the CPU. The kernel can only make use of hardware features
	and any code it provides as part of itself. There's no printf
	function, unless the kernel provides its own <stdio.h> header and a
	printf implementation. There are no security restrictions, no
	safeguards, no debugging mechanisms, only what the kernel provides
	itself. It has absolute and complete power over the
	machine.
	*/

    /* Setup stack */
    movl $stack_top, %esp
    call kmain

    /* TODO: Load GDT */

    /* TODO: Load IDT? */

    /* TODO: Enable paging? */

    call kmain

    cli
1:  hlt
    jmp 1b


.section .bss
.align 16
stack_base:
    .skip 0x4000    /* 16KiB stack */
stack_top:
