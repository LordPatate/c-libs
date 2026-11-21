#include "levenshtein.h"
#include <stdio.h>

#define DIFF(a, b) ((a) > (b) ? (a) - (b) : (b) - (a))

int test(const char *ref, const char *compared, size_t expected) {
    struct result_or_error res = levenshtein(ref, compared);
    if (res.ok) {
        size_t d = res.result.as_ullong;
        size_t n1 = strlen(ref);
        size_t n2 = strlen(compared);
        double r1 = (double) DIFF(d, n1) / (double) n1;
        double r2 = (double) DIFF(d, n2) / (double) n2;
        printf(
            "%s - %s:\n"
            "  distance = %zu\n"
            "  resemblance = %f\n"
            "  #resemblance = %f\n"
            "\n", ref, compared, d, r1, r2
        );
        if (d != expected)
            puts("FAILED");
        return d == expected;
    } else {
        printf("Levenshtein failed (exit status %d).\n", res.error);
        return 0;
    }
}

int test_both(const char *ref, const char *compared, size_t expected) {
    return test(ref, compared, expected) && test(compared, ref, expected);
}

int test_threshold(const char *ref, const char *compared, size_t threshold, size_t expected) {
    struct result_or_error res = partial_match(ref, compared, threshold);
    if (res.ok) {
        size_t d = res.result.as_ullong;
        if (d == threshold) {
            printf("not a match: %s - %s (distance = %zu >= %zu)\n", ref, compared, d, threshold);
        } else {
            printf("partial match: %s - %s (distance = %zu < %zu)\n", ref, compared, d, threshold);
        }
        if (d != expected)
            puts("FAILED");
        return d == expected;
    } else {
        printf("Levenshtein failed (exit status %d).\n", res.error);
        return 0;
    }
}

int test_both_threshold(const char *ref, const char *compared, size_t threshold, size_t expected) {
    return test_threshold(ref, compared, threshold, expected) && test_threshold(compared, ref, threshold, expected);
}

int main(void) {
    int successes = 0, total = 0;
    successes += test_both("abc", "abc", 0); ++total;

    successes += test_both("abc", "abd", 1); ++total;
    successes += test_both("abc", "adc", 1); ++total;
    successes += test_both("abc", "dbc", 1); ++total;

    successes += test_both("abc", "ab", 1); ++total;
    successes += test_both("abc", "ac", 1); ++total;
    successes += test_both("abc", "bc", 1); ++total;

    successes += test_both("abc", "abcd", 1); ++total;
    successes += test_both("abc", "abdc", 1); ++total;
    successes += test_both("abc", "adbc", 1); ++total;
    successes += test_both("abc", "dabc", 1); ++total;

    successes += test_both("abc", "def", 3); ++total;
    successes += test_both("abc", "", 3); ++total;
    successes += test_both("abc", "abcdef", 3); ++total;
    successes += test_both("abc", "defabc", 3); ++total;
    successes += test_both("abc", "adbecf", 3); ++total;

    successes += test_both_threshold("abc", "abc", 1, 0); ++total;
    successes += test_both_threshold("abc", "def", 2, 2); ++total;
    successes += test_both_threshold("abc", "", 4, 3); ++total;
    successes += test_both_threshold("abc", "abcdef", 1, 1); ++total;
    successes += test_both_threshold("abc", "defabc", 3, 3); ++total;
    successes += test_both_threshold("abc", "adbecf", 3, 3); ++total;

    printf(
        "\n-----------------------\n"
        "Passing: %d, failing: %d\n",
        successes, total - successes
    );
}
