 	.code16
	.org 0

	.text

	.global _start

_start:
	mov %cs, %ax
	mov %ax, %ds
	mov %ax, %ss
	movw $0x7c00, %sp
	cld

	xor %ax,%ax

	/* Set mode 3 (80 x 25 text mode) to clear screen */
	mov $3, %ax
	int $0x10

	movw $banner_msg, %si
	call print

	/* Enable unreal mode */
	mov $unreal_mode_msg, %si
	call print

	cli
	push %ds
	lgdt gdtinfo

	mov %cr0, %eax
	or $1, %al
	mov %eax, %cr0
	ljmp $0x8,$pmode

.code32
pmode:
	mov $0x10, %bx
	mov %bx, %ds
	and $0xfe, %al
	mov %eax, %cr0
	ljmp $0x0,$unreal

.code16
unreal:
	pop %ds
	sti

	movw $loading_msg, %si
	call print

	/* Loading kernel to 1M, load & copy sector by sector */
	/*mov $41, %cx*/
	mov $1, %cx
	mov $0x10, %ax
	mov %ax, %es
	movl $0x100000, %edi

load_sector_loop:
	mov (lba), %eax
	inc %eax
	mov %eax, (lba)
	
	/* Load sector to transfer buffer */
	movb $0x42, %ah
	movw $disk_packet, %si
	int $0x13
	jc disk_error

	mov $0x0e2e, %ax
	int $0x10

	/* Copy to destination */
	push %cx
	push %ds
	movw $0x200, %cx
	mov $0x1000, %ax
	mov %ax, %ds
	movw $0x0000, %si
	rep movsb
	pop %ds
	pop %cx

	loop load_sector_loop

	mov $0x0e0d, %ax
	int $0x10
	mov $0x0e0a, %ax
	int $0x10

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

	/* Jump to start of kernel */
	jmp stop
	ljmp $0x1000,$0x0000

stop:
	hlt
	jmp stop

print:
	xor %bx, %bx
	movb $0x0E, %ah

	lodsb
	cmpb $0, %al
	je .exit

	int $0x10
	jmp print

.exit:
	ret

disk_error:
	movw $disk_error_msg, %si
	call print
	jmp stop

banner_msg:
	.string "BoonOS Boot Loader v0.0.1\r\n"

unreal_mode_msg:
	.string "Entering unreal mode.\r\n"

loading_msg:
	.string "Loading BoonOS kernel."

disk_error_msg:
	.string "Disk error.\r\n"

.align 16
gdtinfo:
	.word gdt_end - gdt - 1
	.long gdt

.align 16
gdt:
	.quad 0
codedesc:
	.byte 0xff, 0xff, 0, 0, 0, 0b10011010, 0b00000000, 0
flatdesc:
	.byte 0xff, 0xff, 0, 0, 0, 0b10010010, 0b11001111, 0
gdt_end:

disk_packet:
	.byte 0x10			/* Size of packet */
	.byte 0x00			/* reserved (0) */
sectors:
	.word 0x0000		/* Number of blocks to transfer */
	.word 0x0000		/* offset */
	.word 0x1000		/* segment */
lba:
	.long 0x00000000	/* LBA to load */
