#define ALMOG_PNG_IMPLEMENTATION
#include "include/Almog_PNG.h"


int main(void)
{
    char file_name[] = "../src/test-png.png";
    printf("Loading PNG '%s'.\n", file_name);

    struct Apng_Bin_String file = apng_bin_file_read(file_name);
    apng_decode_png(file);


    apng_bin_string_free(file);

    return 0;
}
