#ifndef ALMOG_AOC_H_
#define ALMOG_AOC_H_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#define MAXDIR 100
#define MAX_LEN_LINE (int)1e3
#define NUM_OF_RACES (int)1e1
#define NUM_OF_DISTANCES (int)5e2
#define dprintSTRING(expr) printf(#expr " = %s\n", expr)
#define dprintCHAR(expr) printf(#expr " = %c\n", expr)
#define dprintINT(expr) printf(#expr " = %d\n", expr)
#define dprintSIZE_T(expr) printf(#expr " = %zu\n", expr)


int aaoc_get_line(FILE *fp, char *dst);
int aaoc_length(char *str);
int aaoc_get_next_word_from_line(char *dst, char *src);
void aaoc_copy_arry_by_indesies(char *target, int start, int end, char *src);
int aaoc_get_word_and_cut(char *dst, char *src);

#ifdef ALMOG_AOC_IMPLEMENTATION

int aaoc_get_line(FILE *fp, char *dst)
{
    int i = 0;
    char c;

    while ((c = fgetc(fp)) != '\n' && c != EOF) {
        dst[i] = c;
        i++;
        if (i >= MAX_LEN_LINE) {
            fprintf(stderr, "ERROR: line too long\n");
            exit(1);
        }
    }
    dst[i] = '\0';
    if (c == EOF) {
        return -1;
    }
    return i;
}

int aaoc_length(char *str)
{
    char c;
    int i = 0;

    while ((c = str[i]) != '\0') {
        i++;
    }
    return i++;
}

int aaoc_get_next_word_from_line(char *dst, char *src)
{
    int i = 0, j = 0;
    char c;

    while (isspace((c = src[i]))) {
        i++;
    }

    while ((c = src[i]) != ' ' &&
                      c != ';' &&
                      c != ':' &&
                      c != ',' &&
                      c != '\n'&&
                      c != '\0') {
                        dst[j] = src[i];
                        i++;
                        j++;
    }

    if ((c == ' ' ||
        c == ';' ||
        c == ':' ||
        c == ',' ||
        c == '\n'||
        c == '\0') && i == 0) {
            dst[j++] = c;
            i++;
    }

    dst[j] = '\0';

    if (j == 0) {
        return -1;
    }
    return i;

}

void aaoc_copy_arry_by_indesies(char *target, int start, int end, char *src)
{
    int j = 0;
    for (int i = start; i < end; i++) {
        target[j] = src[i];
        j++;
    }
    target[j] = '\0';
}

int aaoc_get_word_and_cut(char *dst, char *src)
{
    int last_pos;

    if (src[0] == '\0') {
        return 0;
    }
    last_pos = aaoc_get_next_word_from_line(dst, src);
    if (last_pos == -1) {
        return 0;
    }
    aaoc_copy_arry_by_indesies(src, last_pos, aaoc_length(src), src);
    return 1;
}

#endif /*ALMOG_AOC_IMPLEMENTATION*/

#ifndef ALMOG_DYNAMIC_ARRAY_H_
#define ALMOG_DYNAMIC_ARRAY_H_

#define INIT_CAPACITY 10

#ifndef ADA_MALLOC
#define ADA_MALLOC malloc
#endif /*ADA_MALLOC*/

#ifndef ADA_REALLOC
#define ADA_REALLOC realloc
#endif /*ADA_REALLOC*/

#ifndef ADA_ASSERT
#define ADA_ASSERT assert
#endif /*ADA_ASSERT*/

/* typedef struct {
    size_t length;
    size_t capacity;
    int* elements;
} ada_int_array; */

#define ada_array(type, header) do { header.capacity = INIT_CAPACITY; header.length = 0; header.elements = (type *)ADA_MALLOC(sizeof(type) * header.capacity); ADA_ASSERT(header.elements != NULL); } while (0)
    // do {
    //     header.capacity = INIT_CAPACITY;
    //     header.length = 0;
    //     header.elements = (type *)ADA_MALLOC(sizeof(type) * header.capacity);
    //     ADA_ASSERT(header.elements != NULL);
    // } while (0)

#define ada_resize(type, header, new_capacity) do { type *temp = (type *)ADA_REALLOC((void *)(header.elements), new_capacity*sizeof(type)); if (temp == NULL) { exit(1); } header.elements = temp; ADA_ASSERT(header.elements != NULL); header.capacity = new_capacity; } while (0)
    // do {
    //     type *temp = (type *)ADA_REALLOC((void *)(header.elements), new_capacity*sizeof(type));
    //     if (temp == NULL) {
    //         exit(1);
    //     }
    //     header.elements = temp;
    //     ADA_ASSERT(header.elements != NULL);
    //     header.capacity = new_capacity;
    // } while (0)

#define ada_appand(type, header, value) do { if (header.length >= header.capacity) { ada_resize(type, header, (int)(header.capacity*1.5)); } header.elements[header.length++] = value; } while (0)
    // do {
    //     if (header.length >= header.capacity) {
    //         ada_resize(type, header, (int)(header.capacity*1.5));
    //     }

    //     header.elements[header.length++] = value;

    // } while (0)

#endif /*ALMOG_DYNAMIC_ARRAY_H_*/

#endif /*ALMOG_AOC_H_*/