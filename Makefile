BIN_NAME = compiler
K_FILES = main.kan lexer.kan map.kan std.kan
C_FILES = lib.c
BUILD_FOLDER = build
START_FOLDER = $(shell pwd)
NEW_C_FILES = $(addprefix $(START_FOLDER)/, $(C_FILES))

$(BIN_NAME): $(K_FILES) $(C_FILES)
	mkdir $(BUILD_FOLDER) ; \
	for file in $(K_FILES) ; do \
		gpp $$file -C -o $(BUILD_FOLDER)/$$file ; \
	done
	cd $(BUILD_FOLDER) ; \
	if kantan $(K_FILES) -o compiler.o; then \
		gcc compiler.o $(NEW_C_FILES) -o $(BIN_NAME) ; \
		rm compiler.o ; \
		mv $(BIN_NAME) $(START_FOLDER) ; \
		cd $(START_FOLDER) ; \
		rm -r $(BUILD_FOLDER) ; \
	else \
		pwd ; \
		cd $(START_FOLDER) ; \
		rm -r $(BUILD_FOLDER) ; \
	fi

clean: $(BIN_NAME)
	rm $(BIN_NAME)
