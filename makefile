# tool macros
CC := gcc
CCFLAGS := -Wall -Wextra
DBGFLAGS := -g
LIBS := -lpthread
CCOBJFLAGS := $(CCFLAGS) -c

# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src

# compile macros
TARGET_NAME := demo
ifeq ($(OS),Windows_NT)
    TARGET_NAME := $(addsuffix .exe,$(TARGET_NAME))
endif

TARGET := $(BIN_PATH)/$(TARGET_NAME)

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
DISTCLEAN_LIST := $(OBJ)
CLEAN_LIST := $(TARGET) \
    $(DISTCLEAN_LIST) \
    $(OBJ_PATH) \
    $(BIN_PATH)

# default rule
default: makedir all

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CCOBJFLAGS) -o $@ $<

# non-phony targets
$(TARGET): $(OBJ)
	$(CC) $(CCFLAGS) $(LIBS) -o $@ $(OBJ)

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -rf $(CLEAN_LIST)

.PHONY: help
help:
	@echo "help:"
	@echo "compile with: make all"
	@echo "run: ./bin/demo"

