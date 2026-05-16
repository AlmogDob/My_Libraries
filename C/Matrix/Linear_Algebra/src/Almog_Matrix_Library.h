#ifndef ALMOG_MATRIX_LIBRARY_H_
#define ALMOG_MATRIX_LIBRARY_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
    #define AML_EPS 1e-5
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

struct Aml_Mat2d {
    size_t rows;
    size_t cols;
    size_t stride_r; /* elements to traverse to reach the next row */
    aml_real *elements;
};

struct Aml_Mat2d_uint32 {
    size_t rows;
    size_t cols;
    size_t stride_r; /* elements to traverse to reach the next row */
    uint32_t *elements;
};

#define AML_MAT2D_AT(m, i, j) (m).elements[(AML_ASSERT((i) < (m).rows && (j) < (m).cols), (i) * (m).stride_r + (j))]
#define AML_PI 3.14159265358979323846
#define AML_MAX_POWER_ITERATION 100
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
#define aml_dprintSTRING(expr) printf("[Info] %s:%d:\n" #expr " = %s\n", __FILE__, __LINE__, expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define aml_dprintCHAR(expr) printf("[Info] %s:%d:\n" #expr " = %c\n", __FILE__, __LINE__, expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define aml_dprintINT(expr) printf("[Info] %s:%d:\n" #expr " = %d\n", __FILE__, __LINE__, expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define aml_dprintFLOAT(expr) printf("[Info] %s:%d:\n" #expr " = %#g\n", __FILE__, __LINE__, expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define aml_dprintDOUBLE(expr) printf("[Info] %s:%d:\n" #expr " = %#g\n", __FILE__, __LINE__, expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define aml_dprintSIZE_T(expr) printf("[Info] %s:%d:\n" #expr " = %zu\n", __FILE__, __LINE__, expr)
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
#define AML_PRINT(m) aml_print(m, #m, 0)
#define AML_PRINT_UINT32(m) aml_print_uint32(m, #m, 0)
#define AML_PRINT_AS_COL(m) aml_print_as_col(m, #m, 0)
#define AML_MINOR_PRINT(mm) aml_minor_print(mm, #mm, 0)

enum aml_upper_triangulate_flag{
    AML_ONES_ON_DIAG = 1 << 0,
    AML_ROW_SWAPPING = 1 << 1,
};

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

AML_DEF void                    aml_fill(struct Aml_Mat2d m, aml_real x);
AML_DEF void                    aml_fill_sequence(struct Aml_Mat2d m, aml_real start, aml_real step);
AML_DEF void                    aml_fill_uint32(struct Aml_Mat2d_uint32 m, uint32_t x);

AML_DEF aml_real                aml_inner_product(struct Aml_Mat2d v);
AML_DEF bool                    aml_is_diagonal(struct Aml_Mat2d m);
AML_DEF bool                    aml_is_symmetric(struct Aml_Mat2d m);
AML_DEF bool                    aml_is_tridiagonal(struct Aml_Mat2d m);

AML_DEF void                    aml_make_symmetric(struct Aml_Mat2d m);
AML_DEF void                    aml_make_tridiagonal(struct Aml_Mat2d m);
AML_DEF struct Aml_Mat2d        aml_mat2d_alloc(size_t rows, size_t cols);
AML_DEF void                    aml_mat2d_free(struct Aml_Mat2d m);
AML_DEF void                    aml_mat2d_uint32_free(struct Aml_Mat2d_uint32 m);
AML_DEF struct Aml_Mat2d_uint32 aml_mat2d_uint32_alloc(size_t rows, size_t cols);
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
AML_DEF void                    aml_sub_row_time_factor_to_row(struct Aml_Mat2d m, size_t des_r, size_t src_r, aml_real factor);
AML_DEF void                    aml_swap_rows(struct Aml_Mat2d m, size_t r1, size_t r2);

AML_DEF void                    aml_transpose(struct Aml_Mat2d des, struct Aml_Mat2d src);
AML_DEF void                    aml_transpose_inplace(struct Aml_Mat2d m);

#endif // ALMOG_MATRIX_LIBRARY_H_

#ifdef ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#undef ALMOG_MATRIX_LIBRARY_IMPLEMENTATION


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

AML_DEF void aml_add_col_to_col(struct Aml_Mat2d des, size_t des_col, struct Aml_Mat2d src, size_t src_col)
{
    AML_ASSERT(src_col < src.cols);
    AML_ASSERT(des.rows == src.rows);
    AML_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        AML_MAT2D_AT(des, i, des_col) += AML_MAT2D_AT(src, i, src_col);
    }
}

AML_DEF void aml_add_row_to_row(struct Aml_Mat2d des, size_t des_row, struct Aml_Mat2d src, size_t src_row)
{
    AML_ASSERT(src_row < src.rows);
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        AML_MAT2D_AT(des, des_row, j) += AML_MAT2D_AT(src, src_row, j);
    }
}

AML_DEF void aml_add_row_time_factor_to_row(struct Aml_Mat2d m, size_t des_r, size_t src_r, aml_real factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        AML_MAT2D_AT(m, des_r, j) += factor * AML_MAT2D_AT(m, src_r, j);
    }
}

AML_DEF void aml_add_scalar(struct Aml_Mat2d m, aml_real x)
{
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            AML_MAT2D_AT(m, i, j) += x;
        }
    }
}

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

AML_DEF aml_real aml_calc_col_norma(struct Aml_Mat2d m, size_t c)
{
    AML_ASSERT(c < m.cols);

    aml_real sum = 0;
    for (size_t i = 0; i < m.rows; ++i) {
        sum += AML_MAT2D_AT(m, i, c) * AML_MAT2D_AT(m, i, c);
    }
    return aml_sqrt(sum);
}

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

AML_DEF bool aml_col_is_all_the_same(struct Aml_Mat2d m, aml_real number, size_t c)
{
    for (size_t i = 0; i < m.rows; ++i) {
        if (AML_MAT2D_AT(m, i, c) != number) {
            return false;
        }
    }
    return true;
}

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

AML_DEF void aml_copy_col_from_src_to_des(struct Aml_Mat2d des, size_t des_col, struct Aml_Mat2d src, size_t src_col)
{
    AML_ASSERT(src_col < src.cols);
    AML_ASSERT(des.rows == src.rows);
    AML_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        AML_MAT2D_AT(des, i, des_col) = AML_MAT2D_AT(src, i, src_col);
    }
}

AML_DEF void aml_copy_row_from_src_to_des(struct Aml_Mat2d des, size_t des_row, struct Aml_Mat2d src, size_t src_row)
{
    AML_ASSERT(src_row < src.rows);
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        AML_MAT2D_AT(des, des_row, j) = AML_MAT2D_AT(src, src_row, j);
    }
}

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

AML_DEF struct Aml_Mat2d aml_create_col_ref(struct Aml_Mat2d src, size_t c)
{
    AML_ASSERT(c < src.cols);

    struct Aml_Mat2d col = {.cols = 1,
                 .rows = src.rows,
                 .stride_r = src.stride_r,
                 .elements = &(AML_MAT2D_AT(src, 0, c))};

    return col;
}

void aml_cross(struct Aml_Mat2d dst, struct Aml_Mat2d v1, struct Aml_Mat2d v2)
{
    AML_ASSERT(3 == dst.rows && 1 == dst.cols);
    AML_ASSERT(3 == v1.rows && 1 == v1.cols);
    AML_ASSERT(3 == v2.rows && 1 == v2.cols);

    AML_MAT2D_AT(dst, 0, 0) = AML_MAT2D_AT(v1, 1, 0) * AML_MAT2D_AT(v2, 2, 0) - AML_MAT2D_AT(v1, 2, 0) * AML_MAT2D_AT(v2, 1, 0);
    AML_MAT2D_AT(dst, 1, 0) = AML_MAT2D_AT(v1, 2, 0) * AML_MAT2D_AT(v2, 0, 0) - AML_MAT2D_AT(v1, 0, 0) * AML_MAT2D_AT(v2, 2, 0);
    AML_MAT2D_AT(dst, 2, 0) = AML_MAT2D_AT(v1, 0, 0) * AML_MAT2D_AT(v2, 1, 0) - AML_MAT2D_AT(v1, 1, 0) * AML_MAT2D_AT(v2, 0, 0);
}

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

AML_DEF void aml_fill(struct Aml_Mat2d m, aml_real x)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_MAT2D_AT(m, i, j) = x;
        }
    }
}

AML_DEF void aml_fill_sequence(struct Aml_Mat2d m, aml_real start, aml_real step) {
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            AML_MAT2D_AT(m, i, j) = start + step * aml_offset2d(m, i, j);
        }
    }
}

AML_DEF void aml_fill_uint32(struct Aml_Mat2d_uint32 m, uint32_t x)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_MAT2D_AT(m, i, j) = x;
        }
    }
}

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

AML_DEF bool aml_is_symmetric(struct Aml_Mat2d m)
{
    AML_ASSERT(m.rows == m.cols);

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            if (!AML_IS_ZERO(AML_MAT2D_AT(m, i, j) - AML_MAT2D_AT(m, j , i))) {
                aml_dprintERROR("Not symmetric!. (%zu, %zu): %g != (%zu, %zu): %g", i, j, AML_MAT2D_AT(m, i, j), j, i, AML_MAT2D_AT(m, j, i));
                return false;
            }
        }
    }

    return true;
}

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

AML_DEF void aml_mat2d_free(struct Aml_Mat2d m)
{
    AML_FREE(m.elements);
}

AML_DEF void aml_mat2d_uint32_free(struct Aml_Mat2d_uint32 m)
{
    AML_FREE(m.elements);
}

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

AML_DEF void aml_mult(struct Aml_Mat2d m, aml_real factor)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_MAT2D_AT(m, i, j) *= factor;
        }
    }
}

AML_DEF void aml_mult_row(struct Aml_Mat2d m, size_t r, aml_real factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        AML_MAT2D_AT(m, r, j) *= factor;
    }
}

AML_DEF void aml_normalize(struct Aml_Mat2d m)
{
    aml_real norma = aml_calc_norma(m);
    if (AML_IS_ZERO(norma)) {
        return;
    }

    aml_mult(m, (aml_real)1 / norma);
}

AML_DEF void aml_normalize_inf(struct Aml_Mat2d m)
{
    aml_real norma = aml_calc_norma_inf(m);
    if (AML_IS_ZERO(norma)) {
        return;
    }

    aml_mult(m, (aml_real)1 / norma);
}

AML_DEF size_t aml_offset2d(struct Aml_Mat2d m, size_t i, size_t j)
{
    AML_ASSERT(i < m.rows && j < m.cols);
    return i * m.stride_r + j;
}

AML_DEF size_t aml_offset2d_uint32(struct Aml_Mat2d_uint32 m, size_t i, size_t j)
{
    AML_ASSERT(i < m.rows && j < m.cols);
    return i * m.stride_r + j;
}

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

AML_DEF void aml_print(struct Aml_Mat2d m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows; ++i) {
        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < m.cols; ++j) {
            printf("%9.3g ", AML_MAT2D_AT(m, i, j));
            // printf("%9.6f ", AML_MAT2D_AT(m, i, j));
        }
        printf("\n");
    }
    printf("%*s]\n", (int) padding, "");
}

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

AML_DEF void aml_print_as_col(struct Aml_Mat2d m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows*m.cols; ++i) {
            printf("%*s    ", (int) padding, "");
            printf("%f\n", m.elements[i]);
    }
    printf("%*s]\n", (int) padding, "");
}

AML_DEF aml_real aml_rand_aml_real(void)
{
    return (aml_real) rand() / (aml_real) RAND_MAX;
}

AML_DEF struct Aml_Mat2d aml_realloc(struct Aml_Mat2d m, size_t rows, size_t cols)
{
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (aml_real*)AML_REALLOC(m.elements, sizeof(aml_real)*rows*cols);
    AML_ASSERT(m.elements != NULL);
    
    return m;
}

AML_DEF struct Aml_Mat2d_uint32 aml_realloc_uint32(struct Aml_Mat2d_uint32 m, size_t rows, size_t cols)
{
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (uint32_t*)AML_REALLOC(m.elements, sizeof(uint32_t)*rows*cols);
    AML_ASSERT(m.elements != NULL);
    
    return m;
}

AML_DEF void aml_rotate_mat_180_deg_inplace(struct Aml_Mat2d m)
{
    aml_transpose_inplace(m);
    aml_anti_diag_transpose_inplace(m);
}

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

AML_DEF bool aml_row_is_all_the_same(struct Aml_Mat2d m, aml_real number, size_t r)
{
    for (size_t j = 0; j < m.cols; ++j) {
        if (AML_MAT2D_AT(m, r, j) != number) {
            return false;
        }
    }
    return true;
}

AML_DEF void aml_set_DCM_zyx(struct Aml_Mat2d DCM, float yaw_deg, float pitch_deg, float roll_deg)
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

AML_DEF void aml_set_identity(struct Aml_Mat2d m)
{
    AML_ASSERT(m.cols == m.rows);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_MAT2D_AT(m, i, j) = i == j ? (aml_real)1 : (aml_real)0;
        }
    }
}

AML_DEF void aml_set_rand(struct Aml_Mat2d m, aml_real low, aml_real high)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_MAT2D_AT(m, i, j) = aml_rand_aml_real()*(high - low) + low;
        }
    }
}

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

AML_DEF void aml_shift(struct Aml_Mat2d m, aml_real shift)
{
    AML_ASSERT(m.cols == m.rows); 
    for (size_t i = 0; i < m.rows; i++) {
        AML_MAT2D_AT(m, i, i) += shift;
    }
}

AML_DEF void aml_shift_specific(struct Aml_Mat2d m, aml_real shift, size_t is, size_t ie)
{
    AML_ASSERT(m.cols == m.rows); 
    for (size_t i = is; i <= ie; i++) {
        AML_MAT2D_AT(m, i, i) += shift;
    }
}

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

AML_DEF void aml_sub_col_to_col(struct Aml_Mat2d des, size_t des_col, struct Aml_Mat2d src, size_t src_col)
{
    AML_ASSERT(src_col < src.cols);
    AML_ASSERT(des.rows == src.rows);
    AML_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        AML_MAT2D_AT(des, i, des_col) -= AML_MAT2D_AT(src, i, src_col);
    }
}

AML_DEF void aml_sub_row_to_row(struct Aml_Mat2d des, size_t des_row, struct Aml_Mat2d src, size_t src_row)
{
    AML_ASSERT(src_row < src.rows);
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        AML_MAT2D_AT(des, des_row, j) -= AML_MAT2D_AT(src, src_row, j);
    }
}

AML_DEF void aml_sub_row_time_factor_to_row(struct Aml_Mat2d m, size_t des_r, size_t src_r, aml_real factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        AML_MAT2D_AT(m, des_r, j) -= factor * AML_MAT2D_AT(m, src_r, j);
    }
}

AML_DEF void aml_swap_rows(struct Aml_Mat2d m, size_t r1, size_t r2)
{
    for (size_t j = 0; j < m.cols; j++) {
        aml_real temp = AML_MAT2D_AT(m, r1, j);
        AML_MAT2D_AT(m, r1, j) = AML_MAT2D_AT(m, r2, j);
        AML_MAT2D_AT(m, r2, j) = temp;
    }
}

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

