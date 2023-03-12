# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

BOOTLOADER_BIN=$(BIN_DIR)/bootloader.bin

TARGET?=i686-elf
AS=$(TARGET)-gcc
ASFLAGS=-m32
LD=$(TARGET)-ld
LDFLAGS=-melf_i386

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

SRCS=$(SRC_DIR)/bootloader.s 

S2_SRCS=$(SRC_DIR)/test.s

OBJS=$(patsubst $(SRC_DIR)/%.s,$(OBJ_DIR)/%.o,$(SRCS))

S2_OBJS=$(patsubst $(SRC_DIR)/%.s,$(OBJ_DIR)/%.o,$(S2_SRCS))

.PHONY: all clean run bootloader sector2

all: bootloader sector2
run: all

bootloader: $(BOOTLOADER_BIN)
	@echo Built $(BOOTLOADER_BIN).

sector2: $(BIN_DIR)/sector2.bin
	@echo Built $(BIN_DIR)/sector2.bin.

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BIN_DIR)

$(OBJS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	@echo Assembling $<...
	@mkdir -p $(@D)
	@$(AS) -o $@ -c $< $(ASFLAGS)

$(BOOTLOADER_BIN): $(OBJS)
	@echo Linking $@...
	@mkdir -p $(@D)
	@$(LD) -o $@ $^ $(LDFLAGS) -Tlink.ld --oformat=binary

$(BIN_DIR)/sector2.bin: $(S2_OBJS)
	@echo Linking $@...
	@mkdir -p $(@D)
	$(LD) -o $@ $^ $(LDFLAGS) -Ttext 0x0000 --oformat=binary
	
$(S2_OBJS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	@echo Assembling $(input)...
	@mkdir -p $(@D)
	@$(AS) -o $@ -c $< $(ASFLAGS)
