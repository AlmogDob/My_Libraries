// #define AMD_MEMORY_DEBUG
#define ALMOG_MEMORY_DEBUG_IMPLEMENTATION
#include "../include/Almog_Memory_Debug.h"

#define ALMOG_PNG_IMPLEMENTATION
#include "../include/Almog_PNG.h"


int main(void)
{
    // char file_name[] = "../src/test_images/test-png_wiki.png";
    char file_name[] = "../src/test_images/PngSuite/Corrupted-files/xhdn0g08.png";

    struct Apng_PNG_Image image = {0};
    if (APNG_FAIL == apng_png_load_from_file_name(file_name, &image, true)) {
        return -1;
    }

    apng_pixel_buffer_save_as_png_to_file_name("../output_images/output_default.png", image.pixels, .print_info = true);
    apng_pixel_buffer_save_as_png_to_file_name("../output_images/output_bw16bit.png", image.pixels,
                                               .colour_type = APNG_COLOUR_TYPE_GREYSCALE, 
                                               .bit_depth = 16, .print_info = true);
    apng_pixel_buffer_save_as_png_to_file_name("../output_images/output_bw8bit.png", image.pixels,
                                               .colour_type = APNG_COLOUR_TYPE_GREYSCALE, 
                                               .bit_depth = 8, .print_info = true);
    apng_pixel_buffer_save_as_png_to_file_name("../output_images/output_bw4bit.png", image.pixels,
                                               .colour_type = APNG_COLOUR_TYPE_GREYSCALE, 
                                               .bit_depth = 4, .print_info = true);
    apng_pixel_buffer_save_as_png_to_file_name("../output_images/output_bw2bit.png", image.pixels,
                                               .colour_type = APNG_COLOUR_TYPE_GREYSCALE, 
                                               .bit_depth = 2, .print_info = true);
    apng_pixel_buffer_save_as_png_to_file_name("../output_images/output_bw1bit.png", image.pixels,
                                               .colour_type = APNG_COLOUR_TYPE_GREYSCALE, 
                                               .bit_depth = 1, .print_info = true);
    apng_pixel_buffer_save_as_png_to_file_name("../output_images/output_bwa16bit.png", image.pixels,
                                               .colour_type = APNG_COLOUR_TYPE_GREYSCALE_WITH_ALPHA, 
                                               .bit_depth = 16, .print_info = true);
    apng_pixel_buffer_save_as_png_to_file_name("../output_images/output_bwa8bit.png", image.pixels,
                                               .colour_type = APNG_COLOUR_TYPE_GREYSCALE_WITH_ALPHA, 
                                               .bit_depth = 8, .print_info = true);
    apng_pixel_buffer_save_as_png_to_file_name("../output_images/output_rgb16bit.png", image.pixels,
                                               .colour_type = APNG_COLOUR_TYPE_TRUECOLOUR, 
                                               .bit_depth = 16, .print_info = true);
    apng_pixel_buffer_save_as_png_to_file_name("../output_images/output_rgb8bit.png", image.pixels,
                                               .colour_type = APNG_COLOUR_TYPE_TRUECOLOUR, 
                                               .bit_depth = 8, .print_info = true);
    apng_pixel_buffer_save_as_png_to_file_name("../output_images/output_rgba16bit.png", image.pixels,
                                               .colour_type = APNG_COLOUR_TYPE_TRUECOLOUR_WITH_ALPHA, 
                                               .bit_depth = 16, .print_info = true);
    apng_pixel_buffer_save_as_png_to_file_name("../output_images/output_rgba8bit.png", image.pixels,
                                               .colour_type = APNG_COLOUR_TYPE_TRUECOLOUR_WITH_ALPHA, 
                                               .bit_depth = 8, .print_info = true);


    apng_png_free(&image);
    if (AMD_FAIL == amd_debug_mem()) {
        amd_dprintERROR("%s", "Corrupted memory detected.");
        return -1;
    }
    // amd_debug_mem_print(0);

    return 0;
}
