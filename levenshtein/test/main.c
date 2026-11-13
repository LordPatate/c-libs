#include "levenshtein.h"
#include <stdio.h>

#define DIFF(a, b) ((a) > (b) ? (a) - (b) : (b) - (a))

void test(const char *ref, const char *compared) {
    struct result_or_error res = levenshtein(ref, compared);
    if (res.ok) {
        size_t d = (size_t) res.result;
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
}
