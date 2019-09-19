SHELL := /bin/bash

BIN_NAME = compiler
K_FILES = main.kan \
		  lexer.kan \
		  map.kan \
		  std.kan \
		  parser.kan \
		  ast.kan \
		  mod.kan \
		  ident.kan \
		  record.kan \
		  func.kan \
		  param.kan \
		  precedence.kan \
		  vec.kan \
		  ptrvec.kan \
		  span.kan \
		  file.kan \
		  path.kan \
		  num.kan \
		  source.kan \
		  reader.kan \
		  tyid.kan
C_FILES = lib.c \
		  lib.h \
		  token_types.h \
		  expr_types.h \
		  stmt_types.h \
		  error_code.h \
		  std.h
BUILD_FOLDER = build
START_FOLDER = $(shell pwd)
NEW_C_FILES = $(addprefix $(START_FOLDER)/, $(C_FILES))
KANTAN = kantan

$(BIN_NAME): $(K_FILES) $(C_FILES)
	mkdir $(BUILD_FOLDER) ;
	for file in $(K_FILES) ; do \
		gpp $$file -C -o $(BUILD_FOLDER)/$$file ; \
	done
	pushd $(BUILD_FOLDER) ; \
	if $(KANTAN) $(K_FILES) -o compiler.o ; then \
		gcc -Wall compiler.o $(NEW_C_FILES) -o $(BIN_NAME) ; \
		rm compiler.o ; \
		mv $(BIN_NAME) $(START_FOLDER) ; \
		popd ; \
		rm -r $(BUILD_FOLDER) ; \
	else \
		pwd ; \
		popd ; \
		rm -r $(BUILD_FOLDER) ; \
	fi

clean: $(BIN_NAME)
	rm $(BIN_NAME)
