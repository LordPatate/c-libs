#ifndef LEVENSHTEIN_H
#define LEVENSHTEIN_H

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "res_or_err.h"

enum levenshtein_error {
    LEVENSHTEIN_ALLOC_FAILED = ENOMEM,
};

/* Return the Levenshtein distance between the two strings.
** The .result field of a successful output is a size_t.
** Quadratic time and space complexities
** (uses a cache of size strlen(str1) * strlen(str2)).
*/
struct result_or_error levenshtein(const char *str1, const char *str2);

/* Return the Levenshtein distance between the two strings.
** The .result field of a successful output is a size_t.
** Quadratic time and space complexities (uses a cache of size n1 * n2).
*/
struct result_or_error levenshtein_sized(
    const char *str1, size_t n1,
    const char *str2, size_t n2
);

/* Verify if str1 and str2 partially match.
** A partial match means the levenshtein distance is strictly below threshold.
** Return the distance if it is below threshold, the threshold otherwise.
** Note on performance:
**   This function is not cached, but exits as soon as the number of errors
**   exceeds threshold.
**   Time complexity is in exponential of the threshold.
**   This can makes it considerably faster than the levenshtein functions for
**   very small thresholds, even on gigantic strings; but considerably slower
**   for larger ones.
**   Also, no cache allocation is done, which is why it cannot fail.
*/
size_t partial_match(const char *str1, const char *str2, size_t threshold);

/* Verify if str1 and str2 partially match.
** A partial match means the levenshtein distance is strictly below threshold.
** Return the distance if it is below threshold, the threshold otherwise.
** Note on performance:
**   This function is not cached, but exits as soon as the number of errors
**   exceeds threshold.
**   Time complexity is in exponential of the threshold.
**   This makes it considerably faster than the levenshtein functions for very
**   small thresholds, even on gigantic strings; but considerably slower for
**   larger ones.
**   Also, no cache allocation is done, which is why it cannot fail.
*/
size_t partial_match_sized(
    const char *str1, size_t n1,
    const char *str2, size_t n2,
    size_t threshold
);

#endif /* !LEVENSHTEIN_H */
