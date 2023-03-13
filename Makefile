ISO=boon.iso

SUBDIRS := bootloader kernel

BOOTLOADER_BIN = bootloader/bin/bootloader.bin
KERNEL_BIN = kernel/bin/kernel.bin
#KERNEL_BIN=bootloader/bin/sector2.bin

all: $(SUBDIRS) iso

clean: $(SUBDIRS)
	@rm -f $(ISO)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(ISO): $(SUBDIRS)
	@echo Generating $@...
	@dd if=/dev/zero of=$(ISO) bs=512 seek=0 count=18
	@dd if=$(BOOTLOADER_BIN) of=$(ISO) conv=notrunc bs=512 seek=0 count=1
	@dd if=$(KERNEL_BIN) of=$(ISO) conv=notrunc bs=512 seek=1

iso: $(ISO)
	@echo Built $(ISO).

run: $(ISO)
	@qemu-system-i386 -drive format=raw,file=$(ISO)

.PHONY: all iso clean run $(SUBDIRS)
