/**
 * Apple's reference manual is at: https://developer.apple.com/fonts/TrueType-Reference-Manual/.
 */

/** TODO:
 *  - fix the rasterazing bugs.
 *  - add support for compound glyphs
 *  - add support for all the mendatory font tables.
 *  - add support for OpenType.
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
#include <float.h>

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
        #define ATR_INFINITY FLT_MAX
        #define ATR_EPS   FLT_EPSILON
        #define atr_fabs  fabsf
        #define atr_floor floorf
        #define atr_ceil  ceilf
        #define atr_round roundf
        #define atr_sqrt  sqrtf
        #define atr_cbrt  cbrtf
        #define atr_cos   cosf
        #define atr_sin   sinf
        #define atr_atan2 atan2f
        #define atr_fmod  fmodf
    #else 
        typedef double atr_real_type;
        #define ATR_INFINITY DBL_MAX
        #define ATR_EPS   DBL_EPSILON
        #define atr_fabs  fabs
        #define atr_floor floor
        #define atr_ceil  ceil
        #define atr_round round
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

            size_t    glyphIndexCount;
            uint16_t *glyphIndexArray;
        } format_4;
        struct {
            uint16_t length;
            uint16_t language;
            uint16_t firstCode;
            uint16_t entryCount;
            uint16_t *glyphIndexArray;
        } format_6;
        struct {
            uint32_t length;
            uint32_t language;
            uint32_t startCharCode;
            uint32_t numChars;
            uint16_t *glyphs;
        } format_10;
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

enum Atr_Outline_Flag {
    ATR_OUTLINE_FLAG_ON_CURVE       = 0b000001,
    ATR_OUTLINE_FLAG_X_SHORT_VECTOR = 0b000010,
    ATR_OUTLINE_FLAG_Y_SHORT_VECTOR = 0b000100,
    ATR_OUTLINE_FLAG_REPEAT         = 0b001000,
    ATR_OUTLINE_FLAG_THIS_X_IS_SAME = 0b010000,
    ATR_OUTLINE_FLAG_THIS_Y_IS_SAME = 0b100000,
};

struct Atr_Vec2 {
    atr_real x;
    atr_real y;
};

enum Atr_Glyph_Point_Flag {
    ATR_GPF_ON_CURVE    = 0b001,
    ATR_GPF_CONTOUR_END = 0b010,
    ATR_GPF_GENERATED   = 0b100,
};

struct Atr_Glyph_Point {
    struct Atr_Vec2 pos;
    enum Atr_Glyph_Point_Flag flag;
};

struct Atr_Glyph_Point_Dynamic_Array {
    size_t capacity;
    size_t length;
    struct Atr_Glyph_Point *elements;
};

struct Atr_Glyph {
    struct {
        int16_t numberOfContours;
        int16_t xMin;
        int16_t yMin;
        int16_t xMax;
        int16_t yMax;
    } metadata;
    union {
        struct {
            uint16_t *endPtsOfContours;

            uint16_t  instructionLength;
            uint8_t  *instructions;

            size_t    num_of_raw_points;
            uint8_t  *flags;
            int16_t  *xCoordinates;
            int16_t  *yCoordinates;
            struct Atr_Glyph_Point_Dynamic_Array points;
            struct Atr_Glyph_Point_Dynamic_Array points_temp_for_resizing;
        } simple;
    };
};

struct Atr_Real_Dynamic_Array {
    size_t capacity;
    size_t length;
    atr_real *elements;
};

struct Atr_Table_glyf {
    struct Atr_Table_Header header;

    size_t num_of_glyphs;
    struct Atr_Glyph *glyphs;
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
        struct Atr_Table_cmap cmap;
        struct Atr_Table_head head;
        struct Atr_Table_maxp maxp;
        struct Atr_Table_loca loca;
        struct Atr_Table_glyf glyf;
        struct Atr_Table_hhea hhea;
        struct Atr_Table_hmtx hmtx;
        struct Atr_Table_name name;
        struct Atr_Table_post post;
    } tables;
};

static uint8_t atr_bytes_for_utf8[] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3, 4,4,4,4,4,4,4,4,
};
#define atr_bytes_for_utf8_count (sizeof(atr_bytes_for_utf8) / sizeof(atr_bytes_for_utf8[0]))
#define ATR_UTF8_REPLACEMENT_CHARACTER 0xFFFDu

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

ATR_DEF uint32_t                    atr_4chars_to_uint32(const char *chars);
ATR_DEF uint32_t                    atr_4chars_to_uint32_be(const char *chars);

ATR_DEF uint32_t                    atr_alpha_blend(uint32_t dst, uint32_t src);

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

ATR_DEF void                        atr_circle_fill(struct Atr_Pixel_Buffer screen, atr_real center_x, atr_real center_y, atr_real r, uint32_t color, struct Atr_Offset_Zoom offzoom);
ATR_DEF void                        atr_circle_fill_high_quality(struct Atr_Pixel_Buffer screen, atr_real center_x, atr_real center_y, atr_real r, uint32_t color, struct Atr_Offset_Zoom offzoom);

ATR_DEF uint16_t                    atr_endian_swap_uint16(uint16_t x);
ATR_DEF uint32_t                    atr_endian_swap_uint32(uint32_t x);

ATR_DEF void                        atr_font_free(struct Atr_Font *font);
ATR_DEF enum Atr_Return_Types       atr_font_load_from_file_name(struct Atr_Font *font, char *file_name);

ATR_DEF void                        atr_glyph_append_line(struct Atr_Glyph *glyph, struct Atr_Glyph_Point start, struct Atr_Glyph_Point end);
ATR_DEF void                        atr_glyph_append_quadratic_bezier(struct Atr_Glyph *glyph, struct Atr_Glyph_Point start, struct Atr_Glyph_Point control, struct Atr_Glyph_Point end);
ATR_DEF void                        atr_glyph_free(struct Atr_Glyph *g);
ATR_DEF enum Atr_Return_Types       atr_glyph_parse(struct Atr_Glyph *glyph, struct Atr_Bit_Reader br);
ATR_DEF enum Atr_Return_Types       atr_glyph_parse_simple(struct Atr_Glyph *glyph, struct Atr_Bit_Reader br);
ATR_DEF struct Atr_Glyph_Point      atr_glyph_point_from_raw(const struct Atr_Glyph *glyph, size_t index);
ATR_DEF bool                        atr_glyph_point_is_on_curve(const struct Atr_Glyph_Point *point);
ATR_DEF struct Atr_Glyph_Point      atr_glyph_point_midpoint(struct Atr_Glyph_Point a, struct Atr_Glyph_Point b);
ATR_DEF uint32_t                    atr_glyphIndex_get(struct Atr_Font *font, uint32_t code_point);
ATR_DEF uint32_t                    atr_glyphIndex_get_cmap4(struct Atr_Table_cmap_Subtable *st, uint32_t code_point);

ATR_DEF void                        atr_hexargb_to_rgba(uint32_t color, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);

ATR_DEF void                        atr_line_draw(struct Atr_Pixel_Buffer screen, atr_real x1_input, atr_real y1_input, atr_real x2_input, atr_real y2_input, uint32_t color, struct Atr_Offset_Zoom offzoom);
ATR_DEF void                        atr_line_draw_fix_width(struct Atr_Pixel_Buffer screen, atr_real x1_input, atr_real y1_input, atr_real x2_input, atr_real y2_input, uint32_t color, struct Atr_Offset_Zoom offzoom);
ATR_DEF void                        atr_line_draw_no_antialiasing(struct Atr_Pixel_Buffer screen, atr_real x1_input, atr_real y1_input, atr_real x2_input, atr_real y2_input, uint32_t color, struct Atr_Offset_Zoom offzoom);
ATR_DEF void                        atr_line_horiz_draw(struct Atr_Pixel_Buffer screen, atr_real x1_input, atr_real x2_input, atr_real y_input, uint32_t color, struct Atr_Offset_Zoom offzoom);

ATR_DEF enum Atr_Return_Types       atr_offset_subtable_parse(struct Atr_Font *font);

ATR_DEF void                        atr_pixel_draw(struct Atr_Pixel_Buffer screen, atr_real x, atr_real y, uint32_t color, struct Atr_Offset_Zoom offzoom);

ATR_DEF enum Atr_Return_Types       atr_quadratic_bezier_array_fill(struct Atr_Pixel_Buffer screen, struct Atr_Glyph_Point *points, size_t points_count, uint32_t color, struct Atr_Offset_Zoom offzoom);
ATR_DEF enum Atr_Return_Types       atr_quadratic_bezier_array_fill_no_antialiasing(struct Atr_Pixel_Buffer screen, struct Atr_Glyph_Point *points, size_t points_count, uint32_t color, struct Atr_Offset_Zoom offzoom);
ATR_DEF void                        atr_quadratic_bezier_draw(struct Atr_Pixel_Buffer pixels, struct Atr_Glyph_Point start, struct Atr_Glyph_Point control, struct Atr_Glyph_Point end, uint32_t color, struct Atr_Offset_Zoom offzoom);
ATR_DEF size_t                      atr_quadratic_bezier_get_xs_from_y(struct Atr_Glyph_Point start, struct Atr_Glyph_Point control, struct Atr_Glyph_Point end, atr_real y, atr_real *x1, atr_real *x2, atr_real *dy_dt1, atr_real *dy_dt2);
ATR_DEF bool                        atr_quadratic_bezier_root_is_crossing(bool at_start, bool at_end, atr_real dy_dt);

ATR_DEF void                        atr_rectangle_draw_min_max(struct Atr_Pixel_Buffer screen, atr_real min_x, atr_real max_x, atr_real min_y, atr_real max_y, uint32_t color, struct Atr_Offset_Zoom offzoom);
ATR_DEF uint32_t                    atr_rgba_to_hexargb(int r, int g, int b, int a);

ATR_DEF atr_real                    atr_scale_get_for_em(struct Atr_Font *font, atr_real pixels_per_em);

ATR_DEF uint32_t                    atr_table_checkSum_calc(const uint8_t *bytes, size_t length, int zero_begin, int zero_end);
ATR_DEF void                        atr_table_cmap_free(struct Atr_Font *font);
ATR_DEF enum Atr_Return_Types       atr_table_cmap_parse(struct Atr_Font *font, struct Atr_Table_Header cmap_header);
ATR_DEF enum Atr_Return_Types       atr_table_cmap_subtable_choose(struct Atr_Font *font);
ATR_DEF enum Atr_cmap_Priority      atr_table_cmap_subtable_priority_score(struct Atr_Table_cmap_Subtable *subtable);
ATR_DEF enum Atr_Return_Types       atr_table_directory_parse(struct Atr_Font *font);
ATR_DEF void                        atr_table_glyf_free(struct Atr_Font *font);
ATR_DEF enum Atr_Return_Types       atr_table_glyf_parse(struct Atr_Font *font, struct Atr_Table_Header glyf_header);
ATR_DEF struct Atr_Table_Header *   atr_table_header_find_by_tag_raw(struct Atr_Font *font, uint32_t tag_raw);
ATR_DEF struct Atr_Table_Header     atr_table_header_parse(struct Atr_Font *font, size_t offset);
ATR_DEF enum Atr_Return_Types       atr_table_header_verify_checksum(struct Atr_Font *font, struct Atr_Table_Header header, int checkSumAdjustment_offset);
ATR_DEF enum Atr_Return_Types       atr_table_head_parse(struct Atr_Font *font, struct Atr_Table_Header head_header);
ATR_DEF enum Atr_Return_Types       atr_table_loca_parse(struct Atr_Font *font, struct Atr_Table_Header loca_header);
ATR_DEF enum Atr_Return_Types       atr_table_maxp_parse(struct Atr_Font *font, struct Atr_Table_Header maxp_header);
ATR_DEF struct Atr_Vec2             atr_text_line_draw(struct Atr_Pixel_Buffer screen, struct Atr_Font *font, uint8_t *text, atr_real top_left_x, atr_real top_left_y, atr_real letter_hight, atr_real letter_spacing, uint32_t color, int length, struct Atr_Offset_Zoom offzoom);
ATR_DEF struct Atr_Vec2             atr_text_line_draw_no_antialiasing(struct Atr_Pixel_Buffer screen, struct Atr_Font *font, uint8_t *text, atr_real top_left_x, atr_real top_left_y, atr_real letter_hight, atr_real letter_spacing, uint32_t color, int length, struct Atr_Offset_Zoom offzoom);
ATR_DEF struct Atr_Vec2             atr_text_line_draw_outline(struct Atr_Pixel_Buffer screen, struct Atr_Font *font, uint8_t *text, atr_real top_left_x, atr_real top_left_y, atr_real letter_hight, atr_real letter_spacing, uint32_t color, int length, struct Atr_Offset_Zoom offzoom);

ATR_DEF uint8_t                     atr_u8_clamp_int(int x);
                                    #define atr_uint16_print_binary(value, bit_count) atr_dprintINFO("%s = ", #value); printf("%*.s", 7, ""); atr_uint16_print_binary_imp((value), (bit_count))
ATR_DEF void                        atr_uint16_print_binary_imp(uint16_t value, uint8_t bit_count);
                                    #define atr_uint16_print_hex(value, bit_count) atr_dprintINFO("%s = ", #value); printf("%*.s", 7, ""); atr_uint16_print_hex_imp((value), (bit_count))
ATR_DEF void                        atr_uint16_print_hex_imp(uint16_t value, uint8_t bit_count);
                                    #define atr_uint32_print_binary(value, bit_count) atr_dprintINFO("%s = ", #value); printf("%*.s", 7, ""); atr_uint32_print_binary_imp((value), (bit_count))
ATR_DEF void                        atr_uint32_print_binary_imp(uint32_t value, uint8_t bit_count);
                                    #define atr_uint32_print_hex(value, bit_count) atr_dprintINFO("%s = ", #value); printf("%*.s", 7, ""); atr_uint32_print_hex_imp((value), (bit_count))
ATR_DEF void                        atr_uint32_print_hex_imp(uint32_t value, uint8_t bit_count);
ATR_DEF uint32_t                    atr_utf8_code_point_get_from_raw_char_bytes(uint32_t raw_char_bytes);
ATR_DEF uint32_t                    atr_utf8_decode_next_code_point(uint8_t *text, size_t byte_count, size_t *consumed);
ATR_DEF uint32_t                    atr_utf8_get_next_char_bytes(uint8_t *str, size_t byte_count);
ATR_DEF bool                        atr_utf8_is_continuation_byte(uint8_t byte);
ATR_DEF size_t                      atr_utf8_length(uint8_t *str, size_t byte_count);


#endif /*ALMOG_TEXT_RENDERING_H_*/

#ifdef ALMOG_TEXT_RENDERING_IMPLEMENTATION
#undef ALMOG_TEXT_RENDERING_IMPLEMENTATION


ATR_DEF uint32_t atr_4chars_to_uint32(const char *chars)
{
    return ((uint32_t)(((uint32_t)((chars)[0]) << 0) | ((uint32_t)((chars)[1]) << 8) | ((uint32_t)((chars)[2]) << 16) | ((uint32_t)((chars)[3]) << 24)));
}

ATR_DEF uint32_t atr_4chars_to_uint32_be(const char *chars)
{
    return ((uint32_t)(uint8_t)chars[0] << 24) |
            ((uint32_t)(uint8_t)chars[1] << 16) |
            ((uint32_t)(uint8_t)chars[2] << 8) |
            ((uint32_t)(uint8_t)chars[3]);
}
 
ATR_DEF uint32_t atr_alpha_blend(uint32_t dst, uint32_t src)
{
    uint8_t sr, sg, sb, sa;
    uint8_t dr, dg, db;

    atr_hexargb_to_rgba(src, &sr, &sg, &sb, &sa);
    atr_hexargb_to_rgba(dst, &dr, &dg, &db, NULL);

    atr_real a = (atr_real)sa / 255.0f;

    int r = (int)((atr_real)dr * (1.0f - a) + (atr_real)sr * a);
    int g = (int)((atr_real)dg * (1.0f - a) + (atr_real)sg * a);
    int b = (int)((atr_real)db * (1.0f - a) + (atr_real)sb * a);

    return atr_rgba_to_hexargb(r, g, b, 255);
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

ATR_DEF void atr_circle_fill(struct Atr_Pixel_Buffer screen, atr_real center_x, atr_real center_y, atr_real r, uint32_t color, struct Atr_Offset_Zoom offzoom)
{
    // if (center_x + r < 0 || center_x - r > screen.cols || center_y + r < 0 || center_y - r > screen.rows) {
    //     return;
    // } 
    atr_real x = 0, y = -r, p = -r;
    while (x < -y) {
        if (p > 0) {
            y += 1;
            p += 2 * (x + y) + 1;
            atr_line_draw_no_antialiasing(screen, center_x + x, center_y + y, center_x - x, center_y + y, color, offzoom);
            atr_line_draw_no_antialiasing(screen, center_x + x, center_y - y, center_x - x, center_y - y, color, offzoom);
        } else {
            p += 2 * x + 1;
        }
        atr_line_draw_no_antialiasing(screen, center_x + y, center_y + x, center_x - y, center_y + x, color, offzoom);
        atr_line_draw_no_antialiasing(screen, center_x + y, center_y - x, center_x - y, center_y - x, color, offzoom);
        x += 1;
    }
}

ATR_DEF void atr_circle_fill_high_quality(struct Atr_Pixel_Buffer screen, atr_real center_x, atr_real center_y, atr_real r, uint32_t color, struct Atr_Offset_Zoom offzoom)
{
    atr_real window_w = (atr_real)screen.cols;
    atr_real window_h = (atr_real)screen.rows;
    atr_real zoom = offzoom.zoom_multiplier;

    atr_real center_x1 = (center_x - window_w/2.0f + offzoom.offset_x) * zoom + window_w/2.0f;
    atr_real center_y1 = (center_y - window_h/2.0f + offzoom.offset_y) * zoom + window_h/2.0f;
    atr_real r1        = r * zoom;

    atr_circle_fill(screen, center_x1, center_y1, r1, color, ATR_DEFAULT_OFFSET_ZOOM);
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

    atr_table_glyf_free(font);

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
    const struct Atr_Table_Header *cmap_header = atr_table_header_find_by_tag_raw(&loaded, atr_4chars_to_uint32("cmap"));
    const struct Atr_Table_Header *head_header = atr_table_header_find_by_tag_raw(&loaded, atr_4chars_to_uint32("head"));
    const struct Atr_Table_Header *maxp_header = atr_table_header_find_by_tag_raw(&loaded, atr_4chars_to_uint32("maxp"));
    const struct Atr_Table_Header *loca_header = atr_table_header_find_by_tag_raw(&loaded, atr_4chars_to_uint32("loca"));
    const struct Atr_Table_Header *glyf_header = atr_table_header_find_by_tag_raw(&loaded, atr_4chars_to_uint32("glyf"));
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
     * parse all the glyphs according to the loca table. */
    if (atr_table_glyf_parse(&loaded, *glyf_header) == ATR_FAIL) {
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

ATR_DEF void atr_glyph_append_line(struct Atr_Glyph *glyph, struct Atr_Glyph_Point start, struct Atr_Glyph_Point end)
{
    struct Atr_Glyph_Point control = atr_glyph_point_midpoint(start, end);

    /*
     * This is a line represented as a degenerate quadratic Bézier:
     *
     * start -> midpoint(start, end) -> end
     */
    control.flag = ATR_GPF_GENERATED;

    atr_glyph_append_quadratic_bezier(glyph, start, control, end);
}

ATR_DEF void atr_glyph_append_quadratic_bezier(struct Atr_Glyph *glyph, struct Atr_Glyph_Point start, struct Atr_Glyph_Point control, struct Atr_Glyph_Point end)
{
    atr_ada_append(struct Atr_Glyph_Point, glyph->simple.points, start);
    atr_ada_append(struct Atr_Glyph_Point, glyph->simple.points, control);
    atr_ada_append(struct Atr_Glyph_Point, glyph->simple.points, end);

    atr_ada_append(struct Atr_Glyph_Point, glyph->simple.points_temp_for_resizing, start);
    atr_ada_append(struct Atr_Glyph_Point, glyph->simple.points_temp_for_resizing, control);
    atr_ada_append(struct Atr_Glyph_Point, glyph->simple.points_temp_for_resizing, end);
}

ATR_DEF void atr_glyph_free(struct Atr_Glyph *g)
{
    ATR_ASSERT(g);
    if (g->metadata.numberOfContours >= 0) {
        ATR_FREE(g->simple.endPtsOfContours);
        g->simple.endPtsOfContours = NULL;
        ATR_FREE(g->simple.instructions);
        g->simple.instructions = NULL;
        ATR_FREE(g->simple.flags);
        g->simple.flags = NULL;
        ATR_FREE(g->simple.xCoordinates);
        g->simple.xCoordinates = NULL;
        ATR_FREE(g->simple.yCoordinates);
        g->simple.yCoordinates = NULL;
        ATR_FREE(g->simple.points.elements);
        g->simple.points.elements = NULL;
        g->simple.points.length = 0;
        g->simple.points.capacity = 0;
        ATR_FREE(g->simple.points_temp_for_resizing.elements);
        g->simple.points_temp_for_resizing.elements = NULL;
        g->simple.points_temp_for_resizing.length = 0;
        g->simple.points_temp_for_resizing.capacity = 0;
    }
}

ATR_DEF enum Atr_Return_Types atr_glyph_parse(struct Atr_Glyph *glyph, struct Atr_Bit_Reader br)
{
    ATR_ASSERT(glyph);

    glyph->metadata.numberOfContours          = (int16_t)atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    glyph->metadata.xMin                      = (int16_t)atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    glyph->metadata.yMin                      = (int16_t)atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    glyph->metadata.xMax                      = (int16_t)atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    glyph->metadata.yMax                      = (int16_t)atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));

    if (glyph->metadata.numberOfContours > 0) {
        if (ATR_FAIL == atr_glyph_parse_simple(glyph, br)) {
            atr_dprintERROR("%s", "Failed to parse a simple glyph.");
            return ATR_FAIL;
        }
    }

    return ATR_SUCCESS;
}

ATR_DEF enum Atr_Return_Types atr_glyph_parse_simple(struct Atr_Glyph *glyph, struct Atr_Bit_Reader br)
{
    glyph->simple.endPtsOfContours = ATR_MALLOC(sizeof(uint16_t) * glyph->metadata.numberOfContours);
    if (glyph->simple.endPtsOfContours == NULL) {
        atr_dprintERROR("%s", "Failed to allocate endPtsOfContours array.");
        return ATR_FAIL;
    }
    for (size_t i = 0; i < glyph->metadata.numberOfContours; i++) {
        glyph->simple.endPtsOfContours[i] = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    }
    glyph->simple.num_of_raw_points = glyph->simple.endPtsOfContours[glyph->metadata.numberOfContours - 1] + 1;

    glyph->simple.instructionLength       = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
    glyph->simple.instructions = ATR_MALLOC(sizeof(uint8_t) * glyph->simple.instructionLength);
    if (glyph->simple.instructions == NULL && glyph->simple.instructionLength > 0) {
        atr_dprintERROR("%s", "Failed to allocate instructions array.");
        return ATR_FAIL;
    }
    for (size_t i = 0; i < glyph->simple.instructionLength; i++) {
        glyph->simple.instructions[i]     = (uint8_t)atr_bit_reader_read_bytes(&br, 1);
    }

    glyph->simple.flags = ATR_MALLOC(sizeof(uint8_t) * glyph->simple.num_of_raw_points);
    if (glyph->simple.flags == NULL && glyph->simple.num_of_raw_points > 0) {
        atr_dprintERROR("%s", "Failed to allocate flags array.");
        return ATR_FAIL;
    }

    for (size_t i = 0; i < glyph->simple.num_of_raw_points;) {
        uint8_t flag = (uint8_t)atr_bit_reader_read_bytes(&br, 1);
        glyph->simple.flags[i++] = flag;

        if ((flag & ATR_OUTLINE_FLAG_REPEAT) != 0) {
            uint8_t repeat_count = (uint8_t)atr_bit_reader_read_bytes(&br, 1);
            if ((size_t)repeat_count > glyph->simple.num_of_raw_points - i) {
                atr_dprintERROR("Invalid flag repeat count: %u.", repeat_count);
                return ATR_FAIL;
            }
            for (size_t j = 0; j < repeat_count; ++j) {
                glyph->simple.flags[i++] = flag;
            }
        }
    }
    glyph->simple.xCoordinates = ATR_MALLOC(sizeof(*glyph->simple.xCoordinates) * glyph->simple.num_of_raw_points);
    if (glyph->simple.xCoordinates == NULL && glyph->simple.num_of_raw_points > 0) {
        atr_dprintERROR("%s", "Failed to allocate xCoordinates array.");
        return ATR_FAIL;
    }
    int16_t x = 0;

    for (size_t i = 0; i < glyph->simple.num_of_raw_points; ++i) {
        uint8_t flag = glyph->simple.flags[i];

        if ((flag & ATR_OUTLINE_FLAG_X_SHORT_VECTOR) != 0) {
            uint8_t diff = (uint8_t)atr_bit_reader_read_bytes(&br, 1);
            if ((flag & ATR_OUTLINE_FLAG_THIS_X_IS_SAME) != 0) {
                x += (int16_t)diff;
            } else {
                x -= (int16_t)diff;
            }
        } else if ((flag & ATR_OUTLINE_FLAG_THIS_X_IS_SAME) == 0) {
            int16_t diff = (int16_t)atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
            x += diff;
        }

        glyph->simple.xCoordinates[i] = x;
    }
    glyph->simple.yCoordinates = ATR_MALLOC(sizeof(*glyph->simple.yCoordinates) * glyph->simple.num_of_raw_points);
    if (glyph->simple.yCoordinates == NULL && glyph->simple.num_of_raw_points > 0) {
        atr_dprintERROR("%s", "Failed to allocate yCoordinates array.");
        return ATR_FAIL;
    }
    int16_t y = 0;

    for (size_t i = 0; i < glyph->simple.num_of_raw_points; ++i) {
        uint8_t flag = glyph->simple.flags[i];

        if ((flag & ATR_OUTLINE_FLAG_Y_SHORT_VECTOR) != 0) {
            uint8_t diff = (uint8_t)atr_bit_reader_read_bytes(&br, 1);
            if ((flag & ATR_OUTLINE_FLAG_THIS_Y_IS_SAME) != 0) {
                y += (int16_t)diff;
            } else {
                y -= (int16_t)diff;
            }
        } else if ((flag & ATR_OUTLINE_FLAG_THIS_Y_IS_SAME) == 0) {
            int16_t diff = (int16_t)atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br, 2));
            y += diff;
        }

        glyph->simple.yCoordinates[i] = y;
    }

    atr_ada_init_array(struct Atr_Glyph_Point, glyph->simple.points);
    atr_ada_init_array(struct Atr_Glyph_Point, glyph->simple.points_temp_for_resizing);
    for (size_t contour_index = 0; contour_index < (size_t)glyph->metadata.numberOfContours; ++contour_index) {
        size_t start_point_index = contour_index == 0 ? 0 : (size_t)glyph->simple.endPtsOfContours[contour_index - 1] + 1;
        size_t end_point_index = (size_t)glyph->simple.endPtsOfContours[contour_index];
        size_t point_count = end_point_index - start_point_index + 1;

        if (point_count == 0) {
            atr_dprintERROR("%s", "Glyph contour has no points.");
            return ATR_FAIL;
        }

        struct Atr_Glyph_Point first = atr_glyph_point_from_raw(glyph, start_point_index);
        struct Atr_Glyph_Point last = atr_glyph_point_from_raw(glyph, end_point_index);

        struct Atr_Glyph_Point current;
        size_t start_walk_index;

        /*
        * A TrueType contour must begin at an on-curve point. If its first
        * raw point is off-curve:
        *
        * - Use the last point if that point is on-curve.
        * - Otherwise, create an implied on-curve point halfway between the
        *   first and last off-curve points.
        */
        if (atr_glyph_point_is_on_curve(&first)) {
            current = first;
            start_walk_index = 1;
        } else if (atr_glyph_point_is_on_curve(&last)) {
            current = last;
            start_walk_index = 0;
        } else {
            current = atr_glyph_point_midpoint(last, first);
            start_walk_index = 0;
        }

        bool has_control = false;
        struct Atr_Glyph_Point control = {0};
        size_t contour_output_start = glyph->simple.points.length;

        /*
        * Walk all raw points circularly, starting immediately after the
        * selected starting point.
        */
        for (size_t step = 0; step < point_count; ++step) {
            size_t local_index = (start_walk_index + step) % point_count;
            size_t raw_index = start_point_index + local_index;

            struct Atr_Glyph_Point point = atr_glyph_point_from_raw(glyph, raw_index);

            if (atr_glyph_point_is_on_curve(&point)) {
                if (has_control) {
                    /*
                    * current -- control -- point
                    */
                    atr_glyph_append_quadratic_bezier(glyph, current, control, point);
                    has_control = false;
                } else {
                    /*
                    * Consecutive on-curve points define a line.
                    */
                    atr_glyph_append_line(glyph, current, point);
                }

                current = point;
                continue;
            }

            /*
            * An off-curve point is a quadratic control point.
            */
            if (!has_control) {
                control = point;
                has_control = true;
                continue;
            }

            /*
            * Two consecutive off-curve points imply an on-curve endpoint
            * halfway between them.
            */
            struct Atr_Glyph_Point implied_end = atr_glyph_point_midpoint(control, point);

            atr_glyph_append_quadratic_bezier(glyph, current, control, implied_end);

            current = implied_end;
            control = point;
            has_control = true;
        }

        /*
        * This occurs when the contour started with an implied midpoint and
        * ended on an off-curve point. Close the final quadratic segment.
        */
        if (has_control) {
            atr_glyph_append_quadratic_bezier(glyph, current, control,
                atr_glyph_point_is_on_curve(&first) ? first : atr_glyph_point_midpoint(last, first)
            );
        }

        /*
        * Mark the final endpoint of this contour. Each emitted segment
        * occupies three points, so the last item is always its endpoint.
        */
        if (glyph->simple.points.length > contour_output_start) {
            glyph->simple.points.elements[glyph->simple.points.length - 1].flag |= ATR_GPF_CONTOUR_END;
            glyph->simple.points_temp_for_resizing.elements[glyph->simple.points_temp_for_resizing.length - 1].flag |= ATR_GPF_CONTOUR_END;
        }

    }

    return ATR_SUCCESS;
}

ATR_DEF struct Atr_Glyph_Point atr_glyph_point_from_raw(const struct Atr_Glyph *glyph, size_t index)
{
    return (struct Atr_Glyph_Point){
        .flag = glyph->simple.flags[index],
        .pos = {
            .x = glyph->simple.xCoordinates[index],
            .y = glyph->simple.yCoordinates[index],
        },
    };
}

ATR_DEF bool atr_glyph_point_is_on_curve(const struct Atr_Glyph_Point *point)
{
    return (point->flag & ATR_GPF_ON_CURVE) != 0;
}

ATR_DEF struct Atr_Glyph_Point atr_glyph_point_midpoint(struct Atr_Glyph_Point a, struct Atr_Glyph_Point b)
{
    return (struct Atr_Glyph_Point){
        .flag = ATR_GPF_ON_CURVE | ATR_GPF_GENERATED,
        .pos = {
            .x = (int16_t)(((int32_t)a.pos.x + (int32_t)b.pos.x) / 2),
            .y = (int16_t)(((int32_t)a.pos.y + (int32_t)b.pos.y) / 2),
        },
    };
}

ATR_DEF uint32_t atr_glyphIndex_get(struct Atr_Font *font, uint32_t code_point)
{
    struct Atr_Table_cmap_Subtable st = font->tables.cmap.subtables.elements[font->tables.cmap.chosen_subtable_index];
    ATR_ASSERT(st.format != 14);

    if (st.format == 0) {
        if (code_point >= 256) {
            return 0;
        }
        return st.data.format_0.glyphIndexArray[code_point];
    } else if (st.format == 4) {
        return atr_glyphIndex_get_cmap4(&st, code_point);
    } else if (st.format == 6) {
        size_t first_code  = st.data.format_6.firstCode;
        size_t entry_count = st.data.format_6.entryCount;
            if (first_code <= code_point) {
                if (code_point < first_code + entry_count) {
                    return st.data.format_6.glyphIndexArray[code_point - first_code];
                }
            }
        return 0;
    } else if (st.format == 10) {
        size_t first_code  = st.data.format_10.startCharCode;
        size_t entry_count = st.data.format_10.numChars;
            if (first_code <= code_point) {
                if (code_point < first_code + entry_count) {
                    return st.data.format_10.glyphs[code_point - first_code];
                }
            }
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
        for (size_t i = 0; i < st.data.format_13.nGroups; i++) {
            struct Atr_Table_cmap_Group cg = st.data.format_13.groups[i];
            if (cg.startCharCode <= code_point && code_point <= cg.endCharCode) {
                return cg.startGlyphCode;
            }
        }
        return 0;
    } else {
        return 0;
    }
}

ATR_DEF uint32_t atr_glyphIndex_get_cmap4(struct Atr_Table_cmap_Subtable *st, uint32_t code_point)
{
    /* By AI */
    if (code_point > UINT16_MAX) {
        return 0;
    }

    const uint16_t character_code = (uint16_t)code_point;
    const size_t seg_count = st->data.format_4.segCountx2 / 2;

    if (seg_count == 0) {
        return 0;
    }

    size_t low = 0;
    size_t high = seg_count;
    while (low < high) {
        const size_t middle = low + (high - low) / 2;
        if (st->data.format_4.endCode[middle] < character_code) {
            low = middle + 1;
        } else {
            high = middle;
        }
    }
    if (low == seg_count) {
        return 0;
    }

    const size_t i = low;
    if (character_code < st->data.format_4.startCode[i]) {
        return 0;
    }

    const uint16_t range_offset = st->data.format_4.idRangeOffset[i];
    const int16_t delta = st->data.format_4.idDelta[i];
    /*
     * idRangeOffset == 0:
     *
     * glyphIndex = (character_code + idDelta) mod 65536
     */
    if (range_offset == 0) {
        return (uint16_t)(character_code + delta);
    }

    /*
     * idRangeOffset is measured in bytes relative to the address of
     * idRangeOffset[i].
     *
     * In the serialized cmap:
     *
     *   idRangeOffset[0 ... seg_count - 1]
     *   glyphIndexArray[0 ...]
     *
     * Therefore, convert the pointer-relative location to an index relative
     * to glyphIndexArray.
     */
    if ((range_offset & 1) != 0) {
        /* A UInt16 offset must be aligned to two bytes. */
        return 0;
    }

    const size_t character_offset = (size_t)character_code - st->data.format_4.startCode[i];

    const size_t range_offset_words = range_offset / 2;
    const size_t words_until_glyph_array = seg_count - i;

    /*
     * Prevent unsigned underflow and reject malformed tables where the
     * offset points before glyphIndexArray.
     */
    if (range_offset_words + character_offset < words_until_glyph_array) {
        return 0;
    }

    const size_t glyph_index_array_index = range_offset_words + character_offset - words_until_glyph_array;
    if (glyph_index_array_index >= st->data.format_4.glyphIndexCount) {
        return 0;
    }

    const uint16_t glyph_index = st->data.format_4.glyphIndexArray[glyph_index_array_index];

    /*
     * A zero glyph ID remains zero. Do not apply idDelta to it.
     */
    if (glyph_index == 0) {
        return 0;
    }

    return (uint16_t)(glyph_index + delta);
}

ATR_DEF void atr_hexargb_to_rgba(uint32_t color, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a)
{
    if (a) *a = (uint8_t)((color >> 24) & 0xFF);
    if (r) *r = (uint8_t)((color >> 16) & 0xFF);
    if (g) *g = (uint8_t)((color >> 8) & 0xFF);
    if (b) *b = (uint8_t)((color >> 0) & 0xFF);
}

ATR_DEF void atr_line_draw(struct Atr_Pixel_Buffer screen, atr_real x1_input, atr_real y1_input, atr_real x2_input, atr_real y2_input, uint32_t color, struct Atr_Offset_Zoom offzoom)
{
    uint8_t r, g, b, a;
    atr_hexargb_to_rgba(color, &r, &g, &b, &a);

    if (atr_fabs(y2_input - y1_input) < atr_fabs(x2_input - x1_input)) {
        if (x2_input < x1_input) {
            atr_real temp = x2_input;
            x2_input = x1_input;
            x1_input = temp;

            temp = y2_input;
            y2_input = y1_input;
            y1_input = temp;
        }

        atr_real dx = x2_input - x1_input;
        atr_real dy = y2_input - y1_input;
        atr_real m = dy / dx;

        atr_real overlap = 1 - ((x1_input + (atr_real)0.5) - (int)(x1_input + (atr_real)0.5));
        atr_real dis_start = y1_input - (int)y1_input;
        atr_pixel_draw(screen, (atr_real)((int)(x1_input + (atr_real)0.5)), (atr_real)((int)(y1_input)), atr_rgba_to_hexargb(r, g, b, (int)(a * ((atr_real)1 - dis_start) * overlap)), offzoom);
        atr_pixel_draw(screen, (atr_real)((int)(x1_input + (atr_real)0.5)), (atr_real)((int)(y1_input) + (atr_real)1), atr_rgba_to_hexargb(r, g, b, (int)(a * (dis_start) * overlap)), offzoom);
        overlap = ((x2_input + (atr_real)0.5) - (int)(x2_input + (atr_real)0.5));
        atr_real dis_end = y2_input - (int)y2_input;
        atr_pixel_draw(screen, (atr_real)((int)(x2_input + (atr_real)0.5)), (atr_real)((int)(y2_input)), atr_rgba_to_hexargb(r, g, b, (int)(a * ((atr_real)1 - dis_end) * overlap)), offzoom);
        atr_pixel_draw(screen, (atr_real)((int)(x2_input + (atr_real)0.5)), (atr_real)((int)(y2_input) + (atr_real)1), atr_rgba_to_hexargb(r, g, b, (int)(a * (dis_end) * overlap)), offzoom);

        for (size_t i = 1; i < dx; i++) {
            atr_real x = x1_input + (atr_real)i;
            atr_real y = y1_input + (atr_real)i * m;
            int ix = (int)x;
            int iy = (int)y;
            atr_real down_dis = y - iy;
            atr_real up_dis   = 1 - down_dis;
            atr_pixel_draw(screen, (atr_real)ix, (atr_real)iy, atr_rgba_to_hexargb(r, g, b, (int)(a * up_dis)), offzoom);
            atr_pixel_draw(screen, (atr_real)ix, (atr_real)iy + (atr_real)1, atr_rgba_to_hexargb(r, g, b, (int)(a * down_dis)), offzoom);
        }
    } else {
        if (y2_input < y1_input) {
            atr_real temp = x2_input;
            x2_input = x1_input;
            x1_input = temp;

            temp = y2_input;
            y2_input = y1_input;
            y1_input = temp;
        }

        atr_real dx = x2_input - x1_input;
        atr_real dy = y2_input - y1_input;
        atr_real m = dx / dy;

        atr_real overlap = 1 - ((y1_input + (atr_real)0.5) - (int)(y1_input + (atr_real)0.5));
        atr_real dis_start = y1_input - (int)y1_input;
        atr_pixel_draw(screen, (atr_real)((int)(x1_input)), (atr_real)((int)(y1_input + (atr_real)0.5)), atr_rgba_to_hexargb(r, g, b, (int)(a * ((atr_real)1 - dis_start) * overlap)), offzoom);
        atr_pixel_draw(screen, (atr_real)((int)(x1_input) + (atr_real)1), (atr_real)((int)(y1_input + (atr_real)0.5)), atr_rgba_to_hexargb(r, g, b, (int)(a * (dis_start) * overlap)), offzoom);
        overlap = ((y2_input + (atr_real)0.5) - (int)(y2_input + (atr_real)0.5));
        atr_real dis_end = y2_input - (int)y2_input;
        atr_pixel_draw(screen, (atr_real)((int)(x2_input)), (atr_real)((int)(y2_input + (atr_real)0.5)), atr_rgba_to_hexargb(r, g, b, (int)(a * ((atr_real)1 - dis_end) * overlap)), offzoom);
        atr_pixel_draw(screen, (atr_real)((int)(x2_input) + (atr_real)1), (atr_real)((int)(y2_input + (atr_real)0.5)), atr_rgba_to_hexargb(r, g, b, (int)(a * (dis_end) * overlap)), offzoom);

        for (size_t i = 1; i < dy; i++) {
            atr_real y = y1_input + (atr_real)i;
            atr_real x = x1_input + (atr_real)i * m;
            int ix = (int)x;
            int iy = (int)y;
            atr_real down_dis = x - ix;
            atr_real up_dis   = 1 - down_dis;
            atr_pixel_draw(screen, (atr_real)ix, (atr_real)iy, atr_rgba_to_hexargb(r, g, b, (int)(a * up_dis)), offzoom);
            atr_pixel_draw(screen, (atr_real)ix + (atr_real)1, (atr_real)iy, atr_rgba_to_hexargb(r, g, b, (int)(a * down_dis)), offzoom);
        }
    }
}

ATR_DEF void atr_line_draw_fix_width(struct Atr_Pixel_Buffer screen, atr_real x1_input, atr_real y1_input, atr_real x2_input, atr_real y2_input, uint32_t color, struct Atr_Offset_Zoom offzoom)
{
    atr_real window_w = (atr_real)screen.cols;
    atr_real window_h = (atr_real)screen.rows;
    atr_real zoom = offzoom.zoom_multiplier;

    atr_real x1 = (x1_input - window_w/2.0f + offzoom.offset_x) * zoom + window_w/2.0f;
    atr_real y1 = (y1_input - window_h/2.0f + offzoom.offset_y) * zoom + window_h/2.0f;

    atr_real x2 = (x2_input - window_w/2.0f + offzoom.offset_x) * zoom + window_w/2.0f;
    atr_real y2 = (y2_input - window_h/2.0f + offzoom.offset_y) * zoom + window_h/2.0f;

    atr_line_draw(screen, x1, y1, x2, y2, color, ATR_DEFAULT_OFFSET_ZOOM);
}

ATR_DEF void atr_line_draw_no_antialiasing(struct Atr_Pixel_Buffer screen, atr_real x1_input, atr_real y1_input, atr_real x2_input, atr_real y2_input, uint32_t color, struct Atr_Offset_Zoom offzoom)
{
    /* Bresenham draw line function */
    atr_real x0 = atr_round(x1_input);
    atr_real y0 = atr_round(y1_input);
    atr_real x1 = atr_round(x2_input);
    atr_real y1 = atr_round(y2_input);

    int dx = (int)atr_fabs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -(int)atr_fabs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;

    for (;;) {
        atr_pixel_draw(screen, x0, y0, color, offzoom);
        if (x0 == x1 && y0 == y1) {
            break;
        }

        int error2 = error * 2;
        if (error2 >= dy) {
            error += dy;
            x0 += sx;
        }

        if (error2 <= dx) {
            error += dx;
            y0 += sy;
        }
    }
}

ATR_DEF void atr_line_horiz_draw(struct Atr_Pixel_Buffer screen, atr_real x1_input, atr_real x2_input, atr_real y_input, uint32_t color, struct Atr_Offset_Zoom offzoom)
{
    if (x1_input > x2_input) {
        atr_real temp = x1_input;
        x1_input = x2_input;
        x2_input = temp;
    }

    /*
     * Fill pixels whose centers satisfy:
     *
     *     left <= ix + 0.5 < right
     */
    int ix_begin = (int)atr_ceil(x1_input - 0.5f);
    int ix_end = (int)atr_ceil(x2_input - 0.5f);

    for (int ix = ix_begin; ix < ix_end; ++ix) {
        atr_pixel_draw(screen, (atr_real)ix, y_input, color, offzoom);
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
    if (!(atr_4chars_to_uint32_be("true") == font->offset_subtable.scaler_type || 0x00010000 == font->offset_subtable.scaler_type)) {
        if (atr_4chars_to_uint32_be("typ1") == font->offset_subtable.scaler_type) {
            atr_dprintERROR("%s", "Font type recognized as the old style of PostScript font housed in a sfnt wrapper. This type is not supported.");
            return ATR_FAIL;
        }
        if (atr_4chars_to_uint32_be("OTTO") == font->offset_subtable.scaler_type) {
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

ATR_DEF void atr_pixel_draw(struct Atr_Pixel_Buffer screen, atr_real x, atr_real y, uint32_t color, struct Atr_Offset_Zoom offzoom)
{
    atr_real window_w = (atr_real)screen.cols;
    atr_real window_h = (atr_real)screen.rows;
    atr_real zoom = offzoom.zoom_multiplier;
    
    if (ATR_IS_ZERO(zoom - (atr_real)1)) {
        int ix = (int)(x + offzoom.offset_x);
        int iy = (int)(y + offzoom.offset_y);
        if ((ix >= 0 && iy >= 0) && ((size_t)ix < screen.cols && (size_t)iy < screen.rows)) { /* vec2 is in screen */
            ATR_BUFFER_AT(screen, iy, ix) = atr_alpha_blend(ATR_BUFFER_AT(screen, iy, ix), color);
        }
        return;
    }

    atr_real start_x0 = (x - window_w/2.0f + offzoom.offset_x) * zoom + window_w/2.0f;
    atr_real start_y0 = (y - window_h/2.0f + offzoom.offset_y) * zoom + window_h/2.0f;
    atr_real start_x1 = (x + 1 - window_w/2.0f + offzoom.offset_x) * zoom + window_w/2.0f;
    atr_real start_y1 = (y + 1 - window_h/2.0f + offzoom.offset_y) * zoom + window_h/2.0f;

    int ix0 = (int)atr_floor(atr_min(start_x0, start_x1));
    int iy0 = (int)atr_floor(atr_min(start_y0, start_y1));
    int ix1 = (int)atr_ceil(atr_max(start_x0, start_x1));
    int iy1 = (int)atr_ceil(atr_max(start_y0, start_y1));

    if (offzoom.zoom_multiplier <= 0) return;
    int block = (int)(zoom + (atr_real)0.5);
    if (block < 1) block = 1;

    for (int ix = ix0; ix < ix1; ix++) {
        for (int iy = iy0; iy < iy1; iy++) {
            if ((ix >= 0 && iy >= 0) && ((size_t)ix < screen.cols && (size_t)iy < screen.rows)) { /* vec2 is in screen */
                ATR_BUFFER_AT(screen, iy, ix) = atr_alpha_blend(ATR_BUFFER_AT(screen, iy, ix), color);
            }
        }
    }
}

ATR_DEF enum Atr_Return_Types atr_quadratic_bezier_array_fill(struct Atr_Pixel_Buffer screen, struct Atr_Glyph_Point *points, size_t points_count, uint32_t color, struct Atr_Offset_Zoom offzoom)
{
    ATR_ASSERT(points_count % 3 == 0);

    atr_real glyph_y_max = -ATR_INFINITY;
    atr_real glyph_y_min = ATR_INFINITY;

    // atr_dprintINFO("%zu", points_count);
    for (size_t i = 0; i < points_count; i++) {
        if (points[i].pos.y > glyph_y_max) {
            glyph_y_max = points[i].pos.y;
        }
        if (points[i].pos.y < glyph_y_min) {
            glyph_y_min = points[i].pos.y;
        }
    }

    struct Atr_Real_Dynamic_Array intersection_xs = {0};
    atr_ada_init_array(atr_real, intersection_xs);
    struct Atr_Real_Dynamic_Array intersection_dy_dts = {0};
    atr_ada_init_array(atr_real, intersection_dy_dts);
    // atr_dprintINFO("[%f, %f]", glyph_y_min, glyph_y_max);
    int first_row = (int)atr_floor(glyph_y_min);
    int last_row = (int)atr_ceil(glyph_y_max);
    for (int iy = first_row; iy < last_row; ++iy) {
        atr_real scan_y = (atr_real)iy;
        intersection_xs.length = 0;
        intersection_dy_dts.length = 0;
        for (size_t i = 0; i + 2 < points_count; i += 3) {
            struct Atr_Glyph_Point start = points[i + 0];
            struct Atr_Glyph_Point control = points[i + 1];
            struct Atr_Glyph_Point end = points[i + 2];
            atr_real x1, x2, der1, der2;
            size_t intersection_count = atr_quadratic_bezier_get_xs_from_y(start, control, end, scan_y, &x1, &x2, &der1, &der2);
            #if to_debug
            /*
            * Debug every curve segment at the problematic scanline.
            */
            if (scan_y >= (atr_real)(y_bug - 1) && scan_y <= (atr_real)(y_bug + 1)) {
                atr_dprintINFO(
                    "segment=%zu: count=%zu",
                    i / 3,
                    intersection_count
                );

                if (intersection_count >= 1) {
                    atr_dprintINFO(
                        " x1=%f dy1=%f",
                        (double)x1,
                        (double)der1
                    );
                }

                if (intersection_count >= 2) {
                    atr_dprintINFO(
                        " x2=%f dy2=%f",
                        (double)x2,
                        (double)der2
                    );
                }
            }
            #endif
            if (intersection_count >= 1) {
                atr_ada_append(atr_real, intersection_xs, x1);
                atr_ada_append(atr_real, intersection_dy_dts, der1);
            }
            if (intersection_count >= 2) {
                atr_ada_append(atr_real, intersection_xs, x2);
                atr_ada_append(atr_real, intersection_dy_dts, der2);
            }
        }
        if (intersection_xs.length == 0) continue;

        for (size_t i = 1; i < intersection_xs.length; i++) {
            atr_real x = intersection_xs.elements[i];
            atr_real derivative = intersection_dy_dts.elements[i];

            size_t j = i;
            while (j > 0 &&
                intersection_xs.elements[j - 1] > x) {
                intersection_xs.elements[j] = intersection_xs.elements[j - 1];
                intersection_dy_dts.elements[j] = intersection_dy_dts.elements[j - 1];
                j--;
            }

            intersection_xs.elements[j] = x;
            intersection_dy_dts.elements[j] = derivative;
        }

        #if to_debug
            printf("y: %6.2f | ", scan_y);
            for (size_t x_index = 0; x_index < intersection_xs.length; x_index++) {
                atr_real xi     = intersection_xs.elements[x_index];
                printf("%6.2f ", xi);
            }
            printf("\n");
            printf("          ");
            int winding = 0;
            for (size_t x_index = 0; x_index < intersection_xs.length; x_index++) {
                atr_real xi     = intersection_xs.elements[x_index];
                atr_real deri   = intersection_dy_dts.elements[x_index];
                if (deri > 0) winding++;
                if (deri <= 0) winding--;

                printf("%6d ", winding);
            }
            printf("\n");
            if (winding != 0) {
                atr_dprintERROR(
                    "unbalanced scanline: y=%f winding=%d intersections=%zu\n",
                    (double)scan_y,
                    winding,
                    intersection_xs.length
                );
            }
        #else 
        int winding = 0;
        size_t x_index = 0;

        while (x_index < intersection_xs.length) {
            atr_real x_left = intersection_xs.elements[x_index];
            while (x_index < intersection_xs.length && ATR_IS_ZERO(intersection_xs.elements[x_index] - x_left)) {
                atr_real derivative = intersection_dy_dts.elements[x_index];
                winding += derivative > 0 ? 1 : -1;
                x_index++;
            }

            if (x_index >= intersection_xs.length) {
                break;
            }

            atr_real x_right = intersection_xs.elements[x_index];
            if (winding != 0 && x_right > x_left) {
                atr_line_draw(screen, x_left, scan_y, x_right, scan_y, color, offzoom);
            }
        }
        if (winding != 0) {
            atr_dprintERROR("%s", "incorrect winding number.");
            return ATR_FAIL;
        }
        #endif
    }
    #if to_debug
    ATR_ASSERT(0);
    #endif

    ATR_FREE(intersection_xs.elements);
    ATR_FREE(intersection_dy_dts.elements);

    return ATR_SUCCESS;
}

ATR_DEF enum Atr_Return_Types atr_quadratic_bezier_array_fill_no_antialiasing(struct Atr_Pixel_Buffer screen, struct Atr_Glyph_Point *points, size_t points_count, uint32_t color, struct Atr_Offset_Zoom offzoom)
{
    ATR_ASSERT(points_count % 3 == 0);

    atr_real glyph_y_max = -ATR_INFINITY;
    atr_real glyph_y_min = ATR_INFINITY;

    // atr_dprintINFO("%zu", points_count);
    for (size_t i = 0; i < points_count; i++) {
        if (points[i].pos.y > glyph_y_max) {
            glyph_y_max = points[i].pos.y;
        }
        if (points[i].pos.y < glyph_y_min) {
            glyph_y_min = points[i].pos.y;
        }
    }

    struct Atr_Real_Dynamic_Array intersection_xs = {0};
    atr_ada_init_array(atr_real, intersection_xs);
    struct Atr_Real_Dynamic_Array intersection_dy_dts = {0};
    atr_ada_init_array(atr_real, intersection_dy_dts);
    // atr_dprintINFO("[%f, %f]", glyph_y_min, glyph_y_max);
    int first_row = (int)atr_floor(glyph_y_min);
    int last_row = (int)atr_ceil(glyph_y_max);
    for (int iy = first_row; iy < last_row; ++iy) {
        atr_real scan_y = (atr_real)iy + (atr_real)0.5;
        intersection_xs.length = 0;
        intersection_dy_dts.length = 0;
        for (size_t i = 0; i + 2 < points_count; i += 3) {
            struct Atr_Glyph_Point start = points[i + 0];
            struct Atr_Glyph_Point control = points[i + 1];
            struct Atr_Glyph_Point end = points[i + 2];
            atr_real x1, x2, der1, der2;
            size_t intersection_count = atr_quadratic_bezier_get_xs_from_y(start, control, end, scan_y, &x1, &x2, &der1, &der2);
            #if to_debug
            /*
            * Debug every curve segment at the problematic scanline.
            */
            if (scan_y >= (atr_real)(y_bug - 1) && scan_y <= (atr_real)(y_bug + 1)) {
                atr_dprintINFO(
                    "segment=%zu: count=%zu",
                    i / 3,
                    intersection_count
                );

                if (intersection_count >= 1) {
                    atr_dprintINFO(
                        " x1=%f dy1=%f",
                        (double)x1,
                        (double)der1
                    );
                }

                if (intersection_count >= 2) {
                    atr_dprintINFO(
                        " x2=%f dy2=%f",
                        (double)x2,
                        (double)der2
                    );
                }
            }
            #endif
            if (intersection_count >= 1) {
                atr_ada_append(atr_real, intersection_xs, x1);
                atr_ada_append(atr_real, intersection_dy_dts, der1);
            }
            if (intersection_count >= 2) {
                atr_ada_append(atr_real, intersection_xs, x2);
                atr_ada_append(atr_real, intersection_dy_dts, der2);
            }
        }
        if (intersection_xs.length == 0) continue;

        for (size_t i = 1; i < intersection_xs.length; i++) {
            atr_real x = intersection_xs.elements[i];
            atr_real derivative = intersection_dy_dts.elements[i];

            size_t j = i;
            while (j > 0 &&
                intersection_xs.elements[j - 1] > x) {
                intersection_xs.elements[j] = intersection_xs.elements[j - 1];
                intersection_dy_dts.elements[j] = intersection_dy_dts.elements[j - 1];
                j--;
            }

            intersection_xs.elements[j] = x;
            intersection_dy_dts.elements[j] = derivative;
        }

        #if to_debug
            printf("y: %6.2f | ", scan_y);
            for (size_t x_index = 0; x_index < intersection_xs.length; x_index++) {
                atr_real xi     = intersection_xs.elements[x_index];
                printf("%6.2f ", xi);
            }
            printf("\n");
            printf("          ");
            int winding = 0;
            for (size_t x_index = 0; x_index < intersection_xs.length; x_index++) {
                atr_real xi     = intersection_xs.elements[x_index];
                atr_real deri   = intersection_dy_dts.elements[x_index];
                if (deri > 0) winding++;
                if (deri <= 0) winding--;

                printf("%6d ", winding);
            }
            printf("\n");
            if (winding != 0) {
                atr_dprintERROR(
                    "unbalanced scanline: y=%f winding=%d intersections=%zu\n",
                    (double)scan_y,
                    winding,
                    intersection_xs.length
                );
            }
        #else 
        int winding = 0;
        size_t x_index = 0;

        while (x_index < intersection_xs.length) {
            atr_real x_left = intersection_xs.elements[x_index];
            while (x_index < intersection_xs.length && ATR_IS_ZERO(intersection_xs.elements[x_index] - x_left)) {
                atr_real derivative = intersection_dy_dts.elements[x_index];
                winding += derivative > 0 ? 1 : -1;
                x_index++;
            }

            if (x_index >= intersection_xs.length) {
                break;
            }

            atr_real x_right = intersection_xs.elements[x_index];
            if (winding != 0 && x_right > x_left) {
                atr_line_horiz_draw(screen, x_left, x_right, scan_y, color, offzoom);
            }
        }
        if (winding != 0) {
            atr_dprintERROR("%s", "incorrect winding number.");
            return ATR_FAIL;
        }
        #endif
    }
    #if to_debug
    ATR_ASSERT(0);
    #endif

    ATR_FREE(intersection_xs.elements);
    ATR_FREE(intersection_dy_dts.elements);

    return ATR_SUCCESS;
}

ATR_DEF void atr_quadratic_bezier_draw(struct Atr_Pixel_Buffer pixels, struct Atr_Glyph_Point start, struct Atr_Glyph_Point control, struct Atr_Glyph_Point end, uint32_t color, struct Atr_Offset_Zoom offzoom)
{
    /*
     * Increase this if curves look visibly segmented at high zoom.
     * A more advanced renderer would adapt this based on curve length.
     */
    const size_t steps = 5;

    for (size_t i = 0; i < steps; ++i) {
        atr_real t_i = (atr_real)i / (atr_real)steps;
        atr_real t_ip1 = (atr_real)(i + 1) / (atr_real)steps;

        atr_real inverse_t_i = (atr_real)1 - t_i;
        atr_real inverse_t_ip1 = (atr_real)1 - t_ip1;

        atr_real x_i = inverse_t_i * inverse_t_i * start.pos.x +
            (atr_real)2 * inverse_t_i * t_i * control.pos.x +
            t_i * t_i * end.pos.x;
        atr_real x_ip1 = inverse_t_ip1 * inverse_t_ip1 * start.pos.x +
            (atr_real)2 * inverse_t_ip1 * t_ip1 * control.pos.x +
            t_ip1 * t_ip1 * end.pos.x;

        atr_real y_i = inverse_t_i * inverse_t_i * start.pos.y +
            (atr_real)2 * inverse_t_i * t_i * control.pos.y +
            t_i * t_i * end.pos.y;
        atr_real y_ip1 = inverse_t_ip1 * inverse_t_ip1 * start.pos.y +
            (atr_real)2 * inverse_t_ip1 * t_ip1 * control.pos.y +
            t_ip1 * t_ip1 * end.pos.y;

        atr_line_draw_fix_width(pixels, x_i, y_i, x_ip1, y_ip1, color, offzoom);
    }

    atr_circle_fill_high_quality(pixels, start.pos.x, start.pos.y, 1, 0xFF00FFFF, offzoom);
    atr_circle_fill_high_quality(pixels, end.pos.x, end.pos.y, 1, 0xFF00FFFF, offzoom);
    atr_circle_fill_high_quality(pixels, control.pos.x, control.pos.y, 1, 0xFFFF0000, offzoom);
}

ATR_DEF size_t atr_quadratic_bezier_get_xs_from_y(struct Atr_Glyph_Point start, struct Atr_Glyph_Point control, struct Atr_Glyph_Point end, atr_real y, atr_real *x1, atr_real *x2, atr_real *dy_dt1, atr_real *dy_dt2)
{
    /* Fine tuning by AI */
    atr_real dx12 = control.pos.x - start.pos.x;
    atr_real dx23 = end.pos.x     - control.pos.x;
    atr_real dy12 = control.pos.y - start.pos.y;
    atr_real dy23 = end.pos.y     - control.pos.y;

    /** formula:
     * atr_real x     = (dx23 - dx12) * t * t + 2 * dx12 * t + start.pos.x
     * atr_real y     = (dy23 - dy12) * t * t + 2 * dy12 * t + start.pos.y
     * atr_real dy/dt = 2 * (dy23 - dy12) * t + 2 * dy12
     */

    atr_real a = (dy23 - dy12);
    atr_real b = 2 * dy12;
    atr_real c = start.pos.y - y;

    #if to_debug
    bool debug_scan = atr_fabs(y - (atr_real)(y_bug)) < (atr_real)0.001;
    if (debug_scan) {
        atr_dprintINFO(
            "    equation: "
            "start_y=%9.7f control_y=%9.7f end_y=%9.7f "
            "a=% .9g b=% .9g c=% .9g\n",
            (double)start.pos.y,
            (double)control.pos.y,
            (double)end.pos.y,
            (double)a,
            (double)b,
            (double)c
        );
    }
    #endif

    atr_real roots[2];
    size_t root_count = 0;

    if (ATR_IS_ZERO(a)) {
        if (ATR_IS_ZERO(b)) {
            return 0;
        }

        roots[root_count++] = -c / b;
    } else {
        atr_real d = b * b - (atr_real)4 * a * c;

        atr_real d_scale = atr_fabs(b * b) + atr_fabs((atr_real)4 * a * c) + (atr_real)1;
        atr_real d_tolerance = (atr_real)16 * (atr_real)ATR_EPS * d_scale;

        if (d < -d_tolerance) {
            return 0;
        }

        if (d < (atr_real)0) {
            d = (atr_real)0;
        }

        if (d == (atr_real)0) {
            roots[root_count++] = -b / ((atr_real)2 * a);
        } else {
            atr_real sqrt_d = atr_sqrt(d);
            atr_real q;

            /*
            * Choose the sign that avoids subtracting nearly equal values.
            */
            if (b >= (atr_real)0) {
                q = (atr_real)-0.5 * (b + sqrt_d);
            } else {
                q = (atr_real)-0.5 * (b - sqrt_d);
            }

            if (q == (atr_real)0) {
                roots[root_count++] = -b / ((atr_real)2 * a);
            } else {
                roots[root_count++] = q / a;
                roots[root_count++] = c / q;
            }
        }
    }

    const atr_real root_tolerance = (atr_real)8 * (atr_real)ATR_EPS;

    size_t count = 0;

    for (size_t i = 0; i < root_count; ++i) {
    #if to_debug
        if (debug_scan) {
            atr_dprintINFO("        raw root[%zu]=%.9g\n", i, (double)roots[i]);
        }
    #endif
        atr_real t = roots[i];
        bool at_start = false;
        bool at_end = false;

        if (start.pos.y == y && atr_fabs(t) <= root_tolerance) {
            t = (atr_real)0;
            at_start = true;
        } else if (end.pos.y == y && atr_fabs(t - (atr_real)1) <= root_tolerance) {
            t = (atr_real)1;
            at_end = true;
        } else if (t <= (atr_real)0 || t >= (atr_real)1) {
    #if to_debug
            if (debug_scan) {
                atr_dprintINFO("        rejected: non-interior root %.9g\n", (double)t);
            }
    #endif
            continue;
        }

        atr_real derivative = (atr_real)2 * a * t + b;

        if ((at_start || at_end) && atr_fabs(derivative) <= root_tolerance) {
            /*
            * Use the one-sided direction when the endpoint has a horizontal
            * tangent.
            */
            derivative = at_start ? a : -a;
        }

        if (!at_start && !at_end && derivative == (atr_real)0) {
            /*
            * A stationary interior root is a tangency rather than a crossing.
            */
            continue;
        }

        if (!atr_quadratic_bezier_root_is_crossing(at_start, at_end, derivative)) {
    #if to_debug
            if (debug_scan) {
                atr_dprintINFO("        rejected by crossing rule: "
                    "t=%.9g at_start=%d at_end=%d "
                    "derivative=%.9g\n",
                    (double)t,
                    at_start,
                    at_end,
                    (double)derivative
                );
            }
    #endif
            continue;
        }

        atr_real x = (dx23 - dx12) * t * t + (atr_real)2 * dx12 * t + start.pos.x;
        if (count == 0) {
            if (x1 != NULL) {
                *x1 = x;
            }
            if (dy_dt1 != NULL) {
                *dy_dt1 = derivative;
            }
        } else {
            if (x2 != NULL) {
                *x2 = x;
            }
            if (dy_dt2 != NULL) {
                *dy_dt2 = derivative;
            }
        }

        ++count;
    }

    return count;
}

ATR_DEF bool atr_quadratic_bezier_root_is_crossing(bool at_start, bool at_end, atr_real dy_dt)
{
    /* By AI */
    if (at_start) {
        return dy_dt > (atr_real)0;
    }

    if (at_end) {
        return dy_dt < (atr_real)0;
    }

    return true;
}


ATR_DEF void atr_rectangle_draw_min_max(struct Atr_Pixel_Buffer screen, atr_real min_x, atr_real max_x, atr_real min_y, atr_real max_y, uint32_t color, struct Atr_Offset_Zoom offzoom)
{
    atr_line_draw_no_antialiasing(screen, min_x, min_y, max_x, min_y, color, offzoom);
    atr_line_draw_no_antialiasing(screen, max_x, min_y, max_x, max_y, color, offzoom);
    atr_line_draw_no_antialiasing(screen, max_x, max_y, min_x, max_y, color, offzoom);
    atr_line_draw_no_antialiasing(screen, min_x, max_y, min_x, min_y, color, offzoom);
}

ATR_DEF uint32_t atr_rgba_to_hexargb(int r, int g, int b, int a)
{
    uint32_t ru = atr_u8_clamp_int(r);
    uint32_t gu = atr_u8_clamp_int(g);
    uint32_t bu = atr_u8_clamp_int(b);
    uint32_t au = atr_u8_clamp_int(a);

    return (au << 24) | (ru << 16) | (gu << 8) | bu;
}

ATR_DEF atr_real atr_scale_get_for_em(struct Atr_Font *font, atr_real pixels_per_em)
{
    atr_real units_per_em = font->tables.head.unitsPerEm;
    return pixels_per_em / units_per_em;
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
            ATR_FREE(st->data.format_4.glyphIndexArray);
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
            if (st.data.format_4.endCode == NULL && (st.data.format_4.segCountx2 / 2) > 0) {
                atr_dprintERROR("%s", "Failed to allocate endCode array.");
                return ATR_FAIL;
            }
            for (size_t gi = 0; gi < st.data.format_4.segCountx2 / 2; gi++) {
                st.data.format_4.endCode[gi]        = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            }
            atr_bit_reader_read_bytes(&br_st, 2); /* reserved */
            st.data.format_4.startCode              = ATR_MALLOC(sizeof(uint16_t) * st.data.format_4.segCountx2 / 2);
            if (st.data.format_4.startCode == NULL && (st.data.format_4.segCountx2 / 2)) {
                atr_dprintERROR("%s", "Failed to allocate startCode array.");
                return ATR_FAIL;
            }
            for (size_t gi = 0; gi < st.data.format_4.segCountx2 / 2; gi++) {
                st.data.format_4.startCode[gi]      = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            }
            st.data.format_4.idDelta                = ATR_MALLOC(sizeof(uint16_t) * st.data.format_4.segCountx2 / 2);
            if (st.data.format_4.idDelta == NULL && (st.data.format_4.segCountx2 / 2)) {
                atr_dprintERROR("%s", "Failed to allocate idDelta array.");
                return ATR_FAIL;
            }
            for (size_t gi = 0; gi < st.data.format_4.segCountx2 / 2; gi++) {
                st.data.format_4.idDelta[gi]        = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            }
            st.data.format_4.idRangeOffset          = ATR_MALLOC(sizeof(uint16_t) * st.data.format_4.segCountx2 / 2);
            if (st.data.format_4.idRangeOffset == NULL && (st.data.format_4.segCountx2 / 2)) {
                atr_dprintERROR("%s", "Failed to allocate idRangeOffset array.");
                return ATR_FAIL;
            }
            for (size_t gi = 0; gi < st.data.format_4.segCountx2 / 2; gi++) {
                st.data.format_4.idRangeOffset[gi]  = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            }
            size_t fixed_size = 16 + 4 * st.data.format_4.segCountx2;
            if (fixed_size > st.data.format_4.length) {
                atr_dprintERROR("%s", "Error while parsing cmap subtable with format 4.");
                return ATR_FAIL;
            }
            size_t glyph_id_bytes = st.data.format_4.length - fixed_size;
            if (glyph_id_bytes % 2 != 0) {
                atr_dprintERROR("%s", "Error while parsing cmap subtable with format 4. Glyph id byte count is not even.");
                return ATR_FAIL;
            }
            st.data.format_4.glyphIndexCount = glyph_id_bytes / sizeof(uint16_t);
            st.data.format_4.glyphIndexArray         = ATR_MALLOC(sizeof(uint16_t) * st.data.format_4.glyphIndexCount);
            if (st.data.format_4.glyphIndexArray == NULL && st.data.format_4.glyphIndexCount > 0) {
                atr_dprintERROR("%s", "Failed to allocate glyphIndexArray array.");
                return ATR_FAIL;
            }
            for (size_t gi = 0; gi < st.data.format_4.glyphIndexCount; gi++) {
                st.data.format_4.glyphIndexArray[gi] = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            }
        } else if (st.format == 6) {
            st.data.format_6.length                  = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            st.data.format_6.language                = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            st.data.format_6.firstCode               = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            st.data.format_6.entryCount              = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            st.data.format_6.glyphIndexArray         = ATR_MALLOC(sizeof(uint16_t) * st.data.format_6.entryCount);
            if (st.data.format_6.glyphIndexArray == NULL && st.data.format_6.entryCount > 0) {
                atr_dprintERROR("%s", "Failed to allocate glyphIndexArray array.");
                return ATR_FAIL;
            }
            for (size_t gi = 0; gi < st.data.format_6.entryCount; gi++) {
                st.data.format_6.glyphIndexArray[gi] = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            }
        } else if (st.format == 10) {
            atr_bit_reader_read_bytes(&br_st, 2); /* reserved */
            st.data.format_10.length        = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
            st.data.format_10.language      = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
            st.data.format_10.startCharCode = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
            st.data.format_10.numChars      = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
            st.data.format_10.glyphs        = ATR_MALLOC(sizeof(uint16_t) * st.data.format_10.numChars);
            if (st.data.format_10.glyphs == NULL && st.data.format_10.numChars > 0) {
                atr_dprintERROR("%s", "Failed to allocate glyphs array.");
                return ATR_FAIL;
            }
            for (size_t gi = 0; gi < st.data.format_10.numChars; gi++) {
                st.data.format_10.glyphs[gi] = atr_endian_swap_uint16((uint16_t)atr_bit_reader_read_bytes(&br_st, 2));
            }
        } else if (st.format == 12) {
            atr_bit_reader_read_bytes(&br_st, 2); /* reserved */
            st.data.format_12.length   = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
            st.data.format_12.language = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
            st.data.format_12.nGroups  = atr_endian_swap_uint32(atr_bit_reader_read_bytes(&br_st, 4));
            st.data.format_12.groups   = ATR_MALLOC(sizeof(struct Atr_Table_cmap_Group) * st.data.format_12.nGroups);
            if (st.data.format_12.groups == NULL && st.data.format_12.nGroups > 0) {
                atr_dprintERROR("%s", "Failed to allocate groups array.");
                return ATR_FAIL;
            }
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
            if (st.data.format_13.groups == NULL && st.data.format_13.nGroups > 0) {
                atr_dprintERROR("%s", "Failed to allocate groups array.");
                return ATR_FAIL;
            }
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
        atr_dprintERROR("%s", "Could not find a supported cmap subtable. Unable to continue to parse the font. Only supports formats: 0/4/6/12/13.");
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
    /* By AI */
    uint16_t platform = subtable->platformID;
    uint16_t encoding = subtable->platformSpecificID;
    uint16_t format = subtable->format;

    /**
     * Full-repertoire Unicode.
     *
     * Formats 10 and 12 can represent Unicode code points outside the BMP.
     * No ordering is defined between 0/4 and 3/10; the first encountered
     * subtable with this score wins.
     */
    if (format == 10 || format == 12) {
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
     * Formats 4 and 6 map 16-bit character codes only. No ordering is
     * defined between 0/3 and 3/1.
     */
    if (format == 4 || format == 6) {
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
    if (font->table_directory.elements == NULL && count > 0) {
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

ATR_DEF void atr_table_glyf_free(struct Atr_Font *font)
{
    ATR_ASSERT(font);
    struct Atr_Table_glyf *g = &font->tables.glyf;
    for (size_t i = 0; i < g->num_of_glyphs; i ++) {
        atr_glyph_free(&g->glyphs[i]);
    }
    ATR_FREE(g->glyphs);
    g->glyphs = NULL;
}

ATR_DEF enum Atr_Return_Types atr_table_glyf_parse(struct Atr_Font *font, struct Atr_Table_Header glyf_header)
{
    struct Atr_Table_glyf parsed = {0};
    size_t glyph_count = font->tables.maxp.numGlyphs;

    parsed.header = glyf_header;
    parsed.num_of_glyphs = glyph_count;

    if (glyph_count != 0) {
        parsed.glyphs = ATR_MALLOC(sizeof(*parsed.glyphs) * glyph_count);
        if (parsed.glyphs == NULL) {
            atr_dprintERROR("%s", "Failed to allocate glyph array.");
            return ATR_FAIL;
        }
        for (size_t i = 0; i < glyph_count; ++i) {
            parsed.glyphs[i] = (struct Atr_Glyph){0};
        }
    }

    struct Atr_Bit_Reader gbr = {0};
    for (size_t i = 0; i < glyph_count; ++i) {
        uint32_t start = font->tables.loca.offsets[i];
        uint32_t end = font->tables.loca.offsets[i + 1];
        if (start > end || end > glyf_header.length) {
            atr_dprintERROR("Invalid loca range for glyph %zu: [%u, %u).", i, start, end);
            goto fail;
        }
        if (start == end) {
            continue;
        }
        atr_bit_reader_init_bounded(&gbr, font->file, start + glyf_header.offset, end + glyf_header.offset);
        if (ATR_FAIL == atr_glyph_parse(&parsed.glyphs[i], gbr)) {
            atr_dprintERROR("Failed to parse glyph %zu.", i);
            goto fail;
        }
    }

    atr_table_glyf_free(font);
    font->tables.glyf = parsed;
    return ATR_SUCCESS;

    fail:
        for (size_t i = 0; i < glyph_count; ++i) {
            atr_glyph_free(&parsed.glyphs[i]);
        }
        ATR_FREE(parsed.glyphs);
        return ATR_FAIL;
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
    if (font->tables.maxp.maxZones < 1 || font->tables.maxp.maxZones > 2) {
        atr_dprintWARNING("Invalid maxZones value %u; expected 1 or 2.", font->tables.maxp.maxZones);
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

ATR_DEF struct Atr_Vec2 atr_text_line_draw(struct Atr_Pixel_Buffer screen, struct Atr_Font *font, uint8_t *text, atr_real top_left_x, atr_real top_left_y, atr_real letter_hight, atr_real letter_spacing, uint32_t color, int length, struct Atr_Offset_Zoom offzoom)
{
    size_t text_byte_count = strlen((const char *)text);
    size_t utf8_len = atr_utf8_length(text, text_byte_count);
    if (utf8_len < length) {
        length = (int)utf8_len;
    }
    if (length == -1) {
        length = (int)utf8_len;
    }

    atr_real glyph_y_max = -ATR_INFINITY;
    atr_real glyph_y_min = ATR_INFINITY;
    bool has_drawable_glyph = false;
    size_t consumed = 0;
    for (size_t text_index = 0, char_index = 0; text_index < text_byte_count && char_index < length; text_index += consumed, char_index++) {
        uint32_t c = atr_utf8_decode_next_code_point(text + text_index, text_byte_count - text_index, &consumed);
        if (c == ' ') {
            continue;
        }
        struct Atr_Glyph g = font->tables.glyf.glyphs[atr_glyphIndex_get(font, c)];
        if (g.metadata.yMax > glyph_y_max) {
            glyph_y_max = g.metadata.yMax;
        }
        if (g.metadata.yMin < glyph_y_min) {
            glyph_y_min = g.metadata.yMin;
        }
        has_drawable_glyph = true;
    }
    if (!has_drawable_glyph) {
        return (struct Atr_Vec2){.x = 0, .y = 0};
    }

    atr_real scale = atr_scale_get_for_em(font, letter_hight);
    atr_real pen_x = 0;
    atr_real pen_y = glyph_y_max * scale;
    for (size_t text_index = 0, char_index = 0; text_index < text_byte_count && char_index < length; text_index += consumed, char_index++) {
        uint32_t c = atr_utf8_decode_next_code_point(text + text_index, text_byte_count - text_index, &consumed);
        if (c == ' ') {
            pen_x += 300 * scale;
            continue;
        }

        struct Atr_Glyph g = font->tables.glyf.glyphs[atr_glyphIndex_get(font, c)];
        adl_real x_origin = top_left_x + pen_x;
        adl_real y_origin = top_left_y;
        adl_real x_offset = -g.metadata.xMin * scale;
        // adl_real x_offset = 0;
        adl_real y_offset = pen_y;
        // if (x_origin + x_offset + g.metadata.xMax * scale > screen.cols) {
        //     break;
        // } 
        for (size_t i = 0; i < g.simple.points.length; i++ ) {
            struct Atr_Glyph_Point point = g.simple.points.elements[i];
            g.simple.points_temp_for_resizing.elements[i] = (struct Atr_Glyph_Point){
                .flag = point.flag,
                .pos.x = x_origin + x_offset + point.pos.x * scale,
                .pos.y = y_origin + y_offset - point.pos.y * scale,
            };
        }

        // atr_dprintSIZE_T(g.simple.points.length);

        if (ATR_FAIL == atr_quadratic_bezier_array_fill(screen, g.simple.points_temp_for_resizing.elements, g.simple.points_temp_for_resizing.length, color, offzoom)) {
            atr_dprintERROR("Failed to raseter the letter %c. x_origin = %10.10f, y_origin = %f", (char)c, x_origin, y_origin);
            ATR_ASSERT(0);
        }

        pen_x += letter_spacing + (g.metadata.xMax - g.metadata.xMin) * scale;
    }


    return (struct Atr_Vec2){
        .x = pen_x,
        .y = (glyph_y_max - glyph_y_min) * scale,
    };
}

ATR_DEF struct Atr_Vec2 atr_text_line_draw_no_antialiasing(struct Atr_Pixel_Buffer screen, struct Atr_Font *font, uint8_t *text, atr_real top_left_x, atr_real top_left_y, atr_real letter_hight, atr_real letter_spacing, uint32_t color, int length, struct Atr_Offset_Zoom offzoom)
{
    size_t text_byte_count = strlen((const char *)text);
    size_t utf8_len = atr_utf8_length(text, text_byte_count);
    if (utf8_len < length) {
        length = (int)utf8_len;
    }
    if (length == -1) {
        length = (int)utf8_len;
    }

    atr_real glyph_y_max = -ATR_INFINITY;
    atr_real glyph_y_min = ATR_INFINITY;
    bool has_drawable_glyph = false;
    size_t consumed = 0;
    for (size_t text_index = 0, char_index = 0; text_index < text_byte_count && char_index < length; text_index += consumed, char_index++) {
        uint32_t c = atr_utf8_decode_next_code_point(text + text_index, text_byte_count - text_index, &consumed);
        if (c == ' ') {
            continue;
        }
        struct Atr_Glyph g = font->tables.glyf.glyphs[atr_glyphIndex_get(font, c)];
        if (g.metadata.yMax > glyph_y_max) {
            glyph_y_max = g.metadata.yMax;
        }
        if (g.metadata.yMin < glyph_y_min) {
            glyph_y_min = g.metadata.yMin;
        }
        has_drawable_glyph = true;
    }
    if (!has_drawable_glyph) {
        return (struct Atr_Vec2){.x = 0, .y = 0};
    }

    atr_real scale = atr_scale_get_for_em(font, letter_hight);
    atr_real pen_x = 0;
    atr_real pen_y = glyph_y_max * scale;
    for (size_t text_index = 0, char_index = 0; text_index < text_byte_count && char_index < length; text_index += consumed, char_index++) {
        uint32_t c = atr_utf8_decode_next_code_point(text + text_index, text_byte_count - text_index, &consumed);
        if (c == ' ') {
            pen_x += 300 * scale;
            continue;
        }

        struct Atr_Glyph g = font->tables.glyf.glyphs[atr_glyphIndex_get(font, c)];
        adl_real x_origin = top_left_x + pen_x;
        adl_real y_origin = top_left_y;
        adl_real x_offset = -g.metadata.xMin * scale;
        // adl_real x_offset = 0;
        adl_real y_offset = pen_y;
        // if (x_origin + x_offset + g.metadata.xMax * scale > screen.cols) {
        //     break;
        // } 
        for (size_t i = 0; i < g.simple.points.length; i++ ) {
            struct Atr_Glyph_Point point = g.simple.points.elements[i];
            g.simple.points_temp_for_resizing.elements[i] = (struct Atr_Glyph_Point){
                .flag = point.flag,
                .pos.x = x_origin + x_offset + point.pos.x * scale,
                .pos.y = y_origin + y_offset - point.pos.y * scale,
            };
        }

        // atr_dprintSIZE_T(g.simple.points.length);

        if (ATR_FAIL == atr_quadratic_bezier_array_fill_no_antialiasing(screen, g.simple.points_temp_for_resizing.elements, g.simple.points_temp_for_resizing.length, color, offzoom)) {
            atr_dprintERROR("Failed to raseter the letter %c. x_origin = %10.10f, y_origin = %f", (char)c, x_origin, y_origin);
            ATR_ASSERT(0);
        }

        pen_x += letter_spacing + (g.metadata.xMax - g.metadata.xMin) * scale;
    }


    return (struct Atr_Vec2){
        .x = pen_x,
        .y = (glyph_y_max - glyph_y_min) * scale,
    };
}

ATR_DEF struct Atr_Vec2 atr_text_line_draw_outline(struct Atr_Pixel_Buffer screen, struct Atr_Font *font, uint8_t *text, atr_real top_left_x, atr_real top_left_y, atr_real letter_hight, atr_real letter_spacing, uint32_t color, int length, struct Atr_Offset_Zoom offzoom)
{
    size_t text_byte_count = strlen((const char *)text);
    size_t utf8_len = atr_utf8_length(text, text_byte_count);
    if (utf8_len < length) {
        length = (int)utf8_len;
    }
    if (length == -1) {
        length = (int)utf8_len;
    }

    atr_real glyph_y_max = -ATR_INFINITY;
    atr_real glyph_y_min = ATR_INFINITY;
    bool has_drawable_glyph = false;
    size_t consumed = 0;
    for (size_t text_index = 0; text_index < text_byte_count; text_index += consumed) {
        uint32_t c = atr_utf8_decode_next_code_point(text + text_index, text_byte_count - text_index, &consumed);
        if (c == ' ') {
            continue;
        }
        struct Atr_Glyph g = font->tables.glyf.glyphs[atr_glyphIndex_get(font, c)];
        if (g.metadata.yMax > glyph_y_max) {
            glyph_y_max = g.metadata.yMax;
        }
        if (g.metadata.yMin < glyph_y_min) {
            glyph_y_min = g.metadata.yMin;
        }
        has_drawable_glyph = true;
    }
    if (!has_drawable_glyph) {
        return (struct Atr_Vec2){.x = 0, .y = 0};
    }

    atr_real scale = atr_scale_get_for_em(font, letter_hight);
    atr_real pen_x = 0;
    atr_real pen_y = glyph_y_max * scale;
    for (size_t text_index = 0; text_index < text_byte_count; text_index += consumed) {
        uint32_t c = atr_utf8_decode_next_code_point(text + text_index, text_byte_count - text_index, &consumed);
        if (c == ' ') {
            pen_x += 300 * scale;
            continue;
        }

        struct Atr_Glyph g = font->tables.glyf.glyphs[atr_glyphIndex_get(font, c)];
        adl_real x_origin = top_left_x + pen_x;
        adl_real y_origin = top_left_y;
        adl_real x_offset = -g.metadata.xMin * scale;
        // adl_real x_offset = 0;
        adl_real y_offset = pen_y;
        // if (x_origin + x_offset + g.metadata.xMax * scale > screen.cols) {
        //     break;
        // } 
        for (size_t i = 0; i < g.simple.points.length; i++ ) {
            struct Atr_Glyph_Point point = g.simple.points.elements[i];
            g.simple.points_temp_for_resizing.elements[i] = (struct Atr_Glyph_Point){
                .flag = point.flag,
                .pos.x = x_origin + x_offset + point.pos.x * scale,
                .pos.y = y_origin + y_offset - point.pos.y * scale,
            };
        }

        for (size_t i = 0; i + 2 < g.simple.points_temp_for_resizing.length; i += 3) {
            struct Atr_Glyph_Point start = g.simple.points_temp_for_resizing.elements[i + 0];
            struct Atr_Glyph_Point control = g.simple.points_temp_for_resizing.elements[i + 1];
            struct Atr_Glyph_Point end = g.simple.points_temp_for_resizing.elements[i + 2];
            atr_quadratic_bezier_draw(screen, start, control, end, color, offzoom);
        }

        pen_x += letter_spacing + (g.metadata.xMax - g.metadata.xMin) * scale;
    }

    return (struct Atr_Vec2){
        .x = pen_x,
        .y = (glyph_y_max - glyph_y_min) * scale,
    };
}

ATR_DEF uint8_t atr_u8_clamp_int(int x)
{
    if (x < 0) {
        return 0;
    }
    if (x > 255) {
        return 255;
    }
    return (uint8_t)x;
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

/* This solution for utf-8 is not a full support for utf-8 */
ATR_DEF uint32_t atr_utf8_code_point_get_from_raw_char_bytes(uint32_t raw_char_bytes)
{
    /* By AI */
    uint8_t first_byte = (uint8_t)((raw_char_bytes >> 0) & 0xFFu);
    uint8_t second_byte = (uint8_t)((raw_char_bytes >> 8) & 0xFFu);
    uint8_t third_byte = (uint8_t)((raw_char_bytes >> 16) & 0xFFu);
    uint8_t fourth_byte = (uint8_t)((raw_char_bytes >> 24) & 0xFFu);

    if (first_byte <= 0x7Fu) {
        /* 0xxxxxxx */
        return (uint32_t)first_byte;
    }

    if (first_byte >= 0xC2u && first_byte <= 0xDFu) {
        /* 110xxxxx 10xxxxxx */

        if ((second_byte & 0xC0u) != 0x80u) {
            return ATR_UTF8_REPLACEMENT_CHARACTER;
        }

        return ((uint32_t)(first_byte & 0x1Fu) << 6) |
               ((uint32_t)(second_byte & 0x3Fu) << 0);
    }

    if (first_byte >= 0xE0u && first_byte <= 0xEFu) {
        /* 1110xxxx 10xxxxxx 10xxxxxx */

        if ((second_byte & 0xC0u) != 0x80u ||
            (third_byte & 0xC0u) != 0x80u) {
            return ATR_UTF8_REPLACEMENT_CHARACTER;
        }

        uint32_t code_point = ((uint32_t)(first_byte & 0x0Fu) << 12) |
                              ((uint32_t)(second_byte & 0x3Fu) << 6) |
                              ((uint32_t)(third_byte & 0x3Fu) << 0);

        /*
         * Reject:
         * - overlong sequences, such as E0 80 80 for U+0000
         * - surrogate code points, U+D800 through U+DFFF
         */
        if (code_point < 0x800u ||
            (code_point >= 0xD800u && code_point <= 0xDFFFu)) {
            return ATR_UTF8_REPLACEMENT_CHARACTER;
        }

        return code_point;
    }

    if (first_byte >= 0xF0u && first_byte <= 0xF4u) {
        /* 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */

        if ((second_byte & 0xC0u) != 0x80u ||
            (third_byte & 0xC0u) != 0x80u ||
            (fourth_byte & 0xC0u) != 0x80u) {
            return ATR_UTF8_REPLACEMENT_CHARACTER;
        }

        uint32_t code_point = ((uint32_t)(first_byte & 0x07u) << 18) |
                              ((uint32_t)(second_byte & 0x3Fu) << 12) |
                              ((uint32_t)(third_byte & 0x3Fu) << 6) |
                              ((uint32_t)(fourth_byte & 0x3Fu) << 0);

        /*
         * Reject:
         * - overlong sequences, such as F0 80 80 80 for U+0000
         * - code points above Unicode's U+10FFFF limit
         */
        if (code_point < 0x10000u || code_point > 0x10FFFFu) {
            return ATR_UTF8_REPLACEMENT_CHARACTER;
        }

        return code_point;
    }

    /*
     * Rejects:
     *
     * 80..BF: continuation bytes used as a leading byte
     * C0..C1: overlong two-byte forms
     * F5..FF: invalid in modern UTF-8
     */
    return ATR_UTF8_REPLACEMENT_CHARACTER;
}

ATR_DEF uint32_t atr_utf8_decode_next_code_point(uint8_t *text, size_t byte_count, size_t *consumed)
{
    ATR_ASSERT(text);
    if (consumed) *consumed = atr_bytes_for_utf8[*text]; 
    return atr_utf8_code_point_get_from_raw_char_bytes(atr_utf8_get_next_char_bytes(text, byte_count));
}

ATR_DEF uint32_t atr_utf8_get_next_char_bytes(uint8_t *str, size_t byte_count)
{
    uint32_t result = 0;

    if (str == NULL || byte_count == 0) {
        return 0;
    }
    if (str[0] == '\0') {
        return 0;
    }

    uint8_t char_byte_count = atr_bytes_for_utf8[(uint8_t)str[0]];
    ATR_ASSERT(char_byte_count <= byte_count && "'str' is not long enough to read full utf8 character.");
    
    for (size_t i = 0; i < char_byte_count; i++) {
        uint8_t b = str[i];
        result |= (uint32_t)b << (i * 8);
    }

    return result;
}

ATR_DEF bool atr_utf8_is_continuation_byte(uint8_t byte)
{
    return (byte & 0xC0) == 0x80;
}

ATR_DEF size_t atr_utf8_length(uint8_t *str, size_t byte_count)
{
    size_t i = 0;
    size_t count = 0;

    while (i < byte_count && str[i] != '\0') {
        uint8_t lead = str[i];
        if (lead >= atr_bytes_for_utf8_count) {
            i++;
            count++;
            continue;
        }

        size_t char_bytes = atr_bytes_for_utf8[lead];
        if (char_bytes == 0 || char_bytes > byte_count - i) {
            i++;
            count++;
            continue;
        }

        i += char_bytes;
        count++;
    }

    return count;
}


#endif /*ALMOG_TEXT_RENDERING_IMPLEMENTATION*/
