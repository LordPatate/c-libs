#ifndef PREFIX_TREES_INTERNALS_H
#define PREFIX_TREES_INTERNALS_H

#include "prefix_trees.h"

struct node {
    struct node *children[PTREE_NB_CHILDREN];
    void *data;
};

struct prefix_tree {
    struct node *root;
};

#endif // !PREFIX_TREES_INTERNALS_H
