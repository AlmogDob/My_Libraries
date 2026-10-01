// #define AMD_MEMORY_DEBUG
#define ALMOG_MEMORY_DEBUG_IMPLEMENTATION
#include "../include/Almog_Memory_Debug.h"

#define ALMOG_PNG_IMPLEMENTATION
#include "../include/Almog_PNG.h"


int main(void)
{
    char file_name[] = "../src/test_images/file_example_PNG_3MB.png";

    struct Apng_PNG_Image image = {0};
    apng_png_load_from_file_name(file_name, &image, true);

    apng_pixel_buffer_save_as_png_to_file_name("../output_8bit.png", image.pixels,
                                               .colour_type = APNG_COLOUR_TYPE_GREYSCALE, 
                                               .bit_depth = 8);
    apng_pixel_buffer_save_as_png_to_file_name("../output_4bit.png", image.pixels,
                                               .colour_type = APNG_COLOUR_TYPE_GREYSCALE, 
                                               .bit_depth = 4);
    apng_pixel_buffer_save_as_png_to_file_name("../output_2bit.png", image.pixels,
                                               .colour_type = APNG_COLOUR_TYPE_GREYSCALE, 
                                               .bit_depth = 2);
    apng_pixel_buffer_save_as_png_to_file_name("../output_1bit.png", image.pixels,
                                               .colour_type = APNG_COLOUR_TYPE_GREYSCALE, 
                                               .bit_depth = 1);

    apng_png_free(&image);
    if (AMD_FAIL == amd_debug_mem()) {
        amd_dprintERROR("%s", "Corrupted memory detected.");
        return -1;
    }
    // amd_debug_mem_print(0);

    return 0;
}
