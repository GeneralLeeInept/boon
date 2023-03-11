 	.code16
	.org 0

	.text

	.global _start

_start:
	/* Set mode 3 (80 x 25 text mode) to clear screen */
	mov $3, %ax
	int $0x10

	movw $banner_msg, %si
	call print

	movw $loading_msg, %si
	call print

	/* Load kernel to 0x10000 */
	movb $0x42, %ah
	movw $disk_packet, %si
	int $0x13
	jc disk_error

	jmp $0x1000,$0x0

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
	.string "Loading BoonOS kernel...\r\n"

disk_error_msg:
	.string "Disk error.\r\n"

disk_packet:
	.byte 0x10			/* Size of packet */
	.byte 0x00			/* reserved (0) */
	.word 0x0001		/* Number of blocks to transfer */
	.word 0x1000		/* Transfer buffer */
	.quad 0x00000000	/* (EDD-3.0, optional) 64-bit flat address of transfer buffer;
						   used if DWORD at 04h is FFFFh:FFFFh */

	/* MBR BOOT SIGNATURE */
	.fill 510-(.-_start)
	.word 0xAA55
