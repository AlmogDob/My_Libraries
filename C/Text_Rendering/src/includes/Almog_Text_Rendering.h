/**
 * Apple's reference manual is at: https://developer.apple.com/fonts/TrueType-Reference-Manual/.
 */
#ifndef ALMOG_TEXT_RENDERING_H_
#define ALMOG_TEXT_RENDERING_H_

#if defined(_WIN32) || defined(_WIN64) 
    #pragma warning(disable : 4709)
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#ifndef ATR_ASSERT
    #include <assert.h>
    #define ATR_ASSERT assert
#endif
#ifndef ATR_MALLOC
    #include <stdlib.h>
    #define ATR_MALLOC malloc
#endif
#ifndef ATR_FREE
    #include <stdlib.h>
    #define ATR_FREE free
#endif
#ifndef ATR_REALLOC
    #include <stdlib.h>
    #define ATR_REALLOC realloc
#endif

/*{*/
    /* For explanation see https://github.com/AlmogDob/My_Libraries/tree/master/C/Dynamic_Array*/

    #ifndef ATR_ADA_INIT_CAPACITY
    #define ATR_ADA_INIT_CAPACITY 10
    #endif /*ATR_ADA_INIT_CAPACITY*/

    #ifndef ATR_ADA_MALLOC
    #define ATR_ADA_MALLOC ATR_MALLOC
    #endif /*ATR_ADA_MALLOC*/

    #ifndef ATR_ADA_REALLOC
    #define ATR_ADA_REALLOC ATR_REALLOC
    #endif /*ATR_ADA_REALLOC*/

    #ifndef ATR_ADA_ASSERT
    #define ATR_ADA_ASSERT ATR_ASSERT
    #endif /*ATR_ADA_ASSERT*/

    /* typedef struct {
        size_t length;
        size_t capacity;
        int* elements;
    } atr_ada_int_array; */

    #define atr_ada_init_array(type, header) do {                                               \
            (header).capacity = ATR_ADA_INIT_CAPACITY;                                          \
            (header).length = 0;                                                                \
            (header).elements = (type *)ATR_ADA_MALLOC(sizeof(type) * (header).capacity);       \
            ATR_ADA_ASSERT((header).elements != NULL);                                          \
        } while (0)

    #define atr_ada_resize(type, header, new_capacity) do {                                                                 \
            type *atr_ada_temp_pointer = (type *)ATR_ADA_REALLOC((void *)((header).elements), new_capacity*sizeof(type));   \
            ATR_ADA_ASSERT(atr_ada_temp_pointer != NULL);                                                                   \
            (header).elements = atr_ada_temp_pointer;                                                                       \
            ATR_ADA_ASSERT((header).elements != NULL);                                                                      \
            (header).capacity = new_capacity;                                                                               \
        } while (0)

    #define atr_ada_append(type, header, value) do {                                                \
            if ((header).length >= (header).capacity) {                                             \
                atr_ada_resize(type, (header), (int)((header).capacity + (header).capacity/2 + 1)); \
            }                                                                                       \
            (header).elements[(header).length] = value;                                             \
            (header).length++;                                                                      \
        } while (0)

    #define atr_ada_insert(type, header, value, index) do {                                                                             \
        ATR_ADA_ASSERT((int)(index) >= 0);                                                                                              \
        ATR_ADA_ASSERT((float)(index) - (int)(index) == 0);                                                                             \
        ATR_ADA_ASSERT((header).length > 0 && "You can not insert to an empty array.");                                                 \
        ATR_ADA_ASSERT(index <= (header).length);                                                                                       \
        atr_ada_append(type, (header), (header).elements[(header).length-1]);                                                           \
        for (int atr_ada_for_loop_index = (int)((header).length)-2; atr_ada_for_loop_index > (int)(index); atr_ada_for_loop_index--) {  \
            (header).elements[atr_ada_for_loop_index] = (header).elements [atr_ada_for_loop_index-1];                                   \
        }                                                                                                                               \
        (header).elements[(index)] = value;                                                                                             \
    } while (0)

    #define atr_ada_insert_unordered(type, header, value, index) do {   \
        ATR_ADA_ASSERT((int)(index) >= 0);                              \
        ATR_ADA_ASSERT((float)(index) - (int)(index) == 0);             \
        ATR_ADA_ASSERT(index <= (header).length);                       \
        if ((size_t)(index) == (header).length) {                       \
            atr_ada_append(type, (header), value);                      \
        } else {                                                        \
            atr_ada_append(type, (header), (header).elements[(index)]); \
            (header).elements[(index)] = value;                         \
        }                                                               \
    } while (0)

    #define atr_ada_remove(type, header, index) do {                                                                            \
        ATR_ADA_ASSERT((int)(index) >= 0);                                                                                      \
        ATR_ADA_ASSERT((header).length > 0 && "You can not remove from an empty array.");                                       \
        ATR_ADA_ASSERT((float)(index) - (int)(index) == 0);                                                                     \
        ATR_ADA_ASSERT(index < (header).length);                                                                                \
        for (size_t atr_ada_for_loop_index = (index); atr_ada_for_loop_index < (header).length-1; atr_ada_for_loop_index++) {   \
            (header).elements[atr_ada_for_loop_index] = (header).elements[atr_ada_for_loop_index+1];                            \
        }                                                                                                                       \
        (header).length--;                                                                                                      \
    } while (0)

    #define atr_ada_remove_unordered(type, header, index) do {                              \
        ATR_ADA_ASSERT((int)(index) >= 0);                                                  \
        ATR_ADA_ASSERT((header).length > 0 && "You can not remove from an empty array.");   \
        ATR_ADA_ASSERT(index < (header).length);                                            \
        ATR_ADA_ASSERT((float)(index) - (int)(index) == 0);                                 \
        (header).elements[index] = (header).elements[(header).length-1];                    \
        (header).length--;                                                                  \
    } while (0)
/*}*/

#ifndef atr_real
    #if defined(ATR_SINGLE_PRECISION)
        typedef float atr_real_type;
        #define ATR_EPS   1e-5f
        #define atr_fabs  fabsf
        #define atr_floor floorf
        #define atr_ceil  ceilf
        #define atr_sqrt  sqrtf
        #define atr_cbrt  cbrtf
        #define atr_cos   cosf
        #define atr_sin   sinf
        #define atr_atan2 atan2f
        #define atr_fmod  fmodf
    #else 
        typedef double atr_real_type;
        #define ATR_EPS   1e-10
        #define atr_fabs  fabs
        #define atr_floor floor
        #define atr_ceil  ceil
        #define atr_sqrt  sqrt
        #define atr_cbrt  cbrt
        #define atr_cos   cos
        #define atr_sin   sin
        #define atr_atan2 atan2
        #define atr_fmod  fmod
    #endif
    #define atr_real atr_real_type
#endif

#ifndef ATR_PI
    #define ATR_PI (atr_real)3.14159265358979323846
#endif

enum Atr_Return_Types {
    ATR_FAIL,
    ATR_SUCCESS,
};

struct Atr_Byte_String {
    char *name;
    size_t capacity;
    size_t length;
    size_t cursor;
    uint8_t *elements;
};

struct Atr_Bit_Reader {
    struct Atr_Byte_String file;
    uint8_t current_byte;
    uint8_t bits_left;
};

struct Atr_Offset_Zoom {
    atr_real zoom_multiplier;
    atr_real offset_x;
    atr_real offset_y;
};

struct Atr_Pixel_Buffer {
    size_t rows;
    size_t cols;
    size_t stride_r;
    uint32_t *elements;
};

struct Atr_Offset_Subtable {
    uint32_t scaler_type;
    uint16_t numTables;
    uint16_t searchRange;
    uint16_t entrySelector;
    uint16_t rangeShift;
};

struct Atr_Table_Header {
    union {
        uint32_t tag_raw;
        char tag_str[4];
        uint8_t tag_array[4];
    };  
    uint32_t checkSum;
    uint32_t offset;
    uint32_t length;
};

struct Atr_Table_cmap {
    struct Atr_Table_Header header;
};

struct Atr_Table_glyf {
    struct Atr_Table_Header header;
};

struct Atr_Table_head {
    struct Atr_Table_Header header;
};

struct Atr_Table_hhea {
    struct Atr_Table_Header header;
};

struct Atr_Table_hmtx {
    struct Atr_Table_Header header;
};

struct Atr_Table_loca {
    struct Atr_Table_Header header;
};

struct Atr_Table_maxp {
    struct Atr_Table_Header header;
};

struct Atr_Table_name {
    struct Atr_Table_Header header;
};

struct Atr_Table_post {
    struct Atr_Table_Header header;
};

struct Atr_Font {
    struct Atr_Byte_String file;
    struct Atr_Offset_Subtable offset_subtable;
    struct {
        struct Atr_Table_cmap cmap;
        struct Atr_Table_glyf glyf;
        struct Atr_Table_head head;
        struct Atr_Table_hhea hhea;
        struct Atr_Table_hmtx hmtx;
        struct Atr_Table_loca loca;
        struct Atr_Table_maxp maxp;
        struct Atr_Table_name name;
        struct Atr_Table_post post;
    } tables;
};

#define atr_dprintSTRING(expr) printf("[Info] %s:%d:\n%*s" #expr " = %s\n", __FILE__, __LINE__, 7, "", expr)
#define atr_dprintCHAR(expr) printf("[Info] %s:%d:\n%*s" #expr " = %c\n", __FILE__, __LINE__, 7, "", expr)
#define atr_dprintINT(expr) printf("[Info] %s:%d:\n%*s" #expr " = %d\n", __FILE__, __LINE__, 7, "", expr)
#define atr_dprintFLOAT(expr) printf("[Info] %s:%d:\n%*s" #expr " = %#f\n", __FILE__, __LINE__, 7, "", expr)
#define atr_dprintDOUBLE(expr) printf("[Info] %s:%d:\n%*s" #expr " = %#g\n", __FILE__, __LINE__, 7, "", expr)
#define atr_dprintSIZE_T(expr) printf("[Info] %s:%d:\n%*s" #expr " = %zu\n", __FILE__, __LINE__, 7, "", expr)
#define atr_dprintINFO(fmt, ...) \
    fprintf(stderr, "[Info] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 7, "", __func__, 7, "", __VA_ARGS__)
#define atr_dprintWARNING(fmt, ...) \
    fprintf(stderr, "[Warning] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 10, "", __func__, 10, "", __VA_ARGS__)
#define atr_dprintERROR(fmt, ...) \
    fprintf(stderr, "[Error] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 8, "", __func__, 8, "", __VA_ARGS__)

#define atr_min(a, b) ((a) < (b) ? (a) : (b))
#define atr_max(a, b) ((a) > (b) ? (a) : (b))
#define ATR_IS_ZERO(x) (atr_fabs(x) < ATR_EPS)
#define ATR_BUFFER_AT(m, i, j) (m).elements[(ATR_ASSERT((i) < (m).rows && (j) < (m).cols), (i) * (m).stride_r + (j))]
#define ATR_UNUSED(x) ((void)x)

#define ATR_DEFAULT_OFFSET_ZOOM (struct Atr_Offset_Zoom){.zoom_multiplier = 1, .offset_x = 0, .offset_y = 0}
#define ATR_TABLE_HEADER_SIZE 16
#define ATR_OFFSET_SUBTABLE_SIZE 12

#ifndef ATR_DEF
    #ifdef ATR_DEF_STATIC
        #define ATR_DEF static
    #else
        #define ATR_DEF extern
    #endif
#endif

ATR_DEF uint32_t                atr_4chars_to_uint32_t(const char *chars);
ATR_DEF void                    atr_bit_reader_flash(struct Atr_Bit_Reader *br);
ATR_DEF void                    atr_bit_reader_init(struct Atr_Bit_Reader *br, struct Atr_Byte_String file);
ATR_DEF uint8_t                 atr_bit_reader_read_bit(struct Atr_Bit_Reader *br);
ATR_DEF uint32_t                atr_bit_reader_read_bits(struct Atr_Bit_Reader *br, size_t count);
ATR_DEF uint8_t                 atr_bit_reader_read_byte(struct Atr_Bit_Reader *br);
ATR_DEF uint32_t                atr_bit_reader_read_bytes(struct Atr_Bit_Reader *br, size_t count);

ATR_DEF struct Atr_Byte_String  atr_byte_string_get_from_binary_file_name(char *file_name);
ATR_DEF struct Atr_Byte_String  atr_byte_string_get_from_binary_file_pointer(FILE *fp, char *file_name);
ATR_DEF void                    atr_byte_string_free(struct Atr_Byte_String *bs);

ATR_DEF uint16_t                atr_endian_swap_uint16(uint16_t x);
ATR_DEF uint32_t                atr_endian_swap_uint32(uint32_t x);

ATR_DEF enum Atr_Return_Types   atr_font_load_from_file_name(struct Atr_Font *font, char *file_name);

ATR_DEF enum Atr_Return_Types   atr_offset_subtable_parse(struct Atr_Font *font);

ATR_DEF uint32_t                atr_table_checksum_calc(uint32_t *table, uint32_t length_bytes);
ATR_DEF struct Atr_Table_Header atr_table_header_parse(struct Atr_Font *font, size_t offset);
ATR_DEF atr_real                atr_text_line_draw(struct Atr_Pixel_Buffer screen, struct Atr_Font *font, char *text, atr_real top_left_x, atr_real top_left_y, atr_real letter_width, atr_real letter_hight, atr_real letter_spacing, uint32_t color, size_t length);

                                #define atr_uint16_print_binary(value, bit_count) atr_dprintINFO("%s = ", #value); printf("%*.s", 7, ""); atr_uint16_print_binary_imp((value), (bit_count))
ATR_DEF void                    atr_uint16_print_binary_imp(uint16_t value, uint8_t bit_count);
                                #define atr_uint16_print_hex(value, bit_count) atr_dprintINFO("%s = ", #value); printf("%*.s", 7, ""); atr_uint16_print_hex_imp((value), (bit_count))
ATR_DEF void                    atr_uint16_print_hex_imp(uint16_t value, uint8_t bit_count);
                                #define atr_uint32_print_binary(value, bit_count) atr_dprintINFO("%s = ", #value); printf("%*.s", 7, ""); atr_uint32_print_binary_imp((value), (bit_count))
ATR_DEF void                    atr_uint32_print_binary_imp(uint32_t value, uint8_t bit_count);
                                #define atr_uint32_print_hex(value, bit_count) atr_dprintINFO("%s = ", #value); printf("%*.s", 7, ""); atr_uint32_print_hex_imp((value), (bit_count))
ATR_DEF void                    atr_uint32_print_hex_imp(uint32_t value, uint8_t bit_count);


#endif /*ALMOG_TEXT_RENDERING_H_*/

#ifdef ALMOG_TEXT_RENDERING_IMPLEMENTATION
#undef ALMOG_TEXT_RENDERING_IMPLEMENTATION


ATR_DEF uint32_t atr_4chars_to_uint32_t(const char *chars)
{
    return ((uint32_t)(((uint32_t)((chars)[0]) << 0) | ((uint32_t)((chars)[1]) << 8) | ((uint32_t)((chars)[2]) << 16) | ((uint32_t)((chars)[3]) << 24)));
}
 
/**
 * @brief Discard any unread bits cached in the bit reader.
 * @param br Bit reader to reset to the next byte boundary.
 */
ATR_DEF void atr_bit_reader_flash(struct Atr_Bit_Reader *br)
{
    br->current_byte = 0;
    br->bits_left = 0;
}

/**
 * @brief Initialize a bit reader over a byte string.
 * @param br Bit reader to initialize.
 * @param file Source byte string.
 */
ATR_DEF void atr_bit_reader_init(struct Atr_Bit_Reader *br, struct Atr_Byte_String file)
{
    br->file = file;
    br->current_byte = 0;
    br->bits_left = 0;
}

/**
 * @brief Read one bit from a byte stream.
 *
 * This bit reader consumes bits least-significant-bit first from each byte. When the cached byte is exhausted, it loads
 * the next byte from the underlying byte string and continues reading.
 *
 * @param br Bit reader state.
 * @return The next bit from the compressed stream, either 0 or 1.
 *
 * @pre br->file.cursor must not advance past the end of the underlying buffer.
 * @post The reader state is advanced by one bit.
 */
ATR_DEF uint8_t atr_bit_reader_read_bit(struct Atr_Bit_Reader *br)
{
    if (br->bits_left == 0) {
        ATR_ASSERT(br->file.cursor < br->file.length);
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
 * @brief Read multiple bits.
 *
 * Reads count bits using atr_bit_reader_read_bit() and packs them into the low
 * bits of the result in the same order they were read.
 *
 * @param br Bit reader state.
 * @param count Number of bits to read, up to 32.
 * @return A 32-bit value containing the requested bits.
 */
ATR_DEF uint32_t atr_bit_reader_read_bits(struct Atr_Bit_Reader *br, size_t count)
{
    ATR_ASSERT(count <= 32);

    uint32_t res = 0;

    for (size_t i = 0; i < count; i++) {
        res |= ((uint32_t)atr_bit_reader_read_bit(br)) << i;
    }

    return res;
}

ATR_DEF uint8_t atr_bit_reader_read_byte(struct Atr_Bit_Reader *br)
{
    uint8_t res = 0;

    for (size_t i = 0; i < 8; i++) {
        res |= ((uint8_t)atr_bit_reader_read_bit(br)) << i;
    }

    return res;
}

ATR_DEF uint32_t atr_bit_reader_read_bytes(struct Atr_Bit_Reader *br, size_t count)
{
    ATR_ASSERT(count <= 4);

    uint32_t res = 0;

    for (size_t i = 0; i < count; i++) {
        res |= ((uint32_t)atr_bit_reader_read_byte(br)) << (i * 8);
    }

    return res;
}

/**
 * @brief Read an entire binary file into a byte-string structure.
 * @param file_name Path to the file to read.
 * @return A byte string containing the file contents. On failure, the returned
 *         structure is zero-initialized as much as possible.
 *
 * @note The returned object owns heap memory and must be released with
 *       atr_byte_string_free().
 */
ATR_DEF struct Atr_Byte_String atr_byte_string_get_from_binary_file_name(char *file_name)
{
    struct Atr_Byte_String res = {0};
    
    FILE *fp = fopen(file_name, "rb");
    if (fp == NULL) {
        int err = errno;
        atr_dprintERROR("Cannot open file %s: %s", file_name, strerror(err));
        return res;
    }

    return atr_byte_string_get_from_binary_file_pointer(fp, file_name);
}

ATR_DEF struct Atr_Byte_String atr_byte_string_get_from_binary_file_pointer(FILE *fp, char *file_name)
{
    struct Atr_Byte_String res = {0};

    char temp_file_name = '\0';
    if (!file_name) file_name = &temp_file_name;

    if (fseek(fp, 0, SEEK_END) != 0) {
        int err = errno;
        atr_dprintERROR( "Failed to seek to end of file %s: %s", file_name, strerror(err));
        fclose(fp);
        return res;
    }

    long size = ftell(fp);
    if (size < 0) {
        int err = errno;
        atr_dprintERROR( "Failed to tell size of file %s: %s", file_name, strerror(err));
        fclose(fp);
        return res;
    }

    if (fseek(fp, 0, SEEK_SET) != 0) {
        int err = errno;
        atr_dprintERROR( "Failed to seek to start of file %s: %s", file_name, strerror(err));
        fclose(fp);
        return res;
    }

    res.name = ATR_MALLOC(sizeof(char) * (1 + strlen(file_name)));
    if (res.name == NULL) {
        atr_dprintERROR( "Memory allocation failed for file %s (%zu bytes).", file_name, strlen(file_name));
        fclose(fp);
        res.length = 0;
        return res;
    }
    strncpy(res.name, file_name, strlen(file_name)+1);

    res.length = (size_t)size;
    res.capacity = (size_t)size;
    res.elements = ATR_MALLOC(res.length);
    if (res.elements == NULL) {
        atr_dprintERROR( "Memory allocation failed for file %s (%zu bytes).", file_name, res.length);
        fclose(fp);
        res.length = 0;
        ATR_FREE(res.name);
        return res;
    }

    size_t nread = fread(res.elements, 1, res.length, fp);
    if (nread != res.length) {
        if (ferror(fp)) {
            int err = errno;
            atr_dprintERROR( "Failed to read file %s: %s", file_name, strerror(err));
        } else {
            atr_dprintERROR(
                "Unexpected end of file while reading %s "
                "(expected %zu bytes, got %zu).", file_name, res.length, nread);
        }
        ATR_FREE(res.elements);
        ATR_FREE(res.name);
        res.elements = NULL;
        res.length = 0;
        fclose(fp);
        return res;
    }

    if (fclose(fp) != 0) {
        int err = errno;
        atr_dprintERROR( "Failed to close file %s: %s", file_name, strerror(err));
        ATR_FREE(res.elements);
        ATR_FREE(res.name);
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
ATR_DEF void atr_byte_string_free(struct Atr_Byte_String *bs)
{
    ATR_FREE(bs->elements);
    ATR_FREE(bs->name);
    bs->capacity = 0;
    bs->length = 0;
    bs->cursor = 0;
}

/**
 * @brief Swap the byte order of a 16-bit unsigned integer.
 * @param x Input value.
 * @return Byte-swapped value.
 */
ATR_DEF uint16_t atr_endian_swap_uint16(uint16_t x)
{
    return ((x << 8) | (x >> 8));
}

/**
 * @brief Swap the byte order of a 32-bit unsigned integer.
 * @param x Input value.
 * @return Byte-swapped value.
 */
ATR_DEF uint32_t atr_endian_swap_uint32(uint32_t x)
{
    return ((x << 24) |
            ((x & 0xFF00) << 8) |
            ((x >> 8) & 0xFF00) |
            (x >> 24));
}

ATR_DEF enum Atr_Return_Types atr_font_load_from_file_name(struct Atr_Font *font, char *file_name)
{
    font->file = atr_byte_string_get_from_binary_file_name(file_name);
    
    if (ATR_FAIL == atr_offset_subtable_parse(font)) {
        atr_dprintERROR("Failed to parse offset subtable of font at '%s'", file_name);
    }
    for (size_t table_index = 0; table_index < font->offset_subtable.numTables; table_index++) {
        struct Atr_Table_Header th = atr_table_header_parse(font, ATR_OFFSET_SUBTABLE_SIZE + table_index * ATR_TABLE_HEADER_SIZE);
        atr_dprintINFO("%.4s", th.tag_str);
    }

    return ATR_SUCCESS;
}

ATR_DEF enum Atr_Return_Types atr_offset_subtable_parse(struct Atr_Font *font)
{
    struct Atr_Bit_Reader br = {0};
    atr_bit_reader_init(&br, font->file);

    font->offset_subtable.scaler_type   = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br, 4));
    font->offset_subtable.numTables     = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->offset_subtable.searchRange   = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->offset_subtable.entrySelector = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->offset_subtable.rangeShift    = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));

    /* Checks */
    if (!(atr_4chars_to_uint32_t("true") == font->offset_subtable.scaler_type ||
        0x00010000 == font->offset_subtable.scaler_type)) {
            if (atr_4chars_to_uint32_t("typ1") == font->offset_subtable.scaler_type) {
                atr_dprintERROR("%s", "Font type recognized as the old style of PostScript font housed in a sfnt wrapper. This type is not supported.");
                return ATR_FAIL;
            }
            if (atr_4chars_to_uint32_t("OTTO") == font->offset_subtable.scaler_type) {
                atr_dprintERROR("%s", "Font type recognized as an OpenType font with PostScript outlines. This type is not supported.");
                return ATR_FAIL;
            }
            {
                atr_dprintERROR("Font type not recognized. Only supports TrueType fonts.\n%*.sGot:", 8, "");
                printf("%*.s", 8, ""); atr_uint32_print_hex_imp(font->offset_subtable.scaler_type, 32);
                printf("%*.sExpected\n%*.s0xtrue or 0x00010000.\n", 8, "", 8, "");
            }
            return ATR_FAIL;
    }

    
    return ATR_SUCCESS;
}

ATR_DEF uint32_t atr_table_checksum_calc(uint32_t *table, uint32_t length_bytes)
{
    uint32_t sum = 0;
    uint32_t nlongs = (length_bytes + 3 ) / 4;
    while (nlongs-- > 0) {
        sum += *table++;
    }
    return sum;
}

ATR_DEF struct Atr_Table_Header atr_table_header_parse(struct Atr_Font *font, size_t offset_byte)
{
    struct Atr_Bit_Reader br = {0};
    atr_bit_reader_init(&br, font->file);
    br.file.cursor = offset_byte;

    struct Atr_Table_Header th = {0};
    th.tag_raw  = atr_bit_reader_read_bytes(&br, 4);
    th.checkSum = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br, 4));
    th.offset   = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br, 4));
    th.length   = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br, 4));

    return th;
}

ATR_DEF atr_real atr_text_line_draw(struct Atr_Pixel_Buffer screen, struct Atr_Font *font, char *text, atr_real top_left_x, atr_real top_left_y, atr_real letter_width, atr_real letter_hight, atr_real letter_spacing, uint32_t color, size_t length)
{
    ATR_UNUSED(screen);
    ATR_UNUSED(font);
    ATR_UNUSED(text);
    ATR_UNUSED(top_left_x);
    ATR_UNUSED(top_left_y);
    ATR_UNUSED(letter_width);
    ATR_UNUSED(letter_hight);
    ATR_UNUSED(letter_spacing);
    ATR_UNUSED(color);
    ATR_UNUSED(length);

    return 0;
}

/**
 * @brief Print the lowest bit_count bits of a uint16_t in binary.
 * @param value Value to print.
 * @param bit_count Number of bits to print.
 */
ATR_DEF void atr_uint16_print_binary_imp(uint16_t value, uint8_t bit_count)
{
    ATR_ASSERT(bit_count <= 16);
    printf("b");
    for (int i = (int)bit_count - 1; i >= 0; i--) {
        printf("%c", (value & (1u << i)) ? '1' : '0');
    }
    printf("\n");
}

ATR_DEF void atr_uint16_print_hex_imp(uint16_t value, uint8_t bit_count)
{
    ATR_ASSERT(bit_count <= 16);

    /* Ignore bits above bit_count, including for partial nibbles. */
    if (bit_count < 16) {
        value &= (1u << bit_count) - 1u;
    }

    const uint8_t hex_digit_count = (uint8_t)((bit_count + 3u) / 4u);

    printf("0x");
    for (int i = (int)hex_digit_count - 1; i >= 0; i--) {
        unsigned int digit = (unsigned int)((value >> (i * 4)) & 0xFu);
        printf("%X", digit);
    }
    printf("\n");
}

/**
 * @brief Print the lowest bit_count bits of a uint32_t in binary.
 * @param value Value to print.
 * @param bit_count Number of bits to print.
 */
ATR_DEF void atr_uint32_print_binary_imp(uint32_t value, uint8_t bit_count)
{
    ATR_ASSERT(bit_count <= 32);
    printf("b");
    for (int i = (int)bit_count - 1; i >= 0; i--) {
        printf("%c", (value & (1u << i)) ? '1' : '0');
    }
    printf("\n");
}

ATR_DEF void atr_uint32_print_hex_imp(uint32_t value, uint8_t bit_count)
{
    ATR_ASSERT(bit_count <= 32);

    /* Ignore bits above bit_count, including for partial nibbles. */
    if (bit_count < 32) {
        value &= (1u << bit_count) - 1u;
    }

    const uint8_t hex_digit_count = (uint8_t)((bit_count + 3u) / 4u);

    printf("0x");
    for (int i = (int)hex_digit_count - 1; i >= 0; i--) {
        unsigned int digit = (unsigned int)((value >> (i * 4)) & 0xFu);
        printf("%X", digit);
    }
    printf("\n");
}


#endif /*ALMOG_TEXT_RENDERING_IMPLEMENTATION*/
