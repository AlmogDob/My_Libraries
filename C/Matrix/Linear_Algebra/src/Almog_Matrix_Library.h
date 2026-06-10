/**
 * @file 
 * @brief Basic dense 2D matrix utilities
 *
 * The library stores matrices in row-major order. A matrix view may use a
 * custom row stride, so functions should use AML_MAT2D_AT() instead of
 * assuming contiguous compact storage.
 */

#ifndef ALMOG_MATRIX_LIBRARY_H_
#define ALMOG_MATRIX_LIBRARY_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <stdbool.h>
#include <math.h>

#if defined(_WIN32) || defined(_WIN64) 
    #pragma warning(disable : 4709)
#endif

#ifndef AML_MALLOC
#define AML_MALLOC malloc
#endif //AML_MALLOC

#ifndef AML_REALLOC
#define AML_REALLOC realloc
#endif //AML_REALLOC

#ifndef AML_FREE
#define AML_FREE free
#endif //AML_FREE

#ifndef AML_ASSERT
#include <assert.h>
#define AML_ASSERT assert
#endif //AML_ASSERT

#if defined(AML_SINGLE_PRECISION)
    typedef float aml_real;
    #define AML_EPS 1e-5f
    #define AML_INFINITY FLT_MAX
    #define aml_fmin  fminf
    #define aml_fmax  fmaxf
    #define aml_fabs  fabsf
    #define aml_sqrt  sqrtf
    #define aml_sin   sinf
    #define aml_cos   cosf
    #define aml_exp   expf
    #define aml_ceil  ceilf
    #define aml_hypot hypotf
#else 
    typedef double aml_real;
    #define AML_EPS 1e-10
    #define AML_INFINITY DBL_MAX
    #define aml_fmin  fmin
    #define aml_fmax  fmax
    #define aml_fabs  fabs
    #define aml_sqrt  sqrt
    #define aml_sin   sin
    #define aml_cos   cos
    #define aml_exp   exp
    #define aml_ceil  ceil
    #define aml_hypot hypot
#endif


/**
 * @brief Dense real-valued 2D matrix view.
 *
 * @var Aml_Mat2d::rows
 * Number of logical rows.
 * @var Aml_Mat2d::cols
 * Number of logical columns.
 * @var Aml_Mat2d::stride_r
 * Number of elements to skip in memory to move to the next row.
 * @var Aml_Mat2d::elements
 * Pointer to the first stored element.
 */
struct Aml_Mat2d {
    size_t rows;
    size_t cols;
    size_t stride_r; /* elements to traverse to reach the next row */
    aml_real *elements;
};

/**
 * @brief Dense unsigned-32-bit 2D matrix view.
 */
struct Aml_Mat2d_uint32 {
    size_t rows;
    size_t cols;
    size_t stride_r; /* elements to traverse to reach the next row */
    uint32_t *elements;
};

#define AML_MAT2D_AT(m, i, j) (m).elements[(AML_ASSERT((i) < (m).rows && (j) < (m).cols), (i) * (m).stride_r + (j))]
#define AML_PI 3.14159265358979323846
#define aml_min(a, b) ((a) < (b) ? (a) : (b))
#define aml_max(a, b) ((a) > (b) ? (a) : (b))
#define AML_IS_ZERO(x) (aml_fabs(x) < AML_EPS)
#define AML_MINOR_AT(mm, i, j) AML_MAT2D_AT((mm).ref_mat, (mm).rows_list[i], (mm).cols_list[j])
#define AML_UNUSED(x) (void)x
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define aml_dprintSTRING(expr) printf("[Info] %s:%d:\n%*s" #expr " = %s\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define aml_dprintCHAR(expr) printf("[Info] %s:%d:\n%*s" #expr " = %c\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define aml_dprintINT(expr) printf("[Info] %s:%d:\n%*s" #expr " = %d\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define aml_dprintFLOAT(expr) printf("[Info] %s:%d:\n%*s" #expr " = %#f\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define aml_dprintDOUBLE(expr) printf("[Info] %s:%d:\n%*s" #expr " = %#g\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define aml_dprintSIZE_T(expr) printf("[Info] %s:%d:\n%*s" #expr " = %zu\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define aml_dprintINFO(fmt, ...) \
    fprintf(stderr, "[Info] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 7, "", __func__, 7, "", __VA_ARGS__)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define aml_dprintWARNING(fmt, ...) \
    fprintf(stderr, "[Warning] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 10, "", __func__, 10, "", __VA_ARGS__)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define aml_dprintERROR(fmt, ...) \
    fprintf(stderr, "[Error] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 8, "", __func__, 8, "", __VA_ARGS__)
#define AML_PRINT(m) aml_dprintINFO("%s", ""); printf("\33[A\33[2K\r"); aml_print(m, #m, 7)

    // 
#define AML_PRINT_UINT32(m) aml_print_uint32(m, #m, 0)
#define AML_PRINT_AS_COL(m) aml_print_as_col(m, #m, 0)
#define AML_MINOR_PRINT(mm) aml_minor_print(mm, #mm, 0)


#ifndef AML_DEF
    #ifdef AML_DEF_STATIC
        #define AML_DEF static
    #else
        #define AML_DEF extern
    #endif
#endif

AML_DEF void                    aml_add(struct Aml_Mat2d dst, struct Aml_Mat2d a);
AML_DEF void                    aml_add_col_to_col(struct Aml_Mat2d des, size_t des_col, struct Aml_Mat2d src, size_t src_col);
AML_DEF void                    aml_add_row_to_row(struct Aml_Mat2d des, size_t des_row, struct Aml_Mat2d src, size_t src_row);
AML_DEF void                    aml_add_row_time_factor_to_row(struct Aml_Mat2d m, size_t des_r, size_t src_r, aml_real factor);
AML_DEF void                    aml_add_scalar(struct Aml_Mat2d m, aml_real x);
AML_DEF void                    aml_anti_diag_transpose_inplace(struct Aml_Mat2d m);

AML_DEF aml_real                aml_calc_col_norma(struct Aml_Mat2d m, size_t c);
AML_DEF aml_real                aml_calc_norma(struct Aml_Mat2d m);
AML_DEF aml_real                aml_calc_norma_inf(struct Aml_Mat2d m);
AML_DEF bool                    aml_col_is_all_the_same(struct Aml_Mat2d m, aml_real number, size_t c);
AML_DEF void                    aml_cols_swap(struct Aml_Mat2d m, size_t c1, size_t c2);
AML_DEF void                    aml_copy(struct Aml_Mat2d des, struct Aml_Mat2d src);
AML_DEF void                    aml_copy_uint32(struct Aml_Mat2d_uint32 des, struct Aml_Mat2d_uint32 src);
AML_DEF void                    aml_copy_col_from_src_to_des(struct Aml_Mat2d des, size_t des_col, struct Aml_Mat2d src, size_t src_col);
AML_DEF void                    aml_copy_row_from_src_to_des(struct Aml_Mat2d des, size_t des_row, struct Aml_Mat2d src, size_t src_row);
AML_DEF void                    aml_copy_src_to_des_window(struct Aml_Mat2d des, struct Aml_Mat2d src, size_t is, size_t js, size_t ie, size_t je);
AML_DEF void                    aml_copy_src_window_to_des(struct Aml_Mat2d des, struct Aml_Mat2d src, size_t is, size_t js, size_t ie, size_t je);
AML_DEF struct Aml_Mat2d        aml_create_col_ref(struct Aml_Mat2d src, size_t c);
AML_DEF void                    aml_cross(struct Aml_Mat2d dst, struct Aml_Mat2d v1, struct Aml_Mat2d v2);

AML_DEF void                    aml_dot(struct Aml_Mat2d dst, struct Aml_Mat2d a, struct Aml_Mat2d b);
AML_DEF aml_real                aml_dot_product(struct Aml_Mat2d v1, struct Aml_Mat2d v2);

AML_DEF aml_real                aml_elements_sum(struct Aml_Mat2d m);
AML_DEF aml_real                aml_elements_col_sum(struct Aml_Mat2d m, size_t c);
AML_DEF aml_real                aml_elements_row_sum(struct Aml_Mat2d m, size_t r);

AML_DEF void                    aml_fill(struct Aml_Mat2d m, aml_real x);
AML_DEF void                    aml_fill_sequence(struct Aml_Mat2d m, aml_real start, aml_real step);
AML_DEF void                    aml_fill_uint32(struct Aml_Mat2d_uint32 m, uint32_t x);

AML_DEF aml_real                aml_inner_product(struct Aml_Mat2d v);
AML_DEF bool                    aml_is_close(aml_real a, aml_real b, aml_real eps);
AML_DEF bool                    aml_is_diagonal(struct Aml_Mat2d m);
AML_DEF bool                    aml_is_hessenberg(struct Aml_Mat2d m);
AML_DEF bool                    aml_is_symmetric(struct Aml_Mat2d m);
AML_DEF bool                    aml_is_symmetric_relative(struct Aml_Mat2d m);
AML_DEF bool                    aml_is_tridiagonal(struct Aml_Mat2d m);

AML_DEF void                    aml_make_diagonal(struct Aml_Mat2d m);
AML_DEF void                    aml_make_symmetric(struct Aml_Mat2d m);
AML_DEF void                    aml_make_tridiagonal(struct Aml_Mat2d m);
AML_DEF void                    aml_make_upper_hessenberg_range(struct Aml_Mat2d m, size_t start, size_t end);
AML_DEF struct Aml_Mat2d        aml_mat2d_alloc(size_t rows, size_t cols);
AML_DEF void                    aml_mat2d_free(struct Aml_Mat2d m);
AML_DEF struct Aml_Mat2d_uint32 aml_mat2d_uint32_alloc(size_t rows, size_t cols);
AML_DEF void                    aml_mat2d_uint32_free(struct Aml_Mat2d_uint32 m);
AML_DEF bool                    aml_mat2d_is_all_the_same(struct Aml_Mat2d m, aml_real number);
AML_DEF void                    aml_mult(struct Aml_Mat2d m, aml_real factor);
AML_DEF void                    aml_mult_row(struct Aml_Mat2d m, size_t r, aml_real factor);

AML_DEF void                    aml_normalize(struct Aml_Mat2d m);
AML_DEF void                    aml_normalize_inf(struct Aml_Mat2d m);

AML_DEF size_t                  aml_offset2d(struct Aml_Mat2d m, size_t i, size_t j);
AML_DEF size_t                  aml_offset2d_uint32(struct Aml_Mat2d_uint32 m, size_t i, size_t j);
AML_DEF void                    aml_outer_product(struct Aml_Mat2d des, struct Aml_Mat2d v);

AML_DEF void                    aml_print(struct Aml_Mat2d m, const char *name, size_t padding);
AML_DEF void                    aml_print_uint32(struct Aml_Mat2d_uint32 m, const char *name, size_t padding);
AML_DEF void                    aml_print_as_col(struct Aml_Mat2d m, const char *name, size_t padding);

AML_DEF aml_real                aml_rand_aml_real(void);
AML_DEF struct Aml_Mat2d        aml_realloc(struct Aml_Mat2d m, size_t rows, size_t cols);
AML_DEF struct Aml_Mat2d_uint32 aml_realloc_uint32(struct Aml_Mat2d_uint32 m, size_t rows, size_t cols);
AML_DEF void                    aml_rotate_mat_180_deg_inplace(struct Aml_Mat2d m);
AML_DEF bool                    aml_row_find_first_non_zero_value(struct Aml_Mat2d m, size_t r, size_t *non_zero_col);
AML_DEF bool                    aml_row_is_all_the_same(struct Aml_Mat2d m, aml_real number, size_t r);
AML_DEF void                    aml_rows_swap(struct Aml_Mat2d m, size_t r1, size_t r2);

AML_DEF void                    aml_set_DCM_zyx(struct Aml_Mat2d DCM, float yaw_deg, float pitch_deg, float roll_deg);
AML_DEF void                    aml_set_identity(struct Aml_Mat2d m);
AML_DEF void                    aml_set_rand(struct Aml_Mat2d m, aml_real low, aml_real high);
AML_DEF void                    aml_set_rand_symmetric(struct Aml_Mat2d m, aml_real low, aml_real high);
AML_DEF void                    aml_set_rot_mat_x(struct Aml_Mat2d m, float angle_deg);
AML_DEF void                    aml_set_rot_mat_y(struct Aml_Mat2d m, float angle_deg);
AML_DEF void                    aml_set_rot_mat_z(struct Aml_Mat2d m, float angle_deg);
AML_DEF void                    aml_shift(struct Aml_Mat2d m, aml_real shift);
AML_DEF void                    aml_shift_specific(struct Aml_Mat2d m, aml_real shift, size_t is, size_t ie);
AML_DEF void                    aml_sub(struct Aml_Mat2d dst, struct Aml_Mat2d a);
AML_DEF void                    aml_sub_col_to_col(struct Aml_Mat2d des, size_t des_col, struct Aml_Mat2d src, size_t src_col);
AML_DEF void                    aml_sub_row_to_row(struct Aml_Mat2d des, size_t des_row, struct Aml_Mat2d src, size_t src_row);
AML_DEF void                    aml_sub_src_row_time_factor_from_des_row(struct Aml_Mat2d m, size_t des_r, size_t src_r, aml_real factor);
AML_DEF void                    aml_sub_src_row_time_factor_from_des_row_range(struct Aml_Mat2d m, size_t des_r, size_t src_r, aml_real factor, size_t js, size_t je);

AML_DEF void                    aml_transpose(struct Aml_Mat2d des, struct Aml_Mat2d src);
AML_DEF void                    aml_transpose_inplace(struct Aml_Mat2d m);

#endif // ALMOG_MATRIX_LIBRARY_H_

#ifdef ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#undef ALMOG_MATRIX_LIBRARY_IMPLEMENTATION

/**
 * @brief Add matrix @p a into @p dst element-wise.
 *
 * Computes `dst(i,j) += a(i,j)` for all elements.
 *
 * @param dst Destination matrix, modified in place.
 * @param a Matrix added to @p dst.
 * @pre The matrices must have identical dimensions.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_add(struct Aml_Mat2d dst, struct Aml_Mat2d a)
{
    AML_ASSERT(dst.rows == a.rows);
    AML_ASSERT(dst.cols == a.cols);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            AML_MAT2D_AT(dst, i, j) += AML_MAT2D_AT(a, i, j);
        }
    }
}

/**
 * @brief Add one source column into one destination column.
 *
 * Computes `des(:,des_col) += src(:,src_col)`.
 *
 * @param des Destination matrix, modified in place.
 * @param des_col Destination column index.
 * @param src Source matrix.
 * @param src_col Source column index.
 * @pre `des.rows == src.rows`.
 *
 * Complexity
 * `O(rows)`.
 */
AML_DEF void aml_add_col_to_col(struct Aml_Mat2d des, size_t des_col, struct Aml_Mat2d src, size_t src_col)
{
    AML_ASSERT(src_col < src.cols);
    AML_ASSERT(des.rows == src.rows);
    AML_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        AML_MAT2D_AT(des, i, des_col) += AML_MAT2D_AT(src, i, src_col);
    }
}

/**
 * @brief Add one source row into one destination row.
 *
 * Computes `des(des_row,:) += src(src_row,:)`.
 *
 * @param des Destination matrix, modified in place.
 * @param des_row Destination row index.
 * @param src Source matrix.
 * @param src_row Source row index.
 * @pre `des.cols == src.cols`.
 *
 * Complexity
 * `O(cols)`.
 */
AML_DEF void aml_add_row_to_row(struct Aml_Mat2d des, size_t des_row, struct Aml_Mat2d src, size_t src_row)
{
    AML_ASSERT(src_row < src.rows);
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        AML_MAT2D_AT(des, des_row, j) += AML_MAT2D_AT(src, src_row, j);
    }
}

/**
 * @brief Add a scaled source row into a destination row.
 *
 * Computes `row(des_r) += factor * row(src_r)`.
 *
 * @param m Matrix modified in place.
 * @param des_r Destination row index.
 * @param src_r Source row index.
 * @param factor Scale applied to the source row.
 *
 * Complexity
 * `O(cols)`.
 */
AML_DEF void aml_add_row_time_factor_to_row(struct Aml_Mat2d m, size_t des_r, size_t src_r, aml_real factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        AML_MAT2D_AT(m, des_r, j) += factor * AML_MAT2D_AT(m, src_r, j);
    }
}

/**
 * @brief Add a scalar to every element of a matrix.
 *
 * @param m Matrix modified in place.
 * @param x Scalar added to every entry.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_add_scalar(struct Aml_Mat2d m, aml_real x)
{
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            AML_MAT2D_AT(m, i, j) += x;
        }
    }
}

/**
 * @brief Reflect a square matrix across the anti-diagonal in place.
 *
 * This swaps entry `(i,j)` with `(n-1-j,n-1-i)`.
 *
 * @param m Square matrix modified in place.
 * @pre `m.rows == m.cols`.
 *
 * Complexity
 * `O(n^2)`.
 */
AML_DEF void aml_anti_diag_transpose_inplace(struct Aml_Mat2d m)
{
    AML_ASSERT(m.cols == m.rows);

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols - i; j++) {
            aml_real temp = AML_MAT2D_AT(m, i, j);
            AML_MAT2D_AT(m, i, j) = AML_MAT2D_AT(m, m.rows - j - 1, m.cols - i - 1);
            AML_MAT2D_AT(m, m.rows - j - 1, m.cols - i - 1) = temp;
        }
    }
}

/**
 * @brief Compute the Euclidean norm of a column.
 *
 * Returns `sqrt(sum_i m(i,c)^2)`.
 *
 * @param m Matrix containing the column.
 * @param c Column index.
 * @return Euclidean norm of column @p c.
 *
 * Complexity
 * `O(rows)`.
 */
AML_DEF aml_real aml_calc_col_norma(struct Aml_Mat2d m, size_t c)
{
    AML_ASSERT(c < m.cols);

    aml_real sum = 0;
    for (size_t i = 0; i < m.rows; ++i) {
        sum += AML_MAT2D_AT(m, i, c) * AML_MAT2D_AT(m, i, c);
    }
    return aml_sqrt(sum);
}

/**
 * @brief Compute the Frobenius norm of a matrix.
 *
 * Returns `sqrt(sum_{i,j} m(i,j)^2)`.
 *
 * @param m Input matrix.
 * @return Frobenius norm.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF aml_real aml_calc_norma(struct Aml_Mat2d m)
{
    aml_real sum = 0;

    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            sum += AML_MAT2D_AT(m, i, j) * AML_MAT2D_AT(m, i, j);
        }
    }
    return aml_sqrt(sum);
}
 
/**
 * @brief Compute the infinity-like entrywise max norm used by this library.
 *
 * This implementation returns the maximum absolute element value, not the
 * operator infinity norm.
 *
 * @param m Input matrix.
 * @return `max_{i,j} |m(i,j)|`.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF aml_real aml_calc_norma_inf(struct Aml_Mat2d m)
{
    aml_real max = 0;
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            aml_real current = aml_fabs(AML_MAT2D_AT(m, i, j));
            if (current > max) {
                max = current;
            }
        }
    }

    return max;
}

/**
 * @brief Test whether an entire column is exactly equal to one value.
 *
 * Comparison is exact, not epsilon-based.
 *
 * @param m Input matrix.
 * @param number Value to compare against.
 * @param c Column index.
 * @return `true` if every entry in the column equals @p number.
 *
 * Complexity
 * `O(rows)`.
 */
AML_DEF bool aml_col_is_all_the_same(struct Aml_Mat2d m, aml_real number, size_t c)
{
    for (size_t i = 0; i < m.rows; ++i) {
        if (AML_MAT2D_AT(m, i, c) != number) {
            return false;
        }
    }
    return true;
}

AML_DEF void aml_cols_swap(struct Aml_Mat2d m, size_t c1, size_t c2)
{
    for (size_t i = 0; i < m.rows; i++) {
        aml_real temp = AML_MAT2D_AT(m, i, c1);
        AML_MAT2D_AT(m, i, c1) = AML_MAT2D_AT(m, i, c2);
        AML_MAT2D_AT(m, i, c2) = temp;
    }
}

/**
 * @brief Copy one matrix into another.
 *
 * @param des Destination matrix.
 * @param src Source matrix.
 * @pre Dimensions must match exactly.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_copy(struct Aml_Mat2d des, struct Aml_Mat2d src)
{
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des.rows == src.rows);

    for (size_t i = 0; i < des.rows; ++i) {
        for (size_t j = 0; j < des.cols; ++j) {
            AML_MAT2D_AT(des, i, j) = AML_MAT2D_AT(src, i, j);
        }
    }
}

/**
 * @brief Copy one uint32 matrix into another.
 *
 * @param des Destination matrix.
 * @param src Source matrix.
 * @pre Dimensions must match exactly.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_copy_uint32(struct Aml_Mat2d_uint32 des, struct Aml_Mat2d_uint32 src)
{
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des.rows == src.rows);

    for (size_t i = 0; i < des.rows; ++i) {
        for (size_t j = 0; j < des.cols; ++j) {
            AML_MAT2D_AT(des, i, j) = AML_MAT2D_AT(src, i, j);
        }
    }
}

/**
 * @brief Copy one source column into one destination column.
 *
 * Computes `des(:,des_col) = src(:,src_col)`.
 *
 * @param des Destination matrix.
 * @param des_col Destination column index.
 * @param src Source matrix.
 * @param src_col Source column index.
 * @pre `des.rows == src.rows`.
 *
 * Complexity
 * `O(rows)`.
 */
AML_DEF void aml_copy_col_from_src_to_des(struct Aml_Mat2d des, size_t des_col, struct Aml_Mat2d src, size_t src_col)
{
    AML_ASSERT(src_col < src.cols);
    AML_ASSERT(des.rows == src.rows);
    AML_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        AML_MAT2D_AT(des, i, des_col) = AML_MAT2D_AT(src, i, src_col);
    }
}

/**
 * @brief Copy one source row into one destination row.
 *
 * Computes `des(des_row,:) = src(src_row,:)`.
 *
 * @param des Destination matrix.
 * @param des_row Destination row index.
 * @param src Source matrix.
 * @param src_row Source row index.
 * @pre `des.cols == src.cols`.
 *
 * Complexity
 * `O(cols)`.
 */
AML_DEF void aml_copy_row_from_src_to_des(struct Aml_Mat2d des, size_t des_row, struct Aml_Mat2d src, size_t src_row)
{
    AML_ASSERT(src_row < src.rows);
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        AML_MAT2D_AT(des, des_row, j) = AML_MAT2D_AT(src, src_row, j);
    }
}

/**
 * @brief Copy the full @p src matrix into a window of @p des.
 *
 * The top-left corner in @p des is `(is, js)`. The parameters @p ie and @p je
 * are only used to assert that the destination window size matches the source
 * size.
 *
 * @param des Destination matrix.
 * @param src Source matrix.
 * @param is Destination start row.
 * @param js Destination start column.
 * @param ie Destination end row, used for bounds/shape checks.
 * @param je Destination end column, used for bounds/shape checks.
 *
 * Complexity
 * `O(src.rows * src.cols)`.
 */
AML_DEF void aml_copy_src_to_des_window(struct Aml_Mat2d des, struct Aml_Mat2d src, size_t is, size_t js, size_t ie, size_t je)
{
    AML_ASSERT(je >= js && ie >= is);
    AML_ASSERT(je-js+1 == src.cols);
    AML_ASSERT(ie-is+1 == src.rows);
    AML_ASSERT(je-js+1 <= des.cols);
    AML_ASSERT(ie-is+1 <= des.rows);

    for (size_t index = 0; index < src.rows; ++index) {
        for (size_t jndex = 0; jndex < src.cols; ++jndex) {
            AML_MAT2D_AT(des, is+index, js+jndex) = AML_MAT2D_AT(src, index, jndex);
        }
    }

    AML_UNUSED(is);
    AML_UNUSED(js);
    AML_UNUSED(ie);
    AML_UNUSED(je);
}

/**
 * @brief Copy a rectangular window from @p src into the full @p des matrix.
 *
 * The copied source window is `[is..ie] x [js..je]`, and its size must match
 * the full size of @p des.
 *
 * @param des Destination matrix.
 * @param src Source matrix.
 * @param is Source start row.
 * @param js Source start column.
 * @param ie Source end row.
 * @param je Source end column.
 *
 * Complexity
 * `O(des.rows * des.cols)`.
 */
AML_DEF void aml_copy_src_window_to_des(struct Aml_Mat2d des, struct Aml_Mat2d src, size_t is, size_t js, size_t ie, size_t je)
{
    AML_ASSERT(je >= js && ie >= is);
    AML_ASSERT(je-js+1 == des.cols);
    AML_ASSERT(ie-is+1 == des.rows);
    AML_ASSERT(je-js+1 <= src.cols);
    AML_ASSERT(ie-is+1 <= src.rows);

    for (size_t index = 0; index < des.rows; ++index) {
        for (size_t jndex = 0; jndex < des.cols; ++jndex) {
            AML_MAT2D_AT(des, index, jndex) = AML_MAT2D_AT(src, is+index, js+jndex);
        }
    }

    AML_UNUSED(is);
    AML_UNUSED(js);
    AML_UNUSED(ie);
    AML_UNUSED(je);
}

/**
 * @brief Create a non-owning column view into an existing matrix.
 *
 * The returned matrix has one column and shares storage with @p src.
 *
 * @param src Source matrix.
 * @param c Column index to reference.
 * @return A column view into @p src.
 *
 * Complexity
 * `O(1)`.
 */
AML_DEF struct Aml_Mat2d aml_create_col_ref(struct Aml_Mat2d src, size_t c)
{
    AML_ASSERT(c < src.cols);

    struct Aml_Mat2d col = {.cols = 1,
                 .rows = src.rows,
                 .stride_r = src.stride_r,
                 .elements = &(AML_MAT2D_AT(src, 0, c))};

    return col;
}

/**
 * @brief Compute the 3D cross product of two column vectors.
 *
 * Computes `dst = v1 x v2`.
 *
 * @param dst Output 3x1 vector.
 * @param v1 First 3x1 input vector.
 * @param v2 Second 3x1 input vector.
 *
 * Complexity
 * `O(1)`.
 */
void aml_cross(struct Aml_Mat2d dst, struct Aml_Mat2d v1, struct Aml_Mat2d v2)
{
    AML_ASSERT(3 == dst.rows && 1 == dst.cols);
    AML_ASSERT(3 == v1.rows && 1 == v1.cols);
    AML_ASSERT(3 == v2.rows && 1 == v2.cols);

    AML_MAT2D_AT(dst, 0, 0) = AML_MAT2D_AT(v1, 1, 0) * AML_MAT2D_AT(v2, 2, 0) - AML_MAT2D_AT(v1, 2, 0) * AML_MAT2D_AT(v2, 1, 0);
    AML_MAT2D_AT(dst, 1, 0) = AML_MAT2D_AT(v1, 2, 0) * AML_MAT2D_AT(v2, 0, 0) - AML_MAT2D_AT(v1, 0, 0) * AML_MAT2D_AT(v2, 2, 0);
    AML_MAT2D_AT(dst, 2, 0) = AML_MAT2D_AT(v1, 0, 0) * AML_MAT2D_AT(v2, 1, 0) - AML_MAT2D_AT(v1, 1, 0) * AML_MAT2D_AT(v2, 0, 0);
}
/**
 * @brief Matrix multiplication.
 *
 * Computes `dst = a * b`.
 *
 * @param dst Output matrix.
 * @param a Left factor.
 * @param b Right factor.
 * @pre `a.cols == b.rows`, `dst.rows == a.rows`, `dst.cols == b.cols`.
 * @warning The implementation writes directly into @p dst, so aliasing @p dst
 * with @p a or @p b is unsafe.
 *
 * Complexity
 * `O(a.rows * a.cols * b.cols)`.
 */

AML_DEF void aml_dot(struct Aml_Mat2d dst, struct Aml_Mat2d a, struct Aml_Mat2d b)
{
    AML_ASSERT(a.cols == b.rows);
    AML_ASSERT(a.rows == dst.rows);
    AML_ASSERT(b.cols == dst.cols);

    size_t i, j, k;

    for (i = 0; i < dst.rows; i++) {
        for (j = 0; j < dst.cols; j++) {
            AML_MAT2D_AT(dst, i, j) = 0;
            for (k = 0; k < a.cols; k++) {
                AML_MAT2D_AT(dst, i, j) += AML_MAT2D_AT(a, i, k)*AML_MAT2D_AT(b, k, j);
            }
        }
    }

}

/**
 * @brief Compute the element-wise inner product of two equal-shaped matrices.
 *
 * For vectors this is the usual dot product. For general matrices this is the
 * Frobenius inner product.
 *
 * @param v1 First input matrix.
 * @param v2 Second input matrix.
 * @return `sum_{i,j} v1(i,j) * v2(i,j)`.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF aml_real aml_dot_product(struct Aml_Mat2d m1, struct Aml_Mat2d m2)
{
    AML_ASSERT(m1.rows == m2.rows);
    AML_ASSERT(m1.cols == m2.cols);

    aml_real dot_product = 0;

    for (size_t i = 0; i < m1.rows; i++) {
        for (size_t j = 0; j < m1.cols; j++) {
            dot_product += AML_MAT2D_AT(m1, i, j) * AML_MAT2D_AT(m2, i, j);
        }
    }
    
    return dot_product;
}

/**
 * @brief Sum all matrix elements.
 *
 * @param m Input matrix.
 * @return Sum of all entries.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF aml_real aml_elements_sum(struct Aml_Mat2d m)
{
    aml_real sum = 0;

    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            sum += AML_MAT2D_AT(m, i, j);
        }
    }
    return sum;
}

AML_DEF aml_real aml_elements_col_sum(struct Aml_Mat2d m, size_t c)
{
    aml_real sum = 0;

    for (size_t i = 0; i < m.rows; ++i) {
        sum += AML_MAT2D_AT(m, i, c);
    }
    return sum;
}

AML_DEF aml_real aml_elements_row_sum(struct Aml_Mat2d m, size_t r)
{
    aml_real sum = 0;

    for (size_t j = 0; j < m.cols; ++j) {
        sum += AML_MAT2D_AT(m, r, j);
    }
    return sum;
}

/**
 * @brief Fill a matrix with a constant value.
 *
 * @param m Matrix modified in place.
 * @param x Fill value.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_fill(struct Aml_Mat2d m, aml_real x)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_MAT2D_AT(m, i, j) = x;
        }
    }
}

/**
 * @brief Fill a matrix with an arithmetic progression in storage order.
 *
 * Entry `(i,j)` receives `start + step * aml_offset2d(m, i, j)`.
 *
 * @param m Matrix modified in place.
 * @param start First value.
 * @param step Increment between consecutive stored elements.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_fill_sequence(struct Aml_Mat2d m, aml_real start, aml_real step) {
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            AML_MAT2D_AT(m, i, j) = start + step * aml_offset2d(m, i, j);
        }
    }
}

/**
 * @brief Fill a uint32 matrix with a constant value.
 *
 * @param m Matrix modified in place.
 * @param x Fill value.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_fill_uint32(struct Aml_Mat2d_uint32 m, uint32_t x)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_MAT2D_AT(m, i, j) = x;
        }
    }
}

/**
 * @brief Compute the squared Euclidean norm of a vector-shaped matrix.
 *
 * This function accepts either a column vector or a row vector and returns the
 * sum of squares, without a square root.
 *
 * @param v Input row or column vector.
 * @return `sum v_i^2`.
 *
 * Complexity
 * `O(length)`.
 */
AML_DEF aml_real aml_inner_product(struct Aml_Mat2d v)
{
    AML_ASSERT((1 == v.cols) || (1 == v.rows));

    aml_real dot_product = 0;

    if (1 == v.cols) {
        for (size_t i = 0; i < v.rows; i++) {
            dot_product += AML_MAT2D_AT(v, i, 0) * AML_MAT2D_AT(v, i, 0);
        }
    } else {
        for (size_t j = 0; j < v.cols; j++) {
            dot_product += AML_MAT2D_AT(v, 0, j) * AML_MAT2D_AT(v, 0, j);
        }
    }
    
    return dot_product;
}

AML_DEF bool aml_is_close(aml_real a, aml_real b, aml_real eps)
{
    aml_real scale = aml_fmax((aml_real)1, aml_fmax(aml_fabs(a), aml_fabs(b)));
    return aml_fabs(a - b) <= eps * scale;
}

/**
 * @brief Test whether a square matrix is diagonal.
 *
 * Off-diagonal entries are checked using `AML_IS_ZERO`.
 *
 * @param m Input square matrix.
 * @return `true` if all off-diagonal entries are numerically zero.
 *
 * Complexity
 * `O(n^2)`.
 */
AML_DEF bool aml_is_diagonal(struct Aml_Mat2d m)
{
    AML_ASSERT(m.rows == m.cols);

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            if (i == j) continue;
            if (!AML_IS_ZERO(AML_MAT2D_AT(m, i, j))) {
                aml_dprintERROR("Not diagonal!. (%zu, %zu): %g != 0", i, j, AML_MAT2D_AT(m, i, j));
                return false;
            }
        }
    }

    return true;
}

AML_DEF bool aml_is_hessenberg(struct Aml_Mat2d m)
{
    AML_ASSERT(m.rows == m.cols);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            if (i > j + 1 && !AML_IS_ZERO(AML_MAT2D_AT(m, i, j))) {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief Test whether a square matrix is symmetric.
 *
 * Symmetry is checked with the library epsilon.
 *
 * @param m Input square matrix.
 * @return `true` if `m(i,j) == m(j,i)` up to `AML_EPS`.
 *
 * Complexity
 * `O(n^2)`.
 */
AML_DEF bool aml_is_symmetric(struct Aml_Mat2d m)
{
    AML_ASSERT(m.rows == m.cols);

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            if (!AML_IS_ZERO(AML_MAT2D_AT(m, i, j) - AML_MAT2D_AT(m, j , i))) {
                aml_dprintERROR("Not symmetric!. (%zu, %zu): %-.10g != (%zu, %zu): %-.10g", i, j, AML_MAT2D_AT(m, i, j), j, i, AML_MAT2D_AT(m, j, i));
                return false;
            }
        }
    }

    return true;
}

AML_DEF bool aml_is_symmetric_relative(struct Aml_Mat2d m)
{
    AML_ASSERT(m.rows == m.cols);

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            if (!aml_is_close(AML_MAT2D_AT(m, i, j), AML_MAT2D_AT(m, j , i), AML_EPS)) {
                aml_dprintERROR("Not symmetric!. (%zu, %zu): %-.10g != (%zu, %zu): %-.10g", i, j, AML_MAT2D_AT(m, i, j), j, i, AML_MAT2D_AT(m, j, i));
                return false;
            }
        }
    }

    return true;
}

/**
 * @brief Test whether a square matrix is tridiagonal.
 *
 * Entries outside the main diagonal and first off-diagonals are checked with
 * `AML_IS_ZERO`.
 *
 * @param m Input square matrix.
 * @return `true` if @p m is numerically tridiagonal.
 *
 * Complexity
 * `O(n^2)`.
 */
AML_DEF bool aml_is_tridiagonal(struct Aml_Mat2d m)
{
    AML_ASSERT(m.rows == m.cols);

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            if (i == j || i + 1 == j || j + 1 == i) continue;

            if (!AML_IS_ZERO(AML_MAT2D_AT(m, i, j))) return false;
        }
    }

    return true;
}

AML_DEF void aml_make_diagonal(struct Aml_Mat2d m)
{
    AML_ASSERT(m.rows == m.cols);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            if (i != j) {
                AML_MAT2D_AT(m, i, j) = 0;
            }
        }
    }
}

/**
 * @brief Symmetrize a square matrix by averaging mirrored entries.
 *
 * For each `i < j`, both `m(i,j)` and `m(j,i)` are replaced by their average.
 *
 * @param m Matrix modified in place.
 *
 * Complexity
 * `O(n^2)`.
 */
AML_DEF void aml_make_symmetric(struct Aml_Mat2d m)
{
    AML_ASSERT(m.rows == m.cols);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = i + 1; j < m.cols; ++j) {
            aml_real aij = AML_MAT2D_AT(m, i, j);
            aml_real aji = AML_MAT2D_AT(m, j, i);
            aml_real v = ((aml_real)0.5) * (aij + aji);
            AML_MAT2D_AT(m, i, j) = v;
            AML_MAT2D_AT(m, j, i) = v;
        }
    }
}

/**
 * @brief Zero all entries outside the tridiagonal band.
 *
 * The main diagonal and first upper/lower diagonals are preserved.
 *
 * @param m Square matrix modified in place.
 *
 * Complexity
 * `O(n^2)`.
 */
AML_DEF void aml_make_tridiagonal(struct Aml_Mat2d m) 
{
    AML_ASSERT(m.rows == m.cols);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            if (i + 1 < j || j + 1 < i) {
                AML_MAT2D_AT(m, i, j) = 0;
            }
        }
    }
}

AML_DEF void aml_make_upper_hessenberg_range(struct Aml_Mat2d m, size_t start, size_t end)
{
    AML_ASSERT(m.rows == m.cols);
    AML_ASSERT(start <= end);
    AML_ASSERT(start >= 0);
    AML_ASSERT(end < m.cols);

    for (size_t i = start; i <= end; ++i) {
        for (size_t j = start; j <= end; ++j) {
            if (i > j + 1) {
                AML_MAT2D_AT(m, i, j) = 0;
            }
        }
    }

}

/**
 * @brief Allocate a dense matrix with compact row-major storage.
 *
 * Memory is uninitialized.
 *
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @return Newly allocated matrix.
 *
 * Complexity
 * `O(1)` plus allocation cost.
 */
AML_DEF struct Aml_Mat2d aml_mat2d_alloc(size_t rows, size_t cols)
{
    struct Aml_Mat2d m;
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (aml_real*)AML_MALLOC(sizeof(aml_real)*rows*cols);
    AML_ASSERT(m.elements != NULL);
    
    return m;
}

/**
 * @brief Free a matrix allocated by aml_mat2d_alloc() or aml_realloc().
 *
 * @param m Matrix whose storage will be released.
 *
 * Complexity
 * `O(1)`.
 */
AML_DEF void aml_mat2d_free(struct Aml_Mat2d m)
{
    AML_FREE(m.elements);
}

/**
 * @brief Allocate a dense uint32 matrix with compact row-major storage.
 *
 * Memory is uninitialized.
 *
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @return Newly allocated matrix.
 *
 * Complexity
 * `O(1)` plus allocation cost.
 */
AML_DEF struct Aml_Mat2d_uint32 aml_mat2d_uint32_alloc(size_t rows, size_t cols)
{
    struct Aml_Mat2d_uint32 m;
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (uint32_t*)AML_MALLOC(sizeof(uint32_t)*rows*cols);
    AML_ASSERT(m.elements != NULL);
    
    return m;
}

/**
 * @brief Free a uint32 matrix allocated by aml_mat2d_uint32_alloc().
 *
 * @param m Matrix whose storage will be released.
 *
 * Complexity
 * `O(1)`.
 */
AML_DEF void aml_mat2d_uint32_free(struct Aml_Mat2d_uint32 m)
{
    AML_FREE(m.elements);
}

/**
 * @brief Test whether every element is exactly equal to one value.
 *
 * Comparison is exact, not epsilon-based.
 *
 * @param m Input matrix.
 * @param number Value to compare against.
 * @return `true` if every entry equals @p number.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF bool aml_mat2d_is_all_the_same(struct Aml_Mat2d m, aml_real number)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            if (AML_MAT2D_AT(m, i, j) != number) {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief Scale every element of a matrix by a scalar.
 *
 * @param m Matrix modified in place.
 * @param factor Scalar multiplier.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_mult(struct Aml_Mat2d m, aml_real factor)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_MAT2D_AT(m, i, j) *= factor;
        }
    }
}

/**
 * @brief Scale one row by a scalar.
 *
 * @param m Matrix modified in place.
 * @param r Row index.
 * @param factor Scalar multiplier.
 *
 * Complexity
 * `O(cols)`.
 */
AML_DEF void aml_mult_row(struct Aml_Mat2d m, size_t r, aml_real factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        AML_MAT2D_AT(m, r, j) *= factor;
    }
}

/**
 * @brief Normalize a matrix by its Frobenius norm.
 *
 * If the norm is numerically zero, the matrix is left unchanged.
 *
 * @param m Matrix modified in place.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_normalize(struct Aml_Mat2d m)
{
    aml_real norma = aml_calc_norma(m);
    if (AML_IS_ZERO(norma)) {
        return;
    }

    aml_mult(m, (aml_real)1 / norma);
}

/**
 * @brief Normalize a matrix by its entrywise max absolute value.
 *
 * If the norm is numerically zero, the matrix is left unchanged.
 *
 * @param m Matrix modified in place.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_normalize_inf(struct Aml_Mat2d m)
{
    aml_real norma = aml_calc_norma_inf(m);
    if (AML_IS_ZERO(norma)) {
        return;
    }

    aml_mult(m, (aml_real)1 / norma);
}

/**
 * @brief Convert a 2D matrix index into a linear storage offset.
 *
 * @param m Matrix view.
 * @param i Row index.
 * @param j Column index.
 * @return Linear offset `i * stride_r + j`.
 *
 * Complexity
 * `O(1)`.
 */
AML_DEF size_t aml_offset2d(struct Aml_Mat2d m, size_t i, size_t j)
{
    AML_ASSERT(i < m.rows && j < m.cols);
    return i * m.stride_r + j;
}

/**
 * @brief Convert a 2D uint32 matrix index into a linear storage offset.
 *
 * @param m Matrix view.
 * @param i Row index.
 * @param j Column index.
 * @return Linear offset `i * stride_r + j`.
 *
 * Complexity
 * `O(1)`.
 */
AML_DEF size_t aml_offset2d_uint32(struct Aml_Mat2d_uint32 m, size_t i, size_t j)
{
    AML_ASSERT(i < m.rows && j < m.cols);
    return i * m.stride_r + j;
}

/**
 * @brief Compute the outer product of a vector with itself.
 *
 * If @p v is a column vector, computes `des(i,j) = v(i) * v(j)`.
 * If @p v is a row vector, computes `des(i,j) = v(i) * v(j)` using row access.
 *
 * @param des Output square matrix.
 * @param v Input row or column vector.
 *
 * Complexity
 * `O(n^2)`.
 */
AML_DEF void aml_outer_product(struct Aml_Mat2d des, struct Aml_Mat2d v)
{
    AML_ASSERT(des.cols == des.rows);
    AML_ASSERT((1 == v.cols && des.rows == v.rows) || (1 == v.rows && des.cols == v.cols));

    if (1 == v.cols) {
        for (size_t i = 0; i < des.rows; i++) {
            for (size_t j = 0; j < des.cols; j++) {
                AML_MAT2D_AT(des, i, j) = AML_MAT2D_AT(v, i, 0) * AML_MAT2D_AT(v, j, 0);
            }
        }
    } else {
        for (size_t i = 0; i < des.rows; i++) {
            for (size_t j = 0; j < des.cols; j++) {
                AML_MAT2D_AT(des, i, j) = AML_MAT2D_AT(v, 0, i) * AML_MAT2D_AT(v, 0, j);
            }
        }
    }
}

/**
 * @brief Print a real matrix in a compact human-readable format.
 *
 * @param m Matrix to print.
 * @param name Label shown before the matrix contents.
 * @param padding Left indentation width.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_print(struct Aml_Mat2d m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows; ++i) {
        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < m.cols; ++j) {
            printf("%9.3g ", AML_MAT2D_AT(m, i, j));
            // printf("%12.8f ", AML_MAT2D_AT(m, i, j));
        }
        printf("\n");
    }
    printf("%*s]\n", (int) padding, "");
}

/**
 * @brief Print a uint32 matrix in hexadecimal form.
 *
 * @param m Matrix to print.
 * @param name Label shown before the matrix contents.
 * @param padding Left indentation width.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_print_uint32(struct Aml_Mat2d_uint32 m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows; ++i) {
        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < m.cols; ++j) {
            printf("%#10X ", AML_MAT2D_AT(m, i, j));
        }
        printf("\n");
    }
    printf("%*s]\n", (int) padding, "");
}

/**
 * @brief Print raw stored elements as a single column.
 *
 * This prints `rows * cols` elements in linear storage order, not in 2D form.
 *
 * @param m Matrix to print.
 * @param name Label shown before the values.
 * @param padding Left indentation width.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_print_as_col(struct Aml_Mat2d m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows*m.cols; ++i) {
            printf("%*s    ", (int) padding, "");
            printf("%f\n", m.elements[i]);
    }
    printf("%*s]\n", (int) padding, "");
}

/**
 * @brief Generate a pseudo-random real number in `[0, 1]`.
 *
 * Uses the C standard library `rand()`.
 *
 * @return Pseudo-random value in `[0,1]`.
 *
 * Complexity
 * `O(1)`.
 */
AML_DEF aml_real aml_rand_aml_real(void)
{
    return (aml_real) rand() / (aml_real) RAND_MAX;
}

/**
 * @brief Reallocate a matrix to a new size.
 *
 * The matrix remains row-major and compact after reallocation. Existing data
 * preservation follows the semantics of `realloc()`.
 *
 * @param m Existing matrix.
 * @param rows New row count.
 * @param cols New column count.
 * @return Resized matrix view.
 *
 * Complexity
 * `O(1)` amortized plus reallocation cost.
 */
AML_DEF struct Aml_Mat2d aml_realloc(struct Aml_Mat2d m, size_t rows, size_t cols)
{
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (aml_real*)AML_REALLOC(m.elements, sizeof(aml_real)*rows*cols);
    AML_ASSERT(m.elements != NULL);
    
    return m;
}

/**
 * @brief Reallocate a uint32 matrix to a new size.
 *
 * @param m Existing matrix.
 * @param rows New row count.
 * @param cols New column count.
 * @return Resized matrix view.
 *
 * Complexity
 * `O(1)` amortized plus reallocation cost.
 */
AML_DEF struct Aml_Mat2d_uint32 aml_realloc_uint32(struct Aml_Mat2d_uint32 m, size_t rows, size_t cols)
{
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (uint32_t*)AML_REALLOC(m.elements, sizeof(uint32_t)*rows*cols);
    AML_ASSERT(m.elements != NULL);
    
    return m;
}

/**
 * @brief Rotate a square matrix by 180 degrees in place.
 *
 * Implemented as a transpose followed by an anti-diagonal transpose.
 *
 * @param m Square matrix modified in place.
 *
 * Complexity
 * `O(n^2)`.
 */
AML_DEF void aml_rotate_mat_180_deg_inplace(struct Aml_Mat2d m)
{
    aml_transpose_inplace(m);
    aml_anti_diag_transpose_inplace(m);
}

/**
 * @brief Find the first numerically non-zero entry in a row.
 *
 * Scans columns from left to right starting at column 0 and writes the first
 * matching column index into @p non_zero_col.
 *
 * @param m Input matrix.
 * @param r Row index.
 * @param non_zero_col Output column index.
 * @return `true` if a non-zero entry was found.
 *
 * Complexity
 * `O(cols)`.
 */
AML_DEF bool aml_row_find_first_non_zero_value(struct Aml_Mat2d m, size_t r, size_t *non_zero_col)
{
    for (size_t c = 0; c < m.cols; ++c) {
        if (!AML_IS_ZERO(AML_MAT2D_AT(m, r, c))) {
            *non_zero_col = c;
            return true;
        }
    }
    return false;
}

/**
 * @brief Test whether an entire row is exactly equal to one value.
 *
 * Comparison is exact, not epsilon-based.
 *
 * @param m Input matrix.
 * @param number Value to compare against.
 * @param r Row index.
 * @return `true` if every entry in the row equals @p number.
 *
 * Complexity
 * `O(cols)`.
 */
AML_DEF bool aml_row_is_all_the_same(struct Aml_Mat2d m, aml_real number, size_t r)
{
    for (size_t j = 0; j < m.cols; ++j) {
        if (AML_MAT2D_AT(m, r, j) != number) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Swap two matrix rows in place.
 *
 * @param m Matrix modified in place.
 * @param r1 First row index.
 * @param r2 Second row index.
 *
 * Complexity
 * `O(cols)`.
 */
AML_DEF void aml_rows_swap(struct Aml_Mat2d m, size_t r1, size_t r2)
{
    for (size_t j = 0; j < m.cols; j++) {
        aml_real temp = AML_MAT2D_AT(m, r1, j);
        AML_MAT2D_AT(m, r1, j) = AML_MAT2D_AT(m, r2, j);
        AML_MAT2D_AT(m, r2, j) = temp;
    }
}

/**
 * @brief Build a 3x3 direction-cosine matrix from yaw, pitch, and roll.
 *
 * The implementation computes:
 * `DCM = RotX(roll) * RotY(pitch) * RotZ(yaw)`.
 *
 * @param DCM Output 3x3 matrix.
 * @param yaw_deg Rotation about Z in degrees.
 * @param pitch_deg Rotation about Y in degrees.
 * @param roll_deg Rotation about X in degrees.
 *
 * Complexity
 * `O(1)`.
 */
AML_DEF void aml_set_DCM_xyz(struct Aml_Mat2d DCM, float roll_deg, float pitch_deg, float yaw_deg)
{
    struct Aml_Mat2d RotZ = aml_mat2d_alloc(3,3);
    aml_set_rot_mat_z(RotZ, yaw_deg);
    struct Aml_Mat2d RotY = aml_mat2d_alloc(3,3);
    aml_set_rot_mat_y(RotY, pitch_deg);
    struct Aml_Mat2d RotX = aml_mat2d_alloc(3,3);
    aml_set_rot_mat_x(RotX, roll_deg);
    struct Aml_Mat2d temp = aml_mat2d_alloc(3,3);

    aml_dot(temp, RotY, RotZ);
    aml_dot(DCM, RotX, temp); /* I have a DCM */

    aml_mat2d_free(RotZ);
    aml_mat2d_free(RotY);
    aml_mat2d_free(RotX);
    aml_mat2d_free(temp);
}

/**
 * @brief Set a square matrix to the identity matrix.
 *
 * @param m Square matrix modified in place.
 *
 * Complexity
 * `O(n^2)`.
 */
AML_DEF void aml_set_identity(struct Aml_Mat2d m)
{
    AML_ASSERT(m.cols == m.rows);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_MAT2D_AT(m, i, j) = i == j ? (aml_real)1 : (aml_real)0;
        }
    }
}

/**
 * @brief Fill a matrix with independent uniform random values.
 *
 * Each entry is drawn from `[low, high]`.
 *
 * @param m Matrix modified in place.
 * @param low Lower bound.
 * @param high Upper bound.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_set_rand(struct Aml_Mat2d m, aml_real low, aml_real high)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_MAT2D_AT(m, i, j) = aml_rand_aml_real()*(high - low) + low;
        }
    }
}

/**
 * @brief Fill a square matrix with symmetric random values.
 *
 * The implementation writes random values to `(i,j)` and mirrors them to
 * `(j,i)`, so the matrix is symmetric on return.
 *
 * @param m Square matrix modified in place.
 * @param low Lower bound.
 * @param high Upper bound.
 *
 * Complexity
 * `O(n^2)`.
 */
AML_DEF void aml_set_rand_symmetric(struct Aml_Mat2d m, aml_real low, aml_real high)
{
    AML_ASSERT(m.rows == m.cols);
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            aml_real temp = aml_rand_aml_real() * (high - low) + low;
            AML_MAT2D_AT(m, i, j) = temp;
            AML_MAT2D_AT(m, j, i) = temp;
        }
    }
}

/**
 * @brief Set a 3x3 rotation matrix about the X axis.
 *
 * Angle is specified in degrees.
 *
 * @param m Output 3x3 matrix.
 * @param angle_deg Rotation angle in degrees.
 *
 * Complexity
 * `O(1)`.
 */
AML_DEF void aml_set_rot_mat_x(struct Aml_Mat2d m, float angle_deg)
{
    AML_ASSERT(3 == m.cols && 3 == m.rows);

    float angle_rad = angle_deg * (float)AML_PI / 180.0f;
    aml_set_identity(m);
    AML_MAT2D_AT(m, 1, 1) =  aml_cos(angle_rad);
    AML_MAT2D_AT(m, 1, 2) =  aml_sin(angle_rad);
    AML_MAT2D_AT(m, 2, 1) = -aml_sin(angle_rad);
    AML_MAT2D_AT(m, 2, 2) =  aml_cos(angle_rad);
}

/**
 * @brief Set a 3x3 rotation matrix about the Y axis.
 *
 * Angle is specified in degrees.
 *
 * @param m Output 3x3 matrix.
 * @param angle_deg Rotation angle in degrees.
 *
 * Complexity
 * `O(1)`.
 */
AML_DEF void aml_set_rot_mat_y(struct Aml_Mat2d m, float angle_deg)
{
    AML_ASSERT(3 == m.cols && 3 == m.rows);

    float angle_rad = angle_deg * (float)AML_PI / 180.0f;
    aml_set_identity(m);
    AML_MAT2D_AT(m, 0, 0) =  aml_cos(angle_rad);
    AML_MAT2D_AT(m, 0, 2) = -aml_sin(angle_rad);
    AML_MAT2D_AT(m, 2, 0) =  aml_sin(angle_rad);
    AML_MAT2D_AT(m, 2, 2) =  aml_cos(angle_rad);
}

/**
 * @brief Set a 3x3 rotation matrix about the Z axis.
 *
 * Angle is specified in degrees.
 *
 * @param m Output 3x3 matrix.
 * @param angle_deg Rotation angle in degrees.
 *
 * Complexity
 * `O(1)`.
 */
AML_DEF void aml_set_rot_mat_z(struct Aml_Mat2d m, float angle_deg)
{
    AML_ASSERT(3 == m.cols && 3 == m.rows);

    float angle_rad = angle_deg * (float)AML_PI / 180.0f;
    aml_set_identity(m);
    AML_MAT2D_AT(m, 0, 0) =  aml_cos(angle_rad);
    AML_MAT2D_AT(m, 0, 1) =  aml_sin(angle_rad);
    AML_MAT2D_AT(m, 1, 0) = -aml_sin(angle_rad);
    AML_MAT2D_AT(m, 1, 1) =  aml_cos(angle_rad);
}

/**
 * @brief Add a scalar shift to the diagonal of a square matrix.
 *
 * Computes `m <- m + shift * I`.
 *
 * @param m Matrix modified in place.
 * @param shift Diagonal increment.
 *
 * Complexity
 * `O(n)`.
 */
AML_DEF void aml_shift(struct Aml_Mat2d m, aml_real shift)
{
    AML_ASSERT(m.cols == m.rows); 
    for (size_t i = 0; i < m.rows; i++) {
        AML_MAT2D_AT(m, i, i) += shift;
    }
}

/**
 * @brief Add a scalar shift to part of the diagonal of a square matrix.
 *
 * Only diagonal indices `is..ie` are modified.
 *
 * @param m Matrix modified in place.
 * @param shift Diagonal increment.
 * @param is First diagonal index.
 * @param ie Last diagonal index.
 *
 * Complexity
 * `O(ie - is + 1)`.
 */
AML_DEF void aml_shift_specific(struct Aml_Mat2d m, aml_real shift, size_t is, size_t ie)
{
    AML_ASSERT(m.cols == m.rows); 
    for (size_t i = is; i <= ie; i++) {
        AML_MAT2D_AT(m, i, i) += shift;
    }
}

/**
 * @brief Subtract matrix @p a from @p dst element-wise.
 *
 * Computes `dst(i,j) -= a(i,j)` for all elements.
 *
 * @param dst Destination matrix, modified in place.
 * @param a Matrix subtracted from @p dst.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_sub(struct Aml_Mat2d dst, struct Aml_Mat2d a)
{
    AML_ASSERT(dst.rows == a.rows);
    AML_ASSERT(dst.cols == a.cols);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            AML_MAT2D_AT(dst, i, j) -= AML_MAT2D_AT(a, i, j);
        }
    }
}

/**
 * @brief Subtract one source column from one destination column.
 *
 * Computes `des(:,des_col) -= src(:,src_col)`.
 *
 * @param des Destination matrix.
 * @param des_col Destination column index.
 * @param src Source matrix.
 * @param src_col Source column index.
 *
 * Complexity
 * `O(rows)`.
 */
AML_DEF void aml_sub_col_to_col(struct Aml_Mat2d des, size_t des_col, struct Aml_Mat2d src, size_t src_col)
{
    AML_ASSERT(src_col < src.cols);
    AML_ASSERT(des.rows == src.rows);
    AML_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        AML_MAT2D_AT(des, i, des_col) -= AML_MAT2D_AT(src, i, src_col);
    }
}

/**
 * @brief Subtract one source row from one destination row.
 *
 * Computes `des(des_row,:) -= src(src_row,:)`.
 *
 * @param des Destination matrix.
 * @param des_row Destination row index.
 * @param src Source matrix.
 * @param src_row Source row index.
 *
 * Complexity
 * `O(cols)`.
 */
AML_DEF void aml_sub_row_to_row(struct Aml_Mat2d des, size_t des_row, struct Aml_Mat2d src, size_t src_row)
{
    AML_ASSERT(src_row < src.rows);
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        AML_MAT2D_AT(des, des_row, j) -= AML_MAT2D_AT(src, src_row, j);
    }
}

/**
 * @brief Subtract a scaled source row from a destination row.
 *
 * Computes `row(des_r) -= factor * row(src_r)`.
 *
 * @param m Matrix modified in place.
 * @param des_r Destination row index.
 * @param src_r Source row index.
 * @param factor Row scale factor.
 *
 * Complexity
 * `O(cols)`.
 */
AML_DEF void aml_sub_src_row_time_factor_from_des_row(struct Aml_Mat2d m, size_t des_r, size_t src_r, aml_real factor)
{
    AML_ASSERT(des_r < m.rows);
    AML_ASSERT(src_r < m.rows);
    for (size_t j = 0; j < m.cols; ++j) {
        AML_MAT2D_AT(m, des_r, j) -= factor * AML_MAT2D_AT(m, src_r, j);
    }
}

AML_DEF void aml_sub_src_row_time_factor_from_des_row_range(struct Aml_Mat2d m, size_t des_r, size_t src_r, aml_real factor, size_t js, size_t je)
{
    AML_ASSERT(des_r < m.rows);
    AML_ASSERT(src_r < m.rows);
    AML_ASSERT(je >= js);
    for (size_t j = js; j <= je; ++j) {
        AML_MAT2D_AT(m, des_r, j) -= factor * AML_MAT2D_AT(m, src_r, j);
    }
}

/**
 * @brief Compute the transpose of a matrix.
 *
 * Computes `des = src^T`.
 *
 * @param des Output matrix.
 * @param src Input matrix.
 * @warning This is an out-of-place transpose; aliasing @p des and @p src is
 * not safe.
 *
 * Complexity
 * `O(rows * cols)`.
 */
AML_DEF void aml_transpose(struct Aml_Mat2d des, struct Aml_Mat2d src)
{
    AML_ASSERT(des.cols == src.rows);
    AML_ASSERT(des.rows == src.cols);

    for (size_t index = 0; index < des.rows; ++index) {
        for (size_t jndex = 0; jndex < des.cols; ++jndex) {
            AML_MAT2D_AT(des, index, jndex) = AML_MAT2D_AT(src, jndex, index);
        }
    }
}

/**
 * @brief Transpose a square matrix in place.
 *
 * @param m Square matrix modified in place.
 *
 * par Complexity
 * `O(n^2)`.
 */
AML_DEF void aml_transpose_inplace(struct Aml_Mat2d m)
{
    AML_ASSERT(m.cols == m.rows);

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j <= i; j++) {
            aml_real temp = AML_MAT2D_AT(m, i, j);
            AML_MAT2D_AT(m, i, j) = AML_MAT2D_AT(m, j, i);
            AML_MAT2D_AT(m, j, i) = temp;
        }
    }
}


#endif // ALMOG_MATRIX_LIBRARY_IMPLEMENTATION

