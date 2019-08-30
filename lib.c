#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "./error_code.h"

#define HASH_NUM 65599

// forward decls
int format_str(char **dest, char const *fmt, va_list args);

static char const *const token_strings[] = {
    "error",
    "integer",
    "identifier",
    "let",
    "if",
    "import",
    ";",
    "=",
    "+",
    "-",
    "*",
    "/",
    "(",
    ")"
};

static size_t const len_token_strings = sizeof(token_strings) / sizeof(char *);

static char const *const binary_type_ops[] = {
    "error",
    "+",
    "-",
    "*",
    "/"
};

static size_t const len_binary_strings = sizeof(binary_type_ops) / sizeof(char *);

char const *const tok2str(int32_t type) {
    if (type < 0 || type > len_token_strings) {
        return token_strings[0];
    }

    return token_strings[type];
}

char const *const binary2str(int32_t op) {
    if (op < 0 || op > len_binary_strings) {
        return binary_type_ops[0];
    }

    return binary_type_ops[op];
}

static char const *const error_texts[] = {
    NULL,
    "Path was empty",
    "Could not open file: '%s'",
    "Could not allocate buffer",
    "Could not read file: '%s'"
};

static size_t const len_err_strings = sizeof(error_texts) / sizeof(char *);

char const *const err2str(int32_t err_code, ...) {
    if (err_code < 0 || err_code > len_err_strings) {
        return error_texts[0];
    }

    char *s = NULL;
    va_list args;
    va_start(args, err_code);
    int32_t size = format_str(&s, error_texts[err_code], args);
    va_end(args);

    if (size < 0) {
        return error_texts[0];
    }

    return s;
}

int32_t hash(char const *const key) {
    int32_t h = 0;
    for (char c = *key; c != '\0'; c++) {
        h = h * HASH_NUM + c;
    }
    return h;
}

int32_t get_map_index(int32_t cap, char const *const key) {
    int32_t h = hash(key);
    return h & (cap - 1);
}

int32_t read_file(char const *path, char const **content) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return COULD_NOT_OPEN_FILE;
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = (size_t) ftell(file);
    rewind(file);

    char *buffer = malloc(file_size + 1);

    if (buffer == NULL) {
        fclose(file);
        return COULD_NOT_ALLOCATE_BUFFER;
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);

    if (bytes_read < file_size) {
        fclose(file);
        free(buffer);
        return COULD_NOT_READ_FILE;
    }

    buffer[bytes_read] = '\0';

    fclose(file);
    *content = buffer;

    return 0;
}

int format_str(char **dest, char const *fmt, va_list args){
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

_Float32 int_to_float(int64_t i) {
    return (_Float32) i;
}
