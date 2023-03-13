IMG=boon.img

SUBDIRS := bootloader kernel

BOOTLOADER_BIN = bootloader/bin/bootloader.bin
KERNEL_BIN = kernel/bin/kernel.bin

all: $(SUBDIRS) img

clean: $(SUBDIRS)
	@rm -f $(img)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(IMG): $(SUBDIRS)
	@echo Generating $@...
	@dd if=/dev/zero of=$@ bs=512 seek=0 count=18
	@dd if=$(BOOTLOADER_BIN) of=$@ conv=notrunc bs=512 seek=0 count=1
	@dd if=$(KERNEL_BIN) of=$@ conv=notrunc bs=512 seek=1

img: $(IMG)
	@echo Built $(IMG).

run: $(IMG)
	@qemu-system-i386 -drive format=raw,file=$(IMG)

.PHONY: all img clean run $(SUBDIRS)
