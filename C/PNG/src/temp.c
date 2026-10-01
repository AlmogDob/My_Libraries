// #define AMD_MEMORY_DEBUG
#define ALMOG_MEMORY_DEBUG_IMPLEMENTATION
#include "include/Almog_Memory_Debug.h"

#define ALMOG_PNG_IMPLEMENTATION
#include "include/Almog_PNG.h"


int main(void)
{
    // char file_name[] = "../src/test_images/test-png1.png";
    char file_name[] = "../src/test_images/test-png_wiki.png";

    struct Apng_PNG_Image image = {0};
    apng_png_load_from_file_name(file_name, &image, true);

    _apng_pixel_buffer_save_as_png_to_file_name(("../output.png"), (image.pixels), (struct Apng_Pixel_Buffer_Save_As_PNG_Opt){.colour_type = APNG_COLOUR_TYPE_GREYSCALE});

    apng_png_free(&image);
    if (AMD_FAIL == amd_debug_mem()) {
        amd_dprintERROR("%s", "Corrupted memory detected.");
        return -1;
    }
    // amd_debug_mem_print(0);

    return 0;
}
