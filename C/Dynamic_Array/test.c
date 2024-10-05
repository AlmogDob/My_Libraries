#include <stdio.h>
#include "Almog_Dynamic_Array.h"

typedef struct {
    size_t length;
    size_t capacity;
    int* elements;
} ada_int_array;

void print_int_ada(ada_int_array ada, char *name)
{
    printf("%s\n", name);
    printf("capacity: %zu\n", ada.capacity);
    printf("length: %zu\n[", ada.length);
    if (ada.length == 0) {
        printf("]\n");
        return;
    }
    for (size_t i = 0; i < ada.length - 1; i++) {
        printf("%d, ", ada.elements[i]);
    }
    printf("%d]\n", ada.elements[ada.length - 1]);
}

#define ADA_INT_PRINT(ada) print_int_ada(ada, #ada)

int main()
{
    ada_int_array a;

    ada_array(int, a);

    for (int i = 0; i < 30; i++) {
        ada_appand(int, a, i);
    }
    
    ADA_INT_PRINT(a);

    return 0;
}

