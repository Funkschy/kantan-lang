#include <stdlib.h>

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
    ")",
    "string",
    "EOF"
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

static char const *const error_texts[] = {
    NULL,
    // file errors
    "Path was empty",
    "Could not open file: '%s'",
    "Could not allocate buffer",
    "Could not read file: '%s'",
    // parse errors
    "Unexpected end of file",
    "Expected '%s', but got '%s'",
    "Unknown symbol: '%.*s'",
    "Could not parse statement: '%.*s'"
};

static size_t const len_err_strings = sizeof(error_texts) / sizeof(char *);

