#define MIR_INST_DECL           0
#define MIR_INST_ASSIGN         1
#define MIR_INST_JMP            2
#define MIR_INST_JMPIF          3
#define MIR_INST_RETURN         4
#define MIR_INST_LABEL          5
#define MIR_INST_DELETE         6
#define MIR_INST_NOP            7

#define MIR_ADDR_EMPTY          0
#define MIR_ADDR_NULL           1
#define MIR_ADDR_NAME           2
#define MIR_ADDR_CONST          3
#define MIR_ADDR_TEMP           4
#define MIR_ADDR_GLOBAL         5
#define MIR_ADDR_REF            6

#define MIR_EXPR_BINARY         0
#define MIR_EXPR_UNARY          1
#define MIR_EXPR_COPY           2
#define MIR_EXPR_CALL           3
#define MIR_EXPR_STRUCTGEP      4
#define MIR_EXPR_STRUCTINIT     5
#define MIR_EXPR_NEW            6
#define MIR_EXPR_SIZEOF         7
#define MIR_EXPR_GETPARAM       8
#define MIR_EXPR_BITCAST        9

#define MIR_BIN_ADD_SCALAR      0
#define MIR_BIN_SUB_SCALAR      1

#define MIR_BIN_ADD             2
#define MIR_BIN_SUB             3
#define MIR_BIN_MUL             4
#define MIR_BIN_DIV             5
#define MIR_BIN_MOD             6

#define MIR_BIN_ST              7
#define MIR_BIN_SE              8
#define MIR_BIN_GT              9
#define MIR_BIN_GE              10
#define MIR_BIN_EQ              13
#define MIR_BIN_NE              14

#define MIR_BIN_BOOL_AND        15
#define MIR_BIN_BOOL_OR         16

#define MIR_UNARY_BOOL_NEG      0
#define MIR_UNARY_INT_NEG       1
#define MIR_UNARY_DEREF         2
