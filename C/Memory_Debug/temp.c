#define AMD_MEMORY_DEBUG
#define ALMOG_MEMORY_DEBUG_IMPLEMENTATION
#include "./Almog_Memory_Debug.h"

struct int_array {
    size_t capacity;
    size_t length;
    int *elements; 
};

void int_array_print(struct int_array list)
{
    for (size_t i = 0; i < list.length; i++) {
        printf("%d, ", list.elements[i]);
    }
    printf("\n");
}

int main(void)
{
    printf("Hello, World!\n");

    struct int_array list = {0};
    amd_ada_init_array(int, list);

    for (size_t i = 0; i < 15000; i++) {
        amd_ada_appand(int, list, (int)i);
    }

    free(list.elements);

    amd_debug_mem_print(0);
    amd_dprintSIZE_T(list.capacity);

    amd_debug_mem_reset();
    return 0;
}
