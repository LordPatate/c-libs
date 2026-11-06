#ifndef STRING_BUILDER_INTERNALS_H
#define STRING_BUILDER_INTERNALS_H

struct char_list {
    struct char_list *prev;
    char last;
};

struct string_builder {
    size_t count;
    struct char_list *last;
};

#endif // !STRING_BUILDER_INTERNALS_H
