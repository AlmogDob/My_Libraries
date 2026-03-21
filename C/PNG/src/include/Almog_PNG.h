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
        uint32_t raw_type;
        char type_str[4];
    };
    enum Apng_Chunk_Type type;
};

#define APNG_CHUNK_FOOTER_SIZE 4
struct Apng_Chunk_Footer {
    size_t size;
    size_t index;
    uint32_t CRC;
};

struct Apng_Bin_String {
    char *name;
    size_t length;
    size_t cursor;
    uint8_t *content;
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
    struct Apng_Bin_String file;
    struct Apng_IHDR_Chunk IHDR_chunk;
    struct Apng_PLTE_Chunk PLTE_chunk;
    struct Apng_IDAT_Chunk IDAT_chunk;
    struct Apng_Pixel_Buffer pixels;
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
};

#define APNG_DEF static inline
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

#define APNG_PIXEL_MAT_AT(m, i, j) (m).elements[(APNG_ASSERT(i < m.rows && j < m.cols), (i) * (m).stride_r + (j))]

APNG_DEF struct Apng_Bin_String     apng_bin_file_read(char *file_name);
APNG_DEF void                       apng_bin_string_free(struct Apng_Bin_String bs);
APNG_DEF struct Apng_Chunk_Footer   apng_chunk_footer_get(struct Apng_Bin_String *bs);
APNG_DEF struct Apng_Chunk_Header   apng_chunk_header_get(struct Apng_Bin_String *bs);
APNG_DEF void *                     apng_consume(struct Apng_Bin_String *bs, size_t amount);
APNG_DEF struct Apng_PNG_Image      apng_decode_png(struct Apng_Bin_String file);
APNG_DEF uint32_t                   apng_endian_swap(uint32_t x);
APNG_DEF uint32_t                   apng_four_char_to_uint32_t(const char *str);
APNG_DEF struct Apng_Pixel_Buffer   apng_pixel_buffer_malloc(size_t rows, size_t cols);
APNG_DEF bool                       apng_png_header_signature_correct(struct Apng_PNG_Header h);
APNG_DEF struct Apng_PNG_Header     apng_png_header_get(struct Apng_Bin_String *bs);
APNG_DEF enum Apng_Chunk_Type       apng_type_get_from_raw_type(uint32_t raw_type);
APNG_DEF const char *               apng_type_name_get(enum Apng_Chunk_Type type);

/* chunk parsers */
APNG_DEF enum Apng_Return_Types     apng_IHDR_chunk_parse(struct Apng_IHDR_Chunk *chunk);
APNG_DEF enum Apng_Return_Types     apng_IDAT_chunk_parse(struct Apng_IDAT_Chunk *chunk);
APNG_DEF struct Apng_IDAT_Header    apng_IDAT_header_get_from_IDAT_chunk(struct Apng_IDAT_Chunk chunk);

#endif /*ALMOG_PNG_H_*/

#ifdef ALMOG_PNG_IMPLEMENTATION
#undef ALMOG_PNG_IMPLEMENTATION

APNG_DEF struct Apng_Bin_String apng_bin_file_read(char *file_name)
{
    struct Apng_Bin_String res = {0};
    
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
    res.content = APNG_MALLOC(res.length);
    if (res.content == NULL) {
        apng_dprintERROR( "Memory allocation failed for file %s (%zu bytes).\n", file_name, res.length);
        fclose(fp);
        res.length = 0;
        APNG_FREE(res.name);
        return res;
    }

    size_t nread = fread(res.content, 1, res.length, fp);
    if (nread != res.length) {
        if (ferror(fp)) {
            int err = errno;
            apng_dprintERROR( "Failed to read file %s: %s\n", file_name, strerror(err));
        } else {
            apng_dprintERROR(
                "Unexpected end of file while reading %s "
                "(expected %zu bytes, got %zu).\n", file_name, res.length, nread);
        }
        APNG_FREE(res.content);
        APNG_FREE(res.name);
        res.content = NULL;
        res.length = 0;
        fclose(fp);
        return res;
    }

    if (fclose(fp) != 0) {
        int err = errno;
        apng_dprintERROR( "Failed to close file %s: %s\n", file_name, strerror(err));
        APNG_FREE(res.content);
        APNG_FREE(res.name);
        res.content = NULL;
        res.length = 0;
        return res;
    }

    return res;
}

APNG_DEF void apng_bin_string_free(struct Apng_Bin_String bs)
{
    APNG_FREE(bs.content);
    APNG_FREE(bs.name);
}

APNG_DEF struct Apng_Chunk_Footer apng_chunk_footer_get(struct Apng_Bin_String *bs)
{
    size_t size = APNG_CHUNK_FOOTER_SIZE;
    struct Apng_Chunk_Footer footer = {
        .index = bs->cursor,
        .size = size,
    };
    uint32_t *CRC = (uint32_t *)(apng_consume(bs, 4));
    footer.CRC = apng_endian_swap(*CRC);
    
    return footer;
}

APNG_DEF struct Apng_Chunk_Header apng_chunk_header_get(struct Apng_Bin_String *bs)
{
    size_t size = APNG_CHUNK_HEADER_SIZE;
    struct Apng_Chunk_Header header = {
        .index = bs->cursor,
        .size = size,
    };
    uint32_t *length = (uint32_t *)(apng_consume(bs, APNG_CHUNK_HEADER_SIZE / 2));
    header.length = apng_endian_swap(*length);
    uint32_t *raw_type = (uint32_t *)(apng_consume(bs, APNG_CHUNK_HEADER_SIZE / 2));
    header.raw_type = *raw_type;
    header.type = apng_type_get_from_raw_type(header.raw_type);

    return header;
}

APNG_DEF void * apng_consume(struct Apng_Bin_String *bs, size_t amount_byte)
{
    APNG_ASSERT(amount_byte <= bs->length - bs->cursor);

    void *res = &(bs->content[bs->cursor]);
    bs->cursor += amount_byte;

    return res;
}

APNG_DEF struct Apng_PNG_Image apng_decode_png(struct Apng_Bin_String file)
{
    struct Apng_PNG_Image image = {.file = file};
    APNG_UNUSED(file);

    apng_dprintINFO("Decoding file: '%s'. File size: %zu bytes", image.file.name, image.file.length);

    struct Apng_PNG_Header png_header = apng_png_header_get(&image.file);
    APNG_ASSERT(apng_png_header_signature_correct(png_header));

    for ( ; image.file.cursor < image.file.length ; ) {
        struct Apng_Chunk_Header chunk_header = apng_chunk_header_get(&image.file);
        // printf("%.*s -> %d bytes\n%d\n", 4, chunk_header.type_str, chunk_header.length, chunk_header.type);
        void *chunk_data = apng_consume(&image.file, chunk_header.length);
        struct Apng_Chunk_Footer chunk_footer = apng_chunk_footer_get(&image.file);
        APNG_UNUSED(chunk_footer);

        switch (chunk_header.type) {
            case APNG_TYPE_IHDR: 
            {
                image.IHDR_chunk.index  = chunk_header.index + chunk_header.size;
                image.IHDR_chunk.length = chunk_header.length;
                image.IHDR_chunk.body   = chunk_data;
                apng_IHDR_chunk_parse(&image.IHDR_chunk);
            } break;
            case APNG_TYPE_sRGB: 
            {
                image.sRGB_chunk.index  = chunk_header.index + chunk_header.size;
                image.sRGB_chunk.length = chunk_header.length;
                image.sRGB_chunk.body   = chunk_data;
            } break;
            case APNG_TYPE_gAMA: 
            {
                image.gAMA_chunk.index  = chunk_header.index + chunk_header.size;
                image.gAMA_chunk.length = chunk_header.length;
                image.gAMA_chunk.body   = chunk_data;
            } break;
            case APNG_TYPE_pHYs: 
            {
                image.pHYs_chunk.index  = chunk_header.index + chunk_header.size;
                image.pHYs_chunk.length = chunk_header.length;
                image.pHYs_chunk.body   = chunk_data;
            } break;
            case APNG_TYPE_IDAT: 
            {
                image.IDAT_chunk.index  = chunk_header.index + chunk_header.size;
                image.IDAT_chunk.length = chunk_header.length;
                image.IDAT_chunk.body   = chunk_data;
                apng_IDAT_chunk_parse(&image.IDAT_chunk);
            } break; 
            case APNG_TYPE_IEND: 
            {
                image.IEND_chunk.index  = chunk_header.index + chunk_header.size;
                image.IEND_chunk.length = chunk_header.length;
                image.IEND_chunk.body   = chunk_data;
            } break;
            case APNG_TYPE_UNKNOWN:
            {
                apng_dprintERROR("%s", "Unknown chunk type.");
            } break;
            default:
            {
                apng_dprintERROR("Unsupported chunk type '%s'.", apng_type_name_get(chunk_header.type));
            } break;
        }
    }

    /* checking PNG file correctness */
    if (image.IEND_chunk.index != image.file.length - APNG_CHUNK_FOOTER_SIZE) {
        apng_dprintERROR("%s", "Error in IEND chunk.");
    }

    return image;
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

APNG_DEF struct Apng_PNG_Header apng_png_header_get(struct Apng_Bin_String *bs)
{
    size_t size = 8;
    struct Apng_PNG_Header header = {
        .index = bs->cursor,
        .size = size,
        .signature = (uint8_t *)apng_consume(bs, size),
    };
    
    return header;
}

APNG_DEF enum Apng_Chunk_Type apng_type_get_from_raw_type(uint32_t raw_type)
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
    APNG_ASSERT(chunk->body != NULL);
    APNG_ASSERT(chunk->index != 0);
    APNG_ASSERT(chunk->length != 0);

    chunk->header   = apng_IDAT_header_get_from_IDAT_chunk(*chunk);
    chunk->CM       = chunk->header.zlib_compression_method_flags & 0xF;
    chunk->CINFO    = chunk->header.zlib_compression_method_flags >> 4;
    chunk->FCHECK   = chunk->header.additional_flags & 0x1F;
    chunk->FDICT    = chunk->header.additional_flags >> 5;
    chunk->FLEVEL   = chunk->header.additional_flags >> 6;

    /* checks */
    apng_dprintINT(chunk->CM);
    apng_dprintINT(chunk->CINFO);
    apng_dprintINT(chunk->FCHECK);
    apng_dprintINT(chunk->FDICT);
    apng_dprintINT(chunk->FLEVEL);

    if (chunk->CM != 8) {
        apng_dprintERROR("PNG supports only CM = 8, got %d", chunk->CM);
        return APNG_FAIL;
    }
    if (chunk->CINFO > 7) {
        apng_dprintERROR("CINFO values above 7 are not allowed, got %d", chunk->CINFO);
        return APNG_FAIL;
    }

    return APNG_SUCCESS;
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
