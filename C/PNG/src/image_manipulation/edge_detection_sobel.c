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

void blur_gaussian_bw_fast(Mat2D_uint32 des_u32, struct Apng_Pixel_Buffer src_u32, mat2D_real std)
{
    /* https://en.wikipedia.org/wiki/Gaussian_blur */

    MAT2D_ASSERT(des_u32.cols == src_u32.cols);
    MAT2D_ASSERT(des_u32.rows == src_u32.rows);
    MAT2D_ASSERT(std > 0);

    size_t kernel_size = (size_t)(2 * mat2D_ceil(3 * std) + 1);
    /**
     * For a 1D Gaussian, about:
     * - 68.27% of the mass lies within 1 * std
     * - 95.45% within 2 * std
     * - 99.73% within 3 * std
     * So if you cut the kernel at radius 3 * std, you keep almost all of the Gaussian.
     */

    Mat2D src = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D temp_conv = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D kernel_1D = mat2D_alloc(kernel_size, 1);

    for (size_t i = 0; i < src.rows; i++) {
        for (size_t j = 0; j < src.cols; j++) {
            uint32_t pixel = APNG_PIXEL_BUFFER_AT(src_u32, i, j);
            uint8_t r;
            uint8_t g;
            uint8_t b;
            APNG_HexARGB_TO_RGB_VAR(pixel, r, g, b);
            MAT2D_AT(src, i, j) = 0.299 * r + 0.587 * g + 0.114 * b;
        }
    }

    size_t radius = kernel_1D.rows / 2;
    for (size_t i = 0; i < kernel_1D.rows; i++) {
        int x = (int)i - (int)radius;
        MAT2D_AT(kernel_1D, i, 0) = mat2D_exp(-(mat2D_real)(x * x) / (2 * std * std));
    }
    mat2D_mult(kernel_1D, 1 / mat2D_elements_sum(kernel_1D));

    /* horizontal */
    for (size_t y = 0; y < temp_conv.rows; y++) {
        for (size_t x = 0; x < temp_conv.cols; x++) {
            mat2D_real sum = 0;
            for (int k = -(int)radius; k <= (int)radius; k++) {
                int xx = (int)x + k;
                if (xx < 0) xx = 0;
                if (xx >= (int)temp_conv.cols) xx = (int)temp_conv.cols - 1;
                sum += MAT2D_AT(src, y, xx) * MAT2D_AT(kernel_1D, k + radius, 0);
            }
            MAT2D_AT(temp_conv, y, x) = sum;
        }
    }
    /* vertical */
    for (size_t y = 0; y < conv.rows; y++) {
        for (size_t x = 0; x < conv.cols; x++) {
            mat2D_real sum = 0;
            for (int k = -(int)radius; k <= (int)radius; k++) {
                int yy = (int)y + k;
                if (yy < 0) yy = 0;
                if (yy >= (int)conv.rows) yy = (int)conv.rows - 1;
                sum += MAT2D_AT(temp_conv, yy, x) * MAT2D_AT(kernel_1D, k + radius, 0);
            }
            MAT2D_AT(conv, y, x) = sum;
        }
    }

    for (size_t i = 0; i < conv.rows; i++) {
        for (size_t j = 0; j < conv.cols; j++) {
            mat2D_real value = MAT2D_AT(conv, i, j);
            uint8_t temp, alpha;
            APNG_HexARGB_TO_RGBA_VAR(APNG_PIXEL_BUFFER_AT(src_u32, i, j), temp, temp, temp, alpha);
            MAT2D_AT_UINT32(des_u32, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, alpha);
        }
    }

    mat2D_free(src);
    mat2D_free(temp_conv);
    mat2D_free(conv);
    mat2D_free(kernel_1D);
}

void edge_detection_sobel_5x5(Mat2D_uint32 des_u32, struct Apng_Pixel_Buffer src_u32)
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

    Mat2D kernel_hori = mat2D_alloc(5, 5);
    MAT2D_AT(kernel_hori, 0, 0) =  2; MAT2D_AT(kernel_hori, 0, 1) =  1; MAT2D_AT(kernel_hori, 0, 2) =  0; MAT2D_AT(kernel_hori, 0, 3) = -1; MAT2D_AT(kernel_hori, 0, 4) = -2;
    MAT2D_AT(kernel_hori, 1, 0) =  3; MAT2D_AT(kernel_hori, 1, 1) =  2; MAT2D_AT(kernel_hori, 1, 2) =  0; MAT2D_AT(kernel_hori, 1, 3) = -2; MAT2D_AT(kernel_hori, 1, 4) = -3;
    MAT2D_AT(kernel_hori, 2, 0) =  4; MAT2D_AT(kernel_hori, 2, 1) =  3; MAT2D_AT(kernel_hori, 2, 2) =  0; MAT2D_AT(kernel_hori, 2, 3) = -3; MAT2D_AT(kernel_hori, 2, 4) = -4;
    MAT2D_AT(kernel_hori, 3, 0) =  3; MAT2D_AT(kernel_hori, 3, 1) =  2; MAT2D_AT(kernel_hori, 3, 2) =  0; MAT2D_AT(kernel_hori, 3, 3) = -2; MAT2D_AT(kernel_hori, 3, 4) = -3;
    MAT2D_AT(kernel_hori, 4, 0) =  2; MAT2D_AT(kernel_hori, 4, 1) =  1; MAT2D_AT(kernel_hori, 4, 2) =  0; MAT2D_AT(kernel_hori, 4, 3) = -1; MAT2D_AT(kernel_hori, 4, 4) = -2;
    Mat2D kernel_vert = mat2D_alloc(5, 5);
    MAT2D_AT(kernel_vert, 0, 0) =  2; MAT2D_AT(kernel_vert, 0, 1) =  3; MAT2D_AT(kernel_vert, 0, 2) =  4; MAT2D_AT(kernel_vert, 0, 3) =  3; MAT2D_AT(kernel_vert, 0, 4) =  2;
    MAT2D_AT(kernel_vert, 1, 0) =  1; MAT2D_AT(kernel_vert, 1, 1) =  2; MAT2D_AT(kernel_vert, 1, 2) =  3; MAT2D_AT(kernel_vert, 1, 3) =  2; MAT2D_AT(kernel_vert, 1, 4) =  1;
    MAT2D_AT(kernel_vert, 2, 0) =  0; MAT2D_AT(kernel_vert, 2, 1) =  0; MAT2D_AT(kernel_vert, 2, 2) =  0; MAT2D_AT(kernel_vert, 2, 3) =  0; MAT2D_AT(kernel_vert, 2, 4) =  0;
    MAT2D_AT(kernel_vert, 3, 0) = -1; MAT2D_AT(kernel_vert, 3, 1) = -2; MAT2D_AT(kernel_vert, 3, 2) = -3; MAT2D_AT(kernel_vert, 3, 3) = -2; MAT2D_AT(kernel_vert, 3, 4) = -1;
    MAT2D_AT(kernel_vert, 4, 0) = -2; MAT2D_AT(kernel_vert, 4, 1) = -3; MAT2D_AT(kernel_vert, 4, 2) = -4; MAT2D_AT(kernel_vert, 4, 3) = -3; MAT2D_AT(kernel_vert, 4, 4) = -2;

    mat2D_rotate_mat_180_deg_inplace(kernel_hori);
    mat2D_rotate_mat_180_deg_inplace(kernel_vert);

    mat2D_convolve(conv_hori, src, kernel_hori);
    mat2D_convolve(conv_vert, src, kernel_vert);

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
    mat2D_free(des);
    mat2D_free(kernel_hori);
    mat2D_free(kernel_vert);
    mat2D_free(conv_hori);
    mat2D_free(conv_vert);
}

void edge_detection_sobel_5x5_cutoff(Mat2D_uint32 des_u32, struct Apng_Pixel_Buffer src_u32, mat2D_real cutoff)
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

    Mat2D kernel_hori = mat2D_alloc(5, 5);
    MAT2D_AT(kernel_hori, 0, 0) =  2; MAT2D_AT(kernel_hori, 0, 1) =  1; MAT2D_AT(kernel_hori, 0, 2) =  0; MAT2D_AT(kernel_hori, 0, 3) = -1; MAT2D_AT(kernel_hori, 0, 4) = -2;
    MAT2D_AT(kernel_hori, 1, 0) =  3; MAT2D_AT(kernel_hori, 1, 1) =  2; MAT2D_AT(kernel_hori, 1, 2) =  0; MAT2D_AT(kernel_hori, 1, 3) = -2; MAT2D_AT(kernel_hori, 1, 4) = -3;
    MAT2D_AT(kernel_hori, 2, 0) =  4; MAT2D_AT(kernel_hori, 2, 1) =  3; MAT2D_AT(kernel_hori, 2, 2) =  0; MAT2D_AT(kernel_hori, 2, 3) = -3; MAT2D_AT(kernel_hori, 2, 4) = -4;
    MAT2D_AT(kernel_hori, 3, 0) =  3; MAT2D_AT(kernel_hori, 3, 1) =  2; MAT2D_AT(kernel_hori, 3, 2) =  0; MAT2D_AT(kernel_hori, 3, 3) = -2; MAT2D_AT(kernel_hori, 3, 4) = -3;
    MAT2D_AT(kernel_hori, 4, 0) =  2; MAT2D_AT(kernel_hori, 4, 1) =  1; MAT2D_AT(kernel_hori, 4, 2) =  0; MAT2D_AT(kernel_hori, 4, 3) = -1; MAT2D_AT(kernel_hori, 4, 4) = -2;
    Mat2D kernel_vert = mat2D_alloc(5, 5);
    MAT2D_AT(kernel_vert, 0, 0) =  2; MAT2D_AT(kernel_vert, 0, 1) =  3; MAT2D_AT(kernel_vert, 0, 2) =  4; MAT2D_AT(kernel_vert, 0, 3) =  3; MAT2D_AT(kernel_vert, 0, 4) =  2;
    MAT2D_AT(kernel_vert, 1, 0) =  1; MAT2D_AT(kernel_vert, 1, 1) =  2; MAT2D_AT(kernel_vert, 1, 2) =  3; MAT2D_AT(kernel_vert, 1, 3) =  2; MAT2D_AT(kernel_vert, 1, 4) =  1;
    MAT2D_AT(kernel_vert, 2, 0) =  0; MAT2D_AT(kernel_vert, 2, 1) =  0; MAT2D_AT(kernel_vert, 2, 2) =  0; MAT2D_AT(kernel_vert, 2, 3) =  0; MAT2D_AT(kernel_vert, 2, 4) =  0;
    MAT2D_AT(kernel_vert, 3, 0) = -1; MAT2D_AT(kernel_vert, 3, 1) = -2; MAT2D_AT(kernel_vert, 3, 2) = -3; MAT2D_AT(kernel_vert, 3, 3) = -2; MAT2D_AT(kernel_vert, 3, 4) = -1;
    MAT2D_AT(kernel_vert, 4, 0) = -2; MAT2D_AT(kernel_vert, 4, 1) = -3; MAT2D_AT(kernel_vert, 4, 2) = -4; MAT2D_AT(kernel_vert, 4, 3) = -3; MAT2D_AT(kernel_vert, 4, 4) = -2;

    mat2D_rotate_mat_180_deg_inplace(kernel_hori);
    mat2D_rotate_mat_180_deg_inplace(kernel_vert);

    mat2D_convolve(conv_hori, src, kernel_hori);
    mat2D_convolve(conv_vert, src, kernel_vert);

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
            if (value > cutoff) value = cutoff;
            MAT2D_AT(des, i, j) = value;
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
    mat2D_free(des);
    mat2D_free(kernel_hori);
    mat2D_free(kernel_vert);
    mat2D_free(conv_hori);
    mat2D_free(conv_vert);
}

void edge_detection_sobel_3x3(Mat2D_uint32 des_u32, struct Apng_Pixel_Buffer src_u32)
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

    Mat2D kernel_hori = mat2D_alloc(3, 3);
    MAT2D_AT(kernel_hori, 0, 0) = -1; MAT2D_AT(kernel_hori, 0, 1) = 0; MAT2D_AT(kernel_hori, 0, 2) = 1;
    MAT2D_AT(kernel_hori, 1, 0) = -2; MAT2D_AT(kernel_hori, 1, 1) = 0; MAT2D_AT(kernel_hori, 1, 2) = 2;
    MAT2D_AT(kernel_hori, 2, 0) = -1; MAT2D_AT(kernel_hori, 2, 1) = 0; MAT2D_AT(kernel_hori, 2, 2) = 1;
    Mat2D kernel_vert = mat2D_alloc(3, 3);
    MAT2D_AT(kernel_vert, 0, 0) = -1; MAT2D_AT(kernel_vert, 0, 1) = -2; MAT2D_AT(kernel_vert, 0, 2) = -1;
    MAT2D_AT(kernel_vert, 1, 0) =  0; MAT2D_AT(kernel_vert, 1, 1) =  0; MAT2D_AT(kernel_vert, 1, 2) =  0;
    MAT2D_AT(kernel_vert, 2, 0) =  1; MAT2D_AT(kernel_vert, 2, 1) =  2; MAT2D_AT(kernel_vert, 2, 2) =  1;

    mat2D_rotate_mat_180_deg_inplace(kernel_hori);
    mat2D_rotate_mat_180_deg_inplace(kernel_vert);

    mat2D_convolve(conv_hori, src, kernel_hori);
    mat2D_convolve(conv_vert, src, kernel_vert);

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
    mat2D_free(des);
    mat2D_free(kernel_hori);
    mat2D_free(kernel_vert);
    mat2D_free(conv_hori);
    mat2D_free(conv_vert);
}

void edge_detection_sobel_3x3_cutoff(Mat2D_uint32 des_u32, struct Apng_Pixel_Buffer src_u32, mat2D_real cutoff)
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

    Mat2D kernel_hori = mat2D_alloc(3, 3);
    MAT2D_AT(kernel_hori, 0, 0) = -1; MAT2D_AT(kernel_hori, 0, 1) = 0; MAT2D_AT(kernel_hori, 0, 2) = 1;
    MAT2D_AT(kernel_hori, 1, 0) = -2; MAT2D_AT(kernel_hori, 1, 1) = 0; MAT2D_AT(kernel_hori, 1, 2) = 2;
    MAT2D_AT(kernel_hori, 2, 0) = -1; MAT2D_AT(kernel_hori, 2, 1) = 0; MAT2D_AT(kernel_hori, 2, 2) = 1;
    Mat2D kernel_vert = mat2D_alloc(3, 3);
    MAT2D_AT(kernel_vert, 0, 0) = -1; MAT2D_AT(kernel_vert, 0, 1) = -2; MAT2D_AT(kernel_vert, 0, 2) = -1;
    MAT2D_AT(kernel_vert, 1, 0) =  0; MAT2D_AT(kernel_vert, 1, 1) =  0; MAT2D_AT(kernel_vert, 1, 2) =  0;
    MAT2D_AT(kernel_vert, 2, 0) =  1; MAT2D_AT(kernel_vert, 2, 1) =  2; MAT2D_AT(kernel_vert, 2, 2) =  1;

    mat2D_rotate_mat_180_deg_inplace(kernel_hori);
    mat2D_rotate_mat_180_deg_inplace(kernel_vert);

    mat2D_convolve(conv_hori, src, kernel_hori);
    mat2D_convolve(conv_vert, src, kernel_vert);

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
            if (value > cutoff) value = cutoff;
            MAT2D_AT(des, i, j) = value;
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
    mat2D_free(des);
    mat2D_free(kernel_hori);
    mat2D_free(kernel_vert);
    mat2D_free(conv_hori);
    mat2D_free(conv_vert);
}

Mat2D_uint32 results = {0};
struct Apng_PNG_Image image = {0};

enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
    ws->wanted_fps = 60;
    // ws->to_limit_fps = false;

    // char file_name[] = "../src/test_images/test-png7.png";
    // char file_name[] = "../src/test_images/test-png3.png";
    // char file_name[] = "../src/test_images/file_example_PNG_3MB.png";
    char file_name[] = "../src/test_images/Valve_original.png";

    apng_png_free(&image);
    if (APNG_FAIL == apng_png_load(file_name, &image, true)) {
        return APL_FAIL;
    }

    Mat2D_uint32 temp = mat2D_alloc_uint32(image.pixels.rows, image.pixels.cols);
    results = mat2D_alloc_uint32(image.pixels.rows, image.pixels.cols);

    // edge_detection_sobel_3x3(results, image.pixels);
    // edge_detection_sobel_3x3_cutoff(results, image.pixels, 100);
    // edge_detection_sobel_5x5(results, image.pixels);
    edge_detection_sobel_5x5_cutoff(results, image.pixels, 150);

    // blur_gaussian_bw_fast(temp, image.pixels, 1.5);
    // struct Apng_Pixel_Buffer temp_b = mat2d_u32_as_apng_pixel_buffer(temp);
    // // edge_detection_sobel_3x3(results, temp_b);
    // // edge_detection_sobel_3x3_cutoff(results, temp_b, 100);
    // // edge_detection_sobel_5x5(results, temp_b);
    // edge_detection_sobel_5x5_cutoff(results, temp_b, 100);

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
