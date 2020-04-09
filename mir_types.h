#define MIR_INST_DECL           0
#define MIR_INST_ASSIGN         1
#define MIR_INST_JMP            2
#define MIR_INST_JmpIf          3
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
