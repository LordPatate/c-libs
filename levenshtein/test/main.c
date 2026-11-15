#include "levenshtein.h"
#include <stdio.h>

#define DIFF(a, b) ((a) > (b) ? (a) - (b) : (b) - (a))

void test(const char *ref, const char *compared) {
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
            "\n", ref, compared, d, r1, r2);
    } else {
        printf("Levenshtein failed (exit status %d).\n", res.error);
    }
}

void test_threshold(const char *ref, const char *compared, size_t threshold) {
    struct result_or_error res = partial_match(ref, compared, threshold);
    if (res.ok) {
        size_t d = res.result.as_ullong;
        if (d == threshold) {
            printf("not a match: %s - %s (threshold = %zu)\n", ref, compared, threshold);
        } else {
            printf("partial match: %s - %s (distance = %zu)\n", ref, compared, d);
        }
    } else {
        printf("Levenshtein failed (exit status %d).\n", res.error);
    }
}

int main(void) {
    test("abc", "abc");

    test("abc", "abd");
    test("abc", "adc");
    test("abc", "dbc");

    test("abc", "ab");
    test("abc", "ac");
    test("abc", "bc");

    test("abc", "abcd");
    test("abc", "abdc");
    test("abc", "adbc");
    test("abc", "dabc");

    test("abc", "def");

    test_threshold("abc", "abc", 1);
    test_threshold("abc", "abcd", 1);
    test_threshold("abc", "abcd", 2);
    test_threshold("abc", "def", 3);
    test_threshold("abc", "def", 4);
}
