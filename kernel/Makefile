# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

KERNEL_BIN=$(BIN_DIR)/kernel.bin

AS=as
ASFLAGS=
CC=gcc
CFLAGS=-fno-asynchronous-unwind-tables
LD=ld
LDFLAGS=

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

C_SRCS=\
	$(SRC_DIR)/kernel.c
	
S_SRCS=\
	$(SRC_DIR)/start.s


C_OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))
S_OBJS=$(patsubst $(SRC_DIR)/%.s,$(OBJ_DIR)/%.o,$(S_SRCS))

.PHONY: all clean kernel

all: $(KERNEL_BIN)
	@echo Built $^.

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

$(KERNEL_BIN): $(S_OBJS) $(C_OBJS)
	@echo Linking $<...
	@mkdir -p $(@D)
	@$(LD) -o $@ $^ -Tlink.ld

kernel: $(KERNEL_BIN)