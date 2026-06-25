#ifndef ALMOG_MEMORY_DEBUG_H_
#define ALMOG_MEMORY_DEBUG_H_

#include <stdio.h>

#include "Almog_String_Manipulation.h"

#ifndef AMD_MALLOC
#include <stdlib.h>
#define AMD_MALLOC malloc
#endif /*AMD_MALLOC*/
#ifndef AMD_EXIT
#include <stdlib.h>
#define AMD_EXIT exit
#endif /*AMD_EXIT*/
#ifndef AMD_REALLOC
#include <stdlib.h>
#define AMD_REALLOC realloc
#endif /*AMD_REALLOC*/

/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define amd_dprintSTRING(expr) printf("[Info] %s:%d:\n%*s" #expr " = %s\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define amd_dprintCHAR(expr) printf("[Info] %s:%d:\n%*s" #expr " = %c\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define amd_dprintINT(expr) printf("[Info] %s:%d:\n%*s" #expr " = %d\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define amd_dprintFLOAT(expr) printf("[Info] %s:%d:\n%*s" #expr " = %#f\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define amd_dprintDOUBLE(expr) printf("[Info] %s:%d:\n%*s" #expr " = %#g\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define amd_dprintSIZE_T(expr) printf("[Info] %s:%d:\n%*s" #expr " = %zu\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO variants print to
 * stdout and include file, line, and function information.
 */
#define amd_dprintINFO(fmt, ...) \
    fprintf(stdout, "[Info] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 7, "", __func__, 7, "", __VA_ARGS__)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define amd_dprintWARNING(fmt, ...) \
    fprintf(stderr, "[Warning] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 10, "", __func__, 10, "", __VA_ARGS__)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define amd_dprintERROR(fmt, ...) \
    fprintf(stderr, "[Error] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 8, "", __func__, 8, "", __VA_ARGS__)


/* ADA */
#ifndef AMD_ADA_INIT_CAPACITY
#define AMD_ADA_INIT_CAPACITY 10
#endif /*AMD_ADA_INIT_CAPACITY*/
#ifndef AMD_ADA_MALLOC
#define AMD_ADA_MALLOC AMD_MALLOC
#endif /*AMD_ADA_MALLOC*/
#ifndef AMD_ADA_EXIT
#define AMD_ADA_EXIT AMD_EXIT
#endif /*AMD_ADA_EXIT*/
#ifndef AMD_ADA_REALLOC
#define AMD_ADA_REALLOC AMD_REALLOC
#endif /*AMD_ADA_REALLOC*/
#ifndef AMD_ADA_ASSERT
#include <assert.h>
#define AMD_ADA_ASSERT assert
#endif /*AMD_ADA_ASSERT*/
#define amd_ada_init_array(type, header) do {                                           \
        (header).capacity = AMD_ADA_INIT_CAPACITY;                                      \
        (header).length = 0;                                                        \
        (header).elements = (type *)AMD_ADA_MALLOC(sizeof(type) * (header).capacity);   \
        if ((header).elements == NULL) {                                            \
            AMD_ADA_EXIT(1);                                                            \
        }                                                                           \
    } while (0)
#define amd_ada_resize(type, header, new_capacity) do {                                                         \
        type *amd_ada_temp_pointer = (type *)AMD_ADA_REALLOC((void *)((header).elements), new_capacity*sizeof(type)); \
        if (amd_ada_temp_pointer == NULL) {                                                                     \
            AMD_ADA_EXIT(1);                                                                                        \
        }                                                                                                   \
        (header).elements = amd_ada_temp_pointer;                                                                 \
        AMD_ADA_ASSERT((header).elements != NULL);                                                                \
        (header).capacity = new_capacity;                                                                     \
    } while (0)
#define amd_ada_appand(type, header, value) do {                                            \
        if ((header).length >= (header).capacity) {                                         \
            amd_ada_resize(type, (header), (int)((header).capacity + (header).capacity/2 + 1));   \
        }                                                                               \
        (header).elements[(header).length] = value;                                         \
        (header).length++;                                                                \
    } while (0)
#define amd_ada_insert(type, header, value, index) do {                                                             \
    AMD_ADA_ASSERT((int)(index) >= 0);                                                                              \
    AMD_ADA_ASSERT((float)(index) - (int)(index) == 0);                                                             \
    AMD_ADA_ASSERT((header).length > 0 && "You can not insert to an empty array.");                                 \
    AMD_ADA_ASSERT(index <= (header).length);                                                                       \
    amd_ada_appand(type, (header), (header).elements[(header).length-1]);                                           \
    for (int amd_ada_for_loop_index = (header).length-2; amd_ada_for_loop_index > (int)(index); amd_ada_for_loop_index--) { \
        (header).elements[amd_ada_for_loop_index] = (header).elements [amd_ada_for_loop_index-1];                       \
    }                                                                                                           \
    (header).elements[(index)] = value;                                                                         \
} while (0)
#define amd_ada_insert_unordered(type, header, value, index) do {   \
    AMD_ADA_ASSERT((int)(index) >= 0);                              \
    AMD_ADA_ASSERT((float)(index) - (int)(index) == 0);             \
    AMD_ADA_ASSERT(index <= (header).length);                       \
    if ((size_t)(index) == (header).length) {                   \
        amd_ada_appand(type, (header), value);                      \
    } else {                                                    \
        amd_ada_appand(type, (header), (header).elements[(index)]); \
        (header).elements[(index)] = value;                     \
    }                                                           \
} while (0)
#define amd_ada_remove(type, header, index) do {                                                                    \
    AMD_ADA_ASSERT((int)(index) >= 0);                                                                              \
    AMD_ADA_ASSERT((header).length > 0 && "You can not remove from an empty array.");                               \
    AMD_ADA_ASSERT((float)(index) - (int)(index) == 0);                                                             \
    AMD_ADA_ASSERT(index < (header).length);                                                                        \
    for (size_t amd_ada_for_loop_index = (index); amd_ada_for_loop_index < (header).length-1; amd_ada_for_loop_index++) {   \
        (header).elements[amd_ada_for_loop_index] = (header).elements[amd_ada_for_loop_index+1];                        \
    }                                                                                                           \
    (header).length--;                                                                                          \
} while (0)
#define amd_ada_remove_unordered(type, header, index) do {                              \
    AMD_ADA_ASSERT((int)(index) >= 0);                                                  \
    AMD_ADA_ASSERT((header).length > 0 && "You can not remove from an empty array.");   \
    AMD_ADA_ASSERT(index < (header).length);                                            \
    AMD_ADA_ASSERT((float)(index) - (int)(index) == 0);                                 \
    (header).elements[index] = (header).elements[(header).length-1];                \
    (header).length--;                                                              \
} while (0)
/* ADA */

struct Amd_Mem_Alloc_Buf {
    size_t size;
    void *buf;
};

struct Amd_Mem_Alloc_Line {
    size_t capacity;
    size_t length;
    struct Amd_Mem_Alloc_Buf elements;
    size_t line;
    char file[ASM_MAX_LEN];
    size_t size;
    size_t allocated_over_all;
    size_t freed;
};

#define AMD_MEM_ALLOC_LINES_NUM 1024
#define AMD_MEM_OVER_ALLOC 32
#define AMD_MEM_MAGIC_NUMBER 132

#ifndef AMD_SUCCESS 
    #define AMD_SUCCESS 1
#endif
#ifndef AMD_FAIL
    #define AMD_FAIL 0
#endif

#ifndef AMD_DEF
    #ifdef AMD_DEF_STATIC
        #define AMD_DEF static
    #else
        #define AMD_DEF extern
    #endif
#endif




#endif /*ALMOG_MEMORY_DEBUG_H_*/

#ifdef ALMOG_MEMORY_DEBUG_IMPLEMENTATION
#undef ALMOG_MEMORY_DEBUG_IMPLEMENTATION

struct Amd_Mem_Alloc_Line mem_alloc_lines[AMD_MEM_ALLOC_LINES_NUM];
size_t mem_alloc_lines_count = 0;


#endif /*ALMOG_MEMORY_DEBUG_IMPLEMENTATION*/