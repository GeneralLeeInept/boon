# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

KERNEL_ELF=$(BIN_DIR)/kernel.elf
KERNEL_BIN=$(BIN_DIR)/kernel.bin

TARGET?=i686-elf
AS=$(TARGET)-gcc
ASFLAGS=-gdwarf
CC=$(TARGET)-gcc
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -gdwarf
CPP=$(TARGET)-g++
CPPFLAGS=-std=c++20 -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -Wno-unused-function -gdwarf
LD=$(TARGET)-gcc
LDFLAGS=-ffreestanding -nostdlib -lgcc
OBJCOPY=$(TARGET)-objcopy

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

C_SRCS=$(wildcard $(SRC_DIR)/*.c)
C_OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))
	
CPP_SRCS=$(wildcard $(SRC_DIR)/*.cpp)
CPP_OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SRCS))

S_SRCS=$(wildcard $(SRC_DIR)/*.s)
S_OBJS=$(patsubst $(SRC_DIR)/%.s,$(OBJ_DIR)/%.o,$(S_SRCS))

CRTI_OBJ=$(OBJ_DIR)/crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ=$(OBJ_DIR)/crtn.o

CRT_OBJS=$(CRTI_OBJ) $(CRBEGIN_OBJ) $(CRTEND_OBJ) $(CRTN_OBJ)
LINK_OBJS=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(filter-out $(CRTI_OBJ) $(CRTN_OBJ), $(S_OBJS)) $(C_OBJS) $(CPP_OBJS) $(CRTEND_OBJ) $(CRTN_OBJ)

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

$(CPP_OBJS): $(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@echo Compiling $<...
	@mkdir -p $(@D)
	@$(CPP) -c $< -o $@ $(CPPFLAGS)

$(S_OBJS): $(OBJ_DIR)%.o: $(SRC_DIR)%.s
	@echo Assemblng $<...
	@mkdir -p $(@D)
	@$(AS) -c $< -o $@ $(ASFLAGS)

$(KERNEL_ELF): $(LINK_OBJS)
	@echo Linking $<...
	@mkdir -p $(@D)
	$(LD) -o $@ $^ $(LDFLAGS) -Tlink.ld -Wl,-Map,$(@:.elf=.map)

$(KERNEL_BIN): $(KERNEL_ELF)
	@echo Making $<...
	@mkdir -p $(@D)
	@$(OBJCOPY) -O binary $< $@

kernel: $(KERNEL_BIN)
	@echo Built $(KERNEL_BIN).
