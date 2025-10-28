/* */
/**
 * @file
 * @brief Single-header 3D matrix (tensor) utilities for contiguous float data.
 *
 * @details
 * Matrix3D is a minimal, dependency-free C library for working with dense 3D
 * arrays (tensors) of floats. It provides allocation, initialization, random
 * filling, element-wise addition, scaling, identity construction, copying, and
 * pretty-printing.
 *
 * Memory layout is row-major with the following linearization:
 * index(i, j, k) = ((i * cols) + j) * layers + k
 * - i in [0, rows)
 * - j in [0, cols)
 * - k in [0, layers)
 * - k is the fastest-varying dimension.
 *
 * Usage:
 * - In exactly one translation unit, define MATRIX3D_IMPLEMENTATION before
 *   including this header to get the function definitions.
 * - In all other translation units, include this header without the define.
 *
 * Example:
 * @code
 * #define MATRIX3D_IMPLEMENTATION
 * #include "matrix3d.h"
 *
 * int main(void) {
 *     Mat3D m = mat3D_alloc(2, 3, 4);
 *     mat3D_fill(m, 0.0f);
 *     MAT3D_AT(m, 1, 2, 3) = 42.0f;
 *     MAT3D_PRINT(m);
 *     mat3D_free(m);
 *     return 0;
 * }
 * @endcode
 *
 * Configuration:
 * - Define MATRIX3D_MALLOC to override the allocator used by mat3D_alloc.
 * - Define MATRIX3D_ASSERT to override assertions.
 *
 * Thread-safety:
 * - All functions operate on caller-provided objects. There is no internal
 *   global state other than the use of C's rand() in mat3D_rand_float and
 *   mat3D_rand, which is not thread-safe. Seed with srand() as needed.
 *
 * Error handling:
 * - By default, allocation failure triggers MATRIX3D_ASSERT. If you need
 *   graceful handling, override MATRIX3D_ASSERT.
 * 
 * @note This one-file library is heavily inspired by Tsoding's nn.h
 *       implementation of matrix creation and operation.
 *       you can find the source code in: https://github.com/tsoding/nn.h .
 *       featured in this video of his: https://youtu.be/L1TbWe8bVOc?list=PLpM-Dvs8t0VZPZKggcql-MmjaBdZKeDMw .
 */

#ifndef MATRIX3D_H_
#define MATRIX3D_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @def MATRIX3D_MALLOC
 * @brief Allocation function used by mat3D_alloc.
 *
 * @details
 * Defaults to malloc. You may override this macro before including the header
 * to provide a custom allocator (e.g., an arena or tracking allocator).
 * The memory must be free()'able by mat3D_free (which calls free()) unless
 * you also replace mat3D_free.
 */
#ifndef MATRIX3D_MALLOC
#define MATRIX3D_MALLOC malloc
#endif //MATRIX3D_MALLOC

/**
 * @def MATRIX3D_ASSERT
 * @brief Assertion macro used for parameter and allocation checks.
 *
 * @details
 * Defaults to assert. You may override it to integrate with your project's
 * error handling or to disable assertions in production builds.
 */
#ifndef MATRIX3D_ASSERT
#include <assert.h>
#define MATRIX3D_ASSERT assert
#endif //MATRIX3D_ASSERT

/**
 * @brief Contiguous 3D float matrix (tensor).
 *
 * @details
 * Elements are stored in a single contiguous allocation of
 * rows * cols * layers floats using the layout:
 * ((i * cols) + j) * layers + k
 */
typedef struct {
    /**
     * @brief Number of rows (size of the i dimension).
     */
    size_t rows;
    /**
     * @brief Number of columns (size of the j dimension).
     */
    size_t cols;
    /**
     * @brief Number of layers (size of the k dimension).
     */
    size_t layers;
    /**
     * @brief Row stride in elements.
     *
     * @details
     * For this layout, stride == cols * layers. Exposed for convenience and
     * potential future use. Not required by the provided API.
     */
    size_t stride;
    /**
     * @brief Pointer to the contiguous array of elements.
     */
    float *elements;
    
} Mat3D;

/**
 * @def MAT3D_AT(m, i, j, k)
 * @brief Lvalue (Locator value) access to element at (i, j, k).
 *
 * @param m Mat3D lvalue or expression
 * @param i Row index in [0, m.rows)
 * @param j Column index in [0, m.cols)
 * @param k Layer index in [0, m.layers)
 *
 * @return An lvalue reference to the element at (i, j, k).
 *
 * @warning The macro evaluates its arguments multiple times. Avoid passing
 * expressions with side effects.
 */
#define MAT3D_AT(m, i, j, k) (m).elements[((i)*(m).cols + (j))*(m).layers + (k)]
/**
 * @def MAT3D_PRINT(m)
 * @brief Convenience macro to print a Mat3D with its variable name and no
 * left padding.
 *
 * @param m Mat3D to print.
 */
#define MAT3D_PRINT(m) mat3D_print(m, #m, 0)

float mat3D_rand_float(void);
Mat3D mat3D_alloc(size_t rows, size_t cols, size_t layers);
void mat3D_free(Mat3D m);
void mat3D_fill(Mat3D m, float x);
void mat3D_rand(Mat3D m, float low, float high);
void mat3D_sum(Mat3D dst, Mat3D a);
void mat3D_mult(Mat3D m, float factor);
void mat3D_print(Mat3D m, const char *name, size_t padding);
void mat3D_identity_mat(Mat3D m);
void mat3D_copy(Mat3D res, Mat3D src);

#endif // MATRIX3D_H_

#ifdef MATRIX3D_IMPLEMENTATION
#undef MATRIX3D_IMPLEMENTATION

/**
 * @brief Generate a pseudo-random float in [0, 1].
 *
 * @details
 * Uses C's rand() scaled to [0, 1]. The random stream is shared with the
 * process; call srand() if you want a specific seed.
 *
 * @return Random float in [0, 1].
 *
 * @note Not thread-safe due to rand().
 */
float mat3D_rand_float(void)
{
    return (float) rand() / (float) RAND_MAX;
}

/**
 * @brief Allocate a 3D matrix with the given shape.
 *
 * @param rows Number of rows (i dimension).
 * @param cols Number of columns (j dimension).
 * @param layers Number of layers (k dimension).
 *
 * @return Mat3D with allocated elements and initialized metadata.
 *
 * @pre rows * cols * layers > 0. Behavior is undefined for zero-sized
 * allocations due to assertion on NULL pointers.
 * @post m.elements is non-NULL on success (asserts otherwise).
 * @note Elements are uninitialized.
 * @see mat3D_free
 */
Mat3D mat3D_alloc(size_t rows, size_t cols, size_t layers)
{
    Mat3D m;
    m.rows = rows;
    m.cols = cols;
    m.layers = layers; 
    m.stride = cols * layers;
    m.elements = (float*)MATRIX3D_MALLOC(sizeof(*m.elements)*rows*cols*layers);
    MATRIX3D_ASSERT(m.elements != NULL);
    return m;    
}

/**
 * @brief Free the memory owned by a Mat3D.
 *
 * @param m Mat3D to free.
 *
 * @post m.elements becomes invalid after this call.
 * @note Safe to call with m.elements == NULL (free(NULL) is a no-op).
 * @warning Do not double-free the same Mat3D.
 */
void mat3D_free(Mat3D m)
{
    free(m.elements);
}

/**
 * @brief Fill all elements with a constant value.
 *
 * @param m Destination matrix.
 * @param x Value to assign to each element.
 */
void mat3D_fill(Mat3D m, float x)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            for (size_t k = 0; k < m.layers; ++k) {
                MAT3D_AT(m, i, j, k) = x;
            }
        }
    }
}

/**
 * @brief Fill all elements with random values in [low, high].
 *
 * @param m Destination matrix.
 * @param low Lower bound (inclusive).
 * @param high Upper bound (exclusive when using typical rand()).
 *
 * @note Uses mat3D_rand_float and therefore rand(); not thread-safe.
 */
void mat3D_rand(Mat3D m, float low, float high)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            for (size_t k = 0; k < m.layers; ++k) {
                MAT3D_AT(m, i, j, k) = mat3D_rand_float()*(high - low) + low;
            }
        }
    }
}

/**
 * @brief Element-wise addition: dst += a.
 *
 * @param dst Destination matrix; also the left operand.
 * @param a Right operand; must have the same shape as dst.
 *
 * @pre dst.rows == a.rows, dst.cols == a.cols, dst.layers == a.layers.
 */
void mat3D_sum(Mat3D dst, Mat3D a)
{
    MATRIX3D_ASSERT(dst.rows == a.rows);
    MATRIX3D_ASSERT(dst.cols == a.cols);
    MATRIX3D_ASSERT(dst.layers == a.layers);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            for (size_t k = 0; k < dst.layers; ++k) {
                MAT3D_AT(dst, i, j, k) += MAT3D_AT(a, i, j, k);
            }
        }
    }
}

/**
 * @brief In-place scaling by an integer factor: m *= factor.
 *
 * @param m Matrix to scale.
 * @param factor Scale factor (size_t). Elements are multiplied as floats.
 */
void mat3D_mult(Mat3D m, float factor)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            for (size_t k = 0; k < m.layers; ++k) {
                MAT3D_AT(m, i, j, k) *= factor;
            }
        }
    }
}

/**
 * @brief Print the matrix with a name and left padding.
 *
 * @param m Matrix to print.
 * @param name A label to print before the data (e.g., the variable name).
 * @param padding Number of spaces of left padding for each printed line.
 *
 * @details
 * The output is organized by layers (k), printing each 2D slice with row-wise
 * lines.
 */
void mat3D_print(Mat3D m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t k = 0; k < m.layers; ++k) {
        printf("%*s    k=%zu\n", (int) padding, "", k);
        for (size_t i = 0; i < m.rows; ++i) {
            printf("%*s    ", (int) padding, "");
            for (size_t j = 0; j < m.cols; ++j) {
                printf("%f ", MAT3D_AT(m, i, j, k));
            }
            printf("\n");
        }

        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < m.cols; ++j) {
            printf("---------");
        }
        printf("\n");
    }
        
    printf("%*s]\n", (int) padding, "");
}

/**
 * @brief Set m to the 3D identity tensor (Kronecker delta).
 *
 * @param m Destination matrix.
 *
 * @pre m.rows == m.cols == m.layers.
 *
 * @details
 * Sets element (i, j, k) to 1 when i == j == k, otherwise 0.
 */
void mat3D_identity_mat(Mat3D m)
{
    MATRIX3D_ASSERT(m.cols == m.rows && m.rows == m.layers);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            for (size_t k = 0; k < m.layers; ++k) {
                if (i == j && j == k) {
                    MAT3D_AT(m, i, j, k) = 1;
                }
                else {
                    MAT3D_AT(m, i, j, k) = 0;
                }
            }
        }
    }
}

/**
 * @brief Copy all elements from src to res.
 *
 * @param res Destination matrix.
 * @param src Source matrix.
 *
 * @pre res.rows == src.rows, res.cols == src.cols, res.layers == src.layers.
 */
void mat3D_copy(Mat3D res, Mat3D src)
{
    MATRIX3D_ASSERT(res.cols == src.cols);
    MATRIX3D_ASSERT(res.rows == src.rows);
    MATRIX3D_ASSERT(res.layers == src.layers);

    for (size_t i = 0; i < res.rows; ++i) {
        for (size_t j = 0; j < res.cols; ++j) {
            for (size_t k = 0; k < res.layers; ++k) {
                MAT3D_AT(res, i, j, k) = MAT3D_AT(src, i, j, k);
            }
        }
    }
}

#endif // MATRIX3D_IMPLEMENTATION
