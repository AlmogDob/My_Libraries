
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

void blur_box_blur_bw(Mat2D_uint32 des_u32, struct Apng_Pixel_Buffer src_u32, size_t kernel_size)
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
                uint32_t pixel = APNG_PIXEL_BUFFER_AT(src_u32, i - kernel_size / 2, j - kernel_size / 2);
                uint8_t r;
                uint8_t g;
                uint8_t b;
                APNG_HexARGB_TO_RGB_VAR(pixel, r, g, b);
                MAT2D_AT(src, i, j) = 0.299 * r + 0.587 * g + 0.114 * b;
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

            MAT2D_AT_UINT32(des_u32, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, alpha);
        }
    }

    mat2D_free(src);
    mat2D_free(conv);
    mat2D_free(kernel);
}

void blur_box_blur_rgba(Mat2D_uint32 des_u32, struct Apng_Pixel_Buffer src_u32, size_t kernel_size)
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
                uint32_t pixel = APNG_PIXEL_BUFFER_AT(src_u32, i - kernel_size / 2, j - kernel_size / 2);
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
            MAT2D_AT_UINT32(des_u32, i, j) = APNG_RGBA_TO_hexARGB(MAT2D_AT(conv_r, i, j) / a * 255, MAT2D_AT(conv_g, i, j) / a * 255, MAT2D_AT(conv_b, i, j) / a * 255, a);
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

void blur_gaussian_bw(Mat2D_uint32 des_u32, struct Apng_Pixel_Buffer src_u32, mat2D_real std)
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

    Mat2D src = mat2D_alloc(src_u32.rows + (kernel_size / 2) * 2, src_u32.cols + (kernel_size / 2) * 2);
    Mat2D conv = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D kernel = mat2D_alloc(kernel_size, kernel_size);

    for (size_t i = 0; i < src.rows; i++) {
        for (size_t j = 0; j < src.cols; j++) {
            if (i < kernel_size / 2 || i >= src.rows - kernel_size / 2 || j < kernel_size / 2|| j >= src.cols - kernel_size / 2) {
                MAT2D_AT(src, i, j) = 0;
            } else {
                uint32_t pixel = APNG_PIXEL_BUFFER_AT(src_u32, i - kernel_size / 2, j - kernel_size / 2);
                uint8_t r;
                uint8_t g;
                uint8_t b;
                APNG_HexARGB_TO_RGB_VAR(pixel, r, g, b);
                MAT2D_AT(src, i, j) = 0.299 * r + 0.587 * g + 0.114 * b;
            }
        }
    }

    for (size_t r = 0; r < kernel.rows; r++) {
        for (size_t c = 0; c < kernel.cols; c++) {
            int x = (int)c - (int)kernel_size / 2;
            int y = (int)r - (int)kernel_size / 2;
            MAT2D_AT(kernel, r, c) = (1 / (2 * MAT2D_PI * std * std)) * mat2D_exp(-1 * (x * x + y * y) / (2 * std * std));
        }
    }
    mat2D_mult(kernel, 1 / mat2D_elements_sum(kernel));

    mat2D_convolve(conv, src, kernel);

    for (size_t i = 0; i < conv.rows; i++) {
        for (size_t j = 0; j < conv.cols; j++) {
            mat2D_real value = MAT2D_AT(conv, i, j);

            uint8_t temp, alpha;
            APNG_HexARGB_TO_RGBA_VAR(APNG_PIXEL_BUFFER_AT(src_u32, i, j), temp, temp, temp, alpha);

            MAT2D_AT_UINT32(des_u32, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, alpha);
        }
    }

    mat2D_free(src);
    mat2D_free(conv);
    mat2D_free(kernel);
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

void blur_gaussian_rgba_fast(Mat2D_uint32 des_u32, struct Apng_Pixel_Buffer src_u32, mat2D_real std)
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

    Mat2D src_r = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D src_g = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D src_b = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D src_a = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D temp_conv = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_r = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_g = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_b = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_a = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D kernel_1D = mat2D_alloc(kernel_size, 1);

    for (size_t i = 0; i < src_r.rows; i++) {
        for (size_t j = 0; j < src_r.cols; j++) {
            uint32_t pixel = APNG_PIXEL_BUFFER_AT(src_u32, i, j);
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
            APNG_HexARGB_TO_RGBA_VAR(pixel, r, g, b, a);
            MAT2D_AT(src_r, i, j) = r * a / 255;
            MAT2D_AT(src_g, i, j) = g * a / 255;
            MAT2D_AT(src_b, i, j) = b * a / 255;
            MAT2D_AT(src_a, i, j) = a * a / 255;
        }
    }

    size_t radius = kernel_1D.rows / 2;
    for (size_t i = 0; i < kernel_1D.rows; i++) {
        int x = (int)i - (int)radius;
        MAT2D_AT(kernel_1D, i, 0) = mat2D_exp(-(mat2D_real)(x * x) / (2 * std * std));
    }
    mat2D_mult(kernel_1D, 1 / mat2D_elements_sum(kernel_1D));

    /* r */
    /* horizontal */
    for (size_t y = 0; y < temp_conv.rows; y++) {
        for (size_t x = 0; x < temp_conv.cols; x++) {
            mat2D_real sum = 0;
            for (int k = -(int)radius; k <= (int)radius; k++) {
                int xx = (int)x + k;
                if (xx < 0) xx = 0;
                if (xx >= (int)temp_conv.cols) xx = (int)temp_conv.cols - 1;
                sum += MAT2D_AT(src_r, y, xx) * MAT2D_AT(kernel_1D, k + radius, 0);
            }
            MAT2D_AT(temp_conv, y, x) = sum;
        }
    }
    /* vertical */
    for (size_t y = 0; y < conv_r.rows; y++) {
        for (size_t x = 0; x < conv_r.cols; x++) {
            mat2D_real sum = 0;
            for (int k = -(int)radius; k <= (int)radius; k++) {
                int yy = (int)y + k;
                if (yy < 0) yy = 0;
                if (yy >= (int)conv_r.rows) yy = (int)conv_r.rows - 1;
                sum += MAT2D_AT(temp_conv, yy, x) * MAT2D_AT(kernel_1D, k + radius, 0);
            }
            MAT2D_AT(conv_r, y, x) = sum;
        }
    }
    /* g */
    /* horizontal */
    for (size_t y = 0; y < temp_conv.rows; y++) {
        for (size_t x = 0; x < temp_conv.cols; x++) {
            mat2D_real sum = 0;
            for (int k = -(int)radius; k <= (int)radius; k++) {
                int xx = (int)x + k;
                if (xx < 0) xx = 0;
                if (xx >= (int)temp_conv.cols) xx = (int)temp_conv.cols - 1;
                sum += MAT2D_AT(src_g, y, xx) * MAT2D_AT(kernel_1D, k + radius, 0);
            }
            MAT2D_AT(temp_conv, y, x) = sum;
        }
    }
    /* vertical */
    for (size_t y = 0; y < conv_g.rows; y++) {
        for (size_t x = 0; x < conv_g.cols; x++) {
            mat2D_real sum = 0;
            for (int k = -(int)radius; k <= (int)radius; k++) {
                int yy = (int)y + k;
                if (yy < 0) yy = 0;
                if (yy >= (int)conv_g.rows) yy = (int)conv_g.rows - 1;
                sum += MAT2D_AT(temp_conv, yy, x) * MAT2D_AT(kernel_1D, k + radius, 0);
            }
            MAT2D_AT(conv_g, y, x) = sum;
        }
    }
    /* b */
    /* horizontal */
    for (size_t y = 0; y < temp_conv.rows; y++) {
        for (size_t x = 0; x < temp_conv.cols; x++) {
            mat2D_real sum = 0;
            for (int k = -(int)radius; k <= (int)radius; k++) {
                int xx = (int)x + k;
                if (xx < 0) xx = 0;
                if (xx >= (int)temp_conv.cols) xx = (int)temp_conv.cols - 1;
                sum += MAT2D_AT(src_b, y, xx) * MAT2D_AT(kernel_1D, k + radius, 0);
            }
            MAT2D_AT(temp_conv, y, x) = sum;
        }
    }
    /* vertical */
    for (size_t y = 0; y < conv_b.rows; y++) {
        for (size_t x = 0; x < conv_b.cols; x++) {
            mat2D_real sum = 0;
            for (int k = -(int)radius; k <= (int)radius; k++) {
                int yy = (int)y + k;
                if (yy < 0) yy = 0;
                if (yy >= (int)conv_b.rows) yy = (int)conv_b.rows - 1;
                sum += MAT2D_AT(temp_conv, yy, x) * MAT2D_AT(kernel_1D, k + radius, 0);
            }
            MAT2D_AT(conv_b, y, x) = sum;
        }
    }
    /* a */
    /* horizontal */
    for (size_t y = 0; y < temp_conv.rows; y++) {
        for (size_t x = 0; x < temp_conv.cols; x++) {
            mat2D_real sum = 0;
            for (int k = -(int)radius; k <= (int)radius; k++) {
                int xx = (int)x + k;
                if (xx < 0) xx = 0;
                if (xx >= (int)temp_conv.cols) xx = (int)temp_conv.cols - 1;
                sum += MAT2D_AT(src_a, y, xx) * MAT2D_AT(kernel_1D, k + radius, 0);
            }
            MAT2D_AT(temp_conv, y, x) = sum;
        }
    }
    /* vertical */
    for (size_t y = 0; y < conv_a.rows; y++) {
        for (size_t x = 0; x < conv_a.cols; x++) {
            mat2D_real sum = 0;
            for (int k = -(int)radius; k <= (int)radius; k++) {
                int yy = (int)y + k;
                if (yy < 0) yy = 0;
                if (yy >= (int)conv_a.rows) yy = (int)conv_a.rows - 1;
                sum += MAT2D_AT(temp_conv, yy, x) * MAT2D_AT(kernel_1D, k + radius, 0);
            }
            MAT2D_AT(conv_a, y, x) = sum;
        }
    }

    for (size_t i = 0; i < des_u32.rows; i++) {
        for (size_t j = 0; j < des_u32.cols; j++) {
            mat2D_real a = MAT2D_AT(conv_a, i, j);
            MAT2D_AT_UINT32(des_u32, i, j) = APNG_RGBA_TO_hexARGB(MAT2D_AT(conv_r, i, j) / a * 255, MAT2D_AT(conv_g, i, j) / a * 255, MAT2D_AT(conv_b, i, j) / a * 255, a);
        }
    }

    mat2D_free(src_r);
    mat2D_free(src_g);
    mat2D_free(src_b);
    mat2D_free(src_a);
    mat2D_free(temp_conv);
    mat2D_free(conv_r);
    mat2D_free(conv_g);
    mat2D_free(conv_b);
    mat2D_free(conv_a);
    mat2D_free(kernel_1D);
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
    // char file_name[] = "../src/test_images/gaussian_blue_test.png";

    apng_png_free(&image);
    if (APNG_FAIL == apng_png_load(file_name, &image, true)) {
        return APL_FAIL;
    }

    results = mat2D_alloc_uint32(image.pixels.rows, image.pixels.cols);
    // blur_box_blur_bw(results, image.pixels, 5);
    // blur_box_blur_rgba(results, image.pixels, 5);
    blur_gaussian_bw(results, image.pixels, 3);
    // blur_gaussian_bw_fast(results, image.pixels, 6);
    // blur_gaussian_rgba_fast(results, image.pixels, 5);

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
