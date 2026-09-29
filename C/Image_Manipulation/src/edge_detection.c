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

struct Apng_Pixel_Buffer mat2d_u32_as_apng_pixel_buffer(Mat2D_uint32 m)
{
    struct Apng_Pixel_Buffer b = {
        .rows = m.rows,
        .cols = m.cols,
        .stride_r = m.stride_r,
        .elements = m.elements,
    };
    return b;
}

Mat2D_uint32 apng_pixel_buffer_as_mat2d_u32(struct Apng_Pixel_Buffer b)
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

    // char file_name[] = "../src/test_images/test-png3.png";
    char file_name[] = "../src/test_images/Bikesgray.png";
    // char file_name[] = "../src/test_images/file_example_PNG_3MB.png";
    // char file_name[] = "../src/test_images/Valve_original.png";

    apng_png_free(&image);
    if (APNG_FAIL == apng_png_load(file_name, &image, true)) {
        return APL_FAIL;
    }

    Mat2D_uint32 image_pixels = apng_pixel_buffer_as_mat2d_u32(image.pixels);

    results = mat2D_alloc_uint32(image_pixels.rows, image_pixels.cols);
    Mat2D_uint32 temp = mat2D_alloc_uint32(image_pixels.rows, image_pixels.cols);

    // aim_edge_detection_scharr_3x3(results, image_pixels);
    // aim_edge_detection_sobel_general(results, image_pixels, 3);
    // aim_edge_detection_sobel_general_cutoff(results, image_pixels, 3, 220);
    // aim_edge_detection_sobel_3x3(results, image_pixels);
    // aim_edge_detection_sobel_3x3_cutoff(results, image_pixels, 200);
    // aim_edge_detection_sobel_5x5(results, image_pixels);
    // aim_edge_detection_sobel_5x5_cutoff(results, image_pixels, 100);


    aim_blur_gaussian_bw_fast(temp, image_pixels, 1.0);
    aim_edge_detection_sobel_general_cutoff(results, temp, 5, 150);
    // // aim_edge_detection_sobel_3x3(results, temp_b);
    // // aim_edge_detection_sobel_3x3_cutoff(results, temp, 200);
    // // aim_edge_detection_sobel_5x5(results, temp);
    // aim_edge_detection_sobel_5x5_cutoff(results, temp, 200);

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
    // Mat2D_uint32 pixels = apng_pixel_buffer_as_mat2d_u32(image.pixels);

    for (size_t i = 0; i < results.rows; i++) {
        for (size_t j = 0; j < results.cols; j++) {
            adl_point_draw(window_pixels, (float)(j), (float)(i), MAT2D_AT(results, i, j), offzoom);
        }
    }

    if (ws->elapsed_time_micro_sec > 1) {
        ws->to_render = false;
    }

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_input(struct Apl_Window_State *ws)
{
    if (ws->buttons.e_is_pressed) {
        offzoom.zoom_multiplier *= 1.1;
        // apl_dprintFLOAT(factor);
        ws->to_render = true;
    } else if (ws->buttons.q_is_pressed) {
        offzoom.zoom_multiplier /= 1.1;
        // apl_dprintFLOAT(factor);
        ws->to_render = true;
    } else if (ws->buttons.r_is_pressed) {
        offzoom = ADL_DEFAULT_OFFSET_ZOOM;
        // apl_dprintFLOAT(factor);
        ws->to_render = true;
    } else if (ws->buttons.d_is_pressed) {
        offzoom.offset_x -= results.cols / 100;
        ws->to_render = true;
    } else if (ws->buttons.a_is_pressed) {
        offzoom.offset_x += results.cols / 100;
        ws->to_render = true;
    } else if (ws->buttons.s_is_pressed) {
        offzoom.offset_y -= results.rows / 100;
        ws->to_render = true;
    } else if (ws->buttons.w_is_pressed) {
        offzoom.offset_y += results.rows / 100;
        ws->to_render = true;
    }
    APL_UNUSED(ws);

    return APL_SUCCESS;
}
