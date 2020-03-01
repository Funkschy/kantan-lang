#define OK                              0

#define ERROR_INTERNAL                  1

// file errors
#define ERROR_PATH_EMTPY                2
#define ERROR_COULD_NOT_OPEN_FILE       3
#define ERROR_COULD_NOT_ALLOCATE_BUFFER 4
#define ERROR_COULD_NOT_READ_FILE       5

// Parse Errors
#define ERROR_UNEXPECTED_EOF            6
#define ERROR_EXPECTED_BUT_GOT          7
#define ERROR_UNKNOWN_SYMBOL            8
#define ERROR_CHAR_LIT_LEN              9
#define ERROR_COULD_NOT_PARSE_STMT      10
#define ERROR_INVALID_TYPE_IDENT        11

// Type Errors
#define ERROR_DUPLICATE_DEFINTION       12
#define ERROR_NOT_DEFINED               13
#define ERROR_NOT_ACCESSIBLE_WITH_OP    14
#define ERROR_INCOMPLETE_TYPE           15
#define ERROR_BIN_INVALID_TYPES         16
#define ERROR_BIN_PTR_INVALID           17
#define ERROR_UNARY_REF_RVALUE          18
#define ERROR_UNARY_DEREF_NON_PTR       19
#define ERROR_UNARY_OP_NOT_DEFINED      20
#define ERROR_RETURN_OUTSIDE_OF_FUNC    21
#define ERROR_WRONG_RETURN_TYPE         22
#define ERROR_MISSING_RETURN            23
#define ERROR_USING_OP_ON_TY_DECL       24
#define ERROR_CALLING_NON_FUNCTION      25
#define ERROR_WRONG_NUMBER_OF_ARGS      26
#define ERROR_WRONG_ARG_TYPE            27
#define ERROR_INIT_NON_STRUCT_TYPE      28
#define ERROR_COULD_NOT_INFER_TYPE      29
#define ERROR_INVALID_CAST              30
