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

size_t ada_array_init(size_t _size, size_t _capacity, void *_elements, size_t *length_to_header, size_t *capacity_to_header);
#define ada_array(type, header) ada_array_init(sizeof(type), INIT_CAPACITY, (void *)(header.elements), &(header.length), &(header.capacity))

#endif /*ALMOG_DYNAMIC_ARRAY_H_*/

/*------------------------------------------------------------*/

#ifdef ADA_IMPLEMENTATION

/* returns 0 if fail */
size_t ada_array_init(size_t _size, size_t _capacity, void *_elements, size_t *length_to_header, size_t *capacity_to_header)
{
    _elements = malloc(_size * _capacity);
    *length_to_header = 0;
    *capacity_to_header = _capacity;

    if(_elements) {
        return 1;
    } else {
        return 0;
    }

}

#endif /*ADA_IMPLEMENTATION*/