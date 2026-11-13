#include "levenshtein.h"

enum cache_state {
    NEW = 0,
    SET,
};

struct _cache {
    size_t *array;
    enum cache_state *state;
    size_t dim[2];
};

#define CACHE_AT(i, j) cache->array[(i) * cache->dim[0] + (j)]
#define STATE_AT(i, j) cache->state[(i) * cache->dim[0] + (j)]
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static size_t distance(
    struct _cache *cache,
    const char *str1,
    const char *str2,
    size_t i, size_t j
) {
    if (STATE_AT(i, j) == NEW) {
        STATE_AT(i, j) = SET;
        if (!str1[i]) {
            if (!str2[j]) {
                // Both empty
                CACHE_AT(i, j) = 0;
            } else {
                // str2 non empty
                CACHE_AT(i, j) = strlen(str2 + j);
            }
        } else {
            if (!str2[j]) {
                // str1 non empty
                CACHE_AT(i, j) = strlen(str1 + i);
            } else {
                // both non-empty
                if (str1[i] == str2[j]) {
                    CACHE_AT(i, j) = distance(cache, str1, str2, i + 1, j + 1);
                } else {
                    size_t substitution = distance(cache, str1, str2, i + 1, j + 1);
                    size_t delete1 = distance(cache, str1, str2, i + 1, j);
                    size_t delete2 = distance(cache, str1, str2, i, j + 1);
                    CACHE_AT(i, j) = 1 + MIN(substitution, MIN(delete1, delete2));
                }
            }
        }
    }
    return CACHE_AT(i, j);
}

struct result_or_error levenshtein(const char *str1, const char *str2) {
    struct result_or_error result;
    size_t n1 = strlen(str1) + 1;
    size_t n2 = strlen(str2) + 1;
    struct _cache cache = {
        .array = calloc(n1 * n2, sizeof(size_t)),
        .state = calloc(n1 * n2, sizeof(size_t)),
        .dim = {n1, n2},
    };
    if (cache.array && cache.state) {
        result = RESULT(distance(&cache, str1, str2, 0, 0));
    } else {
        result = ERROR(LEVENSHTEIN_ALLOC_FAILED);
    }
    free(cache.array);
    free(cache.state);
    return result;
}
