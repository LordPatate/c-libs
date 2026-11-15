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
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static size_t distance(
    size_t i, size_t j,
    struct _cache *cache,
    const char *str1,
    const char *str2,
    size_t threshold
) {
    if (STATE_AT(i, j) == NEW) {
        size_t d = threshold;
        if (i == cache->dim[0]) {
            if (j == cache->dim[1]) {
                d = 0;
            } else { // str2 non empty
                d = MIN(threshold, cache->dim[1] - j);
            }
        } else { // str1 non empty
            if (j == cache->dim[1]) {
                d = MIN(threshold, cache->dim[0] - i);
            } else { // str2 non empty
                if (str1[i] == str2[j]) {
                    d = distance(i + 1, j + 1, cache, str1, str2, threshold);
                } else { // First chars differ
                    threshold -= 1;
                    if (threshold > 0) {
                        size_t substitution = distance(i + 1, j + 1, cache, str1, str2, threshold);
                        size_t delete1 = distance(i + 1, j, cache, str1, str2, threshold);
                        size_t delete2 = distance(i, j + 1, cache, str1, str2, threshold);
                        d = 1 + MIN(substitution, MIN(delete1, delete2));
                    }
                }
            }
        }
        CACHE_AT(i, j) = d;
        STATE_AT(i, j) = SET;
    }
    return CACHE_AT(i, j);
}

struct result_or_error partial_match_sized(
    const char *str1, size_t n1,
    const char *str2, size_t n2,
    size_t threshold
) {
    struct result_or_error result;
    struct _cache cache = {
        .array = calloc((n1 + 1) * (n2 + 1), sizeof(size_t)),
        .state = calloc((n1 + 1) * (n2 + 1), sizeof(size_t)),
        .dim = {n1, n2},
    };
    if (cache.array && cache.state) {
        result = ULLONG_RESULT(distance(0, 0, &cache, str1, str2, threshold));
    } else {
        result = ERROR(LEVENSHTEIN_ALLOC_FAILED);
    }
    free(cache.array);
    free(cache.state);
    return result;
}

struct result_or_error partial_match(const char *str1, const char *str2, size_t threshold) {
    return partial_match_sized(str1, strlen(str1), str2, strlen(str2), threshold);
}

struct result_or_error levenshtein_sized(
    const char *str1, size_t n1,
    const char *str2, size_t n2
) {
    return partial_match_sized(str1, n1, str2, n2, MAX(n1, n2));
}

struct result_or_error levenshtein(const char *str1, const char *str2) {
    return levenshtein_sized(str1, strlen(str1), str2, strlen(str2));
}
