#define ALMOG_PNG_IMPLEMENTATION
#include "include/Almog_PNG.h"


int main(void)
{
    char file_name[] = "../src/test-png1.png";

    struct Apng_Byte_String file = apng_bin_file_read(file_name);
    struct Apng_PNG_Image image = {0};
    apng_png_decode(file, &image);


    apng_byte_string_free(file);

    return 0;
}
