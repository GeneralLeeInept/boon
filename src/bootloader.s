 	.code16
	.org 0

	.text

	.global _start

_start:
	mov $3, %ax
	int $0x10

    movw $banner_msg, %si
	call print

	movw $loading_msg, %si
	call print

.end:
	hlt
	jmp .end

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

banner_msg:
	.string "BoonOS Boot Loader v0.0.1\r\n"

loading_msg:
    .string "Loading BoonOS kernel...\r\n"

/* MBR BOOT SIGNATURE */
	.org 0x1fe
	.word 0xAA55
