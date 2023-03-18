ISO=boon.iso

KERNEL_ELF = kernel/bin/kernel.elf

all: $(ISO)

clean:
	@make -C kernel clean
	@rm -rf isodir
	@rm -f $(ISO)

$(KERNEL_ELF): 
	@make -C kernel all

$(ISO): $(KERNEL_ELF) boot/grub.cfg
	@mkdir -p isodir/boot/grub
	@cp boot/grub.cfg isodir/boot/grub/
	@cp $(KERNEL_ELF) isodir/boot/
	@grub-mkrescue -o $(ISO) isodir

run: $(ISO)
	@qemu-system-i386 -cdrom $(ISO)

.PHONY: all clean run $(KERNEL_ELF)
