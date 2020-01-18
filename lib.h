#include <stdlib.h>
#include <stdint.h>

static char const *const builtin_files[] = {
    "io.kan"
};

static size_t const len_builtin_files = sizeof(builtin_files) / sizeof(char *);

int32_t num_builtin_files() {
    return len_builtin_files;
}

char const *const *get_builtin_files() {
    return builtin_files;
}

// TOKEN types
static char const *const token_strings[] = {
    "unknown token",
    "invalid char literal",
    "integer",
    "identifier",
    "let",
    "if",
    "else",
    "import",
    "def",
    "while",
    "new",
    "delete",
    "return",
    "type",
    "struct",
    "as",
    "extern",
    ";",
    ",",
    ":",
    ".",
    "...",
    "!",
    "!=",
    "=",
    "==",
    "+",
    "-",
    "*",
    "/",
    "%",
    "<",
    "<=",
    ">",
    ">=",
    "&",
    "&&",
    "|",
    "||",
    "(",
    ")",
    "{",
    "}",
    "string",
    "char",
    "EOF"
};

static size_t const len_token_strings = sizeof(token_strings) / sizeof(char *);

// BINARY types
static char const *const binary_type_ops[] = {
    "error",
    "+",
    "-",
    "*",
    "/",
    "%",
    "<",
    "<=",
    ">",
    ">=",
    "&&",
    "||",
    "==",
    "!="
};

static size_t const len_binary_strings = sizeof(binary_type_ops) / sizeof(char *);

// UNARY types
static char const *const unary_type_ops[] = {
    "error",
    "&",
    "*",
    "!",
    "-"
};

static size_t const len_unary_strings = sizeof(unary_type_ops) / sizeof(char *);

// EXPR types
static char const *const expr_type_strings[] = {
    "error",
    "int",
    "string",
    "char",
    "binary expression",
    "unary expression",
    "identifier",
    "assignment",
    "function call",
    "struct initialization",
    "access expression",
    "cast expression",
    "new expression"
};

static size_t const len_expr_strings = sizeof(expr_type_strings) / sizeof(char *);

// ERROR types
static char const *const error_texts[] = {
    NULL,
    "Internal error: '%s'",
    // file errors
    "Path was empty",
    "Could not open file: '%s'",
    "Could not allocate buffer",
    "Could not read file: '%s'",
    // parse errors
    "Unexpected end of file",
    "Expected '%s', but got '%s'",
    "Unknown symbol: '%.*s'",
    "Char literal with length %1$d: '%.*s'",
    "Could not parse statement: \n%.*s",
    "Illegal type identifier '%.*s': %s",
    // type errors
    "Duplicate definition of '%.*s'",
    "Identifier '%.*s' is not in scope",
    "Type '%s' cannot be accessed with '%s' operator",
    "First element of access chain should be Identifier, but was '%s'"
};

static size_t const len_err_strings = sizeof(error_texts) / sizeof(char *);

