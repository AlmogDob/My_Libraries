#define ALMOG_HUFFMAN_IMPLEMENTATION
#include "./include/Almog_Huffman.h"

struct Ah_Table_Element {
    uint8_t name;
    size_t count;
};

struct Ah_Table {
    size_t length;
    size_t capacity;
    struct Ah_Table_Element *elements;
};

void ah_table_element_print(struct Ah_Table_Element e)
{
    printf("name: 0x%02x | count: %zu\n", e.name, e.count);
}

void ah_table_print(struct Ah_Table table)
{
    for (size_t i = 0; i < table.length; i++) {
        printf("%3zu: ", i);
        ah_table_element_print(table.elements[i]);
    }
}

int ah_table_find_name(struct Ah_Table table, uint8_t name)
{
    if (table.length == 0) return -1;

    for (size_t i = 0; i < table.length; i++) {
        if (table.elements[i].name == name) {
            return (int)i;
        }
    }

    return -1;
}

void ah_swap(struct Ah_Table_Element v[], int i, int j)
{
    struct Ah_Table_Element temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

void ah_qsort(struct Ah_Table_Element v[], int left, int right)
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

int main(void)
{
    
    // FILE *fp = fopen("../input.txt", "rt");
    FILE *fp = fopen("../src/temp.c", "rt");
    
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

    ah_qsort(table.elements, 0, table.length-1);

    ah_table_print(table);

    return 0;
}
