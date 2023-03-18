IMG=boon.img

SUBDIRS := bootloader kernel

BOOTLOADER_BIN = bootloader/bin/bootloader.bin
KERNEL_BIN = kernel/bin/kernel.bin

all: iso

clean: $(SUBDIRS)
	@rm -f $(img)
	@rm -rf isodir
	@rm -f $(iso)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(IMG): $(SUBDIRS)
	@echo Generating $@...
	@dd if=/dev/zero of=$@ bs=512 seek=0 count=2880
	@dd if=$(BOOTLOADER_BIN) of=$@ conv=notrunc bs=512 seek=0 count=1
	@dd if=$(KERNEL_BIN) of=$@ conv=notrunc bs=512 seek=1

img: $(IMG)
	@echo Built $(IMG).

iso: kernel
	@mkdir -p isodir/boot/grub
	@cp boot/grub.cfg isodir/boot/grub/
	@cp kernel/bin/kernel.elf isodir/boot/
	@grub-mkrescue -o boon.iso isodir

run: $(IMG)
	@qemu-system-i386 -cdrom boon.iso

.PHONY: all img clean run $(SUBDIRS) iso
