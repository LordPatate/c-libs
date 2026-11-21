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

int test_threshold(const char *ref, const char *compared, size_t threshold, size_t expected) {
    struct result_or_error res = partial_match(ref, compared, threshold);
    if (res.ok) {
        size_t d = res.result.as_ullong;
        if (d == threshold) {
            printf("not a match: %s - %s (distance >= %zu)\n", ref, compared, d);
        } else {
            printf("partial match: %s - %s (distance = %zu)\n", ref, compared, d);
        }
        if (d != expected)
            puts("FAILED");
        return d == expected;
    } else {
        printf("Levenshtein failed (exit status %d).\n", res.error);
        return 0;
    }
}

int main(void) {
    int successes = 0, total = 0;
    successes += test("abc", "abc", 0); ++total;

    successes += test("abc", "abd", 1); ++total;
    successes += test("abc", "adc", 1); ++total;
    successes += test("abc", "dbc", 1); ++total;

    successes += test("abc", "ab", 1); ++total;
    successes += test("abc", "ac", 1); ++total;
    successes += test("abc", "bc", 1); ++total;

    successes += test("abc", "abcd", 1); ++total;
    successes += test("abc", "abdc", 1); ++total;
    successes += test("abc", "adbc", 1); ++total;
    successes += test("abc", "dabc", 1); ++total;

    successes += test("abc", "def", 3); ++total;
    successes += test("abc", "", 3); ++total;
    successes += test("abc", "abcdef", 3); ++total;
    successes += test("abc", "defabc", 3); ++total;
    successes += test("abc", "adbecf", 3); ++total;

    successes += test_threshold("abc", "abc", 1, 0); ++total;
    successes += test_threshold("abc", "def", 2, 2); ++total;
    successes += test_threshold("abc", "", 4, 3); ++total;
    successes += test_threshold("abc", "abcdef", 1, 1); ++total;
    successes += test_threshold("abc", "defabc", 3, 3); ++total;
    successes += test_threshold("abc", "adbecf", 3, 3); ++total;

    printf(
        "\n-----------------------\n"
        "Passing: %d, failing: %d\n",
        successes, total - successes
    );
}
