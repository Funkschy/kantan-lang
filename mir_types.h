#define MIR_INST_DECL           1
#define MIR_INST_ASSIGN         2
#define MIR_INST_JMP            3
#define MIR_INST_JMPIF          4
#define MIR_INST_RETURN         5
#define MIR_INST_LABEL          6
#define MIR_INST_DELETE         7
#define MIR_INST_NOP            8

#define MIR_ADDR_EMPTY          1
#define MIR_ADDR_NULL           2
#define MIR_ADDR_NAME           3
#define MIR_ADDR_CONST          4
#define MIR_ADDR_GLOBAL         5
#define MIR_ADDR_REF            6

#define MIR_EXPR_BINARY         1
#define MIR_EXPR_UNARY          2
#define MIR_EXPR_COPY           3
#define MIR_EXPR_CALL           4
#define MIR_EXPR_STRUCTGEP      5
#define MIR_EXPR_STRUCTINIT     6
#define MIR_EXPR_NEW            7
#define MIR_EXPR_SIZEOF         8
#define MIR_EXPR_GETPARAM       9
#define MIR_EXPR_BITCAST        10

#define MIR_BIN_ADD_SCALAR      1
#define MIR_BIN_SUB_SCALAR      2

#define MIR_BIN_ADD             3
#define MIR_BIN_SUB             4
#define MIR_BIN_MUL             5
#define MIR_BIN_DIV             6
#define MIR_BIN_MOD             7

#define MIR_BIN_ST              8
#define MIR_BIN_SE              9
#define MIR_BIN_GT              10
#define MIR_BIN_GE              11
#define MIR_BIN_EQ              14
#define MIR_BIN_NE              15

#define MIR_BIN_BOOL_AND        16
#define MIR_BIN_BOOL_OR         17

#define MIR_UNARY_BOOL_NEG      1
#define MIR_UNARY_INT_NEG       2
#define MIR_UNARY_DEREF         3
