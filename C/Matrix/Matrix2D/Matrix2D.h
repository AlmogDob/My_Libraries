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

#define MAT2D_PI 3.14159265358979323846

#define MAT2D_EPS 1e-15

#define MAT2D_MAX_POWER_ITERATION 100


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
 * @def MAT2D_PRINT_UINT32
 * @brief Convenience macro to print a matrix with its variable name.
 */
#define MAT2D_PRINT_UINT32(m) mat2D_print_uint32(m, #m, 0)

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

#define mat2D_min(a, b) (a) < (b) ? (a) : (b)
#define mat2D_max(a, b) (a) > (b) ? (a) : (b)

#define mat2D_dprintDOUBLE(expr) printf(#expr " = %#g\n", expr)

#define mat2D_dprintSIZE_T(expr) printf(#expr " = %zu\n", expr)

#define mat2D_dprintINT(expr) printf(#expr " = %d\n", expr)

enum mat2D_upper_triangulate_flag{
    MAT2D_ONES_ON_DIAG = 1 << 0,
    MAT2D_ROW_SWAPPING = 1 << 1,
};

void            mat2D_add(Mat2D dst, Mat2D a);
void            mat2D_add_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col);
void            mat2D_add_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row);
void            mat2D_add_row_time_factor_to_row(Mat2D m, size_t des_r, size_t src_r, double factor);
Mat2D           mat2D_alloc(size_t rows, size_t cols);
Mat2D_uint32    mat2D_alloc_uint32(size_t rows, size_t cols);

double          mat2D_calc_col_norma(Mat2D m, size_t c);
double          mat2D_calc_norma(Mat2D m);
double          mat2D_calc_norma_inf(Mat2D m);
bool            mat2D_col_is_all_digit(Mat2D m, double digit, size_t c);
void            mat2D_copy(Mat2D des, Mat2D src);
void            mat2D_copy_col_from_src_to_des(Mat2D des, size_t des_col, Mat2D src, size_t src_col);
void            mat2D_copy_row_from_src_to_des(Mat2D des, size_t des_row, Mat2D src, size_t src_row);
void            mat2D_copy_src_to_des_window(Mat2D des, Mat2D src, size_t is, size_t js, size_t ie, size_t je);
void            mat2D_copy_src_window_to_des(Mat2D des, Mat2D src, size_t is, size_t js, size_t ie, size_t je);
Mat2D           mat2D_create_col_ref(Mat2D src, size_t c);
void            mat2D_cross(Mat2D dst, Mat2D v1, Mat2D v2);

void            mat2D_dot(Mat2D dst, Mat2D a, Mat2D b);
double          mat2D_dot_product(Mat2D v1, Mat2D v2);
double          mat2D_det(Mat2D m);
double          mat2D_det_2x2_mat(Mat2D m);
double          mat2D_det_2x2_mat_minor(Mat2D_Minor mm);

void            mat2D_eig_check(Mat2D A, Mat2D eigenvalues, Mat2D eigenvectors, Mat2D res);
void            mat2D_eig_power_iteration(Mat2D A, Mat2D eigenvalues, Mat2D eigenvectors, Mat2D init_vector, bool norm_inf_vectors);

void            mat2D_fill(Mat2D m, double x);
void            mat2D_fill_sequence(Mat2D m, double start, double step);
void            mat2D_fill_uint32(Mat2D_uint32 m, uint32_t x);
bool            mat2D_find_first_non_zero_value(Mat2D m, size_t r, size_t *non_zero_col);
void            mat2D_free(Mat2D m);
void            mat2D_free_uint32(Mat2D_uint32 m);

double          mat2D_inner_product(Mat2D v);
void            mat2D_invert(Mat2D des, Mat2D src);

void            mat2D_LUP_decomposition_with_swap(Mat2D src, Mat2D l, Mat2D p, Mat2D u);

void            mat2D_make_orthogonal_Gaussian_elimination(Mat2D des, Mat2D A);
void            mat2D_make_orthogonal_modified_Gram_Schmidt(Mat2D des, Mat2D A);
bool            mat2D_mat_is_all_digit(Mat2D m, double digit);
Mat2D_Minor     mat2D_minor_alloc_fill_from_mat(Mat2D ref_mat, size_t i, size_t j);
Mat2D_Minor     mat2D_minor_alloc_fill_from_mat_minor(Mat2D_Minor ref_mm, size_t i, size_t j);
double          mat2D_minor_det(Mat2D_Minor mm);
void            mat2D_minor_free(Mat2D_Minor mm);
void            mat2D_minor_print(Mat2D_Minor mm, const char *name, size_t padding);
void            mat2D_mult(Mat2D m, double factor);
void            mat2D_mult_row(Mat2D m, size_t r, double factor);

size_t          mat2D_offset2d(Mat2D m, size_t i, size_t j);
size_t          mat2D_offset2d_uint32(Mat2D_uint32 m, size_t i, size_t j);
void            mat2D_outer_product(Mat2D des, Mat2D v);

int             mat2D_power_iterate(Mat2D A, Mat2D v, double *lambda, double shift, bool norm_inf_v);
void            mat2D_print(Mat2D m, const char *name, size_t padding);
void            mat2D_print_uint32(Mat2D_uint32 m, const char *name, size_t padding);
void            mat2D_print_as_col(Mat2D m, const char *name, size_t padding);

void            mat2D_rand(Mat2D m, double low, double high);
double          mat2D_rand_double(void);
size_t          mat2D_reduce(Mat2D m);
bool            mat2D_row_is_all_digit(Mat2D m, double digit, size_t r);

void            mat2D_set_DCM_zyx(Mat2D DCM, float yaw_deg, float pitch_deg, float roll_deg);
void            mat2D_set_identity(Mat2D m);
void            mat2D_set_rot_mat_x(Mat2D m, float angle_deg);
void            mat2D_set_rot_mat_y(Mat2D m, float angle_deg);
void            mat2D_set_rot_mat_z(Mat2D m, float angle_deg);
void            mat2D_shift(Mat2D m, double shift);
void            mat2D_solve_linear_sys_LUP_decomposition(Mat2D A, Mat2D x, Mat2D B);
void            mat2D_sub(Mat2D dst, Mat2D a);
void            mat2D_sub_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col);
void            mat2D_sub_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row);
void            mat2D_sub_row_time_factor_to_row(Mat2D m, size_t des_r, size_t src_r, double factor);
void            mat2D_SVD_full(Mat2D A, Mat2D U, Mat2D S, Mat2D V, Mat2D init_vec_u, Mat2D init_vec_v, bool return_v_transpose);
void            mat2D_SVD_thin(Mat2D A, Mat2D U, Mat2D S, Mat2D V, Mat2D init_vec_u, Mat2D init_vec_v, bool return_v_transpose);
void            mat2D_swap_rows(Mat2D m, size_t r1, size_t r2);

void            mat2D_transpose(Mat2D des, Mat2D src);

double          mat2D_upper_triangulate(Mat2D m, uint8_t flags);

#endif // MATRIX2D_H_

#ifdef MATRIX2D_IMPLEMENTATION
#undef MATRIX2D_IMPLEMENTATION


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
 * @brief Add a source row into a destination row.
 *
 * @details
 * Performs: des[des_row, :] += src[src_row, :]
 *
 * @param des Destination matrix (same number of columns as @p src ).
 * @param des_row Row index in destination.
 * @param src Source matrix.
 * @param src_row Row index in source.
 *
 * @pre des.cols == src.cols
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
 * @brief Compute the Euclidean (L2) norm of a matrix column.
 *
 * @param m Matrix.
 * @param c Column index.
 * @return \(\sqrt{\sum_{i=0}^{m.rows-1} m_{i,c}^2}\).
 *
 * @pre c < m.cols
 */
double mat2D_calc_col_norma(Mat2D m, size_t c)
{
    MAT2D_ASSERT(c < m.cols);

    double sum = 0;
    for (size_t i = 0; i < m.rows; ++i) {
        sum += MAT2D_AT(m, i, c) * MAT2D_AT(m, i, c);
    }
    return sqrt(sum);
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
 * @brief Compute the maximum absolute element value of a matrix.
 *
 * @param m Matrix.
 * @return The element-wise maximum: \(\max_{i,j} |m_{ij}|\).
 *
 * @note Despite the name, this is not the induced matrix infinity norm
 *       (maximum row sum). It is the max-absolute-entry metric.
 */
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
 * @brief Copy a column from src into a column of des.
 * @param des Destination matrix (same row count as src).
 * @param des_col Column index in destination.
 * @param src Source matrix.
 * @param src_col Column index in source.
 * 
 * @pre des.rows == src.rows
 * @pre des_col < des.cols and src_col < src.cols
 */ 
void mat2D_copy_col_from_src_to_des(Mat2D des, size_t des_col, Mat2D src, size_t src_col)
{
    MAT2D_ASSERT(src_col < src.cols);
    MAT2D_ASSERT(des.rows == src.rows);
    MAT2D_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        MAT2D_AT(des, i, des_col) = MAT2D_AT(src, i, src_col);
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
void mat2D_copy_row_from_src_to_des(Mat2D des, size_t des_row, Mat2D src, size_t src_row)
{
    MAT2D_ASSERT(src_row < src.rows);
    MAT2D_ASSERT(des.cols == src.cols);
    MAT2D_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        MAT2D_AT(des, des_row, j) = MAT2D_AT(src, src_row, j);
    }
}

/**
 * @brief Copy @p src into a window of @p des.
 *
 * @details
 * Copies the entire @p src matrix into @p des at the rectangular region:
 * rows [@p is, @p ie] and columns [@p js, @p je] (inclusive).
 *
 * @param des Destination matrix.
 * @param src Source matrix copied into the destination window.
 * @param is Start row index in destination (inclusive).
 * @param js Start column index in destination (inclusive).
 * @param ie End row index in destination (inclusive).
 * @param je End column index in destination (inclusive).
 *
 * @pre (je - js + 1) == src.cols and (ie - is + 1) == src.rows.
 */
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

/**
 * @brief Create a non-owning column "view" into an existing matrix.
 *
 * @details Returns a Mat2D with shape (src.rows x 1) whose elements pointer
 *          refers to the first element of column @p c in @p src. The returned
 *          view preserves src.stride_r and therefore works for both contiguous
 *          and strided matrices.
 *
 * @param src Source matrix.
 * @param c Column index in @p src.
 * @return A shallow Mat2D view (does not allocate, does not own memory).
 *
 * @pre c < src.cols
 * @warning The returned matrix aliases @p src: modifying it modifies @p src.
 * @warning The returned view is invalid after @p src.elements is freed/reallocated.
 */
Mat2D mat2D_create_col_ref(Mat2D src, size_t c)
{
    MAT2D_ASSERT(c < src.cols);

    Mat2D col = {.cols = 1,
                 .rows = src.rows,
                 .stride_r = src.stride_r,
                 .elements = &(MAT2D_AT(src, 0, c))};

    return col;
}

/**
 * @brief 3D cross product: dst = a x b for 3x1 vectors.
 * @param dst 3x1 destination vector.
 * @param a 3x1 input vector.
 * @param b 3x1 input vector.
 * @pre All matrices have shape 3x1.
 */
void mat2D_cross(Mat2D dst, Mat2D v1, Mat2D v2)
{
    MAT2D_ASSERT(3 == dst.rows && 1 == dst.cols);
    MAT2D_ASSERT(3 == v1.rows && 1 == v1.cols);
    MAT2D_ASSERT(3 == v2.rows && 1 == v2.cols);

    MAT2D_AT(dst, 0, 0) = MAT2D_AT(v1, 1, 0) * MAT2D_AT(v2, 2, 0) - MAT2D_AT(v1, 2, 0) * MAT2D_AT(v2, 1, 0);
    MAT2D_AT(dst, 1, 0) = MAT2D_AT(v1, 2, 0) * MAT2D_AT(v2, 0, 0) - MAT2D_AT(v1, 0, 0) * MAT2D_AT(v2, 2, 0);
    MAT2D_AT(dst, 2, 0) = MAT2D_AT(v1, 0, 0) * MAT2D_AT(v2, 1, 0) - MAT2D_AT(v1, 1, 0) * MAT2D_AT(v2, 0, 0);
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
double mat2D_dot_product(Mat2D v1, Mat2D v2)
{
    MAT2D_ASSERT(v1.rows == v2.rows);
    MAT2D_ASSERT(v1.cols == v2.cols);
    MAT2D_ASSERT((1 == v1.cols && 1 == v2.cols) || (1 == v1.rows && 1 == v2.rows));

    double dot_product = 0;

    if (1 == v1.cols) {
        for (size_t i = 0; i < v1.rows; i++) {
            dot_product += MAT2D_AT(v1, i, 0) * MAT2D_AT(v2, i, 0);
        }
    } else {
        for (size_t j = 0; j < v1.cols; j++) {
            dot_product += MAT2D_AT(v1, 0, j) * MAT2D_AT(v2, 0, j);
        }
    }
    
    return dot_product;
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
    double factor = mat2D_upper_triangulate(temp_m, MAT2D_ROW_SWAPPING);
    double diag_mul = 1; 
    for (size_t i = 0; i < temp_m.rows; i++) {
        diag_mul *= MAT2D_AT(temp_m, i, i);
    }
    mat2D_free(temp_m);

    return diag_mul / factor;
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
 * @brief Check an eigen-decomposition by forming the residual \(A V - V \Lambda\).
 *
 * @param A Square matrix (N x N).
 * @param eigenvalues Diagonal matrix \(\Lambda\) (N x N).
 * @param eigenvectors Matrix of eigenvectors \(V\) (N x N), typically with
 *                     eigenvectors stored as columns.
 * @param res Destination matrix (N x N) receiving the residual.
 *
 * @post @p res is overwritten with \(A V - V \Lambda\).
 *
 * @pre All inputs are N x N and shapes match.
 */
void mat2D_eig_check(Mat2D A, Mat2D eigenvalues, Mat2D eigenvectors, Mat2D res)
{
    MAT2D_ASSERT(A.cols == A.rows); 
    MAT2D_ASSERT(eigenvalues.cols == A.cols);
    MAT2D_ASSERT(eigenvalues.rows == A.rows);
    MAT2D_ASSERT(eigenvectors.cols == A.cols);
    MAT2D_ASSERT(eigenvectors.rows == A.rows);
    MAT2D_ASSERT(res.cols == A.cols);
    MAT2D_ASSERT(res.rows == A.rows);

    #if 1
    mat2D_dot(res, A, eigenvectors);
    Mat2D VL = mat2D_alloc(A.rows, A.cols);
    mat2D_dot(VL, eigenvectors, eigenvalues);

    mat2D_sub(res, VL);

    mat2D_free(VL);
    #else
    Mat2D temp_v = mat2D_alloc(A.rows, 1);
    for (size_t i = 0; i < A.rows; i++) {
        Mat2D eig_vector = {.cols = 1,
                            .elements = &MAT2D_AT(eigenvectors, 0, i),
                            .rows = A.rows,
                            .stride_r = eigenvectors.stride_r};
        Mat2D v = {.cols = 1,
                   .elements = &MAT2D_AT(res, 0, i),
                   .rows = A.rows,
                   .stride_r = res.stride_r};
        
        mat2D_dot(temp_v, A, eig_vector);
        mat2D_copy(v, eig_vector);
        mat2D_mult(v, MAT2D_AT(eigenvalues, i, i ));

        mat2D_sub(v, temp_v);
    }
    mat2D_free(temp_v);
    #endif
}

/**
 * @brief Estimate eigenvalues/eigenvectors using repeated power iteration with deflation.
 *
 * @details
 * Repeatedly applies mat2D_power_iterate() to estimate an eigenpair of the
 * current matrix B, stores it into @p eigenvalues and @p eigenvectors, then
 * deflates B by subtracting \(\lambda v v^T\).
 *
 * The vector @p init_vector is copied into each eigenvector slot as the initial
 * guess before running iteration.
 *
 * @param A Input square matrix (N x N).
 * @param[out] eigenvalues Destination (N x N) written as a diagonal matrix.
 * @param[out] eigenvectors Destination (N x N) whose columns are the estimated
 *                          eigenvectors.
 * @param init_vector Initial guess (N x 1), must have non-zero norm.
 * @param norm_inf_vectors If true, each output eigenvector column is normalized
 *                         by mat2D_normalize_inf().
 *
 * @warning This implementation is primarily educational and makes strong
 *          assumptions; it may fail or be inaccurate for matrices that do not
 *          satisfy the power-iteration convergence conditions.
 *
 * @pre A is square; eigenvalues/eigenvectors are N x N; init_vector is N x 1.
 */
void mat2D_eig_power_iteration(Mat2D A, Mat2D eigenvalues, Mat2D eigenvectors, Mat2D init_vector, bool norm_inf_vectors)
{
    /* https://www.youtube.com/watch?v=c8DIOzuZqBs */

    /** Conditions:
     *      - The eigenvectors must form an orthonormal basis 
     *      - The largest eigenvalue must be positive and unique
     */ 

    MAT2D_ASSERT(A.cols == A.rows); 
    MAT2D_ASSERT(eigenvalues.cols == A.cols);
    MAT2D_ASSERT(eigenvalues.rows == A.rows);
    MAT2D_ASSERT(eigenvectors.cols == A.cols);
    MAT2D_ASSERT(eigenvectors.rows == A.rows);
    MAT2D_ASSERT(init_vector.cols == 1);
    MAT2D_ASSERT(init_vector.rows == A.rows);
    MAT2D_ASSERT(mat2D_calc_norma_inf(init_vector) > 0);

    mat2D_set_identity(eigenvalues);
    Mat2D B = mat2D_alloc(A.rows, A.cols);
    Mat2D temp_mat = mat2D_alloc(A.rows, A.cols);
    mat2D_copy(B, A);

    for (int i = 0, shift_value = 0; i < (int)A.rows; i++) {
        mat2D_copy_src_to_des_window(eigenvectors, init_vector, 0, i, init_vector.rows-1, i);
        Mat2D v = {.cols = init_vector.cols,
                   .elements = &MAT2D_AT(eigenvectors, 0, i),
                   .rows = init_vector.rows,
                   .stride_r = eigenvectors.stride_r};
        if (mat2D_power_iterate(B, v, &MAT2D_AT(eigenvalues, i, i), shift_value, 0)) { /* norm_inf_v must be zero*/
            shift_value++;
            i--;
            continue;
        } else {
            shift_value = 0;
        }
        mat2D_outer_product(temp_mat, v);
        mat2D_mult(temp_mat, MAT2D_AT(eigenvalues, i, i));
        mat2D_sub(B, temp_mat);
    }

    if (norm_inf_vectors) {
        for (size_t c = 0; c < eigenvectors.cols; c++) {
            Mat2D v = {.cols = init_vector.cols,
                    .elements = &MAT2D_AT(eigenvectors, 0, c),
                    .rows = init_vector.rows,
                    .stride_r = eigenvectors.stride_r};
            mat2D_normalize_inf(v);
        }
    }

    mat2D_free(B);
    mat2D_free(temp_mat);
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
 * @brief Find the first non-zero (per MAT2D_EPS) element in a row.
 *
 * @param m Matrix to search.
 * @param r Row index to search (0-based).
 * @param[out] non_zero_col On success, receives the column index of the first
 *                          element in row @p r such that !MAT2D_IS_ZERO(value).
 * @return true if a non-zero element was found, false if the row is all zeros
 *         (within MAT2D_EPS).
 *
 * @note Scans columns from 0 to m.cols-1 (left to right).
 */
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
 * @brief Compute the inner product of a vector with itself: dot(v, v).
 *
 * @param v Vector (shape n x 1 or 1 x n).
 * @return \(\sum_k v_k^2\) (the squared Euclidean norm).
 *
 * @pre v.cols == 1 || v.rows == 1
 */
double mat2D_inner_product(Mat2D v)
{
    MAT2D_ASSERT((1 == v.cols) || (1 == v.rows));

    double dot_product = 0;

    if (1 == v.cols) {
        for (size_t i = 0; i < v.rows; i++) {
            dot_product += MAT2D_AT(v, i, 0) * MAT2D_AT(v, i, 0);
        }
    } else {
        for (size_t j = 0; j < v.cols; j++) {
            dot_product += MAT2D_AT(v, 0, j) * MAT2D_AT(v, 0, j);
        }
    }
    
    return dot_product;
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
 * Forms an augmented matrix [src | I], performs Gauss-Jordan style reduction
 * in-place (via mat2D_reduce()), and then copies the right half into @p des.
 *
 * @warning This routine does not explicitly detect singular matrices. If
 *          @p src is singular (or nearly singular), mat2D_reduce() may assert
 *          on a near-zero pivot or produce unstable/undefined results.
 */
void mat2D_invert(Mat2D des, Mat2D src)
{
    MAT2D_ASSERT(src.cols == src.rows && "Must be an NxN matrix");
    MAT2D_ASSERT(des.cols == src.cols && des.rows == des.cols);

    Mat2D m = mat2D_alloc(src.rows, src.cols * 2);
    mat2D_copy_src_to_des_window(m, src, 0, 0, src.rows-1, src.cols-1);

    mat2D_set_identity(des);
    mat2D_copy_src_to_des_window(m, des, 0, src.cols, des.rows-1, 2 * des.cols-1);
    
    mat2D_reduce(m);

    mat2D_copy_src_window_to_des(des, m, 0, src.cols, des.rows-1, 2 * des.cols-1);

    mat2D_free(m);
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

    for (size_t i = 0; i < (size_t)mat2D_min(u.rows-1, u.cols); i++) {
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
 * @brief Attempt to build an orthogonal(ized) matrix from A using Gaussian elimination.
 *
 * @details
 * This routine follows the idea sketched in the in-body notes:
 * it forms \(A^T\) and \(A^T A\), augments \([A^T A \mid A^T]\), performs
 * elimination, then transposes the resulting right block into @p des.
 *
 * Mathematical condition (from the function's internal comment):
 *  - \(A^T A\) must be full rank (invertible). Equivalently, the columns of
 *    \(A\) must be linearly independent and non-zero.
 *
 * @param des Destination matrix.
 * @param A Input matrix.
 *
 * @pre des.rows == A.rows and des.cols == A.cols
 * @warning This is an educational routine; it is not a standard QR/GS
 *          implementation and may be numerically unstable.
 * @warning Prints debug output via MAT2D_PRINT(temp).
 */
void mat2D_make_orthogonal_Gaussian_elimination(Mat2D des, Mat2D A)
{
    /* https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process */
    /** 
     * A^TA must be fully ranked, i.e. 
     * columns must be linearly independent and non zero.
     * 
     */

    MAT2D_ASSERT(des.cols == A.cols);
    MAT2D_ASSERT(des.rows == A.rows);

    Mat2D AT = mat2D_alloc(A.cols, A.rows);
    Mat2D ATA = mat2D_alloc(A.cols, A.cols);
    Mat2D temp = mat2D_alloc(ATA.rows, ATA.cols + A.cols);
    Mat2D temp_des = mat2D_alloc(des.cols, des.rows);

    mat2D_transpose(AT, A);
    mat2D_dot(ATA, AT, A);
    mat2D_copy_src_to_des_window(temp, ATA, 0, 0, ATA.rows-1, ATA.cols-1);
    mat2D_copy_src_to_des_window(temp, AT, 0, ATA.cols, AT.rows-1, ATA.cols + AT.cols-1);

    MAT2D_PRINT(temp);

    mat2D_upper_triangulate(temp, MAT2D_ONES_ON_DIAG);

    mat2D_copy_src_window_to_des(temp_des, temp, 0, ATA.cols, AT.rows-1, ATA.cols + AT.cols-1);

    mat2D_transpose(des, temp_des);

    MAT2D_PRINT(temp);

    mat2D_free(AT);
    mat2D_free(ATA);
    mat2D_free(temp);
    mat2D_free(temp_des);
}

/**
 * @brief Build an orthonormal basis using modified Gram-Schmidt.
 *
 * @details
 * Uses a modified Gram-Schmidt process on the columns of @p des.
 * The implementation copies the leading non-zero columns of @p A into @p des,
 * and initializes the remaining columns of @p des with random values before
 * orthogonalization/normalization, attempting to complete a full basis.
 *
 * Mathematical conditions:
 *  - Gram-Schmidt requires non-zero vectors. This code stops copying columns
 *    from @p A once it encounters a column with (near) zero norm.
 *  - For stable/meaningful results, the set of input columns you expect to
 *    preserve should be linearly independent; otherwise a vector can become
 *    (near) zero during orthogonalization and normalization may divide by zero.
 *
 * @param des Destination matrix (overwritten).
 * @param A Input matrix providing initial columns.
 *
 * @pre des.rows == A.rows
 * @pre des.cols == des.rows (destination is square)
 *
 * @warning Uses rand() via mat2D_rand() for the extra columns.
 */
void mat2D_make_orthogonal_modified_Gram_Schmidt(Mat2D des, Mat2D A)
{
    /* https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process */
    MAT2D_ASSERT(des.rows == A.rows);
    MAT2D_ASSERT(des.cols == des.rows);

    size_t num_non_zero_vec = 0;
    for (size_t c = 0; c < A.cols; c++) {
        if (MAT2D_IS_ZERO(mat2D_calc_col_norma(A, c))) {
            break;
        }
        num_non_zero_vec++;
    }

    mat2D_dprintSIZE_T(num_non_zero_vec);

    mat2D_rand(des, 1, 2);

    Mat2D temp_col = mat2D_alloc(des.rows, 1);
    for (size_t c = 0; c < num_non_zero_vec; c++) {
        mat2D_copy_col_from_src_to_des(des, c, A, c);
    }
    for (size_t c = 0; c < des.cols-1; c++) {
        Mat2D vc = mat2D_create_col_ref(des, c);
        double vc_vc = mat2D_inner_product(vc);
        for (size_t k = c+1; k < des.cols; k++) {
            mat2D_copy(temp_col, vc);
            Mat2D vk = mat2D_create_col_ref(des, k);
            double vk_vc = mat2D_dot_product(vc, vk);
            mat2D_mult(temp_col, vk_vc / vc_vc);
            mat2D_sub(vk, temp_col);
        }
        if (!MAT2D_IS_ZERO(mat2D_calc_norma(vc))) {
            mat2D_normalize(vc);
        }
    }
    Mat2D vc = mat2D_create_col_ref(des, des.cols-1);
    if (!MAT2D_IS_ZERO(mat2D_calc_norma(vc))) {
        mat2D_normalize(vc);
    }


    mat2D_free(temp_col);    
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
 * @brief Compute the outer product of a vector with itself: des = v * v^T.
 *
 * @param des Destination square matrix (n x n).
 * @param v Vector (shape n x 1 or 1 x n).
 *
 * @post @p des is fully overwritten with \(v v^T\).
 *
 * @pre des.rows == des.cols
 * @pre (v.cols == 1 && des.rows == v.rows) || (v.rows == 1 && des.cols == v.cols)
 */
void mat2D_outer_product(Mat2D des, Mat2D v)
{
    MAT2D_ASSERT(des.cols == des.rows);
    MAT2D_ASSERT((1 == v.cols && des.rows == v.rows) || (1 == v.rows && des.cols == v.cols));

    // mat2D_fill(des, 0);

    if (1 == v.cols) {
        for (size_t i = 0; i < des.rows; i++) {
            for (size_t j = 0; j < des.cols; j++) {
                MAT2D_AT(des, i, j) = MAT2D_AT(v, i, 0) * MAT2D_AT(v, j, 0);
            }
        }
    } else {
        for (size_t i = 0; i < des.rows; i++) {
            for (size_t j = 0; j < des.cols; j++) {
                MAT2D_AT(des, i, j) = MAT2D_AT(v, 0, i) * MAT2D_AT(v, 0, j);
            }
        }
    }
}

/**
 * @brief Approximate an eigenpair using (shifted) power iteration.
 *
 * @details
 * Runs power iteration on the shifted matrix \(B = A - \text{shift} \cdot I\).
 * The input/output vector @p v is iteratively updated (in-place) and normalized.
 * An eigenvalue estimate is written to @p lambda (if non-NULL) as:
 *   \(\lambda \approx \lambda(B) + \text{shift}\).
 *
 * @param A Square matrix (N x N).
 * @param[in,out] v Initial guess vector (N x 1). Overwritten with the estimated
 *                  dominant eigenvector of the shifted matrix.
 * @param[out] lambda Optional output for the eigenvalue estimate (may be NULL).
 * @param shift Diagonal shift applied as described above.
 * @param norm_inf_v If true, normalize @p v at the end by mat2D_normalize_inf().
 *
 * @retval 0 Converged (difference below MAT2D_EPS within MAT2D_MAX_POWER_ITERATION).
 * @retval 1 Did not converge (often corresponds to sign-flip/alternation behavior).
 *
 * @pre A is square and v has shape (A.rows x 1).
 */
int mat2D_power_iterate(Mat2D A, Mat2D v, double *lambda, double shift, bool norm_inf_v)
{
    /* https://www.youtube.com/watch?v=SkPusgctgpI */

    /** Conditions:
     *      - The eigenvectors must form a basis 
     *      - The largest eigenvalue must be positive and unique
     */ 

    MAT2D_ASSERT(A.cols == A.rows); 
    MAT2D_ASSERT(v.cols == 1);
    MAT2D_ASSERT(v.rows == A.rows);
    MAT2D_ASSERT(mat2D_calc_norma_inf(v) > 0);

    Mat2D current_v = mat2D_alloc(v.rows, v.cols);
    Mat2D temp_v = mat2D_alloc(v.rows, v.cols);
    Mat2D B = mat2D_alloc(A.rows, A.cols);
    mat2D_copy(B, A);
    mat2D_shift(B, shift * -1.0);

    double temp_lambda = 0;
    double diff = 0;

    /* Rayleigh quotient */
    mat2D_dot(temp_v, B, v);
    temp_lambda = mat2D_dot_product(temp_v, v) / (mat2D_calc_norma(v) * mat2D_calc_norma(v));
    int i = 0;
    for (i = 0; i < MAT2D_MAX_POWER_ITERATION; i++) {
        mat2D_copy(current_v, v);
        mat2D_dot(v, B, current_v);
        mat2D_normalize(v);
        mat2D_mult(v, temp_lambda > 0 ? 1 : -1);
        // mat2D_mult(v, fabs(lambda) / lambda);
        mat2D_dot(temp_v, B, v);
        temp_lambda = mat2D_dot_product(temp_v, v);

        mat2D_sub(current_v, v);
        diff = mat2D_calc_norma_inf(current_v);
        if (diff < MAT2D_EPS) {
            break;
        }
    }

    mat2D_free(current_v);
    mat2D_free(temp_v);
    mat2D_free(B);

    if (norm_inf_v) mat2D_normalize_inf(v);
    if (lambda) *lambda = temp_lambda + shift;

    if (diff > MAT2D_EPS) {
        return 1; /* eigenvector alternating between two options */
    } else {
        return 0;
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
 * @brief Print a matrix to stdout with a name and indentation padding.
 * @param m Matrix to print.
 * @param name Label to print.
 * @param padding Left padding in spaces.
 */
void mat2D_print_uint32(Mat2D_uint32 m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows; ++i) {
        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < m.cols; ++j) {
            printf("%#10X ", MAT2D_AT_UINT32(m, i, j));
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
 * @brief Reduce a matrix in-place to reduced row echelon form (RREF) and return its rank.
 *
 * @param m Matrix modified in-place.
 * @return The computed rank (number of pivot rows found).
 *
 * @details
 * Internally calls mat2D_upper_triangulate() and then performs backward
 * elimination and row scaling to produce a reduced row echelon form.
 *
 * @note When used on an augmented matrix (e.g. [A | I]), this can be used as
 *       part of Gauss-Jordan inversion when A is nonsingular.
 */
size_t mat2D_reduce(Mat2D m)
{
    /* preforming Gauss–Jordan reduction to Reduced Row Echelon Form (RREF) */
    /* Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */

    mat2D_upper_triangulate(m, MAT2D_ONES_ON_DIAG | MAT2D_ROW_SWAPPING);

    size_t rank = 0;

    for (int r = (int)m.rows-1; r >= 0; r--) {
        size_t c = m.cols-1;
        if (!mat2D_find_first_non_zero_value(m, r, &c)) {
            continue; /* row of zeros */
        }
        for (int i = 0; i < r; i++) {
            double factor = MAT2D_AT(m, i, c);
            mat2D_sub_row_time_factor_to_row(m, i, r, factor);
        }
        rank++;
    }

    return rank;
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

    float angle_rad = angle_deg * (float)MAT2D_PI / 180.0f;
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

    float angle_rad = angle_deg * (float)MAT2D_PI / 180.0f;
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

    float angle_rad = angle_deg * (float)MAT2D_PI / 180.0f;
    mat2D_set_identity(m);
    MAT2D_AT(m, 0, 0) =  cos(angle_rad);
    MAT2D_AT(m, 0, 1) =  sin(angle_rad);
    MAT2D_AT(m, 1, 0) = -sin(angle_rad);
    MAT2D_AT(m, 1, 1) =  cos(angle_rad);
}

/**
 * @brief Add a scalar shift to the diagonal: m[i,i] += shift.
 *
 * @param m Square matrix modified in-place.
 * @param shift Value added to each diagonal element.
 *
 * @pre m.rows == m.cols
 */
void mat2D_shift(Mat2D m, double shift)
{
    MAT2D_ASSERT(m.cols == m.rows); 
    for (size_t i = 0; i < m.rows; i++) {
        MAT2D_AT(m, i, i) += shift;
    }
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
 * @brief Compute a "full" SVD by post-orthogonalizing the thin result.
 *
 * @details
 * Calls @ref mat2D_SVD_thin() first, then applies @ref mat2D_make_orthogonal_modified_Gram_Schmidt()
 * to expand/orthogonalize U and V into full orthonormal bases.
 *
 * Mathematical/algorithmic conditions (inherited from internals):
 *  - mat2D_SVD_thin() uses repeated power iteration on \(A A^T\) or \(A^T A\),
 *    so convergence depends on the power-iteration conditions documented in
 *    mat2D_power_iterate()/mat2D_eig_power_iteration (dominant eigenvalue
 *    separation, suitable initial vectors, etc.).
 *  - The Gram-Schmidt completion step assumes it can produce non-zero
 *    independent vectors; if columns become (near) dependent, normalization can
 *    be unstable.
 *
 * @param A Input matrix (n x m).
 * @param U Output U (n x n).
 * @param S Output S (n x m) with singular values on its diagonal.
 * @param V Output V (m x m) or V^T depending on @p return_v_transpose.
 * @param init_vec_u Initial vector for eigen/power iteration when using \(A A^T\) (n x 1).
 * @param init_vec_v Initial vector for eigen/power iteration when using \(A^T A\) (m x 1).
 * @param return_v_transpose If true, writes V^T into @p V (in-place transpose at the end).
 *
 * @pre U.rows==U.cols==A.rows
 * @pre V.rows==V.cols==A.cols
 * @pre S.rows==A.rows and S.cols==A.cols
 */
void mat2D_SVD_full(Mat2D A, Mat2D U, Mat2D S, Mat2D V, Mat2D init_vec_u, Mat2D init_vec_v, bool return_v_transpose)
{
    mat2D_SVD_thin(A, U, S, V, init_vec_u, init_vec_v, false);

    Mat2D U_full = mat2D_alloc(U.rows, U.cols);
    Mat2D V_full = mat2D_alloc(V.rows, V.cols);

    mat2D_make_orthogonal_modified_Gram_Schmidt(U_full, U);
    mat2D_make_orthogonal_modified_Gram_Schmidt(V_full, V);

    mat2D_copy(U, U_full);
    if (return_v_transpose) {
        mat2D_transpose(V, V_full);
    } else {
        mat2D_copy(V, V_full);
    }

    mat2D_free(U_full);
    mat2D_free(V_full);
}

/**
 * @brief Compute an SVD using eigen-decomposition + power iteration (educational).
 *
 * @details
 * Implements the standard identities:
 *  - Left singular vectors: eigenvectors of \(A A^T\)
 *  - Right singular vectors: eigenvectors of \(A^T A\)
 *  - Singular values: \(\sigma_i = \sqrt{\lambda_i}\) of the chosen PSD matrix
 *
 * The function chooses:
 *  - If n <= m: compute eigenpairs of \(A A^T\) (n x n), fill U directly,
 *    then compute \(v_i = A^T u_i / \sigma_i\).
 *  - Else: compute eigenpairs of \(A^T A\) (m x m), fill V directly,
 *    then compute \(u_i = A v_i / \sigma_i\).
 *
 * Mathematical conditions:
 *  - Power iteration assumes the eigenvectors form a basis for the matrix being
 *    iterated and that the dominant eigenvalue is positive and unique, which
 *    improves convergence.
 *  - The initial vectors (@p init_vec_u / @p init_vec_v) must be non-zero and
 *    should not be (nearly) orthogonal to the dominant eigenvector(s), or
 *    convergence can be slow/fail.
 *
 * Notes on numerical behavior:
 *  - \(A A^T\) and \(A^T A\) are symmetric positive semidefinite in exact
 *    arithmetic, so eigenvalues should be non-negative. Due to floating-point
 *    error, small negative values may appear; this implementation clamps those
 *    to 0 by setting the corresponding singular value to 0.
 *
 * @param A Input matrix (n x m).
 * @param U Output matrix (n x n). Only the first min(n,m) columns corresponding
 *          to non-zero singular values are meaningfully populated by this step.
 * @param S Output matrix (n x m). Singular values written on the diagonal.
 * @param V Output matrix (m x m) (or V^T if @p return_v_transpose is true).
 * @param init_vec_u Initial vector for eigen iteration in the \(A A^T\) path (n x 1).
 * @param init_vec_v Initial vector for eigen iteration in the \(A^T A\) path (m x 1).
 * @param return_v_transpose If true, the function transposes V before returning.
 *
 * @pre U.rows==U.cols==A.rows
 * @pre V.rows==V.cols==A.cols
 * @pre S.rows==A.rows and S.cols==A.cols
 * @pre init_vec_u.rows==A.rows && init_vec_u.cols==1
 * @pre init_vec_v.rows==A.cols && init_vec_v.cols==1
 *
 * @warning This is not a production-quality SVD (no QR/SVD bidiagonalization).
 *          It is sensitive to convergence issues of power iteration and to
 *          deflation error accumulation.
 */
void mat2D_SVD_thin(Mat2D A, Mat2D U, Mat2D S, Mat2D V, Mat2D init_vec_u, Mat2D init_vec_v, bool return_v_transpose)
{
    /* https://www.youtube.com/watch?v=nbBvuuNVfco */
    /* https://en.wikipedia.org/wiki/Singular_value_decomposition */
    size_t n = A.rows;
    size_t m = A.cols;
    MAT2D_ASSERT(U.rows == n);
    MAT2D_ASSERT(U.cols == n);
    MAT2D_ASSERT(S.rows == n);
    MAT2D_ASSERT(S.cols == m);
    MAT2D_ASSERT(V.rows == m);
    MAT2D_ASSERT(V.cols == m);
    MAT2D_ASSERT(init_vec_u.rows == n);
    MAT2D_ASSERT(init_vec_u.cols == 1);
    MAT2D_ASSERT(init_vec_v.rows == m);
    MAT2D_ASSERT(init_vec_v.cols == 1);

    mat2D_fill(U, 0);
    mat2D_fill(S, 0);
    mat2D_fill(V, 0);

    Mat2D AT = mat2D_alloc(m, n);
    mat2D_transpose(AT, A);

    if (n <= m) {
        Mat2D AAT = mat2D_alloc(n, n);
        Mat2D left_eigenvalues = mat2D_alloc(n, n);
        Mat2D left_eigenvectors = mat2D_alloc(n, n);
        Mat2D temp_u_vec = mat2D_alloc(n, 1);
        Mat2D temp_v_vec = mat2D_alloc(m, 1);
        mat2D_dot(AAT, A, AT);
        mat2D_eig_power_iteration(AAT, left_eigenvalues, left_eigenvectors, init_vec_u, 0);
        /* fill matrix sigma (S) */
        size_t non_zero_n = 0;
        for (size_t i = 0; i < n; i++) {
            if (MAT2D_IS_ZERO(MAT2D_AT(left_eigenvalues, i, i)) || MAT2D_AT(left_eigenvalues, i, i) < 0) {
                MAT2D_AT(S, i, i) = 0; /* AAT is positive definet */
            } else {
                MAT2D_AT(S, i, i) = sqrt(MAT2D_AT(left_eigenvalues, i, i));
                non_zero_n++;
            }
        }
        /** 
         * fill U with the eigenvectors of AAT that have non zero singular value
         * and fill V with the corresponding eigenvector according to:
         *      v_i = A^T*u_i / sigma_i
         */
        for (size_t c = 0; c < non_zero_n; c++) {
            mat2D_copy_col_from_src_to_des(U, c, left_eigenvectors, c);
            mat2D_copy_col_from_src_to_des(temp_u_vec, 0, left_eigenvectors, c);
            mat2D_dot(temp_v_vec, AT, temp_u_vec);
            mat2D_mult(temp_v_vec, 1.0 / MAT2D_AT(S, c, c));
            mat2D_copy_col_from_src_to_des(V, c, temp_v_vec, 0);
        }
        mat2D_free(AAT);
        mat2D_free(left_eigenvalues);
        mat2D_free(left_eigenvectors);
        mat2D_free(temp_u_vec);
        mat2D_free(temp_v_vec);
    } else {
        Mat2D ATA = mat2D_alloc(m, m);
        Mat2D right_eigenvalues = mat2D_alloc(m, m);
        Mat2D right_eigenvectors = mat2D_alloc(m, m);
        Mat2D temp_u_vec = mat2D_alloc(n, 1);
        Mat2D temp_v_vec = mat2D_alloc(m, 1);
        mat2D_dot(ATA, AT, A);
        mat2D_eig_power_iteration(ATA, right_eigenvalues, right_eigenvectors, init_vec_v, 0);
        /* fill matrix sigma (S) */
        size_t non_zero_m = 0;
        for (size_t i = 0; i < m; i++) {
            if (MAT2D_IS_ZERO(MAT2D_AT(right_eigenvalues, i, i)) || MAT2D_AT(right_eigenvalues, i, i) < 0) {
                MAT2D_AT(S, i, i) = 0; /* ATA is positive definet */
            } else {
                MAT2D_AT(S, i, i) = sqrt(MAT2D_AT(right_eigenvalues, i, i));
                non_zero_m++;
            }
        }
        /** 
         * fill V with the eigenvectors of ATA that have non zero singular value
         * and fill U with the corresponding eigenvector according to:
         *      u_i = A*v_i / sigma_i
         */
        for (size_t c = 0; c < non_zero_m; c++) {
            mat2D_copy_col_from_src_to_des(V, c, right_eigenvectors, c);
            mat2D_copy_col_from_src_to_des(temp_v_vec, 0, right_eigenvectors, c);
            mat2D_dot(temp_u_vec, A, temp_v_vec);
            mat2D_mult(temp_u_vec, 1.0 / MAT2D_AT(S, c, c));
            mat2D_copy_col_from_src_to_des(U, c, temp_u_vec, 0);
        }
        mat2D_free(ATA);
        mat2D_free(right_eigenvalues);
        mat2D_free(right_eigenvectors);
        mat2D_free(temp_u_vec);
        mat2D_free(temp_v_vec);
    }

    if (return_v_transpose) {
        Mat2D v_trans = mat2D_alloc(V.cols, V.rows);
        mat2D_transpose(v_trans, V);
        mat2D_copy(V, v_trans);

        mat2D_free(v_trans);
    }

    mat2D_free(AT);
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
 * @brief Transform a matrix to (row-echelon) upper triangular form by forward elimination.
 *
 * @param m Matrix transformed in-place.
 * @return A determinant sign factor caused by row swaps: +1.0 or -1.0.
 *
 * @details
 * This routine performs Gaussian elimination using row operations of the form:
 *   row_j = row_j - (m[j,i] / m[i,i]) * row_i
 * which do not change the determinant. Row swaps flip the determinant sign and
 * are tracked by the returned factor.
 * Performs partial pivoting by selecting the row with the largest absolute
 * pivot candidate in each column. Uses elimination operations that (in exact
 * arithmetic) do not change the determinant. Each row swap flips the
 * determinant sign; the cumulative sign is returned.
 *
 * @warning Not robust for linearly dependent rows or very small pivots.
 */
double mat2D_upper_triangulate(Mat2D m, uint8_t flags)
{
    /* preforming Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */
    /* returns the factor multiplying the determinant */

    double factor_to_return = 1;

    size_t r = 0;
    for (size_t c = 0; c < m.cols && r < m.rows; c++) {
        if (flags & MAT2D_ROW_SWAPPING) {
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
        }

        double pivot = MAT2D_AT(m, r, c);
        MAT2D_ASSERT(!MAT2D_IS_ZERO(pivot));

        if (flags & MAT2D_ONES_ON_DIAG) {
            mat2D_mult_row(m, r, 1.0 / pivot);
            factor_to_return *= pivot;
            pivot = 1.0;
        }

        /* Eliminate entries below pivot in column c */
        for (size_t i = r + 1; i < m.rows; i++) {
            double f = MAT2D_AT(m, i, c) / pivot;
            mat2D_sub_row_time_factor_to_row(m, i, r, f);
        }
        r++;
    }
    return factor_to_return;
}

#endif // MATRIX2D_IMPLEMENTATION