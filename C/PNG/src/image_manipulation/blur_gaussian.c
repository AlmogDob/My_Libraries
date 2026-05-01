
#include <stdio.h>
#include <stdbool.h>

// #define MAT2D_SINGLE_PRECISION

#define APL_SETUP
#define APL_INPUT
#define APL_UPDATE
#define APL_RENDER
#define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#include "../include/Almog_Platform_Library.h"

#define ADL_ASSERT APL_ASSERT
#define MATRIX2D_IMPLEMENTATION
#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "../include/Almog_Draw_Library.h"

#define APNG_ASSERT APL_ASSERT
#define ALMOG_PNG_IMPLEMENTATION
#include "../include/Almog_PNG.h"

#define ALMOG_IMAGE_MANIPULATION_IMPLEMENTATION
#include "../include/Almog_Image_Manipulation.h"

Mat2D apl_depth_buffer_as_mat2d(struct Apl_Depth_Buffer b)
{
    Mat2D m = {
        .rows = b.rows,
        .cols = b.cols,
        .stride_r = b.stride_r,
        .elements = (mat2D_real *)b.elements,
    };
    return m;
}

Mat2D_uint32 apng_pixel_buffer_mat2d_u32(struct Apng_Pixel_Buffer b)
{
    Mat2D_uint32 m = {
        .rows = b.rows,
        .cols = b.cols,
        .stride_r = b.stride_r,
        .elements = b.elements,
    };
    return m;
}

Mat2D_uint32 apl_pixel_buffer_mat2d_u32(struct Apl_Pixel_Buffer b)
{
    Mat2D_uint32 m = {
        .rows = b.rows,
        .cols = b.cols,
        .stride_r = b.stride_r,
        .elements = b.elements,
    };
    return m;
}

Mat2D_uint32 results = {0};
struct Apng_PNG_Image image = {0};
Offset_zoom_param offzoom = {0};

enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
    ws->wanted_fps = 60;
    // ws->to_limit_fps = false;
    offzoom = ADL_DEFAULT_OFFSET_ZOOM;

    // char file_name[] = "../src/test_images/test-png7.png";
    char file_name[] = "../src/test_images/test-png_wiki.png";
    // char file_name[] = "../src/test_images/file_example_PNG_3MB.png";
    // char file_name[] = "../src/test_images/gaussian_blur_test.png";

    apng_png_free(&image);
    if (APNG_FAIL == apng_png_load(file_name, &image, true)) {
        return APL_FAIL;
    }

    Mat2D_uint32 image_pixels = apng_pixel_buffer_mat2d_u32(image.pixels);

    results = mat2D_alloc_uint32(image_pixels.rows, image_pixels.cols);
    // aim_blur_gaussian_bw(results, image_pixels, 6);
    // aim_blur_gaussian_bw_fast(results, image_pixels, 6);
    aim_blur_gaussian_rgba_fast(results, image_pixels, 6);

    // mat2D_copy_uint32(results, image_pixels);

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_update(struct Apl_Window_State *ws)
{
    APL_UNUSED(ws);


    return APL_SUCCESS;
}

double factor = 1;
enum Apl_Return_Types apl_render(struct Apl_Window_State *ws)
{
    Mat2D_uint32 window_pixels = apl_pixel_buffer_mat2d_u32(ws->window_pixels_mat);
    // Mat2D_uint32 pixels = apng_pixel_buffer_mat2d_u32(image.pixels);

    for (size_t i = 0; i < results.rows; i++) {
        for (size_t j = 0; j < results.cols; j++) {
            for (size_t u = 0; u < factor; u++) {
                for (size_t v = 0; v < factor; v++) {
                    adl_point_draw(window_pixels, (float)(j * factor + v), (float)(i * factor + u), MAT2D_AT(results, i, j), ADL_DEFAULT_OFFSET_ZOOM);
                }
            }
        }
    }

    if (ws->elapsed_time_micro_sec > 1) {
        ws->to_render = false;
    }

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_input(struct Apl_Window_State *ws)
{
    if (ws->mouse.left_button_is_pressed) {
        factor *= 1.1;
        // apl_dprintFLOAT(factor);
        ws->to_render = true;
    } else if (ws->mouse.right_button_is_pressed) {
        factor /= 1.1;
        // apl_dprintFLOAT(factor);
        ws->to_render = true;
    } else if (ws->buttons.r_is_pressed) {
        factor = 1;
        // apl_dprintFLOAT(factor);
        ws->to_render = true;
    }
    APL_UNUSED(ws);

    return APL_SUCCESS;
}
