#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define HASH_NUM 65599

static char const *const token_strings[] = {
    "error",
    "integer",
    "identifier",
    "let",
    "if",
    "import",
    ";",
    "=",
    "+",
    "-",
    "*",
    "/",
    "(",
    ")"
};

static size_t const len_token_strings = sizeof(token_strings) / sizeof(char *);

static char const *const binary_type_ops[] = {
    "error",
    "+",
    "-",
    "*",
    "/"
};

static size_t const len_binary_strings = sizeof(binary_type_ops) / sizeof(char *);

int32_t hash(char const *key) {
    int32_t h = 0;
    for (char c = *key; c != '\0'; c++) {
        h = h * HASH_NUM + c;
    }
    return h;
}

char const *const tok2str(int32_t type) {
    if (type > len_token_strings) {
        return "eof";
    }

    return token_strings[type];
}

char const *const binary2str(int32_t op) {
    if (op > len_binary_strings) {
        return "error";
    }

    return binary_type_ops[op];
}

char const *const read_file(char const *const path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file '%s'\n", path);
        exit(-1);
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = (size_t) ftell(file);
    rewind(file);

    char *buffer = malloc(file_size + 1);

    if (buffer == NULL) {
        fprintf(stderr, "Could not allocate enough memory to open file '%s'\n", path);
        exit(-2);
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);

    if (bytes_read < file_size) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(-3);
    }

    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

