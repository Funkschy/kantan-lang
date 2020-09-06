#include <stdlib.h>
#include <stdint.h>

static char const *const builtin_files[] = {
    "io.kan"
};

static size_t const len_builtin_files = sizeof(builtin_files) / sizeof(char *);

size_t num_builtin_files() {
    return len_builtin_files;
}

char const *const *get_builtin_files() {
    return builtin_files;
}

// TOKEN types
static char const *const token_strings[] = {
    "unknown token",
    "invalid char literal",
    "invalid escape sequence",
    "invalid identifier",
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
    "enum",
    "union",
    "as",
    "extern",
    "export",
    "delegate",
    "sizeof",
    "for",
    "undefined",
    "continue",
    "break",
    "defer",
    ";",
    ",",
    ":",
    ".",
    "...",
    "!",
    "!=",
    "==",
    "+",
    "-",
    "*",
    "/",
    "%",
    "=",
    "+=",
    "-=",
    "*=",
    "/=",
    "%=",
    "&=",
    "|=",
    "^=",
    "<",
    "<=",
    ">",
    ">=",
    "&",
    "&&",
    "|",
    "||",
    "^",
    "~",
    "<<",
    ">>",
    "(",
    ")",
    "{",
    "}",
    "[",
    "]",
    "string",
    "char",
    "EOF"
};

static size_t const len_token_strings = sizeof(token_strings) / sizeof(char *);

// BINARY types
static char const *const binary_type_ops[] = {
    "error",
    "+",  // BINARY_ADD
    "-",  // BINARY_SUB
    "*",  // BINARY_MUL
    "/",  // BINARY_DIV
    "%",  // BINARY_MOD
    "&",
    "|",
    "^",
    "<",  // BINARY_ST
    "<=", // BINARY_SE
    ">",  // BINARY_GT
    ">=", // BINARY_GE
    "==", // BINARY_EQ
    "!=", // BINARY_NE
    "&&", // BINARY_LOG_AND
    "||", // BINARY_LOG_OR
    "<<",
    ">>",
};

static size_t const len_binary_strings = sizeof(binary_type_ops) / sizeof(char *);

// UNARY types
static char const *const unary_type_ops[] = {
    "error",
    "&",
    "*",
    "!",
    "-",
    "~"
};

static size_t const len_unary_strings = sizeof(unary_type_ops) / sizeof(char *);

// EXPR types
static char const *const expr_type_strings[] = {
    "error",
    "int literal",
    "float literal",
    "null literal",
    "undefined value",
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
    "sizeof expression",
    "index expression"
};

static size_t const len_expr_strings = sizeof(expr_type_strings) / sizeof(char *);

// STMT types
static char const *const stmt_type_strings[] = {
    "error",
    "expression",
    "let",
    "import",
    "function declaration",
    "structured type declaration",
    "enum type declaration",
    "union type declaration",
    "block",
    "if",
    "while",
    "delete",
    "return",
    "continue",
    "break",
    "defer",
    "for"
};

static size_t const len_stmt_strings = sizeof(stmt_type_strings) / sizeof(char *);

// Type types
static char const *const type_type_strings[] = {
    "error",
    "pointer",
    "array",
    "bool",
    "int",
    "f32",
    "f64",
    "void",
    "function",
    "function",
    "module",
    "struct",
    "enum",
    "union"
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
    "Could not parse statement",
    "Illegal identifier '%.*s'",
    "Illegal type identifier '%.*s': %s",
    "Invalid escape sequence '%.*s'",
    "Expected 1 receiver for Method, but got %u",
    // type errors
    "Duplicate definition of '%.*s'",
    "No %s called '%.*s' in %.*s",
    "Type '%s' cannot be accessed with '%s' operator",
    "Incomplete type '%.*s'",
    "Invalid operator for types '%s' and '%s'",
    "Pointers may only be compared or subtracted",
    "Cannot take address of rvalue",
    "Trying to dereference non pointer type '%s'",
    "Unary operator '%s' not defined for '%s'",
    "Wrong return type for function. Expected '%s', but got '%s'",
    "Missing return statement",
    "Trying to return a value inside a 'void' function",
    "Illegal use of declared type",
    "Type '%s' is not callable",
    "Wrong number of arguments. Expected '%u', but got '%u'",
    "Wrong argument type. Expected '%s', but got '%s'",
    "Expected the name of a struct or union, but got '%s'",
    "Unions can only be initialized with one value",
    "The field '%.*s' in '%.*s' was declared with type '%s', but was initialized with '%s'",
    "Could not infer type",
    "'%s' cannot be casted into '%s'",
    "Invalid type for condition. Expected 'bool', but got '%s'",
    "Trying to delete non pointer type: '%s'",
    "Variable was declared with type '%s', but initialized with type '%s'",
    "The type '%s' cannot be assigned to a variable",
    "Expression is not assignable",
    "Invalid expression",
    "Cannot use unsized type '%s' inside a struct",
    "Cannot use unsized type '%s' as a function parameter",
    "Cannot take the size of an unsized type '%s'",
    "Unsized type '%s' cannot be dereferenced",
    "Duplicate entry '%.*s' in enum '%.*s'",
    "Invalid enum start value type '%s' in enum '%.*s'",
    "Cannot declare method on primitive type '%s'",
    "Methods can only be declare on types from the same module",
    "Cannot take method pointer from instance. Use '&%s' instead",
    "Cannot take the address of a type",
    "Illegal expression for unary operand",
    "Expression cannot be evaluated at compile time",
    "Trying to access private symbol '%.*s' from '%.*s'",
    "%s statements may not be used outside of loops",
    "Wrong field. Expected '%.*s', but got '%.*s'",
    "Expression of type '%s' cannot be indexed with type '%s'"
};

static size_t const len_err_strings = sizeof(error_texts) / sizeof(char *);

// MIR binary kinds
static char const *const mir_binary_kind_strings[] = {
    NULL,
    "+",  // MIR_BIN_ADD
    "-",  // MIR_BIN_SUB
    "*",  // MIR_BIN_MUL
    "/",  // MIR_BIN_DIV
    "%",  // MIR_BIN_MOD
    "&",
    "|",
    "^",
    "<",  // MIR_BIN_ST
    "<=", // MIR_BIN_SE
    ">",  // MIR_BIN_GT
    ">=", // MIR_BIN_GE
    "==", // MIR_BIN_EQ
    "!=", // MIR_BIN_NE
    "&&", // MIR_BIN_BOOL_AND
    "||", // MIR_BIN_BOOL_OR
    "<<",
    ">>",
    "s+",  // MIR_BIN_ADD_SCALAR
    "s-",  // MIR_BIN_SUB_SCALAR
};

static size_t const len_mir_binary_kind_strings = sizeof(mir_binary_kind_strings) / sizeof(char *);

// MIR unary kinds
static char const *const mir_unary_kind_strings[] = {
    NULL,
    "!", // MIR_UNARY_BOOL_NEG
    "-", // MIR_UNARY_INT_NEG
    "*", // MIR_UNARY_DEREF
};

static size_t const len_mir_unary_kind_strings = sizeof(mir_unary_kind_strings) / sizeof(char *);

