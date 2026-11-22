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

#define _AT(i, j) (i) * cache->dim[1] + (j)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static size_t distance_cached(
    size_t i, size_t j,
    const char *str1, size_t n1,
    const char *str2, size_t n2,
    struct _cache *cache
) {
    if (i == n1) {
        return n2 - j;
    }
    if (j == n2) {
        return n1 - i;
    }
    if (cache->state[_AT(i, j)] == NEW) {
        size_t d;
        if (str1[i] == str2[j]) {
            d = distance_cached(i + 1, j + 1, str1, n1, str2, n2, cache);
        } else { // First chars differ
            size_t substitution = distance_cached(i + 1, j + 1, str1, n1, str2, n2, cache);
            size_t delete1 = distance_cached(i + 1, j, str1, n1, str2, n2, cache);
            size_t delete2 = distance_cached(i, j + 1, str1, n1, str2, n2, cache);
            d = 1 + MIN(substitution, MIN(delete1, delete2));
        }
        cache->array[_AT(i, j)] = d;
        cache->state[_AT(i, j)] = SET;
    }
    return cache->array[_AT(i, j)];
}

struct result_or_error levenshtein_sized(
    const char *str1, size_t n1,
    const char *str2, size_t n2
) {
    struct result_or_error result;
    struct _cache cache = {
        .array = calloc(n1 * n2, sizeof(size_t)),
        .state = calloc(n1 * n2, sizeof(size_t)),
        .dim = {n1, n2},
    };
    if (cache.array && cache.state) {
        result = ULLONG_RESULT(distance_cached(0, 0, str1, n1, str2, n2, &cache));
    } else {
        result = ERROR(LEVENSHTEIN_ALLOC_FAILED);
    }
    free(cache.array);
    free(cache.state);
    return result;
}

struct result_or_error levenshtein(const char *str1, const char *str2) {
    return levenshtein_sized(str1, strlen(str1), str2, strlen(str2));
}

static size_t distance_with_threshold(
    size_t i, size_t j,
    const char *str1, size_t n1,
    const char *str2, size_t n2,
    size_t threshold
) {
    if (i == n1) {
        return n2 - j;
    }
    if (j == n2) {
        return n1 - i;
    }
    size_t d = threshold;
    if (str1[i] == str2[j]) {
        d = distance_with_threshold(i + 1, j + 1, str1, n1, str2, n2, threshold);
    } else { // First chars differ
        threshold -= 1;
        if (threshold > 0) {
            size_t substitution = distance_with_threshold(i + 1, j + 1, str1, n1, str2, n2, threshold);
            size_t delete1 = distance_with_threshold(i + 1, j, str1, n1, str2, n2, threshold);
            size_t delete2 = distance_with_threshold(i, j + 1, str1, n1, str2, n2, threshold);
            d = 1 + MIN(substitution, MIN(delete1, delete2));
        }
    }
    return d;
}

size_t partial_match_sized(
    const char *str1, size_t n1,
    const char *str2, size_t n2,
    size_t threshold
) {
    return MIN(threshold, distance_with_threshold(0, 0, str1, n1, str2, n2, threshold));
}

size_t partial_match(const char *str1, const char *str2, size_t threshold) {
    return partial_match_sized(str1, strlen(str1), str2, strlen(str2), threshold);
}
