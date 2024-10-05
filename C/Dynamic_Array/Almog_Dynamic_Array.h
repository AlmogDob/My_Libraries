#ifndef ALMOG_DYNAMIC_ARRAY_H_
#define ALMOG_DYNAMIC_ARRAY_H_

#include <stdlib.h>
#define INIT_CAPACITY 10

#ifndef ADA_MALLOC
#define ADA_MALLOC malloc
#endif /*ADA_MALLOC*/

/* typedef struct {
    size_t length;
    size_t capacity;
    int* elements;
} ada_int_array; */

void *ada_array_init(size_t _size, size_t _capacity, size_t *length_to_header, size_t *capacity_to_header);
#define ada_array(type, header) (type *)ada_array_init(sizeof(type), INIT_CAPACITY, &(header.length), &(header.capacity))

#endif /*ALMOG_DYNAMIC_ARRAY_H_*/

/*------------------------------------------------------------*/

#ifdef ADA_IMPLEMENTATION

void *ada_array_init(size_t _size, size_t _capacity, size_t *length_to_header, size_t *capacity_to_header)
{
    void *ptr = 0;
    ptr = ADA_MALLOC(_size * _capacity);

    if (ptr) {
        *length_to_header = 0;
        *capacity_to_header = _capacity;
    }
    return ptr;
}

#endif /*ADA_IMPLEMENTATION*/