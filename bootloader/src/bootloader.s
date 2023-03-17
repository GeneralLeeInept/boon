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

	/* Enable 4GiB memory addressing */
	cli
	push %ds
	lgdt gdtinfo_unreal

 	/* Enable protected mode */
	mov %cr0, %eax
	or $1, %al
	mov %eax, %cr0

	/* Load flatdesc descriptor */
	mov $(flatdesc - gdt_unreal), %bx
	mov %bx, %ds

	/* Leave protected mode */
	and $0xfe, %al
	mov %eax, %cr0
	pop %ds
	sti

	movw $loading_msg, %si
	call print

	/* Loading kernel to 1M, load & copy sector by sector */
	mov $25, %cx
	xor %ax, %ax
	mov %ax, %es
	movl $0x100000, %edi

load_sector_loop:
	mov (lba), %ax
	inc %ax
	mov %ax, (lba)
	
	/* Load sector to transfer buffer */
	movb $0x42, %ah
	movw $disk_packet, %si
	int $0x13
	jc disk_error

	mov $0x0e2e, %ax
	int $0x10

	/* Copy to destination */
	push %ds
	mov $0x1000, %ax
	mov %ax, %ds
	movw $0x0000, %si
	push %cx
	movw $0x100, %cx

copy_sector_loop:
	mov %ds:(%si), %ax
	mov %ax, %es:(%edi)
	inc %edi
	inc %edi
	inc %si
	inc %si
	loop copy_sector_loop

	pop %cx
	pop %ds

	loop load_sector_loop

	mov $0x0e0d, %ax
	int $0x10
	mov $0x0e0a, %ax
	int $0x10

    movb $0x00, %ah
    movb $0x13, %al
    int $0x10

	/* Setup bootinfo for the kernel */
	mov $0x1000, %ax
	mov %ax, %es
	mov $0, %di
	mov $0x03, %ah
	int $0x10
	mov %dx, %es:(%di)

	/* Jump to start of kernel */
	cli
	lgdt gdtinfo_pm
	mov %cr0, %eax
	or $1, %al
	mov %eax, %cr0
	ljmp $0x8,$start_kernel

.code32
start_kernel:
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	movl $0x10000, %ebx	/* Pointer to bootinfo */
	movl $0x100000, %eax
	jmpl *%eax

.code16
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

loading_msg:
	.string "Loading BoonOS kernel."

disk_error_msg:
	.string "Disk error.\r\n"

.p2align 2
gdt_unreal:
	.quad 0	/* NULL desc */
flatdesc:
	.byte 0xff, 0xff, 0, 0, 0, 0b10010010, 0b11001111, 0 /* flatdesc - 4GiB */
gdt_unreal_end:

gdtinfo_unreal:
	.word gdt_unreal_end - gdt_unreal - 1
	.long gdt_unreal

.p2align 2
gdt_pm:
.nulldesc:
	.quad 0
.codedesc:
	.word 0xffff		/* Limit bits 0..15 */
	.word 0x0000		/* Base bits 0..15 */
	.byte 0x00			/* Base bits 16..23 */
	.byte 0b10011010	/* Access: P=1, DPL=00, S=1, E=1, DC=0, RW=1, A=0 */
	.byte 0b11001111	/* Low nibble: Limit bits 16..19; High nibble Flags: G=1, DB=1, L=0, Reserved=0 */
	.byte 0x00			/* Base bits 24..31 */
.datadesc:
	.word 0xffff
	.word 0x0000
	.byte 0x00
	.byte 0b10010010
	.byte 0b11001111
	.byte 0x00
gdt_pm_end:

gdtinfo_pm:
	.word gdt_pm_end - gdt_pm - 1
	.long gdt_pm

disk_packet:
	.byte 0x10			/* Size of packet */
	.byte 0x00			/* reserved (0) */
sectors:
	.word 0x0001		/* Number of blocks to transfer */
	.word 0x0000		/* Transfer buffer offset */
	.word 0x1000		/* Transfer buffer segment */
lba:
	.long 0x00000000	/* LBA to load */
