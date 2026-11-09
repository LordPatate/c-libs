#ifndef PREFIX_TREES_H
#define PREFIX_TREES_H

#include <stdlib.h>

#include "res_or_err.h"

enum ptree_nb_children {PTREE_NB_CHILDREN = 128};

enum ptree_error_code {
    PTREE_OK,
    PTREE_ALLOCATION_FAILED,
    PTREE_INVALID_CHAR,
    PTREE_KEY_NOT_FOUND,
};

struct prefix_tree;

// Return a new empty prefix tree or NULL if allocation failed.
struct prefix_tree *empty_prefix_tree();

// Add the key to the tree with data associated.
// Return PTREE_OK on success or an appropriate error code otherwise.
enum ptree_error_code ptree_push(struct prefix_tree *tree, char *key, void *data);

// Return a pointer to the data associated with the key in the tree,
// or an appropriate error code in case of failure.
struct result_or_error ptree_search(struct prefix_tree *tree, char *key);

// Modify the tree by replacing the data of each non-empty node
// with the result of transform applied to the node and context as second argument.
void ptree_map(struct prefix_tree *tree, void *(*transform)(void *, void *), void *context);

// Return the result of aggregate(node.data, aggregate(...), context)
// called recursively on each node, using initial as return value for empty nodes.
void *ptree_fold(struct prefix_tree *tree, void *(aggregate)(void *, void *, void *), void *initial, void *context);

// Free the data of each node and the nodes themselves.
void ptree_free(struct prefix_tree *tree, void (*disposer)(void *));

#include "prefix_trees_internals.h"

#endif  // !PREFIX_TREES_H
