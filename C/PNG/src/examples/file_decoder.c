#include <string.h>

#define ALMOG_PNG_IMPLEMENTATION
#include "../include/Almog_PNG.h"

#define MAX_WORD_LEN 1500

int main(int argc, char const *argv[])
{
    if (argc != 3) {
        apng_dprintERROR("Usage: '%s' 'image file to decode' 'output file for data'.", argv[0]);
        return -1;
    }
    char image_file_name[MAX_WORD_LEN];
    char output_file_name[MAX_WORD_LEN];
    strncpy(image_file_name, argv[1], MAX_WORD_LEN);
    strncpy(output_file_name, argv[2], MAX_WORD_LEN);

    struct Apng_Byte_String code_chunk_data = {0};
    apng_ada_init_array(uint8_t, code_chunk_data);

    struct Apng_PNG_Image image = {0};
    if (APNG_FAIL == apng_png_load_from_file_name(image_file_name, &image, .print_info = true, .cODE_chunk_data = &code_chunk_data)) {
        apng_dprintERROR("Failed to load PNG from file '%s'.", image_file_name);
        return -1;
    }

    FILE *fp = fopen(output_file_name, "wb");
    if (fp == NULL) {
        apng_dprintERROR("Could not open file '%s'.", output_file_name);
        return -1;
    }
    (void)fwrite(code_chunk_data.elements, 1, code_chunk_data.length, fp);

    apng_png_free(&image);
    fclose(fp);

    return 0;
}
