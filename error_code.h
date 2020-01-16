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
#define ERROR_INVALID_ACCESS_TYPE       15
