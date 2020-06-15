#define EXPR_INT        1
#define EXPR_FLOAT      2
#define EXPR_NULL       3
#define EXPR_UNDEFINED  4
#define EXPR_STRING     5
#define EXPR_CHAR       6
#define EXPR_BINARY     7
#define EXPR_UNARY      8
#define EXPR_IDENT      9
#define EXPR_ASSIGN     10
#define EXPR_CALL       11
#define EXPR_INIT       12
#define EXPR_ACCESS     13
#define EXPR_AS         14
#define EXPR_NEW        15
#define EXPR_SIZEOF     16

// These have to match the mir_types

#define BINARY_ADD      1
#define BINARY_SUB      2
#define BINARY_MUL      3
#define BINARY_DIV      4
#define BINARY_MOD      5

#define BINARY_ST       6
#define BINARY_SE       7
#define BINARY_GT       8
#define BINARY_GE       9

#define BINARY_EQ       10
#define BINARY_NE       11
#define BINARY_LOG_AND  12
#define BINARY_LOG_OR   13

#define UNARY_REF       1
#define UNARY_DEREF     2
#define UNARY_NEG_BOOL  3
#define UNARY_NEG_NUM   4
