#include <stdio.h>
#define ADA_IMPLEMENTATION
#include "Almog_Dynamic_Array.h"

typedef struct {
    size_t length;
    size_t capacity;
    int* elements;
} ada_int_array;

int main()
{
    ada_int_array a;

    a.elements = ada_array(int, a);

    printf("capacity: %zu\n", a.capacity);
    printf("length: %zu\n", a.length);

    return 0;
}
