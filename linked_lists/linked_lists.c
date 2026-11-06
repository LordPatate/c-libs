#include "linked_lists.h"

struct list_node *insert_after(struct list_node *prev, void *data) {
    struct list_node *node = malloc(sizeof (struct list_node));
    if (node) {
        node->data = data;
        if (prev) {
            node->next = prev->next;
            prev->next = node;
        } else { // prev == NULL
            node->next = NULL;
        }
    }
    return node;
}

void pop_next(struct list_node *prev) {
    if (prev == NULL) return;
    struct list_node *node = prev->next;
    if (node) {
        prev->next = node->next;
        free(node);
    }
}

struct list_node *search_first_node(int (*predicate)(struct list_node *, void *), void *context, struct list_node *head) {
    struct list_node *node = head;
    while (node && predicate(node, context) == 0) {
        node = node->next;
    }
    return node;
}

void map_list(struct list_node *head, void *(*transform)(void *, void *), void *context) {
    if (head != NULL) {
        map_list(head->next, transform, context);
        head->data = transform(head->data, context);
    }
}

void *fold_list(struct list_node *head, void *(aggregate)(void *, void *, void *), void *initial, void *context) {
    if (head == NULL) {
        return initial;
    } else {
        void *last_result = fold_list(head->next, aggregate, initial, context);
        return aggregate(head->data, last_result, context);
    }
}

void free_list(struct list_node *head, void (*disposer)(void *)) {
    if (head) {
        free_list(head->next, disposer);
        disposer(head->data);
        free(head);
    }
}
