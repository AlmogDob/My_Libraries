#ifndef ALMOG_HUFFMAN_H_
#define ALMOG_HUFFMAN_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "Almog_Dynamic_Array.h"

struct Ah_Table_Element {
    uint8_t name;
    size_t count;
};

struct Ah_Table {
    size_t length;
    size_t capacity;
    struct Ah_Table_Element *elements;
};

struct Ah_Tree_Node {
    int name;
    size_t count;
    int left_index;
    int right_index;
};

struct Ah_Tree {
    size_t length;
    size_t capacity;
    struct Ah_Tree_Node *elements;
};

#ifndef AH_ASSERT
#include <assert.h>
#define AH_ASSERT assert
#endif

#ifndef AH_CALLOC
#include <stdlib.h>
#define AH_CALLOC calloc
#endif

#ifndef AH_FREE
#include <stdlib.h>
#define AH_FREE free
#endif

#define AH_DEF static inline
#define AH_UNUSED(x) ((void)x)
#define ah_dprintSTRING(expr) printf("[Info] %s:%d:\n" #expr " = %s\n", __FILE__, __LINE__, expr)
#define ah_dprintCHAR(expr) printf("[Info] %s:%d:\n" #expr " = %c\n", __FILE__, __LINE__, expr)
#define ah_dprintINT(expr) printf("[Info] %s:%d:\n" #expr " = %d\n", __FILE__, __LINE__, expr)
#define ah_dprintFLOAT(expr) printf("[Info] %s:%d:\n" #expr " = %#g\n", __FILE__, __LINE__, expr)
#define ah_dprintDOUBLE(expr) printf("[Info] %s:%d:\n" #expr " = %#g\n", __FILE__, __LINE__, expr)
#define ah_dprintSIZE_T(expr) printf("[Info] %s:%d:\n" #expr " = %zu\n", __FILE__, __LINE__, expr)
#define ah_dprintINFO(fmt, ...) \
    fprintf(stderr, "[Info] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 7, "", __func__, 7, "", __VA_ARGS__)
#define ah_dprintWARNING(fmt, ...) \
    fprintf(stderr, "[Warning] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 10, "", __func__, 10, "", __VA_ARGS__)
#define ah_dprintERROR(fmt, ...) \
    fprintf(stderr, "[Error] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 8, "", __func__, 8, "", __VA_ARGS__)

AH_DEF void                 ah_qsort(struct Ah_Table_Element v[], int left, int right);
AH_DEF void                 ah_swap(struct Ah_Table_Element v[], int i, int j);
AH_DEF void                 ah_table_element_print(struct Ah_Table_Element e);
AH_DEF int                  ah_table_find_name(struct Ah_Table table, uint8_t name);
AH_DEF void                 ah_table_print(struct Ah_Table table);
AH_DEF int                  ah_tree_find_node(struct Ah_Tree tree, struct Ah_Tree_Node node);
AH_DEF struct Ah_Tree_Node  ah_tree_node_create_from_table_element(struct Ah_Table_Element e);
AH_DEF struct Ah_Tree_Node  ah_tree_nodes_join(struct Ah_Tree tree, size_t a_index, size_t b_index);
AH_DEF void                 ah_tree_node_print(struct Ah_Tree_Node t);
AH_DEF struct Ah_Tree_Node  ah_tree_pop(struct Ah_Tree *tree);
AH_DEF void                 ah_tree_print(struct Ah_Tree t);
AH_DEF void                 ah_tree_print_ascii(const struct Ah_Tree *tree);
AH_DEF void                 ah_tree_print_ascii_from(const struct Ah_Tree *tree, int root_index);
AH_DEF void                 ah_tree_print_ascii_node_label(struct Ah_Tree_Node node);
AH_DEF void                 ah_tree_print_ascii_rec(const struct Ah_Tree *tree, int node_index, bool *has_more, int depth, bool is_left, bool is_root);
AH_DEF void                 ah_tree_push(struct Ah_Tree *tree, struct Ah_Tree_Node node);

#endif /*ALMOG_HUFFMAN_H_*/

#ifdef ALMOG_HUFFMAN_IMPLEMENTATION
#undef ALMOG_HUFFMAN_IMPLEMENTATION

AH_DEF void ah_qsort(struct Ah_Table_Element v[], int left, int right)
{
    int i, last;

    if (left >= right)                  /* do nothing if array contains */
        return;                         /* fewer than two elements */
    ah_swap(v, left, (left + right) / 2);  /* move partition elem */
    last = left;                        /* to v[0] */
    for (i = left + 1; i <= right; i++) /* partition */
        if (v[i].count > v[left].count)
            ah_swap(v, ++last, i);
    ah_swap(v, left, last); /* restore partition elem */
    ah_qsort(v, left, last - 1);
    ah_qsort(v, last + 1, right);
}

AH_DEF void ah_swap(struct Ah_Table_Element v[], int i, int j)
{
    struct Ah_Table_Element temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

AH_DEF void ah_table_element_print(struct Ah_Table_Element e)
{
    printf("name: 0x%02x | count: %zu\n", e.name, e.count);
}

AH_DEF int ah_table_find_name(struct Ah_Table table, uint8_t name)
{
    if (table.length == 0) return -1;

    for (size_t i = 0; i < table.length; i++) {
        if (table.elements[i].name == name) {
            return (int)i;
        }
    }

    return -1;
}

AH_DEF void ah_table_print(struct Ah_Table table)
{
    for (size_t i = 0; i < table.length; i++) {
        printf("%3zu: ", i);
        ah_table_element_print(table.elements[i]);
    }
}

AH_DEF int ah_tree_find_node(struct Ah_Tree tree, struct Ah_Tree_Node node)
{
    for (size_t i = 0; i < tree.length; i++) {
        struct Ah_Tree_Node current_node = tree.elements[i];
        if (current_node.count == node.count &&
            current_node.left_index == node.left_index &&
            current_node.name == node.name &&
            current_node.right_index == node.right_index) {
                return (int)i;
            }
    }

    return -1;
}

AH_DEF struct Ah_Tree_Node ah_tree_node_create_from_table_element(struct Ah_Table_Element e)
{
    return (struct Ah_Tree_Node){
        .count = e.count,
        .name = e.name,
        .left_index = -1,
        .right_index = -1
    };
}

AH_DEF struct Ah_Tree_Node ah_tree_nodes_join(struct Ah_Tree tree, size_t a_index, size_t b_index)
{
    AH_ASSERT(a_index >= 0 && (size_t)a_index < tree.length);
    AH_ASSERT(b_index >= 0 && (size_t)b_index < tree.length);

    return (struct Ah_Tree_Node){
        .count = tree.elements[a_index].count + tree.elements[b_index].count,
        .name = -1,
        .left_index = (int)a_index,
        .right_index = (int)b_index,
    };
}

AH_DEF void ah_tree_node_print(struct Ah_Tree_Node t)
{
    if (isprint(t.name) && t.name != '\\' && t.name != '\'') {
        printf("name: '%c' | count: %3zu | (%3d,%3d)\n", t.name, t.count, t.left_index, t.right_index);
    } else {
        printf("name: %3d | count: %3zu | (%3d,%3d)\n", t.name, t.count, t.left_index, t.right_index);
    }
}

AH_DEF struct Ah_Tree_Node ah_tree_pop(struct Ah_Tree *tree)
{
    AH_ASSERT(tree != NULL);
    AH_ASSERT(tree->length > 0 && "ah_tree_pop on empty tree");
    return tree->elements[--tree->length];
}

AH_DEF void ah_tree_print(struct Ah_Tree t)
{
    for (size_t i = 0; i < t.length; i++) {
        printf("%3zu: ", i);
        ah_tree_node_print(t.elements[i]);
    }
}

AH_DEF void ah_tree_print_ascii(const struct Ah_Tree *tree)
{
    if (tree == NULL || tree->length == 0) {
        printf("(empty tree)\n");
        return;
    }

    ah_tree_print_ascii_from(tree, (int)tree->length - 1);
}

AH_DEF void ah_tree_print_ascii_from(const struct Ah_Tree *tree, int root_index)
{
    if (tree == NULL || tree->length == 0) {
        printf("(empty tree)\n");
        return;
    }

    if (root_index < 0 || (size_t)root_index >= tree->length) {
        printf("(invalid root index: %d)\n", root_index);
        return;
    }

    bool *has_more = AH_CALLOC(tree->length + 1, sizeof(bool));
    if (has_more == NULL) {
        printf("(memory allocation failed)\n");
        return;
    }

    ah_tree_print_ascii_rec(tree, root_index, has_more, 0, false, true);

    AH_FREE(has_more);
}

AH_DEF void ah_tree_print_ascii_node_label(struct Ah_Tree_Node node)
{
    if (node.name == -1) {
        printf("[* count=%zu]\n", node.count);
        return;
    }

    unsigned char ch = (unsigned char)node.name;

    if (isprint(ch) && ch != '\\' && ch != '\'') {
        printf("['%c' count=%zu]\n", ch, node.count);
    } else {
        printf("[0x%02X count=%zu]\n", ch, node.count);
    }
}

AH_DEF void ah_tree_print_ascii_rec(const struct Ah_Tree *tree, int node_index, bool *has_more, int depth, bool is_left, bool is_root)
{
    if (node_index < 0) {
        return;
    }

    struct Ah_Tree_Node node = tree->elements[node_index];

    for (int i = 0; i < depth; i++) {
        printf("%s", has_more[i] ? "|   " : "    ");
    }

    if (!is_root) {
        printf("%s", is_left ? "\b\b\b\b|-- " : "\b\b\b\b\\-- ");
    }

    ah_tree_print_ascii_node_label(node);

    bool has_left = node.left_index != -1;
    bool has_right = node.right_index != -1;

    if (has_left) {
        has_more[depth] = has_right;
        ah_tree_print_ascii_rec(
            tree,
            node.left_index,
            has_more,
            depth + 1,
            true,
            false
        );
    }

    if (has_right) {
        has_more[depth] = false;
        ah_tree_print_ascii_rec(
            tree,
            node.right_index,
            has_more,
            depth + 1,
            false,
            false
        );
    }
}

AH_DEF void ah_tree_push(struct Ah_Tree *tree, struct Ah_Tree_Node node)
{
    /* If too slow, might want to implement min-heap algorithm */
    size_t i = 0;

    while (i < tree->length && tree->elements[i].count > node.count) {
        i++;
    }

    while (i < tree->length && tree->elements[i].count == node.count) {
        i++;
    }

    if (i == tree->length) {
        ada_appand(struct Ah_Tree_Node, *tree, node);
    } else {
        ada_insert(struct Ah_Tree_Node, *tree, node, (int)i);
    }
}



#endif /*ALMOG_HUFFMAN_IMPLEMENTATION*/
