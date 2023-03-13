# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

KERNEL_ELF=$(BIN_DIR)/kernel.elf
KERNEL_BIN=$(BIN_DIR)/kernel.bin

TARGET?=i686-elf
AS=$(TARGET)-gcc
ASFLAGS=-m32 -gdwarf
CC=$(TARGET)-gcc
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -gdwarf
LD=$(TARGET)-gcc
LDFLAGS=-ffreestanding -nostdlib -lgcc
OBJCOPY=$(TARGET)-objcopy

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

C_SRCS=\
	$(SRC_DIR)/kernel.c
	
S_SRCS=\
	$(SRC_DIR)/start.s


C_OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))
S_OBJS=$(patsubst $(SRC_DIR)/%.s,$(OBJ_DIR)/%.o,$(S_SRCS))

.PHONY: all clean kernel run

all: kernel

run: kernel

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BIN_DIR)

$(C_OBJS): $(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@echo Compiling $<...
	@mkdir -p $(@D)
	@$(CC) -c $< -o $@ $(CFLAGS)

$(S_OBJS): $(OBJ_DIR)%.o: $(SRC_DIR)%.s
	@echo Assemblng $<...
	@mkdir -p $(@D)
	@$(AS) -c $< -o $@ $(ASFLAGS)

$(KERNEL_ELF): $(S_OBJS) $(C_OBJS)
	@echo Linking $<...
	@mkdir -p $(@D)
	@$(LD) -o $@ $^ $(LDFLAGS) -Tlink.ld

$(KERNEL_BIN): $(KERNEL_ELF)
	@echo Making $<...
	@mkdir -p $(@D)
	@$(OBJCOPY) -O binary $< $@

kernel: $(KERNEL_BIN)
	@echo Built $(KERNEL_BIN).
