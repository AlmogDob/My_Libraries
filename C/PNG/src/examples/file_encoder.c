// #define AMD_MEMORY_DEBUG
#define ALMOG_MEMORY_DEBUG_IMPLEMENTATION
#include "../include/Almog_Memory_Debug.h"

#define ALMOG_PNG_IMPLEMENTATION
#include "../include/Almog_PNG.h"


int main(void)
{
    // char file_name[] = "../output.png";
    char file_name[] = "../src/test_images/test-png_wiki.png";
    char code_file_name[] = "../build/test_suite.exe";
    struct Apng_Byte_String code_chunk_data = apng_bin_file_read(code_file_name);

    struct Apng_PNG_Image image = {0};
    if (APNG_FAIL == apng_png_load_from_file_name(file_name, &image, true)) {
        apng_dprintERROR("Failed to load PNG from file '%s'.", file_name);
        return -1;
    }

    if (APNG_FAIL == apng_pixel_buffer_save_as_png_to_file_name("../output.png", image.pixels, .colour_type = APNG_COLOUR_TYPE_GREYSCALE, .print_info = true, .cODE_chunk_data = code_chunk_data)) {
        apng_dprintERROR("%s", "Failed to save PNG.");
        return -1;
    }

    apng_png_free(&image);
    if (AMD_FAIL == amd_debug_mem()) {
        amd_dprintERROR("%s", "Corrupted memory detected.");
        return -1;
    }
    amd_debug_mem_print(0);

    return 0;
}
