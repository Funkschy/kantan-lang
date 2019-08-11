#include <stdio.h>
#include <stdint.h>
#include <string.h>

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
    "/"
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
