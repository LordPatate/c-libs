#include "string_builder.h"

struct string_builder *new_string_builder() {
    struct string_builder *builder = malloc(sizeof(struct string_builder));
    if (builder) {
        *builder = (struct string_builder) {
            .count = 0,
            .last = NULL,
        };
    }
    return builder;
}

int append(char c, struct string_builder *builder) {
    struct char_list *new_head = malloc(sizeof(struct char_list));
    if (new_head) {
        *new_head = (struct char_list) {
            .prev = builder->last,
            .last = c,
        };
        builder->last = new_head;
        builder->count += 1;
        return STRING_BUILDER_OK;
    }
    return STRING_BUILDER_ALLOCATION_FAILED;
}

char *build_string(struct string_builder *builder) {
    size_t total_size = builder->count;
    char *array = malloc(total_size + 1);
    if (array) {
        size_t i = total_size;
        array[i] = '\0';
        --i;
        struct char_list *list;
        list = builder->last;
        while (list) {
            array[i] = list->last;
            --i;
            list = list->prev;
        }
    }
    return array;
}

static void free_char_list(struct char_list *list) {
    if (list) {
        free_char_list(list->prev);
        free(list);
    }
}

void free_string_builder(struct string_builder *builder) {
    free_char_list(builder->last);
    free(builder);
}
