/**
 * Apple's reference manual is at: https://developer.apple.com/fonts/TrueType-Reference-Manual/.
 */
#ifndef ALMOG_TEXT_RENDERING_H_
#define ALMOG_TEXT_RENDERING_H_

#if defined(_WIN32) || defined(_WIN64) 
    #pragma warning(disable : 4709)
#endif

#include <stdio.h>
#include <errno.h>
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

struct Atr_Table_cmap_Group {
    uint32_t startCharCode;
    uint32_t endCharCode;
    uint32_t startGlyphCode;
};

enum Atr_cmap_Priority {
    ATR_cmap_PRIORITY_NONE = 0,
    ATR_cmap_PRIORITY_SYMBOL = 1,
    ATR_cmap_PRIORITY_LAST_RESORT = 2,
    ATR_cmap_PRIORITY_UNICODE_BMP = 3,
    ATR_cmap_PRIORITY_UNICODE_FULL = 4,
};

struct Atr_Table_cmap_Subtable {
    uint16_t platformID;
    uint16_t platformSpecificID;
    uint32_t relative_offset;
    uint32_t absolute_offset;
    uint16_t format;
    union {
        struct {
            uint16_t length;
            uint16_t language;
            uint8_t  glyphIndexArray[256];
        } format_0;
        struct {
            uint16_t length;
            uint16_t language;
            uint16_t segCountx2;
            uint16_t searchRange;
            uint16_t entrySelector;
            uint16_t rangeShift;

            uint16_t *endCode;
            uint16_t *startCode;
            uint16_t *idDelta;
            uint16_t *idRangeOffset;

            size_t    glyphIdexCount;
            uint16_t *glyphIdexArray;
        } format_4;
        struct {
            uint16_t length;
            uint16_t language;
            uint16_t firstCode;
            uint16_t entryCount;
            uint16_t *glyphIdexArray;
        } format_6;
        struct {
            uint32_t length;
            uint32_t language;
            uint32_t nGroups;
            struct Atr_Table_cmap_Group *groups;
        } format_12;
        struct {
            uint32_t length;
            uint32_t language;
            uint32_t nGroups;
            struct Atr_Table_cmap_Group *groups;
        } format_13;
    } data; /* 0 4 6 12 13 ?*/
};

struct Atr_Table_cmap {
    struct Atr_Table_Header header;
    uint16_t version;
    uint16_t numberSubtables;
    struct {
        size_t length;
        size_t capacity;
        struct Atr_Table_cmap_Subtable *elements;
    } subtables;
    uint16_t chosen_subtable_index;
    uint16_t variation_subtable_index;
    bool has_variation_subtable;
};

struct Atr_Table_glyf {
    struct Atr_Table_Header header;
};

struct Atr_Table_head {
    struct Atr_Table_Header header;
    uint16_t version_hole_part;
    uint16_t version_frac_part;
    uint16_t fontRevision_hole_part;
    uint16_t fontRevision_frac_part;
    uint32_t checkSumAdjustment;
    uint32_t magicNumber;
    uint16_t flags;
    uint16_t unitsPerEm;
    uint64_t created;
    uint64_t modified;
    int16_t  xMin;
    int16_t  yMin;
    int16_t  xMax;
    int16_t  yMax;
    uint16_t macStyle;
    uint16_t lowestRecPPEM;
    int16_t  fontDirectionHint;
    int16_t  indexToLocFormat;
    int16_t  glyphDataFormat;
};

struct Atr_Table_hhea {
    struct Atr_Table_Header header;
};

struct Atr_Table_hmtx {
    struct Atr_Table_Header header;
};

struct Atr_Table_loca {
    struct Atr_Table_Header header;
    int16_t indexToLocFormat;
    uint16_t numGlyphs;
    size_t length;
    uint32_t *offsets;
};

struct Atr_Table_maxp {
    struct Atr_Table_Header header;
    uint16_t version_hole_part;
    uint16_t version_frac_part;
    uint16_t numGlyphs;
    uint16_t maxPoints;
    uint16_t maxContours;
    uint16_t maxComponentPoints;
    uint16_t maxComponentContours;
    uint16_t maxZones;
    uint16_t maxTwilightPoints;
    uint16_t maxStorage;
    uint16_t maxFunctionDefs;
    uint16_t maxInstructionDefs;
    uint16_t maxStackElements;
    uint16_t maxSizeOfInstructions;
    uint16_t maxComponentElements;
    uint16_t maxComponentDepth;
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
        size_t length;
        struct Atr_Table_Header *elements;
    } table_directory;
    struct {
        struct Atr_Table_head head;
        struct Atr_Table_cmap cmap;
        struct Atr_Table_maxp maxp;
        struct Atr_Table_glyf glyf;
        struct Atr_Table_loca loca;
        struct Atr_Table_hhea hhea;
        struct Atr_Table_hmtx hmtx;
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

ATR_DEF uint32_t                    atr_4chars_to_uint32_t(const char *chars);
ATR_DEF void                        atr_bit_reader_flush(struct Atr_Bit_Reader *br);
ATR_DEF void                        atr_bit_reader_init(struct Atr_Bit_Reader *br, struct Atr_Byte_String file);
ATR_DEF void                        atr_bit_reader_init_bounded(struct Atr_Bit_Reader *br, struct Atr_Byte_String file, size_t start_offset, size_t end_offset);
ATR_DEF uint8_t                     atr_bit_reader_read_bit(struct Atr_Bit_Reader *br);
ATR_DEF uint32_t                    atr_bit_reader_read_bits(struct Atr_Bit_Reader *br, size_t count);
ATR_DEF uint8_t                     atr_bit_reader_read_byte(struct Atr_Bit_Reader *br);
ATR_DEF uint32_t                    atr_bit_reader_read_bytes(struct Atr_Bit_Reader *br, size_t count);

ATR_DEF struct Atr_Byte_String      atr_byte_string_get_from_binary_file_name(char *file_name);
ATR_DEF struct Atr_Byte_String      atr_byte_string_get_from_binary_file_pointer(FILE *fp, char *file_name);
ATR_DEF void                        atr_byte_string_free(struct Atr_Byte_String *bs);

ATR_DEF uint16_t                    atr_endian_swap_uint16(uint16_t x);
ATR_DEF uint32_t                    atr_endian_swap_uint32(uint32_t x);

ATR_DEF void                        atr_font_free(struct Atr_Font *font);
ATR_DEF enum Atr_Return_Types       atr_font_load_from_file_name(struct Atr_Font *font, char *file_name);

ATR_DEF uint32_t                    atr_glyphIndex_get(struct Atr_Font *font, uint32_t code_point);

ATR_DEF enum Atr_Return_Types       atr_offset_subtable_parse(struct Atr_Font *font);

ATR_DEF uint32_t                    atr_table_checkSum_calc(const uint8_t *bytes, size_t length, int zero_begin, int zero_end);
ATR_DEF void                        atr_table_cmap_free(struct Atr_Font *font);
ATR_DEF enum Atr_Return_Types       atr_table_cmap_parse(struct Atr_Font *font, struct Atr_Table_Header cmap_header);
ATR_DEF enum Atr_Return_Types       atr_table_cmap_subtable_choose(struct Atr_Font *font);
ATR_DEF enum Atr_cmap_Priority      atr_table_cmap_subtable_priority_score(struct Atr_Table_cmap_Subtable *subtable);
ATR_DEF enum Atr_Return_Types       atr_table_directory_parse(struct Atr_Font *font);
ATR_DEF struct Atr_Table_Header *   atr_table_header_find_by_tag_raw(struct Atr_Font *font, uint32_t tag_raw);
ATR_DEF struct Atr_Table_Header     atr_table_header_parse(struct Atr_Font *font, size_t offset);
ATR_DEF enum Atr_Return_Types       atr_table_header_verify_checksum(struct Atr_Font *font, struct Atr_Table_Header header, int checkSumAdjustment_offset);
ATR_DEF enum Atr_Return_Types       atr_table_head_parse(struct Atr_Font *font, struct Atr_Table_Header head_header);
ATR_DEF enum Atr_Return_Types       atr_table_loca_parse(struct Atr_Font *font, struct Atr_Table_Header loca_header);
ATR_DEF enum Atr_Return_Types       atr_table_maxp_parse(struct Atr_Font *font, struct Atr_Table_Header maxp_header);
ATR_DEF atr_real                    atr_text_line_draw(struct Atr_Pixel_Buffer screen, struct Atr_Font *font, char *text, atr_real top_left_x, atr_real top_left_y, atr_real letter_width, atr_real letter_hight, atr_real letter_spacing, uint32_t color, size_t length);

                                    #define atr_uint16_print_binary(value, bit_count) atr_dprintINFO("%s = ", #value); printf("%*.s", 7, ""); atr_uint16_print_binary_imp((value), (bit_count))
ATR_DEF void                        atr_uint16_print_binary_imp(uint16_t value, uint8_t bit_count);
                                    #define atr_uint16_print_hex(value, bit_count) atr_dprintINFO("%s = ", #value); printf("%*.s", 7, ""); atr_uint16_print_hex_imp((value), (bit_count))
ATR_DEF void                        atr_uint16_print_hex_imp(uint16_t value, uint8_t bit_count);
                                    #define atr_uint32_print_binary(value, bit_count) atr_dprintINFO("%s = ", #value); printf("%*.s", 7, ""); atr_uint32_print_binary_imp((value), (bit_count))
ATR_DEF void                        atr_uint32_print_binary_imp(uint32_t value, uint8_t bit_count);
                                    #define atr_uint32_print_hex(value, bit_count) atr_dprintINFO("%s = ", #value); printf("%*.s", 7, ""); atr_uint32_print_hex_imp((value), (bit_count))
ATR_DEF void                        atr_uint32_print_hex_imp(uint32_t value, uint8_t bit_count);


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
ATR_DEF void atr_bit_reader_flush(struct Atr_Bit_Reader *br)
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

ATR_DEF void atr_bit_reader_init_bounded(struct Atr_Bit_Reader *br, struct Atr_Byte_String file, size_t start_offset, size_t end_offset)
{
    ATR_ASSERT(br != NULL);
    ATR_ASSERT(start_offset <= end_offset);
    ATR_ASSERT(start_offset <= file.length);
    ATR_ASSERT(end_offset <= file.length);

    atr_bit_reader_init(br, file);

    uint8_t *base = br->file.elements;
    br->file.elements = base + start_offset;
    br->file.length = end_offset - start_offset;
    br->file.capacity = br->file.length;
    br->file.cursor = 0;
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
        atr_dprintERROR("Memory allocation failed for file %s (%zu bytes).", file_name, res.length);
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
    bs->elements = NULL;
    bs->name = NULL;
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

ATR_DEF void atr_font_free(struct Atr_Font *font)
{
    ATR_ASSERT(font);

    atr_byte_string_free(&(font->file));

    ATR_FREE(font->table_directory.elements);
    font->table_directory.elements = NULL;
    font->table_directory.length = 0;

    atr_table_cmap_free(font);

    ATR_FREE(font->tables.loca.offsets);
    font->tables.loca.offsets = NULL;
    font->tables.loca.length = 0;

    *font = (struct Atr_Font){0};
}

ATR_DEF enum Atr_Return_Types atr_font_load_from_file_name(struct Atr_Font *font, char *file_name)
{
    struct Atr_Font loaded = {0};
    if (font == NULL) {
        return ATR_FAIL;
    }
    loaded.file = atr_byte_string_get_from_binary_file_name(file_name);
    if (loaded.file.elements == NULL) {
        return ATR_FAIL;
    }
    if (loaded.file.length < ATR_OFFSET_SUBTABLE_SIZE) {
        atr_dprintERROR("%s", "File is too small to contain an sfnt header.");
        goto fail;
    }
    if (atr_offset_subtable_parse(&loaded) == ATR_FAIL) {
        goto fail;
    }
    if (atr_table_directory_parse(&loaded) == ATR_FAIL) {
        goto fail;
    }
    const struct Atr_Table_Header *head_header = atr_table_header_find_by_tag_raw(&loaded, atr_4chars_to_uint32_t("head"));
    const struct Atr_Table_Header *maxp_header = atr_table_header_find_by_tag_raw(&loaded, atr_4chars_to_uint32_t("maxp"));
    const struct Atr_Table_Header *loca_header = atr_table_header_find_by_tag_raw(&loaded, atr_4chars_to_uint32_t("loca"));
    const struct Atr_Table_Header *glyf_header = atr_table_header_find_by_tag_raw(&loaded, atr_4chars_to_uint32_t("glyf"));
    const struct Atr_Table_Header *cmap_header = atr_table_header_find_by_tag_raw(&loaded, atr_4chars_to_uint32_t("cmap"));
    if (head_header == NULL || maxp_header == NULL ||
        loca_header == NULL || glyf_header == NULL ||
        cmap_header == NULL) {
        atr_dprintERROR("%s", "Font is missing one or more required TrueType tables.");
        goto fail;
    }
    /*
     * cmap does not depend on loca, head, or maxp. It can be
     * parsed after the required metadata tables.
     */
    if (atr_table_header_verify_checksum(&loaded, *cmap_header, -1) == ATR_FAIL) {
        goto fail;
    }
    if (atr_table_cmap_parse(&loaded, *cmap_header) == ATR_FAIL) {
        goto fail;
    }
    /*
     * Parse head. loca depends on indexToLocFormat.
     */
    if (atr_table_header_verify_checksum(&loaded, *head_header, 8) == ATR_FAIL) {
        goto fail;
    }
    if (atr_table_head_parse(&loaded, *head_header) == ATR_FAIL) {
        goto fail;
    }
    /*
     * Parse maxp. loca depends on numGlyphs.
     */
    if (atr_table_header_verify_checksum(&loaded, *maxp_header, -1) == ATR_FAIL) {
        goto fail;
    }
    if (atr_table_maxp_parse(&loaded, *maxp_header) == ATR_FAIL) {
        goto fail;
    }
    /*
     * glyf has no fields to parse yet, but save its directory record.
     * loca offsets are relative to the beginning of glyf.
     */
    if (atr_table_header_verify_checksum(&loaded, *glyf_header, -1) == ATR_FAIL) {
        goto fail;
    }
    loaded.tables.glyf.header = *glyf_header;
    /*
     * loca is parsed only after head and maxp and glyf-header are available.
     */
    if (atr_table_header_verify_checksum(&loaded, *loca_header, -1) == ATR_FAIL) {
        goto fail;
    }
    if (atr_table_loca_parse(&loaded, *loca_header) == ATR_FAIL) {
        goto fail;
    }

    /*
     * Commit the successfully parsed font.
     *
     * The caller must have initialized *font to {0}, or have loaded
     * it previously using this API.
     */
    atr_font_free(font);
    *font = loaded;

    return ATR_SUCCESS;

    fail:
        atr_font_free(&loaded);
        return ATR_FAIL;
}

ATR_DEF uint32_t atr_glyphIndex_get(struct Atr_Font *font, uint32_t code_point)
{
    struct Atr_Table_cmap_Subtable st = font->tables.cmap.subtables.elements[font->tables.cmap.chosen_subtable_index];
    ATR_ASSERT(st.format != 14);

    if (st.format == 0) {
        for (size_t i = 0; i < st.data.format_0.length; i++) {
            if (st.data.format_0.glyphIndexArray[i] == (uint8_t)code_point) {
                return (uint32_t)i;
            }
        }
        return 0;
    } else if (st.format == 4) {
        return 0;
    } else if (st.format == 12) {
        /** TODO:
         * Implement binary search for improved performance. The charCodes should be sorted.
         */
        for (size_t i = 0; i < st.data.format_12.nGroups; i++) {
            struct Atr_Table_cmap_Group cg = st.data.format_12.groups[i];
            if (cg.startCharCode <= code_point && code_point <= cg.endCharCode) {
                return code_point - cg.startCharCode + cg.startGlyphCode;
            }
        }
        return 0;
    } else if (st.format == 13) {
        for (size_t i = 0; i < st.data.format_12.nGroups; i++) {
            struct Atr_Table_cmap_Group cg = st.data.format_12.groups[i];
            if (cg.startCharCode <= code_point && code_point <= cg.endCharCode) {
                return cg.startGlyphCode;
            }
        }
        return 0;
    } else {
        return 0;
    }
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

ATR_DEF uint32_t atr_table_checkSum_calc(const uint8_t *bytes, size_t length, int zero_begin, int zero_end)
{
    /* By AI */
    uint32_t sum = 0;

    for (size_t i = 0; i < length; i += 4) {
        uint32_t word = 0;
        for (size_t j = 0; j < 4; j++) {
            size_t index = i + j;
            uint8_t byte = 0;
            if (index < length &&
                !(index >= zero_begin && index < zero_end)) {
                byte = bytes[index];
            }
            /*
             * TrueType/OpenType checksums use big-endian words.
             * Missing bytes at the end are zero-padding.
             */
            word |= (uint32_t)byte << (24 - j * 8);
        }

        sum += word;
    }

    return sum;
}

ATR_DEF void atr_table_cmap_free(struct Atr_Font *font)
{
    ATR_ASSERT(font);
    struct Atr_Table_cmap *cmap = &font->tables.cmap;
    for (size_t i = 0; i < cmap->subtables.length; i++) {
        struct Atr_Table_cmap_Subtable *st = &cmap->subtables.elements[i];
        if (st->format == 4) {
            ATR_FREE(st->data.format_4.endCode);
            ATR_FREE(st->data.format_4.glyphIdexArray);
            ATR_FREE(st->data.format_4.idDelta);
            ATR_FREE(st->data.format_4.idRangeOffset);
            ATR_FREE(st->data.format_4.startCode);
        }
        if (st->format == 12) {
            ATR_FREE(st->data.format_12.groups);
        }
        if (st->format == 13) {
            ATR_FREE(st->data.format_13.groups);
        }
    }

    ATR_FREE(cmap->subtables.elements);
    *cmap = (struct Atr_Table_cmap){0};
}

ATR_DEF enum Atr_Return_Types atr_table_cmap_parse(struct Atr_Font *font, struct Atr_Table_Header cmap_header)
{
    struct Atr_Bit_Reader br = {0};
    atr_bit_reader_init(&br, font->file);
    br.file.cursor = cmap_header.offset;

    struct Atr_Bit_Reader br_st = {0};
    atr_bit_reader_init(&br_st, font->file);

    atr_ada_init_array(struct Atr_Table_cmap_Subtable, font->tables.cmap.subtables);
    font->tables.cmap.header          = cmap_header;
    font->tables.cmap.version         = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.cmap.numberSubtables = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));

    for (size_t i = 0; i < font->tables.cmap.numberSubtables; i++) {
        struct Atr_Table_cmap_Subtable st = {
            .platformID         = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2)),
            .platformSpecificID = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2)),
            .relative_offset    = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br, 4)),
        };
        if (st.relative_offset >= cmap_header.length) {
            atr_dprintERROR("%s", "cmap subtable offset is invalid.");
            return ATR_FAIL;
        }
        st.absolute_offset    = cmap_header.offset + st.relative_offset,
        br_st.file.cursor = st.absolute_offset;
        st.format = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));

        /* Parse different cmap subtable formats */
        if (st.format == 0) {
            st.data.format_0.length   = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            if (st.data.format_0.length != 262) {
                atr_dprintERROR("Error while parsing cmap subtable with format 0. Got length of %u, but expected length of 262", st.data.format_0.length);
                return ATR_FAIL;
            }
            st.data.format_0.language = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            for (size_t gi = 0; gi < 256; gi++) {
                st.data.format_0.glyphIndexArray[gi] = atr_bit_reader_read_byte(&br_st);
            }
            st.data.format_0.length = 256;
        } else if (st.format == 4) {
            st.data.format_4.length                 = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            st.data.format_4.language               = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
             
            st.data.format_4.segCountx2             = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            st.data.format_4.searchRange            = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            st.data.format_4.entrySelector          = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            st.data.format_4.rangeShift             = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));

            st.data.format_4.endCode                = ATR_MALLOC(sizeof(uint16_t) * st.data.format_4.segCountx2 / 2);
            ATR_ASSERT(st.data.format_4.endCode);
            for (size_t gi = 0; gi < st.data.format_4.segCountx2 / 2; gi++) {
                st.data.format_4.endCode[gi]        = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            }
            atr_bit_reader_read_bytes(&br_st, 2); /* reserved */
            st.data.format_4.startCode              = ATR_MALLOC(sizeof(uint16_t) * st.data.format_4.segCountx2 / 2);
            ATR_ASSERT(st.data.format_4.startCode);
            for (size_t gi = 0; gi < st.data.format_4.segCountx2 / 2; gi++) {
                st.data.format_4.startCode[gi]      = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            }
            st.data.format_4.idDelta                = ATR_MALLOC(sizeof(uint16_t) * st.data.format_4.segCountx2 / 2);
            ATR_ASSERT(st.data.format_4.idDelta);
            for (size_t gi = 0; gi < st.data.format_4.segCountx2 / 2; gi++) {
                st.data.format_4.idDelta[gi]        = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            }
            st.data.format_4.idRangeOffset          = ATR_MALLOC(sizeof(uint16_t) * st.data.format_4.segCountx2 / 2);
            ATR_ASSERT(st.data.format_4.idRangeOffset);
            for (size_t gi = 0; gi < st.data.format_4.segCountx2 / 2; gi++) {
                st.data.format_4.idRangeOffset[gi]  = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            }
            size_t fixed_size = 16 + 4 * st.data.format_4.segCountx2;
            if (fixed_size >= st.data.format_4.length) {
                atr_dprintERROR("%s", "Error while parsing cmap subtable with format 4.");
                return ATR_FAIL;
            }
            size_t glyph_id_bytes = st.data.format_4.length - fixed_size;
            if (glyph_id_bytes % 2 != 0) {
                return ATR_FAIL;
            }
            st.data.format_4.glyphIdexCount = glyph_id_bytes / sizeof(uint16_t);
            st.data.format_4.glyphIdexArray         = ATR_MALLOC(sizeof(uint16_t) * st.data.format_4.glyphIdexCount);
            ATR_ASSERT(st.data.format_4.glyphIdexArray);
            for (size_t gi = 0; gi < st.data.format_4.glyphIdexCount; gi++) {
                st.data.format_4.glyphIdexArray[gi] = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            }
        } else if (st.format == 12) {
            atr_bit_reader_read_bytes(&br_st, 2); /* reserved */
            st.data.format_12.length   = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
            st.data.format_12.language = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
            st.data.format_12.nGroups  = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
            st.data.format_12.groups   = ATR_MALLOC(sizeof(struct Atr_Table_cmap_Group) * st.data.format_12.nGroups);
            ATR_ASSERT(st.data.format_12.groups);
            for (size_t gi = 0; gi < st.data.format_12.nGroups; gi++) {
                st.data.format_12.groups[gi].startCharCode  = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
                st.data.format_12.groups[gi].endCharCode    = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
                st.data.format_12.groups[gi].startGlyphCode = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
            }
        } else if (st.format == 13) {
            atr_bit_reader_read_bytes(&br_st, 2); /* reserved */
            st.data.format_13.length   = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
            st.data.format_13.language = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
            st.data.format_13.nGroups  = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
            st.data.format_13.groups   = ATR_MALLOC(sizeof(struct Atr_Table_cmap_Group) * st.data.format_13.nGroups);
            ATR_ASSERT(st.data.format_13.groups);
            for (size_t gi = 0; gi < st.data.format_13.nGroups; gi++) {
                st.data.format_13.groups[gi].startCharCode  = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
                st.data.format_13.groups[gi].endCharCode    = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
                st.data.format_13.groups[gi].startGlyphCode = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
            }
        } else {
            ; /* Unsupported format */
        }
        atr_ada_append(struct Atr_Table_cmap_Subtable, font->tables.cmap.subtables, st);
    }

    if (ATR_FAIL == atr_table_cmap_subtable_choose(font)) {
        atr_dprintERROR("%s", "Could not find a supported cmap subtable. Unable to continue to parse the font. Only supports formats: 0/4/12/13.");
        return ATR_FAIL;
    }

    /* Checks */
    if (font->tables.cmap.version != 0) {
        atr_dprintERROR("cmap version is incorrect. Got %u but expected 0.", font->tables.cmap.version);
        return ATR_FAIL;
    }

    /*
    atr_dprintINT(font->tables.cmap.version);
    atr_dprintINT(font->tables.cmap.numberSubtables);
    for (size_t i = 0; i < font->tables.cmap.numberSubtables; i++) {
        atr_dprintINFO("subtable index: %zu", i);
        atr_dprintINT(font->tables.cmap.subtables.elements[i].platformID);
        atr_dprintINT(font->tables.cmap.subtables.elements[i].platformSpecificID);
        atr_dprintINT(font->tables.cmap.subtables.elements[i].relative_offset);
        atr_dprintINT(font->tables.cmap.subtables.elements[i].format);
        if (font->tables.cmap.subtables.elements[i].format == 12) {
            atr_dprintINT(font->tables.cmap.subtables.elements[i].data.format_12.nGroups);
        }
        if (font->tables.cmap.subtables.elements[i].format == 13) {
            atr_dprintINT(font->tables.cmap.subtables.elements[i].data.format_13.nGroups);
        }
    }
    atr_dprintINT(font->tables.cmap.chosen_subtable_index);
    */

    return ATR_SUCCESS;
}

ATR_DEF enum Atr_Return_Types atr_table_cmap_subtable_choose(struct Atr_Font *font)
{
    struct Atr_Table_cmap *cmap = &font->tables.cmap;
    uint16_t best_index = 0;
    int best_score = 0;
    bool primary_found = false;
    bool variation_found = false;

    for (uint16_t i = 0; i < cmap->subtables.length; ++i) {
        struct Atr_Table_cmap_Subtable *subtable = &cmap->subtables.elements[i];

        /** 
         * Format 14 is supplemental. Do not consider it as the
         * primary character-to-glyph mapping.
         */
        if (subtable->platformID == 0 && subtable->platformSpecificID == 5 && subtable->format == 14) {
            cmap->variation_subtable_index = i;
            variation_found = true;
            continue;
        }

        enum Atr_cmap_Priority score = atr_table_cmap_subtable_priority_score(subtable);

        /**
         * Using '>' means the first equally ranked encoding wins.
         * This is deterministic because cmap records have a required sorting order.
         */
        if (score > best_score) {
            best_score = score;
            best_index = i;
            primary_found = true;
        }
    }

    if (!primary_found) {
        return ATR_FAIL;
    }
    cmap->chosen_subtable_index = best_index;

    /**
     * Format 14 can supplement only a Unicode format 4 or 12 cmap.
     */
    const struct Atr_Table_cmap_Subtable *primary = &cmap->subtables.elements[best_index];
    bool primary_supports_variations = primary->format == 4 || primary->format == 12;
    cmap->has_variation_subtable = variation_found && primary_supports_variations;

    return ATR_SUCCESS;
}

ATR_DEF enum Atr_cmap_Priority atr_table_cmap_subtable_priority_score(struct Atr_Table_cmap_Subtable *subtable)
{
    uint16_t platform = subtable->platformID;
    uint16_t encoding = subtable->platformSpecificID;
    uint16_t format = subtable->format;

    /**
     * Full-repertoire Unicode.
     *
     * Apple does not define an order between 0/4 and 3/10, so they
     * receive the same score. The first one encountered wins.
     */
    if (format == 12) {
        if (platform == 0 && encoding == 4) {
            return ATR_cmap_PRIORITY_UNICODE_FULL;
        }

        if (platform == 3 && encoding == 10) {
            return ATR_cmap_PRIORITY_UNICODE_FULL;
        }
    }

    /**
     * BMP-only Unicode.
     *
     * Apple does not define an order between 0/3 and 3/1.
     */
    if (format == 4) {
        if (platform == 0 && encoding == 3) {
            return ATR_cmap_PRIORITY_UNICODE_BMP;
        }

        if (platform == 3 && encoding == 1) {
            return ATR_cmap_PRIORITY_UNICODE_BMP;
        }
    }

    /**
     * Format 13 is a specialized last-resort mapping.
     */
    if (platform == 0 && encoding == 6 && format == 13) {
        return ATR_cmap_PRIORITY_LAST_RESORT;
    }

    /**
     * Windows Symbol is non-Unicode and is only a fallback.
     */
    if (platform == 3 && encoding == 0 && format == 4) {
        return ATR_cmap_PRIORITY_SYMBOL;
    }

    return ATR_cmap_PRIORITY_NONE;
}

ATR_DEF enum Atr_Return_Types atr_table_directory_parse(struct Atr_Font *font)
{
    /* By AI */
    size_t count = font->offset_subtable.numTables;
    size_t directory_size = ATR_OFFSET_SUBTABLE_SIZE + count * ATR_TABLE_HEADER_SIZE;

    if (directory_size > font->file.length) {
        atr_dprintERROR("%s", "Table directory lies outside the file.");
        return ATR_FAIL;
    }

    font->table_directory.elements = ATR_MALLOC(sizeof(struct Atr_Table_Header) * count);
    if (font->table_directory.elements == NULL && count != 0) {
        atr_dprintERROR("%s", "Failed to allocate table directory.");
        return ATR_FAIL;
    }
    font->table_directory.length = count;

    for (size_t i = 0; i < count; i++) {
        size_t offset = ATR_OFFSET_SUBTABLE_SIZE + i * ATR_TABLE_HEADER_SIZE;
        struct Atr_Table_Header header = atr_table_header_parse(font, offset);

        if (header.offset > font->file.length || header.length > font->file.length - header.offset) {
            atr_dprintERROR("Table %.4s lies outside the font file.", header.tag_str);
            return ATR_FAIL;
        }

        for (size_t previous = 0; previous < i; previous++) {
            if (font->table_directory.elements[previous].tag_raw == header.tag_raw) {
                atr_dprintERROR("Duplicate table tag %.4s.", header.tag_str);
                return ATR_FAIL;
            }
        }

        font->table_directory.elements[i] = header;
    }

    return ATR_SUCCESS;
}

ATR_DEF struct Atr_Table_Header * atr_table_header_find_by_tag_raw(struct Atr_Font *font, uint32_t tag_raw)
{
    for (size_t i = 0; i < font->table_directory.length; i++) {
        struct Atr_Table_Header *header = &font->table_directory.elements[i];
        if (header->tag_raw == tag_raw) {
            return header;
        }
    }

    return NULL;
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

ATR_DEF enum Atr_Return_Types atr_table_header_verify_checksum(struct Atr_Font *font, struct Atr_Table_Header header, int checkSumAdjustment_offset)
{
    /* By AI */
    if (header.offset > font->file.length ||
        header.length > font->file.length - header.offset) {
        atr_dprintERROR("%s", "table lies outside the font file.");
        return ATR_FAIL;
    }

    if (checkSumAdjustment_offset >= 0) {
        if (checkSumAdjustment_offset >= 0) {
            size_t adjustment = (size_t)checkSumAdjustment_offset;
            if (adjustment > header.length || header.length - adjustment < 4) {
                atr_dprintERROR("%s", "Checksum-adjustment field lies outside the table.");
                return ATR_FAIL;
            }
        }
    }

    const uint8_t *table = font->file.elements + header.offset;

    uint32_t calculated = atr_table_checkSum_calc(table, header.length, checkSumAdjustment_offset, checkSumAdjustment_offset + 4);

    if (calculated != header.checkSum) {
        atr_dprintERROR("Invalid table checksum. Got %X, expected %X.", calculated, header.checkSum);
        return ATR_FAIL;
    }

    return ATR_SUCCESS;
}

ATR_DEF enum Atr_Return_Types atr_table_head_parse(struct Atr_Font *font, struct Atr_Table_Header head_header)
{
    struct Atr_Bit_Reader br = {0};
    atr_bit_reader_init(&br, font->file);
    br.file.cursor = head_header.offset;

    font->tables.head.header = head_header;
    font->tables.head.version_hole_part      = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.head.version_frac_part      = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.head.fontRevision_hole_part = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.head.fontRevision_frac_part = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.head.checkSumAdjustment     = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br, 4));
    font->tables.head.magicNumber            = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br, 4));
    font->tables.head.flags                  = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.head.unitsPerEm             = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    /* skipping datetime fildes */
    atr_bit_reader_read_bytes(&br, 4);
    atr_bit_reader_read_bytes(&br, 4);
    atr_bit_reader_read_bytes(&br, 4);
    atr_bit_reader_read_bytes(&br, 4);
    /* done skipping */
    font->tables.head.xMin                   = (int16_t)atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.head.yMin                   = (int16_t)atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.head.xMax                   = (int16_t)atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.head.yMax                   = (int16_t)atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.head.macStyle               = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.head.lowestRecPPEM          = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.head.fontDirectionHint      = (int16_t)atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.head.indexToLocFormat       = (int16_t)atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.head.glyphDataFormat        = (int16_t)atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));

    /* Checks */
    if (0x5f0f3cf5 != font->tables.head.magicNumber) {
        atr_dprintERROR("Magic number is incorrect. Got %u but expected 0x5F0F3CF5.", font->tables.head.magicNumber);
        return ATR_FAIL;
    }
    if ((font->tables.head.flags & (1u << 5)) != 0) {
        atr_dprintERROR("%s", "Corrupted flags. The sixth bit is not zero.");
        return ATR_FAIL;
    }
    if (font->tables.head.unitsPerEm < 64 || font->tables.head.unitsPerEm > 16384) {
        atr_dprintERROR("Unacceptable unitsPerEm. Got %u and expected a value in the range [64, 16384].", font->tables.head.unitsPerEm);
        return ATR_FAIL;
    }
    if (font->tables.head.xMin > font->tables.head.xMax) {
        atr_dprintERROR("%s", "Unacceptable x range. Got xMin > xMax");
        return ATR_FAIL;
    }
    if (font->tables.head.yMin > font->tables.head.yMax) {
        atr_dprintERROR("%s", "Unacceptable y range. Got yMin > yMax");
        return ATR_FAIL;
    }
    if (font->tables.head.macStyle & 0xFF80u) {
        atr_dprintERROR("Reserved macStyle bits are set: 0x%04X.", font->tables.head.macStyle);
        return ATR_FAIL;
    }
    int d = font->tables.head.fontDirectionHint;
    if (d != -2 && d != -1 && d != 0 && d != 1 && d != 2) {
        atr_dprintERROR("fontDirectionHint value is incorrect. Got %d, expected -2/-1/0/1/2.", d);
        return ATR_FAIL;
    }
    d = font->tables.head.indexToLocFormat;
    if (d != 0 && d != 1) {
        atr_dprintERROR("Invalid indexToLocFormat: %d.", font->tables.head.indexToLocFormat);
        return ATR_FAIL;
    }
    if (font->tables.head.glyphDataFormat != 0) {
        atr_dprintERROR("Incorrect glyphDataFormat. Got %d, expected 0.", font->tables.head.glyphDataFormat);
        return ATR_FAIL;
    }
    if (br.file.cursor != head_header.offset + head_header.length) {
        atr_dprintERROR("%s", "Something went wrong will parsing the head table.");
        return ATR_FAIL;
    }

    /*
    atr_dprintINT(font->tables.head.version_hole_part);
    atr_dprintINT(font->tables.head.version_frac_part);
    atr_dprintINT(font->tables.head.fontRevision_hole_part);
    atr_dprintINT(font->tables.head.fontRevision_frac_part);
    atr_dprintINT(font->tables.head.checkSumAdjustment);
    atr_uint32_print_hex(font->tables.head.magicNumber, 32);
    atr_dprintINT(font->tables.head.unitsPerEm);
    atr_dprintINT(font->tables.head.xMin);
    atr_dprintINT(font->tables.head.yMin);
    atr_dprintINT(font->tables.head.xMax);
    atr_dprintINT(font->tables.head.yMax);
    atr_dprintINT(font->tables.head.macStyle);
    atr_dprintINT(font->tables.head.lowestRecPPEM);
    atr_dprintINT(font->tables.head.fontDirectionHint);
    atr_dprintINT(font->tables.head.indexToLocFormat);
    atr_dprintINT(font->tables.head.glyphDataFormat);
    */

    return ATR_SUCCESS;
}

ATR_DEF enum Atr_Return_Types atr_table_loca_parse(struct Atr_Font *font, struct Atr_Table_Header loca_header)
{
    /* By AI */
    struct Atr_Table_loca *loca = &font->tables.loca;
    size_t num_glyphs = (size_t)font->tables.maxp.numGlyphs;
    int16_t format = font->tables.head.indexToLocFormat;
    size_t entry_size;

    if (format == 0) {
        entry_size = sizeof(uint16_t);
    } else if (format == 1) {
        entry_size = sizeof(uint32_t);
    } else {
        atr_dprintERROR("Unexpected indexToLocFormat. Got %d, expected 0 or 1.", format);
        return ATR_FAIL;
    }

    /*
     * loca contains numGlyphs + 1 offsets, including the final
     * offset marking the end of the last glyph.
     */
    size_t offset_count = num_glyphs + 1;
    size_t expected_length = offset_count * entry_size;
    if ((size_t)loca_header.length != expected_length) {
        atr_dprintERROR("Invalid loca length. Got %u, expected %zu.", loca_header.length, expected_length);
        return ATR_FAIL;
    }

    /*
     * This requires glyf.header to have been set before loca is parsed.
     */
    uint32_t glyf_length = font->tables.glyf.header.length;

    uint32_t *offsets = ATR_MALLOC(sizeof(*offsets) * offset_count);
    if (offsets == NULL) {
        atr_dprintERROR("%s", "Failed to allocate loca offsets.");
        return ATR_FAIL;
    }

    struct Atr_Bit_Reader br = {0};
    atr_bit_reader_init(&br, font->file);
    br.file.cursor = loca_header.offset;

    for (size_t i = 0; i < offset_count; i++) {
        uint32_t offset;
        if (format == 0) {
            uint16_t short_offset = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
            /*
             * Short loca offsets are stored divided by two.
             */
            offset = (uint32_t)short_offset * 2u;
        } else {
            offset = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br, 4));
        }

        if (i > 0 && offset < offsets[i - 1]) {
            atr_dprintERROR("loca offset at index %zu is less than the previous offset.", i);
            ATR_FREE(offsets);
            return ATR_FAIL;
        }

        if (offset > glyf_length) {
            atr_dprintERROR("loca offset %u exceeds glyf length %u.", offset, glyf_length);
            ATR_FREE(offsets);
            return ATR_FAIL;
        }

        offsets[i] = offset;
    }

    /*
     * Replace any previously parsed loca data only after successful
     * parsing.
     */
    ATR_FREE(loca->offsets);

    loca->header = loca_header;
    loca->numGlyphs = font->tables.maxp.numGlyphs;
    loca->indexToLocFormat = format;
    loca->offsets = offsets;
    loca->length = offset_count;

    return ATR_SUCCESS;
}

ATR_DEF enum Atr_Return_Types atr_table_maxp_parse(struct Atr_Font *font, struct Atr_Table_Header maxp_header)
{
    struct Atr_Bit_Reader br = {0};
    atr_bit_reader_init(&br, font->file);
    br.file.cursor = maxp_header.offset;

    font->tables.maxp.header                = maxp_header;
    font->tables.maxp.version_hole_part     = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.maxp.version_frac_part     = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.maxp.numGlyphs             = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.maxp.maxPoints             = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.maxp.maxContours           = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.maxp.maxComponentPoints    = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.maxp.maxComponentContours  = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.maxp.maxZones              = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.maxp.maxTwilightPoints     = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.maxp.maxStorage            = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.maxp.maxFunctionDefs       = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.maxp.maxInstructionDefs    = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.maxp.maxStackElements      = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.maxp.maxSizeOfInstructions = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.maxp.maxComponentElements  = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    font->tables.maxp.maxComponentDepth     = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));

    /* Checks */
    if (font->tables.maxp.maxZones != 2) {
        atr_dprintERROR("Error while parsing the maxp table. Got maxZones of %u, but expected maxZones of 2.", font->tables.maxp.maxZones);
        return ATR_FAIL;
    }

    /*
    atr_dprintINT(font->tables.maxp.version_hole_part    );
    atr_dprintINT(font->tables.maxp.version_frac_part    );
    atr_dprintINT(font->tables.maxp.numGlyphs            );
    atr_dprintINT(font->tables.maxp.maxPoints            );
    atr_dprintINT(font->tables.maxp.maxContours          );
    atr_dprintINT(font->tables.maxp.maxComponentPoints   );
    atr_dprintINT(font->tables.maxp.maxComponentContours );
    atr_dprintINT(font->tables.maxp.maxZones             );
    atr_dprintINT(font->tables.maxp.maxTwilightPoints    );
    atr_dprintINT(font->tables.maxp.maxStorage           );
    atr_dprintINT(font->tables.maxp.maxFunctionDefs      );
    atr_dprintINT(font->tables.maxp.maxInstructionDefs   );
    atr_dprintINT(font->tables.maxp.maxStackElements     );
    atr_dprintINT(font->tables.maxp.maxSizeOfInstructions);
    atr_dprintINT(font->tables.maxp.maxComponentElements );
    atr_dprintINT(font->tables.maxp.maxComponentDepth    );
    */

    return ATR_SUCCESS;
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
