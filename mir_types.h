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
#define MIR_ADDR_UNDEFINED      3
#define MIR_ADDR_NAME           4
#define MIR_ADDR_CONST          5
#define MIR_ADDR_GLOBAL         6
#define MIR_ADDR_REF            7
#define MIR_ADDR_ARG            8

#define MIR_EXPR_BINARY         1
#define MIR_EXPR_UNARY          2
#define MIR_EXPR_COPY           3
#define MIR_EXPR_CALL           4
#define MIR_EXPR_GEP            5
#define MIR_EXPR_STRUCTINIT     6
#define MIR_EXPR_NEW            7
#define MIR_EXPR_SIZEOF         8
#define MIR_EXPR_CAST           9

#define MIR_BIN_ADD             1
#define MIR_BIN_SUB             2
#define MIR_BIN_MUL             3
#define MIR_BIN_DIV             4
#define MIR_BIN_MOD             5

#define MIR_BIN_ST              6
#define MIR_BIN_SE              7
#define MIR_BIN_GT              8
#define MIR_BIN_GE              9
#define MIR_BIN_EQ              10
#define MIR_BIN_NE              11

#define MIR_BIN_BOOL_AND        12
#define MIR_BIN_BOOL_OR         13

#define MIR_BIN_ADD_SCALAR      14
#define MIR_BIN_SUB_SCALAR      15

#define MIR_UNARY_BOOL_NEG      1
#define MIR_UNARY_NUM_NEG       2
#define MIR_UNARY_DEREF         3

#define MIR_CAST_BIT            1
#define MIR_CAST_INT            2
