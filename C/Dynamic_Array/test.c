#include <stdio.h>
#include "Almog_Dynamic_Array.h"

typedef struct {
    size_t length;
    size_t capacity;
    int* elements;
} ada_int_array;

typedef struct {
    size_t length;
    size_t capacity;
    float* elements;
} ada_float_array;

void print_int_ada(ada_int_array ada, char *name)
{
    printf("%s\n", name);
    printf("capacity: %zu\n", ada.capacity);
    printf("length: %zu\n[", ada.length);
    if (ada.length == 0) {
        printf("]\n\n");
        return;
    }
    for (size_t i = 0; i < ada.length - 1; i++) {
        printf("%d, ", ada.elements[i]);
    }
    printf("%d]\n\n", ada.elements[ada.length - 1]);
}
#define ADA_INT_PRINT(ada) print_int_ada(ada, #ada)

void print_float_ada(ada_float_array ada, char *name)
{
    printf("%s\n", name);
    printf("capacity: %zu\n", ada.capacity);
    printf("length: %zu\n[", ada.length);
    if (ada.length == 0) {
        printf("]\n\n");
        return;
    }
    for (size_t i = 0; i < ada.length - 1; i++) {
        printf("%g, ", ada.elements[i]);
    }
    printf("%g]\n\n", ada.elements[ada.length - 1]);
}
#define ADA_FLOAT_PRINT(ada) print_float_ada(ada, #ada)

int main()
{
    ada_int_array a;

    ada_init_array(int, a);

    for (int i = 0; i < 14; i++) {
        ada_appand(int, a, i);
    }
    
    ADA_INT_PRINT(a);

    ada_insert(int, a, 100, 1);
    ada_insert(int, a, 100, 1);
    ADA_INT_PRINT(a);
    

    ada_float_array b;

    ada_init_array(float, b);

    for (int i = 0; i < 69; i++) {
        ada_appand(float, b, i/2.0);
    }

    ADA_FLOAT_PRINT(b);


    return 0;
}

