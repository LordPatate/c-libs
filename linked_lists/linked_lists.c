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

void *shallow_data_copier(void *data) { return data; }

void no_op(void *arg) { (void) arg; }

static struct list_node *copy_node(struct list_node *node, void *copy_data(void *), void (*disposer)(void *)) {
    void *data_copy = copy_data(node->data);
    if (!data_copy) return 0;
    struct list_node dummy = { 0 };
    struct list_node *node_copy = insert_after(&dummy, data_copy);
    if (!node_copy) { disposer(data_copy); }
    return node_copy;
}

struct result_or_error copy_list(struct list_node *head, void *copy_data(void *), void (*disposer)(void *)) {
    if (!head) return PTR_RESULT(head);
    struct result_or_error r = copy_list(head->next, copy_data, disposer);
    if (!r.ok) return r;
    struct list_node *head_copy = copy_node(head, copy_data, disposer);
    if (!head_copy) { free_list(r.result.as_ptr, disposer); return ERROR(ENOMEM); };
    head_copy->next = r.result.as_ptr;
    return PTR_RESULT(head_copy);
}

static struct ordered_pair split(struct list_node *head) {
    struct ordered_pair p = { 0 };
    if (!head) return p;
    p.right = copy_node(head, shallow_data_copier, no_op);
    if (!p.right) { p.error = ENOMEM; return p; }
    if (!head->next) return p;
    p.left = copy_node(head->next, shallow_data_copier, no_op);
    if (!p.left) { free(p.right); p.right = 0; p.error = ENOMEM; return p; }
    struct ordered_pair tails = split(head->next->next);
    if (tails.error) { free(p.right); free(p.left); return tails; }
    p.right->next = tails.right;
    p.left->next = tails.left;
    return p;
}

static struct result_or_error merge(
    struct list_node *left, struct list_node *right,
    struct ordered_pair (*order)(struct list_node *, struct list_node *)
) {
    struct list_node *copy;
    if (!left) { return PTR_RESULT(right); }
    if (!right) return PTR_RESULT(left);
    struct ordered_pair p = order(left, right);
    if (p.error) return ERROR(p.error);
    struct result_or_error r = merge(p.left->next, p.right, order);
    if (!r.ok) { return r; }
    p.left->next = r.result.as_ptr;
    return PTR_RESULT(p.left);
}

struct result_or_error linked_list_merge_sort(struct list_node *head, struct ordered_pair (*order)(struct list_node *, struct list_node *)) {
    if (!head) return PTR_RESULT(head);
    if (!head->next) {
        struct list_node *copy = copy_node(head, shallow_data_copier, no_op);
        if (copy) return PTR_RESULT(copy);
        else return ERROR(ENOMEM);
    }
    struct ordered_pair p = split(head);
    if (p.error) { return ERROR(p.error); };
    struct result_or_error r;
    r = linked_list_merge_sort(p.left, order);
    if (r.ok) {
        struct list_node *left = r.result.as_ptr;
        r = linked_list_merge_sort(p.right, order);
        if (r.ok) {
            struct list_node *right = r.result.as_ptr;
            r = merge(left, right, order);
            free_list(right, no_op);
        }
        free_list(left, no_op);
    }
    free_list(p.left, no_op);
    free_list(p.right, no_op);
    return r;
}

void free_list(struct list_node *head, void (*disposer)(void *)) {
    if (head) {
        free_list(head->next, disposer);
        disposer(head->data);
        free(head);
    }
}
