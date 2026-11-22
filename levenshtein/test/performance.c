#include "levenshtein.h"
#include <stdio.h>
#include <errno.h>

char *build_string(char *pattern, size_t size) {
    size_t sizeof_pattern = strlen(pattern);
    size_t repeat = size / sizeof_pattern;
    char *s = malloc(sizeof (char) * (size + 1));
    if (s) {
        for (size_t i = 0; i < repeat; ++i) {
            for (size_t j = 0; j < sizeof_pattern; ++j) {
                s[i * sizeof_pattern + j] = pattern[j];
            }
        }
    }
    return s;
}

int main(int argc, char **argv) {
    size_t size = 10000, threshold = 0;
    if (argc - 1 >= 1) {
        errno = 0;
        size = strtoul(argv[1], NULL, 10);
        if (errno != 0) { perror("strtoul"); return 1; }
        if (argc - 1 >= 2) {
            errno = 0;
            threshold = strtoul(argv[2], NULL, 10);
            if (errno != 0) { perror("strtoul"); return 1; }
        }
    }
    int exit_status = 0;
    printf("size: %lu\n", size);
    char *str1 = build_string("aaaab", size);
    printf("str1 (first 20): %.20s\n", str1);
    char *str2 = build_string("bbbbb", size);
    printf("str2 (first 20): %.20s\n", str2);
    if (str1 == NULL || str2 == NULL) { exit_status = 1; goto exit; }
    size_t d;
    if (threshold > 0) {
        printf("threshold: %lu\n", threshold);
        d = partial_match_sized(str1, size, str2, size, threshold);
    } else {
        struct result_or_error res;
        res = levenshtein_sized(str1, size, str2, size);
        if (res.ok) {
            d = res.result.as_ullong;
        } else { errno = res.error; perror("levenshtein"); exit_status = 1; }
    }
    printf("%lu\n", d);
exit:
    free(str1);
    free(str2);
    return exit_status;
}
