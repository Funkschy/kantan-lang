SHELL := /bin/bash

BIN_NAME = compiler
K_FILES = src/ast.kan \
		  src/codegen.kan \
		  src/config.kan \
		  src/dbg.kan \
		  src/error.kan \
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
		  src/tychk/tychk.kan \
		  src/tychk/lookup.kan \
		  src/tychk/waiting.kan \
		  src/tyid.kan \
		  src/vec.kan \
		  src/vmap.kan
PREPROCCESSOR_FILES = src/parser.kan src/transform.kan

C_SRC_FILES = lib.c posixlink.c
C_FILES = $(C_SRC_FILES) lib.h
BUILD_FOLDER = build
START_FOLDER = $(shell pwd)
NEW_C_FILES = $(addprefix $(START_FOLDER)/, $(C_FILES))
C_OBJ_FILES = $(C_SRC_FILES:.c=.c.o)

LLVM_PATH = $(HOME)/Downloads/llvm/llvm-10.0.0.src/build
LLVM_CONFIG = $(LLVM_PATH)/bin/llvm-config

LLVM_LIB_NAMES = core target analysis support x86codegen webassemblycodegen linker passes

LLVM_C_FLAGS = $(shell $(LLVM_CONFIG) --cflags)
LLVM_LD_FLAGS = $(shell $(LLVM_CONFIG) --ldflags)
LLVM_LIBS = $(shell $(LLVM_CONFIG) --libs $(LLVM_LIB_NAMES))
LLVM_SYS_LIBS = $(shell $(LLVM_CONFIG) --system-libs)

KANTAN_STABLE = $(START_FOLDER)/../kantan -g
KANTAN_KANTAN_MEMCHECK = valgrind --leak-check=full --suppressions=$(START_FOLDER)/suppress-llvm-errors.supp $(START_FOLDER)/compiler
KANTAN_KANTAN_MASSIF = valgrind --tool=massif --massif-out-file=../massif.out $(START_FOLDER)/compiler
KANTAN_KANTAN = $(KANTAN_KANTAN_MEMCHECK) -g

$(BIN_NAME) : preprocess cdeps
	pushd $(BUILD_FOLDER) ; \
	if $(KANTAN_STABLE) $(K_FILES) $(PREPROCCESSOR_FILES) -o out.o; then \
		g++ $(LLVM_LD_FLAGS) -o $(BIN_NAME) out.o $(C_OBJ_FILES) $(LLVM_LIBS) $(LLVM_SYS_LIBS); \
		mv $(BIN_NAME) $(START_FOLDER) ; \
	fi ; \
	popd ;

.PHONY: self
self : $(BIN_NAME)
	pushd $(BUILD_FOLDER) ; \
	$(KANTAN_KANTAN) $(K_FILES) $(PREPROCCESSOR_FILES) -o out.o ; \
	g++ $(LLVM_LD_FLAGS) -o self-hosted out.o $(C_OBJ_FILES) $(LLVM_LIBS) $(LLVM_SYS_LIBS); \
	mv self-hosted $(START_FOLDER)/self-hosted ; \
	popd ;

.PHONY: cdeps
cdeps : $(C_FILES)
	pushd $(BUILD_FOLDER) ; \
	for file in $(C_SRC_FILES) ; do \
		gcc -Wall -c ../$$file -o $(addsuffix .o, $$file); \
	done; \
	popd

.PHONY: preprocess
preprocess : $(K_FILES) $(PREPROCCESSOR_FILES)
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
