#ifndef ALMOG_HUFFMAN_H_
#define ALMOG_HUFFMAN_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
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
    int left_index;
    int right_index;
    size_t count;
};

struct Ah_Tree {
    size_t length;
    size_t capacity;
    struct Ah_Tree_Node *elements;
};

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

struct Ah_Bit_Writer {
    FILE *fp;
    uint8_t current_byte;
    uint8_t bit_count;
};

struct Ah_Bit_Reader {
    FILE *fp;
    uint8_t current_byte;
    uint8_t bits_left;
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

#ifndef AH_DEF
    #ifdef AH_DEF_STATIC
        #define AH_DEF static
    #else
        #define AH_DEF extern
    #endif
#endif
#define AH_UNUSED(x) ((void)x)
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

AH_DEF int                  ah_bit_reader_read_bit(struct Ah_Bit_Reader *br);
AH_DEF void                 ah_bit_reader_init(struct Ah_Bit_Reader *br, FILE *fp);
AH_DEF void                 ah_bit_writer_flush(struct Ah_Bit_Writer *bw);
AH_DEF void                 ah_bit_writer_init(struct Ah_Bit_Writer *bw, FILE *fp);
AH_DEF void                 ah_bit_writer_write_bit(struct Ah_Bit_Writer *bw, uint8_t bit);
AH_DEF void                 ah_bit_writer_write_code(struct Ah_Bit_Writer *bw, const char *code);
AH_DEF bool                 ah_code_create_from_tree_leaf(struct Ah_Tree tree, int current_node_index, size_t leaf_index, struct Ah_Code *code);
AH_DEF void                 ah_code_print(struct Ah_Code code);
AH_DEF struct Ah_Code_Table ah_code_table_create(struct Ah_Tree tree);
AH_DEF void                 ah_code_table_create_internal(struct Ah_Tree tree, struct Ah_Code_Table *table, struct Ah_Code *buff, int current_node_index);
AH_DEF int                  ah_code_table_get_index_by_name(struct Ah_Code_Table table, uint8_t name);
AH_DEF void                 ah_code_table_print(struct Ah_Code_Table t);
AH_DEF void                 ah_decode_file(const char *file_path);
AH_DEF void                 ah_encode_file(const char *file_path);
AH_DEF void                 ah_qsort(struct Ah_Table_Element v[], int left, int right);
AH_DEF void                 ah_swap(struct Ah_Table_Element v[], int i, int j);
AH_DEF struct Ah_Table      ah_table_create(const char *file_path, size_t *amount_of_bytes);
AH_DEF void                 ah_table_element_print(struct Ah_Table_Element e);
AH_DEF int                  ah_table_find_name(struct Ah_Table table, uint8_t name);
AH_DEF void                 ah_table_print(struct Ah_Table table);
AH_DEF struct Ah_Tree       ah_tree_create_from_table(struct Ah_Table table);
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

AH_DEF int ah_bit_reader_read_bit(struct Ah_Bit_Reader *br)
{
    if (br->bits_left == 0) {
        int c = fgetc(br->fp);
        if (c == EOF) {
            return -1;
        }
        br->current_byte = (uint8_t)c;
        br->bits_left = 8;
    }

    int bit = (br->current_byte & 0x80) ? 1 : 0;
    br->current_byte <<= 1;
    br->bits_left--;
    return bit;
}

AH_DEF void ah_bit_reader_init(struct Ah_Bit_Reader *br, FILE *fp)
{
    br->fp = fp;
    br->current_byte = 0;
    br->bits_left = 0;
}

AH_DEF void ah_bit_writer_flush(struct Ah_Bit_Writer *bw)
{
    if (bw->bit_count > 0) {
        bw->current_byte <<= (8 - bw->bit_count);
        fputc(bw->current_byte, bw->fp);
        bw->current_byte = 0;
        bw->bit_count = 0;
    }
}

AH_DEF void ah_bit_writer_init(struct Ah_Bit_Writer *bw, FILE *fp)
{
    bw->fp = fp;
    bw->current_byte = 0;
    bw->bit_count = 0;
}

AH_DEF void ah_bit_writer_write_bit(struct Ah_Bit_Writer *bw, uint8_t bit)
{
    bw->current_byte <<= 1;
    bw->current_byte |= (bit & 1);
    bw->bit_count++;

    if (bw->bit_count == 8) {
        ah_bit_writer_flush(bw);
    }
}

AH_DEF void ah_bit_writer_write_code(struct Ah_Bit_Writer *bw, const char *code)
{
    for (size_t i = 0; code[i] != '\0'; i++) {
        ah_bit_writer_write_bit(bw, code[i] == '1');
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

AH_DEF void ah_code_print(struct Ah_Code code)
{
    if (isprint(code.name) && code.name != '\\' && code.name != '\'') {
        printf("name: '%2c  ' -> code: %s\n", code.name, code.code);
    } else {
        printf("name: '0x%02x' -> code: %s\n", code.name, code.code);
    }
}

AH_DEF struct Ah_Code_Table ah_code_table_create(struct Ah_Tree tree)
{
    struct Ah_Code buff = {0};
    struct Ah_Code_Table code_table = {0};
    ada_init_array(struct Ah_Code, code_table);
    ah_code_table_create_internal(tree, &code_table, &buff, (int)tree.length-1);

    return code_table;
}

AH_DEF void ah_code_table_create_internal(struct Ah_Tree tree, struct Ah_Code_Table *table, struct Ah_Code *buff, int current_node_index)
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
    ah_code_table_create_internal(tree, table, buff, tree.elements[current_node_index].left_index);
    buff->curser--;
    buff->code[buff->curser++] = '1';
    ah_code_table_create_internal(tree, table, buff, tree.elements[current_node_index].right_index);
    buff->curser--;
}

AH_DEF int ah_code_table_get_index_by_name(struct Ah_Code_Table table, uint8_t name)
{
    for (size_t i = 0; i < table.length; i++) {
        if (table.elements[i].name == name) {
            return (int)i;
        }
    }

    return -1;
}

AH_DEF void ah_code_table_print(struct Ah_Code_Table t)
{
    for (size_t i = 0; i < t.length; i++) {
        printf("%3zu: ", i);
        ah_code_print(t.elements[i]);
    }
}

AH_DEF void ah_decode_file(const char *file_path)
{
    FILE *in = fopen(file_path, "rb");
    if (in == NULL) {
        ah_dprintERROR("Unable to open file '%s'.", file_path);
        exit(1);
    }

    uint32_t amount_of_bytes = 0;
    uint32_t amount_of_tree_nodes = 0;

    if (fread(&amount_of_bytes, sizeof(amount_of_bytes), 1, in) != 1) {
        ah_dprintERROR("%s", "Failed to read original byte count.");
        fclose(in);
        exit(1);
    }

    if (fread(&amount_of_tree_nodes, sizeof(amount_of_tree_nodes), 1, in) != 1) {
        ah_dprintERROR("%s", "Failed to read tree node count.");
        fclose(in);
        exit(1);
    }

    if (amount_of_tree_nodes == 0) {
        ah_dprintERROR("%s", "Invalid encoded file: tree has zero nodes.");
        fclose(in);
        exit(1);
    }

    struct Ah_Tree tree = {0};
    tree.length = amount_of_tree_nodes;
    tree.capacity = amount_of_tree_nodes;
    tree.elements = AH_CALLOC(tree.capacity, sizeof(tree.elements[0]));
    if (tree.elements == NULL) {
        ah_dprintERROR("%s", "Memory allocation failed while reading tree.");
        fclose(in);
        exit(1);
    }

    if (fread(tree.elements, sizeof(tree.elements[0]), tree.length, in) !=
        tree.length) {
        ah_dprintERROR("%s", "Failed to read serialized Huffman tree.");
        AH_FREE(tree.elements);
        fclose(in);
        exit(1);
    }

    char file_out[256];
    size_t file_path_len = strlen(file_path);
    
    if (file_path_len > 5 &&
        strcmp(file_path + file_path_len - 5, ".huff") == 0) {
        snprintf(
            file_out,
            sizeof(file_out),
            "%.*s.decoded",
            (int)(file_path_len - 5),
            file_path
        );
    } else {
        snprintf(file_out, sizeof(file_out), "%s.decoded", file_path);
    }

    FILE *out = fopen(file_out, "wb");
    if (out == NULL) {
        ah_dprintERROR("Unable to create output file '%s'.", file_out);
        AH_FREE(tree.elements);
        fclose(in);
        exit(1);
    }

    int root_index = (int)tree.length - 1;
    struct Ah_Tree_Node root = tree.elements[root_index];

    if (root.left_index == -1 && root.right_index == -1) {
        for (uint32_t i = 0; i < amount_of_bytes; i++) {
            fputc((uint8_t)root.name, out);
        }
    } else {
        struct Ah_Bit_Reader br;
        ah_bit_reader_init(&br, in);

        uint32_t written = 0;
        int current_index = root_index;

        while (written < amount_of_bytes) {
            int bit = ah_bit_reader_read_bit(&br);
            if (bit == -1) {
                ah_dprintERROR(
                    "%s",
                    "Unexpected end of file while decoding bitstream."
                );
                AH_FREE(tree.elements);
                fclose(in);
                fclose(out);
                exit(1);
            }

            current_index = bit == 0
                ? tree.elements[current_index].left_index
                : tree.elements[current_index].right_index;

            if (current_index < 0 || (size_t)current_index >= tree.length) {
                ah_dprintERROR("%s", "Corrupted encoded file: invalid tree traversal.");
                AH_FREE(tree.elements);
                fclose(in);
                fclose(out);
                exit(1);
            }

            if (tree.elements[current_index].left_index == -1 &&
                tree.elements[current_index].right_index == -1) {
                fputc((uint8_t)tree.elements[current_index].name, out);
                written++;
                current_index = root_index;
            }
        }
    }

    AH_FREE(tree.elements);
    fclose(in);
    fclose(out);
}

AH_DEF void ah_encode_file(const char *file_path)
{
    size_t amount_of_bytes;
    struct Ah_Table table = ah_table_create(file_path, &amount_of_bytes);

    struct Ah_Tree tree = ah_tree_create_from_table(table);

    struct Ah_Code_Table code_table = ah_code_table_create(tree);

    ah_code_table_print(code_table); 

    FILE *in = fopen(file_path, "rb");
    char file_out[256];
    snprintf(file_out, 256, "%s.huff", file_path);
    FILE *out = fopen(file_out, "wb");

    {
        uint32_t amount_of_bytes_uint32 = (uint32_t)amount_of_bytes;
        uint32_t amount_of_tree_nodes = (uint32_t)tree.length;

        fwrite(&amount_of_bytes_uint32, sizeof(amount_of_bytes_uint32), 1, out);
        fwrite(&amount_of_tree_nodes, sizeof(amount_of_tree_nodes), 1, out);
        fwrite(tree.elements, sizeof(tree.elements[0]), tree.length, out);
    }

    struct Ah_Bit_Writer bw;
    ah_bit_writer_init(&bw, out);

    for (int c = fgetc(in); c != EOF; c = fgetc(in)) {
        int index = ah_code_table_get_index_by_name(code_table, (uint8_t)c);
        if (index == -1) {
            ah_dprintERROR("Byte 0x%02X was read during encoding but not found in the code table.", (uint8_t)c);
            exit(1);
        }
        ah_bit_writer_write_code(&bw, code_table.elements[index].code);
    }

    ah_bit_writer_flush(&bw);

    fclose(in);
    fclose(out);
}

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

AH_DEF struct Ah_Table ah_table_create(const char *file_path, size_t *amount_of_bytes)
{
    if (amount_of_bytes) *amount_of_bytes = 0;
    FILE *fp = fopen(file_path, "rb");
    if (fp == NULL) {
        ah_dprintERROR("Unable to open file '%s'.", file_path);
        exit(1);
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
        if (amount_of_bytes) (*amount_of_bytes)++;
    }
    fclose(fp);

    if (table.length == 0) {
        ah_dprintWARNING("%s", "Empty input file. Nothing done.");
        exit(0);
    }

    ah_qsort(table.elements, 0, (int)table.length-1);

    return table;
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

AH_DEF struct Ah_Tree ah_tree_create_from_table(struct Ah_Table table)
{
    struct Ah_Tree tree = {0};
    ada_init_array(struct Ah_Tree_Node, tree);

    struct Ah_Tree priority_queue = {0};
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

    AH_FREE(priority_queue.elements);

    return tree;
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
