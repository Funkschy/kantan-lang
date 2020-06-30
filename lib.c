#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>

#include "./lib.h"

#define HASH_NUM 65599
#define DEBUG_ASSERT 1

// forward decls
int vformat_str(char **dest, char const *fmt, va_list args);

int32_t get_errno() {
    return errno;
}

char const *const get_str(int32_t index, size_t len, char const *const * array) {
    if (index < 0 || index > len) {
        return array[0];
    }

    return array[index];
}

char const *const tok2str(int32_t type) {
    return get_str(type, len_token_strings, token_strings);
}

char const *const binary2str(int32_t op) {
    return get_str(op, len_binary_strings, binary_type_ops);
}

char const *const unary2str(int32_t op) {
    return get_str(op, len_unary_strings, unary_type_ops);
}

char const *const mir_binary2str(int32_t op) {
    return get_str(op, len_mir_binary_kind_strings, mir_binary_kind_strings);
}

char const *const mir_unary2str(int32_t op) {
    return get_str(op, len_mir_unary_kind_strings, mir_unary_kind_strings);
}

char const *const expr2str(int32_t type) {
    return get_str(type, len_expr_strings, expr_type_strings);
}

char const *const stmt2str(int32_t type) {
    return get_str(type, len_stmt_strings, stmt_type_strings);
}

char const *const type2str(int32_t type) {
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
    int32_t size = vformat_str(&s, fmt, args);
    va_end(args);

    if (size < 0) {
        return;
    }

    printf("%s\n", s);
    free(s);

    abort();
#endif
}

char const *const l_format_str(int32_t *len, char const *fmt, ...) {
    char *s = NULL;
    va_list args;
    va_start(args, fmt);
    int32_t size = vformat_str(&s, fmt, args);
    va_end(args);

    if (size < 0) {
        return NULL;
    }

    *len = size;
    return s;
}

char const *const format_str(char const *fmt, ...) {
    char *s = NULL;
    va_list args;
    va_start(args, fmt);
    int32_t size = vformat_str(&s, fmt, args);
    va_end(args);

    if (size < 0) {
        return NULL;
    }

    return s;
}

char const *const err2str(int32_t err_code, ...) {
    if (err_code < 0 || err_code > len_err_strings) {
        return error_texts[0];
    }

    char *s = NULL;
    va_list args;
    va_start(args, err_code);
    int32_t size = vformat_str(&s, error_texts[err_code], args);
    va_end(args);

    if (size < 0) {
        return error_texts[0];
    }

    return s;
}

int32_t read_char(char const *string, int32_t s_len, int32_t *ch) {
    int32_t c_len = 1;

    if ((*string & 0xf8) == 0xf0) {
        c_len = 4;
    } else if ((*string & 0xf0) == 0xe0) {
        c_len = 3;
    } else if ((*string & 0xe0) == 0xc0) {
        c_len = 2;
    }

    if (c_len > s_len) {
        return s_len;
    }

    if (ch != NULL) {
        memcpy(ch, string, c_len);
    }

    return c_len;
}

int32_t hash(char const *const key, int32_t len) {
    int32_t h = 0;
    for (int32_t i = 0; i < len; i++) {
        h = h * HASH_NUM + key[i];
    }
    return h;
}

int32_t get_map_index(int32_t cap, char const *const key, int32_t key_len) {
    int32_t h = hash(key, key_len);
    return h & (cap - 1);
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

int32_t read_file(char const *path, char const **content, int32_t *len) {
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

int vformat_str(char **dest, char const *fmt, va_list args){
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

int32_t int_num_digits(int32_t i) {
    // pass NULL as str, so that we just get the size
    return snprintf(NULL, 0, "%d", i);
}

uint64_t next_pow_of_2(uint64_t num) {
    uint64_t n = num - 1;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    // if n is 0 add 1
    n += 1 + (n == 0);
    return n;
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

int32_t char_to_int(char c) {
    return (int32_t) c;
}

char int_to_char(int32_t i) {
    return (char) i;
}

int32_t bool_to_int(bool b) {
    return (int32_t) b;
}

char int_to_bool(int32_t i) {
    return (bool) i;
}

int32_t ptr_to_int(void* ptr) {
    return (int32_t)((size_t) ptr);
}

void *int_to_ptr(int32_t i) {
    size_t s = i;
    return *((void **)&s);
}

void get_sys(bool *is_linux, bool *is_darwin, bool *is_win32) {
#if defined(linux) || defined(__linux__)
    if (is_linux) *is_linux = true;
#elif defined(darwin) || defined(__APPLE__)
    if (is_darwin) *is_darwin = true;
#elif defined(WIN32) || defined(_WIN32)
    if (is_win32) *is_win32 = true;
#endif
}
