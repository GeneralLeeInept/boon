# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

AS=clang
ASFLAGS=-target i386

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

SRCS=$(SRC_DIR)/test.s

OBJS=$(patsubst $(SRC_DIR)/%.s,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all test clean

all: test
	@echo Done.


test: $(OBJ_DIR) $(OBJS)


$(OBJS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	@echo Assembling $<...
	@$(AS) $(ASFLAGS) -o $@ -c $<


$(OBJ_DIR):
	@echo Creating directory $@...
	@mkdir -p $@


clean:
	@rm -f $(OBJS)
	@rm -rf $(OBJ_DIR)
