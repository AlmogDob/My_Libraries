/**
 * This PNG decoder/incoder is based on the PNG parser created in Handmade Hero, Casey Muratori's famous video series. 
 * A link to the video on YouTube: https://youtu.be/lkEWbIUEuN0
 * A link to the website: https://mollyrocket.com/#handmade
 * 
 * You can find the PNG specification (second edition) in the link below:
 * https://www.w3.org/TR/2003/REC-PNG-20031110/
 */
#ifndef ALMOG_PNG_H_
#define ALMOG_PNG_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "include/Almog_Dynamic_Array.h"

#ifndef APNG_MALLOC
#include <stdlib.h>
#define APNG_MALLOC malloc
#endif

#ifndef APNG_FREE
#include <stdlib.h>
#define APNG_FREE free
#endif

#ifndef APNG_ASSERT
#include <assert.h>
#define APNG_ASSERT assert
#endif

enum Apng_Return_Types {
    APNG_SUCCESS,
    APNG_FAIL,
};
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
    uint8_t bits_used;
};

struct Apng_Huffman_Entrys_Table {
    size_t capacity;
    size_t length;
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
    void *body;   
};

#define APNG_IDAT_HEADER_SIZE 2
struct Apng_IDAT_Header {
    size_t size;
    size_t index;
    uint8_t zlib_compression_method_flags;
    uint8_t additional_flags;
};

struct Apng_IDAT_Chunk {
    size_t index;
    uint32_t length;
    uint8_t *body;   
    struct Apng_IDAT_Header header;
    uint8_t CM;
    uint8_t CINFO;
    uint8_t FCHECK;
    uint8_t FDICT;
    uint8_t FLEVEL;

    size_t LZ77_window_size;
};

#define APNG_IDAT_FOOTER_SIZE 4
struct Apng_IDAT_Footer {
    size_t size;
    size_t index;
    uint32_t check_value;
};

struct Apng_IEND_Chunk {
    size_t index;
    uint32_t length;
    void *body;   
};

struct Apng_cHRM_Chunk {
    size_t index;
    uint32_t length;
    void *body;   
};

struct Apng_gAMA_Chunk {
    size_t index;
    uint32_t length;
    void *body;   
};

struct Apng_iCCP_Chunk {
    size_t index;
    uint32_t length;
    void *body;   
};

struct Apng_sBIT_Chunk {
    size_t index;
    uint32_t length;
    void *body;   
};

struct Apng_sRGB_Chunk {
    size_t index;
    uint32_t length;
    void *body;   
};

struct Apng_bKGD_Chunk {
    size_t index;
    uint32_t length;
    void *body;   
};

struct Apng_hIST_Chunk {
    size_t index;
    uint32_t length;
    void *body;   
};

struct Apng_tRNS_Chunk {
    size_t index;
    uint32_t length;
    void *body;   
};

struct Apng_pHYs_Chunk {
    size_t index;
    uint32_t length;
    void *body;   
};

struct Apng_sPLT_Chunk {
    size_t index;
    uint32_t length;
    void *body;   
};

struct Apng_tIME_Chunk {
    size_t index;
    uint32_t length;
    void *body;   
};

struct Apng_iTXt_Chunk {
    size_t index;
    uint32_t length;
    void *body;   
};

struct Apng_tEXt_Chunk {
    size_t index;
    uint32_t length;
    void *body;   
};

struct Apng_zTXt_Chunk {
    size_t index;
    uint32_t length;
    void *body;   
};

struct Apng_PNG_Image {
    struct Apng_Byte_String file;
    struct Apng_Bit_Reader br;
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
        #define APNG_DEF static
    #else
        #define APNG_DEF extern
    #endif
#endif
#define APNG_UNUSED(x) ((void)x)

#define apng_dprintSTRING(expr) printf("[Info] %s:%d:\n" #expr " = %s\n", __FILE__, __LINE__, expr)
#define apng_dprintCHAR(expr) printf("[Info] %s:%d:\n" #expr " = %c\n", __FILE__, __LINE__, expr)
#define apng_dprintINT(expr) printf("[Info] %s:%d:\n" #expr " = %d\n", __FILE__, __LINE__, expr)
#define apng_dprintFLOAT(expr) printf("[Info] %s:%d:\n" #expr " = %#g\n", __FILE__, __LINE__, expr)
#define apng_dprintDOUBLE(expr) printf("[Info] %s:%d:\n" #expr " = %#g\n", __FILE__, __LINE__, expr)
#define apng_dprintSIZE_T(expr) printf("[Info] %s:%d:\n" #expr " = %zu\n", __FILE__, __LINE__, expr)
#define apng_dprintINFO(fmt, ...) \
    fprintf(stderr, "[Info] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 7, "", __func__, 7, "", __VA_ARGS__)
#define apng_dprintWARNING(fmt, ...) \
    fprintf(stderr, "[Warning] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 10, "", __func__, 10, "", __VA_ARGS__)
#define apng_dprintERROR(fmt, ...) \
    fprintf(stderr, "[Error] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 8, "", __func__, 8, "", __VA_ARGS__)

#define APNG_PIXEL_BUFFER_AT(m, i, j) (m).elements[(APNG_ASSERT(i < m.rows && j < m.cols), (i) * (m).stride_r + (j))]
#define APNG_STATIC_ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))
#define APNG_HLIT_OFFSET 257
#define APNG_HDIST_OFFSET 1
#define APNG_HCLEN_OFFSET 4
#define APNG_MAX_NUM_OF_CODE_LENGTH_CODE_LENGTH 19
#define APNG_CODE_LENGTH_CODE_LENGTH_LENGTH 3

APNG_DEF struct Apng_Byte_String    apng_bin_file_read(char *file_name);
APNG_DEF void                       apng_byte_string_free(struct Apng_Byte_String bs);
APNG_DEF void                       apng_bit_reader_flash(struct Apng_Bit_Reader *br);
APNG_DEF void                       apng_bit_reader_init(struct Apng_Bit_Reader *br, struct Apng_Byte_String file);
APNG_DEF uint8_t                    apng_bit_reader_read_bit(struct Apng_Bit_Reader *br);
APNG_DEF uint32_t                   apng_bit_reader_read_bits(struct Apng_Bit_Reader *br, size_t count);
APNG_DEF struct Apng_Chunk_Footer   apng_chunk_footer_get(struct Apng_Byte_String *bs);
APNG_DEF struct Apng_Chunk_Header   apng_chunk_header_get(struct Apng_Byte_String *bs);
APNG_DEF void *                     apng_consume_bytes(struct Apng_Byte_String *bs, size_t amount);
APNG_DEF enum Apng_Return_Types     apng_crc32_check(struct Apng_Chunk_Header header, void *chunk_data, struct Apng_Chunk_Footer footer);
APNG_DEF uint32_t                   apng_crc32_update(uint32_t crc, uint8_t *buf, size_t buf_len);
APNG_DEF uint32_t                   apng_endian_swap(uint32_t x);
APNG_DEF uint32_t                   apng_four_char_to_uint32_t(const char *str);
APNG_DEF struct Apng_Pixel_Buffer   apng_pixel_buffer_malloc(size_t rows, size_t cols);
APNG_DEF struct Apng_PNG_Image      apng_png_decode(struct Apng_Byte_String file);
APNG_DEF bool                       apng_png_header_signature_correct(struct Apng_PNG_Header h);
APNG_DEF struct Apng_PNG_Header     apng_png_header_get(struct Apng_Byte_String *bs);
APNG_DEF enum Apng_Chunk_Type       apng_type_get_from_type_raw(uint32_t raw_type);
APNG_DEF const char *               apng_type_name_get(enum Apng_Chunk_Type type);

/* chunk parsers */
APNG_DEF enum Apng_Return_Types     apng_IHDR_chunk_parse(struct Apng_IHDR_Chunk *chunk);
APNG_DEF enum Apng_Return_Types     apng_IDAT_chunk_parse(struct Apng_IDAT_Chunk *chunk);
APNG_DEF enum Apng_Return_Types     apng_IDAT_decompress(struct Apng_PNG_Image *image);
APNG_DEF struct Apng_IDAT_Header    apng_IDAT_header_get_from_IDAT_chunk(struct Apng_IDAT_Chunk chunk);

#endif /*ALMOG_PNG_H_*/

#ifdef ALMOG_PNG_IMPLEMENTATION
#undef ALMOG_PNG_IMPLEMENTATION

APNG_DEF struct Apng_Byte_String apng_bin_file_read(char *file_name)
{
    struct Apng_Byte_String res = {0};
    
    FILE *fp = fopen(file_name, "rb");
    if (fp == NULL) {
        int err = errno;
        apng_dprintERROR( "Cannot open file %s: %s\n", file_name, strerror(err));
        return res;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        int err = errno;
        apng_dprintERROR( "Failed to seek to end of file %s: %s\n", file_name, strerror(err));
        fclose(fp);
        return res;
    }

    long size = ftell(fp);
    if (size < 0) {
        int err = errno;
        apng_dprintERROR( "Failed to tell size of file %s: %s\n", file_name, strerror(err));
        fclose(fp);
        return res;
    }

    if (fseek(fp, 0, SEEK_SET) != 0) {
        int err = errno;
        apng_dprintERROR( "Failed to seek to start of file %s: %s\n", file_name, strerror(err));
        fclose(fp);
        return res;
    }

    res.name = APNG_MALLOC(sizeof(char) * (1 + strlen(file_name)));
    if (res.name == NULL) {
        apng_dprintERROR( "Memory allocation failed for file %s (%zu bytes).\n", file_name, strlen(file_name));
        fclose(fp);
        res.length = 0;
        return res;
    }
    strncpy(res.name, file_name, strlen(file_name)+1);

    res.length = (size_t)size;
    res.capacity = (size_t)size;
    res.elements = APNG_MALLOC(res.length);
    if (res.elements == NULL) {
        apng_dprintERROR( "Memory allocation failed for file %s (%zu bytes).\n", file_name, res.length);
        fclose(fp);
        res.length = 0;
        APNG_FREE(res.name);
        return res;
    }

    size_t nread = fread(res.elements, 1, res.length, fp);
    if (nread != res.length) {
        if (ferror(fp)) {
            int err = errno;
            apng_dprintERROR( "Failed to read file %s: %s\n", file_name, strerror(err));
        } else {
            apng_dprintERROR(
                "Unexpected end of file while reading %s "
                "(expected %zu bytes, got %zu).\n", file_name, res.length, nread);
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
        apng_dprintERROR( "Failed to close file %s: %s\n", file_name, strerror(err));
        APNG_FREE(res.elements);
        APNG_FREE(res.name);
        res.elements = NULL;
        res.length = 0;
        return res;
    }

    return res;
}

APNG_DEF void apng_byte_string_free(struct Apng_Byte_String bs)
{
    APNG_FREE(bs.elements);
    APNG_FREE(bs.name);
}

APNG_DEF void apng_bit_reader_flash(struct Apng_Bit_Reader *br)
{
    APNG_ASSERT(br->file.cursor < br->file.length);
    uint8_t c = br->file.elements[br->file.cursor++];
    br->current_byte = c;
    br->bits_left = 8;
}

APNG_DEF void apng_bit_reader_init(struct Apng_Bit_Reader *br, struct Apng_Byte_String file)
{
    br->file = file;
    br->current_byte = 0;
    br->bits_left = 0;
}

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

APNG_DEF uint32_t apng_bit_reader_read_bits(struct Apng_Bit_Reader *br, size_t count)
{
    APNG_ASSERT(count <= 32);

    uint32_t res = 0;

    for (size_t i = 0; i < count; i++) {
        #if 0
        res <<= 1;
        res |= apng_bit_reader_read_bit(br);
        #else
        res |= ((uint32_t)apng_bit_reader_read_bit(br)) << i;
        #endif

    }

    return res;
}

APNG_DEF struct Apng_Chunk_Footer apng_chunk_footer_get(struct Apng_Byte_String *bs)
{
    size_t size = APNG_CHUNK_FOOTER_SIZE;
    struct Apng_Chunk_Footer footer = {
        .index = bs->cursor,
        .size = size,
    };
    uint32_t *CRC = (uint32_t *)(apng_consume_bytes(bs, 4));
    footer.CRC = apng_endian_swap(*CRC);
    
    return footer;
}

APNG_DEF struct Apng_Chunk_Header apng_chunk_header_get(struct Apng_Byte_String *bs)
{
    size_t size = APNG_CHUNK_HEADER_SIZE;
    struct Apng_Chunk_Header header = {
        .index = bs->cursor,
        .size = size,
    };
    uint32_t *length = (uint32_t *)(apng_consume_bytes(bs, APNG_CHUNK_HEADER_SIZE / 2));
    header.length = apng_endian_swap(*length);
    uint32_t *raw_type = (uint32_t *)(apng_consume_bytes(bs, APNG_CHUNK_HEADER_SIZE / 2));
    header.type_raw = *raw_type;
    header.type = apng_type_get_from_type_raw(header.type_raw);

    return header;
}

APNG_DEF void * apng_consume_bytes(struct Apng_Byte_String *bs, size_t amount_byte)
{
    APNG_ASSERT(amount_byte <= bs->length - bs->cursor);

    void *res = &(bs->elements[bs->cursor]);
    bs->cursor += amount_byte;

    return res;
}

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

APNG_DEF uint32_t apng_endian_swap(uint32_t x)
{
    return ((x << 24) |
            ((x & 0xFF00) << 8) |
            ((x >> 8) & 0xFF00) |
            (x >> 24));
}

APNG_DEF uint32_t apng_four_char_to_uint32_t(const char * str)
{
    return ((uint32_t)(((uint32_t)((str)[0]) << 0) | ((uint32_t)((str)[1]) << 8) | ((uint32_t)((str)[2]) << 16) | ((uint32_t)((str)[3]) << 24)));
}

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

APNG_DEF struct Apng_PNG_Image apng_png_decode(struct Apng_Byte_String file)
{
    struct Apng_PNG_Image image = {.file = file};
    APNG_UNUSED(file);
    apng_bit_reader_init(&image.br, image.file);

    apng_dprintINFO("Decoding file: '%s'. File size: %zu bytes", image.file.name, image.file.length);

    struct Apng_PNG_Header png_header = apng_png_header_get(&image.file);
    APNG_ASSERT(apng_png_header_signature_correct(png_header));

    for ( ; image.file.cursor < image.file.length ; ) {
        struct Apng_Chunk_Header chunk_header = apng_chunk_header_get(&image.file);
        // printf("%.*s -> %d bytes\n%d\n", 4, chunk_header.type_str, chunk_header.length, chunk_header.type);
        void *chunk_data = apng_consume_bytes(&image.file, chunk_header.length);
        struct Apng_Chunk_Footer chunk_footer = apng_chunk_footer_get(&image.file);

        if (APNG_FAIL == apng_crc32_check(chunk_header, chunk_data, chunk_footer)) {
            apng_dprintERROR("Failed to decode PNG in file '%s'.", file.name);
            return image;
        }

        switch (chunk_header.type) {
            case APNG_TYPE_IHDR: 
            {
                image.chunks.IHDR_chunk.index  = chunk_header.index + chunk_header.size;
                image.chunks.IHDR_chunk.length = chunk_header.length;
                image.chunks.IHDR_chunk.body   = chunk_data;
                apng_IHDR_chunk_parse(&image.chunks.IHDR_chunk);
            } break;
            case APNG_TYPE_sRGB: 
            {
                image.chunks.sRGB_chunk.index  = chunk_header.index + chunk_header.size;
                image.chunks.sRGB_chunk.length = chunk_header.length;
                image.chunks.sRGB_chunk.body   = chunk_data;
            } break;
            case APNG_TYPE_gAMA: 
            {
                image.chunks.gAMA_chunk.index  = chunk_header.index + chunk_header.size;
                image.chunks.gAMA_chunk.length = chunk_header.length;
                image.chunks.gAMA_chunk.body   = chunk_data;
            } break;
            case APNG_TYPE_pHYs: 
            {
                image.chunks.pHYs_chunk.index  = chunk_header.index + chunk_header.size;
                image.chunks.pHYs_chunk.length = chunk_header.length;
                image.chunks.pHYs_chunk.body   = chunk_data;
            } break;
            case APNG_TYPE_IDAT: 
            {
                if (image.chunks.IDAT_chunk.body != NULL) {
                    apng_dprintERROR("%s", "Encountered more than one IDAT chunk. Currently not supported.");
                    return image;
                }
                image.chunks.IDAT_chunk.index  = chunk_header.index + chunk_header.size;
                image.chunks.IDAT_chunk.length = chunk_header.length;
                image.chunks.IDAT_chunk.body   = chunk_data;
                apng_IDAT_chunk_parse(&image.chunks.IDAT_chunk);

                #if 0
                printf("IDAT length: %u\n", image.chunks.IDAT_chunk.length);
                for (size_t i = 0;
                    i < 64 && i < image.chunks.IDAT_chunk.length;
                    i++) {
                    printf("%02X ", image.chunks.IDAT_chunk.body[i]);
                }
                printf("\n");
                #endif
            } break; 
            case APNG_TYPE_IEND: 
            {
                image.chunks.IEND_chunk.index  = chunk_header.index + chunk_header.size;
                image.chunks.IEND_chunk.length = chunk_header.length;
                image.chunks.IEND_chunk.body   = chunk_data;
            } break;
            case APNG_TYPE_UNKNOWN:
            {
                apng_dprintERROR("%s", "Unknown chunk type.");
                return image;
            } break;
            default:
            {
                apng_dprintERROR("Unsupported chunk type '%s'.", apng_type_name_get(chunk_header.type));
                return image;
            } break;
        }
    }

    /* checking PNG file correctness */
    if (image.chunks.IEND_chunk.index != image.file.length - APNG_CHUNK_FOOTER_SIZE) {
        apng_dprintERROR("%s", "Error in IEND chunk.");
    }

    /* decompressing the image */
    {
        enum Apng_Return_Types rt = apng_IDAT_decompress(&image);
        APNG_UNUSED(rt);
    }

    return image;
}

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

APNG_DEF enum Apng_Return_Types apng_IHDR_chunk_parse(struct Apng_IHDR_Chunk *chunk)
{
    APNG_ASSERT(chunk->body != NULL);
    APNG_ASSERT(chunk->index != 0);
    APNG_ASSERT(chunk->length != 0);

    uint32_t *width = (uint32_t *)&chunk->body[0];
    chunk->width = apng_endian_swap(*width);
    uint32_t *height = (uint32_t *)&chunk->body[4];
    chunk->height = apng_endian_swap(*height);
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
    
    if (chunk->color_type != 6) {
        apng_dprintERROR("Unsupported color type. Supports color type 6 but got %d", chunk->color_type);
        return APNG_FAIL;
    } else {
        if (chunk->bit_depth != 8) {
            apng_dprintERROR("Unsupported bit depth. Supports bit depth 8 but got %d", chunk->bit_depth);
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

APNG_DEF enum Apng_Return_Types apng_IDAT_chunk_parse(struct Apng_IDAT_Chunk *chunk)
{
    /*ZLIB specification: https://www.ietf.org/rfc/rfc1951.txt */

    APNG_ASSERT(chunk->body != NULL);
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

APNG_DEF enum Apng_Return_Types apng_IDAT_decompress(struct Apng_PNG_Image *image)
{
    /*ZLIB specification: https://www.ietf.org/rfc/rfc1951.txt */

    size_t width = image->chunks.IHDR_chunk.width;
    size_t height = image->chunks.IHDR_chunk.height;
    image->pixels = apng_pixel_buffer_malloc(height, width);

    struct Apng_Byte_String temp_bs = {0};
    ada_init_array(uint8_t, temp_bs);
    struct Apng_Huffman_Entrys_Table huffman_table = {0};
    ada_init_array(struct Apng_Huffman_Entry, huffman_table);

    struct Apng_IDAT_Chunk idat = image->chunks.IDAT_chunk;
    struct Apng_Bit_Reader *br  = &image->br;
    br->file.cursor = idat.index + APNG_IDAT_HEADER_SIZE;

    uint32_t BFINAL;
    uint32_t BTYPE;
    enum Apng_Return_Types rt = APNG_SUCCESS;
    do {
        /* read block header */
        BFINAL = apng_bit_reader_read_bits(br, 1);
        BTYPE  = apng_bit_reader_read_bits(br, 2);

        apng_dprintINT(BFINAL);
        apng_dprintINT(BTYPE);
        switch (BTYPE) {
            case 0:
            { 
                /* no compression */
                apng_bit_reader_flash(br);
                uint32_t LEN  = apng_bit_reader_read_bits(br, 16);
                uint32_t NLEN = apng_bit_reader_read_bits(br, 16);
                if (LEN != ~NLEN) {
                    apng_dprintERROR("%s", "LEN/NLEN mismatch.");
                    rt = APNG_FAIL;
                    goto apng_IDAT_decompress_end;
                }

                uint8_t *literal_data = apng_consume_bytes(&image->file, LEN);
                for (size_t i = 0; i < LEN; i++) {
                    ada_appand(uint8_t, temp_bs, literal_data[i]);
                }

            } break;
            case 1:
            {
                /* static Huffman codes */
            } break;
            case 2:
                /** dynamic Huffman codes
                 * There are 5 sections:
                 *  1. Code lengths for code lengths. (to generate the code length Huffman code)
                 *  2. Code lengths for the literal/length alphabet (encoded using the code length Huffman code)
                 *  3. Code lengths for the distance alphabet (encoded using the code length Huffman code)
                 *  4. The actual compressed data of the block. (encoded using the literal/length and distance Huffman codes)
                 *  5. The literal/length symbol 256 'end of data' (encoded using the literal/length Huffman code)
                */
                uint32_t HLIT  = apng_bit_reader_read_bits(br, 5) + APNG_HLIT_OFFSET;
                uint32_t HDIST = apng_bit_reader_read_bits(br, 5) + APNG_HDIST_OFFSET;
                uint32_t HCLEN = apng_bit_reader_read_bits(br, 4) + APNG_HCLEN_OFFSET;

                apng_dprintINT(HLIT);
                apng_dprintINT(HDIST);
                apng_dprintINT(HCLEN);

                uint32_t HCLEN_swizzle[] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
                uint32_t code_length_of_code_length[APNG_MAX_NUM_OF_CODE_LENGTH_CODE_LENGTH] = {0};
                for (size_t i = 0; i < HCLEN; i++) {
                    code_length_of_code_length[HCLEN_swizzle[i]] = apng_bit_reader_read_bits(br, APNG_CODE_LENGTH_CODE_LENGTH_LENGTH);
                }
                #if 0
                for (size_t i = 0; i < APNG_MAX_NUM_OF_CODE_LENGTH_CODE_LENGTH; i++) {
                    printf("index %2zu -> %u\n", i, code_length_of_code_length[i]);
                }
                #endif

                exit(1);

                // uint32_t lit_len_dist_table[512] = {0};
            {
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

apng_IDAT_decompress_end:
    APNG_FREE(temp_bs.elements);
    APNG_FREE(huffman_table.elements);
    return rt;
}

APNG_DEF struct Apng_IDAT_Header apng_IDAT_header_get_from_IDAT_chunk(struct Apng_IDAT_Chunk chunk)
{
    APNG_ASSERT(chunk.body != NULL);
    APNG_ASSERT(chunk.index != 0);
    APNG_ASSERT(chunk.length != 0);
    
    size_t size = APNG_IDAT_HEADER_SIZE;
    struct Apng_IDAT_Header header = {
        .index = chunk.index,
        .size = size,
    };

    header.zlib_compression_method_flags = chunk.body[0];
    header.additional_flags = chunk.body[1];

    return header;
}

#endif /*ALMOG_PNG_IMPLEMENTATION*/
