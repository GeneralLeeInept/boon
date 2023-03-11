ISO=boon.iso

SUBDIRS := bootloader kernel

BOOTLOADER_BIN = bootloader/bin/bootloader.bin

all: $(SUBDIRS) iso

clean: $(SUBDIRS)
	@rm -f $(ISO)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(ISO): $(SUBDIRS)
	@echo Generating $@...
	@dd if=$(BOOTLOADER_BIN) of=$(ISO) conv=notrunc bs=512 seek=0 count=1

iso: $(ISO)
	@echo Built $(ISO).

run: $(ISO)
	@qemu-system-i386 -drive format=raw,file=$(ISO)

.PHONY: all clean run $(SUBDIRS)
