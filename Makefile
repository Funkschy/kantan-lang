SHELL := /bin/bash

BIN_NAME = compiler
K_FILES = src/ast.kan \
		  src/codegen.kan \
		  src/config.kan \
		  src/dbg.kan \
		  src/error.kan \
		  src/escape.kan \
		  src/file.kan \
		  src/func.kan \
		  src/ident.kan \
		  src/lexer.kan \
		  src/llvm-bindings.kan \
		  src/main.kan \
		  src/map.kan \
		  src/mir.kan \
		  src/mirc.kan \
		  src/mod.kan \
		  src/modsort.kan \
		  src/name.kan \
		  src/nt.kan \
		  src/opt.kan \
		  src/path.kan \
		  src/precedence.kan \
		  src/ptrvec.kan \
		  src/reader.kan \
		  src/record.kan \
		  src/scope.kan \
		  src/source.kan \
		  src/span.kan \
		  src/std.kan \
		  src/str.kan \
		  src/symbol.kan \
		  src/target.kan \
		  src/ty.kan \
		  src/tychk/lookup.kan \
		  src/tychk/tychk.kan \
		  src/tychk/waiting.kan \
		  src/tyid.kan \
		  src/typaram.kan \
		  src/vec.kan \
		  src/vmap.kan

START_FOLDER := $(shell pwd)

LLVM_PATH ?= $(HOME)/Downloads/llvm/llvm-10.0.0.src/build
KANTAN_STABLE ?= $(START_FOLDER)/../kantan -g

BUILD_FOLDER := build
PREPROCCESSOR_FILES := src/parser.kan src/transform.kan
C_SRC_FILES := lib.c posixlink.c
C_FILES := $(C_SRC_FILES) lib.h

NEW_PREPROCESS_FILES = $(addprefix $(BUILD_FOLDER)/, $(PREPROCCESSOR_FILES))
NEW_K_FILES = $(addprefix $(BUILD_FOLDER)/, $(K_FILES))
C_OBJ_FILES = $(addprefix $(BUILD_FOLDER)/,$(C_SRC_FILES:.c=.c.o))

LLVM_CONFIG = $(LLVM_PATH)/bin/llvm-config
LLVM_LIB_NAMES = x86codegen webassemblycodegen passes

LLVM_C_FLAGS = $(shell $(LLVM_CONFIG) --cflags)
LLVM_LD_FLAGS = $(shell $(LLVM_CONFIG) --ldflags)
LLVM_LIBS = $(shell $(LLVM_CONFIG) --libs $(LLVM_LIB_NAMES))
LLVM_SYS_LIBS = $(shell $(LLVM_CONFIG) --system-libs)

LD_FLAGS = $(LLVM_LD_FLAGS) -fdata-sections -ffunction-sections
LIBS = $(LLVM_LIBS) $(LLVM_SYS_LIBS) -Wl,--gc-sections

KANTAN_KANTAN_MEMCHECK = valgrind --leak-check=full --suppressions=$(START_FOLDER)/suppress-llvm-errors.supp $(START_FOLDER)/compiler
KANTAN_KANTAN_MASSIF = valgrind --tool=massif --massif-out-file=../massif.out $(START_FOLDER)/compiler
KANTAN_KANTAN = $(START_FOLDER)/$(BIN_NAME) -g -O3

$(BIN_NAME) : $(NEW_PREPROCESS_FILES) $(NEW_K_FILES) $(C_OBJ_FILES)
	if $(KANTAN_STABLE) $(NEW_K_FILES) $(NEW_PREPROCESS_FILES) -o out.o; then \
		g++ $(LD_FLAGS) -o $(BIN_NAME) out.o $(C_OBJ_FILES) $(LIBS); \
		rm out.o ; \
	fi

self : $(BIN_NAME)
	$(KANTAN_KANTAN) $(NEW_K_FILES) $(NEW_PREPROCESS_FILES) -o out.o ; \
	g++ $(LD_FLAGS) -o self-hosted out.o $(C_OBJ_FILES) $(LIBS); \
	rm out.o ; \
	mv self-hosted $(START_FOLDER)/self

$(C_OBJ_FILES) : $(C_FILES)
	for file in $(C_SRC_FILES) ; do \
		gcc -O3 -Wall -c $$file -o $(BUILD_FOLDER)/$(addsuffix .o, $$file); \
	done

# This is needed because some Kantan files still need the C Preprocessor
$(NEW_PREPROCESS_FILES) $(NEW_K_FILES) : $(K_FILES) $(PREPROCCESSOR_FILES)
	mkdir -p $(BUILD_FOLDER) ;
	for file in $(K_FILES) ; do \
		cp -r --parents $$file $(BUILD_FOLDER)/; \
	done
	for file in $(PREPROCCESSOR_FILES) ; do \
		cp -r --parents $$file $(BUILD_FOLDER)/; \
		gpp $$file -C -o $(BUILD_FOLDER)/$$file ; \
	done

.PHONY: clean
clean :
	rm $(BIN_NAME) ; \
	rm -r $(BUILD_FOLDER)
