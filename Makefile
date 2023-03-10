ISO=boon.iso

SUBDIRS := bootloader

BOOTLOADER_BIN = bootloader/bin/bootloader.bin

all: $(SUBDIRS) iso

clean: $(SUBDIRS)
	@rm -f $(ISO)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(ISO): $(BOOTLOADER_BIN)
	@echo Generating $@...
	@dd if=$(BOOTLOADER_BIN) of=$(ISO) conv=notrunc bs=512 seek=0 count=1

iso: $(ISO)
	@echo Built $(ISO).

.PHONY: all clean $(SUBDIRS)
