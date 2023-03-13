# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

BOOTLOADER_ELF=$(BIN_DIR)/bootloader.elf
BOOTLOADER_BIN=$(BIN_DIR)/bootloader.bin

TARGET?=i686-elf
AS=$(TARGET)-gcc
ASFLAGS=-m32 -gdwarf
LD=$(TARGET)-ld
LDFLAGS=-melf_i386
OBJCOPY=$(TARGET)-objcopy

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

SRCS=$(SRC_DIR)/bootloader.s 

OBJS=$(patsubst $(SRC_DIR)/%.s,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all clean run bootloader sector2

all: bootloader sector2
run: all

bootloader: $(BOOTLOADER_BIN)
	@echo Built $(BOOTLOADER_BIN).

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BIN_DIR)

$(OBJS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	@echo Assembling $<...
	@mkdir -p $(@D)
	@$(AS) -o $@ -c $< $(ASFLAGS)

$(BOOTLOADER_ELF): $(OBJS)
	@echo Linking $@...
	@mkdir -p $(@D)
	@$(LD) -o $@ $^ $(LDFLAGS) -Tlink.ld 

$(BOOTLOADER_BIN): $(BOOTLOADER_ELF)
	@echo Making $@...
	@mkdir -p $(@D)
	@$(OBJCOPY) -O binary $< $@
