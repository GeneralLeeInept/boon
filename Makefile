ISO=boon.iso

KERNEL_ELF = kernel/bin/kernel.elf

all: iso

clean:
	@make -C kernel clean
	@rm -rf isodir
	@rm -f $(ISO)

kernel:
	@make -C kernel all

iso: $(ISO)

$(KERNEL_ELF): kernel

$(ISO): $(KERNEL_ELF) boot/grub.cfg
	@mkdir -p isodir/boot/grub
	@cp boot/grub.cfg isodir/boot/grub/
	@cp kernel/bin/kernel.elf isodir/boot/
	@grub-mkrescue -o $(ISO) isodir

run: iso
	@qemu-system-i386 -cdrom $(ISO)

.PHONY: all clean kernel iso run
