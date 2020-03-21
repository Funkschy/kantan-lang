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
    "float",
    "null",
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
    "sizeof",
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
    "int literal",
    "float literal",
    "null literal",
    "string literal",
    "char literal",
    "binary expression",
    "unary expression",
    "identifier",
    "assignment",
    "function call",
    "struct initialization",
    "access expression",
    "cast expression",
    "new expression",
    "sizeof expression"
};

static size_t const len_expr_strings = sizeof(expr_type_strings) / sizeof(char *);

// Type types
static char const *const type_type_strings[] = {
    "error",
    "pointer",
    "bool",
    "i32",
    "f32",
    "char",
    "void",
    "string",
    "function",
    "module",
    "struct"
};

static size_t const len_type_strings = sizeof(type_type_strings) / sizeof(char *);

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
    "Incomplete type '%.*s'",
    "Invalid operator for types '%s' and '%s'",
    "Pointers may only be compared or subtracted",
    "Cannot take address of rvalue",
    "Trying to dereference non pointer type '%s'",
    "Unary operator '%s' not defined for '%s'",
    "Return statements may not be used outside of functions",
    "Wrong return type for function. Expected '%s', but got '%s'",
    "Missing return statement",
    "Illegal use of declared type",
    "Type '%s' is not callable",
    "Wrong number of arguments, expected '%d', but got '%d'",
    "Wrong argument type. Expected '%s', but got '%s'",
    "Expected the name of a struct, but got '%s'",
    "Could not infer type",
    "'%s' cannot be casted into '%s'",
    "Invalid type for condition, expected 'bool', but got '%s'",
    "Trying to delete non pointer type: '%s'"
};

static size_t const len_err_strings = sizeof(error_texts) / sizeof(char *);

