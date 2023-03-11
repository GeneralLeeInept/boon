# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

BOOTLOADER_BIN=$(BIN_DIR)/bootloader.bin

AS=gcc
ASFLAGS=
LD=ld
LDFLAGS=

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

SRCS=$(SRC_DIR)/bootloader.s

OBJS=$(patsubst $(SRC_DIR)/%.s,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all clean run bootloader

all: bootloader
run: bootloader

bootloader: $(BOOTLOADER_BIN)
	@echo Built $(BOOTLOADER_BIN).

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
	@$(LD) -o $@ $^ -Ttext 0x7C00 --oformat=binary
