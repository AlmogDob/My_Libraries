/**
 * @file
 * @brief Single-header PNG decoder
 *
 * This library reads a PNG file into memory, validates the PNG signature and
 * chunk CRCs, concatenates and parses the IDAT zlib stream, inflates DEFLATE
 * blocks, reverses PNG scanline filters, and writes the decoded image into a
 * 32-bit pixel buffer.
 * This PNG decoder is based on the PNG parser created in Handmade Hero, Casey Muratori's famous video series. 
 * A link to the video on YouTube: https://youtu.be/lkEWbIUEuN0
 * A link to the website: https://mollyrocket.com/#handmade
 * 
 * You can find the PNG specification (second edition) in the link below:
 * https://www.w3.org/TR/2003/REC-PNG-20031110/
 * 
 * ZLIB specification: https://www.ietf.org/rfc/rfc1950.txt 
 * DEFLATE specification: https://www.ietf.org/rfc/rfc1951.txt 
 */
#ifndef ALMOG_PNG_H_
#define ALMOG_PNG_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

// #include "include/Almog_Dynamic_Array.h"
#ifndef ALMOG_DYNAMIC_ARRAY_H_
#define ALMOG_DYNAMIC_ARRAY_H_

/**
 * @def ADA_INIT_CAPACITY
 * @brief Default initial capacity used by ada_init_array.
 *
 * You may override this by defining ADA_INIT_CAPACITY before including this file.
 */
#ifndef ADA_INIT_CAPACITY
#define ADA_INIT_CAPACITY 10
#endif /*ADA_INIT_CAPACITY*/

/**
 * @def ADA_MALLOC
 * @brief Allocation function used by this header (defaults to malloc).
 *
 * Define ADA_MALLOC to a compatible allocator before including this file to
 * override the default.
 */
#ifndef ADA_MALLOC
#include <stdlib.h>
#define ADA_MALLOC malloc
#endif /*ADA_MALLOC*/

/**
 * @def ADA_REALLOC
 * @brief Reallocation function used by this header (defaults to realloc).
 *
 * Define ADA_REALLOC to a compatible reallocator before including this file to
 * override the default.
 */
#ifndef ADA_REALLOC
#include <stdlib.h>
#define ADA_REALLOC realloc
#endif /*ADA_REALLOC*/

/**
 * @def ADA_ASSERT
 * @brief Assertion macro used by this header (defaults to assert).
 *
 * Define ADA_ASSERT before including this file to override. When NDEBUG is
 * defined, standard assert() is disabled.
 */
#ifndef ADA_ASSERT
#include <assert.h>
#define ADA_ASSERT assert
#endif /*ADA_ASSERT*/

/* typedef struct {
    size_t length;
    size_t capacity;
    int* elements;
} ada_int_array; */

/**
 * @def ada_init_array(type, header)
 * @brief Initialize an array header and allocate its initial storage.
 *
 * @param type   Element type stored in the array (e.g., int).
 * @param header Lvalue of the header struct containing fields:
 *               length, capacity, and elements.
 *
 * @pre header is a modifiable lvalue; header.elements is uninitialized or
 *      ignored and will be overwritten.
 * @post header.length == 0, header.capacity == INIT_CAPACITY,
 *       header.elements != NULL (or ADA_ASSERT fails).
 *
 * @note Allocation uses ADA_MALLOC and is checked via ADA_ASSERT.
 */
#define ada_init_array(type, header) do {                                       \
        (header).capacity = ADA_INIT_CAPACITY;                                        \
        (header).length = 0;                                                      \
        (header).elements = (type *)ADA_MALLOC(sizeof(type) * (header).capacity);   \
        ADA_ASSERT((header).elements != NULL);                                    \
    } while (0)

    /**
 * @def ada_resize(type, header, new_capacity)
 * @brief Resize the underlying storage to hold new_capacity elements.
 *
 * @param type         Element type stored in the array.
 * @param header       Lvalue of the header struct.
 * @param new_capacity New capacity in number of elements.
 *
 * @pre new_capacity >= header.length (otherwise elements beyond new_capacity
 *      are lost and length will not be adjusted).
 * @post header.capacity == new_capacity and header.elements points to a block
 *       large enough for new_capacity elements.
 *
 * @warning On allocation failure, this macro asserts
 * @note Reallocation uses ADA_REALLOC and is also checked via ADA_ASSERT.
 */
#define ada_resize(type, header, new_capacity) do {                                                         \
        type *ada_temp_pointer = (type *)ADA_REALLOC((void *)((header).elements), new_capacity*sizeof(type)); \
        ADA_ASSERT(ada_temp_pointer != NULL);                                                                 \
        (header).elements = ada_temp_pointer;                                                                 \
        ADA_ASSERT((header).elements != NULL);                                                                \
        (header).capacity = new_capacity;                                                                     \
    } while (0)

/**
 * @def ada_appand(type, header, value)
 * @brief Append a value to the end of the array, growing if necessary.
 *
 * @param type   Element type stored in the array.
 * @param header Lvalue of the header struct.
 * @param value  Value to append.
 *
 * @post header.length is incremented by 1; the last element equals value.
 *
 * @note Growth factor is (int)(header.capacity * 1.5). Because of truncation,
 *       very small capacities may not grow (e.g., from 1 to 1). With the
 *       default INIT_CAPACITY=10 this is typically not an issue unless you
 *       manually shrink capacity. Ensure growth always increases capacity by
 *       at least 1 if you customize this macro.
 */
#define ada_appand(type, header, value) do {                                            \
        if ((header).length >= (header).capacity) {                                         \
            ada_resize(type, (header), (int)((header).capacity + (header).capacity/2 + 1));   \
        }                                                                               \
        (header).elements[(header).length] = value;                                         \
        (header).length++;                                                                \
    } while (0)

/**
 * @def ada_insert(type, header, value, index)
 * @brief Insert value at position index, preserving order (O(n)).
 *
 * @param type   Element type stored in the array.
 * @param header Lvalue of the header struct.
 * @param value  Value to insert.
 * @param index  Destination index in the range [0, header.length].
 *
 * @pre 0 <= index <= header.length.
 * @pre header.length > 0 if index == header.length (this macro reads the last
 *      element internally). For inserting into an empty array, use
 *      ada_appand or ada_insert_unordered.
 * @post Element is inserted at index; subsequent elements are shifted right;
 *       header.length is incremented by 1.
 *
 * @note This macro asserts index is non-negative and an integer value using
 *       ADA_ASSERT. No explicit upper-bound assert is performed.
 */
#define ada_insert(type, header, value, index) do {                                                             \
    ADA_ASSERT((int)(index) >= 0);                                                                              \
    ADA_ASSERT((float)(index) - (int)(index) == 0);                                                             \
    ada_appand(type, (header), (header).elements[(header).length-1]);                                                 \
    for (int ada_for_loop_index = (header).length-2; ada_for_loop_index > (int)(index); ada_for_loop_index--) {   \
        (header).elements[ada_for_loop_index] = (header).elements [ada_for_loop_index-1];                           \
    }                                                                                                           \
    (header).elements[(index)] = value;                                                                           \
} while (0)


/**
 * @def ada_insert_unordered(type, header, value, index)
 * @brief Insert value at index without preserving order (O(1) amortized).
 *
 * If index == header.length, this behaves like an append. Otherwise, the
 * current element at index is moved to the end, and value is written at index.
 *
 * @param type   Element type stored in the array.
 * @param header Lvalue of the header struct.
 * @param value  Value to insert.
 * @param index  Index in the range [0, header.length].
 *
 * @pre 0 <= index <= header.length.
 * @post header.length is incremented by 1; array order is not preserved.
 */
#define ada_insert_unordered(type, header, value, index) do {   \
    ADA_ASSERT((int)(index) >= 0);                              \
    ADA_ASSERT((float)(index) - (int)(index) == 0);             \
    if ((size_t)(index) == (header).length) {                     \
        ada_appand(type, (header), value);                        \
    } else {                                                    \
        ada_appand(type, (header), (header).elements[(index)]);     \
        (header).elements[(index)] = value;                       \
    }                                                           \
} while (0)

/**
 * @def ada_remove(type, header, index)
 * @brief Remove element at index, preserving order (O(n)).
 *
 * @param type   Element type stored in the array.
 * @param header Lvalue of the header struct.
 * @param index  Index in the range [0, header.length - 1].
 *
 * @pre 0 <= index < header.length.
 * @post header.length is decremented by 1; subsequent elements are shifted
 *       left by one position. The element beyond the new length is left
 *       uninitialized.
 */
#define ada_remove(type, header, index) do {                                                                \
    ADA_ASSERT((int)(index) >= 0);                                                                          \
    ADA_ASSERT((float)(index) - (int)(index) == 0);                                                         \
    for (size_t ada_for_loop_index = (index); ada_for_loop_index < (header).length-1; ada_for_loop_index++) { \
        (header).elements[ada_for_loop_index] = (header).elements[ada_for_loop_index+1];                        \
    }                                                                                                       \
    (header).length--;                                                                                        \
} while (0)

/**
 * @def ada_remove_unordered(type, header, index)
 * @brief Remove element at index by moving the last element into its place
 *        (O(1)); order is not preserved.
 *
 * @param type   Element type stored in the array.
 * @param header Lvalue of the header struct.
 * @param index  Index in the range [0, header.length - 1].
 *
 * @pre 0 <= index < header.length and header.length > 0.
 * @post header.length is decremented by 1; array order is not preserved.
 */
#define ada_remove_unordered(type, header, index) do {          \
    ADA_ASSERT((int)(index) >= 0);                              \
    ADA_ASSERT((float)(index) - (int)(index) == 0);             \
    (header).elements[index] = (header).elements[(header).length-1];  \
    (header).length--;                                            \
} while (0) {;}

#endif /*ALMOG_DYNAMIC_ARRAY_H_*/

/**
 * @def APNG_MALLOC
 * @brief Allocation function used by the PNG library.
 *
 * Define APNG_MALLOC before including this file to override the default
 * allocator.
 */
#ifndef APNG_MALLOC
#include <stdlib.h>
#define APNG_MALLOC malloc
#endif

/**
 * @def APNG_FREE
 * @brief Deallocation function used by the PNG library.
 *
 * Define APNG_FREE before including this file to override the default
 * deallocator.
 */
#ifndef APNG_FREE
#include <stdlib.h>
#define APNG_FREE free
#endif

/**
 * @def APNG_ASSERT
 * @brief Assertion macro used by the PNG library.
 *
 * Define APNG_ASSERT before including this file to override the default
 * assertion facility.
 */
#ifndef APNG_ASSERT
#include <assert.h>
#define APNG_ASSERT assert
#endif

enum Apng_Return_Types {
    APNG_SUCCESS,
    APNG_FAIL,
};
/**
 * @def APNG_OK
 * @brief Alias for APNG_SUCCESS.
 */
#define APNG_OK APNG_SUCCESS

struct Apng_Pixel_Buffer {
    size_t rows;
    size_t cols;
    size_t stride_r;
    uint32_t *elements;
};

struct Apng_Byte_String {
    char *name;
    size_t capacity;
    size_t length;
    size_t cursor;
    uint8_t *elements;
};

struct Apng_Bit_Reader {
    struct Apng_Byte_String file;
    uint8_t current_byte;
    uint8_t bits_left;
};

struct Apng_Huffman_Entry {
    uint16_t symbol;
    uint16_t code;
    uint8_t code_length;
};
/**
 * @def APNG_HUFFMAN_CODE_MAX_LENGTH
 * @brief Maximum Huffman code length supported by this implementation.
 */
#define APNG_HUFFMAN_CODE_MAX_LENGTH 15

struct Apng_Huffman_Entrys_Table {
    size_t capacity;
    size_t length;
    uint8_t min_code_length;
    struct Apng_Huffman_Entry *elements;
};

enum Apng_Chunk_Type {
    APNG_TYPE_UNKNOWN,
    APNG_TYPE_IHDR,
    APNG_TYPE_PLTE,
    APNG_TYPE_IDAT,
    APNG_TYPE_IEND,
    APNG_TYPE_cHRM,
    APNG_TYPE_gAMA,
    APNG_TYPE_iCCP,
    APNG_TYPE_sBIT,
    APNG_TYPE_sRGB,
    APNG_TYPE_bKGD,
    APNG_TYPE_hIST,
    APNG_TYPE_tRNS,
    APNG_TYPE_pHYs,
    APNG_TYPE_sPLT,
    APNG_TYPE_tIME,
    APNG_TYPE_iTXt,
    APNG_TYPE_tEXt,
    APNG_TYPE_zTXt,
    APNG_TYPE_COUNT
};

struct Apng_PNG_Header {
    size_t size;
    size_t index;
    uint8_t *signature;
};

/**
 * @def APNG_CHUNK_HEADER_SIZE
 * @brief Size, in bytes, of a PNG chunk header
 *        (length + type = 4 + 4).
 */
#define APNG_CHUNK_HEADER_SIZE 8
struct Apng_Chunk_Header {
    size_t size;
    size_t index;
    uint32_t length;
    union {
        uint32_t type_raw;
        char type_str[4];
        uint8_t type_array[4];
    };
    enum Apng_Chunk_Type type;
};

/**
 * @def APNG_CHUNK_FOOTER_SIZE
 * @brief Size, in bytes, of a PNG chunk footer
 *        (CRC = 4 bytes).
 */
#define APNG_CHUNK_FOOTER_SIZE 4
struct Apng_Chunk_Footer {
    size_t size;
    size_t index;
    uint32_t CRC;
};

struct Apng_IHDR_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
    uint32_t width;
    uint32_t height;
    uint8_t bit_depth;
    uint8_t color_type;
    uint8_t compression_method;
    uint8_t filter_method;
    uint8_t interlace_method;
};

struct Apng_PLTE_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

/**
 * @def APNG_IDAT_ZLIB_HEADER_SIZE
 * @brief Size, in bytes, of the zlib header at the beginning of IDAT data.
 */
#define APNG_IDAT_ZLIB_HEADER_SIZE 2
struct Apng_IDAT_Header {
    size_t size;
    size_t index;
    uint8_t zlib_compression_method_flags;
    uint8_t additional_flags;
};

struct Apng_IDAT_Chunk {
    size_t index;
    uint32_t length;
    struct Apng_Byte_String IDAT_data;
    struct Apng_IDAT_Header header;
    uint8_t CM;
    uint8_t CINFO;
    uint8_t FCHECK;
    uint8_t FDICT;
    uint8_t FLEVEL;

    size_t LZ77_window_size;
};

/**
 * @def APNG_IDAT_FOOTER_SIZE
 * @brief Size, in bytes, of the Adler-32 checksum at the end of zlib data.
 */
#define APNG_IDAT_FOOTER_SIZE 4
struct Apng_IDAT_Footer {
    size_t size;
    size_t index;
    uint32_t check_value;
};

struct Apng_IEND_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

struct Apng_cHRM_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

struct Apng_gAMA_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

struct Apng_iCCP_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

struct Apng_sBIT_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

struct Apng_sRGB_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

struct Apng_bKGD_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

struct Apng_hIST_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

struct Apng_tRNS_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

struct Apng_pHYs_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

struct Apng_sPLT_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

struct Apng_tIME_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

struct Apng_iTXt_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

struct Apng_tEXt_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

struct Apng_zTXt_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
};

struct Apng_PNG_Image {
    struct Apng_Byte_String file;
    // struct Apng_Bit_Reader br;
    struct Apng_Pixel_Buffer pixels;
    struct {
        struct Apng_IHDR_Chunk IHDR_chunk;
        struct Apng_PLTE_Chunk PLTE_chunk;
        struct Apng_IDAT_Chunk IDAT_chunk;
        struct Apng_IEND_Chunk IEND_chunk;
        struct Apng_cHRM_Chunk cHRM_chunk;
        struct Apng_gAMA_Chunk gAMA_chunk;
        struct Apng_iCCP_Chunk iCCP_chunk;
        struct Apng_sBIT_Chunk sBIT_chunk;
        struct Apng_sRGB_Chunk sRGB_chunk;
        struct Apng_bKGD_Chunk bKGD_chunk;
        struct Apng_hIST_Chunk hIST_chunk;
        struct Apng_tRNS_Chunk tRNS_chunk;
        struct Apng_pHYs_Chunk pHYs_chunk;
        struct Apng_sPLT_Chunk sPLT_chunk;
        struct Apng_tIME_Chunk tIME_chunk;
        struct Apng_iTXt_Chunk iTXt_chunk;
        struct Apng_tEXt_Chunk tEXt_chunk;
        struct Apng_zTXt_Chunk zTXt_chunk;
    } chunks;
};

#ifndef APNG_DEF
    #ifdef APNG_DEF_STATIC
        /**
         * @def APNG_DEF
         * @brief Storage-class specifier used by the single-header API.
         *
         * When APNG_DEF_STATIC is defined, all declarations become static.
         * Otherwise they default to extern.
         */
        #define APNG_DEF static
    #else
        /**
         * @def APNG_DEF
         * @brief Storage-class specifier used by the single-header API.
         *
         * When APNG_DEF_STATIC is not defined, all declarations default to
         * extern.
         */
        #define APNG_DEF extern
    #endif
#endif
/**
 * @def APNG_UNUSED
 * @brief Mark a parameter or local value as intentionally unused.
 */
#define APNG_UNUSED(x) ((void)x)

/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define apng_dprintSTRING(expr) printf("[Info] %s:%d:\n" #expr " = %s\n", __FILE__, __LINE__, expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define apng_dprintCHAR(expr) printf("[Info] %s:%d:\n" #expr " = %c\n", __FILE__, __LINE__, expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define apng_dprintINT(expr) printf("[Info] %s:%d:\n" #expr " = %d\n", __FILE__, __LINE__, expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define apng_dprintFLOAT(expr) printf("[Info] %s:%d:\n" #expr " = %#g\n", __FILE__, __LINE__, expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define apng_dprintDOUBLE(expr) printf("[Info] %s:%d:\n" #expr " = %#g\n", __FILE__, __LINE__, expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define apng_dprintSIZE_T(expr) printf("[Info] %s:%d:\n" #expr " = %zu\n", __FILE__, __LINE__, expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define apng_dprintINFO(fmt, ...) \
    fprintf(stderr, "[Info] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 7, "", __func__, 7, "", __VA_ARGS__)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define apng_dprintWARNING(fmt, ...) \
    fprintf(stderr, "[Warning] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 10, "", __func__, 10, "", __VA_ARGS__)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define apng_dprintERROR(fmt, ...) \
    fprintf(stderr, "[Error] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 8, "", __func__, 8, "", __VA_ARGS__)

/**
 * @def apng_min
 * @brief Return the smaller of two values.
 *
 * @warning This macro evaluates its arguments more than once.
 */
#define apng_min(a, b) ((a) < (b) ? (a) : (b))
/**
 * @def apng_max
 * @brief Return the larger of two values.
 *
 * @warning This macro evaluates its arguments more than once.
 */
#define apng_max(a, b) ((a) > (b) ? (a) : (b))
/**
 * @def APNG_PIXEL_BUFFER_AT
 * @brief Access a pixel in a pixel buffer by row and column.
 *
 * Expands to an lvalue expression. In debug builds it asserts that the
 * requested indices are within bounds.
 */
#define APNG_PIXEL_BUFFER_AT(m, i, j) (m).elements[(APNG_ASSERT((i) < (m).rows && (j) < (m).cols), (i) * (m).stride_r + (j))]
/**
 * @def APNG_HexARGB_TO_RGBA_VAR
 * @brief Unpack a 32-bit 0xAARRGGBB value into separate RGBA variables.
 *
 * The arguments r, g, b, and a must be modifiable lvalues.
 */
#define APNG_HexARGB_TO_RGBA_VAR(x, r, g, b, a) r = ((x)>>(8*2)&0xFF); g = ((x)>>(8*1)&0xFF); b = ((x)>>(8*0)&0xFF); a = ((x)>>(8*3)&0xFF)
/**
 * @def APNG_HexARGB_TO_RGB_VAR
 * @brief Unpack a 32-bit 0xAARRGGBB value into separate RGB variables.
 *
 * The arguments r, g, and b must be modifiable lvalues.
 */
#define APNG_HexARGB_TO_RGB_VAR(x, r, g, b) r = ((x)>>(8*2)&0xFF); g = ((x)>>(8*1)&0xFF); b = ((x)>>(8*0)&0xFF)
/**
 * @def APNG_RGBA_TO_hexARGB
 * @brief Pack RGBA channels into a 32-bit 0xAARRGGBB integer.
 *
 * Alpha is clamped to 255. RGB inputs are used as provided.
 */
#define APNG_RGBA_TO_hexARGB(r, g, b, a) (int)(0x01000000l*(unsigned int)(apng_max(0, apng_min(a, 255))) + 0x010000*(int)(apng_max(0, apng_min(r, 255))) + 0x000100*(int)(apng_max(0, apng_min(g, 255))) + 0x000001*(int)(apng_max(0, apng_min(b, 255))))
/**
 * @def APNG_STATIC_ARRAY_LEN
 * @brief Return the number of elements in a compile-time array.
 *
 * @warning This macro must be used with an actual array, not a pointer.
 */
#define APNG_STATIC_ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_HLIT_OFFSET 257
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_HDIST_OFFSET 1
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_HCLEN_OFFSET 4
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_MAX_NUM_OF_CODE_LENGTH_CODE_LENGTH 19
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_CODE_LENGTH_CODE_LENGTH_LENGTH 3
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_BFINAL_SIZE 1
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_BTYPE_SIZE 2
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_LEN_SIZE 16
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_NLEN_SIZE 16
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_HLIT_SIZE 5
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_HDIST_SIZE 5
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_HCLEN_SIZE 4
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_LIT_LEN_CODE_LENGTH_MAX_COUNT 286
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_DIST_CODE_LENGTH_MAX_COUNT 32
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_FIX_HUFFMAN_HLIT 288
/**
 * @name DEFLATE field sizes, offsets, and fixed-table limits
 * @brief Constants used while parsing and decoding DEFLATE blocks.
 */
#define APNG_FIX_HUFFMAN_HDIST 32

APNG_DEF enum Apng_Return_Types             apng_adler32_check(uint32_t original_adler32, uint8_t *buffer, size_t buffer_length);
APNG_DEF uint32_t                           apng_adler32_update(uint32_t adler, uint8_t *buffer, size_t buffer_length);
APNG_DEF struct Apng_Byte_String            apng_bin_file_read(char *file_name);
APNG_DEF void                               apng_byte_string_free(struct Apng_Byte_String *bs);
APNG_DEF void                               apng_bit_reader_flash(struct Apng_Bit_Reader *br);
APNG_DEF void                               apng_bit_reader_init(struct Apng_Bit_Reader *br, struct Apng_Byte_String file);
APNG_DEF uint8_t                            apng_bit_reader_read_bit(struct Apng_Bit_Reader *br);
APNG_DEF uint32_t                           apng_bit_reader_read_bits(struct Apng_Bit_Reader *br, size_t count);
APNG_DEF uint16_t                           apng_uint16_bits_reverse(uint16_t value, uint8_t bit_count);
APNG_DEF struct Apng_Chunk_Footer           apng_chunk_footer_get(struct Apng_Byte_String *bs);
APNG_DEF struct Apng_Chunk_Header           apng_chunk_header_get(struct Apng_Byte_String *bs);
APNG_DEF void *                             apng_consume_bytes(struct Apng_Byte_String *bs, size_t amount);
APNG_DEF enum Apng_Return_Types             apng_crc32_check(struct Apng_Chunk_Header header, void *chunk_data, struct Apng_Chunk_Footer footer);
APNG_DEF uint32_t                           apng_crc32_update(uint32_t crc, uint8_t *buf, size_t buf_len);
APNG_DEF uint32_t                           apng_endian_swap_uint32(uint32_t x);
APNG_DEF uint16_t                           apng_endian_swap_uint16(uint16_t x);
APNG_DEF uint32_t                           apng_four_char_to_uint32_t(const char *str);
APNG_DEF enum Apng_Return_Types             apng_huffman_decode_symbol(struct Apng_Huffman_Entrys_Table table, struct Apng_Bit_Reader *br, uint16_t *symbol);
APNG_DEF struct Apng_Huffman_Entrys_Table   apng_huffman_entry_table_create(uint32_t *code_length_array, size_t code_length_array_len);
APNG_DEF void                               apng_huffman_entry_print(struct Apng_Huffman_Entry entry);
APNG_DEF void                               apng_huffman_entry_table_print(struct Apng_Huffman_Entrys_Table table);
APNG_DEF enum Apng_Return_Types             apng_huffman_entry_table_get_symbol(struct Apng_Huffman_Entrys_Table table, uint16_t code, uint8_t code_length, uint16_t *symbol);
APNG_DEF enum Apng_Return_Types             apng_lit_len_dist_code_length_decode(struct Apng_Huffman_Entrys_Table dict_huffman, struct Apng_Bit_Reader *br, uint32_t HLIT, uint32_t HDIST, uint32_t *lit_len_dist_code_length);
APNG_DEF struct Apng_Pixel_Buffer           apng_pixel_buffer_malloc(size_t rows, size_t cols);
APNG_DEF enum Apng_Return_Types             apng_png_decode(struct Apng_Byte_String file, struct Apng_PNG_Image *image, bool print_info);
APNG_DEF void                               apng_png_free(struct Apng_PNG_Image *image);
APNG_DEF bool                               apng_png_header_signature_correct(struct Apng_PNG_Header h);
APNG_DEF enum Apng_Return_Types             apng_png_load(char *file_name, struct Apng_PNG_Image *image, bool print_info);
APNG_DEF struct Apng_PNG_Header             apng_png_header_get(struct Apng_Byte_String *bs);
APNG_DEF void                               apng_uint16_print_binary(uint16_t value, uint8_t bit_count);
APNG_DEF enum Apng_Chunk_Type               apng_type_get_from_type_raw(uint32_t raw_type);
APNG_DEF const char *                       apng_type_name_get(enum Apng_Chunk_Type type);

/* chunk parsers */
APNG_DEF enum Apng_Return_Types             apng_IHDR_chunk_parse(struct Apng_IHDR_Chunk *chunk);
APNG_DEF enum Apng_Return_Types             apng_IDAT_chunk_parse(struct Apng_IDAT_Chunk *chunk);
APNG_DEF enum Apng_Return_Types             apng_IDAT_decode(struct Apng_PNG_Image *image);
APNG_DEF enum Apng_Return_Types             apng_IDAT_decompress(struct Apng_PNG_Image *image, struct Apng_Byte_String *temp_bs);
APNG_DEF enum Apng_Return_Types             apng_IDAT_unfiltering(uint8_t *unfiltered_data, uint8_t *decompressed_data, size_t width, size_t height, size_t num_of_channels, size_t bit_per_channel);
APNG_DEF struct Apng_IDAT_Header            apng_IDAT_header_get_from_IDAT_chunk(struct Apng_IDAT_Chunk chunk);

#endif /*ALMOG_PNG_H_*/

#ifdef ALMOG_PNG_IMPLEMENTATION
#undef ALMOG_PNG_IMPLEMENTATION

/**
 * @brief Verify that a buffer matches an expected Adler-32 checksum.
 * @param original_adler32 Expected Adler-32 value from the zlib stream.
 * @param buffer Buffer to validate.
 * @param buffer_length Length of buffer in bytes.
 * @return APNG_SUCCESS if the checksum matches, otherwise APNG_FAIL.
 */
APNG_DEF enum Apng_Return_Types apng_adler32_check(uint32_t original_adler32, uint8_t *buffer, size_t buffer_length)
{
    uint32_t adler = apng_adler32_update((uint32_t)1, buffer, buffer_length);

    if (adler != original_adler32) {
        apng_dprintERROR("Failed adler32 check of the zlib. Expected: %u but got: %u", original_adler32, adler);
        return APNG_FAIL;
    } else {
        return APNG_SUCCESS;
    }
}

/**
 * @brief Update or compute an Adler-32 checksum over a buffer.
 * @param adler Initial Adler-32 state. Use 1 for a fresh checksum.
 * @param buffer Input buffer.
 * @param buffer_length Length of buffer in bytes.
 * @return Updated Adler-32 checksum.
 */
APNG_DEF uint32_t apng_adler32_update(uint32_t adler, uint8_t *buffer, size_t buffer_length)
{
    /* according to the ZLIB specification: https://www.ietf.org/rfc/rfc1950.txt */
    uint32_t BASE = 65521; /* largest prime smaller than 65536 */
    uint32_t s1 = adler & 0xffff;
    uint32_t s2 = (adler >> 16) & 0xffff;

    for (size_t n = 0; n < buffer_length; n++) {
        s1 = (s1 + buffer[n]) % BASE;
        s2 = (s2 + s1) % BASE;
    }

    return (s2 << 16) + s1;
}

/**
 * @brief Read an entire binary file into a byte-string structure.
 * @param file_name Path to the file to read.
 * @return A byte string containing the file contents. On failure, the returned
 *         structure is zero-initialized as much as possible.
 *
 * @note The returned object owns heap memory and must be released with
 *       apng_byte_string_free().
 */
APNG_DEF struct Apng_Byte_String apng_bin_file_read(char *file_name)
{
    struct Apng_Byte_String res = {0};
    
    FILE *fp = fopen(file_name, "rb");
    if (fp == NULL) {
        int err = errno;
        apng_dprintERROR( "Cannot open file %s: %s", file_name, strerror(err));
        return res;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        int err = errno;
        apng_dprintERROR( "Failed to seek to end of file %s: %s", file_name, strerror(err));
        fclose(fp);
        return res;
    }

    long size = ftell(fp);
    if (size < 0) {
        int err = errno;
        apng_dprintERROR( "Failed to tell size of file %s: %s", file_name, strerror(err));
        fclose(fp);
        return res;
    }

    if (fseek(fp, 0, SEEK_SET) != 0) {
        int err = errno;
        apng_dprintERROR( "Failed to seek to start of file %s: %s", file_name, strerror(err));
        fclose(fp);
        return res;
    }

    res.name = APNG_MALLOC(sizeof(char) * (1 + strlen(file_name)));
    if (res.name == NULL) {
        apng_dprintERROR( "Memory allocation failed for file %s (%zu bytes).", file_name, strlen(file_name));
        fclose(fp);
        res.length = 0;
        return res;
    }
    strncpy(res.name, file_name, strlen(file_name)+1);

    res.length = (size_t)size;
    res.capacity = (size_t)size;
    res.elements = APNG_MALLOC(res.length);
    if (res.elements == NULL) {
        apng_dprintERROR( "Memory allocation failed for file %s (%zu bytes).", file_name, res.length);
        fclose(fp);
        res.length = 0;
        APNG_FREE(res.name);
        return res;
    }

    size_t nread = fread(res.elements, 1, res.length, fp);
    if (nread != res.length) {
        if (ferror(fp)) {
            int err = errno;
            apng_dprintERROR( "Failed to read file %s: %s", file_name, strerror(err));
        } else {
            apng_dprintERROR(
                "Unexpected end of file while reading %s "
                "(expected %zu bytes, got %zu).", file_name, res.length, nread);
        }
        APNG_FREE(res.elements);
        APNG_FREE(res.name);
        res.elements = NULL;
        res.length = 0;
        fclose(fp);
        return res;
    }

    if (fclose(fp) != 0) {
        int err = errno;
        apng_dprintERROR( "Failed to close file %s: %s", file_name, strerror(err));
        APNG_FREE(res.elements);
        APNG_FREE(res.name);
        res.elements = NULL;
        res.length = 0;
        return res;
    }

    return res;
}

/**
 * @brief Free the storage owned by a byte string and reset its fields.
 * @param bs Byte string to release.
 */
APNG_DEF void apng_byte_string_free(struct Apng_Byte_String *bs)
{
    APNG_FREE(bs->elements);
    APNG_FREE(bs->name);
    bs->capacity = 0;
    bs->length = 0;
    bs->cursor = 0;
}

/**
 * @brief Discard any unread bits cached in the bit reader.
 * @param br Bit reader to reset to the next byte boundary.
 */
APNG_DEF void apng_bit_reader_flash(struct Apng_Bit_Reader *br)
{
    br->current_byte = 0;
    br->bits_left = 0;
}

/**
 * @brief Initialize a bit reader over a byte string.
 * @param br Bit reader to initialize.
 * @param file Source byte string.
 */
APNG_DEF void apng_bit_reader_init(struct Apng_Bit_Reader *br, struct Apng_Byte_String file)
{
    br->file = file;
    br->current_byte = 0;
    br->bits_left = 0;
}

/**
 * @brief Read one bit from the stream.
 * @param br Bit reader.
 * @return The next bit, read least-significant-bit first within each byte.
 */
APNG_DEF uint8_t apng_bit_reader_read_bit(struct Apng_Bit_Reader *br)
{
    if (br->bits_left == 0) {
        APNG_ASSERT(br->file.cursor < br->file.length);
        uint8_t c = br->file.elements[br->file.cursor++];
        br->current_byte = c;
        br->bits_left = 8;
    }

    uint8_t bit = (br->current_byte & 1) ? 1 : 0;
    br->current_byte >>= 1;
    br->bits_left--;
    return bit;
}

/**
 * @brief Read multiple bits from the stream.
 * @param br Bit reader.
 * @param count Number of bits to read; must be at most 32.
 * @return The bits packed into the low bits of the result in read order.
 */
APNG_DEF uint32_t apng_bit_reader_read_bits(struct Apng_Bit_Reader *br, size_t count)
{
    APNG_ASSERT(count <= 32);

    uint32_t res = 0;

    for (size_t i = 0; i < count; i++) {
        res |= ((uint32_t)apng_bit_reader_read_bit(br)) << i;
    }

    return res;
}

/**
 * @brief Reverse the lowest bit_count bits of a 16-bit value.
 * @param value Input value.
 * @param bit_count Number of low bits to reverse.
 * @return Bit-reversed value.
 */
APNG_DEF uint16_t apng_uint16_bits_reverse(uint16_t value, uint8_t bit_count)
{
    if (value == 0) return 0;
    uint16_t result = 0;
    for (uint8_t i = 0; i < bit_count; i++) {
        result <<= 1;
        result |= (value & 1u);
        value >>= 1;
    }
    return result;
}

/**
 * @brief Parse a PNG chunk footer from the current cursor position.
 * @param bs Byte string whose cursor points at a chunk CRC.
 * @return Parsed chunk footer.
 */
APNG_DEF struct Apng_Chunk_Footer apng_chunk_footer_get(struct Apng_Byte_String *bs)
{
    size_t size = APNG_CHUNK_FOOTER_SIZE;
    struct Apng_Chunk_Footer footer = {
        .index = bs->cursor,
        .size = size,
    };
    uint32_t *CRC = (uint32_t *)(apng_consume_bytes(bs, 4));
    footer.CRC = apng_endian_swap_uint32(*CRC);
    
    return footer;
}

/**
 * @brief Parse a PNG chunk header from the current cursor position.
 * @param bs Byte string whose cursor points at a chunk header.
 * @return Parsed chunk header.
 */
APNG_DEF struct Apng_Chunk_Header apng_chunk_header_get(struct Apng_Byte_String *bs)
{
    size_t size = APNG_CHUNK_HEADER_SIZE;
    struct Apng_Chunk_Header header = {
        .index = bs->cursor,
        .size = size,
    };
    uint32_t *length = (uint32_t *)(apng_consume_bytes(bs, APNG_CHUNK_HEADER_SIZE / 2));
    header.length = apng_endian_swap_uint32(*length);
    uint32_t *raw_type = (uint32_t *)(apng_consume_bytes(bs, APNG_CHUNK_HEADER_SIZE / 2));
    header.type_raw = *raw_type;
    header.type = apng_type_get_from_type_raw(header.type_raw);

    return header;
}

/**
 * @brief Return a pointer to the next amount bytes and advance the cursor.
 * @param bs Source byte string.
 * @param amount Number of bytes to consume.
 * @return Pointer to the first consumed byte.
 *
 * @pre amount must not exceed the number of remaining bytes.
 */
APNG_DEF void * apng_consume_bytes(struct Apng_Byte_String *bs, size_t amount_byte)
{
    APNG_ASSERT(amount_byte <= bs->length - bs->cursor);

    void *res = &(bs->elements[bs->cursor]);
    bs->cursor += amount_byte;

    return res;
}

/**
 * @brief Verify the CRC of a PNG chunk.
 * @param header Chunk header.
 * @param chunk_data Pointer to the chunk body.
 * @param footer Chunk footer containing the file CRC.
 * @return APNG_SUCCESS if the CRC matches, otherwise APNG_FAIL.
 */
APNG_DEF enum Apng_Return_Types apng_crc32_check(struct Apng_Chunk_Header header, void *chunk_data, struct Apng_Chunk_Footer footer)
{
    uint32_t calculated_crc = 0xFFFFFFFFu;
    calculated_crc = apng_crc32_update(calculated_crc, header.type_array, 4);
    calculated_crc = apng_crc32_update(calculated_crc, (uint8_t *)chunk_data, header.length);
    calculated_crc ^= 0xFFFFFFFFu;
    if (calculated_crc == footer.CRC) {
        return APNG_SUCCESS;
    } else {
        apng_dprintERROR("CRC mismatch for chunk '%.*s'. Computed 0x%08X but file has 0x%08X.",
            4, header.type_str, calculated_crc, footer.CRC);
        return APNG_FAIL;
    }
}

/**
 * @brief Update a CRC-32 value over a byte buffer.
 * @param crc Initial CRC state.
 * @param buf Input bytes.
 * @param buf_len Number of bytes in buf.
 * @return Updated CRC-32 value.
 */
APNG_DEF uint32_t apng_crc32_update(uint32_t crc, uint8_t *buf, size_t buf_len)
{
    for (size_t i = 0; i < buf_len; i++) {
        crc ^= buf[i];
        for (size_t bit = 0; bit < 8; bit++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320u;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}

/**
 * @brief Swap the byte order of a 32-bit unsigned integer.
 * @param x Input value.
 * @return Byte-swapped value.
 */
APNG_DEF uint32_t apng_endian_swap_uint32(uint32_t x)
{
    return ((x << 24) |
            ((x & 0xFF00) << 8) |
            ((x >> 8) & 0xFF00) |
            (x >> 24));
}

/**
 * @brief Swap the byte order of a 16-bit unsigned integer.
 * @param x Input value.
 * @return Byte-swapped value.
 */
APNG_DEF uint16_t apng_endian_swap_uint16(uint16_t x)
{
    return ((x << 8) | (x >> 8));
}

/**
 * @brief Pack four characters into a uint32_t in library-defined order.
 * @param str Pointer to at least four characters.
 * @return Packed 32-bit value suitable for chunk-type comparisons.
 */
APNG_DEF uint32_t apng_four_char_to_uint32_t(const char * str)
{
    return ((uint32_t)(((uint32_t)((str)[0]) << 0) | ((uint32_t)((str)[1]) << 8) | ((uint32_t)((str)[2]) << 16) | ((uint32_t)((str)[3]) << 24)));
}

/**
 * @brief Decode one symbol using a Huffman table and bit reader.
 * @param table Huffman decode table.
 * @param br Bit reader positioned at the encoded symbol.
 * @param symbol Output symbol.
 * @return APNG_SUCCESS on success, otherwise APNG_FAIL.
 */
APNG_DEF enum Apng_Return_Types apng_huffman_decode_symbol(struct Apng_Huffman_Entrys_Table table, struct Apng_Bit_Reader *br, uint16_t *symbol)
{
    uint16_t code = 0;

    for (uint8_t len = 1; len <= APNG_HUFFMAN_CODE_MAX_LENGTH; len++) {
        code |= (uint16_t)apng_bit_reader_read_bit(br) << (len - 1);

        for (size_t i = 0; i < table.length; i++) {
            struct Apng_Huffman_Entry entry = table.elements[i];
            if (entry.code_length == len && entry.code == code) {
                *symbol = entry.symbol;
                return APNG_SUCCESS;
            }
        }
    }

    apng_dprintERROR("%s", "Failed to decode symbol");
    return APNG_FAIL;
}

/**
 * @brief Build a canonical Huffman decode table from code lengths.
 * @param code_length_array Array of code lengths indexed by symbol.
 * @param code_length_array_len Number of entries in code_length_array.
 * @return Constructed Huffman table.
 *
 * @note The returned table owns a dynamically allocated elements buffer. The
 *       caller is responsible for freeing table.elements when no longer needed.
 */
APNG_DEF struct Apng_Huffman_Entrys_Table apng_huffman_entry_table_create(uint32_t *code_length_array, size_t code_length_array_len)
{
    /** TODO:
     * Instand of having an array of unordered code but ordered symbols.
     * Don't do a dynamic array. Do a fixed malloced array and the code is the index and the symbol is the value.
     */
    APNG_ASSERT(APNG_HUFFMAN_CODE_MAX_LENGTH <= 16 && "I am assuming the maximum length of a Huffman code is 16."
        "If not, think on using a different algorithm.");
    
    struct Apng_Huffman_Entrys_Table huffman_table = {0};
    ada_init_array(struct Apng_Huffman_Entry, huffman_table);

    uint32_t code_length_his[APNG_HUFFMAN_CODE_MAX_LENGTH + 1] = {0};/* If APNG_HUFFMAN_CODE_MAX_LENGTH is 16, then the histogram should be 0->16 */
    for (size_t i = 0; i < code_length_array_len; i++) {
        ++code_length_his[code_length_array[i]];
    }
    #if 0
    for (size_t i = 0; i < APNG_STATIC_ARRAY_LEN(code_length_his); i++) {
        printf("%d, ", code_length_his[i]);
    }
    printf("\n");
    #endif

    code_length_his[0] = 0;
    uint32_t next_code[APNG_STATIC_ARRAY_LEN(code_length_his)] = {0};
    for (uint32_t bits = 1, code = 0; bits <= APNG_HUFFMAN_CODE_MAX_LENGTH; bits++) {
        code = (code + code_length_his[bits-1]) << 1;
        next_code[bits] = code;
    }
    #if 0
    for (size_t i = 0; i < APNG_STATIC_ARRAY_LEN(next_code); i++) {
        printf("%d, ", next_code[i]);
    }
    printf("\n");
    #endif

    for (size_t i = 0; i < code_length_array_len; i++) {
        uint32_t len = code_length_array[i];
        if (len != 0) {
            uint32_t code = next_code[code_length_array[i]]++;
            struct Apng_Huffman_Entry entry = {
                .symbol = (uint16_t)i,
                .code = apng_uint16_bits_reverse((uint16_t)code, (uint8_t)len),
                // .code = code,
                .code_length = (uint8_t)len,
            };
            ada_appand(struct Apng_Huffman_Entry, huffman_table, entry);
        }
    }

    #if 0
    apng_huffman_entry_table_print(huffman_table);
    for (size_t i = 0; i < APNG_STATIC_ARRAY_LEN(next_code); i++) {
        printf("%d, ", next_code[i]);
    }
    printf("\n");
    #endif

    uint8_t min_code_length = APNG_HUFFMAN_CODE_MAX_LENGTH;
    for (size_t i = 0; i < huffman_table.length; i++) {
        uint8_t len = huffman_table.elements[i].code_length;
        if (len < min_code_length) {
            min_code_length = len;
        }
    }
    huffman_table.min_code_length = min_code_length;

    return huffman_table;
}

/**
 * @brief Print one Huffman entry for debugging.
 * @param entry Entry to print.
 */
APNG_DEF void apng_huffman_entry_print(struct Apng_Huffman_Entry entry)
{
    printf("{.symbol = %6d, .len = %2u, .code = %-6d = ", entry.symbol, entry.code_length, entry.code);
    apng_uint16_print_binary(entry.code, entry.code_length);
    printf("%*s}\n", 15-entry.code_length, " ");
}

/**
 * @brief Print an entire Huffman table for debugging.
 * @param table Table to print.
 */
APNG_DEF void apng_huffman_entry_table_print(struct Apng_Huffman_Entrys_Table table)
{
    for (size_t i = 0; i < table.length; i++) {
        printf("%3zu: ", i);
        apng_huffman_entry_print(table.elements[i]);
    }
}

/**
 * @brief Look up a symbol by exact code and code length.
 * @param table Huffman table.
 * @param code Huffman code bits.
 * @param code_length Number of valid bits in code.
 * @param symbol Output symbol.
 * @return APNG_SUCCESS if a matching symbol exists, otherwise APNG_FAIL.
 */
APNG_DEF enum Apng_Return_Types apng_huffman_entry_table_get_symbol(struct Apng_Huffman_Entrys_Table table, uint16_t code, uint8_t code_length, uint16_t *symbol)
{
    for (size_t i = 0; i < table.length; i++) {
        struct Apng_Huffman_Entry entry = table.elements[i];
        if (entry.code == code && entry.code_length == code_length) {
            *symbol = entry.symbol;
            return APNG_SUCCESS;
        }
    }

    return APNG_FAIL;
}

/**
 * @brief Decode the dynamic DEFLATE literal/length and distance code lengths.
 * @param dict_huffman Huffman table used for code-length symbols.
 * @param br Bit reader.
 * @param HLIT Number of literal/length code lengths to decode.
 * @param HDIST Number of distance code lengths to decode.
 * @param lit_len_dist_code_length Output array of HLIT + HDIST code lengths.
 * @return APNG_SUCCESS on success, otherwise APNG_FAIL.
 */
APNG_DEF enum Apng_Return_Types apng_lit_len_dist_code_length_decode(struct Apng_Huffman_Entrys_Table dict_huffman, struct Apng_Bit_Reader *br, uint32_t HLIT, uint32_t HDIST, uint32_t *lit_len_dist_code_length)
{
    uint32_t total = HLIT + HDIST;
    size_t length_index;
    for (length_index = 0; length_index < total;) {
        uint16_t symbol;
        if (APNG_FAIL == apng_huffman_decode_symbol(dict_huffman, br, &symbol)) {
            apng_dprintERROR("%s", "Failed to decode lit len dist code lengths");
            return APNG_FAIL;
        }

        if (symbol <= 15) {
            lit_len_dist_code_length[length_index++] = symbol;
        } else if (symbol == 16) {
            if (length_index == 0) {
                apng_dprintERROR("%s", "Repeat code 16 appeared before any previous code length.");
                return APNG_FAIL;
            }
            uint32_t rep_count = 3 + apng_bit_reader_read_bits(br, 2);
            uint32_t rep_value = lit_len_dist_code_length[length_index - 1];
            if (length_index + rep_count > total) {
                apng_dprintERROR("%s", "Code-length repeat overruns output.");
                return APNG_FAIL;
            }

            for (uint32_t i = 0; i < rep_count; i++) {
                lit_len_dist_code_length[length_index++] = rep_value;
            }
        } else if (symbol == 17) {
            uint32_t rep_count = 3 + apng_bit_reader_read_bits(br, 3);
            if (length_index + rep_count > total) {
                apng_dprintERROR("%s", "Code-length repeat overruns output.");
                return APNG_FAIL;
            }

            for (uint32_t i = 0; i < rep_count; i++) {
                lit_len_dist_code_length[length_index++] = 0;
            }
        } else if (symbol == 18) {
            uint32_t rep_count = 11 + apng_bit_reader_read_bits(br, 7);
            if (length_index + rep_count > total) {
                apng_dprintERROR("%s", "Code-length repeat overruns output.");
                return APNG_FAIL;
            }

            for (uint32_t i = 0; i < rep_count; i++) {
                lit_len_dist_code_length[length_index++] = 0;
            }
        } else {
            apng_dprintERROR("Invalid code-length symbol: %u", symbol);
            return APNG_FAIL;
        }
    }

    APNG_ASSERT(length_index == total);

    return APNG_SUCCESS;
}

/**
 * @brief Allocate a pixel buffer of rows by cols pixels.
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @return Allocated pixel buffer.
 *
 * @note The caller owns the returned buffer and must free elements when no
 *       longer needed.
 */
APNG_DEF struct Apng_Pixel_Buffer apng_pixel_buffer_malloc(size_t rows, size_t cols)
{
    struct Apng_Pixel_Buffer m;
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (uint32_t*)APNG_MALLOC(sizeof(uint32_t)*rows*cols);
    APNG_ASSERT(m.elements != NULL);
    
    return m;
}

/**
 * @brief Decode a PNG image from an in-memory byte string.
 * @param file PNG file contents.
 * @param image Output image structure.
 * @param print_info When true, emit informational diagnostics.
 * @return APNG_SUCCESS on success, otherwise APNG_FAIL.
 */
APNG_DEF enum Apng_Return_Types apng_png_decode(struct Apng_Byte_String file, struct Apng_PNG_Image *image, bool print_info)
{
    image->file = file;
    APNG_UNUSED(file);
    enum Apng_Return_Types rt = APNG_OK;

    if (print_info) apng_dprintINFO("Decoding file: '%s'. File size: %zu bytes", image->file.name, image->file.length);

    struct Apng_PNG_Header png_header = apng_png_header_get(&image->file);
    APNG_ASSERT(apng_png_header_signature_correct(png_header));

    for ( ; image->file.cursor < image->file.length ; ) {
        struct Apng_Chunk_Header chunk_header = apng_chunk_header_get(&image->file);
        // printf("%.*s -> %d bytes\n%d\n", 4, chunk_header.type_str, chunk_header.length, chunk_header.type);
        void *chunk_data = apng_consume_bytes(&image->file, chunk_header.length);
        struct Apng_Chunk_Footer chunk_footer = apng_chunk_footer_get(&image->file);

        if (APNG_FAIL == apng_crc32_check(chunk_header, chunk_data, chunk_footer)) {
            apng_dprintERROR("Failed to decode PNG in file '%s'.", file.name);
            rt = APNG_FAIL;
            goto apng_decode_exit;
        }

        // apng_dprintSTRING(apng_type_name_get(chunk_header.type));
        switch (chunk_header.type) {
            case APNG_TYPE_IHDR: 
            {
                image->chunks.IHDR_chunk.index  = chunk_header.index + chunk_header.size;
                image->chunks.IHDR_chunk.length = chunk_header.length;
                image->chunks.IHDR_chunk.body   = chunk_data;
                rt = apng_IHDR_chunk_parse(&image->chunks.IHDR_chunk);
                if (rt == APNG_FAIL) {
                    apng_dprintERROR("%s", "Failed to parse IHDR chunk.");
                    goto apng_decode_exit;
                }
            } break;
            // case APNG_TYPE_sRGB: 
            // {
            //     image->chunks.sRGB_chunk.index  = chunk_header.index + chunk_header.size;
            //     image->chunks.sRGB_chunk.length = chunk_header.length;
            //     image->chunks.sRGB_chunk.body   = chunk_data;
            // } break;
            // case APNG_TYPE_gAMA: 
            // {
            //     image->chunks.gAMA_chunk.index  = chunk_header.index + chunk_header.size;
            //     image->chunks.gAMA_chunk.length = chunk_header.length;
            //     image->chunks.gAMA_chunk.body   = chunk_data;
            // } break;
            // case APNG_TYPE_pHYs: 
            // {
            //     image->chunks.pHYs_chunk.index  = chunk_header.index + chunk_header.size;
            //     image->chunks.pHYs_chunk.length = chunk_header.length;
            //     image->chunks.pHYs_chunk.body   = chunk_data;
            // } break;
            case APNG_TYPE_IDAT: 
            {
                image->chunks.IDAT_chunk.index  = chunk_header.index + chunk_header.size;
                image->chunks.IDAT_chunk.length = chunk_header.length;
                if (image->chunks.IDAT_chunk.IDAT_data.elements == NULL) {
                    ada_init_array(uint8_t, image->chunks.IDAT_chunk.IDAT_data);
                }
                for (size_t i = 0; i < image->chunks.IDAT_chunk.length; i++) {
                    ada_appand(uint8_t, image->chunks.IDAT_chunk.IDAT_data, ((uint8_t *)chunk_data)[i]);
                }
                rt = apng_IDAT_chunk_parse(&image->chunks.IDAT_chunk);
                if (rt == APNG_FAIL) {
                    apng_dprintERROR("%s", "Failed to parse IDAT chunk.");
                    goto apng_decode_exit;
                }
            } break; 
            case APNG_TYPE_IEND: 
            {
                image->chunks.IEND_chunk.index  = chunk_header.index + chunk_header.size;
                image->chunks.IEND_chunk.length = chunk_header.length;
                image->chunks.IEND_chunk.body   = chunk_data;
            } break;
            case APNG_TYPE_UNKNOWN:
            {
                apng_dprintERROR("%s", "Unknown chunk type.");
                goto apng_decode_exit;
            } break;
            default:
            {
                if (print_info) printf("Chunk %s unused.\n", apng_type_name_get(chunk_header.type));
                bool ancillary = (chunk_header.type_array[0] & 0x20) != 0;
                if (!ancillary) {
                    apng_dprintERROR("Unsupported critical chunk type '%.*s'.", 4, chunk_header.type_array);
                    rt = APNG_FAIL;
                    goto apng_decode_exit;
                }
            } break;
        }
    }
    /* printing INFO */
    float compression_ratio = 0;
    if (image->chunks.IHDR_chunk.color_type == 6) {
        compression_ratio = image->chunks.IHDR_chunk.width * image->chunks.IHDR_chunk.height * 4.0f / image->file.length;
    } else if (image->chunks.IHDR_chunk.color_type == 2) {
        compression_ratio = image->chunks.IHDR_chunk.width * image->chunks.IHDR_chunk.height * 3.0f / image->file.length;
    } else if (image->chunks.IHDR_chunk.color_type == 0) {
        compression_ratio = image->chunks.IHDR_chunk.width * image->chunks.IHDR_chunk.height * 1.0f / image->file.length;
    } else if (image->chunks.IHDR_chunk.color_type == 4) {
        compression_ratio = image->chunks.IHDR_chunk.width * image->chunks.IHDR_chunk.height * 2.0f / image->file.length;
    }
    if (print_info) apng_dprintINFO("The image size is %u x %u || The color type is %u and the bit depth is %u || The compression ratio was %f.",
        image->chunks.IHDR_chunk.width, image->chunks.IHDR_chunk.height, image->chunks.IHDR_chunk.color_type, image->chunks.IHDR_chunk.bit_depth, compression_ratio);

    /* checking PNG file correctness */
    if (image->chunks.IEND_chunk.index != image->file.length - APNG_CHUNK_FOOTER_SIZE) {
        apng_dprintERROR("%s", "Error in IEND chunk.");
        rt = APNG_FAIL;
        goto apng_decode_exit;
    }

    /* decompressing the image */
    {
        rt = apng_IDAT_decode(image);
        if (rt == APNG_FAIL) {
            apng_dprintERROR("%s", "Failed to decode the IDAT chunks.");
            goto apng_decode_exit;
        }
    }

apng_decode_exit:
    return rt;
}

/**
 * @brief Free all memory owned by a decoded PNG image and zero the structure.
 * @param image Image to release.
 */
APNG_DEF void apng_png_free(struct Apng_PNG_Image *image)
{
    if (image == NULL) {
        return;
    }
    apng_byte_string_free(&image->file);
    apng_byte_string_free(&image->chunks.IDAT_chunk.IDAT_data);
    APNG_FREE(image->pixels.elements);

    memset(image, 0, sizeof(*image));
}

/**
 * @brief Check whether a parsed PNG header contains the correct 8-byte
 *        signature.
 * @param h Parsed PNG header.
 * @return true if the signature is valid, otherwise false.
 */
APNG_DEF bool apng_png_header_signature_correct(struct Apng_PNG_Header h)
{
    char correct_signature[] = {137, 80, 78, 71, 13, 10, 26, 10};
    if (h.index != 0) {
        apng_dprintERROR("Incorrect PNG header. Expected at index 0 but got index %zu.", h.index);
        return false;
    }
    if (h.size != 8) {
        apng_dprintERROR("Incorrect PNG header. Expected size 8 but got size %zu.", h.size);
        return false;
    }
    if (memcmp(h.signature, correct_signature, 8) != 0) {
        apng_dprintERROR(
            "Incorrect PNG header. Expected content "
            "{137, 80, 78, 71, 13, 10, 26, 10} but got "
            "{%u, %u, %u, %u, %u, %u, %u, %u}.",
            h.signature[0], h.signature[1], h.signature[2], h.signature[3],
            h.signature[4], h.signature[5], h.signature[6], h.signature[7]
        );
        return false;
    }

    return true;
}

/**
 * @brief Load a PNG file from disk and decode it.
 * @param file_name Path to the PNG file.
 * @param image Output image structure.
 * @param print_info When true, emit informational diagnostics.
 * @return APNG_SUCCESS on success, otherwise APNG_FAIL.
 */
APNG_DEF enum Apng_Return_Types apng_png_load(char *file_name, struct Apng_PNG_Image *image, bool print_info)
{
    struct Apng_Byte_String file = apng_bin_file_read(file_name);
    if (file.name == NULL) {
        apng_dprintERROR("Failed to open file at '%s'.", file_name);
        return APNG_FAIL;
    }
    if (APNG_FAIL == apng_png_decode(file, image, print_info)) {
        apng_dprintERROR("Failed to load png image '%s'.", file_name);
        return APNG_FAIL;
    }

    return APNG_SUCCESS;
}

/**
 * @brief Parse the PNG file signature header from the current cursor.
 * @param bs Byte string whose cursor points at the PNG signature.
 * @return Parsed PNG header.
 */
APNG_DEF struct Apng_PNG_Header apng_png_header_get(struct Apng_Byte_String *bs)
{
    size_t size = 8;
    struct Apng_PNG_Header header = {
        .index = bs->cursor,
        .size = size,
        .signature = (uint8_t *)apng_consume_bytes(bs, size),
    };
    
    return header;
}

/**
 * @brief Print the lowest bit_count bits of a uint16_t in binary.
 * @param value Value to print.
 * @param bit_count Number of bits to print.
 */
APNG_DEF void apng_uint16_print_binary(uint16_t value, uint8_t bit_count)
{
    for (int i = (int)bit_count - 1; i >= 0; i--) {
        printf("%c", (value & (1u << i)) ? '1' : '0');
    }
}

/**
 * @brief Convert a raw 4-byte chunk type to the corresponding enum value.
 * @param raw_type Packed raw chunk type.
 * @return Matching chunk-type enum, or APNG_TYPE_UNKNOWN if unsupported.
 */
APNG_DEF enum Apng_Chunk_Type apng_type_get_from_type_raw(uint32_t raw_type)
{
    if (raw_type == apng_four_char_to_uint32_t("IHDR")) {
        return APNG_TYPE_IHDR;
    } else if (raw_type == apng_four_char_to_uint32_t("PLTE")) {
        return APNG_TYPE_PLTE;
    } else if (raw_type == apng_four_char_to_uint32_t("IDAT")) {
        return APNG_TYPE_IDAT;
    } else if (raw_type == apng_four_char_to_uint32_t("IEND")) {
        return APNG_TYPE_IEND;
    } else if (raw_type == apng_four_char_to_uint32_t("cHRM")) {
        return APNG_TYPE_cHRM;
    } else if (raw_type == apng_four_char_to_uint32_t("gAMA")) {
        return APNG_TYPE_gAMA;
    } else if (raw_type == apng_four_char_to_uint32_t("iCCP")) {
        return APNG_TYPE_iCCP;
    } else if (raw_type == apng_four_char_to_uint32_t("sBIT")) {
        return APNG_TYPE_sBIT;
    } else if (raw_type == apng_four_char_to_uint32_t("sRGB")) {
        return APNG_TYPE_sRGB;
    } else if (raw_type == apng_four_char_to_uint32_t("bKGD")) {
        return APNG_TYPE_bKGD;
    } else if (raw_type == apng_four_char_to_uint32_t("hIST")) {
        return APNG_TYPE_hIST;
    } else if (raw_type == apng_four_char_to_uint32_t("tRNS")) {
        return APNG_TYPE_tRNS;
    } else if (raw_type == apng_four_char_to_uint32_t("pHYs")) {
        return APNG_TYPE_pHYs;
    } else if (raw_type == apng_four_char_to_uint32_t("sPLT")) {
        return APNG_TYPE_sPLT;
    } else if (raw_type == apng_four_char_to_uint32_t("tIME")) {
        return APNG_TYPE_tIME;
    } else if (raw_type == apng_four_char_to_uint32_t("iTXt")) {
        return APNG_TYPE_iTXt;
    } else if (raw_type == apng_four_char_to_uint32_t("tEXt")) {
        return APNG_TYPE_tEXt;
    } else if (raw_type == apng_four_char_to_uint32_t("zTXt")) {
        return APNG_TYPE_zTXt;
    } else {
        return APNG_TYPE_UNKNOWN;
    }
}

/**
 * @brief Return a human-readable name for an Apng_Chunk_Type enum value.
 * @param type Chunk-type enum.
 * @return String representation of the enum value.
 */
APNG_DEF const char * apng_type_name_get(enum Apng_Chunk_Type type)
{
    switch (type) {
        case APNG_TYPE_UNKNOWN:
        {
            return "APNG_TYPE_UNKNOWN";
        } break;
        case APNG_TYPE_IHDR:
        {
            return "APNG_TYPE_IHDR";
        } break;
        case APNG_TYPE_PLTE:
        {
            return "APNG_TYPE_PLTE";
        } break;
        case APNG_TYPE_IDAT:
        {
            return "APNG_TYPE_IDAT";
        } break;
        case APNG_TYPE_IEND:
        {
            return "APNG_TYPE_IEND";
        } break;
        case APNG_TYPE_cHRM:
        {
            return "APNG_TYPE_cHRM";
        } break;
        case APNG_TYPE_gAMA:
        {
            return "APNG_TYPE_gAMA";
        } break;
        case APNG_TYPE_iCCP:
        {
            return "APNG_TYPE_iCCP";
        } break;
        case APNG_TYPE_sBIT:
        {
            return "APNG_TYPE_sBIT";
        } break;
        case APNG_TYPE_sRGB:
        {
            return "APNG_TYPE_sRGB";
        } break;
        case APNG_TYPE_bKGD:
        {
            return "APNG_TYPE_bKGD";
        } break;
        case APNG_TYPE_hIST:
        {
            return "APNG_TYPE_hIST";
        } break;
        case APNG_TYPE_tRNS:
        {
            return "APNG_TYPE_tRNS";
        } break;
        case APNG_TYPE_pHYs:
        {
            return "APNG_TYPE_pHYs";
        } break;
        case APNG_TYPE_sPLT:
        {
            return "APNG_TYPE_sPLT";
        } break;
        case APNG_TYPE_tIME:
        {
            return "APNG_TYPE_tIME";
        } break;
        case APNG_TYPE_iTXt:
        {
            return "APNG_TYPE_iTXt";
        } break;
        case APNG_TYPE_tEXt:
        {
            return "APNG_TYPE_tEXt";
        } break;
        case APNG_TYPE_zTXt:
        {
            return "APNG_TYPE_zTXt";
        } break;
        case APNG_TYPE_COUNT:
        {
            return "APNG_TYPE_COUNT";
        } break;
        default :
        {
            apng_dprintERROR("Unknown APNG_TYPE: '%d'", type);
            exit(1);
        } break;
    }
}

/**
 * @brief Parse and validate the contents of an IHDR chunk.
 * @param chunk IHDR chunk to parse.
 * @return APNG_SUCCESS on success, otherwise APNG_FAIL.
 */
APNG_DEF enum Apng_Return_Types apng_IHDR_chunk_parse(struct Apng_IHDR_Chunk *chunk)
{
    APNG_ASSERT(chunk->body != NULL);
    APNG_ASSERT(chunk->index != 0);
    APNG_ASSERT(chunk->length != 0);
    APNG_ASSERT(chunk->length == 13);

    uint32_t *width = (uint32_t *)&chunk->body[0];
    chunk->width = apng_endian_swap_uint32(*width);
    uint32_t *height = (uint32_t *)&chunk->body[4];
    chunk->height = apng_endian_swap_uint32(*height);
    chunk->bit_depth = chunk->body[8];
    chunk->color_type = chunk->body[9];
    chunk->compression_method = chunk->body[10];
    chunk->filter_method = chunk->body[11];
    chunk->interlace_method = chunk->body[12];

    /* checks */
    // apng_dprintINT(chunk->width);
    // apng_dprintINT(chunk->height);
    // apng_dprintINT(chunk->bit_depth);
    // apng_dprintINT(chunk->color_type);
    // apng_dprintINT(chunk->compression_method);
    // apng_dprintINT(chunk->filter_method);
    // apng_dprintINT(chunk->interlace_method);
    
    if (chunk->color_type == 3 || chunk->color_type == 1 || chunk->color_type > 6) {
        apng_dprintERROR("Unsupported color type. Supports color type 0 and 2 and 4 and 6 but got %d", chunk->color_type);
        return APNG_FAIL;
    } else {
        if (!(chunk->bit_depth == 1 || chunk->bit_depth == 2 || chunk->bit_depth == 4 || chunk->bit_depth == 8)) {
            apng_dprintERROR("Unsupported bit depth. Supports bit depth 1 and 2 and 4 and 8 but got %d", chunk->bit_depth);
            return APNG_FAIL;
        }
    }
    if (chunk->compression_method != 0) {
        apng_dprintERROR("Unsupported compression method. Specification supports compression method 0 but got %d", chunk->compression_method);
        return APNG_FAIL;
    }
    if (chunk->filter_method != 0) {
        apng_dprintERROR("Unsupported filter method. Specification supports filter method 0 but got %d", chunk->filter_method);
        return APNG_FAIL;
    }
    if (chunk->interlace_method != 0) {
        apng_dprintERROR("Unsupported interlace method. Supports interlace method 0 but got %d", chunk->interlace_method);
        return APNG_FAIL;
    }


    return APNG_SUCCESS;
}

/**
 * @brief Parse and validate the zlib header stored in concatenated IDAT data.
 * @param chunk IDAT chunk state.
 * @return APNG_SUCCESS on success, otherwise APNG_FAIL.
 */
APNG_DEF enum Apng_Return_Types apng_IDAT_chunk_parse(struct Apng_IDAT_Chunk *chunk)
{
    /*ZLIB specification: https://www.ietf.org/rfc/rfc1951.txt */

    APNG_ASSERT(chunk->IDAT_data.elements != NULL);
    APNG_ASSERT(chunk->index != 0);
    APNG_ASSERT(chunk->length != 0);

    chunk->header   = apng_IDAT_header_get_from_IDAT_chunk(*chunk);
    chunk->CM       = chunk->header.zlib_compression_method_flags & 0xF;
    chunk->CINFO    = chunk->header.zlib_compression_method_flags >> 4;
    chunk->FCHECK   = chunk->header.additional_flags & ~((~0)<<5);
    chunk->FDICT    = (chunk->header.additional_flags >> 5) & 1;
    chunk->FLEVEL   = chunk->header.additional_flags >> 6;

    chunk->LZ77_window_size = 1ull << (chunk->CINFO + 8);

    /* checks */
    // apng_dprintINT(chunk->CM);
    // apng_dprintINT(chunk->CINFO);
    // apng_dprintINT(chunk->FCHECK);
    // apng_dprintINT(chunk->FDICT);
    // apng_dprintINT(chunk->FLEVEL);
    // apng_dprintSIZE_T(chunk->LZ77_window_size);

    if (chunk->CM != 8) {
        apng_dprintERROR("PNG supports only CM = 8, got %d", chunk->CM);
        return APNG_FAIL;
    }
    if (chunk->CINFO > 7) {
        apng_dprintERROR("CINFO values above 7 are not allowed, got %d", chunk->CINFO);
        return APNG_FAIL;
    }
    if (chunk->FDICT != 0) {
        apng_dprintERROR("Supports only FDICT = 0, got %d", chunk->FDICT);
        return APNG_FAIL;
    }
    if ((chunk->header.zlib_compression_method_flags * 256 + chunk->header.additional_flags) % 31 != 0) {
        apng_dprintERROR("%s", "FCHECK is not set properly");
        return APNG_FAIL;
    }

    return APNG_SUCCESS;
}

/**
 * @brief Decode the image payload from the already-collected IDAT stream.
 * @param image PNG image whose IHDR and IDAT state were previously parsed.
 * @return APNG_SUCCESS on success, otherwise APNG_FAIL.
 */
APNG_DEF enum Apng_Return_Types apng_IDAT_decode(struct Apng_PNG_Image *image)
{
    size_t width = image->chunks.IHDR_chunk.width;
    size_t height = image->chunks.IHDR_chunk.height;
    image->pixels = apng_pixel_buffer_malloc(height, width);

    enum Apng_Return_Types rt = APNG_SUCCESS;

    struct Apng_Byte_String decompress_bs = {0};
    ada_init_array(uint8_t, decompress_bs);
    struct Apng_Byte_String unfiltered_bs = {0};
    ada_init_array(uint8_t, unfiltered_bs);

    rt = apng_IDAT_decompress(image, &decompress_bs);
    if (rt == APNG_FAIL) {
        apng_dprintERROR("%s", "Failed to decompress the IDAT chunks.");
        goto apng_IDAT_decode_end;
    }
    size_t bits_per_pixel = image->chunks.IHDR_chunk.bit_depth *
                        ((image->chunks.IHDR_chunk.color_type == 0) ? 1 :
                         (image->chunks.IHDR_chunk.color_type == 2) ? 3 :
                         (image->chunks.IHDR_chunk.color_type == 4) ? 2 : 4);
    size_t bytes_per_row = (image->chunks.IHDR_chunk.width * bits_per_pixel + 7) / 8;
    size_t expected_size = image->chunks.IHDR_chunk.height * (1 + bytes_per_row);
    if (decompress_bs.length != expected_size) {
        apng_dprintERROR("Decompressed size mismatch. Expected %zu, got %zu", expected_size, decompress_bs.length);
        rt = APNG_FAIL;
        goto apng_IDAT_decode_end;
    }
    /* allocate enough bytes in unfiltered_bs */
    for (size_t i = 0; i < decompress_bs.length; i++) {
        ada_appand(uint8_t, unfiltered_bs, decompress_bs.elements[i]);
    }
    /* set bytes per pixel according to the color type in IHDR */
    size_t num_of_channels = 4;
    size_t bit_per_channel = image->chunks.IHDR_chunk.bit_depth;
    if (image->chunks.IHDR_chunk.color_type == 6) {
        num_of_channels = 4;
    } else if (image->chunks.IHDR_chunk.color_type == 2) {
        num_of_channels = 3;
    } else if (image->chunks.IHDR_chunk.color_type == 0) {
        num_of_channels = 1;
    } else if (image->chunks.IHDR_chunk.color_type == 4) {
        num_of_channels = 2;
    }
    size_t bytes_per_pixel = ((bit_per_channel + 7) / 8) * num_of_channels;

    #if 1
    rt = apng_IDAT_unfiltering(unfiltered_bs.elements, decompress_bs.elements, width, height, num_of_channels, bit_per_channel); 
    if (rt == APNG_FAIL) {
        apng_dprintERROR("%s", "Failed to decompress the IDAT chunks.");
        goto apng_IDAT_decode_end;
    }
    #endif

    /* swizzle and copy the color channels */
    for (size_t i = 0; i < image->pixels.rows; i++) {
        for (size_t j = 0; j < image->pixels.cols; j++) {
            if (image->chunks.IHDR_chunk.color_type == 6) {
                size_t idx = i * bytes_per_row + j * bytes_per_pixel;
                uint8_t r = unfiltered_bs.elements[idx + 0];
                uint8_t g = unfiltered_bs.elements[idx + 1];
                uint8_t b = unfiltered_bs.elements[idx + 2];
                uint8_t a = unfiltered_bs.elements[idx + 3];
                APNG_PIXEL_BUFFER_AT(image->pixels, i, j) = APNG_RGBA_TO_hexARGB(r, g, b, a);
            } else if (image->chunks.IHDR_chunk.color_type == 4) {
                size_t idx = i * bytes_per_row + j * bytes_per_pixel;
                uint8_t value = unfiltered_bs.elements[idx + 0];
                uint8_t a = unfiltered_bs.elements[idx + 1];
                APNG_PIXEL_BUFFER_AT(image->pixels, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, a);
            } else if (image->chunks.IHDR_chunk.color_type == 2) {
                size_t idx = i * bytes_per_row + j * bytes_per_pixel;
                uint8_t r = unfiltered_bs.elements[idx + 0];
                uint8_t g = unfiltered_bs.elements[idx + 1];
                uint8_t b = unfiltered_bs.elements[idx + 2];
                APNG_PIXEL_BUFFER_AT(image->pixels, i, j) = APNG_RGBA_TO_hexARGB(r, g, b, 255);
            } else if (image->chunks.IHDR_chunk.color_type == 0) {
                uint8_t *row = &unfiltered_bs.elements[i * bytes_per_row];
                if (bit_per_channel == 1) {
                    uint8_t packed = row[j / 8];
                    uint8_t bit_index = (uint8_t)(7 - (j % 8));
                    uint8_t sample = (packed >> bit_index) & 0x01;
                    uint8_t value = sample ? 255 : 0;
                    APNG_PIXEL_BUFFER_AT(image->pixels, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, 255);
                } else if (bit_per_channel == 2) {
                    uint8_t packed = row[j / 4];
                    uint8_t bit_index = (uint8_t)(6 - 2 * (j % 4));
                    uint8_t sample = (packed >> bit_index) & 0x03;
                    uint8_t value = (uint8_t)((sample * 255) / 3);
                    APNG_PIXEL_BUFFER_AT(image->pixels, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, 255);
                } else if (bit_per_channel == 4) {
                    uint8_t packed = row[j / 2];
                    uint8_t sample = (j % 2) ? (packed & 0x0F) : (packed >> 4);
                    uint8_t value = (uint8_t)((sample * 255) / 15);
                    APNG_PIXEL_BUFFER_AT(image->pixels, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, 255);
                } else if (bit_per_channel == 8) {
                    uint8_t value = row[j];
                    APNG_PIXEL_BUFFER_AT(image->pixels, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, 255);
                } else {
                    apng_dprintERROR("Unsupported grayscale bit depth: %zu", bit_per_channel);
                    rt = APNG_FAIL;
                    goto apng_IDAT_decode_end;
                }
            }
        }
    }

apng_IDAT_decode_end:
    apng_byte_string_free(&decompress_bs);
    apng_byte_string_free(&unfiltered_bs);
    return rt;
}

struct Apng_Huffman_Entry len_extra[] = {
    {.symbol = 3  , .code_length = 0}, /* 257 */
    {.symbol = 4  , .code_length = 0}, /* 258 */
    {.symbol = 5  , .code_length = 0}, /* 259 */
    {.symbol = 6  , .code_length = 0}, /* 260 */
    {.symbol = 7  , .code_length = 0}, /* 261 */
    {.symbol = 8  , .code_length = 0}, /* 262 */
    {.symbol = 9  , .code_length = 0}, /* 263 */
    {.symbol = 10 , .code_length = 0}, /* 264 */
    {.symbol = 11 , .code_length = 1}, /* 265 */
    {.symbol = 13 , .code_length = 1}, /* 266 */
    {.symbol = 15 , .code_length = 1}, /* 267 */
    {.symbol = 17 , .code_length = 1}, /* 268 */
    {.symbol = 19 , .code_length = 2}, /* 269 */
    {.symbol = 23 , .code_length = 2}, /* 270 */
    {.symbol = 27 , .code_length = 2}, /* 271 */
    {.symbol = 31 , .code_length = 2}, /* 272 */
    {.symbol = 35 , .code_length = 3}, /* 273 */
    {.symbol = 43 , .code_length = 3}, /* 274 */
    {.symbol = 51 , .code_length = 3}, /* 275 */
    {.symbol = 59 , .code_length = 3}, /* 276 */
    {.symbol = 67 , .code_length = 4}, /* 277 */
    {.symbol = 83 , .code_length = 4}, /* 278 */
    {.symbol = 99 , .code_length = 4}, /* 279 */
    {.symbol = 115, .code_length = 4}, /* 280 */
    {.symbol = 131, .code_length = 5}, /* 281 */
    {.symbol = 163, .code_length = 5}, /* 282 */
    {.symbol = 195, .code_length = 5}, /* 283 */
    {.symbol = 227, .code_length = 5}, /* 284 */
    {.symbol = 258, .code_length = 0}, /* 285 */
};

struct Apng_Huffman_Entry dist_extra[] = {
    {.symbol = 1    , .code_length = 0 }, /* 0  */
    {.symbol = 2    , .code_length = 0 }, /* 1  */
    {.symbol = 3    , .code_length = 0 }, /* 2  */
    {.symbol = 4    , .code_length = 0 }, /* 3  */
    {.symbol = 5    , .code_length = 1 }, /* 4  */
    {.symbol = 7    , .code_length = 1 }, /* 5  */
    {.symbol = 9    , .code_length = 2 }, /* 6  */
    {.symbol = 13   , .code_length = 2 }, /* 7  */
    {.symbol = 17   , .code_length = 3 }, /* 8  */
    {.symbol = 25   , .code_length = 3 }, /* 9  */
    {.symbol = 33   , .code_length = 4 }, /* 10 */
    {.symbol = 49   , .code_length = 4 }, /* 11 */
    {.symbol = 65   , .code_length = 5 }, /* 12 */
    {.symbol = 97   , .code_length = 5 }, /* 13 */
    {.symbol = 129  , .code_length = 6 }, /* 14 */
    {.symbol = 193  , .code_length = 6 }, /* 15 */
    {.symbol = 257  , .code_length = 7 }, /* 16 */
    {.symbol = 385  , .code_length = 7 }, /* 17 */
    {.symbol = 513  , .code_length = 8 }, /* 18 */
    {.symbol = 769  , .code_length = 8 }, /* 19 */
    {.symbol = 1025 , .code_length = 9 }, /* 20 */
    {.symbol = 1537 , .code_length = 9 }, /* 21 */
    {.symbol = 2049 , .code_length = 10}, /* 22 */
    {.symbol = 3073 , .code_length = 10}, /* 23 */
    {.symbol = 4097 , .code_length = 11}, /* 24 */
    {.symbol = 6145 , .code_length = 11}, /* 25 */
    {.symbol = 8193 , .code_length = 12}, /* 26 */
    {.symbol = 12289, .code_length = 12}, /* 27 */
    {.symbol = 16385, .code_length = 13}, /* 28 */
    {.symbol = 24577, .code_length = 13}, /* 29 */
};

/**
 * @brief Inflate the concatenated IDAT zlib stream into raw filtered scanline
 *        bytes.
 * @param image PNG image containing parsed IDAT data.
 * @param temp_bs Output byte string for decompressed data.
 * @return APNG_SUCCESS on success, otherwise APNG_FAIL.
 *
 * @pre temp_bs must already be initialized and writable.
 */
APNG_DEF enum Apng_Return_Types apng_IDAT_decompress(struct Apng_PNG_Image *image, struct Apng_Byte_String *temp_bs)
{
    /*ZLIB specification: https://www.ietf.org/rfc/rfc1950.txt */
    /*DEFLATE specification: https://www.ietf.org/rfc/rfc1951.txt */

    struct Apng_Huffman_Entrys_Table huffman_table = {0};
    ada_init_array(struct Apng_Huffman_Entry, huffman_table);

    struct Apng_Bit_Reader temp_br = {
        .bits_left = 0,
        .current_byte = 0,
        .file = image->chunks.IDAT_chunk.IDAT_data,
    };
    temp_br.file.cursor += APNG_IDAT_ZLIB_HEADER_SIZE;/* skipping the zlib header */
    struct Apng_Bit_Reader *br  = &temp_br;

    uint32_t BFINAL;
    uint32_t BTYPE;
    enum Apng_Return_Types rt = APNG_SUCCESS;
    do {
        /* read block header */
        BFINAL = apng_bit_reader_read_bits(br, APNG_BFINAL_SIZE);
        BTYPE  = apng_bit_reader_read_bits(br, APNG_BTYPE_SIZE);

        // apng_dprintINT(BFINAL);
        // apng_dprintINT(BTYPE);
        switch (BTYPE) {
        case 0:
        { 
            /* no compression */
            apng_bit_reader_flash(br);
            // uint16_t LEN  = apng_endian_swap_uint16((uint16_t)apng_bit_reader_read_bits(br, APNG_LEN_SIZE));
            // uint16_t NLEN = apng_endian_swap_uint16((uint16_t)apng_bit_reader_read_bits(br, APNG_NLEN_SIZE));
            uint16_t LEN  = (uint16_t)apng_bit_reader_read_bits(br, APNG_LEN_SIZE);
            uint16_t NLEN = (uint16_t)apng_bit_reader_read_bits(br, APNG_NLEN_SIZE);
            if (LEN != (uint16_t)~NLEN) {
                apng_dprintERROR("%s", "LEN/NLEN mismatch.");
                rt = APNG_FAIL;
                goto apng_IDAT_decompress_end;
            }

            uint8_t *literal_data = apng_consume_bytes(&br->file, LEN);
            for (size_t i = 0; i < LEN; i++) {
                ada_appand(uint8_t, *temp_bs, literal_data[i]);
            }

        } break;
        case 1:
        {
            uint32_t HLIT  = APNG_FIX_HUFFMAN_HLIT;
            uint32_t HDIST = APNG_FIX_HUFFMAN_HDIST;
            uint32_t lit_len_dist_code_length[APNG_FIX_HUFFMAN_HLIT + APNG_FIX_HUFFMAN_HDIST] = {0};
            for (size_t i = 0; i < HLIT + HDIST; i++) {
                if (i >= 0 && i <= 143) {
                    lit_len_dist_code_length[i] = 8;
                } else if (i >= 144 && i <= 255) {
                    lit_len_dist_code_length[i] = 9;
                } else if (i >= 256 && i <= 279) {
                    lit_len_dist_code_length[i] = 7;
                } else if (i >= 280 && i <= 287) {
                    lit_len_dist_code_length[i] = 8;
                } else if (i >= 288 && i <= 319) {
                    lit_len_dist_code_length[i] = 5;
                }
            }

            struct Apng_Huffman_Entrys_Table lit_len_huffman = apng_huffman_entry_table_create(lit_len_dist_code_length, HLIT);
            struct Apng_Huffman_Entrys_Table dist_huffman = apng_huffman_entry_table_create(lit_len_dist_code_length + HLIT, HDIST);

            /* decoding the actual data */
            for (;;) {
                uint16_t lit_len;
                if (APNG_FAIL == apng_huffman_decode_symbol(lit_len_huffman, br, &lit_len)) {
                    apng_dprintERROR("%s", "Failed to decode a lit/len symbol from the actual data.");
                    rt = APNG_FAIL;
                    goto apng_IDAT_decompress_end;
                }

                if (lit_len <= 255) {
                    ada_appand(uint8_t, *temp_bs, (uint8_t)(lit_len & 0xFF));
                } else if (lit_len > 256) {
                    uint32_t len_extra_index = lit_len - 257;
                    struct Apng_Huffman_Entry len_extra_entry = len_extra[len_extra_index];
                    // uint32_t len = (uint32_t)len_extra_entry.symbol + (uint32_t)apng_uint16_bits_reverse((uint16_t)apng_bit_reader_read_bits(br, len_extra_entry.code_length), len_extra_entry.code_length);
                    uint32_t len = (uint32_t)len_extra_entry.symbol + apng_bit_reader_read_bits(br, len_extra_entry.code_length);

                    uint16_t dist_extra_index;
                    if (APNG_FAIL == apng_huffman_decode_symbol(dist_huffman, br, &dist_extra_index)) {
                        apng_dprintERROR("%s", "Failed to decode a dist symbol from the actual data.");
                        rt = APNG_FAIL;
                        goto apng_IDAT_decompress_end;
                    }
                    struct Apng_Huffman_Entry dist_extra_entry = dist_extra[dist_extra_index];
                    // uint32_t dist = (uint32_t)dist_extra_entry.symbol + (uint32_t)apng_uint16_bits_reverse((uint16_t)apng_bit_reader_read_bits(br, dist_extra_entry.code_length), dist_extra_entry.code_length);
                    uint32_t dist = (uint32_t)dist_extra_entry.symbol + apng_bit_reader_read_bits(br, dist_extra_entry.code_length);
                    if (dist == 0 || dist > temp_bs->length) {
                        apng_dprintERROR("%s", "Invalid distance.");
                        rt = APNG_FAIL;
                        goto apng_IDAT_decompress_end;
                    }
                    
                    for (size_t i = 0, src = temp_bs->length - dist; i < len; i++) {
                        uint8_t b = temp_bs->elements[src + i];
                        ada_appand(uint8_t, *temp_bs, b);
                    }
                } else { /* lit_len == 256 */
                    break;
                }
            }

        } break;
        case 2:
        {
            /** dynamic Huffman codes
             * There are 5 sections:
             *  1. Code lengths for code lengths. (to generate the code length Huffman code)
             *  2. Code lengths for the literal/length alphabet (encoded using the code length Huffman code)
             *  3. Code lengths for the distance alphabet (encoded using the code length Huffman code)
             *  4. The actual compressed data of the block. (encoded using the literal/length and distance Huffman codes)
             *  5. The literal/length symbol 256 'end of data' (encoded using the literal/length Huffman code)
            */

            uint32_t HLIT  = apng_bit_reader_read_bits(br, APNG_HLIT_SIZE) + APNG_HLIT_OFFSET;
            uint32_t HDIST = apng_bit_reader_read_bits(br, APNG_HDIST_SIZE) + APNG_HDIST_OFFSET;
            uint32_t HCLEN = apng_bit_reader_read_bits(br, APNG_HCLEN_SIZE) + APNG_HCLEN_OFFSET;

            /* decoding the code length Huffman */
            uint32_t HCLEN_swizzle[] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
            uint32_t code_length_of_code_length[APNG_MAX_NUM_OF_CODE_LENGTH_CODE_LENGTH] = {0};
            for (size_t i = 0; i < HCLEN; i++) {
                code_length_of_code_length[HCLEN_swizzle[i]] = apng_bit_reader_read_bits(br, APNG_CODE_LENGTH_CODE_LENGTH_LENGTH);
            }
            struct Apng_Huffman_Entrys_Table dict_huffman = apng_huffman_entry_table_create(code_length_of_code_length, APNG_STATIC_ARRAY_LEN(code_length_of_code_length));

            /* decoding the lit/len and dist Huffman */
            uint32_t lit_len_dist_code_length[APNG_LIT_LEN_CODE_LENGTH_MAX_COUNT + APNG_DIST_CODE_LENGTH_MAX_COUNT] = {0};
            if (APNG_FAIL == apng_lit_len_dist_code_length_decode(dict_huffman, br, HLIT, HDIST, lit_len_dist_code_length))
            {
                apng_dprintERROR("%s", "Failed to decode dynamic Huffman code lengths.");
                rt = APNG_FAIL;
                goto apng_IDAT_decompress_end;
            }
            struct Apng_Huffman_Entrys_Table lit_len_huffman = apng_huffman_entry_table_create(lit_len_dist_code_length, HLIT);
            struct Apng_Huffman_Entrys_Table dist_huffman = apng_huffman_entry_table_create(lit_len_dist_code_length + HLIT, HDIST);

            /* decoding the actual data */
            for (;;) {
                uint16_t lit_len;
                if (APNG_FAIL == apng_huffman_decode_symbol(lit_len_huffman, br, &lit_len)) {
                    apng_dprintERROR("%s", "Failed to decode a lit/len symbol from the actual data.");
                    rt = APNG_FAIL;
                    goto apng_IDAT_decompress_end;
                }

                if (lit_len <= 255) {
                    ada_appand(uint8_t, *temp_bs, (uint8_t)(lit_len & 0xFF));
                } else if (lit_len > 256) {
                    uint32_t len_extra_index = lit_len - 257;
                    struct Apng_Huffman_Entry len_extra_entry = len_extra[len_extra_index];
                    // uint32_t len = (uint32_t)len_extra_entry.symbol + (uint32_t)apng_uint16_bits_reverse((uint16_t)apng_bit_reader_read_bits(br, len_extra_entry.code_length), len_extra_entry.code_length);
                    uint32_t len = (uint32_t)len_extra_entry.symbol + apng_bit_reader_read_bits(br, len_extra_entry.code_length);

                    uint16_t dist_extra_index;
                    if (APNG_FAIL == apng_huffman_decode_symbol(dist_huffman, br, &dist_extra_index)) {
                        apng_dprintERROR("%s", "Failed to decode a dist symbol from the actual data.");
                        rt = APNG_FAIL;
                        goto apng_IDAT_decompress_end;
                    }
                    struct Apng_Huffman_Entry dist_extra_entry = dist_extra[dist_extra_index];
                    // uint32_t dist = (uint32_t)dist_extra_entry.symbol + (uint32_t)apng_uint16_bits_reverse((uint16_t)apng_bit_reader_read_bits(br, dist_extra_entry.code_length), dist_extra_entry.code_length);
                    uint32_t dist = (uint32_t)dist_extra_entry.symbol + apng_bit_reader_read_bits(br, dist_extra_entry.code_length);
                    if (dist == 0 || dist > temp_bs->length) {
                        apng_dprintERROR("%s", "Invalid distance.");
                        rt = APNG_FAIL;
                        goto apng_IDAT_decompress_end;
                    }
                    
                    for (size_t i = 0, src = temp_bs->length - dist; i < len; i++) {
                        uint8_t b = temp_bs->elements[src + i];
                        ada_appand(uint8_t, *temp_bs, b);
                    }
                } else { /* lit_len == 256 */
                    break;
                }
            }
        } break;
        case 3:
        {
            apng_dprintERROR("%s", "BTYPE of 3 encountered. Spec does not supports this.");
            rt = APNG_FAIL;
            goto apng_IDAT_decompress_end;
        } break;
        default:
        {
            apng_dprintERROR("%s", "UNREACHABLE");
            rt = APNG_FAIL;
            goto apng_IDAT_decompress_end;
        }
        }
    } while (!BFINAL);

    /* Adler-32 check */
    if (br->bits_left != 0) {
        apng_bit_reader_flash(br);
    }
    uint32_t *original_adler32_ptr = (uint32_t *)apng_consume_bytes(&br->file, 4);
    uint32_t original_adler32 = apng_endian_swap_uint32(*original_adler32_ptr);
    rt = apng_adler32_check(original_adler32, temp_bs->elements, temp_bs->length);
    if (rt == APNG_FAIL) {
        apng_dprintERROR("%s", "Failed to decompress the data correctly, adler32 error.");
        goto apng_IDAT_decompress_end;
    }

apng_IDAT_decompress_end:
    APNG_FREE(huffman_table.elements);
    return rt;
}

/**
 * @brief Reverse PNG scanline filtering.
 * @param unfiltered_data Output buffer for reconstructed scanline bytes.
 * @param decompressed_data Input buffer containing filter bytes and filtered rows.
 * @param width Image width in pixels.
 * @param height Image height in pixels.
 * @param num_of_channels Number of channels per pixel.
 * @param bit_per_channel Bit depth of each channel.
 * @return APNG_SUCCESS on success, otherwise APNG_FAIL.
 */
APNG_DEF enum Apng_Return_Types apng_IDAT_unfiltering(uint8_t *unfiltered_data, uint8_t *decompressed_data, size_t width, size_t height, size_t num_of_channels, size_t bit_per_channel)
{
    uint8_t *src = decompressed_data;
    uint8_t *des = unfiltered_data;
    uint8_t *row_above = NULL;
    size_t bits_per_pixel = num_of_channels * bit_per_channel;
    size_t bytes_in_pixel = (bits_per_pixel + 7) / 8;
    size_t width_in_bytes = (width * bits_per_pixel + 7) / 8;

    for (size_t r = 0; r < height; r++) {
        uint8_t filter = *src++;
        uint8_t *current_row = des;
        switch (filter) {
        case 0:
        {
            for (size_t x = 0; x < width_in_bytes; x++) {
                current_row[x] = src[x];
            }
        } break;
        case 1:
        {
            for (size_t x = 0; x < width_in_bytes; x++) {
                uint8_t a_byte = (x >= bytes_in_pixel) ? current_row[x - bytes_in_pixel] : 0 ;

                current_row[x] = (uint8_t)src[x] + (uint8_t)a_byte;
            }
        } break;
        case 2:
        {
            for (size_t x = 0; x < width_in_bytes; x++) {
                uint8_t b_byte = row_above ? row_above[x] : 0;

                current_row[x] = (uint8_t)src[x] + (uint8_t)b_byte;
            }
        } break;
        case 3:
        {
            for (size_t x = 0; x < width_in_bytes; x++) {
                uint8_t a_byte = (x >= bytes_in_pixel) ? current_row[x - bytes_in_pixel] : 0 ;
                uint8_t b_byte = row_above ? row_above[x] : 0;

                current_row[x] = (uint8_t)src[x] + (uint8_t)(((uint32_t)a_byte + (uint32_t)b_byte) / 2);
            }
        } break;
        case 4:
        {
            for (size_t x = 0; x < width_in_bytes; x++) {
                uint8_t a_byte = (x >= bytes_in_pixel) ? current_row[x - bytes_in_pixel] : 0 ;
                uint8_t b_byte = row_above ? row_above[x] : 0;
                uint8_t c_byte = (x >= bytes_in_pixel && row_above) ? row_above[x - bytes_in_pixel] : 0 ;

                int p = (int)a_byte + (int)b_byte - (int)c_byte;
                int pa = p - a_byte;
                if (pa < 0) pa = -pa;
                int pb = p - b_byte;
                if (pb < 0) pb = -pb;
                int pc = p - c_byte;
                if (pc < 0) pc = -pc;

                int paeth = (int)c_byte;
                if ((pa <= pb) && (pa <= pc)) {
                    paeth = (int)a_byte;
                } else if (pb <= pc) {
                    paeth = (int)b_byte;
                }

                current_row[x] = (uint8_t)src[x] + (uint8_t)paeth;
            }
        } break;
        default: 
        {
            apng_dprintERROR("Unknown row filter :%d", filter);
            return APNG_FAIL;
        }
        }
        src += width_in_bytes;
        row_above = current_row;
        des += width_in_bytes;
    }

    return APNG_SUCCESS;
}

/**
 * @brief Read the zlib header fields from the start of concatenated IDAT data.
 * @param chunk IDAT chunk state.
 * @return Parsed IDAT/zlib header.
 */
APNG_DEF struct Apng_IDAT_Header apng_IDAT_header_get_from_IDAT_chunk(struct Apng_IDAT_Chunk chunk)
{
    APNG_ASSERT(chunk.IDAT_data.elements != NULL);
    APNG_ASSERT(chunk.index != 0);
    APNG_ASSERT(chunk.length != 0);
    APNG_ASSERT(chunk.length >= 2);
    
    size_t size = APNG_IDAT_ZLIB_HEADER_SIZE;
    struct Apng_IDAT_Header header = {
        .index = chunk.index,
        .size = size,
    };
    header.zlib_compression_method_flags = chunk.IDAT_data.elements[0];
    header.additional_flags = chunk.IDAT_data.elements[1];

    return header;
}

#endif /*ALMOG_PNG_IMPLEMENTATION*/
