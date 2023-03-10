# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

AS=as
ASFLAGS=
LD=ld
LDFLAGS=

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

BOOTLOADER_SRCS=$(SRC_DIR)/bootloader.s

BOOTLOADER_OBJS=$(patsubst $(SRC_DIR)/%.s,$(OBJ_DIR)/%.o,$(BOOTLOADER_SRCS))

BOOTLOADER_BIN=$(BIN_DIR)/bootloader.bin
ISO=boon.iso

.PHONY: all dirs bootloader iso clean

all: dirs iso

dirs:
	@echo Creating output directories...
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BIN_DIR)
	@rm -f $(ISO)

$(BOOTLOADER_OBJS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	@echo Assembling $<...
	@$(AS) -o $@ -c $< $(ASFLAGS)

$(BOOTLOADER_BIN): $(BOOTLOADER_OBJS)
	@echo Linking $@...
	@$(LD) -o $@ $^ -Ttext 0x7C00 --oformat=binary

bootloader: $(BOOTLOADER_BIN)
	@echo Built $@.

$(ISO): $(BOOTLOADER_BIN)
	@echo Generating $@...
	@dd if=$(BOOTLOADER_BIN) of=$(ISO) conv=notrunc bs=512 seek=0 count=1

iso: $(ISO)
	@echo Built $@.