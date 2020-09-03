SHELL := /bin/bash

BIN_NAME = compiler
K_FILES = ast.kan \
		  codegen.kan \
		  config.kan \
		  dbg.kan \
		  error.kan \
		  file.kan \
		  func.kan \
		  ident.kan \
		  lexer.kan \
		  llvm-bindings.kan \
		  main.kan \
		  map.kan \
		  mir.kan \
		  mirc.kan \
		  mod.kan \
		  modsort.kan \
		  name.kan \
		  nt.kan \
		  opt.kan \
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
		  symbol.kan \
		  target.kan \
		  transform.kan \
		  ty.kan \
		  tychk/tychk.kan \
		  tychk/lookup.kan \
		  tychk/waiting.kan \
		  tyid.kan \
		  vec.kan \
		  vmap.kan
PREPROCCESSOR_FILES = parser.kan transform.kan

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

$(BIN_NAME) : $(K_FILES) $(C_FILES)
	mkdir -p $(BUILD_FOLDER) ;
	for file in $(K_FILES) ; do \
		cp -r --parents $$file $(BUILD_FOLDER)/; \
	done
	for file in $(PREPROCCESSOR_FILES) ; do \
		gpp $$file -C -o $(BUILD_FOLDER)/$$file ; \
	done
	pushd $(BUILD_FOLDER) ; \
	if $(KANTAN_STABLE) $(K_FILES) -o out.o; then \
		for file in $(C_SRC_FILES) ; do \
			gcc -Wall -c ../$$file -o $(addsuffix .o, $$file); \
		done; \
		g++ $(LLVM_LD_FLAGS) -o $(BIN_NAME) out.o $(C_OBJ_FILES) $(LLVM_LIBS) $(LLVM_SYS_LIBS); \
		mv $(BIN_NAME) $(START_FOLDER) ; \
		popd ; \
	else \
		pwd ; \
		popd ; \
	fi

self : $(BIN_NAME) $(K_FILES) $(C_FILES)
	mkdir -p $(BUILD_FOLDER) ;
	for file in $(K_FILES) ; do \
		gpp $$file -C -o $(BUILD_FOLDER)/$$file ; \
	done
	pushd $(BUILD_FOLDER) ; \
	$(KANTAN_KANTAN) $(K_FILES) -o out.o ; \
	for file in $(C_SRC_FILES) ; do \
		gcc -Wall -c ../$$file -o $(addsuffix .o, $$file); \
	done; \
	g++ $(LLVM_LD_FLAGS) -o self-hosted out.o $(C_OBJ_FILES) $(LLVM_LIBS) $(LLVM_SYS_LIBS); \
	mv self-hosted $(START_FOLDER)/self-hosted ; \
	popd ; \

clean :
	rm $(BIN_NAME) ; \
	rm -r $(BUILD_FOLDER)
