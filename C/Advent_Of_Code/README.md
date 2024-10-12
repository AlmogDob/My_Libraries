Set of string parsing function and macros for dynamic arrays.

The dynamic array works with struct. for example 
``` C
typedef struct {
    size_t length;
    size_t capacity;
    int* elements;
} ada_int_array;
```
