#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// TODO: this entire implementation is really inefficient

typedef struct {
    int32_t orig_idx;
    int32_t value;
} tuple;

void swap(tuple *first, tuple *second) {
    tuple temp = *first;
    *first = *second;
    *second = temp;
}

void inner_isort(tuple *arr, int32_t len) {
    if (len <= 1) {
        return;
    }

    // use last elem as pivot
    tuple *pivot = arr + len - 1;

    int32_t j = len - 1;
    int32_t i = -1;

    while(1) {
        while(arr[++i].value < pivot->value);
        while(arr[--j].value > pivot->value && j > 0);

        if (i >= j) break;

        swap(arr + i, arr + j);
    }

    swap(arr + i, pivot);
    inner_isort(arr, i);
    inner_isort(arr + i + 1, len - i - 1);
}

// returns an array (with length len) with the original indices of import_nums
//                   0  1  2  3
//                   ----------
// e.g. import_nums: 2, 4, 1, 2
// returns:          2, 0, 3, 1
int32_t* isort(int32_t const *import_nums, int32_t len) {
    tuple *tuples = malloc(len * sizeof(tuple));
    for(int32_t i = 0; i < len; i++) {
        tuples[i] = (tuple) { .orig_idx = i, .value = import_nums[i] };
    }

    inner_isort(tuples, len);

    int32_t *result = malloc(len * sizeof(int32_t));
    for(int32_t i = 0; i < len; i++) {
        result[i] = tuples[i].orig_idx;
    }

    free(tuples);

    return result;
}
