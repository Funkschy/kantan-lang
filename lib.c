#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define HASH_NUM 65599

static char const *const token_strings[] = {
    "error",
    "integer",
    "identifier",
    "eof"
};

int32_t hash(char const *key) {
    int32_t h = 0;
    for (char c = *key; c != '\0'; c++) {
        h = h * HASH_NUM + c;
    }
    return h;
}

char const* const tok2str(int32_t type) {
    return token_strings[type];
}
