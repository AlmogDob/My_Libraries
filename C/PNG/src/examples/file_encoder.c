#include <string.h>

#define ALMOG_PNG_IMPLEMENTATION
#include "../include/Almog_PNG.h"

#define MAX_WORD_LEN 1500

int main(int argc, char const *argv[])
{
    if (argc != 4) {
        apng_dprintERROR("Usage: '%s' 'image file' 'output image file' 'data to encode'.", argv[0]);
        return -1;
    }
    char image_file_name[MAX_WORD_LEN];
    char output_file_name[MAX_WORD_LEN];
    char code_file_name[MAX_WORD_LEN];
    strncpy(image_file_name, argv[1], MAX_WORD_LEN);
    strncpy(output_file_name, argv[2], MAX_WORD_LEN);
    strncpy(code_file_name, argv[3], MAX_WORD_LEN);

    struct Apng_Byte_String code_chunk_data = apng_bin_file_read(code_file_name);

    struct Apng_PNG_Image image = {0};
    if (APNG_FAIL == apng_png_load_from_file_name(image_file_name, &image, true)) {
        apng_dprintERROR("Failed to load PNG from file '%s'.", image_file_name);
        return -1;
    }

    if (APNG_FAIL == apng_pixel_buffer_save_as_png_to_file_name(output_file_name, image.pixels, .print_info = true, .cODE_chunk_data = code_chunk_data)) {
        apng_dprintERROR("%s", "Failed to save PNG.");
        return -1;
    }

    apng_png_free(&image);

    return 0;
}
