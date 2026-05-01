#ifndef ALMOG_IMAGE_MANIPULATION_H_
#define ALMOG_IMAGE_MANIPULATION_H_

#include "Matrix2D.h"

#ifndef AIM_DEF
    #ifdef AIM_DEF_STATIC
        #define AIM_DEF static
    #else
        #define AIM_DEF extern
    #endif
#endif

AIM_DEF void aim_blur_box_blur_bw(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size);
AIM_DEF void aim_blur_box_blur_rgba(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size);
AIM_DEF void aim_blur_gaussian_bw(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real std);
AIM_DEF void aim_blur_gaussian_bw_fast(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real std);
AIM_DEF void aim_blur_gaussian_rgba_fast(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real std);
AIM_DEF void aim_build_sobel_1d(Mat2D smooth, Mat2D deriv);
AIM_DEF void aim_build_sobel_kernels(Mat2D gx, Mat2D gy, size_t kernel_size);
AIM_DEF void aim_edge_detection_shcarr_3x3(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32);
AIM_DEF void aim_edge_detection_sobel_3x3(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32);
AIM_DEF void aim_edge_detection_sobel_3x3_cutoff(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real cutoff);
AIM_DEF void aim_edge_detection_sobel_5x5(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32);
AIM_DEF void aim_edge_detection_sobel_5x5_cutoff(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real cutoff);
AIM_DEF void aim_edge_detection_sobel_general(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size);
AIM_DEF void aim_edge_detection_sobel_general_cutoff(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size, mat2D_real cutoff);
AIM_DEF void aim_fill_binomial_row(Mat2D v);



#endif /*ALMOG_IMAGE_MANIPULATION_H_*/

#ifdef ALMOG_IMAGE_MANIPULATION_IMPLEMENTATION
#undef ALMOG_IMAGE_MANIPULATION_IMPLEMENTATION

AIM_DEF void aim_blur_box_blur_bw(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size)
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

AIM_DEF void aim_blur_box_blur_rgba(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size)
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

AIM_DEF void aim_blur_gaussian_bw(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real std)
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
                uint32_t pixel = MAT2D_AT(src_u32, i - kernel_size / 2, j - kernel_size / 2);
                uint8_t r;
                uint8_t g;
                uint8_t b;
                APNG_HexARGB_TO_RGB_VAR(pixel, r, g, b);
                MAT2D_AT(src, i, j) = 0.2126 * r + 0.7152 * g + 0.0722 * b;
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
            APNG_HexARGB_TO_RGBA_VAR(MAT2D_AT(src_u32, i, j), temp, temp, temp, alpha);

            MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, alpha);
        }
    }

    mat2D_free(src);
    mat2D_free(conv);
    mat2D_free(kernel);
}

AIM_DEF void aim_blur_gaussian_bw_fast(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real std)
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
            uint32_t pixel = MAT2D_AT(src_u32, i, j);
            uint8_t r;
            uint8_t g;
            uint8_t b;
            APNG_HexARGB_TO_RGB_VAR(pixel, r, g, b);
            MAT2D_AT(src, i, j) = 0.2126 * r + 0.7152 * g + 0.0722 * b;
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
            APNG_HexARGB_TO_RGBA_VAR(MAT2D_AT(src_u32, i, j), temp, temp, temp, alpha);
            MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, alpha);
        }
    }

    mat2D_free(src);
    mat2D_free(temp_conv);
    mat2D_free(conv);
    mat2D_free(kernel_1D);
}

AIM_DEF void aim_blur_gaussian_rgba_fast(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real std)
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
            uint32_t pixel = MAT2D_AT(src_u32, i, j);
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
            MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(MAT2D_AT(conv_r, i, j) / a * 255, MAT2D_AT(conv_g, i, j) / a * 255, MAT2D_AT(conv_b, i, j) / a * 255, a);
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

AIM_DEF void aim_build_sobel_1d(Mat2D smooth, Mat2D deriv)
{
    MAT2D_ASSERT(smooth.rows == 1 || smooth.cols == 1);
    MAT2D_ASSERT(deriv.rows == 1 || deriv.cols == 1);

    size_t n_s = (smooth.rows == 1) ? smooth.cols : smooth.rows;
    size_t n_d = (deriv.rows == 1) ? deriv.cols : deriv.rows;

    MAT2D_ASSERT(n_s == n_d);
    MAT2D_ASSERT(n_s >= 3);
    MAT2D_ASSERT(n_s % 2 == 1);

    size_t n = n_s;

    aim_fill_binomial_row(smooth);

    Mat2D temp = mat2D_alloc(n - 2, 1);
    aim_fill_binomial_row(temp);

    mat2D_fill(deriv, 0);

    if (deriv.rows == 1) {
        for (size_t i = 0; i < n - 2; i++) {
            mat2D_real t = MAT2D_AT(temp, i, 0);
            MAT2D_AT(deriv, 0, i)     -= t;
            MAT2D_AT(deriv, 0, i + 2) += t;
        }
    } else {
        for (size_t i = 0; i < n - 2; i++) {
            mat2D_real t = MAT2D_AT(temp, i, 0);
            MAT2D_AT(deriv, i, 0)     -= t;
            MAT2D_AT(deriv, i + 2, 0) += t;
        }
    }

    mat2D_free(temp);
}

AIM_DEF void aim_build_sobel_kernels(Mat2D gx, Mat2D gy, size_t kernel_size)
{
    MAT2D_ASSERT(gx.rows == kernel_size && gx.cols == kernel_size);
    MAT2D_ASSERT(gy.rows == kernel_size && gy.cols == kernel_size);
    MAT2D_ASSERT(kernel_size >= 3);
    MAT2D_ASSERT(kernel_size % 2 == 1);

    Mat2D smooth_col = mat2D_alloc(kernel_size, 1);
    Mat2D deriv_col = mat2D_alloc(kernel_size, 1);
    Mat2D smooth_row = mat2D_alloc(1, kernel_size);
    Mat2D deriv_row = mat2D_alloc(1, kernel_size);

    aim_build_sobel_1d(smooth_col, deriv_col);
    mat2D_transpose(smooth_row, smooth_col);
    mat2D_transpose(deriv_row, deriv_col);

    mat2D_dot(gx, smooth_col, deriv_row);
    mat2D_dot(gy, deriv_col, smooth_row);

    mat2D_free(smooth_col);
    mat2D_free(deriv_col);
    mat2D_free(smooth_row);
    mat2D_free(deriv_row);
}

AIM_DEF void aim_edge_detection_shcarr_3x3(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32)
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
                uint32_t pixel = MAT2D_AT(src_u32, i - 1, j - 1);
                uint8_t r;
                uint8_t g;
                uint8_t b;
                APNG_HexARGB_TO_RGB_VAR(pixel, r, g, b);
                MAT2D_AT(src, i, j) = 0.299 * r + 0.587 * g + 0.114 * b;
            }
        }
    }

    Mat2D kernel_hori = mat2D_alloc(3, 3);
    MAT2D_AT(kernel_hori, 0, 0) =  3 ; MAT2D_AT(kernel_hori, 0, 1) =  0 ; MAT2D_AT(kernel_hori, 0, 2) = -3 ;
    MAT2D_AT(kernel_hori, 1, 0) =  10; MAT2D_AT(kernel_hori, 1, 1) =  0 ; MAT2D_AT(kernel_hori, 1, 2) = -10;
    MAT2D_AT(kernel_hori, 2, 0) =  3 ; MAT2D_AT(kernel_hori, 2, 1) =  0 ; MAT2D_AT(kernel_hori, 2, 2) = -3 ;
    Mat2D kernel_vert = mat2D_alloc(3, 3);
    MAT2D_AT(kernel_vert, 0, 0) =  3 ; MAT2D_AT(kernel_vert, 0, 1) =  10; MAT2D_AT(kernel_vert, 0, 2) =  3 ;
    MAT2D_AT(kernel_vert, 1, 0) =  0 ; MAT2D_AT(kernel_vert, 1, 1) =  0 ; MAT2D_AT(kernel_vert, 1, 2) =  0 ;
    MAT2D_AT(kernel_vert, 2, 0) = -3 ; MAT2D_AT(kernel_vert, 2, 1) = -10; MAT2D_AT(kernel_vert, 2, 2) = -3 ;

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
            APNG_HexARGB_TO_RGBA_VAR(MAT2D_AT(src_u32, i, j), temp, temp, temp, alpha);
            MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, alpha);
        }
    }

    mat2D_free(src);
    mat2D_free(des);
    mat2D_free(kernel_hori);
    mat2D_free(kernel_vert);
    mat2D_free(conv_hori);
    mat2D_free(conv_vert);
}

AIM_DEF void aim_edge_detection_sobel_3x3(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32)
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
                uint32_t pixel = MAT2D_AT(src_u32, i - 1, j - 1);
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
            APNG_HexARGB_TO_RGBA_VAR(MAT2D_AT(src_u32, i, j), temp, temp, temp, alpha);
            MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, alpha);
        }
    }

    mat2D_free(src);
    mat2D_free(des);
    mat2D_free(kernel_hori);
    mat2D_free(kernel_vert);
    mat2D_free(conv_hori);
    mat2D_free(conv_vert);
}

AIM_DEF void aim_edge_detection_sobel_3x3_cutoff(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real cutoff)
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
                uint32_t pixel = MAT2D_AT(src_u32, i - 1, j - 1);
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
            APNG_HexARGB_TO_RGBA_VAR(MAT2D_AT(src_u32, i, j), temp, temp, temp, alpha);
            MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, alpha);
        }
    }

    mat2D_free(src);
    mat2D_free(des);
    mat2D_free(kernel_hori);
    mat2D_free(kernel_vert);
    mat2D_free(conv_hori);
    mat2D_free(conv_vert);
}

AIM_DEF void aim_edge_detection_sobel_5x5(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32)
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
                uint32_t pixel = MAT2D_AT(src_u32, i - 2, j - 2);
                uint8_t r;
                uint8_t g;
                uint8_t b;
                uint8_t a;
                APNG_HexARGB_TO_RGBA_VAR(pixel, r, g, b, a);
                MAT2D_AT(src, i, j) = 0.299 * r + 0.587 * g + 0.114 * b;
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
            APNG_HexARGB_TO_RGBA_VAR(MAT2D_AT(src_u32, i, j), temp, temp, temp, alpha);
            MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, alpha);
        }
    }

    mat2D_free(src);
    mat2D_free(des);
    mat2D_free(kernel_hori);
    mat2D_free(kernel_vert);
    mat2D_free(conv_hori);
    mat2D_free(conv_vert);
}

AIM_DEF void aim_edge_detection_sobel_5x5_cutoff(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real cutoff)
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
                uint32_t pixel = MAT2D_AT(src_u32, i - 2, j - 2);
                uint8_t r;
                uint8_t g;
                uint8_t b;
                uint8_t a;
                APNG_HexARGB_TO_RGBA_VAR(pixel, r, g, b, a);
                MAT2D_AT(src, i, j) = 0.299 * r + 0.587 * g + 0.114 * b;
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
            APNG_HexARGB_TO_RGBA_VAR(MAT2D_AT(src_u32, i, j), temp, temp, temp, alpha);
            MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, alpha);
        }
    }

    mat2D_free(src);
    mat2D_free(des);
    mat2D_free(kernel_hori);
    mat2D_free(kernel_vert);
    mat2D_free(conv_hori);
    mat2D_free(conv_vert);
}

AIM_DEF void aim_edge_detection_sobel_general(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size)
{
    /* https://en.wikipedia.org/wiki/Sobel_operator */

    MAT2D_ASSERT(des_u32.cols == src_u32.cols);
    MAT2D_ASSERT(des_u32.rows == src_u32.rows);
    MAT2D_ASSERT(kernel_size >= 3);
    MAT2D_ASSERT(kernel_size % 2 == 1);

    size_t radius = kernel_size / 2;

    Mat2D src = mat2D_alloc(src_u32.rows + 2 * radius, src_u32.cols + 2 * radius);
    Mat2D des = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_hori = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_vert = mat2D_alloc(src_u32.rows, src_u32.cols);

    Mat2D kernel_hori = mat2D_alloc(kernel_size, kernel_size);
    Mat2D kernel_vert = mat2D_alloc(kernel_size, kernel_size);

    for (size_t i = 0; i < src.rows; i++) {
        for (size_t j = 0; j < src.cols; j++) {
            if (i < radius || i >= src.rows - radius ||
                j < radius || j >= src.cols - radius) {
                MAT2D_AT(src, i, j) = 0;
            } else {
                uint32_t pixel = MAT2D_AT(src_u32, i - radius, j - radius);
                uint8_t r;
                uint8_t g;
                uint8_t b;
                APNG_HexARGB_TO_RGB_VAR(pixel, r, g, b);
                MAT2D_AT(src, i, j) = 0.299 * r + 0.587 * g + 0.114 * b;
            }
        }
    }

    aim_build_sobel_kernels(kernel_hori, kernel_vert, kernel_size);

    mat2D_rotate_mat_180_deg_inplace(kernel_hori);
    mat2D_rotate_mat_180_deg_inplace(kernel_vert);

    /* mat2D_convolve rotates kernels in-place, so use copies */
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
            APNG_HexARGB_TO_RGBA_VAR(MAT2D_AT(src_u32, i, j), temp, temp, temp, alpha);

            MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, alpha);
        }
    }

    mat2D_free(src);
    mat2D_free(des);
    mat2D_free(conv_hori);
    mat2D_free(conv_vert);
    mat2D_free(kernel_hori);
    mat2D_free(kernel_vert);
}

AIM_DEF void aim_edge_detection_sobel_general_cutoff(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size, mat2D_real cutoff)
{
    /* https://en.wikipedia.org/wiki/Sobel_operator */

    MAT2D_ASSERT(des_u32.cols == src_u32.cols);
    MAT2D_ASSERT(des_u32.rows == src_u32.rows);
    MAT2D_ASSERT(kernel_size >= 3);
    MAT2D_ASSERT(kernel_size % 2 == 1);

    size_t radius = kernel_size / 2;

    Mat2D src = mat2D_alloc(src_u32.rows + 2 * radius, src_u32.cols + 2 * radius);
    Mat2D des = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_hori = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_vert = mat2D_alloc(src_u32.rows, src_u32.cols);

    Mat2D kernel_hori = mat2D_alloc(kernel_size, kernel_size);
    Mat2D kernel_vert = mat2D_alloc(kernel_size, kernel_size);

    for (size_t i = 0; i < src.rows; i++) {
        for (size_t j = 0; j < src.cols; j++) {
            if (i < radius || i >= src.rows - radius ||
                j < radius || j >= src.cols - radius) {
                MAT2D_AT(src, i, j) = 0;
            } else {
                uint32_t pixel = MAT2D_AT(src_u32, i - radius, j - radius);
                uint8_t r;
                uint8_t g;
                uint8_t b;
                APNG_HexARGB_TO_RGB_VAR(pixel, r, g, b);
                MAT2D_AT(src, i, j) = 0.299 * r + 0.587 * g + 0.114 * b;
            }
        }
    }

    aim_build_sobel_kernels(kernel_hori, kernel_vert, kernel_size);

    mat2D_rotate_mat_180_deg_inplace(kernel_hori);
    mat2D_rotate_mat_180_deg_inplace(kernel_vert);

    /* mat2D_convolve rotates kernels in-place, so use copies */
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
            APNG_HexARGB_TO_RGBA_VAR(MAT2D_AT(src_u32, i, j), temp, temp, temp, alpha);

            MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(value, value, value, alpha);
        }
    }

    mat2D_free(src);
    mat2D_free(des);
    mat2D_free(conv_hori);
    mat2D_free(conv_vert);
    mat2D_free(kernel_hori);
    mat2D_free(kernel_vert);
}

AIM_DEF void aim_fill_binomial_row(Mat2D v)
{
    MAT2D_ASSERT(v.rows == 1 || v.cols == 1);

    size_t n = (v.rows == 1) ? v.cols : v.rows;

    for (size_t i = 0; i < n; i++) {
        if (v.rows == 1) {
            MAT2D_AT(v, 0, i) = 0;
        } else {
            MAT2D_AT(v, i, 0) = 0;
        }
    }

    if (v.rows == 1) {
        MAT2D_AT(v, 0, 0) = 1;
    } else {
        MAT2D_AT(v, 0, 0) = 1;
    }

    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = i + 1; j > 0; j--) {
            if (v.rows == 1) {
                MAT2D_AT(v, 0, j) += MAT2D_AT(v, 0, j - 1);
            } else {
                MAT2D_AT(v, j, 0) += MAT2D_AT(v, j - 1, 0);
            }
        }
    }
}


#endif /*ALMOG_IMAGE_MANIPULATION_IMPLEMENTATION*/
