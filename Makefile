BIN_NAME = compiler
K_FILES = $(shell find src -name "*.kan")

START_FOLDER := $(shell pwd)

CC ?= gcc
CPPC ?= g++
LLVM_PATH ?= $(HOME)/Downloads/llvm/llvm-10.0.0.src/build
KANTAN_STABLE ?= /usr/local/bin/kantan

KANTAN_STABLE_FLAGS := -g
C_FILES := lib.c posixlink.c
C_OBJS := $(C_FILES:.c=.o)

LLVM_CONFIG = $(LLVM_PATH)/bin/llvm-config
LLVM_LIB_NAMES = x86codegen webassemblycodegen passes

LLVM_C_FLAGS = $(shell $(LLVM_CONFIG) --cflags)
LLVM_LD_FLAGS = $(shell $(LLVM_CONFIG) --ldflags)
LLVM_LIBS = $(shell $(LLVM_CONFIG) --libs $(LLVM_LIB_NAMES))
LLVM_SYS_LIBS = $(shell $(LLVM_CONFIG) --system-libs)

LD_FLAGS = $(LLVM_LD_FLAGS) -fdata-sections -ffunction-sections
LIBS = $(LLVM_LIBS) $(LLVM_SYS_LIBS) -Wl,--gc-sections
C_FLAGS := -O3 -Wall -Wextra -pedantic -Werror

KANTAN_KANTAN_MEMCHECK = valgrind --leak-check=full --suppressions=$(START_FOLDER)/suppress-llvm-errors.supp $(START_FOLDER)/compiler
KANTAN_KANTAN_MASSIF = valgrind --tool=massif --massif-out-file=../massif.out $(START_FOLDER)/compiler
KANTAN_KANTAN = $(START_FOLDER)/$(BIN_NAME) -g -O3

$(BIN_NAME) : $(K_FILES) $(C_OBJS)
	$(KANTAN_STABLE) $(KANTAN_STABLE_FLAGS) $(K_FILES) -o $(BIN_NAME).o
	g++ $(LD_FLAGS) $(C_OBJS) $(BIN_NAME).o -o $(BIN_NAME) $(LIBS)

self : $(BIN_NAME) $(K_FILES) $(C_OBJS)
	$(KANTAN_KANTAN) $(K_FILES) -o out.o
	g++ $(LD_FLAGS) -o self out.o $(C_FILES:.c=.o) $(LIBS)
	rm out.o

$(C_OBJS) : $(C_FILES)
	$(CC) $(C_FLAGS) $(C_FILES) -c

.PHONY: clean
clean :
	rm $(BIN_NAME)
	rm self
	rm $(C_OBJS)
