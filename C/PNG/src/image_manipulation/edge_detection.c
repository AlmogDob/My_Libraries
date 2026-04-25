#include <stdio.h>
#include <stdbool.h>

#define MAT2D_SINGLE_PRECISION

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

void edge_detection5x5(Mat2D_uint32 des_u32, struct Apng_Pixel_Buffer src_u32)
{
    /* https://en.wikipedia.org/wiki/Sobel_operator */

    MAT2D_ASSERT(des_u32.cols == src_u32.cols);
    MAT2D_ASSERT(des_u32.rows == src_u32.rows);

    Mat2D src = mat2D_alloc(src_u32.rows + 4, src_u32.cols + 4);
    Mat2D des = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_hori = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_vert = mat2D_alloc(src_u32.rows, src_u32.cols);


    for (size_t i = 0; i < src.rows; i++) {
        for (size_t j = 0; j < src.cols; j++) {
            if (i == 0 || i == src.rows - 1 || j == 0 || j == src.cols - 1) {
                MAT2D_AT(src, i, j) = 0;
            } else if (i == 1 || i == src.rows - 2 || j == 1 || j == src.cols - 2) {
                MAT2D_AT(src, i, j) = 0;
            } else {
                uint32_t pixel = APNG_PIXEL_BUFFER_AT(src_u32, i - 2, j - 2);
                uint8_t r;
                uint8_t g;
                uint8_t b;
                uint8_t a;
                APNG_HexARGB_TO_RGBA_VAR(pixel, r, g, b, a);
                MAT2D_AT(src, i, j) = 0.299 * r + 0.587 * g + 0.114 * b;
                // MAT2D_AT(src, i, j) = (MAT2D_AT(src, i, j) + a) / 2;
            }
        }
    }

    Mat2D kernal_hori = mat2D_alloc(5, 5);
    MAT2D_AT(kernal_hori, 0, 0) =  2; MAT2D_AT(kernal_hori, 0, 1) =  1; MAT2D_AT(kernal_hori, 0, 2) =  0; MAT2D_AT(kernal_hori, 0, 3) = -1; MAT2D_AT(kernal_hori, 0, 4) = -2;
    MAT2D_AT(kernal_hori, 1, 0) =  3; MAT2D_AT(kernal_hori, 1, 1) =  2; MAT2D_AT(kernal_hori, 1, 2) =  0; MAT2D_AT(kernal_hori, 1, 3) = -2; MAT2D_AT(kernal_hori, 1, 4) = -3;
    MAT2D_AT(kernal_hori, 2, 0) =  4; MAT2D_AT(kernal_hori, 2, 1) =  3; MAT2D_AT(kernal_hori, 2, 2) =  0; MAT2D_AT(kernal_hori, 2, 3) = -3; MAT2D_AT(kernal_hori, 2, 4) = -4;
    MAT2D_AT(kernal_hori, 3, 0) =  3; MAT2D_AT(kernal_hori, 3, 1) =  2; MAT2D_AT(kernal_hori, 3, 2) =  0; MAT2D_AT(kernal_hori, 3, 3) = -2; MAT2D_AT(kernal_hori, 3, 4) = -3;
    MAT2D_AT(kernal_hori, 4, 0) =  2; MAT2D_AT(kernal_hori, 4, 1) =  1; MAT2D_AT(kernal_hori, 4, 2) =  0; MAT2D_AT(kernal_hori, 4, 3) = -1; MAT2D_AT(kernal_hori, 4, 4) = -2;
    Mat2D kernal_vert = mat2D_alloc(5, 5);
    MAT2D_AT(kernal_vert, 0, 0) =  2; MAT2D_AT(kernal_vert, 0, 1) =  3; MAT2D_AT(kernal_vert, 0, 2) =  4; MAT2D_AT(kernal_vert, 0, 3) =  3; MAT2D_AT(kernal_vert, 0, 4) =  2;
    MAT2D_AT(kernal_vert, 1, 0) =  1; MAT2D_AT(kernal_vert, 1, 1) =  2; MAT2D_AT(kernal_vert, 1, 2) =  3; MAT2D_AT(kernal_vert, 1, 3) =  2; MAT2D_AT(kernal_vert, 1, 4) =  1;
    MAT2D_AT(kernal_vert, 2, 0) =  0; MAT2D_AT(kernal_vert, 2, 1) =  0; MAT2D_AT(kernal_vert, 2, 2) =  0; MAT2D_AT(kernal_vert, 2, 3) =  0; MAT2D_AT(kernal_vert, 2, 4) =  0;
    MAT2D_AT(kernal_vert, 3, 0) = -1; MAT2D_AT(kernal_vert, 3, 1) = -2; MAT2D_AT(kernal_vert, 3, 2) = -3; MAT2D_AT(kernal_vert, 3, 3) = -2; MAT2D_AT(kernal_vert, 3, 4) = -1;
    MAT2D_AT(kernal_vert, 4, 0) = -2; MAT2D_AT(kernal_vert, 4, 1) = -3; MAT2D_AT(kernal_vert, 4, 2) = -4; MAT2D_AT(kernal_vert, 4, 3) = -3; MAT2D_AT(kernal_vert, 4, 4) = -2;

    mat2D_rotate_mat_180_deg_inplace(kernal_hori);
    mat2D_rotate_mat_180_deg_inplace(kernal_vert);

    mat2D_convolve(conv_hori, src, kernal_hori);
    mat2D_convolve(conv_vert, src, kernal_vert);

    for (size_t i = 0; i < des.rows; i++) {
        for (size_t j = 0; j < des.cols; j++) {
            MAT2D_AT(des, i, j) = hypot(MAT2D_AT(conv_hori, i, j), MAT2D_AT(conv_vert, i, j));
        }
    }

    mat2D_normalize_inf(des);
    mat2D_mult(des, 255);

    for (size_t i = 0; i < des.rows; i++) {
        for (size_t j = 0; j < des.cols; j++) {
            mat2D_real value = MAT2D_AT(des, i, j);

            uint8_t temp, alpha;
            APNG_HexARGB_TO_RGBA_VAR(APNG_PIXEL_BUFFER_AT(src_u32, i, j), temp, temp, temp, alpha);

            MAT2D_AT_UINT32(des_u32, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, alpha);
        }
    }

    mat2D_free(src);
    mat2D_free(conv_hori);
    mat2D_free(conv_vert);
}

void edge_detection3x3(Mat2D_uint32 des_u32, struct Apng_Pixel_Buffer src_u32)
{
    /* https://en.wikipedia.org/wiki/Sobel_operator */

    MAT2D_ASSERT(des_u32.cols == src_u32.cols);
    MAT2D_ASSERT(des_u32.rows == src_u32.rows);

    Mat2D src = mat2D_alloc(src_u32.rows + 2, src_u32.cols + 2);
    Mat2D des = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_hori = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_vert = mat2D_alloc(src_u32.rows, src_u32.cols);


    for (size_t i = 0; i < src.rows; i++) {
        for (size_t j = 0; j < src.cols; j++) {
            if (i == 0 || i == src.rows - 1 || j == 0 || j == src.cols - 1) {
                MAT2D_AT(src, i, j) = 0;
            } else {
                uint32_t pixel = APNG_PIXEL_BUFFER_AT(src_u32, i - 1, j - 1);
                uint8_t r;
                uint8_t g;
                uint8_t b;
                APNG_HexARGB_TO_RGB_VAR(pixel, r, g, b);
                MAT2D_AT(src, i, j) = 0.299 * r + 0.587 * g + 0.114 * b;
            }
        }
    }

    Mat2D kernal_hori = mat2D_alloc(3, 3);
    MAT2D_AT(kernal_hori, 0, 0) = -1; MAT2D_AT(kernal_hori, 0, 1) = 0; MAT2D_AT(kernal_hori, 0, 2) = 1;
    MAT2D_AT(kernal_hori, 1, 0) = -2; MAT2D_AT(kernal_hori, 1, 1) = 0; MAT2D_AT(kernal_hori, 1, 2) = 2;
    MAT2D_AT(kernal_hori, 2, 0) = -1; MAT2D_AT(kernal_hori, 2, 1) = 0; MAT2D_AT(kernal_hori, 2, 2) = 1;
    Mat2D kernal_vert = mat2D_alloc(3, 3);
    MAT2D_AT(kernal_vert, 0, 0) = -1; MAT2D_AT(kernal_vert, 0, 1) = -2; MAT2D_AT(kernal_vert, 0, 2) = -1;
    MAT2D_AT(kernal_vert, 1, 0) =  0; MAT2D_AT(kernal_vert, 1, 1) =  0; MAT2D_AT(kernal_vert, 1, 2) =  0;
    MAT2D_AT(kernal_vert, 2, 0) =  1; MAT2D_AT(kernal_vert, 2, 1) =  2; MAT2D_AT(kernal_vert, 2, 2) =  1;

    mat2D_rotate_mat_180_deg_inplace(kernal_hori);
    mat2D_rotate_mat_180_deg_inplace(kernal_vert);

    mat2D_convolve(conv_hori, src, kernal_hori);
    mat2D_convolve(conv_vert, src, kernal_vert);

    for (size_t i = 0; i < des.rows; i++) {
        for (size_t j = 0; j < des.cols; j++) {
            MAT2D_AT(des, i, j) = hypot(MAT2D_AT(conv_hori, i, j), MAT2D_AT(conv_vert, i, j));
        }
    }

    mat2D_normalize_inf(des);
    mat2D_mult(des, 255);

    for (size_t i = 0; i < des.rows; i++) {
        for (size_t j = 0; j < des.cols; j++) {
            mat2D_real value = MAT2D_AT(des, i, j);

            uint8_t temp, alpha;
            APNG_HexARGB_TO_RGBA_VAR(APNG_PIXEL_BUFFER_AT(src_u32, i, j), temp, temp, temp, alpha);

            MAT2D_AT_UINT32(des_u32, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, alpha);
        }
    }

    mat2D_free(src);
    mat2D_free(conv_hori);
    mat2D_free(conv_vert);
}

Mat2D_uint32 results = {0};
struct Apng_PNG_Image image = {0};

enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
    ws->wanted_fps = 60;
    // ws->to_limit_fps = false;

    char file_name[] = "../src/test_images/test-png7.png";
    // char file_name[] = "../src/test_images/file_example_PNG_3MB.png";
    apng_png_free(&image);
    if (APNG_FAIL == apng_png_load(file_name, &image, false)) {
        return APL_FAIL;
    }

    results = mat2D_alloc_uint32(image.pixels.rows, image.pixels.cols);
    // edge_detection3x3(results, image.pixels);
    edge_detection5x5(results, image.pixels);

    // results = apng_pixel_buffer_mat2d_u32(image.pixels);

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
                    adl_point_draw(window_pixels, (float)(j * factor + v), (float)(i * factor + u), MAT2D_AT_UINT32(results, i, j), ADL_DEFAULT_OFFSET_ZOOM);
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
