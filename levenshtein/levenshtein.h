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
struct result_or_error levenshtein(const char *str1, const char *str2);

#endif /* !LEVENSHTEIN_H */
