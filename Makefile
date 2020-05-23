SHELL := /bin/bash

BIN_NAME = compiler
K_FILES = ast.kan \
		  codegen.kan \
		  dbg.kan \
		  file.kan \
		  func.kan \
		  ident.kan \
		  lexer.kan \
		  llvm.kan \
		  main.kan \
		  map.kan \
		  mir.kan \
		  mirc.kan \
		  mod.kan \
		  nt.kan \
		  num.kan \
		  param.kan \
		  parser.kan \
		  path.kan \
		  precedence.kan \
		  ptrvec.kan \
		  reader.kan \
		  record.kan \
		  scope.kan \
		  source.kan \
		  span.kan \
		  std.kan \
		  str.kan \
		  transform.kan \
		  ty.kan \
		  tychk.kan \
		  tyid.kan \
		  vec.kan \
		  vmap.kan
C_SRC_FILES = lib.c modsort.c
C_FILES = $(C_SRC_FILES) \
		  lib.h \
		  token_types.h \
		  expr_types.h \
		  stmt_types.h \
		  error_code.h \
		  ty_types.h \
		  mir_types.h \
		  std.h
BUILD_FOLDER = build
START_FOLDER = $(shell pwd)
NEW_C_FILES = $(addprefix $(START_FOLDER)/, $(C_FILES))
C_OBJ_FILES = $(C_SRC_FILES:.c=.c.o)

LLVM_PATH = $(HOME)/Downloads/llvm/clang+llvm-10.0.0-x86_64-linux-gnu-ubuntu-18.04
LLVM_CONFIG = $(LLVM_PATH)/bin/llvm-config

LLVM_LIB_NAMES = core target analysis support x86codegen
CPP_LIBS = -lpthread -lncurses

LLVM_C_FLAGS = $(shell $(LLVM_CONFIG) --cflags)
LLVM_LD_FLAGS = $(shell $(LLVM_CONFIG) --ldflags)
LLVM_LIBS = $(shell $(LLVM_CONFIG) --libs $(LLVM_LIB_NAMES))

KANTAN_RUST = kantan
KANTAN_KANTAN = valgrind --leak-check=full $(START_FOLDER)/compiler
KANTAN_KANTAN_MASSIF = valgrind --tool=massif --massif-out-file=../massif.out $(START_FOLDER)/compiler

$(BIN_NAME) : $(K_FILES) $(C_FILES)
	mkdir $(BUILD_FOLDER) ;
	for file in $(K_FILES) ; do \
		gpp $$file -C -o $(BUILD_FOLDER)/$$file ; \
	done
	pushd $(BUILD_FOLDER) ; \
	if $(KANTAN_RUST) $(K_FILES) -o ktemp.o ; then \
		for file in $(C_SRC_FILES) ; do \
			gcc -Wall -c ../$$file -o $(addsuffix .o, $$file); \
		done; \
		g++ $(LLVM_LD_FLAGS) -o $(BIN_NAME) ktemp.o $(C_OBJ_FILES) $(LLVM_LIBS) $(CPP_LIBS); \
		mv $(BIN_NAME) $(START_FOLDER) ; \
		popd ; \
		rm -r $(BUILD_FOLDER) ; \
	else \
		pwd ; \
		popd ; \
		rm -r $(BUILD_FOLDER) ; \
	fi

self : $(BIN_NAME) $(K_FILES) $(C_FILES)
	mkdir $(BUILD_FOLDER) ;
	for file in $(K_FILES) ; do \
		gpp $$file -C -o $(BUILD_FOLDER)/$$file ; \
	done
	pushd $(BUILD_FOLDER) ; \
	$(KANTAN_KANTAN) $(K_FILES) ; \
	popd ; \
	rm -r $(BUILD_FOLDER) ; \

clean :
	rm $(BIN_NAME) ; \
	rm -r $(BUILD_FOLDER)
