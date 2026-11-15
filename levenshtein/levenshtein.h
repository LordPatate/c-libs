#ifndef LEVENSHTEIN_H
#define LEVENSHTEIN_H

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "res_or_err.h"

enum levenshtein_error {
    LEVENSHTEIN_ALLOC_FAILED = ENOMEM,
};

// Return the Levenshtein distance between the two strings.
// The .result field of a successful output is a size_t.
struct result_or_error levenshtein(const char *str1, const char *str2);

// Verify if str1 and str2 is a partially match.
// A partial match means the levenshtein distance is strictly below threshold.
// Return the distance if it is below threshold, the threshold otherwise.
// Most likely faster than levenshtein(str1, str2) for small thresholds.
// The .result field of a successful output is a size_t.
struct result_or_error partial_match(const char *str1, const char *str2, size_t threshold);

// Return the Levenshtein distance between the two strings.
// The .result field of a successful output is a size_t.
struct result_or_error levenshtein_sized(
    const char *str1, size_t n1,
    const char *str2, size_t n2
);

// Verify if str1 and str2 is a partially match.
// A partial match means the levenshtein distance is strictly below threshold.
// Return the distance if it is below threshold, the threshold otherwise.
// Most likely faster than levenshtein(str1, str2) for small thresholds.
// The .result field of a successful output is a size_t.
struct result_or_error partial_match_sized(
    const char *str1, size_t n1,
    const char *str2, size_t n2,
    size_t threshold
);

#endif /* !LEVENSHTEIN_H */
