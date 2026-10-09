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
    "../src/test_images/PngSuite/PngSuite.png",

    "../src/test_images/PngSuite/Basic-formats/basn0g01.png",
    "../src/test_images/PngSuite/Basic-formats/basn0g02.png",
    "../src/test_images/PngSuite/Basic-formats/basn0g04.png",
    "../src/test_images/PngSuite/Basic-formats/basn0g08.png",
    "../src/test_images/PngSuite/Basic-formats/basn0g16.png",
    "../src/test_images/PngSuite/Basic-formats/basn2c08.png",
    "../src/test_images/PngSuite/Basic-formats/basn2c16.png",
    "../src/test_images/PngSuite/Basic-formats/basn3p01.png",
    "../src/test_images/PngSuite/Basic-formats/basn3p02.png",
    "../src/test_images/PngSuite/Basic-formats/basn3p04.png",
    "../src/test_images/PngSuite/Basic-formats/basn3p08.png",
    "../src/test_images/PngSuite/Basic-formats/basn4a08.png",
    "../src/test_images/PngSuite/Basic-formats/basn4a16.png",
    "../src/test_images/PngSuite/Basic-formats/basn6a08.png",
    "../src/test_images/PngSuite/Basic-formats/basn6a16.png",

    // "../src/test_images/PngSuite/Interlacing/basi0g01.png", /* not supported */
    // "../src/test_images/PngSuite/Interlacing/basi0g02.png", /* not supported */
    // "../src/test_images/PngSuite/Interlacing/basi0g04.png", /* not supported */
    // "../src/test_images/PngSuite/Interlacing/basi0g08.png", /* not supported */
    // "../src/test_images/PngSuite/Interlacing/basi0g16.png", /* not supported */
    // "../src/test_images/PngSuite/Interlacing/basi2c08.png", /* not supported */
    // "../src/test_images/PngSuite/Interlacing/basi2c16.png", /* not supported */
    // "../src/test_images/PngSuite/Interlacing/basi3p01.png", /* not supported */
    // "../src/test_images/PngSuite/Interlacing/basi3p02.png", /* not supported */
    // "../src/test_images/PngSuite/Interlacing/basi3p04.png", /* not supported */
    // "../src/test_images/PngSuite/Interlacing/basi3p08.png", /* not supported */
    // "../src/test_images/PngSuite/Interlacing/basi4a08.png", /* not supported */
    // "../src/test_images/PngSuite/Interlacing/basi4a16.png", /* not supported */
    // "../src/test_images/PngSuite/Interlacing/basi6a08.png", /* not supported */
    // "../src/test_images/PngSuite/Interlacing/basi6a16.png", /* not supported */

    "../src/test_images/PngSuite/Odd-Sizes/s01n3p01.png",
    "../src/test_images/PngSuite/Odd-Sizes/s02n3p01.png",
    "../src/test_images/PngSuite/Odd-Sizes/s03n3p01.png",
    "../src/test_images/PngSuite/Odd-Sizes/s04n3p01.png",
    "../src/test_images/PngSuite/Odd-Sizes/s05n3p02.png",
    "../src/test_images/PngSuite/Odd-Sizes/s06n3p02.png",
    "../src/test_images/PngSuite/Odd-Sizes/s07n3p02.png",
    "../src/test_images/PngSuite/Odd-Sizes/s08n3p02.png",
    "../src/test_images/PngSuite/Odd-Sizes/s09n3p02.png",
    "../src/test_images/PngSuite/Odd-Sizes/s32n3p04.png",
    "../src/test_images/PngSuite/Odd-Sizes/s33n3p04.png",
    "../src/test_images/PngSuite/Odd-Sizes/s34n3p04.png",
    "../src/test_images/PngSuite/Odd-Sizes/s35n3p04.png",
    "../src/test_images/PngSuite/Odd-Sizes/s36n3p04.png",
    "../src/test_images/PngSuite/Odd-Sizes/s37n3p04.png",
    "../src/test_images/PngSuite/Odd-Sizes/s38n3p04.png",
    "../src/test_images/PngSuite/Odd-Sizes/s39n3p04.png",
    "../src/test_images/PngSuite/Odd-Sizes/s40n3p04.png",
    // "../src/test_images/PngSuite/Odd-Sizes/s01i3p01.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s02i3p01.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s03i3p01.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s04i3p01.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s05i3p02.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s06i3p02.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s07i3p02.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s08i3p02.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s09i3p02.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s32i3p04.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s33i3p04.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s34i3p04.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s35i3p04.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s36i3p04.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s37i3p04.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s38i3p04.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s39i3p04.png", /* not supported */
    // "../src/test_images/PngSuite/Odd-Sizes/s40i3p04.png", /* not supported */

    // "../src/test_images/PngSuite/Background-Colors/bgai4a08.png", /* not supported */
    // "../src/test_images/PngSuite/Background-Colors/bgai4a16.png", /* not supported */
    "../src/test_images/PngSuite/Background-Colors/bgan6a08.png",
    "../src/test_images/PngSuite/Background-Colors/bgan6a16.png",
    "../src/test_images/PngSuite/Background-Colors/bgbn4a08.png",
    "../src/test_images/PngSuite/Background-Colors/bggn4a16.png",
    "../src/test_images/PngSuite/Background-Colors/bgwn6a08.png",
    "../src/test_images/PngSuite/Background-Colors/bgyn6a16.png",

    "../src/test_images/PngSuite/Transparency/tbbn0g04.png",
    "../src/test_images/PngSuite/Transparency/tbbn2c16.png",
    "../src/test_images/PngSuite/Transparency/tbbn3p08.png",
    "../src/test_images/PngSuite/Transparency/tbgn2c16.png",
    "../src/test_images/PngSuite/Transparency/tbgn3p08.png",
    "../src/test_images/PngSuite/Transparency/tbrn2c08.png",
    "../src/test_images/PngSuite/Transparency/tbwn0g16.png",
    "../src/test_images/PngSuite/Transparency/tbwn3p08.png",
    "../src/test_images/PngSuite/Transparency/tbyn3p08.png",
    "../src/test_images/PngSuite/Transparency/tp0n0g08.png",
    "../src/test_images/PngSuite/Transparency/tp0n2c08.png",
    "../src/test_images/PngSuite/Transparency/tp0n3p08.png",
    "../src/test_images/PngSuite/Transparency/tp1n3p08.png",
    "../src/test_images/PngSuite/Transparency/tm3n3p02.png",

    "../src/test_images/PngSuite/Gamma-Values/g03n0g16.png",
    "../src/test_images/PngSuite/Gamma-Values/g04n0g16.png",
    "../src/test_images/PngSuite/Gamma-Values/g05n0g16.png",
    "../src/test_images/PngSuite/Gamma-Values/g07n0g16.png",
    "../src/test_images/PngSuite/Gamma-Values/g10n0g16.png",
    "../src/test_images/PngSuite/Gamma-Values/g25n0g16.png",
    "../src/test_images/PngSuite/Gamma-Values/g03n2c08.png",
    "../src/test_images/PngSuite/Gamma-Values/g04n2c08.png",
    "../src/test_images/PngSuite/Gamma-Values/g05n2c08.png",
    "../src/test_images/PngSuite/Gamma-Values/g07n2c08.png",
    "../src/test_images/PngSuite/Gamma-Values/g10n2c08.png",
    "../src/test_images/PngSuite/Gamma-Values/g25n2c08.png",
    "../src/test_images/PngSuite/Gamma-Values/g03n3p04.png",
    "../src/test_images/PngSuite/Gamma-Values/g04n3p04.png",
    "../src/test_images/PngSuite/Gamma-Values/g05n3p04.png",
    "../src/test_images/PngSuite/Gamma-Values/g07n3p04.png",
    "../src/test_images/PngSuite/Gamma-Values/g10n3p04.png",
    "../src/test_images/PngSuite/Gamma-Values/g25n3p04.png",

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

    "../src/test_images/PngSuite/Additional-Palettes/pp0n2c16.png",
    "../src/test_images/PngSuite/Additional-Palettes/pp0n6a08.png",
    "../src/test_images/PngSuite/Additional-Palettes/ps1n0g08.png",
    "../src/test_images/PngSuite/Additional-Palettes/ps1n2c16.png",
    "../src/test_images/PngSuite/Additional-Palettes/ps2n0g08.png",
    "../src/test_images/PngSuite/Additional-Palettes/ps2n2c16.png",

    "../src/test_images/PngSuite/Ancillary-Chunks/ccwn2c08.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/ccwn3p08.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/cdfn2c08.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/cdhn2c08.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/cdsn2c08.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/cdun2c08.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/ch1n3p04.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/ch2n3p08.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/cm0n0g04.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/cm7n0g04.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/cm9n0g04.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/cs3n2c16.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/cs3n3p08.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/cs5n2c08.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/cs5n3p08.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/cs8n2c08.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/cs8n3p08.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/ct0n0g04.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/ct1n0g04.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/ctzn0g04.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/cten0g04.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/ctfn0g04.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/ctgn0g04.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/cthn0g04.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/ctjn0g04.png",
    "../src/test_images/PngSuite/Ancillary-Chunks/exif2c08.png",

    "../src/test_images/PngSuite/Chunk-Ordering/oi1n0g16.png",
    "../src/test_images/PngSuite/Chunk-Ordering/oi1n2c16.png",
    "../src/test_images/PngSuite/Chunk-Ordering/oi2n0g16.png",
    "../src/test_images/PngSuite/Chunk-Ordering/oi2n2c16.png",
    "../src/test_images/PngSuite/Chunk-Ordering/oi4n0g16.png",
    "../src/test_images/PngSuite/Chunk-Ordering/oi4n2c16.png",
    "../src/test_images/PngSuite/Chunk-Ordering/oi9n0g16.png",
    "../src/test_images/PngSuite/Chunk-Ordering/oi9n2c16.png",

    "../src/test_images/PngSuite/Zlib-Compression/z00n2c08.png",
    "../src/test_images/PngSuite/Zlib-Compression/z03n2c08.png",
    "../src/test_images/PngSuite/Zlib-Compression/z06n2c08.png",
    "../src/test_images/PngSuite/Zlib-Compression/z09n2c08.png",

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
