#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>

#include "./lib.h"

#define DEBUG_ASSERT 1

// forward decls
ssize_t vformat_str(char **dest, char const *fmt, va_list args);

int32_t get_errno() {
    return errno;
}

char *get_str(int32_t index, size_t len, char * const * const array) {
    if (index < 0 || (size_t)index > len) {
        return array[0];
    }

    return array[index];
}

char *tok2str(int32_t type) {
    return get_str(type, len_token_strings, token_strings);
}

char *binary2str(int32_t op) {
    return get_str(op, len_binary_strings, binary_type_ops);
}

char *unary2str(int32_t op) {
    return get_str(op, len_unary_strings, unary_type_ops);
}

char *mir_binary2str(int32_t op) {
    return get_str(op, len_mir_binary_kind_strings, mir_binary_kind_strings);
}

char *mir_unary2str(int32_t op) {
    return get_str(op, len_mir_unary_kind_strings, mir_unary_kind_strings);
}

char *expr2str(int32_t type) {
    return get_str(type, len_expr_strings, expr_type_strings);
}

char *stmt2str(int32_t type) {
    return get_str(type, len_stmt_strings, stmt_type_strings);
}

char *type2str(int32_t type) {
    return get_str(type, len_type_strings, type_type_strings);
}

void assert_fmt(bool condition, char const *fmt, ...) {
#if DEBUG_ASSERT
    if (condition) {
        return;
    }

    char *s = NULL;
    va_list args;
    va_start(args, fmt);
    ssize_t size = vformat_str(&s, fmt, args);
    va_end(args);

    if (size < 0) {
        return;
    }

    printf("%s\n", s);
    free(s);

    abort();
#endif
}

char *l_format_str(size_t *len, char const *fmt, ...) {
    char *s = NULL;
    va_list args;
    va_start(args, fmt);
    ssize_t size = vformat_str(&s, fmt, args);
    va_end(args);

    if (size < 0) {
        return NULL;
    }

    *len = size;
    return s;
}

char *format_str(char const *fmt, ...) {
    char *s = NULL;
    va_list args;
    va_start(args, fmt);
    ssize_t size = vformat_str(&s, fmt, args);
    va_end(args);

    if (size < 0) {
        return NULL;
    }

    return s;
}

char *err2str(int32_t err_code, ...) {
    if (err_code < 0 || (size_t)err_code > len_err_strings) {
        return error_texts[0];
    }

    char *s = NULL;
    va_list args;
    va_start(args, err_code);
    ssize_t size = vformat_str(&s, error_texts[err_code], args);
    va_end(args);

    if (size < 0) {
        return error_texts[0];
    }

    return s;
}

size_t read_char(char const *string, size_t s_len, int32_t *ch) {
    int32_t c_len = 1;

    if ((*string & 0xf8) == 0xf0) {
        c_len = 4;
    } else if ((*string & 0xf0) == 0xe0) {
        c_len = 3;
    } else if ((*string & 0xe0) == 0xc0) {
        c_len = 2;
    }

    if ((size_t)c_len > s_len) {
        return s_len;
    }

    if (ch != NULL) {
        memcpy(ch, string, c_len);
    }

    return (size_t) c_len;
}

bool is_file(char const *path) {
    struct stat s;
    if (stat(path, &s) == 0) {
        return s.st_mode & S_IFREG;
    }
    return false;
}

// these have to match error.kan
#define ERROR_COULD_NOT_OPEN_FILE       3
#define ERROR_COULD_NOT_ALLOCATE_BUFFER 4
#define ERROR_COULD_NOT_READ_FILE       5

int32_t read_file(char const *path, char const **content, size_t *len) {
    if (!is_file(path)) {
        // TODO(#4): custom error, and check if this works on windows
        return ERROR_COULD_NOT_OPEN_FILE;
    }

    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        return ERROR_COULD_NOT_OPEN_FILE;
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = (size_t) ftell(file);
    rewind(file);

    char *buffer = malloc(file_size + 1);

    if (buffer == NULL) {
        fclose(file);
        return ERROR_COULD_NOT_ALLOCATE_BUFFER;
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);

    if (bytes_read < file_size) {
        fclose(file);
        free(buffer);
        return ERROR_COULD_NOT_READ_FILE;
    }

    buffer[bytes_read] = '\0';

    fclose(file);
    *content = buffer;
    *len = file_size;

    return 0;
}

ssize_t vformat_str(char **dest, char const *fmt, va_list args){
    int size = 0;
    va_list tmp_args;

    // vsnprintf modifies the arg list, so we have to copy it here
    va_copy(tmp_args, args);

    // pass NULL as str, so that we just get the size
    size = vsnprintf(NULL, 0, fmt, tmp_args);

    va_end(tmp_args);

    if (size < 0) {
        return size;
    }

    char *str = malloc(size + 1);
    if (str == NULL) {
        return -1;
    }
    size = vsprintf(str, fmt, args);
    *dest = str;

    return size;
}

size_t int_num_digits(size_t i) {
    // pass NULL as str, so that we just get the size
    return snprintf(NULL, 0, "%lu", i);
}

void flag_set(uint32_t *flags, uint32_t flag) {
    *flags |= flag;
}

bool flag_get(uint32_t *flags, uint32_t flag) {
    return *flags & flag;
}

void flag_unset(uint32_t *flags, uint32_t flag) {
    if (flag_get(flags, flag)) {
        *flags ^= flag;
    }
}

size_t bool_to_int(bool b) {
    return b;
}

bool int_to_bool(size_t i) {
    return (bool) i;
}

size_t ptr_to_int(void* ptr) {
    return (size_t) ptr;
}

void *int_to_ptr(size_t i) {
    size_t s = i;
    return (void *)s;
}

enum OS {
    NONE, UNKNOWN, LINUX, DARWIN, WIN32
};

void get_os(int32_t *os) {
#if defined(linux) || defined(__linux__)
    *os = (int32_t) LINUX;
#elif defined(darwin) || defined(__APPLE__)
    *os = (int32_t) DARWIN;
#elif defined(WIN32) || defined(_WIN32)
    *os = (int32_t) WIN32;
#else
    *os = (int32_t) NONE;
#endif
}
