#ifndef LINKED_LISTS_H
#define LINKED_LISTS_H

#include "res_or_err.h"
#include <errno.h>
#include <stdlib.h>

struct list_node {
    struct list_node *next;
    void *data;
};


// Create a new node containing `data`.
// If `prev` is not NULL, insert the new node immediately after `prev` in the list.
// Return a pointer to the created and inserted node or NULL on failure.
struct list_node *insert_after(struct list_node *prev, void *data);

// Remove and free the node after `prev` from the list.
// Do nothing if `prev` is the last element or is NULL.
void pop_next(struct list_node *prev);

// Return the first node for which the predicate returns a non-zero value
// when called with context as second argument.
// Return NULL if none was found.
struct list_node *search_first_node(int (*predicate)(struct list_node *, void *), void *context, struct list_node *head);

// Modify the list by replacing the data of each node
// with the result of transform applied to the node and context as second argument.
void map_list(struct list_node *head, void *(*transform)(void *, void *), void *context);

// Return the result of aggregate(node.data, aggregate(...), context)
// called recursively on each node, using initial as value for the second argument on the last node.
void *fold_list(struct list_node *head, void *(aggregate)(void *, void *, void *), void *initial, void *context);

// Return data without actually copying it.
void *shallow_data_copier(void *data);

// Can be used as disposer that does nothing when performing a shallow copy.
void no_op(void *arg);

// Return a deep copy of the list.
// disposer() is used on already copied data when the list copy fails.
// copy_list(head, shallow_data_copier, no_op) copies the list but not the data.
struct result_or_error copy_list(struct list_node *head, void *copy_data(void *), void (*disposer)(void *));

struct ordered_pair {
    struct list_node *left;
    struct list_node *right;
    int error;  // Non-zero for errors
};

// Return a pointer to a sorted copy of the list.
struct result_or_error linked_list_merge_sort(struct list_node *head, struct ordered_pair (*order)(struct list_node *, struct list_node *));

// Free all nodes and their data.
void free_list(struct list_node *head, void (*disposer)(void *));

#endif /* !LINKED_LISTS_H */
