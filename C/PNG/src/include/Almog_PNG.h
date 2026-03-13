/**
 * This PNG decoder/incoder is based on the PNG parser created in Handmade Hero, Casey Muratori's famous video series. 
 * A link to the video on YouTube: https://youtu.be/lkEWbIUEuN0
 * A link to the website: https://mollyrocket.com/#handmade
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

enum Apng_Chunk_Type {
    APNG_UNKNOWN,
    APNG_IHDR,
    APNG_PLTE,
    APNG_IDAT,
    APNG_IEND,
    APNG_cHRM,
    APNG_gAMA,
    APNG_iCCP,
    APNG_sBIT,
    APNG_sRGB,
    APNG_bKGD,
    APNG_hIST,
    APNG_tRNS,
    APNG_pHYs,
    APNG_sPLT,
    APNG_tIME,
    APNG_iTXt,
    APNG_tEXt,
    APNG_zTXt,
    APNG_TYPE_LENGTH
};

struct Apng_PNG_Header {
    size_t size;
    size_t index;
    uint8_t *signature;
};

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

struct Apng_PNG_Image {
    struct Apng_Bin_String file;
};

#define APNG_DEF static inline
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

APNG_DEF struct Apng_Bin_String     apng_bin_file_read(char *file_name);
APNG_DEF void                       apng_bin_string_free(struct Apng_Bin_String bs);
APNG_DEF struct Apng_Chunk_Footer   apng_chunk_footer_get(struct Apng_Bin_String *bs);
APNG_DEF struct Apng_Chunk_Header   apng_chunk_header_get(struct Apng_Bin_String *bs);
APNG_DEF void *                     apng_consume(struct Apng_Bin_String *bs, size_t amount);
APNG_DEF void                       apng_decode_png(struct Apng_Bin_String file);
APNG_DEF uint32_t                   apng_endian_swap(uint32_t x);
APNG_DEF uint32_t                   apng_four_char_to_uint32_t(const char *str);
APNG_DEF bool                       apng_png_header_signature_correct(struct Apng_PNG_Header h);
APNG_DEF struct Apng_PNG_Header     apng_png_header_get(struct Apng_Bin_String *bs);
APNG_DEF enum Apng_Chunk_Type       apng_type_get_from_raw_type(uint32_t raw_type);

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
    size_t size = 4;
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
    size_t size = 4 + 4;
    struct Apng_Chunk_Header header = {
        .index = bs->cursor,
        .size = size,
    };
    uint32_t *length = (uint32_t *)(apng_consume(bs, 4));
    header.length = apng_endian_swap(*length);
    uint32_t *raw_type = (uint32_t *)(apng_consume(bs, 4));
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


APNG_DEF void apng_decode_png(struct Apng_Bin_String file)
{
    struct Apng_PNG_Image image = {0};
    image.file = file;
    (void)file;

    printf("Decoding file: '%s'\nFile size: %zu bytes\n", image.file.name, image.file.length);

    struct Apng_PNG_Header png_header = apng_png_header_get(&image.file);
    APNG_ASSERT(apng_png_header_signature_correct(png_header));

    for ( ; image.file.cursor < image.file.length ; ) {
        struct Apng_Chunk_Header chunk_header = apng_chunk_header_get(&image.file);
        printf("%.*s -> %d bytes\n%d\n", 4, chunk_header.type_str, chunk_header.length, chunk_header.type);
        void *chunk_data = apng_consume(&image.file, chunk_header.length);
        struct Apng_Chunk_Footer chunk_footer = apng_chunk_footer_get(&image.file);
    }
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
        return APNG_IHDR;
    } else if (raw_type == apng_four_char_to_uint32_t("PLTE")) {
        return APNG_PLTE;
    } else if (raw_type == apng_four_char_to_uint32_t("IDAT")) {
        return APNG_IDAT;
    } else if (raw_type == apng_four_char_to_uint32_t("IEND")) {
        return APNG_IEND;
    } else if (raw_type == apng_four_char_to_uint32_t("cHRM")) {
        return APNG_cHRM;
    } else if (raw_type == apng_four_char_to_uint32_t("gAMA")) {
        return APNG_gAMA;
    } else if (raw_type == apng_four_char_to_uint32_t("iCCP")) {
        return APNG_iCCP;
    } else if (raw_type == apng_four_char_to_uint32_t("sBIT")) {
        return APNG_sBIT;
    } else if (raw_type == apng_four_char_to_uint32_t("sRGB")) {
        return APNG_sRGB;
    } else if (raw_type == apng_four_char_to_uint32_t("bKGD")) {
        return APNG_bKGD;
    } else if (raw_type == apng_four_char_to_uint32_t("hIST")) {
        return APNG_hIST;
    } else if (raw_type == apng_four_char_to_uint32_t("tRNS")) {
        return APNG_tRNS;
    } else if (raw_type == apng_four_char_to_uint32_t("pHYs")) {
        return APNG_pHYs;
    } else if (raw_type == apng_four_char_to_uint32_t("sPLT")) {
        return APNG_sPLT;
    } else if (raw_type == apng_four_char_to_uint32_t("tIME")) {
        return APNG_tIME;
    } else if (raw_type == apng_four_char_to_uint32_t("iTXt")) {
        return APNG_iTXt;
    } else if (raw_type == apng_four_char_to_uint32_t("tEXt")) {
        return APNG_tEXt;
    } else if (raw_type == apng_four_char_to_uint32_t("zTXt")) {
        return APNG_zTXt;
    } else {
        return APNG_UNKNOWN;
    }
}


#endif /*ALMOG_PNG_IMPLEMENTATION*/
