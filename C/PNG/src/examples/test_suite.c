#include <stdio.h>
#include <stdbool.h>

#define APL_SETUP
#define APL_INPUT
#define APL_UPDATE
#define APL_RENDER

#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#define ALMOG_PNG_IMPLEMENTATION

#define APL_ADL_BRIDGE_IMPLEMENTATION
#include "../include/APL_ADL_Bridge.h"

#define APL_APNG_BRIDGE_IMPLEMENTATION
#include "../include/APL_APNG_Bridge.h"


struct Apng_PNG_Image image = {0};
size_t image_index = 0;
char *file_name[] = {
    "../src/test_images/PngSuite/Basic-formats/basn0g01.png",
    "../src/test_images/PngSuite/Basic-formats/basn0g02.png",
    "../src/test_images/PngSuite/Basic-formats/basn0g04.png",
    "../src/test_images/PngSuite/Basic-formats/basn0g08.png",
    // "../src/test_images/PngSuite/Basic-formats/basn0g16.png", /* unsupported */
    "../src/test_images/PngSuite/Basic-formats/basn2c08.png",
    // "../src/test_images/PngSuite/Basic-formats/basn2c16.png", /* unsupported */
    // "../src/test_images/PngSuite/Basic-formats/basn3p01.png", /* unsupported */
    // "../src/test_images/PngSuite/Basic-formats/basn3p02.png", /* unsupported */
    // "../src/test_images/PngSuite/Basic-formats/basn3p04.png", /* unsupported */
    // "../src/test_images/PngSuite/Basic-formats/basn3p08.png", /* unsupported */
    "../src/test_images/PngSuite/Basic-formats/basn4a08.png",
    // "../src/test_images/PngSuite/Basic-formats/basn4a16.png", /* unsupported */
    "../src/test_images/PngSuite/Basic-formats/basn6a08.png",
    // "../src/test_images/PngSuite/Basic-formats/basn6a16.png", /* unsupported */
    "../src/test_images/PngSuite/Image-filtering/f00n0g08.png",
    "../src/test_images/PngSuite/Image-filtering/f00n2c08.png",
    "../src/test_images/PngSuite/Image-filtering/f01n0g08.png",
    "../src/test_images/PngSuite/Image-filtering/f01n2c08.png",
    "../src/test_images/PngSuite/Image-filtering/f02n0g08.png",
    "../src/test_images/PngSuite/Image-filtering/f02n2c08.png",
    "../src/test_images/PngSuite/Image-filtering/f03n0g08.png",
    "../src/test_images/PngSuite/Image-filtering/f03n2c08.png",
    "../src/test_images/PngSuite/Image-filtering/f04n0g08.png",
    "../src/test_images/PngSuite/Image-filtering/f04n2c08.png",
    "../src/test_images/PngSuite/Image-filtering/f99n0g04.png",
};
size_t num_of_images = sizeof(file_name) / sizeof(file_name[0]);
bool print_info = true;

enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
    ws->wanted_fps = 60;
    // ws->to_limit_fps = false;


    return APL_SUCCESS;
}

enum Apl_Return_Types apl_update(struct Apl_Window_State *ws)
{
    APL_UNUSED(ws);

    apng_png_free(&image);
    if (APNG_FAIL == apng_png_load_from_file_name(file_name[image_index], &image, print_info)) {
        return APL_FAIL;
    }
    print_info = false;

    return APL_SUCCESS;
}

apl_real zoom = 1;
apl_real off_x = 0;
apl_real off_y = 0;
enum Apl_Return_Types apl_render(struct Apl_Window_State *ws)
{
    apl_apng_apng_pixel_buffer_copy_to_apl_pixel_buffer(ws->window_pixels_mat, image.pixels, .offset_x = off_x, .offset_y = off_y, .zoom = zoom);

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_input(struct Apl_Window_State *ws)
{
    if (ws->buttons.e_is_pressed) {
        zoom *= 1.1f;
        ws->to_render = true;
    } else if (ws->buttons.q_is_pressed) {
        zoom /= 1.1f;
        ws->to_render = true;
    } else if (ws->buttons.r_is_pressed) {
        zoom = 1;
        off_x = 0;
        off_y = 0;
        ws->to_render = true;
    } else if (ws->buttons.d_is_pressed) {
        off_x -= 1 / zoom * ws->window_pixels_mat.cols / 100;
        ws->to_render = true;
    } else if (ws->buttons.a_is_pressed) {
        off_x += 1 / zoom * ws->window_pixels_mat.cols / 100;
        ws->to_render = true;
    } else if (ws->buttons.s_is_pressed) {
        off_y -= 1 / zoom * ws->window_pixels_mat.rows / 100;
        ws->to_render = true;
    } else if (ws->buttons.w_is_pressed) {
        off_y += 1 / zoom * ws->window_pixels_mat.rows / 100;
        ws->to_render = true;
    } else if (ws->buttons.up_is_pressed) {
        apl_sleep((size_t)100e3);
        if (image_index < num_of_images - 1) {
            image_index++;
            print_info = true;
            ws->to_render = true;
        }
    } else if (ws->buttons.down_is_pressed) {
        apl_sleep((size_t)100e3);
        if (image_index > 0) {
            image_index--;
            print_info = true;
            ws->to_render = true;
        }
    }
    

    return APL_SUCCESS;
}
