#define ALMOG_HUFFMAN_IMPLEMENTATION
#include "./include/Almog_Huffman.h"

struct Ah_Code {
    uint8_t name;
    uint8_t curser;
    char code[256];
};

struct Ah_Code_Table {
    size_t length;
    size_t capacity;
    struct Ah_Code *elements;
};

AH_DEF void ah_code_print(struct Ah_Code code) 
{
    if (isprint(code.name) && code.name != '\\' && code.name != '\'') {
        printf("name: '%c' -> code: %s\n", code.name, code.code);
    } else {
        printf("name: 0x%02x -> code: %s\n", code.name, code.code);
    }
}

AH_DEF void ah_code_table_print(struct Ah_Code_Table t)
{
    for (size_t i = 0; i < t.length; i++) {
        printf("%zu: ", i);
        ah_code_print(t.elements[i]);
    }
}

AH_DEF bool ah_code_create_from_tree_leaf(struct Ah_Tree tree, int current_node_index, size_t leaf_index, struct Ah_Code *code)
{
    AH_ASSERT(tree.elements[leaf_index].left_index == -1 && "not a leaf");
    AH_ASSERT(tree.elements[leaf_index].right_index == -1 && "not a leaf");

    if (current_node_index < 0 || (size_t)current_node_index >= tree.length) {
        return false;
    }
    
    printf("%3d: ", current_node_index);
    ah_tree_node_print(tree.elements[current_node_index]);

    if (current_node_index == leaf_index) {
        code->name = (uint8_t)tree.elements[leaf_index].name;
        code->code[code->curser] = '\0';
        return true;
    } else if (tree.elements[current_node_index].left_index == -1 && tree.elements[current_node_index].right_index == -1) {
        code->code[code->curser] = '\0';
        ah_dprintSTRING(code->code);
        return false;
    }

    code->code[code->curser++] = '0'; 
    if (!ah_code_create_from_tree_leaf(tree, tree.elements[current_node_index].left_index, leaf_index, code)) {
        code->curser--;
        code->code[code->curser++] = '1';
    } else {
        return true;
    }

    if (!ah_code_create_from_tree_leaf(tree, tree.elements[current_node_index].right_index, leaf_index, code)) {
        code->curser--;
        return false;
    } else {
        return true;
    }
}

AH_DEF void ah_code_table_create(struct Ah_Tree tree, struct Ah_Code_Table *table, struct Ah_Code *buff, int current_node_index)
{
    AH_ASSERT(current_node_index >= 0 || (size_t)current_node_index < tree.length);
    // printf("%3zu: ", current_node_index);
    // ah_tree_node_print(tree.elements[current_node_index]);

    if (tree.elements[current_node_index].left_index == -1 && tree.elements[current_node_index].right_index == -1) {
        buff->name = (uint8_t)tree.elements[current_node_index].name;
        if (buff->curser == 0) {
            buff->code[buff->curser++] = '0';
        }
        buff->code[buff->curser] = '\0';
        // ah_dprintSTRING(buff->code);
        ada_appand(struct Ah_Code, *table, *buff);
        // ah_code_table_print(*table);
        return;
    }

    buff->code[buff->curser++] = '0'; 
    ah_code_table_create(tree, table, buff, tree.elements[current_node_index].left_index);
    buff->curser--;
    buff->code[buff->curser++] = '1';
    ah_code_table_create(tree, table, buff, tree.elements[current_node_index].right_index);
    buff->curser--;
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
    if (table.length == 0) {
        ah_dprintERROR("%s", "Empty input file.");
        return 0;
    }

    ah_qsort(table.elements, 0, (int)table.length-1);

    ah_table_print(table);

    struct Ah_Tree tree = {0};
    struct Ah_Tree priority_queue = {0};
    ada_init_array(struct Ah_Tree_Node, tree);
    ada_init_array(struct Ah_Tree_Node, priority_queue);
    
    for (size_t i = 0; i < table.length; i++) {
        ada_appand(struct Ah_Tree_Node, tree, ah_tree_node_create_from_table_element(table.elements[i]));
        ada_appand(struct Ah_Tree_Node, priority_queue, ah_tree_node_create_from_table_element(table.elements[i]));
    }

    for ( ; priority_queue.length > 1 ;) {
        struct Ah_Tree_Node a = ah_tree_pop(&priority_queue);
        struct Ah_Tree_Node b = ah_tree_pop(&priority_queue);
        struct Ah_Tree_Node new_node = ah_tree_nodes_join(tree, ah_tree_find_node(tree, a), ah_tree_find_node(tree, b));
        ada_appand(struct Ah_Tree_Node, tree, new_node);
        ah_tree_push(&priority_queue, new_node);
    }

    printf("##############################\n");

    // ah_tree_print(tree);
    // ah_tree_print_ascii(&tree);

    struct Ah_Code buff = {0};
    struct Ah_Code_Table code_table = {0};
    ada_init_array(struct Ah_Code, code_table);
    ah_code_table_create(tree, &code_table, &buff, (int)tree.length-1);

    ah_code_table_print(code_table); 

    return 0;
}
