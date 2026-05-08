/**
 * @file
 * @brief Single-header image processing utilities built on top of Matrix2D.
 *
 * This library provides a small collection of image-processing routines that
 * operate on 2D matrices of 32-bit packed ARGB pixels (`Mat2D_uint32`).
 *
 * The library includes:
 * - box blur
 * - Gaussian blur
 * - median filtering
 * - unsharp-mask style sharpening
 * - Sobel and Scharr-based edge detection
 * - helper routines for constructing generalized Sobel kernels
 *
 * General behavior:
 * - Most grayscale operations convert the input RGB image into a single-channel
 *   luminance image, process it, and then write the result back into all RGB
 *   channels while preserving the original alpha channel.
 * - Most RGBA operations process each color channel separately using
 *   alpha-aware or premultiplied-alpha style logic, then reconstruct the final
 *   pixel.
 * - Several functions use explicit zero-padding, while some "fast" Gaussian
 *   variants use clamped borders instead. This means edge behavior may differ
 *   slightly between implementations.
 *
 * Typical usage:
 * - Use blur functions to smooth noise or soften an image.
 * - Use median filters to reduce salt-and-pepper noise.
 * - Use sharpen functions to enhance local contrast after a blur.
 * - Use edge detection functions to generate grayscale edge maps.
 * - Use the Sobel-kernel builders if you want to construct derivative kernels
 *   for your own custom image-processing pipeline.
 *
 * Single-header integration:
 * - Include this header normally wherever you need the declarations.
 * - In exactly one C/C++ source file, define
 *   `ALMOG_IMAGE_MANIPULATION_IMPLEMENTATION` before including this header to
 *   generate the implementation.
 * - If you want the functions to have internal linkage, define
 *   `AIM_DEF_STATIC` before including the header.
 *
 * Example:
 * @code{.c}
 * // In one .c file:
 * #define ALMOG_IMAGE_MANIPULATION_IMPLEMENTATION
 * #include "AlmogImageManipulation.h"
 *
 * // In other files:
 * #include "AlmogImageManipulation.h"
 * @endcode
 *
 * Dependencies:
 * - `Matrix2D.h`
 * - matrix allocation, convolution, transpose, dot-product and normalization
 *   helpers from the Matrix2D library
 * - packed-pixel conversion macros such as `APNG_HexARGB_TO_RGB_VAR`,
 *   `APNG_HexARGB_TO_RGBA_VAR`, and `APNG_RGBA_TO_hexARGB`
 */

#ifndef ALMOG_IMAGE_MANIPULATION_H_
#define ALMOG_IMAGE_MANIPULATION_H_

#include "Matrix2D.h"

/**
 * @def AIM_DEF
 * @brief Storage-class control macro for the library's function definitions.
 *
 * Behavior:
 * - Expands to `extern` by default.
 * - Expands to `static` if `AIM_DEF_STATIC` is defined before including
 *   this header.
 *
 * This is useful for choosing between:
 * - normal externally linked functions, or
 * - internal-linkage single-translation-unit usage.
 */
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
AIM_DEF void aim_edge_detection_scharr_3x3(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32);
AIM_DEF void aim_edge_detection_sobel_3x3(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32);
AIM_DEF void aim_edge_detection_sobel_3x3_cutoff(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real cutoff);
AIM_DEF void aim_edge_detection_sobel_5x5(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32);
AIM_DEF void aim_edge_detection_sobel_5x5_cutoff(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real cutoff);
AIM_DEF void aim_edge_detection_sobel_general(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size);
AIM_DEF void aim_edge_detection_sobel_general_cutoff(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size, mat2D_real cutoff);
AIM_DEF void aim_fill_binomial_row(Mat2D v);
AIM_DEF void aim_median_filter_bw(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size);
AIM_DEF void aim_median_filter_rgba(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size);
AIM_DEF void aim_sharpen_bw(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real std, mat2D_real amount);
AIM_DEF void aim_sharpen_rgba(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real std, mat2D_real amount);



#endif /*ALMOG_IMAGE_MANIPULATION_H_*/

#ifdef ALMOG_IMAGE_MANIPULATION_IMPLEMENTATION
#undef ALMOG_IMAGE_MANIPULATION_IMPLEMENTATION

/**
 * @brief Apply a box blur to an image after converting it to grayscale.
 *
 * This function converts the source image to a grayscale luminance image using
 * weighted RGB coefficients, applies a normalized square box filter of size
 * `kernel_size x kernel_size`, and writes the blurred grayscale result to the
 * destination image.
 *
 * The output RGB channels all receive the same grayscale value. The alpha
 * channel is copied from the source image.
 *
 * Internally, this version uses explicit zero-padding around the source image.
 *
 * Typical use:
 * - fast and simple smoothing
 * - reducing fine detail before edge detection
 * - producing a soft grayscale version of an image
 *
 * @param[out] des_u32 Destination image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 * @param[in]  kernel_size Odd kernel size greater than 2.
 */
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

/**
 * @brief Apply a box blur to an RGBA image.
 *
 * This function blurs each channel with a normalized square box filter while
 * handling transparency in an alpha-aware manner. The RGB channels are first
 * multiplied by alpha, blurred, and then divided by the blurred alpha to
 * reconstruct the final color.
 *
 * This helps avoid dark halos around transparent regions compared to naively
 * blurring RGB independently.
 *
 * Internally, this version uses explicit zero-padding around the source image.
 *
 * Typical use:
 * - softening full-color images
 * - blurring sprites or UI textures that contain transparency
 * - preprocessing before compositing or stylization
 *
 * @param[out] des_u32 Destination image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 * @param[in]  kernel_size Odd kernel size greater than 2.
 */
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
            if (a > 0) MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(MAT2D_AT(conv_r, i, j) / a * 255, MAT2D_AT(conv_g, i, j) / a * 255, MAT2D_AT(conv_b, i, j) / a * 255, a);
            else MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(0, 0, 0, 0);
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

/**
 * @brief Apply a full 2D Gaussian blur to a grayscale version of the image.
 *
 * This function converts the source image to grayscale, builds a 2D Gaussian
 * kernel from the supplied standard deviation, normalizes it, and convolves the
 * image with that kernel.
 *
 * The Gaussian kernel size is chosen automatically as:
 * `2 * ceil(3 * std) + 1`
 * which retains almost all of the Gaussian mass.
 *
 * The output is grayscale in RGB, while the original alpha channel is
 * preserved.
 *
 * This is the more direct 2D implementation and is typically slower than the
 * separable "fast" variant.
 *
 * Typical use:
 * - high-quality grayscale smoothing
 * - denoising before edge detection
 * - building blurred images for unsharp masking experiments
 *
 * @param[out] des_u32 Destination image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 * @param[in]  std Standard deviation of the Gaussian. Must be greater than 0.
 */
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

/**
 * @brief Apply a separable Gaussian blur to a grayscale version of the image.
 *
 * This function is an optimized grayscale Gaussian blur. Instead of using a
 * full 2D kernel, it builds a 1D Gaussian kernel and applies it in two passes:
 * one horizontal pass followed by one vertical pass.
 *
 * Because the Gaussian is separable, this produces the same conceptual result
 * as a 2D Gaussian blur with much lower computational cost.
 *
 * Border handling here is clamped: samples outside the image are replaced by
 * the nearest valid pixel.
 *
 * The output is grayscale in RGB, while the original alpha channel is
 * preserved.
 *
 * Typical use:
 * - fast grayscale smoothing
 * - preprocessing in real-time or repeated pipelines
 * - a better default choice than the full 2D version when performance matters
 *
 * @param[out] des_u32 Destination image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 * @param[in]  std Standard deviation of the Gaussian. Must be greater than 0.
 */
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

/**
 * @brief Apply a separable Gaussian blur to an RGBA image.
 *
 * This function performs an optimized Gaussian blur on a color image with
 * transparency by processing the image in premultiplied-alpha form. A 1D
 * Gaussian kernel is applied horizontally and vertically to each channel.
 *
 * This implementation is intended for efficient blur in color pipelines where
 * preserving sensible behavior around transparent edges is important.
 *
 * Border handling is clamped to the nearest valid pixel.
 *
 * Typical use:
 * - fast soft blur for color images
 * - blurring textures, sprites, and translucent images
 * - pre-blur stage for bloom, glow, or soft UI effects
 *
 * @param[out] des_u32 Destination image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 * @param[in]  std Standard deviation of the Gaussian. Must be greater than 0.
 */
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
            if (a > 0) MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(MAT2D_AT(conv_r, i, j) / a * 255, MAT2D_AT(conv_g, i, j) / a * 255, MAT2D_AT(conv_b, i, j) / a * 255, a);
            else MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(0, 0, 0, 0);
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

/**
 * @brief Build the 1D smoothing and derivative vectors used for a generalized
 * Sobel operator.
 *
 * This helper constructs a pair of 1D vectors of equal odd length:
 * - `smooth`: a binomial row used as the smoothing component
 * - `deriv`: a derivative-like vector derived from a smaller binomial row
 *
 * These vectors are then typically combined by outer products to form 2D Sobel
 * kernels.
 *
 * Typical use:
 * - internal support for generalized Sobel kernel generation
 * - custom derivative filters for image analysis
 *
 * @param[out] smooth A row or column vector that receives the smoothing
 *                    coefficients.
 * @param[out] deriv A row or column vector that receives the derivative
 *                   coefficients.
 */
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

/**
 * @brief Build generalized 2D Sobel kernels of arbitrary odd size.
 *
 * This function constructs horizontal and vertical Sobel-like kernels of size
 * `kernel_size x kernel_size` using the 1D smoothing and derivative vectors
 * produced by `aim_build_sobel_1d()`.
 *
 * The resulting kernels can be used to estimate horizontal and vertical image
 * gradients at scales larger than the standard 3x3 Sobel operator.
 *
 * Typical use:
 * - internal support for generalized Sobel edge detection
 * - custom edge detectors where a larger support region is desired
 *
 * @param[out] gx Output horizontal-gradient kernel.
 * @param[out] gy Output vertical-gradient kernel.
 * @param[in]  kernel_size Odd kernel size greater than or equal to 3.
 */
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

/**
 * @brief Detect edges using the 3x3 Scharr operator.
 *
 * This function converts the source image to grayscale, computes horizontal and
 * vertical gradients with the Scharr 3x3 kernels, combines them using gradient
 * magnitude, normalizes the result, and writes a grayscale edge image.
 *
 * Scharr is often preferred over Sobel for 3x3 kernels because it improves
 * rotational symmetry and can produce stronger, cleaner gradients.
 *
 * The alpha channel is preserved from the original image.
 *
 * Typical use:
 * - extracting a grayscale edge map
 * - emphasizing contours for later segmentation or stylization
 * - replacing 3x3 Sobel when better small-kernel gradient quality is desired
 *
 * @param[out] des_u32 Destination edge image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 */
AIM_DEF void aim_edge_detection_scharr_3x3(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32)
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

/**
 * @brief Detect edges using the classic 3x3 Sobel operator.
 *
 * This function converts the source image to grayscale, convolves it with the
 * standard 3x3 Sobel horizontal and vertical kernels, computes gradient
 * magnitude, normalizes the result to the range used by the implementation, and
 * writes a grayscale edge map.
 *
 * The output highlights areas of strong intensity change. The alpha channel is
 * preserved from the source.
 *
 * Typical use:
 * - simple edge extraction
 * - feature preprocessing
 * - visualizing image gradients
 *
 * @param[out] des_u32 Destination edge image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 */
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

/**
 * @brief Detect edges using the 3x3 Sobel operator and clamp strong responses.
 *
 * This function performs the same processing as `aim_edge_detection_sobel_3x3()`
 * but additionally caps gradient magnitudes at `cutoff`, then renormalizes the
 * result.
 *
 * This can reduce domination by a few extremely strong edges and make weaker
 * edges more visible after normalization.
 *
 * Typical use:
 * - edge maps where very strong edges would otherwise wash out subtle ones
 * - stylized edge extraction with controlled contrast
 *
 * @param[out] des_u32 Destination edge image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 * @param[in]  cutoff Maximum allowed post-normalization edge value before the
 *                    second renormalization step.
 */
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

/**
 * @brief Detect edges using a fixed 5x5 Sobel-like operator.
 *
 * This function uses larger hard-coded Sobel-style kernels to compute image
 * gradients on a broader neighborhood than the 3x3 version. Larger kernels are
 * generally less sensitive to very small noise but produce thicker, smoother
 * edge responses.
 *
 * The source image is first converted to grayscale, then convolved with the
 * horizontal and vertical kernels. The gradient magnitude is normalized and
 * stored as grayscale output while preserving source alpha.
 *
 * Typical use:
 * - edge detection with more spatial smoothing than 3x3 Sobel
 * - extracting broader, less noisy edge structures
 *
 * @param[out] des_u32 Destination edge image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 */
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

/**
 * @brief Detect edges using a 5x5 Sobel-like operator with response clamping.
 *
 * This function behaves like `aim_edge_detection_sobel_5x5()` but clips strong
 * edge responses to `cutoff` before a second normalization step.
 *
 * This is useful when you want to prevent a few dominant edges from controlling
 * the full dynamic range of the output.
 *
 * Typical use:
 * - balanced edge visualization
 * - stylized edge maps with reduced extreme contrast
 *
 * @param[out] des_u32 Destination edge image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 * @param[in]  cutoff Maximum allowed post-normalization edge value before the
 *                    second renormalization step.
 */
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

/**
 * @brief Detect edges using a generalized Sobel operator of arbitrary odd size.
 *
 * This function constructs Sobel-like horizontal and vertical kernels of size
 * `kernel_size x kernel_size`, applies them to a grayscale version of the
 * image, computes gradient magnitude, normalizes the result, and stores it as a
 * grayscale edge image.
 *
 * Compared to fixed-size Sobel, this allows you to tune the spatial scale of
 * edge detection:
 * - smaller kernels react to finer detail
 * - larger kernels produce broader and often smoother edge responses
 *
 * Typical use:
 * - experimenting with custom Sobel scales
 * - edge detection on noisy images where larger support is useful
 * - custom image-analysis pipelines
 *
 * @param[out] des_u32 Destination edge image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 * @param[in]  kernel_size Odd kernel size greater than or equal to 3.
 */
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

/**
 * @brief Detect edges using a generalized Sobel operator and clamp strong
 * responses.
 *
 * This function is the cutoff-enabled version of
 * `aim_edge_detection_sobel_general()`. After computing gradient magnitudes and
 * performing an initial normalization, values above `cutoff` are clipped and
 * the image is renormalized again.
 *
 * This gives more control over edge-map contrast, especially when a few strong
 * gradients dominate the image.
 *
 * Typical use:
 * - generalized multiscale edge detection with dynamic-range control
 * - making weaker edges more visible in the final output
 *
 * @param[out] des_u32 Destination edge image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 * @param[in]  kernel_size Odd kernel size greater than or equal to 3.
 * @param[in]  cutoff Maximum allowed post-normalization edge value before the
 *                    second renormalization step.
 */
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

/**
 * @brief Fill a row or column vector with binomial coefficients.
 *
 * This helper populates a 1D `Mat2D` vector with the coefficients of a binomial
 * row, equivalent to one row of Pascal's triangle.
 *
 * For example, a vector of length 5 becomes:
 * `1, 4, 6, 4, 1`
 *
 * These coefficients are commonly used for smoothing kernels and are also used
 * in this library to construct generalized Sobel operators.
 *
 * Typical use:
 * - generating smoothing weights
 * - building generalized derivative filters
 *
 * @param[out] v A row or column vector that will be filled with binomial
 *               coefficients.
 */
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

/**
 * @brief Apply a median filter to a grayscale version of the image.
 *
 * This function converts the source image to grayscale and applies a median
 * filter over a square neighborhood of size `kernel_size x kernel_size`.
 *
 * In interior pixels, the median of the neighborhood is used. Near borders,
 * where the padded region contains zeros, the function falls back to averaging
 * non-zero entries instead of taking a direct median.
 *
 * Median filters are particularly effective at removing impulsive
 * salt-and-pepper noise while preserving edges better than linear blurs.
 *
 * The output RGB channels are grayscale; the source alpha channel is preserved.
 *
 * Typical use:
 * - denoising scanned or thresholded images
 * - removing isolated outlier pixels
 * - preprocessing before segmentation or edge detection
 *
 * @param[out] des_u32 Destination image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 * @param[in]  kernel_size Odd kernel size greater than 2.
 */
AIM_DEF void aim_median_filter_bw(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size)
{
    /* https://en.wikipedia.org/wiki/Median_filter */

    MAT2D_ASSERT(des_u32.cols == src_u32.cols);
    MAT2D_ASSERT(des_u32.rows == src_u32.rows);
    MAT2D_ASSERT(kernel_size > 2);
    MAT2D_ASSERT(kernel_size % 2);

    Mat2D src = mat2D_alloc(src_u32.rows + (kernel_size / 2) * 2, src_u32.cols + (kernel_size / 2) * 2);
    Mat2D conv = mat2D_alloc(src_u32.rows, src_u32.cols);

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

    Mat2D window = {0};
    for (size_t i = 0; i < conv.rows; i++) {
        for (size_t j = 0; j < conv.cols; j++) {
            if (i < kernel_size / 2 || i >= conv.rows - kernel_size / 2 || j < kernel_size / 2|| j >= conv.cols - kernel_size / 2) {
                window.elements = &MAT2D_AT(src, i, j);
                window.cols = kernel_size;
                window.rows = kernel_size;
                window.stride_r = src.stride_r;
                mat2D_real sum = mat2D_elements_sum(window);
                MAT2D_AT(conv, i, j) = sum / mat2D_non_zero_entrys_count(window);
            } else {
                window.elements = &MAT2D_AT(src, i, j);
                window.cols = kernel_size;
                window.rows = kernel_size;
                window.stride_r = src.stride_r;

                MAT2D_AT(conv, i, j) = mat2D_median_element(window);
            }
        }
    }

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
}

/**
 * @brief Apply a median filter to an RGBA image.
 *
 * This function performs median filtering independently on alpha-aware color
 * channels and alpha itself. RGB channels are handled in premultiplied-alpha
 * style, and the final color is reconstructed by dividing by the filtered alpha.
 *
 * As in the grayscale version, border pixels use an average of non-zero
 * entries rather than a strict median over zero-padded neighborhoods.
 *
 * This function is useful for removing speckle or impulse noise in color images
 * while generally preserving edge locations better than blur-based smoothing.
 *
 * Typical use:
 * - denoising full-color images with transparency
 * - reducing isolated artifacts in sprites or texture atlases
 *
 * @param[out] des_u32 Destination image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 * @param[in]  kernel_size Odd kernel size greater than 2.
 */
AIM_DEF void aim_median_filter_rgba(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, size_t kernel_size)
{
    /* https://en.wikipedia.org/wiki/Median_filter */

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

    Mat2D window = {0};
    mat2D_real sum = 0;
    for (size_t i = 0; i < conv_r.rows; i++) {
        for (size_t j = 0; j < conv_r.cols; j++) {
            if (i < kernel_size / 2 || i >= conv_r.rows - kernel_size / 2 || j < kernel_size / 2|| j >= conv_r.cols - kernel_size / 2) {
                /*r*/
                window.elements = &MAT2D_AT(src_r, i, j);
                window.cols = kernel_size;
                window.rows = kernel_size;
                window.stride_r = src_r.stride_r;
                sum = mat2D_elements_sum(window);
                MAT2D_AT(conv_r, i, j) = sum / mat2D_non_zero_entrys_count(window);
                /*g*/
                window.elements = &MAT2D_AT(src_g, i, j);
                window.cols = kernel_size;
                window.rows = kernel_size;
                window.stride_r = src_g.stride_r;
                sum = mat2D_elements_sum(window);
                MAT2D_AT(conv_g, i, j) = sum / mat2D_non_zero_entrys_count(window);
                /*a*/
                window.elements = &MAT2D_AT(src_a, i, j);
                window.cols = kernel_size;
                window.rows = kernel_size;
                window.stride_r = src_a.stride_r;
                sum = mat2D_elements_sum(window);
                MAT2D_AT(conv_a, i, j) = sum / mat2D_non_zero_entrys_count(window);
                /*b*/
                window.elements = &MAT2D_AT(src_b, i, j);
                window.cols = kernel_size;
                window.rows = kernel_size;
                window.stride_r = src_b.stride_r;
                sum = mat2D_elements_sum(window);
                MAT2D_AT(conv_b, i, j) = sum / mat2D_non_zero_entrys_count(window);
            } else {
                /*r*/
                window.elements = &MAT2D_AT(src_r, i, j);
                window.cols = kernel_size;
                window.rows = kernel_size;
                window.stride_r = src_r.stride_r;
                MAT2D_AT(conv_r, i, j) = mat2D_median_element(window);
                /*g*/
                window.elements = &MAT2D_AT(src_g, i, j);
                window.cols = kernel_size;
                window.rows = kernel_size;
                window.stride_r = src_g.stride_r;
                MAT2D_AT(conv_g, i, j) = mat2D_median_element(window);
                /*b*/
                window.elements = &MAT2D_AT(src_b, i, j);
                window.cols = kernel_size;
                window.rows = kernel_size;
                window.stride_r = src_b.stride_r;
                MAT2D_AT(conv_b, i, j) = mat2D_median_element(window);
                /*a*/
                window.elements = &MAT2D_AT(src_a, i, j);
                window.cols = kernel_size;
                window.rows = kernel_size;
                window.stride_r = src_a.stride_r;
                MAT2D_AT(conv_a, i, j) = mat2D_median_element(window);
            }
        }
    }

    for (size_t i = 0; i < des_u32.rows; i++) {
        for (size_t j = 0; j < des_u32.cols; j++) {
            mat2D_real a = MAT2D_AT(conv_a, i, j);
            if (a > 0) MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(MAT2D_AT(conv_r, i, j) / a * 255, MAT2D_AT(conv_g, i, j) / a * 255, MAT2D_AT(conv_b, i, j) / a * 255, a);
            else MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(0, 0, 0, 0);
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
}

/**
 * @brief Sharpen a grayscale version of the image using unsharp masking.
 *
 * This function converts the source to grayscale and builds a sharpening kernel
 * from:
 * - a Gaussian blur kernel controlled by `std`
 * - an identity kernel
 * - a user-controlled sharpening factor `amount`
 *
 * Conceptually, the function enhances detail by subtracting a blurred version
 * of the image from the original and adding the difference back in.
 *
 * The output is grayscale in RGB, while the original alpha channel is
 * preserved.
 *
 * Typical use:
 * - restoring local contrast after blur
 * - making edges and textures more pronounced
 * - grayscale enhancement for analysis or stylization
 *
 * @param[out] des_u32 Destination image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 * @param[in]  std Standard deviation used for the Gaussian blur component.
 * @param[in]  amount Sharpening strength. Larger values increase edge emphasis.
 */
AIM_DEF void aim_sharpen_bw(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real std, mat2D_real amount)
{
    /**
     * https://blog.demofox.org/2022/02/26/image-sharpening-convolution-kernels/
     * https://en.wikipedia.org/wiki/Unsharp_masking
     */

    MAT2D_ASSERT(des_u32.cols == src_u32.cols);
    MAT2D_ASSERT(des_u32.rows == src_u32.rows);

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
    Mat2D blur_kernel = mat2D_alloc(kernel_size, kernel_size);
    Mat2D original_kernel = mat2D_alloc(kernel_size, kernel_size);

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

    for (size_t r = 0; r < blur_kernel.rows; r++) {
        for (size_t c = 0; c < blur_kernel.cols; c++) {
            int x = (int)c - (int)kernel_size / 2;
            int y = (int)r - (int)kernel_size / 2;
            MAT2D_AT(blur_kernel, r, c) = (1 / (2 * MAT2D_PI * std * std)) * mat2D_exp(-1 * (x * x + y * y) / (2 * std * std));
        }
    }
    mat2D_mult(blur_kernel, 1 / mat2D_elements_sum(blur_kernel));

    mat2D_fill(original_kernel, 0);
    MAT2D_AT(original_kernel, original_kernel.rows/2, original_kernel.cols/2) = 1;

    mat2D_mult(original_kernel, 1 + amount);
    mat2D_copy(kernel, original_kernel);
    mat2D_mult(blur_kernel, -amount);
    mat2D_add(kernel, blur_kernel);

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
    mat2D_free(blur_kernel);
    mat2D_free(original_kernel);
}

/**
 * @brief Sharpen an RGBA image using unsharp masking.
 *
 * This function performs alpha-aware sharpening on a color image. It builds a
 * sharpening kernel from an identity kernel and a Gaussian blur kernel, then
 * applies that kernel to premultiplied-alpha color channels and alpha.
 *
 * The result enhances local contrast while handling transparency more safely
 * than naïve per-channel sharpening.
 *
 * Typical use:
 * - increasing apparent detail in color images
 * - sharpening blurred textures or sprites
 * - post-processing after soft blur or resizing
 *
 * @param[out] des_u32 Destination image. Must have the same dimensions as
 *                     `src_u32`.
 * @param[in]  src_u32 Source image in packed 32-bit ARGB format.
 * @param[in]  std Standard deviation used for the Gaussian blur component.
 * @param[in]  amount Sharpening strength. Larger values increase edge emphasis.
 */
AIM_DEF void aim_sharpen_rgba(Mat2D_uint32 des_u32, Mat2D_uint32 src_u32, mat2D_real std, mat2D_real amount)
{
    /**
     * https://blog.demofox.org/2022/02/26/image-sharpening-convolution-kernels/
     * https://en.wikipedia.org/wiki/Unsharp_masking
     */

    MAT2D_ASSERT(des_u32.cols == src_u32.cols);
    MAT2D_ASSERT(des_u32.rows == src_u32.rows);

    size_t kernel_size = (size_t)(2 * mat2D_ceil(3 * std) + 1);
    /**
     * For a 1D Gaussian, about:
     * - 68.27% of the mass lies within 1 * std
     * - 95.45% within 2 * std
     * - 99.73% within 3 * std
     * So if you cut the kernel at radius 3 * std, you keep almost all of the Gaussian.
     */

    Mat2D src_r = mat2D_alloc(src_u32.rows + (kernel_size / 2) * 2, src_u32.cols + (kernel_size / 2) * 2);
    Mat2D src_g = mat2D_alloc(src_u32.rows + (kernel_size / 2) * 2, src_u32.cols + (kernel_size / 2) * 2);
    Mat2D src_b = mat2D_alloc(src_u32.rows + (kernel_size / 2) * 2, src_u32.cols + (kernel_size / 2) * 2);
    Mat2D src_a = mat2D_alloc(src_u32.rows + (kernel_size / 2) * 2, src_u32.cols + (kernel_size / 2) * 2);
    Mat2D conv_r = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_g = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_b = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D conv_a = mat2D_alloc(src_u32.rows, src_u32.cols);
    Mat2D kernel = mat2D_alloc(kernel_size, kernel_size);
    Mat2D blur_kernel = mat2D_alloc(kernel_size, kernel_size);
    Mat2D original_kernel = mat2D_alloc(kernel_size, kernel_size);

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

    for (size_t r = 0; r < blur_kernel.rows; r++) {
        for (size_t c = 0; c < blur_kernel.cols; c++) {
            int x = (int)c - (int)kernel_size / 2;
            int y = (int)r - (int)kernel_size / 2;
            MAT2D_AT(blur_kernel, r, c) = (1 / (2 * MAT2D_PI * std * std)) * mat2D_exp(-1 * (x * x + y * y) / (2 * std * std));
        }
    }
    mat2D_mult(blur_kernel, 1 / mat2D_elements_sum(blur_kernel));

    mat2D_fill(original_kernel, 0);
    MAT2D_AT(original_kernel, original_kernel.rows/2, original_kernel.cols/2) = 1;

    mat2D_mult(original_kernel, 1 + amount);
    mat2D_copy(kernel, original_kernel);
    mat2D_mult(blur_kernel, -amount);
    mat2D_add(kernel, blur_kernel);

    mat2D_convolve(conv_r, src_r, kernel);
    mat2D_convolve(conv_g, src_g, kernel);
    mat2D_convolve(conv_b, src_b, kernel);
    mat2D_convolve(conv_a, src_a, kernel);

    for (size_t i = 0; i < des_u32.rows; i++) {
        for (size_t j = 0; j < des_u32.cols; j++) {
            mat2D_real a = MAT2D_AT(conv_a, i, j);
            if (a > 0) MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(MAT2D_AT(conv_r, i, j) / a * 255, MAT2D_AT(conv_g, i, j) / a * 255, MAT2D_AT(conv_b, i, j) / a * 255, a);
            else MAT2D_AT(des_u32, i, j) = APNG_RGBA_TO_hexARGB(0, 0, 0, 0);
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
    mat2D_free(blur_kernel);
    mat2D_free(original_kernel);
}




#endif /*ALMOG_IMAGE_MANIPULATION_IMPLEMENTATION*/
