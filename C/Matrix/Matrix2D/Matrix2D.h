/**
 * @file
 * @brief Lightweight 2D matrix helpers (double / uint32_t).
 *
 * @details
 * This single-header module provides small utilities for dense row-major
 * matrices:
 *  - Allocation/free for Mat2D (double) and Mat2D_uint32
 *  - Basic arithmetic and row/column operations
 *  - Matrix multiplication, transpose, dot and cross products
 *  - Determinant and inversion (Gaussian / Gauss-Jordan style)
 *  - A simple LUP decomposition helper and a linear system solver
 *  - Rotation matrix helpers (X/Y/Z) and a Z-Y-X DCM builder (as implemented)
 *  - “Minor” views (index lists into a reference matrix) for educational
 *    determinant-by-minors computation
 *
 * Storage model
 *  - Matrices are dense and row-major (C-style).
 *  - Element at row i and column j (0-based) is:
 *      elements[i * stride_r + j]
 *  - For matrices created by mat2D_alloc(), stride_r == cols.
 *
 * Usage
 *  - In exactly one translation unit, define MATRIX2D_IMPLEMENTATION before
 *    including this header to compile the implementation.
 *  - In all other files, include the header without that macro to get
 *    declarations only.
 *
 * Example:
 *   #define MATRIX2D_IMPLEMENTATION
 *   #include "matrix2d.h"
 *
 *
 * Notes and limitations
 *  - This one-file library is heavily inspired by Tsoding's nn.h
 *    implementation of matrix creation and operations:
 *    https://github.com/tsoding/nn.h and the video:
 *    https://youtu.be/L1TbWe8bVOc?list=PLpM-Dvs8t0VZPZKggcql-MmjaBdZKeDMw
 *  - All APIs assume the caller provides correctly-sized destination matrices.
 *    Shape mismatches are checked with MAT2D_ASSERT in many routines.
 *  - This library does not try to be numerically robust:
 *      - Pivoting is limited (only performed when a pivot is “near zero” per
 *        MAT2D_EPS in several routines).
 *      - Ill-conditioned matrices may produce inaccurate determinants/inverses.
 *  - RNG uses C rand(); it is not cryptographically secure.
 *
 * @warning Numerical stability and correctness
 *  - mat2D_minor_det() is factorial-time and is intended only for very small
 *    matrices (educational use).
 *  - mat2D_invert() uses Gauss-Jordan elimination and may be unstable for
 *    ill-conditioned matrices. Consider a more robust decomposition for
 *    production use (full pivoting / QR / SVD).
 *  - Several routines do not guard against aliasing (e.g. dst == a). Unless
 *    documented otherwise, assume inputs and outputs must not overlap.
 */

#ifndef MATRIX2D_H_
#define MATRIX2D_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/**
 * @def MAT2D_MALLOC
 * @brief Allocation function used by this library.
 *
 * @details
 * Defaults to malloc(). Override by defining MAT2D_MALLOC before including
 * this header to use a custom allocator.
 */
#ifndef MAT2D_MALLOC
#define MAT2D_MALLOC malloc
#endif //MAT2D_MALLOC

/**
 * @def MAT2D_FREE
 * @brief Deallocation function used by this library.
 *
 * @details
 * Defaults to free(). Override by defining MAT2D_FREE before including this
 * header to match a custom allocator.
 */
#ifndef MAT2D_FREE
#define MAT2D_FREE free
#endif //MAT2D_FREE

/**
 * @def MAT2D_ASSERT
 * @brief Assertion macro used by this library for parameter validation.
 *
 * @details
 * Defaults to assert(). Override by defining MAT2D_ASSERT before including
 * this header to customize validation behavior.
 */
#ifndef MAT2D_ASSERT
#include <assert.h>
#define MAT2D_ASSERT assert
#endif //MAT2D_ASSERT

/**
 * @brief Dense row-major matrix of double.
 *
 * @details
 *  - rows     Number of rows (height).
 *  - cols     Number of columns (width).
 *  - stride_r Number of elements between successive rows in memory.
 *            For contiguous storage, stride_r == cols.
 *  - elements Pointer to a contiguous buffer of rows * cols doubles.
 *
 * @note This type is a shallow handle; copying Mat2D copies the pointer, not
 *       the underlying data.
 */
typedef struct {
    size_t rows;
    size_t cols;
    size_t stride_r; /* elements to traverse to reach the next row */
    double *elements;
} Mat2D;

/**
 * @brief Dense row-major matrix of uint32_t.
 *
 * @details
 * Same layout rules as Mat2D, but with uint32_t elements.
 */
typedef struct {
    size_t rows;
    size_t cols;
    size_t stride_r; /* elements to traverse to reach the next row */
    uint32_t *elements;
} Mat2D_uint32;

/**
 * @brief A minor "view" into a reference matrix.
 *
 * @details
 * Represents a minor by excluding one row and one column of a reference matrix.
 * The minor does not own the reference matrix data; instead it stores two index
 * arrays (rows_list, cols_list) mapping minor coordinates to the reference
 * matrix coordinates.
 *
 * Memory ownership:
 *  - rows_list and cols_list are heap-allocated by the minor allocators and
 *    must be freed with mat2D_minor_free().
 *  - ref_mat.elements is not owned by the minor and must not be freed by
 *    mat2D_minor_free().
 */
typedef struct {
    size_t rows;
    size_t cols;
    size_t stride_r; /* logical stride for the minor shape (not used for access) */
    size_t *rows_list;
    size_t *cols_list;
    Mat2D ref_mat;
} Mat2D_Minor;

/**
 * @def MAT2D_AT(m, i, j)
 * @brief Access element (i, j) of a Mat2D (0-based).
 *
 * @details
 * Expands to row-major indexing using stride_r:
 *   (m).elements[(i) * (m).stride_r + (j)]
 *
 * @warning In the “fast” configuration this macro performs no bounds checking.
 */

/**
 * @def MAT2D_AT_UINT32(m, i, j)
 * @brief Access element (i, j) of a Mat2D_uint32 (0-based).
 *
 * @warning In the “fast” configuration this macro performs no bounds checking.
 */
#if 1
#define MAT2D_AT(m, i, j) (m).elements[mat2D_offset2d((m), (i), (j))]
#define MAT2D_AT_UINT32(m, i, j) (m).elements[mat2D_offset2d_uint32((m), (i), (j))]
#else /* use this macro for batter performance but no assertion */
#define MAT2D_AT(m, i, j) (m).elements[(i) * (m).stride_r + (j)]
#define MAT2D_AT_UINT32(m, i, j) (m).elements[(i) * (m).stride_r + (j)]
#endif

#ifndef MAT2D_PI
    #define MAT2D_PI 3.14159265358979323846
#endif

#ifndef MAT2D_EPS
    #define MAT2D_EPS 1e-15
#endif

/**
 * @def MAT2D_IS_ZERO(x)
 * @brief Test whether a floating-point value is “near zero”.
 *
 * @details
 * Uses fabs(x) < MAT2D_EPS.
 */
#define MAT2D_IS_ZERO(x) (fabs(x) < MAT2D_EPS)

/**
 * @def MAT2D_MINOR_AT(mm, i, j)
 * @brief Access element (i, j) of a Mat2D_Minor (0-based).
 *
 * @details
 * Dereferences into the underlying reference matrix using rows_list/cols_list.
 */
#define MAT2D_MINOR_AT(mm, i, j) MAT2D_AT((mm).ref_mat, (mm).rows_list[i], (mm).cols_list[j])

/**
 * @def MAT2D_PRINT
 * @brief Convenience macro to print a matrix with its variable name.
 */
#define MAT2D_PRINT(m) mat2D_print(m, #m, 0)

/**
 * @def MAT2D_PRINT_AS_COL
 * @brief Convenience macro to print a matrix as a single column with its name.
 */
#define MAT2D_PRINT_AS_COL(m) mat2D_print_as_col(m, #m, 0)

/**
 * @def MAT2D_MINOR_PRINT
 * @brief Convenience macro to print a minor with its variable name.
 */
#define MAT2D_MINOR_PRINT(mm) mat2D_minor_print(mm, #mm, 0)

/**
 * @def mat2D_normalize(m)
 * @brief Normalize a matrix in-place to unit Frobenius norm.
 *
 * @details
 * Equivalent to:
 *   m *= 1.0 / mat2D_calc_norma(m)
 *
 * @warning If the Frobenius norm is 0, this performs a division by zero.
 */
#define mat2D_normalize(m) mat2D_mult((m), 1.0 / mat2D_calc_norma((m)))

#define mat2D_normalize_inf(m) mat2D_mult((m), 1.0 / mat2D_calc_norma_inf((m)))

#define mat2D_dprintDOUBLE(expr) printf(#expr " = %#g\n", expr)

#define mat2D_dprintSIZE_T(expr) printf(#expr " = %zu\n", expr)

double          mat2D_rand_double(void);

Mat2D           mat2D_alloc(size_t rows, size_t cols);
Mat2D_uint32    mat2D_alloc_uint32(size_t rows, size_t cols);
void            mat2D_free(Mat2D m);
void            mat2D_free_uint32(Mat2D_uint32 m);
size_t          mat2D_offset2d(Mat2D m, size_t i, size_t j);
size_t          mat2D_offset2d_uint32(Mat2D_uint32 m, size_t i, size_t j);

void            mat2D_fill(Mat2D m, double x);
void            mat2D_fill_sequence(Mat2D m, double start, double step);
void            mat2D_fill_uint32(Mat2D_uint32 m, uint32_t x);
void            mat2D_rand(Mat2D m, double low, double high);

void            mat2D_dot(Mat2D dst, Mat2D a, Mat2D b);
double          mat2D_dot_product(Mat2D a, Mat2D b);
void            mat2D_cross(Mat2D dst, Mat2D a, Mat2D b);

void            mat2D_add(Mat2D dst, Mat2D a);
void            mat2D_add_row_time_factor_to_row(Mat2D m, size_t des_r, size_t src_r, double factor);

void            mat2D_sub(Mat2D dst, Mat2D a);
void            mat2D_sub_row_time_factor_to_row(Mat2D m, size_t des_r, size_t src_r, double factor);

void            mat2D_mult(Mat2D m, double factor);
void            mat2D_mult_row(Mat2D m, size_t r, double factor);

void            mat2D_print(Mat2D m, const char *name, size_t padding);
void            mat2D_print_as_col(Mat2D m, const char *name, size_t padding);

void            mat2D_set_identity(Mat2D m);
void            mat2D_set_rot_mat_x(Mat2D m, float angle_deg);
void            mat2D_set_rot_mat_y(Mat2D m, float angle_deg);
void            mat2D_set_rot_mat_z(Mat2D m, float angle_deg);
void            mat2D_set_DCM_zyx(Mat2D DCM, float yaw_deg, float pitch_deg, float roll_deg);

void            mat2D_copy(Mat2D des, Mat2D src);
void            mat2D_copy_src_window_to_des(Mat2D des, Mat2D src, size_t is, size_t js, size_t ie, size_t je);
void            mat2D_copy_src_to_des_window(Mat2D des, Mat2D src, size_t is, size_t js, size_t ie, size_t je);

void            mat2D_get_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col);
void            mat2D_add_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col);
void            mat2D_sub_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col);

void            mat2D_swap_rows(Mat2D m, size_t r1, size_t r2);
void            mat2D_get_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row);
void            mat2D_add_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row);
void            mat2D_sub_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row);

double          mat2D_calc_norma(Mat2D m);
double          mat2D_calc_norma_inf(Mat2D m);

bool            mat2D_mat_is_all_digit(Mat2D m, double digit);
bool            mat2D_row_is_all_digit(Mat2D m, double digit, size_t r);
bool            mat2D_col_is_all_digit(Mat2D m, double digit, size_t c);

double          mat2D_det_2x2_mat(Mat2D m);
double          mat2D_upper_triangulate(Mat2D m);
bool            mat2D_find_first_non_zero_value(Mat2D m, size_t r, size_t *non_zero_col);
size_t          mat2D_reduce(Mat2D m);
double          mat2D_det(Mat2D m);
void            mat2D_LUP_decomposition_with_swap(Mat2D src, Mat2D l, Mat2D p, Mat2D u);
void            mat2D_transpose(Mat2D des, Mat2D src);
void            mat2D_invert(Mat2D des, Mat2D src);
void            mat2D_solve_linear_sys_LUP_decomposition(Mat2D A, Mat2D x, Mat2D B);

Mat2D_Minor     mat2D_minor_alloc_fill_from_mat(Mat2D ref_mat, size_t i, size_t j);
Mat2D_Minor     mat2D_minor_alloc_fill_from_mat_minor(Mat2D_Minor ref_mm, size_t i, size_t j);
void            mat2D_minor_free(Mat2D_Minor mm);
void            mat2D_minor_print(Mat2D_Minor mm, const char *name, size_t padding);
double          mat2D_det_2x2_mat_minor(Mat2D_Minor mm);
double          mat2D_minor_det(Mat2D_Minor mm);

#endif // MATRIX2D_H_

#ifdef MATRIX2D_IMPLEMENTATION
#undef MATRIX2D_IMPLEMENTATION


/**
 * @brief Return a pseudo-random double in the range [0, 1].
 *
 * @details
 * Uses rand() / RAND_MAX from the C standard library.
 *
 * @note This RNG is not cryptographically secure and may have weak statistical
 *       properties depending on the platform.
 */
double mat2D_rand_double(void)
{
    return (double) rand() / (double) RAND_MAX;
}

/**
 * @brief Allocate a rows-by-cols matrix of double.
 *
 * @param rows Number of rows. Must be > 0.
 * @param cols Number of columns. Must be > 0.
 * @return A Mat2D owning a contiguous buffer of rows * cols elements.
 *
 * @post The returned matrix has stride_r == cols.
 * @post The returned matrix must be released with mat2D_free().
 *
 * @warning This function asserts allocation success via MAT2D_ASSERT.
 */
Mat2D mat2D_alloc(size_t rows, size_t cols)
{
    Mat2D m;
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (double*)MAT2D_MALLOC(sizeof(double)*rows*cols);
    MAT2D_ASSERT(m.elements != NULL);
    
    return m;
}

/**
 * @brief Allocate a rows-by-cols matrix of uint32_t.
 *
 * @param rows Number of rows. Must be > 0.
 * @param cols Number of columns. Must be > 0.
 * @return A Mat2D_uint32 owning a contiguous buffer of rows * cols elements.
 *
 * @post The returned matrix has stride_r == cols.
 * @post The returned matrix must be released with mat2D_free_uint32().
 *
 * @warning This function asserts allocation success via MAT2D_ASSERT.
 */
Mat2D_uint32 mat2D_alloc_uint32(size_t rows, size_t cols)
{
    Mat2D_uint32 m;
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (uint32_t*)MAT2D_MALLOC(sizeof(uint32_t)*rows*cols);
    MAT2D_ASSERT(m.elements != NULL);
    
    return m;
}

/**
 * @brief Free the buffer owned by a Mat2D.
 *
 * @param m Matrix whose elements were allocated via MAT2D_MALLOC.
 *
 * @note This does not modify @p m (it is passed by value).
 * @note It is safe to call with m.elements == NULL.
 */
void mat2D_free(Mat2D m)
{
    MAT2D_FREE(m.elements);
}

/**
 * @brief Free the buffer owned by a Mat2D_uint32.
 *
 * @param m Matrix whose elements were allocated via MAT2D_MALLOC.
 *
 * @note This does not modify @p m (it is passed by value).
 * @note It is safe to call with m.elements == NULL.
 */
void mat2D_free_uint32(Mat2D_uint32 m)
{
    MAT2D_FREE(m.elements);
}

/**
 * @brief Compute the linear offset of element (i, j) in a Mat2D.
 *
 * @param m Matrix.
 * @param i Row index (0-based).
 * @param j Column index (0-based).
 * @return The linear offset i * stride_r + j.
 *
 * @pre 0 <= i < m.rows and 0 <= j < m.cols (checked by MAT2D_ASSERT).
 */
size_t mat2D_offset2d(Mat2D m, size_t i, size_t j)
{
    MAT2D_ASSERT(i < m.rows && j < m.cols);
    return i * m.stride_r + j;
}

/**
 * @brief Compute the linear offset of element (i, j) in a Mat2D_uint32.
 *
 * @param m Matrix.
 * @param i Row index (0-based).
 * @param j Column index (0-based).
 * @return The linear offset i * stride_r + j.
 *
 * @pre 0 <= i < m.rows and 0 <= j < m.cols (checked by MAT2D_ASSERT).
 */
size_t mat2D_offset2d_uint32(Mat2D_uint32 m, size_t i, size_t j)
{
    MAT2D_ASSERT(i < m.rows && j < m.cols);
    return i * m.stride_r + j;
}

/**
 * @brief Fill all elements of a matrix of doubles with a scalar value.
 * @param m Matrix to fill.
 * @param x Value to assign to every element.
 */
void mat2D_fill(Mat2D m, double x)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT2D_AT(m, i, j) = x;
        }
    }
}

/**
 * @brief Fill a matrix with an arithmetic sequence laid out in row-major order.
 * @param m Matrix to fill.
 * @param start First value in the sequence.
 * @param step Increment between consecutive elements.
 * @details Element at linear index k gets value start + step * k.
 */
void mat2D_fill_sequence(Mat2D m, double start, double step) {
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            MAT2D_AT(m, i, j) = start + step * mat2D_offset2d(m, i, j);
        }
    }
}

/**
 * @brief Fill all elements of a matrix of uint32_t with a scalar value.
 * @param m Matrix to fill.
 * @param x Value to assign to every element.
 */
void mat2D_fill_uint32(Mat2D_uint32 m, uint32_t x)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT2D_AT_UINT32(m, i, j) = x;
        }
    }
}

/**
 * @brief Fill a matrix with pseudo-random doubles in [low, high].
 *
 * @param m Matrix to fill.
 * @param low Lower bound (inclusive).
 * @param high Upper bound (inclusive).
 *
 * @pre high > low (not checked here; caller responsibility).
 * @note Uses mat2D_rand_double() (rand()).
 */
void mat2D_rand(Mat2D m, double low, double high)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT2D_AT(m, i, j) = mat2D_rand_double()*(high - low) + low;
        }
    }
}

/**
 * @brief Matrix product: dst = a * b.
 *
 * @param dst Destination matrix (size a.rows x b.cols).
 * @param a Left matrix (size a.rows x a.cols).
 * @param b Right matrix (size a.cols x b.cols).
 *
 * @pre a.cols == b.rows
 * @pre dst.rows == a.rows
 * @pre dst.cols == b.cols
 *
 * @post dst is fully overwritten.
 *
 * @warning dst must not alias a or b (overlap is not handled).
 */
void mat2D_dot(Mat2D dst, Mat2D a, Mat2D b)
{
    MAT2D_ASSERT(a.cols == b.rows);
    MAT2D_ASSERT(a.rows == dst.rows);
    MAT2D_ASSERT(b.cols == dst.cols);

    size_t i, j, k;

    for (i = 0; i < dst.rows; i++) {
        for (j = 0; j < dst.cols; j++) {
            MAT2D_AT(dst, i, j) = 0;
            for (k = 0; k < a.cols; k++) {
                MAT2D_AT(dst, i, j) += MAT2D_AT(a, i, k)*MAT2D_AT(b, k, j);
            }
        }
    }

}

/**
 * @brief Dot product between two vectors.
 *
 * @param a Vector (shape n x 1 or 1 x n).
 * @param b Vector (same shape as a).
 * @return The scalar dot product sum.
 *
 * @pre a.rows == b.rows and a.cols == b.cols
 * @pre (a.cols == 1 && b.cols == 1) || (a.rows == 1 && b.rows == 1)
 */
double mat2D_dot_product(Mat2D a, Mat2D b)
{
    MAT2D_ASSERT(a.rows == b.rows);
    MAT2D_ASSERT(a.cols == b.cols);
    MAT2D_ASSERT((1 == a.cols && 1 == b.cols) || (1 == a.rows && 1 == b.rows));

    double dot_product = 0;

    if (1 == a.cols) {
        for (size_t i = 0; i < a.rows; i++) {
            dot_product += MAT2D_AT(a, i, 0) * MAT2D_AT(b, i, 0);
        }
    } else {
        for (size_t j = 0; j < a.cols; j++) {
            dot_product += MAT2D_AT(a, 0, j) * MAT2D_AT(b, 0, j);
        }
    }
    
    return dot_product;
}

/**
 * @brief 3D cross product: dst = a x b for 3x1 vectors.
 * @param dst 3x1 destination vector.
 * @param a 3x1 input vector.
 * @param b 3x1 input vector.
 * @pre All matrices have shape 3x1.
 */
void mat2D_cross(Mat2D dst, Mat2D a, Mat2D b)
{
    MAT2D_ASSERT(3 == dst.rows && 1 == dst.cols);
    MAT2D_ASSERT(3 == a.rows && 1 == a.cols);
    MAT2D_ASSERT(3 == b.rows && 1 == b.cols);

    MAT2D_AT(dst, 0, 0) = MAT2D_AT(a, 1, 0) * MAT2D_AT(b, 2, 0) - MAT2D_AT(a, 2, 0) * MAT2D_AT(b, 1, 0);
    MAT2D_AT(dst, 1, 0) = MAT2D_AT(a, 2, 0) * MAT2D_AT(b, 0, 0) - MAT2D_AT(a, 0, 0) * MAT2D_AT(b, 2, 0);
    MAT2D_AT(dst, 2, 0) = MAT2D_AT(a, 0, 0) * MAT2D_AT(b, 1, 0) - MAT2D_AT(a, 1, 0) * MAT2D_AT(b, 0, 0);
}

/**
 * @brief In-place addition: dst += a.
 *
 * @param dst Destination matrix to be incremented.
 * @param a Summand of same shape as dst.
 *
 * @pre dst and a have identical shape.
 */
void mat2D_add(Mat2D dst, Mat2D a)
{
    MAT2D_ASSERT(dst.rows == a.rows);
    MAT2D_ASSERT(dst.cols == a.cols);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            MAT2D_AT(dst, i, j) += MAT2D_AT(a, i, j);
        }
    }
}

/**
 * @brief Row operation: row(des_r) += factor * row(src_r).
 * @param m Matrix.
 * @param des_r Destination row index.
 * @param src_r Source row index.
 * @param factor Scalar multiplier.
 * 
 * @warning Indices are not bounds-checked in this routine.
 */
void mat2D_add_row_time_factor_to_row(Mat2D m, size_t des_r, size_t src_r, double factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        MAT2D_AT(m, des_r, j) += factor * MAT2D_AT(m, src_r, j);
    }
}

/**
 * @brief In-place subtraction: dst -= a.
 * @param dst Destination matrix to be decremented.
 * @param a Subtrahend of same shape as dst.
 *
 * @pre dst and a have identical shape.
 */
void mat2D_sub(Mat2D dst, Mat2D a)
{
    MAT2D_ASSERT(dst.rows == a.rows);
    MAT2D_ASSERT(dst.cols == a.cols);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            MAT2D_AT(dst, i, j) -= MAT2D_AT(a, i, j);
        }
    }
}

/**
 * @brief Row operation: row(des_r) -= factor * row(src_r).
 * @param m Matrix.
 * @param des_r Destination row index.
 * @param src_r Source row index.
 * @param factor Scalar multiplier.
 * 
 * @warning Indices are not bounds-checked in this routine.
 */
void mat2D_sub_row_time_factor_to_row(Mat2D m, size_t des_r, size_t src_r, double factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        MAT2D_AT(m, des_r, j) -= factor * MAT2D_AT(m, src_r, j);
    }
}

/**
 * @brief In-place scalar multiplication: m *= factor.
 * @param m Matrix.
 * @param factor Scalar multiplier.
 */
void mat2D_mult(Mat2D m, double factor)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT2D_AT(m, i, j) *= factor;
        }
    }
}

/**
 * @brief In-place row scaling: row(r) *= factor.
 * @param m Matrix.
 * @param r Row index.
 * @param factor Scalar multiplier.
 * 
 * @warning Indices are not bounds-checked in this routine.
 */
void mat2D_mult_row(Mat2D m, size_t r, double factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        MAT2D_AT(m, r, j) *= factor;
    }
}

/**
 * @brief Print a matrix to stdout with a name and indentation padding.
 * @param m Matrix to print.
 * @param name Label to print.
 * @param padding Left padding in spaces.
 */
void mat2D_print(Mat2D m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows; ++i) {
        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < m.cols; ++j) {
            printf("%9.6f ", MAT2D_AT(m, i, j));
        }
        printf("\n");
    }
    printf("%*s]\n", (int) padding, "");
}

/**
 * @brief Print a matrix as a flattened column vector to stdout.
 * @param m Matrix to print (flattened in row-major).
 * @param name Label to print.
 * @param padding Left padding in spaces.
 */
void mat2D_print_as_col(Mat2D m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows*m.cols; ++i) {
            printf("%*s    ", (int) padding, "");
            printf("%f\n", m.elements[i]);
    }
    printf("%*s]\n", (int) padding, "");
}

/**
 * @brief Set a square matrix to the identity matrix.
 * @param m Matrix (must be square).
 *
 * @pre m.rows == m.cols (checked by MAT2D_ASSERT).
 */
void mat2D_set_identity(Mat2D m)
{
    MAT2D_ASSERT(m.cols == m.rows);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT2D_AT(m, i, j) = i == j ? 1 : 0;
            // if (i == j) {
            //     MAT2D_AT(m, i, j) = 1;
            // }
            // else {
            //     MAT2D_AT(m, i, j) = 0;
            // }
        }
    }
}

/**
 * @brief Set a 3x3 rotation matrix for rotation about the X-axis.
 * 
 * @param m 3x3 destination matrix.
 * @param angle_deg Angle in degrees.
 * 
 * @details
 * The matrix written is:
 *   [ 1, 0     ,  0      ]
 *   [ 0, cos(a),  sin(a) ]
 *   [ 0,-sin(a),  cos(a) ]
 */ 
void mat2D_set_rot_mat_x(Mat2D m, float angle_deg)
{
    MAT2D_ASSERT(3 == m.cols && 3 == m.rows);

    float angle_rad = angle_deg * MAT2D_PI / 180;
    mat2D_set_identity(m);
    MAT2D_AT(m, 1, 1) =  cos(angle_rad);
    MAT2D_AT(m, 1, 2) =  sin(angle_rad);
    MAT2D_AT(m, 2, 1) = -sin(angle_rad);
    MAT2D_AT(m, 2, 2) =  cos(angle_rad);
}

/**
 * @brief Set a 3x3 rotation matrix for rotation about the Y-axis.
 *
 * @param m 3x3 destination matrix.
 * @param angle_deg Angle in degrees.
 *
 * @details
 * The matrix written is:
 *   [ cos(a), 0,-sin(a) ]
 *   [ 0     , 1, 0      ]
 *   [ sin(a), 0, cos(a) ]
 */
void mat2D_set_rot_mat_y(Mat2D m, float angle_deg)
{
    MAT2D_ASSERT(3 == m.cols && 3 == m.rows);

    float angle_rad = angle_deg * MAT2D_PI / 180;
    mat2D_set_identity(m);
    MAT2D_AT(m, 0, 0) =  cos(angle_rad);
    MAT2D_AT(m, 0, 2) = -sin(angle_rad);
    MAT2D_AT(m, 2, 0) =  sin(angle_rad);
    MAT2D_AT(m, 2, 2) =  cos(angle_rad);
}

/**
 * @brief Set a 3x3 rotation matrix for rotation about the Z-axis.
 *
 * @param m 3x3 destination matrix.
 * @param angle_deg Angle in degrees.
 *
 * @details
 * The matrix written is:
 *   [ cos(a), sin(a), 0 ]
 *   [-sin(a), cos(a), 0 ]
 *   [ 0     , 0     , 1 ]
 */
void mat2D_set_rot_mat_z(Mat2D m, float angle_deg)
{
    MAT2D_ASSERT(3 == m.cols && 3 == m.rows);

    float angle_rad = angle_deg * MAT2D_PI / 180;
    mat2D_set_identity(m);
    MAT2D_AT(m, 0, 0) =  cos(angle_rad);
    MAT2D_AT(m, 0, 1) =  sin(angle_rad);
    MAT2D_AT(m, 1, 0) = -sin(angle_rad);
    MAT2D_AT(m, 1, 1) =  cos(angle_rad);
}

/**
 * @brief Build a 3x3 direction cosine matrix (DCM) from Z-Y-X Euler angles.
 * @param DCM 3x3 destination matrix.
 * @param yaw_deg Rotation about Z in degrees.
 * @param pitch_deg Rotation about Y in degrees.
 * @param roll_deg Rotation about X in degrees.
 * 
 * @details Computes DCM = R_x(roll) * R_y(pitch) * R_z(yaw).
 * 
 * @note This routine allocates temporary 3x3 matrices internally. 
 */
void mat2D_set_DCM_zyx(Mat2D DCM, float yaw_deg, float pitch_deg, float roll_deg)
{
    Mat2D RotZ = mat2D_alloc(3,3);
    mat2D_set_rot_mat_z(RotZ, yaw_deg);
    Mat2D RotY = mat2D_alloc(3,3);
    mat2D_set_rot_mat_y(RotY, pitch_deg);
    Mat2D RotX = mat2D_alloc(3,3);
    mat2D_set_rot_mat_x(RotX, roll_deg);
    Mat2D temp = mat2D_alloc(3,3);

    mat2D_dot(temp, RotY, RotZ);
    mat2D_dot(DCM, RotX, temp); /* I have a DCM */

    mat2D_free(RotZ);
    mat2D_free(RotY);
    mat2D_free(RotX);
    mat2D_free(temp);
}

/**
 * @brief Copy all elements from src to des.
 * @param des Destination matrix.
 * @param src Source matrix.
 * @pre Shapes match.
 *
 * @pre des and src have identical shape.
 */
void mat2D_copy(Mat2D des, Mat2D src)
{
    MAT2D_ASSERT(des.cols == src.cols);
    MAT2D_ASSERT(des.rows == src.rows);

    for (size_t i = 0; i < des.rows; ++i) {
        for (size_t j = 0; j < des.cols; ++j) {
            MAT2D_AT(des, i, j) = MAT2D_AT(src, i, j);
        }
    }
}

/**
 * @brief Copy a rectangular window from src into des.
 * @param des Destination matrix. Must have size (ie - is + 1) x (je - js + 1).
 * @param src Source matrix.
 * @param is Start row index in src (inclusive).
 * @param js Start column index in src (inclusive).
 * @param ie End row index in src (inclusive).
 * @param je End column index in src (inclusive).
 * @pre 0 <= is <= ie < src.rows, 0 <= js <= je < src.cols.
 */
void mat2D_copy_src_window_to_des(Mat2D des, Mat2D src, size_t is, size_t js, size_t ie, size_t je)
{
    MAT2D_ASSERT(je >= js && ie >= is);
    MAT2D_ASSERT(je-js+1 == des.cols);
    MAT2D_ASSERT(ie-is+1 == des.rows);
    MAT2D_ASSERT(je-js+1 <= src.cols);
    MAT2D_ASSERT(ie-is+1 <= src.rows);

    for (size_t index = 0; index < des.rows; ++index) {
        for (size_t jndex = 0; jndex < des.cols; ++jndex) {
            MAT2D_AT(des, index, jndex) = MAT2D_AT(src, is+index, js+jndex);
        }
    }
}

void mat2D_copy_src_to_des_window(Mat2D des, Mat2D src, size_t is, size_t js, size_t ie, size_t je)
{
    MAT2D_ASSERT(je >= js && ie >= is);
    MAT2D_ASSERT(je-js+1 == src.cols);
    MAT2D_ASSERT(ie-is+1 == src.rows);
    MAT2D_ASSERT(je-js+1 <= des.cols);
    MAT2D_ASSERT(ie-is+1 <= des.rows);

    for (size_t index = 0; index < src.rows; ++index) {
        for (size_t jndex = 0; jndex < src.cols; ++jndex) {
            MAT2D_AT(des, is+index, js+jndex) = MAT2D_AT(src, index, jndex);
        }
    }
}

/**
 * @brief Copy a column from src into a column of des.
 * @param des Destination matrix (same row count as src).
 * @param des_col Column index in destination.
 * @param src Source matrix.
 * @param src_col Column index in source.
 * 
 * @pre des.rows == src.rows
 * @pre des_col < des.cols and src_col < src.cols
 */ 
void mat2D_get_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col)
{
    MAT2D_ASSERT(src_col < src.cols);
    MAT2D_ASSERT(des.rows == src.rows);
    MAT2D_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        MAT2D_AT(des, i, des_col) = MAT2D_AT(src, i, src_col);
    }
}

/**
 * @brief Add a source column into a destination column.
 *
 * @details
 * Performs: des[:, des_col] += src[:, src_col]
 * @param des Destination matrix (same row count as src).
 * @param des_col Column index in destination.
 * @param src Source matrix.
 * @param src_col Column index in source.
 */
void mat2D_add_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col)
{
    MAT2D_ASSERT(src_col < src.cols);
    MAT2D_ASSERT(des.rows == src.rows);
    MAT2D_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        MAT2D_AT(des, i, des_col) += MAT2D_AT(src, i, src_col);
    }
}

/**
 * @brief Subtract a source column from a destination column.
 *
 * @details
 * Performs: des[:, des_col] -= src[:, src_col]
 * @param des Destination matrix (same row count as src).
 * @param des_col Column index in destination.
 * @param src Source matrix.
 * @param src_col Column index in source.
 */
void mat2D_sub_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col)
{
    MAT2D_ASSERT(src_col < src.cols);
    MAT2D_ASSERT(des.rows == src.rows);
    MAT2D_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        MAT2D_AT(des, i, des_col) -= MAT2D_AT(src, i, src_col);
    }
}

/**
 * @brief Swap two rows of a matrix in-place.
 * @param m Matrix.
 * @param r1 First row index.
 * @param r2 Second row index.
 * 
 * @warning Row indices are not bounds-checked in this routine. 
 */
void mat2D_swap_rows(Mat2D m, size_t r1, size_t r2)
{
    for (size_t j = 0; j < m.cols; j++) {
        double temp = MAT2D_AT(m, r1, j);
        MAT2D_AT(m, r1, j) = MAT2D_AT(m, r2, j);
        MAT2D_AT(m, r2, j) = temp;
    }
}

/**
 * @brief Copy a row from src into a row of des.
 * @param des Destination matrix (same number of columns as src).
 * @param des_row Row index in destination.
 * @param src Source matrix.
 * @param src_row Row index in source.
 *
 * @pre des.cols == src.cols
 */
void mat2D_get_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row)
{
    MAT2D_ASSERT(src_row < src.rows);
    MAT2D_ASSERT(des.cols == src.cols);
    MAT2D_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        MAT2D_AT(des, des_row, j) = MAT2D_AT(src, src_row, j);
    }
}

/**
 * @param src_row Row index in source.
 *
 * @pre des.cols == src.cols
 * @param des Destination matrix (same number of columns as src).
 * @param des_row Row index in destination.
 * @param src Source matrix.
 * @param src_row Row index in source.
 */
void mat2D_add_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row)
{
    MAT2D_ASSERT(src_row < src.rows);
    MAT2D_ASSERT(des.cols == src.cols);
    MAT2D_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        MAT2D_AT(des, des_row, j) += MAT2D_AT(src, src_row, j);
    }
}

/**
 * @brief Subtract a source row from a destination row.
 *
 * @details
 * Performs: des[des_row, :] -= src[src_row, :]
 * @param des Destination matrix (same number of columns as src).
 * @param des_row Row index in destination.
 * @param src Source matrix.
 * @param src_row Row index in source.
 */
void mat2D_sub_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row)
{
    MAT2D_ASSERT(src_row < src.rows);
    MAT2D_ASSERT(des.cols == src.cols);
    MAT2D_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        MAT2D_AT(des, des_row, j) -= MAT2D_AT(src, src_row, j);
    }
}

/**
 * @brief Compute the Frobenius norm of a matrix, sqrt(sum(m_ij^2)).
 * @param m Matrix.
 * @return Frobenius norm.
 */
double mat2D_calc_norma(Mat2D m)
{
    double sum = 0;

    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            sum += MAT2D_AT(m, i, j) * MAT2D_AT(m, i, j);
        }
    }
    return sqrt(sum);
}

double mat2D_calc_norma_inf(Mat2D m)
{
    double max = 0;
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            double current = fabs(MAT2D_AT(m, i, j));
            if (current > max) {
                max = current;
            }
        }
    }

    return max;
}

/**
 * @brief Check if all elements of a matrix equal a given digit.
 * @param m Matrix.
 * @param digit Value to compare.
 * @return true if every element equals digit, false otherwise.
 *
 * @warning Uses exact floating-point equality.
 */
bool mat2D_mat_is_all_digit(Mat2D m, double digit)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            if (MAT2D_AT(m, i, j) != digit) {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief Check if all elements of a row equal a given digit.
 * @param m Matrix.
 * @param digit Value to compare.
 * @param r Row index.
 * @return true if every element equals digit, false otherwise.
 *
 * @warning Uses exact floating-point equality.
 */
bool mat2D_row_is_all_digit(Mat2D m, double digit, size_t r)
{
    for (size_t j = 0; j < m.cols; ++j) {
        if (MAT2D_AT(m, r, j) != digit) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Check if all elements of a column equal a given digit.
 * @param m Matrix.
 * @param digit Value to compare.
 * @param c Column index.
 * @return true if every element equals digit, false otherwise.
 *
 * @warning Uses exact floating-point equality.
 */
bool mat2D_col_is_all_digit(Mat2D m, double digit, size_t c)
{
    for (size_t i = 0; i < m.rows; ++i) {
        if (MAT2D_AT(m, i, c) != digit) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Determinant of a 2x2 matrix.
 * @param m Matrix (must be 2x2).
 * @return det(m) = m00*m11 - m01*m10.
 */
double mat2D_det_2x2_mat(Mat2D m)
{
    MAT2D_ASSERT(2 == m.cols && 2 == m.rows && "Not a 2x2 matrix");
    return MAT2D_AT(m, 0, 0) * MAT2D_AT(m, 1, 1) - MAT2D_AT(m, 0, 1) * MAT2D_AT(m, 1, 0);
}

/**
 * @brief Forward elimination to transform a matrix to upper triangular form.
 *
 * @param m Matrix transformed in-place.
 * @return Product of row scaling factors (currently 1 in this implementation).
 * @note Used as part of determinant computation via triangularization.
 * @warning Not robust for linearly dependent rows or tiny pivots.
 * @return A factor tracking the effect of row swaps on the determinant.
 *         Currently this is ±1 depending on the number of row swaps performed.
 *
 * @details
 * This routine performs Gaussian elimination using row operations of the form:
 *   row_j = row_j - (m[j,i] / m[i,i]) * row_i
 * which do not change the determinant. Row swaps flip the determinant sign and
 * are tracked by the returned factor.
 */
double mat2D_upper_triangulate(Mat2D m)
{
    /* preforming Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */
    /* returns the factor multiplying the determinant */

    double factor_to_return = 1;

    size_t r = 0;
    for (size_t c = 0; c < m.cols && r < m.rows; c++) {
        /* finding biggest first number (absolute value); partial pivoting */
        size_t piv = r;
        double best = fabs(MAT2D_AT(m, r, c));
        for (size_t i = r + 1; i < m.rows; i++) {
            double v = fabs(MAT2D_AT(m, i, c));
            if (v > best) {
                best = v;
                piv = i;
            }
        }
        if (MAT2D_IS_ZERO(best)) {
            continue; /* move to next column, same pivot row r */
        }
        if (piv != r) {
            mat2D_swap_rows(m, piv, r);
            factor_to_return *= -1.0;
        }

        /* Eliminate entries below pivot in column c */
        double pivot = MAT2D_AT(m, r, c);
        MAT2D_ASSERT(!MAT2D_IS_ZERO(pivot));
        for (size_t i = r + 1; i < m.rows; i++) {
            double f = MAT2D_AT(m, i, c) / pivot;
            mat2D_sub_row_time_factor_to_row(m, i, r, f);
        }
        r++;
    }
    return factor_to_return;
}

bool mat2D_find_first_non_zero_value(Mat2D m, size_t r, size_t *non_zero_col)
{
    for (size_t c = 0; c < m.cols; ++c) {
        if (!MAT2D_IS_ZERO(MAT2D_AT(m, r, c))) {
            *non_zero_col = c;
            return true;
        }
    }
    return false;
}

/**
 * @brief Reduce a matrix to identity using Gauss-Jordan style elimination.
 *
 * @param m Matrix modified in-place.
 * @return A multiplicative factor that tracks the effect of row swaps and
 *         row scalings performed inside this routine (useful when relating the
 *         transformation to determinants).
 *
 * @details
 * Internally calls mat2D_upper_triangulate() and then performs backward
 * elimination and row scaling to reach identity (if the matrix is nonsingular
 * and pivots are usable).
 */
size_t mat2D_reduce(Mat2D m)
{
    /* preforming Gauss–Jordan reduction to Reduced Row Echelon Form (RREF) */
    /* Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */

    mat2D_upper_triangulate(m);

    size_t rank = 0;

    for (int r = m.rows-1; r >= 0; r--) {
        size_t c = m.cols-1;
        if (!mat2D_find_first_non_zero_value(m, r, &c)) {
            continue; /* row of zeros */
        }

        double pivot = MAT2D_AT(m, r, c);
        MAT2D_ASSERT(!MAT2D_IS_ZERO(pivot));
        mat2D_mult_row(m, r, 1.0 / pivot);

        for (int i = 0; i < r; i++) {
            double factor = MAT2D_AT(m, i, c);
            mat2D_sub_row_time_factor_to_row(m, i, r, factor);
        }
        rank++;
    }

    return rank;
}

/**
 * @brief Determinant of a square matrix via Gaussian elimination.
 *
 * @param m Square matrix.
 * @return det(m).
 *
 * @details
 * Copies @p m internally, transforms the copy to upper triangular form, and
 * returns the product of diagonal elements adjusted by the row-swap factor.
 *
 * @warning The early “all-zero row/column” check uses exact comparisons to 0.
 * @warning Limited pivoting may cause poor numerical results for some inputs.
 */
double mat2D_det(Mat2D m)
{
    MAT2D_ASSERT(m.cols == m.rows && "should be a square matrix");

    /* checking if there is a row or column with all zeros */
    /* checking rows */
    for (size_t i = 0; i < m.rows; i++) {
        if (mat2D_row_is_all_digit(m, 0, i)) {
            return 0;
        }
    }
    /* checking cols */
    for (size_t j = 0; j < m.rows; j++) {
        if (mat2D_col_is_all_digit(m, 0, j)) {
            return 0;
        }
    }

    #if 0/* This is an implementation of naive determinant calculation using minors. This is too slow */
    double det = 0;
    /* TODO: finding beast row or col? */
    for (size_t i = 0, j = 0; i < m.rows; i++) { /* first column */
        if (MAT2D_AT(m, i, j) < 1e-10) continue;
        Mat2D_Minor sub_mm = mat2D_minor_alloc_fill_from_mat(m, i, j);
        int factor = (i+j)%2 ? -1 : 1;
        if (sub_mm.cols != 2) {
            MAT2D_ASSERT(sub_mm.cols == sub_mm.rows && "should be a square matrix");
            det += MAT2D_AT(m, i, j) * (factor) * mat2D_minor_det(sub_mm);
        } else if (sub_mm.cols == 2 && sub_mm.rows == 2) {
            det += MAT2D_AT(m, i, j) * (factor) * mat2D_det_2x2_mat_minor(sub_mm);;
        }
        mat2D_minor_free(sub_mm);
    }
    #endif

    Mat2D temp_m = mat2D_alloc(m.rows, m.cols);
    mat2D_copy(temp_m, m);
    double factor = mat2D_upper_triangulate(temp_m);
    double diag_mul = 1; 
    for (size_t i = 0; i < temp_m.rows; i++) {
        diag_mul *= MAT2D_AT(temp_m, i, i);
    }
    mat2D_free(temp_m);

    return diag_mul / factor;
}

/**
 * @brief Compute LUP decomposition: P*A = L*U with L unit diagonal.
 *
 * @param src Input matrix A (not modified by this function).
 * @param l Output lower-triangular-like matrix (intended to have unit diagonal).
 * @param p Output permutation matrix.
 * @param u Output upper-triangular-like matrix.
 *
 * @pre src is square.
 * @pre l, p, u are allocated with the same shape as src.
 *
 * @warning Pivoting is limited: a row swap is performed only when the pivot is
 *          “near zero” (MAT2D_IS_ZERO()).
 * @warning This routine swaps rows of L during decomposition; for a standard
 *          LUP implementation, care is required when swapping partially-built L.
 */
void mat2D_LUP_decomposition_with_swap(Mat2D src, Mat2D l, Mat2D p, Mat2D u)
{
    /* performing LU decomposition Following the Wikipedia page: https://en.wikipedia.org/wiki/LU_decomposition */

    mat2D_copy(u, src);
    mat2D_set_identity(p);
    mat2D_fill(l, 0);

    for (size_t i = 0; i < (size_t)fmin(u.rows-1, u.cols); i++) {
        if (MAT2D_IS_ZERO(MAT2D_AT(u, i, i))) {   /* swapping only if it is zero */
            /* finding biggest first number (absolute value) */
            size_t biggest_r = i;
            for (size_t index = i; index < u.rows; index++) {
                if (fabs(MAT2D_AT(u, index, i)) > fabs(MAT2D_AT(u, biggest_r, i))) {
                    biggest_r = index;
                }
            }
            if (i != biggest_r) {
                mat2D_swap_rows(u, i, biggest_r);
                mat2D_swap_rows(p, i, biggest_r);
                mat2D_swap_rows(l, i, biggest_r);
            }
        }
        for (size_t j = i+1; j < u.cols; j++) {
            double factor = 1 / MAT2D_AT(u, i, i);
            if (!isfinite(factor)) {
                printf("%s:%d:\n%s:\n[Error] unable to transfrom into uper triangular matrix. Probably some of the rows are not independent.\n", __FILE__, __LINE__, __func__);
            }
            double mat_value = MAT2D_AT(u, j, i);
            mat2D_sub_row_time_factor_to_row(u, j, i, mat_value * factor);
            MAT2D_AT(l, j, i) = mat_value * factor;
        }
        MAT2D_AT(l, i, i) = 1;
    }
    MAT2D_AT(l, l.rows-1, l.cols-1) = 1;
}

/**
 * @brief Transpose a matrix: des = src^T.
 * @param des Destination matrix (shape src.cols x src.rows).
 * @param src Source matrix.
 *
 * @warning If des aliases src, results are undefined (no in-place transpose).
 */
void mat2D_transpose(Mat2D des, Mat2D src)
{
    MAT2D_ASSERT(des.cols == src.rows);
    MAT2D_ASSERT(des.rows == src.cols);

    for (size_t index = 0; index < des.rows; ++index) {
        for (size_t jndex = 0; jndex < des.cols; ++jndex) {
            MAT2D_AT(des, index, jndex) = MAT2D_AT(src, jndex, index);
        }
    }
}

/**
 * @brief Invert a square matrix using Gauss-Jordan elimination.
 *
 * @param des Destination matrix (same shape as src).
 * @param src Source square matrix.
 *
 * @pre src is square.
 * @pre des is allocated as the same shape as src.
 *
 * @details
 * On singular matrices (det == 0), prints an error message, writes all zeros
 * into @p des, and returns.
 *
 * @warning This implementation performs limited pivoting (only when a pivot is
 *          “near zero” per MAT2D_EPS). It may be unstable for ill-conditioned
 *          matrices.
 * @warning This routine computes det(src) first, which performs an additional
 *          elimination pass and can amplify numerical issues.
 */
void mat2D_invert(Mat2D des, Mat2D src)
{
    MAT2D_ASSERT(src.cols == src.rows && "Must be an NxN matrix");
    MAT2D_ASSERT(des.cols == src.cols && des.rows == des.cols);

    Mat2D m = mat2D_alloc(src.rows, src.cols * 2);
    mat2D_copy_src_to_des_window(m, src, 0, 0, src.rows-1, src.cols-1);

    mat2D_set_identity(des);
    mat2D_copy_src_to_des_window(m, des, 0, src.cols, des.rows-1, 2 * des.cols-1);
    
    MAT2D_PRINT(m);

    mat2D_reduce(m);

    MAT2D_PRINT(m);

    mat2D_copy_src_window_to_des(des, m, 0, src.cols, des.rows-1, 2 * des.cols-1);

    mat2D_free(m);
}

/**
 * @brief Solve the linear system A x = B using an LUP-based approach.
 *
 * @param A Coefficient matrix (N x N).
 * @param x Solution vector (N x 1). Written on success.
 * @param B Right-hand side vector (N x 1).
 *
 * @details
 * This routine computes an LUP decomposition and then forms explicit inverses
 * of L and U (inv(L), inv(U)) to compute:
 *   x = inv(U) * inv(L) * (P * B)
 *
 * @warning Explicitly inverting L and U is typically less stable and slower
 *          than forward/back substitution. Prefer substitution for
 *          production-quality solvers.
 */
void mat2D_solve_linear_sys_LUP_decomposition(Mat2D A, Mat2D x, Mat2D B)
{
    MAT2D_ASSERT(A.cols == x.rows);
    MAT2D_ASSERT(1 == x.cols);
    MAT2D_ASSERT(A.rows == B.rows);
    MAT2D_ASSERT(1 == B.cols);

    Mat2D y     = mat2D_alloc(x.rows, x.cols);
    Mat2D l     = mat2D_alloc(A.rows, A.cols);
    Mat2D p     = mat2D_alloc(A.rows, A.cols);
    Mat2D u     = mat2D_alloc(A.rows, A.cols);
    Mat2D inv_l = mat2D_alloc(l.rows, l.cols);
    Mat2D inv_u = mat2D_alloc(u.rows, u.cols);

    mat2D_LUP_decomposition_with_swap(A, l, p, u);

    mat2D_invert(inv_l, l);
    mat2D_invert(inv_u, u);

    mat2D_fill(x, 0);   /* x here is only a temp mat*/
    mat2D_fill(y, 0);
    mat2D_dot(x, p, B);
    mat2D_dot(y, inv_l, x);

    mat2D_fill(x, 0);
    mat2D_dot(x, inv_u, y);

    mat2D_free(y);
    mat2D_free(l);
    mat2D_free(p);
    mat2D_free(u);
    mat2D_free(inv_l);
    mat2D_free(inv_u);
}

/**
 * @brief Allocate a minor view by excluding row i and column j of ref_mat.
 * @param ref_mat Reference square matrix.
 * @param i Excluded row index in ref_mat.
 * @param j Excluded column index in ref_mat.
 * @return A Mat2D_Minor that references ref_mat.
 *
 * @note The returned minor owns rows_list and cols_list and must be released
 *       with mat2D_minor_free().
 * @note The returned minor does not own ref_mat.elements.
 */
Mat2D_Minor mat2D_minor_alloc_fill_from_mat(Mat2D ref_mat, size_t i, size_t j)
{
    MAT2D_ASSERT(ref_mat.cols == ref_mat.rows && "minor is defined only for square matrix");

    Mat2D_Minor mm;
    mm.cols = ref_mat.cols-1;
    mm.rows = ref_mat.rows-1;
    mm.stride_r = ref_mat.cols-1;
    mm.cols_list = (size_t*)MAT2D_MALLOC(sizeof(size_t)*(ref_mat.cols-1));
    mm.rows_list = (size_t*)MAT2D_MALLOC(sizeof(size_t)*(ref_mat.rows-1));
    mm.ref_mat = ref_mat;

    MAT2D_ASSERT(mm.cols_list != NULL && mm.rows_list != NULL);

    for (size_t index = 0, temp_index = 0; index < ref_mat.rows; index++) {
        if (index != i) {
            mm.rows_list[temp_index] = index;
            temp_index++;
        }
    }
    for (size_t jndex = 0, temp_jndex = 0; jndex < ref_mat.cols; jndex++) {
        if (jndex != j) {
            mm.cols_list[temp_jndex] = jndex;
            temp_jndex++;
        }
    }

    return mm;
}

/**
 * @brief Allocate a nested minor view from an existing minor by excluding row i
 *        and column j of the minor.
 * @param ref_mm Reference minor.
 * @param i Excluded row index in the minor.
 * @param j Excluded column index in the minor.
 * @return A new Mat2D_Minor that references the same underlying matrix.
 *
 * @note The returned minor owns rows_list and cols_list and must be released
 *       with mat2D_minor_free().
 * @note The returned minor does not own the underlying reference matrix data.
 */
Mat2D_Minor mat2D_minor_alloc_fill_from_mat_minor(Mat2D_Minor ref_mm, size_t i, size_t j)
{
    MAT2D_ASSERT(ref_mm.cols == ref_mm.rows && "minor is defined only for square matrix");

    Mat2D_Minor mm;
    mm.cols = ref_mm.cols-1;
    mm.rows = ref_mm.rows-1;
    mm.stride_r = ref_mm.cols-1;
    mm.cols_list = (size_t*)MAT2D_MALLOC(sizeof(size_t)*(ref_mm.cols-1));
    mm.rows_list = (size_t*)MAT2D_MALLOC(sizeof(size_t)*(ref_mm.rows-1));
    mm.ref_mat = ref_mm.ref_mat;

    MAT2D_ASSERT(mm.cols_list != NULL && mm.rows_list != NULL);

    for (size_t index = 0, temp_index = 0; index < ref_mm.rows; index++) {
        if (index != i) {
            mm.rows_list[temp_index] = ref_mm.rows_list[index];
            temp_index++;
        }
    }
    for (size_t jndex = 0, temp_jndex = 0; jndex < ref_mm.cols; jndex++) {
        if (jndex != j) {
            mm.cols_list[temp_jndex] = ref_mm.cols_list[jndex];
            temp_jndex++;
        }
    }

    return mm;
}

/**
 * @brief Free the index arrays owned by a minor.
 * @param mm Minor to free.
 * @note After this call, mm.rows_list and mm.cols_list are invalid.
 */
void mat2D_minor_free(Mat2D_Minor mm)
{
    MAT2D_FREE(mm.cols_list);
    MAT2D_FREE(mm.rows_list);
}

/**
 * @brief Print a minor matrix to stdout with a name and indentation padding.
 * @param mm Minor to print.
 * @param name Label to print.
 * @param padding Left padding in spaces.
 */
void mat2D_minor_print(Mat2D_Minor mm, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < mm.rows; ++i) {
        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < mm.cols; ++j) {
            printf("%f ", MAT2D_MINOR_AT(mm, i, j));
        }
        printf("\n");
    }
    printf("%*s]\n", (int) padding, "");
}

/**
 * @brief Determinant of a 2x2 minor.
 * @param mm Minor (must be 2x2).
 * @return det(mm).
 */
double mat2D_det_2x2_mat_minor(Mat2D_Minor mm)
{
    MAT2D_ASSERT(2 == mm.cols && 2 == mm.rows && "Not a 2x2 matrix");
    return MAT2D_MINOR_AT(mm, 0, 0) * MAT2D_MINOR_AT(mm, 1, 1) - MAT2D_MINOR_AT(mm, 0, 1) * MAT2D_MINOR_AT(mm, 1, 0);
}

/**
 * @brief Determinant of a minor via recursive expansion by minors.
 * @param mm Square minor.
 * @return det(mm).
 * @warning Exponential complexity (factorial). Intended for educational or
 *          very small matrices only.
 */
double mat2D_minor_det(Mat2D_Minor mm)
{
    MAT2D_ASSERT(mm.cols == mm.rows && "should be a square matrix");

    double det = 0;
    /* TODO: finding beast row or col? */
    for (size_t i = 0, j = 0; i < mm.rows; i++) { /* first column */
        if (fabs(MAT2D_MINOR_AT(mm, i, j)) < 1e-10) continue;
        Mat2D_Minor sub_mm = mat2D_minor_alloc_fill_from_mat_minor(mm, i, j);
        int factor = (i+j)%2 ? -1 : 1;
        if (sub_mm.cols != 2) {
            MAT2D_ASSERT(sub_mm.cols == sub_mm.rows && "should be a square matrix");
            det += MAT2D_MINOR_AT(mm, i, j) * (factor) * mat2D_minor_det(sub_mm);
        } else if (sub_mm.cols == 2 && sub_mm.rows == 2) {
            det += MAT2D_MINOR_AT(mm, i, j) * (factor) * mat2D_det_2x2_mat_minor(sub_mm);;
        }
        mat2D_minor_free(sub_mm);
    }
    return det;
}

#endif // MATRIX2D_IMPLEMENTATION