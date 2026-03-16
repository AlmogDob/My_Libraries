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

AH_DEF void     ah_qsort(struct Ah_Table_Element v[], int left, int right);
AH_DEF void     ah_swap(struct Ah_Table_Element v[], int i, int j);
AH_DEF void     ah_table_element_print(struct Ah_Table_Element e);
AH_DEF int      ah_table_find_name(struct Ah_Table table, uint8_t name);
AH_DEF void     ah_table_print(struct Ah_Table table);

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



#endif /*ALMOG_HUFFMAN_IMPLEMENTATION*/
