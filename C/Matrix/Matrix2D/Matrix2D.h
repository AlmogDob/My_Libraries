/**
 * @file 
 * @brief A single-header C library for simple 2D matrix operations on doubles
 *        and uint32_t, including allocation, basic arithmetic, linear algebra,
 *        and helpers (LUP, inverse, determinant, DCM, etc.).
 *
 * @details
 * - Storage is contiguous row-major (C-style). The element at row i,
 *   column j (0-based) is located at `elements[i * stride_r + j]`.
 * - Dense matrices of `double` are represented by Mat2D, and dense matrices
 *   of `uint32_t` are represented by Mat2D_uint32.
 * - Some routines assert shape compatibility using MATRIX2D_ASSERT.
 * - Random number generation uses the C library `rand()`; it is not
 *   cryptographically secure.
 * - Inversion is done via Gauss-Jordan elimination with partial pivoting only
 *   when a pivot is zero; this can be numerically unstable for ill-conditioned
 *   matrices. See notes below.
 * - To compile the implementation, define MATRIX2D_IMPLEMENTATION in exactly
 *   one translation unit before including this header.
 *
 * Example:
 *   #define MATRIX2D_IMPLEMENTATION
 *   #include "matrix2d.h"
 *
 * @note This one-file library is heavily inspired by Tsoding's nn.h
 *       implementation of matrix creation and operations:
 *       https://github.com/tsoding/nn.h and the video:
 *       https://youtu.be/L1TbWe8bVOc?list=PLpM-Dvs8t0VZPZKggcql-MmjaBdZKeDMw
 *
 * @warning Numerical stability:
 * - There is a set of functions for minors that can be used to compute the
 *   determinant, but that approach is factorial in complexity and too slow for
 *   larger matrices. This library uses Gaussian elimination instead.
 * - The inversion function can fail or be unstable if pivot values become very
 *   small. Consider preconditioning or using a more robust decomposition (e.g.,
 *   full pivoting, SVD) for ill-conditioned problems.
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
 * @def MATRIX2D_MALLOC
 * @brief Allocation function used by the library.
 * @details
 * Defaults to `malloc`. Override by defining MATRIX2D_MALLOC before including
 * this header if you want to use a custom allocator.
 */
#ifndef MAT2D_MALLOC
#define MAT2D_MALLOC malloc
#endif //MAT2D_MALLOC

#ifndef MAT2D_FREE
#define MAT2D_FREE free
#endif //MAT2D_FREE

/**
 * @def MAT2D_ASSERT
 * @brief Assertion macro used by the library for parameter validation.
 * @details
 * Defaults to C `assert`. Override by defining MAT2D_ASSERT before including
 * this header if you want custom behavior.
 */
#ifndef MAT2D_ASSERT
#include <assert.h>
#define MAT2D_ASSERT assert
#endif //MAT2D_ASSERT

/**
 * @brief Dense row-major matrix of doubles.
 *
 * @details
 * - rows: number of rows (height)
 * - cols: number of columns (width)
 * - stride_r: number of elements between successive rows in memory
 *   (for contiguous storage, stride_r == cols)
 * - elements: pointer to contiguous storage of size rows * cols
 */
typedef struct {
    size_t rows;
    size_t cols;
    size_t stride_r; /* how many element you need to traves to get to the element underneath */
    double *elements;
} Mat2D;

/**
 * @brief Dense row-major matrix of uint32_t.
 *
 * @details
 * - rows: number of rows (height)
 * - cols: number of columns (width)
 * - stride_r: number of elements between successive rows in memory
 *   (for contiguous storage, stride_r == cols)
 * - elements: pointer to contiguous storage of size rows * cols
 */
typedef struct {
    size_t rows;
    size_t cols;
    size_t stride_r; /* how many element you need to traves to get to the element underneath */
    uint32_t *elements;
} Mat2D_uint32;

/**
 * @brief A minor "view" into a reference matrix.
 *
 * @details
 * Represents a minor by excluding one row and one column of a reference matrix.
 * It holds index lists mapping into the reference matrix, without owning the
 * data of the reference matrix itself.
 *
 * Memory ownership:
 * - rows_list and cols_list are heap-allocated by minor allocators and must be
 *   freed with mat2D_minor_free.
 * - The underlying matrix data (ref_mat.elements) is not owned by the minor and
 *   must not be freed by the minor functions.
 */
typedef struct {
    size_t rows;
    size_t cols;
    size_t stride_r; /* how many element you need to traves to get to the element underneath */
    size_t *rows_list;
    size_t *cols_list;
    Mat2D ref_mat;
} Mat2D_Minor;

/**
 * @def MAT2D_AT
 * @brief Access element (i, j) of a Mat2D (0-based).
 * @warning This macro does not perform bounds checking in the fast
 *          configuration. Use carefully.
 */

/**
 * @def MAT2D_AT_UINT32
 * @brief Access element (i, j) of a Mat2D_uint32 (0-based).
 * @warning This macro does not perform bounds checking in the fast
 *          configuration. Use carefully.
 */
#if 0
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

#define MAT2D_IS_ZERO(x) (fabs(x) < MAT2D_EPS)

/**
 * @def MAT2D_MINOR_AT
 * @brief Access element (i, j) of a Mat2D_Minor (0-based), dereferencing into
 *        the underlying reference matrix.
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
 * @def mat2D_normalize
 * @brief In-place normalization of all elements so that the Frobenius norm
 *        becomes 1.
 * @details Equivalent to: m *= 1.0 / mat2D_calc_norma(m).
 */
#define mat2D_normalize(m) mat2D_mult((m), 1.0 / mat2D_calc_norma((m)))

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
double          mat2D_make_identity(Mat2D m);
void            mat2D_set_rot_mat_x(Mat2D m, float angle_deg);
void            mat2D_set_rot_mat_y(Mat2D m, float angle_deg);
void            mat2D_set_rot_mat_z(Mat2D m, float angle_deg);
void            mat2D_set_DCM_zyx(Mat2D DCM, float yaw_deg, float pitch_deg, float roll_deg);

void            mat2D_copy(Mat2D des, Mat2D src);
void            mat2D_copy_mat_to_mat_at_window(Mat2D des, Mat2D src, size_t is, size_t js, size_t ie, size_t je);

void            mat2D_get_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col);
void            mat2D_add_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col);
void            mat2D_sub_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col);

void            mat2D_swap_rows(Mat2D m, size_t r1, size_t r2);
void            mat2D_get_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row);
void            mat2D_add_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row);
void            mat2D_sub_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row);

double          mat2D_calc_norma(Mat2D m);

bool            mat2D_mat_is_all_digit(Mat2D m, double digit);
bool            mat2D_row_is_all_digit(Mat2D m, double digit, size_t r);
bool            mat2D_col_is_all_digit(Mat2D m, double digit, size_t c);

double          mat2D_det_2x2_mat(Mat2D m);
double          mat2D_upper_triangulate(Mat2D m);
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
 * @note Uses C library rand() and RAND_MAX. Not cryptographically secure.
 */
double mat2D_rand_double(void)
{
    return (double) rand() / (double) RAND_MAX;
}

/**
 * @brief Allocate a rows x cols matrix of doubles.
 * @param rows Number of rows (>= 1).
 * @param cols Number of columns (>= 1).
 * @return A Mat2D with contiguous storage; must be freed with mat2D_free.
 * @post m.stride_r == cols.
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
 * @brief Allocate a rows x cols matrix of uint32_t.
 * @param rows Number of rows (>= 1).
 * @param cols Number of columns (>= 1).
 * @return A Mat2D_uint32 with contiguous storage; free with mat2D_free_uint32.
 * @post m.stride_r == cols.
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
 * @brief Free the memory owned by a Mat2D (elements pointer).
 * @param m Matrix whose elements were allocated via MAT2D_MALLOC.
 * @note Safe to call with m.elements == NULL.
 */
void mat2D_free(Mat2D m)
{
    MAT2D_FREE(m.elements);
}

/**
 * @brief Free the memory owned by a Mat2D_uint32 (elements pointer).
 * @param m Matrix whose elements were allocated via MAT2D_MALLOC.
 * @note Safe to call with m.elements == NULL.
 */
void mat2D_free_uint32(Mat2D_uint32 m)
{
    MAT2D_FREE(m.elements);
}

/**
 * @brief Compute the linear offset of element (i, j) in a Mat2D.
 * @param m Matrix.
 * @param i Row index (0-based).
 * @param j Column index (0-based).
 * @return The linear offset i * stride_r + j.
 * @pre 0 <= i < rows, 0 <= j < cols (asserted).
 */
size_t mat2D_offset2d(Mat2D m, size_t i, size_t j)
{
    MAT2D_ASSERT(i < m.rows && j < m.cols);
    return i * m.stride_r + j;
}

/**
 * @brief Compute the linear offset of element (i, j) in a Mat2D_uint32.
 * @param m Matrix.
 * @param i Row index (0-based).
 * @param j Column index (0-based).
 * @return The linear offset i * stride_r + j.
 * @pre 0 <= i < rows, 0 <= j < cols (asserted).
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
 * @brief Fill a matrix with random doubles in [low, high].
 * @param m Matrix to fill.
 * @param low Lower bound (inclusive).
 * @param high Upper bound (inclusive).
 * @pre high > low.
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
 * @param dst Destination matrix (size a.rows x b.cols).
 * @param a Left matrix (size a.rows x a.cols).
 * @param b Right matrix (size a.cols x b.cols).
 * @pre a.cols == b.rows, dst.rows == a.rows, dst.cols == b.cols.
 * @post dst is overwritten.
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
 * @param a Vector (shape n x 1 or 1 x n).
 * @param b Vector (same shape as a).
 * @return The scalar dot product sum.
 * @pre a.rows == b.rows, a.cols == b.cols, and one dimension equals 1.
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
 * @param dst Destination matrix to be incremented.
 * @param a Summand of same shape as dst.
 * @pre Shapes match.
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
 * @pre Shapes match.
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
 * @pre m.rows == m.cols.
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
 * @brief Reduce a matrix to identity via Gauss-Jordan elimination and return
 *        the cumulative scaling factor.
 * @param m Matrix reduced in-place to identity (if nonsingular).
 * @return The product of row scaling factors applied during elimination.
 * @note Intended as a helper for determinant-related operations.
 * @warning Not robust to singular or ill-conditioned matrices.
 */
double mat2D_make_identity(Mat2D m)
{
    /* make identity matrix using Gauss elimination */
    /* preforming Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */
    /* returns the factor multiplying the determinant */

    double factor_to_return = mat2D_upper_triangulate(m);
    
    double factor = 1 / MAT2D_AT(m, m.rows-1, m.cols-1);
    mat2D_mult_row(m, m.rows-1, factor);
    factor_to_return *= factor;
    for (size_t c = m.cols-1; c > 0; c--) {
        double factor = 1 / MAT2D_AT(m, c, c);
        mat2D_mult_row(m, c, factor);
        for (int r = c-1; r >= 0; r--) {
            mat2D_sub_row_time_factor_to_row(m, r, c, MAT2D_AT(m, r, c));
        }
    }


    return factor_to_return;
}

/**
 * @brief Set a 3x3 rotation matrix for rotation about the X-axis.
 * @param m 3x3 destination matrix.
 * @param angle_deg Angle in degrees.
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
 * @param m 3x3 destination matrix.
 * @param angle_deg Angle in degrees.
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
 * @param m 3x3 destination matrix.
 * @param angle_deg Angle in degrees.
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
 * @details Computes DCM = R_x(roll) * R_y(pitch) * R_z(yaw).
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
void mat2D_copy_mat_to_mat_at_window(Mat2D des, Mat2D src, size_t is, size_t js, size_t ie, size_t je)
{
    MAT2D_ASSERT(je >= js && ie >= is);
    MAT2D_ASSERT(je-js+1 == des.cols);
    MAT2D_ASSERT(ie-is+1 == des.rows);

    for (size_t index = 0; index < des.rows; ++index) {
        for (size_t jndex = 0; jndex < des.cols; ++jndex) {
            MAT2D_AT(des, index, jndex) = MAT2D_AT(src, is+index, js+jndex);
        }
    }
}

/**
 * @brief Copy a column from src into a column of des.
 * @param des Destination matrix (same row count as src).
 * @param des_col Column index in destination.
 * @param src Source matrix.
 * @param src_col Column index in source.
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
 * @brief Add a source column into a destination column: des[:, des_col] += src[:, src_col].
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
 * @brief Subtract a source column from a destination column: des[:, des_col] -= src[:, src_col].
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
 * @brief Add a source row into a destination row: des[des_row, :] += src[src_row, :].
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
 * @brief Subtract a source row from a destination row: des[des_row, :] -= src[src_row, :].
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

/**
 * @brief Check if all elements of a matrix equal a given digit.
 * @param m Matrix.
 * @param digit Value to compare.
 * @return true if every element equals digit, false otherwise.
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
 * @return det(m) = a11 a22 - a12 a21.
 */
double mat2D_det_2x2_mat(Mat2D m)
{
    MAT2D_ASSERT(2 == m.cols && 2 == m.rows && "Not a 2x2 matrix");
    return MAT2D_AT(m, 0, 0) * MAT2D_AT(m, 1, 1) - MAT2D_AT(m, 0, 1) * MAT2D_AT(m, 1, 0);
}

/**
 * @brief Forward elimination to transform a matrix to upper triangular form.
 * @param m Matrix transformed in-place.
 * @return Product of row scaling factors (currently 1 in this implementation).
 * @note Used as part of determinant computation via triangularization.
 * @warning Not robust for linearly dependent rows or tiny pivots.
 */
double mat2D_upper_triangulate(Mat2D m)
{
    /* preforming Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */
    /* returns the factor multiplying the determinant */

    double factor_to_return = 1;

    size_t size = (size_t)fmin(m.rows, m.cols);
    for (size_t i = 0; i < size; i++) {
        if (MAT2D_IS_ZERO(MAT2D_AT(m, i, i))) {   /* swapping only if it is zero */
            /* finding biggest first number (absolute value) */
            size_t biggest_r = i;
            for (size_t index = i; index < m.rows; index++) {
                if (fabs(MAT2D_AT(m, index, i)) > fabs(MAT2D_AT(m, biggest_r, i))) {
                    biggest_r = index;
                }
            }
            if (i != biggest_r) {
                mat2D_swap_rows(m, i, biggest_r);
                factor_to_return *= -1;
            }
        }
        for (size_t j = i+1; j < m.rows; j++) {
            double factor = 1 / MAT2D_AT(m, i, i);
            if (!isfinite(factor)) {
                printf("%s:%d:\n%s:\n[Error] unable to transfrom into uperr triangular matrix. Probably some of the rows are not independent.\n", __FILE__, __LINE__, __func__);
            }
            double mat_value = MAT2D_AT(m, j, i);
            mat2D_sub_row_time_factor_to_row(m, j, i, mat_value * factor);
        }
    }
    return factor_to_return;
}

/**
 * @brief Determinant of an NxN matrix via Gaussian elimination.
 * @param m Square matrix.
 * @return det(m).
 * @details Copies m internally, triangulates it, and returns the product of
 *          diagonal elements (adjusted by any scaling factor as implemented).
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

    /* This is an implementation of naive determinant calculation using minors. This is too slow */

    // double det = 0;
    // /* TODO: finding beast row or col? */
    // for (size_t i = 0, j = 0; i < m.rows; i++) { /* first column */
    //     if (MAT2D_AT(m, i, j) < 1e-10) continue;
    //     Mat2D_Minor sub_mm = mat2D_minor_alloc_fill_from_mat(m, i, j);
    //     int factor = (i+j)%2 ? -1 : 1;
    //     if (sub_mm.cols != 2) {
    //         MAT2D_ASSERT(sub_mm.cols == sub_mm.rows && "should be a square matrix");
    //         det += MAT2D_AT(m, i, j) * (factor) * mat2D_minor_det(sub_mm);
    //     } else if (sub_mm.cols == 2 && sub_mm.rows == 2) {
    //         det += MAT2D_AT(m, i, j) * (factor) * mat2D_det_2x2_mat_minor(sub_mm);;
    //     }
    //     mat2D_minor_free(sub_mm);
    // }

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
 * @param src Input matrix A (not modified).
 * @param l Lower triangular matrix with unit diagonal (output).
 * @param p Permutation matrix (output).
 * @param u Upper triangular matrix (output).
 * @pre l, p, u are allocated to match src shape; src is square.
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
 * @param des Destination matrix (same shape as src).
 * @param src Source square matrix.
 * @pre src is square and nonsingular.
 * @details If det(src) == 0, prints an error and sets des to all zeros.
 * @warning May be numerically unstable for ill-conditioned matrices.
 */
void mat2D_invert(Mat2D des, Mat2D src)
{
    MAT2D_ASSERT(src.cols == src.rows && "should be an NxN matrix");
    MAT2D_ASSERT(des.cols == src.cols && des.rows == des.cols);

    Mat2D m = mat2D_alloc(src.rows, src.cols);
    mat2D_copy(m, src);

    mat2D_set_identity(des);

    if (!(mat2D_det(m))) {
        mat2D_fill(des, 0);
        printf("%s:%d:\n%s:\n[Error] Can't invert the matrix. Determinant is zero! Set the inverse matrix to all zeros\n", __FILE__, __LINE__, __func__);
        mat2D_free(m);
        return;
    }

    size_t size = (size_t)fmin(m.rows, m.cols);
    for (size_t i = 0; i < size; i++) {
        if (MAT2D_IS_ZERO(MAT2D_AT(m, i, i))) {   /* swapping only if it is zero */
            /* finding biggest first number (absolute value) */
            size_t biggest_r = i;
            for (size_t index = i; index < m.rows; index++) {
                if (fabs(MAT2D_AT(m, index, i)) > fabs(MAT2D_AT(m, biggest_r, i))) {
                    biggest_r = index;
                }
            }
            if (i != biggest_r) {
                mat2D_swap_rows(m, i, biggest_r);
                mat2D_swap_rows(des, i, biggest_r);
                printf("%s:%d:\n%s:\n[INFO] swapping row %zu with row %zu.\n", __FILE__, __LINE__, __func__, i, biggest_r);
            } else {
                MAT2D_ASSERT(0 && "can't inverse");
            }
        }
        for (size_t j = i+1; j < size; j++) {
            double factor = 1 / MAT2D_AT(m, i, i);
            double mat_value = MAT2D_AT(m, j, i);
            mat2D_sub_row_time_factor_to_row(m, j, i, mat_value * factor);

            mat2D_sub_row_time_factor_to_row(des, j, i, mat_value * factor);
        }
    }
    double factor = 1 / MAT2D_AT(m, m.rows-1, m.cols-1);
    mat2D_mult_row(m, m.rows-1, factor);
    mat2D_mult_row(des, des.rows-1, factor);
    for (size_t c = m.cols-1; c > 0; c--) {
        double factor = 1 / MAT2D_AT(m, c, c);
        mat2D_mult_row(m, c, factor);
        mat2D_mult_row(des, c, factor);
        for (int r = c-1; r >= 0; r--) {
            double mat_value = MAT2D_AT(m, r, c);
            mat2D_sub_row_time_factor_to_row(m, r, c, mat_value);
            mat2D_sub_row_time_factor_to_row(des, r, c, mat_value);
        }
    }

    mat2D_free(m);
}

/**
 * @brief Solve the linear system A x = B using LUP decomposition.
 * @param A Coefficient matrix (NxN).
 * @param x Solution vector (N x 1) (output).
 * @param B Right-hand side vector (N x 1).
 * @details Internally computes LUP and uses explicit inverses of L and U.
 * @warning Forming inverses explicitly can be less stable; a forward/backward
 *          substitution would be preferable for production-quality code.
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
 * @note Free rows_list and cols_list with mat2D_minor_free when done.
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
 * @note Free rows_list and cols_list with mat2D_minor_free when done.
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