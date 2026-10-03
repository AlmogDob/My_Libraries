// #define AMD_MEMORY_DEBUG
#define ALMOG_MEMORY_DEBUG_IMPLEMENTATION
#include "../include/Almog_Memory_Debug.h"

#define ALMOG_PNG_IMPLEMENTATION
#include "../include/Almog_PNG.h"


int main(void)
{
    char file_name[] = "../output.png";
    // char file_name[] = "../src/test_images/test-png_wiki.png";
    char code_file_name[] = "../build/output.exe";
    struct Apng_Byte_String code_chunk_data = {0};
    apng_ada_init_array(uint8_t, code_chunk_data);

    struct Apng_PNG_Image image = {0};
    if (APNG_FAIL == apng_png_load_from_file_name_get_cODE_chunk_data(file_name, &image, true, &code_chunk_data)) {
        apng_dprintERROR("Failed to load PNG from file '%s'.", file_name);
        return -1;
    }

    FILE *fp = fopen(code_file_name, "wb");
    if (fp == NULL) {
        apng_dprintERROR("Could not open file '%s'.", code_file_name);
        return -1;
    }
    size_t count = fwrite(code_chunk_data.elements, 1, code_chunk_data.length, fp);

    apng_png_free(&image);
    if (AMD_FAIL == amd_debug_mem()) {
        amd_dprintERROR("%s", "Corrupted memory detected.");
        return -1;
    }
    amd_debug_mem_print(0);

    return 0;
}
