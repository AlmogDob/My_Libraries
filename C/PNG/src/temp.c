#define ALMOG_PNG_IMPLEMENTATION
#include "include/Almog_PNG.h"


int main(void)
{
    char file_name[] = "../src/test_images/test-png1.png";

    struct Apng_PNG_Image image = {0};
    apng_png_load(file_name, &image, true);


    return 0;
}
