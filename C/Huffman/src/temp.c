#define ALMOG_HUFFMAN_IMPLEMENTATION
#include "./include/Almog_Huffman.h"

struct Ah_Tree_Node ah_tree_node_create_from_table_element(struct Ah_Table_Element e)
{
    return (struct Ah_Tree_Node){
        .count = e.count,
        .name = e.name,
        .left_index = -1,
        .right_index = -1
    };
}

struct Ah_Tree_Node ah_tree_nodes_join(struct Ah_Tree tree, size_t a_index, size_t b_index)
{
    return (struct Ah_Tree_Node){
        .count = tree.elements[a_index].count + tree.elements[b_index].count,
        .name = -1,
        .left_index = (int)a_index,
        .right_index = (int)b_index,
    };
}

void ah_tree_node_print(struct Ah_Tree_Node t)
{
    printf("name: %4d | count: %3zu | (%3d,%3d)\n", t.name, t.count, t.left_index, t.right_index);
}

void ah_tree_print(struct Ah_Tree t)
{
    for (size_t i = 0; i < t.length; i++) {
        printf("%3zu: ", i);
        ah_tree_node_print(t.elements[i]);
    }
}

struct Ah_Tree_Node ah_tree_pop(struct Ah_Tree *tree)
{
    return tree->elements[--tree->length];
}

void ah_tree_push(struct Ah_Tree *tree, struct Ah_Tree_Node node)
{
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

int ah_tree_find_node(struct Ah_Tree tree, struct Ah_Tree_Node node)
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

static void ah_tree_print_ascii_node_label(struct Ah_Tree_Node node)
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

static void ah_tree_print_ascii_rec(
    const struct Ah_Tree *tree,
    int node_index,
    bool *has_more,
    int depth,
    bool is_left,
    bool is_root
)
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

void ah_tree_print_ascii_from(const struct Ah_Tree *tree, int root_index)
{
    if (tree == NULL || tree->length == 0) {
        printf("(empty tree)\n");
        return;
    }

    if (root_index < 0 || (size_t)root_index >= tree->length) {
        printf("(invalid root index: %d)\n", root_index);
        return;
    }

    bool *has_more = calloc(tree->length + 1, sizeof(bool));
    if (has_more == NULL) {
        printf("(memory allocation failed)\n");
        return;
    }

    ah_tree_print_ascii_rec(tree, root_index, has_more, 0, false, true);

    free(has_more);
}

void ah_tree_print_ascii(const struct Ah_Tree *tree)
{
    if (tree == NULL || tree->length == 0) {
        printf("(empty tree)\n");
        return;
    }

    ah_tree_print_ascii_from(tree, (int)tree->length - 1);
}

int main(void)
{
    
    FILE *fp = fopen("../input.txt", "rt");
    // FILE *fp = fopen("../src/temp.c", "rt");
    if (fp == NULL) {
        ah_dprintERROR("%s", "unable to open file.");
        return -1;
    }
    
    struct Ah_Table table = {0};
    ada_init_array(struct Ah_Table_Element, table);

    for (int c = fgetc(fp); c != EOF; c = fgetc(fp)) {
        int rv = ah_table_find_name(table, (uint8_t)c);
        if (rv == -1) {
            struct Ah_Table_Element e = {
                .name = (uint8_t)c,
                .count = 1,
            };
            ada_appand(struct Ah_Table_Element, table, e);
        } else {
            table.elements[rv].count++;
        }
    }
    printf("\n");

    ah_qsort(table.elements, 0, (int)table.length-1);

    ah_table_print(table);

    printf("---------------------------------\n");

    struct Ah_Tree tree = {0};
    struct Ah_Tree priority_queue = {0};
    ada_init_array(struct Ah_Tree_Node, tree);
    ada_init_array(struct Ah_Tree_Node, priority_queue);
    
    for (size_t i = 0; i < table.length; i++) {
        ada_appand(struct Ah_Tree_Node, tree, ah_tree_node_create_from_table_element(table.elements[i]));
        ada_appand(struct Ah_Tree_Node, priority_queue, ah_tree_node_create_from_table_element(table.elements[i]));
    }

    ah_tree_print(priority_queue);
    
    for ( ; priority_queue.length > 1 ;) {
        struct Ah_Tree_Node a = ah_tree_pop(&priority_queue);
        struct Ah_Tree_Node b = ah_tree_pop(&priority_queue);
        struct Ah_Tree_Node new_node = ah_tree_nodes_join(tree, ah_tree_find_node(tree, a), ah_tree_find_node(tree, b));
        ada_appand(struct Ah_Tree_Node, tree, new_node);
        ah_tree_push(&priority_queue, new_node);
    }

    printf("##############################\n");

    ah_tree_print_ascii(&tree);

    return 0;
}
