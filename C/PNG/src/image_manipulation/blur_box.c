
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

void blur_box_blur_bw(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size)
{
    /* https://en.wikipedia.org/wiki/Box_blur */
    MAT2D_ASSERT(des_u32.cols == src_u32.cols);
    MAT2D_ASSERT(des_u32.rows == src_u32.rows);
    MAT2D_ASSERT(kernel_size > 2);
    MAT2D_ASSERT(kernel_size % 2);

    Mat2D src = mat2D_alloc(src_u32.rows + (kernel_size / 2) * 2, src_u32.cols + (kernel_size / 2) * 2);
    Mat2D conv = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D kernel = mat2D_alloc(kernel_size, kernel_size);

    for (size_t i = 0; i < src.rows; i++) {
        for (size_t j = 0; j < src.cols; j++) {
            if (i < kernel_size / 2 || i >= src.rows - kernel_size / 2 || j < kernel_size / 2|| j >= src.cols - kernel_size / 2) {
                MAT2D_AT(src, i, j) = 0;
            } else {
                uint32_t pixel = MAT2D_AT(src_u32, i - kernel_size / 2, j - kernel_size / 2);
                uint8_t r;
                uint8_t g;
                uint8_t b;
                APNG_HexARGB_TO_RGB_VAR(pixel, r, g, b);
                MAT2D_AT(src, i, j) = 0.2126 * r + 0.7152 * g + 0.0722 * b;
            }
        }
    }

    mat2D_fill(kernel, 1);
    mat2D_mult(kernel, 1 / mat2D_elements_sum(kernel));

    mat2D_convolve(conv, src, kernel);

    for (size_t i = 0; i < conv.rows; i++) {
        for (size_t j = 0; j < conv.cols; j++) {
            mat2D_real value = MAT2D_AT(conv, i, j);

            uint8_t temp, alpha;
            APNG_HexARGB_TO_RGBA_VAR(APNG_PIXEL_BUFFER_AT(src_u32, i, j), temp, temp, temp, alpha);

            MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, alpha);
        }
    }

    mat2D_free(src);
    mat2D_free(conv);
    mat2D_free(kernel);
}

void blur_box_blur_rgba(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size)
{
    /* https://en.wikipedia.org/wiki/Box_blur */

    MAT2D_ASSERT(des_u32.cols == src_u32.cols);
    MAT2D_ASSERT(des_u32.rows == src_u32.rows);
    MAT2D_ASSERT(kernel_size > 2);
    MAT2D_ASSERT(kernel_size % 2);

    Mat2D src_r = mat2D_alloc(src_u32.rows + (kernel_size / 2) * 2, src_u32.cols + (kernel_size / 2) * 2);
    Mat2D src_g = mat2D_alloc(src_u32.rows + (kernel_size / 2) * 2, src_u32.cols + (kernel_size / 2) * 2);
    Mat2D src_b = mat2D_alloc(src_u32.rows + (kernel_size / 2) * 2, src_u32.cols + (kernel_size / 2) * 2);
    Mat2D src_a = mat2D_alloc(src_u32.rows + (kernel_size / 2) * 2, src_u32.cols + (kernel_size / 2) * 2);
    Mat2D conv_r = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_g = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_b = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_a = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D kernel = mat2D_alloc(kernel_size, kernel_size);

    for (size_t i = 0; i < src_r.rows; i++) {
        for (size_t j = 0; j < src_r.cols; j++) {
            if (i < kernel_size / 2 || i >= src_r.rows - kernel_size / 2 || j < kernel_size / 2|| j >= src_r.cols - kernel_size / 2) {
                MAT2D_AT(src_r, i, j) = 0;
                MAT2D_AT(src_g, i, j) = 0;
                MAT2D_AT(src_b, i, j) = 0;
                MAT2D_AT(src_a, i, j) = 0;
            } else {
                uint32_t pixel = MAT2D_AT(src_u32, i - kernel_size / 2, j - kernel_size / 2);
                uint8_t r;
                uint8_t g;
                uint8_t b;
                uint8_t a;
                APNG_HexARGB_TO_RGBA_VAR(pixel, r, g, b, a);
                MAT2D_AT(src_r, i, j) = r * a / 255;
                MAT2D_AT(src_g, i, j) = g * a / 255;
                MAT2D_AT(src_b, i, j) = b * a / 255;
                MAT2D_AT(src_a, i, j) = a;
            }
        }
    }

    mat2D_fill(kernel, 1);
    mat2D_mult(kernel, 1 / mat2D_elements_sum(kernel));

    mat2D_convolve(conv_r, src_r, kernel);
    mat2D_convolve(conv_g, src_g, kernel);
    mat2D_convolve(conv_b, src_b, kernel);
    mat2D_convolve(conv_a, src_a, kernel);

    for (size_t i = 0; i < des_u32.rows; i++) {
        for (size_t j = 0; j < des_u32.cols; j++) {
            mat2D_real a = MAT2D_AT(conv_a, i, j);
            MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(MAT2D_AT(conv_r, i, j) / a * 255, MAT2D_AT(conv_g, i, j) / a * 255, MAT2D_AT(conv_b, i, j) / a * 255, a);
        }
    }

    mat2D_free(src_r);
    mat2D_free(src_g);
    mat2D_free(src_b);
    mat2D_free(src_a);
    mat2D_free(conv_r);
    mat2D_free(conv_g);
    mat2D_free(conv_b);
    mat2D_free(conv_a);
    mat2D_free(kernel);
}

Mat2D_uint32 results = {0};
struct Apng_PNG_Image image = {0};

enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
    ws->wanted_fps = 60;
    // ws->to_limit_fps = false;

    char file_name[] = "../src/test_images/test-png7.png";
    // char file_name[] = "../src/test_images/test-png3.png";
    // char file_name[] = "../src/test_images/file_example_PNG_3MB.png";

    apng_png_free(&image);
    if (APNG_FAIL == apng_png_load(file_name, &image, true)) {
        return APL_FAIL;
    }

    Mat2D_uint32 image_pixels = apng_pixel_buffer_mat2d_u32(image.pixels);

    results = mat2D_alloc_uint32(image_pixels.rows, image_pixels.cols);
    blur_box_blur_bw(results, image_pixels, 3);
    // blur_box_blur_rgba(results, image.pixels, 5);

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
