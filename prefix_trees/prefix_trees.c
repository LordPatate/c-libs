#include "prefix_trees.h"

static struct node *_new_leaf(void *data) {
    struct node *leaf = malloc(sizeof(struct node));
    if (leaf != NULL) {
        for (int i = 0; i < PTREE_NB_CHILDREN; ++i) {
            leaf->children[i] = NULL;
        }
        leaf->data = data;
    }
    return leaf;
}

struct prefix_tree *empty_prefix_tree() {
    struct prefix_tree *tree = malloc(sizeof(struct prefix_tree));
    if (tree != NULL) {
        tree->root = _new_leaf(NULL);
    }
    return tree;
}

struct result_or_error _ptree_push(struct node *tree, char *key, void *data) {
    if (tree == NULL) {
        tree = _new_leaf(NULL);
        if (tree == NULL) {
            return ERROR(PTREE_ALLOCATION_FAILED);
        }
    }
    if (*key) {
        if (*key >= PTREE_NB_CHILDREN) return ERROR(PTREE_INVALID_CHAR);
        struct result_or_error push_res = _ptree_push(tree->children[*key], key + 1, data);
        if (push_res.ok == ERROR_TYPE) return push_res;
        tree->children[*key] = push_res.result;
    } else {
        tree->data = data;
    }
    return RESULT(tree);
}
enum ptree_error_code ptree_push(struct prefix_tree *tree, char *key, void *data) {
    struct result_or_error push_res = _ptree_push(tree->root, key, data);
    if (push_res.ok) {
        tree->root = push_res.result;
        return PTREE_OK;
    } else {
        return push_res.error;
    }
}

struct result_or_error _ptree_search(struct node *tree, char *key) {
    if (tree == NULL) {
        return ERROR(PTREE_KEY_NOT_FOUND);
    } else {
        if (*key) {
            if (*key >= PTREE_NB_CHILDREN) return ERROR(PTREE_INVALID_CHAR);
            return _ptree_search(tree->children[*key], key + 1);
        } else {
            return RESULT(tree->data);
        }
    }
}
struct result_or_error ptree_search(struct prefix_tree *tree, char *key) {
    return _ptree_search(tree->root, key);
}

void _ptree_map(struct node *tree, void *(*transform)(void *, void *), void *context) {
    if (tree != NULL) {
        for (int i = 0; i < PTREE_NB_CHILDREN; ++i) {
            _ptree_map(tree->children[i], transform, context);
        }
        if (tree->data != NULL)
            tree->data = transform(tree->data, context);
    }
}
void ptree_map(struct prefix_tree *tree, void *(*transform)(void *, void *), void *context) {
    _ptree_map(tree->root, transform, context);
}


void *_ptree_fold(struct node *tree, void *(*aggregate)(void *, void *, void *), void *initial, void *context) {
    if (tree == NULL) {
        return initial;
    } else {
        void *last_result;
        for (int i = 0; i < PTREE_NB_CHILDREN; ++i) {
            last_result = _ptree_fold(tree->children[i], aggregate, initial, context);
            last_result = aggregate(tree->data, last_result, context);
        }
        return last_result;
    }
}
void *ptree_fold(struct prefix_tree *tree, void *(aggregate)(void *, void *, void *), void *initial, void *context) {
    _ptree_fold(tree->root, aggregate, initial, context);
}


void _ptree_free(struct node *tree, void (*disposer)(void *)) {
    if (tree != NULL) {
        for (int i = 0; i < PTREE_NB_CHILDREN; ++i) {
            _ptree_free(tree->children[i], disposer);
        }
        if (tree->data) {
            disposer(tree->data);
        }
        free(tree);
    }
}
void ptree_free(struct prefix_tree *tree, void (*disposer)(void *)) {
    _ptree_free(tree->root, disposer);
    tree->root = NULL;
    free(tree);
}
