#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <stdlib.h>
#include <errno.h>

enum string_builder_error {
    STRING_BUILDER_OK,
    STRING_BUILDER_ALLOCATION_FAILED = ENOMEM,
};

struct string_builder;

struct string_builder *new_string_builder();
int append(char c, struct string_builder *builder);
// Allocate an null-terminated string containing a copy of each characters
// appended to the string builder, in the same order.
char *build_string(struct string_builder *builder);
void free_string_builder(struct string_builder *builder);

#include "string_builder_internals.h"

#endif // !STRING_BUILDER_H
