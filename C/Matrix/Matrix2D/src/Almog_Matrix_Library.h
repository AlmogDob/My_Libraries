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
#else 
    typedef double aml_real;
    #define AML_EPS 1e-15
    #define aml_fmin  fmin
    #define aml_fmax  fmax
    #define aml_fabs  fabs
    #define aml_sqrt  sqrt
    #define aml_sin   sin
    #define aml_cos   cos
    #define aml_exp   exp
    #define aml_ceil  ceil
#endif

typedef struct {
    size_t rows;
    size_t cols;
    size_t stride_r; /* elements to traverse to reach the next row */
    aml_real *elements;
} Aml_Mat2d;

typedef struct {
    size_t rows;
    size_t cols;
    size_t stride_r; /* elements to traverse to reach the next row */
    uint32_t *elements;
} Aml_Mat2d_uint32;

typedef struct {
    size_t rows;
    size_t cols;
    size_t stride_r; /* logical stride for the minor shape (not used for access) */
    size_t *rows_list;
    size_t *cols_list;
    Aml_Mat2d ref_mat;
} Aml_Mat2d_Minor;

#define AML_AT(m, i, j) (m).elements[(AML_ASSERT((i) < (m).rows && (j) < (m).cols), (i) * (m).stride_r + (j))]

#define AML_PI 3.14159265358979323846
#define AML_MAX_POWER_ITERATION 100


#define AML_IS_ZERO(x) (aml_fabs(x) < AML_EPS)

#define AML_MINOR_AT(mm, i, j) AML_AT((mm).ref_mat, (mm).rows_list[i], (mm).cols_list[j])

#define AML_PRINT(m) aml_print(m, #m, 0)

#define AML_PRINT_UINT32(m) aml_print_uint32(m, #m, 0)

#define AML_PRINT_AS_COL(m) aml_print_as_col(m, #m, 0)

#define AML_MINOR_PRINT(mm) aml_minor_print(mm, #mm, 0)

#define aml_min(a, b) ((a) < (b) ? (a) : (b))
#define aml_max(a, b) ((a) > (b) ? (a) : (b))

#define aml_dprintDOUBLE(expr) printf(#expr " = %#g\n", expr)

#define aml_dprintSIZE_T(expr) printf(#expr " = %zu\n", expr)

#define aml_dprintINT(expr) printf(#expr " = %d\n", expr)

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

AML_DEF void          aml_add(Aml_Mat2d dst, Aml_Mat2d a);
AML_DEF void          aml_add_col_to_col(Aml_Mat2d des, size_t des_col, Aml_Mat2d src, size_t src_col);
AML_DEF void          aml_add_row_to_row(Aml_Mat2d des, size_t des_row, Aml_Mat2d src, size_t src_row);
AML_DEF void          aml_add_row_time_factor_to_row(Aml_Mat2d m, size_t des_r, size_t src_r, aml_real factor);
AML_DEF void          aml_add_scalar(Aml_Mat2d m, aml_real x);
AML_DEF Aml_Mat2d         aml_alloc(size_t rows, size_t cols);
AML_DEF Aml_Mat2d_uint32  aml_alloc_uint32(size_t rows, size_t cols);
AML_DEF void          aml_anti_diag_transpose_inplace(Aml_Mat2d m);
AML_DEF void          aml_apply_householder_left(Aml_Mat2d A, size_t row0, size_t col0, Aml_Mat2d v);
AML_DEF void          aml_apply_householder_right(Aml_Mat2d A, size_t col0, Aml_Mat2d v);
AML_DEF aml_real    aml_calc_col_norma(Aml_Mat2d m, size_t c);
AML_DEF aml_real    aml_calc_norma(Aml_Mat2d m);
AML_DEF aml_real    aml_calc_norma_inf(Aml_Mat2d m);
AML_DEF bool          aml_col_is_all_digit(Aml_Mat2d m, aml_real digit, size_t c);
AML_DEF void          aml_convolve(Aml_Mat2d m, Aml_Mat2d a, Aml_Mat2d b);
AML_DEF void          aml_copy(Aml_Mat2d des, Aml_Mat2d src);
AML_DEF void          aml_copy_uint32(Aml_Mat2d_uint32 des, Aml_Mat2d_uint32 src);
AML_DEF void          aml_copy_col_from_src_to_des(Aml_Mat2d des, size_t des_col, Aml_Mat2d src, size_t src_col);
AML_DEF void          aml_copy_row_from_src_to_des(Aml_Mat2d des, size_t des_row, Aml_Mat2d src, size_t src_row);
AML_DEF void          aml_copy_src_to_des_window(Aml_Mat2d des, Aml_Mat2d src, size_t is, size_t js, size_t ie, size_t je);
AML_DEF void          aml_copy_src_window_to_des(Aml_Mat2d des, Aml_Mat2d src, size_t is, size_t js, size_t ie, size_t je);
AML_DEF Aml_Mat2d         aml_create_col_ref(Aml_Mat2d src, size_t c);
AML_DEF void          aml_cross(Aml_Mat2d dst, Aml_Mat2d v1, Aml_Mat2d v2);

AML_DEF void          aml_dot(Aml_Mat2d dst, Aml_Mat2d a, Aml_Mat2d b);
AML_DEF aml_real    aml_dot_product(Aml_Mat2d v1, Aml_Mat2d v2);
AML_DEF aml_real    aml_det(Aml_Mat2d m);
AML_DEF aml_real    aml_det_2x2_mat(Aml_Mat2d m);
AML_DEF aml_real    aml_det_2x2_mat_minor(Aml_Mat2d_Minor mm);

AML_DEF void          aml_eig_check(Aml_Mat2d A, Aml_Mat2d eigenvalues, Aml_Mat2d eigenvectors, Aml_Mat2d res);
AML_DEF void          aml_eig_power_iteration(Aml_Mat2d A, Aml_Mat2d eigenvalues, Aml_Mat2d eigenvectors, Aml_Mat2d init_vector, bool norm_inf_vectors);
AML_DEF aml_real    aml_elements_sum(Aml_Mat2d m);

AML_DEF void          aml_fill(Aml_Mat2d m, aml_real x);
AML_DEF void          aml_fill_sequence(Aml_Mat2d m, aml_real start, aml_real step);
AML_DEF void          aml_fill_uint32(Aml_Mat2d_uint32 m, uint32_t x);
AML_DEF bool          aml_find_first_non_zero_value(Aml_Mat2d m, size_t r, size_t *non_zero_col);
AML_DEF void          aml_free(Aml_Mat2d m);
AML_DEF void          aml_free_uint32(Aml_Mat2d_uint32 m);

AML_DEF void          aml_householder_matrix_get(Aml_Mat2d des, Aml_Mat2d v);
AML_DEF void          aml_householder_top_element_vector_get(Aml_Mat2d v_des, Aml_Mat2d x);

AML_DEF aml_real    aml_inner_product(Aml_Mat2d v);
AML_DEF void          aml_invert(Aml_Mat2d des, Aml_Mat2d src);
AML_DEF bool          aml_is_symmetric(Aml_Mat2d m);

AML_DEF void          aml_LUP_decomposition_with_swap(Aml_Mat2d src, Aml_Mat2d l, Aml_Mat2d p, Aml_Mat2d u);

AML_DEF void          aml_make_orthogonal_Gaussian_elimination(Aml_Mat2d des, Aml_Mat2d A);
AML_DEF void          aml_make_orthogonal_modified_Gram_Schmidt(Aml_Mat2d des, Aml_Mat2d A);
AML_DEF bool          aml_mat_is_all_digit(Aml_Mat2d m, aml_real digit);
AML_DEF Aml_Mat2d_Minor   aml_minor_alloc_fill_from_mat(Aml_Mat2d ref_mat, size_t i, size_t j);
AML_DEF Aml_Mat2d_Minor   aml_minor_alloc_fill_from_mat_minor(Aml_Mat2d_Minor ref_mm, size_t i, size_t j);
AML_DEF aml_real    aml_minor_det(Aml_Mat2d_Minor mm);
AML_DEF void          aml_minor_free(Aml_Mat2d_Minor mm);
AML_DEF void          aml_minor_print(Aml_Mat2d_Minor mm, const char *name, size_t padding);
AML_DEF void          aml_mult(Aml_Mat2d m, aml_real factor);
AML_DEF void          aml_mult_row(Aml_Mat2d m, size_t r, aml_real factor);

AML_DEF void          aml_normalize(Aml_Mat2d m);
AML_DEF void          aml_normalize_inf(Aml_Mat2d m);

AML_DEF size_t        aml_offset2d(Aml_Mat2d m, size_t i, size_t j);
AML_DEF size_t        aml_offset2d_uint32(Aml_Mat2d_uint32 m, size_t i, size_t j);
AML_DEF void          aml_outer_product(Aml_Mat2d des, Aml_Mat2d v);

AML_DEF int           aml_power_iterate(Aml_Mat2d A, Aml_Mat2d v, aml_real *lambda, aml_real shift, bool norm_inf_v);
AML_DEF void          aml_print(Aml_Mat2d m, const char *name, size_t padding);
AML_DEF void          aml_print_uint32(Aml_Mat2d_uint32 m, const char *name, size_t padding);
AML_DEF void          aml_print_as_col(Aml_Mat2d m, const char *name, size_t padding);
AML_DEF void          aml_project_out_columns(Aml_Mat2d v, Aml_Mat2d basis, size_t used_cols);

AML_DEF void          aml_QR_householder_factorization(Aml_Mat2d Q, Aml_Mat2d R, Aml_Mat2d src);
AML_DEF void          aml_QR_householder_factorization_fast(Aml_Mat2d Q, Aml_Mat2d R, Aml_Mat2d src);

AML_DEF void          aml_rand(Aml_Mat2d m, aml_real low, aml_real high);
AML_DEF aml_real    aml_rand_aml_real(void);
AML_DEF Aml_Mat2d         aml_realloc(Aml_Mat2d m, size_t rows, size_t cols);
AML_DEF Aml_Mat2d_uint32  aml_realloc_uint32(Aml_Mat2d_uint32 m, size_t rows, size_t cols);
AML_DEF size_t        aml_reduce(Aml_Mat2d m);
AML_DEF void          aml_rotate_mat_180_deg_inplace(Aml_Mat2d m);
AML_DEF bool          aml_row_is_all_digit(Aml_Mat2d m, aml_real digit, size_t r);

AML_DEF void          aml_set_DCM_zyx(Aml_Mat2d DCM, float yaw_deg, float pitch_deg, float roll_deg);
AML_DEF void          aml_set_identity(Aml_Mat2d m);
AML_DEF void          aml_set_rot_mat_x(Aml_Mat2d m, float angle_deg);
AML_DEF void          aml_set_rot_mat_y(Aml_Mat2d m, float angle_deg);
AML_DEF void          aml_set_rot_mat_z(Aml_Mat2d m, float angle_deg);
AML_DEF void          aml_shift(Aml_Mat2d m, aml_real shift);
AML_DEF void          aml_solve_linear_sys_LUP_decomposition(Aml_Mat2d A, Aml_Mat2d x, Aml_Mat2d B);
AML_DEF void          aml_sub(Aml_Mat2d dst, Aml_Mat2d a);
AML_DEF void          aml_sub_col_to_col(Aml_Mat2d des, size_t des_col, Aml_Mat2d src, size_t src_col);
AML_DEF void          aml_sub_row_to_row(Aml_Mat2d des, size_t des_row, Aml_Mat2d src, size_t src_row);
AML_DEF void          aml_sub_row_time_factor_to_row(Aml_Mat2d m, size_t des_r, size_t src_r, aml_real factor);
AML_DEF void          aml_SVD_full(Aml_Mat2d A, Aml_Mat2d U, Aml_Mat2d S, Aml_Mat2d V, Aml_Mat2d init_vec_u, Aml_Mat2d init_vec_v, bool return_v_transpose);
AML_DEF void          aml_SVD_thin(Aml_Mat2d A, Aml_Mat2d U, Aml_Mat2d S, Aml_Mat2d V, Aml_Mat2d init_vec_u, Aml_Mat2d init_vec_v, bool return_v_transpose);
AML_DEF void          aml_swap_rows(Aml_Mat2d m, size_t r1, size_t r2);

AML_DEF void          aml_transpose(Aml_Mat2d des, Aml_Mat2d src);
AML_DEF void          aml_transpose_inplace(Aml_Mat2d m);

AML_DEF aml_real    aml_upper_triangulate(Aml_Mat2d m, uint8_t flags);

#endif // ALMOG_MATRIX_LIBRARY_H_

#ifdef ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#undef ALMOG_MATRIX_LIBRARY_IMPLEMENTATION


AML_DEF void aml_add(Aml_Mat2d dst, Aml_Mat2d a)
{
    AML_ASSERT(dst.rows == a.rows);
    AML_ASSERT(dst.cols == a.cols);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            AML_AT(dst, i, j) += AML_AT(a, i, j);
        }
    }
}

AML_DEF void aml_add_col_to_col(Aml_Mat2d des, size_t des_col, Aml_Mat2d src, size_t src_col)
{
    AML_ASSERT(src_col < src.cols);
    AML_ASSERT(des.rows == src.rows);
    AML_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        AML_AT(des, i, des_col) += AML_AT(src, i, src_col);
    }
}

AML_DEF void aml_add_row_to_row(Aml_Mat2d des, size_t des_row, Aml_Mat2d src, size_t src_row)
{
    AML_ASSERT(src_row < src.rows);
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        AML_AT(des, des_row, j) += AML_AT(src, src_row, j);
    }
}

AML_DEF void aml_add_row_time_factor_to_row(Aml_Mat2d m, size_t des_r, size_t src_r, aml_real factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        AML_AT(m, des_r, j) += factor * AML_AT(m, src_r, j);
    }
}

AML_DEF void aml_add_scalar(Aml_Mat2d m, aml_real x)
{
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            AML_AT(m, i, j) += x;
        }
    }
}

AML_DEF Aml_Mat2d aml_alloc(size_t rows, size_t cols)
{
    Aml_Mat2d m;
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (aml_real*)AML_MALLOC(sizeof(aml_real)*rows*cols);
    AML_ASSERT(m.elements != NULL);
    
    return m;
}

AML_DEF Aml_Mat2d_uint32 aml_alloc_uint32(size_t rows, size_t cols)
{
    Aml_Mat2d_uint32 m;
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (uint32_t*)AML_MALLOC(sizeof(uint32_t)*rows*cols);
    AML_ASSERT(m.elements != NULL);
    
    return m;
}

AML_DEF void aml_anti_diag_transpose_inplace(Aml_Mat2d m)
{
    AML_ASSERT(m.cols == m.rows);

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols - i; j++) {
            aml_real temp = AML_AT(m, i, j);
            AML_AT(m, i, j) = AML_AT(m, m.rows - j - 1, m.cols - i - 1);
            AML_AT(m, m.rows - j - 1, m.cols - i - 1) = temp;
        }
    }
}

AML_DEF void aml_apply_householder_left(Aml_Mat2d A, size_t row0, size_t col0, Aml_Mat2d v)
{
    aml_real vv = aml_inner_product(v);
    AML_ASSERT(!AML_IS_ZERO(vv));

    aml_real beta = (aml_real)2 / vv;

    for (size_t j = col0; j < A.cols; ++j) {
        aml_real dot = 0;
        for (size_t i = 0; i < v.rows; ++i) {
            dot += AML_AT(v, i, 0) * AML_AT(A, row0 + i, j);
        }
        dot *= beta;

        for (size_t i = 0; i < v.rows; ++i) {
            AML_AT(A, row0 + i, j) -= dot * AML_AT(v, i, 0);
        }
    }
}

AML_DEF void aml_apply_householder_right(Aml_Mat2d A, size_t col0, Aml_Mat2d v)
{
    aml_real vv = aml_inner_product(v);
    AML_ASSERT(!AML_IS_ZERO(vv));

    aml_real beta = (aml_real)2 / vv;

    for (size_t i = 0; i < A.rows; ++i) {
        aml_real dot = 0;
        for (size_t j = 0; j < v.rows; ++j) {
            dot += AML_AT(A, i, col0 + j) * AML_AT(v, j, 0);
        }
        dot *= beta;

        for (size_t j = 0; j < v.rows; ++j) {
            AML_AT(A, i, col0 + j) -= dot * AML_AT(v, j, 0);
        }
    }
}

AML_DEF aml_real aml_calc_col_norma(Aml_Mat2d m, size_t c)
{
    AML_ASSERT(c < m.cols);

    aml_real sum = 0;
    for (size_t i = 0; i < m.rows; ++i) {
        sum += AML_AT(m, i, c) * AML_AT(m, i, c);
    }
    return aml_sqrt(sum);
}

AML_DEF aml_real aml_calc_norma(Aml_Mat2d m)
{
    aml_real sum = 0;

    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            sum += AML_AT(m, i, j) * AML_AT(m, i, j);
        }
    }
    return aml_sqrt(sum);
}
 
AML_DEF aml_real aml_calc_norma_inf(Aml_Mat2d m)
{
    aml_real max = 0;
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            aml_real current = aml_fabs(AML_AT(m, i, j));
            if (current > max) {
                max = current;
            }
        }
    }

    return max;
}

AML_DEF bool aml_col_is_all_digit(Aml_Mat2d m, aml_real digit, size_t c)
{
    for (size_t i = 0; i < m.rows; ++i) {
        if (AML_AT(m, i, c) != digit) {
            return false;
        }
    }
    return true;
}

void aml_convolve(Aml_Mat2d m, Aml_Mat2d a, Aml_Mat2d b) 
{
    AML_ASSERT(a.cols >= b.cols);
    AML_ASSERT(a.rows >= b.rows);
    AML_ASSERT(m.rows == (a.rows - b.rows + 1));
    AML_ASSERT(m.cols == (a.cols - b.cols + 1));
    AML_ASSERT(b.cols == b.rows);

    aml_rotate_mat_180_deg_inplace(b);

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            Aml_Mat2d window = {
                .cols = b.cols,
                .rows = b.rows,
                .stride_r = a.stride_r,
                .elements = &(AML_AT(a, i, j))
            };

            AML_AT(m, i, j) = aml_dot_product(window, b);
        }
    }

}

AML_DEF void aml_copy(Aml_Mat2d des, Aml_Mat2d src)
{
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des.rows == src.rows);

    for (size_t i = 0; i < des.rows; ++i) {
        for (size_t j = 0; j < des.cols; ++j) {
            AML_AT(des, i, j) = AML_AT(src, i, j);
        }
    }
}

AML_DEF void aml_copy_uint32(Aml_Mat2d_uint32 des, Aml_Mat2d_uint32 src)
{
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des.rows == src.rows);

    for (size_t i = 0; i < des.rows; ++i) {
        for (size_t j = 0; j < des.cols; ++j) {
            AML_AT(des, i, j) = AML_AT(src, i, j);
        }
    }
}

AML_DEF void aml_copy_col_from_src_to_des(Aml_Mat2d des, size_t des_col, Aml_Mat2d src, size_t src_col)
{
    AML_ASSERT(src_col < src.cols);
    AML_ASSERT(des.rows == src.rows);
    AML_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        AML_AT(des, i, des_col) = AML_AT(src, i, src_col);
    }
}

AML_DEF void aml_copy_row_from_src_to_des(Aml_Mat2d des, size_t des_row, Aml_Mat2d src, size_t src_row)
{
    AML_ASSERT(src_row < src.rows);
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        AML_AT(des, des_row, j) = AML_AT(src, src_row, j);
    }
}

AML_DEF void aml_copy_src_to_des_window(Aml_Mat2d des, Aml_Mat2d src, size_t is, size_t js, size_t ie, size_t je)
{
    AML_ASSERT(je >= js && ie >= is);
    AML_ASSERT(je-js+1 == src.cols);
    AML_ASSERT(ie-is+1 == src.rows);
    AML_ASSERT(je-js+1 <= des.cols);
    AML_ASSERT(ie-is+1 <= des.rows);

    for (size_t index = 0; index < src.rows; ++index) {
        for (size_t jndex = 0; jndex < src.cols; ++jndex) {
            AML_AT(des, is+index, js+jndex) = AML_AT(src, index, jndex);
        }
    }
}

AML_DEF void aml_copy_src_window_to_des(Aml_Mat2d des, Aml_Mat2d src, size_t is, size_t js, size_t ie, size_t je)
{
    AML_ASSERT(je >= js && ie >= is);
    AML_ASSERT(je-js+1 == des.cols);
    AML_ASSERT(ie-is+1 == des.rows);
    AML_ASSERT(je-js+1 <= src.cols);
    AML_ASSERT(ie-is+1 <= src.rows);

    for (size_t index = 0; index < des.rows; ++index) {
        for (size_t jndex = 0; jndex < des.cols; ++jndex) {
            AML_AT(des, index, jndex) = AML_AT(src, is+index, js+jndex);
        }
    }
}

AML_DEF Aml_Mat2d aml_create_col_ref(Aml_Mat2d src, size_t c)
{
    AML_ASSERT(c < src.cols);

    Aml_Mat2d col = {.cols = 1,
                 .rows = src.rows,
                 .stride_r = src.stride_r,
                 .elements = &(AML_AT(src, 0, c))};

    return col;
}

void aml_cross(Aml_Mat2d dst, Aml_Mat2d v1, Aml_Mat2d v2)
{
    AML_ASSERT(3 == dst.rows && 1 == dst.cols);
    AML_ASSERT(3 == v1.rows && 1 == v1.cols);
    AML_ASSERT(3 == v2.rows && 1 == v2.cols);

    AML_AT(dst, 0, 0) = AML_AT(v1, 1, 0) * AML_AT(v2, 2, 0) - AML_AT(v1, 2, 0) * AML_AT(v2, 1, 0);
    AML_AT(dst, 1, 0) = AML_AT(v1, 2, 0) * AML_AT(v2, 0, 0) - AML_AT(v1, 0, 0) * AML_AT(v2, 2, 0);
    AML_AT(dst, 2, 0) = AML_AT(v1, 0, 0) * AML_AT(v2, 1, 0) - AML_AT(v1, 1, 0) * AML_AT(v2, 0, 0);
}

AML_DEF void aml_dot(Aml_Mat2d dst, Aml_Mat2d a, Aml_Mat2d b)
{
    AML_ASSERT(a.cols == b.rows);
    AML_ASSERT(a.rows == dst.rows);
    AML_ASSERT(b.cols == dst.cols);

    size_t i, j, k;

    for (i = 0; i < dst.rows; i++) {
        for (j = 0; j < dst.cols; j++) {
            AML_AT(dst, i, j) = 0;
            for (k = 0; k < a.cols; k++) {
                AML_AT(dst, i, j) += AML_AT(a, i, k)*AML_AT(b, k, j);
            }
        }
    }

}

AML_DEF aml_real aml_dot_product(Aml_Mat2d m1, Aml_Mat2d m2)
{
    AML_ASSERT(m1.rows == m2.rows);
    AML_ASSERT(m1.cols == m2.cols);

    aml_real dot_product = 0;

    for (size_t i = 0; i < m1.rows; i++) {
        for (size_t j = 0; j < m1.cols; j++) {
            dot_product += AML_AT(m1, i, j) * AML_AT(m2, i, j);
        }
    }
    
    return dot_product;
}

AML_DEF aml_real aml_det(Aml_Mat2d m)
{
    AML_ASSERT(m.cols == m.rows && "should be a square matrix");

    /* checking if there is a row or column with all zeros */
    /* checking rows */
    for (size_t i = 0; i < m.rows; i++) {
        if (aml_row_is_all_digit(m, 0, i)) {
            return 0;
        }
    }
    /* checking cols */
    for (size_t j = 0; j < m.rows; j++) {
        if (aml_col_is_all_digit(m, 0, j)) {
            return 0;
        }
    }

    Aml_Mat2d temp_m = aml_alloc(m.rows, m.cols);
    aml_copy(temp_m, m);
    aml_real factor = aml_upper_triangulate(temp_m, AML_ROW_SWAPPING);
    aml_real diag_mul = 1; 
    for (size_t i = 0; i < temp_m.rows; i++) {
        diag_mul *= AML_AT(temp_m, i, i);
    }
    aml_free(temp_m);

    return diag_mul / factor;
}

AML_DEF aml_real aml_det_2x2_mat(Aml_Mat2d m)
{
    AML_ASSERT(2 == m.cols && 2 == m.rows && "Not a 2x2 matrix");
    return AML_AT(m, 0, 0) * AML_AT(m, 1, 1) - AML_AT(m, 0, 1) * AML_AT(m, 1, 0);
}

AML_DEF aml_real aml_det_2x2_mat_minor(Aml_Mat2d_Minor mm)
{
    AML_ASSERT(2 == mm.cols && 2 == mm.rows && "Not a 2x2 matrix");
    return AML_MINOR_AT(mm, 0, 0) * AML_MINOR_AT(mm, 1, 1) - AML_MINOR_AT(mm, 0, 1) * AML_MINOR_AT(mm, 1, 0);
}

AML_DEF void aml_eig_check(Aml_Mat2d A, Aml_Mat2d eigenvalues, Aml_Mat2d eigenvectors, Aml_Mat2d res)
{
    AML_ASSERT(A.cols == A.rows); 
    AML_ASSERT(eigenvalues.cols == A.cols);
    AML_ASSERT(eigenvalues.rows == A.rows);
    AML_ASSERT(eigenvectors.cols == A.cols);
    AML_ASSERT(eigenvectors.rows == A.rows);
    AML_ASSERT(res.cols == A.cols);
    AML_ASSERT(res.rows == A.rows);

    aml_dot(res, A, eigenvectors);
    Aml_Mat2d VL = aml_alloc(A.rows, A.cols);
    aml_dot(VL, eigenvectors, eigenvalues);

    aml_sub(res, VL);

    aml_free(VL);
}

/**
 * @brief Estimate eigenvalues/eigenvectors using repeated power iteration with deflation.
 *
 * @details
 * Repeatedly applies aml_power_iterate() to estimate an eigenpair of the
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
 *                         by aml_normalize_inf().
 *
 * @warning This implementation is primarily educational and makes strong
 *          assumptions; it may fail or be inaccurate for matrices that do not
 *          satisfy the power-iteration convergence conditions.
 *
 * @pre A is square; eigenvalues/eigenvectors are N x N; init_vector is N x 1.
 */
void aml_eig_power_iteration(Aml_Mat2d A, Aml_Mat2d eigenvalues, Aml_Mat2d eigenvectors, Aml_Mat2d init_vector, bool norm_inf_vectors)
{
    /* https://www.youtube.com/watch?v=c8DIOzuZqBs */

    /** Conditions:
     *      - The eigenvectors must form an orthonormal basis 
     *      - The largest eigenvalue must be positive and unique
     */ 

    AML_ASSERT(A.cols == A.rows); 
    AML_ASSERT(eigenvalues.cols == A.cols);
    AML_ASSERT(eigenvalues.rows == A.rows);
    AML_ASSERT(eigenvectors.cols == A.cols);
    AML_ASSERT(eigenvectors.rows == A.rows);
    AML_ASSERT(init_vector.cols == 1);
    AML_ASSERT(init_vector.rows == A.rows);
    AML_ASSERT(aml_calc_norma_inf(init_vector) > 0);

    aml_fill(eigenvalues, 0);
    Aml_Mat2d B = aml_alloc(A.rows, A.cols);
    Aml_Mat2d temp_mat = aml_alloc(A.rows, A.cols);
    aml_copy(B, A);

    for (int i = 0, shift_value = 0; i < (int)A.rows; i++) {
        aml_copy_src_to_des_window(eigenvectors, init_vector, 0, i, init_vector.rows-1, i);
        Aml_Mat2d v = {.cols = init_vector.cols,
                   .elements = &AML_AT(eigenvectors, 0, i),
                   .rows = init_vector.rows,
                   .stride_r = eigenvectors.stride_r};
        aml_project_out_columns(v, eigenvectors, i);
        if (!AML_IS_ZERO(aml_calc_norma(v))) {
            aml_normalize(v);
        }
        if (aml_power_iterate(B, v, &AML_AT(eigenvalues, i, i), shift_value, 0)) { /* norm_inf_v must be zero*/
            shift_value++;
            i--;
            continue;
        } else {
            shift_value = 0;
        }
        if (!AML_IS_ZERO(aml_calc_norma(v))) {
            aml_normalize(v);
        }
        aml_outer_product(temp_mat, v);
        aml_mult(temp_mat, AML_AT(eigenvalues, i, i));
        aml_sub(B, temp_mat);
    }

    if (norm_inf_vectors) {
        for (size_t c = 0; c < eigenvectors.cols; c++) {
            Aml_Mat2d v = {.cols = init_vector.cols,
                    .elements = &AML_AT(eigenvectors, 0, c),
                    .rows = init_vector.rows,
                    .stride_r = eigenvectors.stride_r};
            aml_normalize_inf(v);
        }
    }

    aml_free(B);
    aml_free(temp_mat);
}

AML_DEF aml_real aml_elements_sum(Aml_Mat2d m)
{
    aml_real sum = 0;

    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            sum += AML_AT(m, i, j);
        }
    }
    return sum;
}

AML_DEF void aml_fill(Aml_Mat2d m, aml_real x)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_AT(m, i, j) = x;
        }
    }
}

AML_DEF void aml_fill_sequence(Aml_Mat2d m, aml_real start, aml_real step) {
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            AML_AT(m, i, j) = start + step * aml_offset2d(m, i, j);
        }
    }
}

AML_DEF void aml_fill_uint32(Aml_Mat2d_uint32 m, uint32_t x)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_AT(m, i, j) = x;
        }
    }
}

AML_DEF bool aml_find_first_non_zero_value(Aml_Mat2d m, size_t r, size_t *non_zero_col)
{
    for (size_t c = 0; c < m.cols; ++c) {
        if (!AML_IS_ZERO(AML_AT(m, r, c))) {
            *non_zero_col = c;
            return true;
        }
    }
    return false;
}

AML_DEF void aml_free(Aml_Mat2d m)
{
    AML_FREE(m.elements);
}

AML_DEF void aml_free_uint32(Aml_Mat2d_uint32 m)
{
    AML_FREE(m.elements);
}

AML_DEF void aml_householder_matrix_get(Aml_Mat2d des, Aml_Mat2d v)
{
    AML_ASSERT(v.cols == 1);
    AML_ASSERT(des.cols == des.rows);
    AML_ASSERT(des.cols == v.rows);

    Aml_Mat2d outer_product = aml_alloc(v.rows, v.rows);
    aml_outer_product(outer_product, v);
    aml_real inner_product = aml_inner_product(v);
    aml_mult(outer_product, (aml_real)2 / inner_product);
    
    aml_set_identity(des);

    aml_sub(des, outer_product);

    aml_free(outer_product);
}

AML_DEF void aml_householder_top_element_vector_get(Aml_Mat2d v_des, Aml_Mat2d x)
{
    AML_ASSERT(x.cols == 1);
    AML_ASSERT(v_des.cols == x.cols);
    AML_ASSERT(v_des.rows == x.rows);

    aml_real norm = aml_calc_norma(x);

    aml_copy(v_des, x);
    
    if (AML_AT(x, 0, 0) > 0) {
        AML_AT(v_des, 0, 0) += norm;
    } else {
        AML_AT(v_des, 0, 0) -= norm;
    }
}

AML_DEF aml_real aml_inner_product(Aml_Mat2d v)
{
    AML_ASSERT((1 == v.cols) || (1 == v.rows));

    aml_real dot_product = 0;

    if (1 == v.cols) {
        for (size_t i = 0; i < v.rows; i++) {
            dot_product += AML_AT(v, i, 0) * AML_AT(v, i, 0);
        }
    } else {
        for (size_t j = 0; j < v.cols; j++) {
            dot_product += AML_AT(v, 0, j) * AML_AT(v, 0, j);
        }
    }
    
    return dot_product;
}

AML_DEF void aml_invert(Aml_Mat2d des, Aml_Mat2d src)
{
    AML_ASSERT(src.cols == src.rows && "Must be an NxN matrix");
    AML_ASSERT(des.cols == src.cols && des.rows == des.cols);

    Aml_Mat2d m = aml_alloc(src.rows, src.cols * 2);
    aml_copy_src_to_des_window(m, src, 0, 0, src.rows-1, src.cols-1);

    aml_set_identity(des);
    aml_copy_src_to_des_window(m, des, 0, src.cols, des.rows-1, 2 * des.cols-1);
    
    aml_reduce(m);

    aml_copy_src_window_to_des(des, m, 0, src.cols, des.rows-1, 2 * des.cols-1);

    aml_free(m);
}

AML_DEF bool aml_is_symmetric(Aml_Mat2d m)
{
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            if (!AML_IS_ZERO(AML_AT(m, i, j) - AML_AT(m, j , i))) {
                return false;
            }
        }
    }

    return true;
}

AML_DEF void aml_LUP_decomposition_with_swap(Aml_Mat2d src, Aml_Mat2d l, Aml_Mat2d p, Aml_Mat2d u)
{
    /* performing LU decomposition Following the Wikipedia page: https://en.wikipedia.org/wiki/LU_decomposition */

    aml_copy(u, src);
    aml_set_identity(p);
    aml_fill(l, 0);

    for (size_t i = 0; i < (size_t)aml_min(u.rows-1, u.cols); i++) {
        if (AML_IS_ZERO(AML_AT(u, i, i))) {   /* swapping only if it is zero */
            /* finding biggest first number (absolute value) */
            size_t biggest_r = i;
            for (size_t index = i; index < u.rows; index++) {
                if (aml_fabs(AML_AT(u, index, i)) > aml_fabs(AML_AT(u, biggest_r, i))) {
                    biggest_r = index;
                }
            }
            if (i != biggest_r) {
                aml_swap_rows(u, i, biggest_r);
                aml_swap_rows(p, i, biggest_r);
                aml_swap_rows(l, i, biggest_r);
            }
        }
        for (size_t j = i+1; j < u.rows; j++) {
            aml_real factor = 1 / AML_AT(u, i, i);
            if (!isfinite(factor)) {
                printf("%s:%d:\n%s:\n[Error] unable to transfrom into uper triangular matrix. Probably some of the rows are not independent.\n", __FILE__, __LINE__, __func__);
            }
            aml_real mat_value = AML_AT(u, j, i);
            aml_sub_row_time_factor_to_row(u, j, i, mat_value * factor);
            AML_AT(l, j, i) = mat_value * factor;
        }
        AML_AT(l, i, i) = 1;
    }
    AML_AT(l, l.rows-1, l.cols-1) = 1;
}

AML_DEF void aml_make_orthogonal_Gaussian_elimination(Aml_Mat2d des, Aml_Mat2d A)
{
    /* https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process */
    /** 
     * A^TA must be fully ranked, i.e. 
     * columns must be linearly independent and non zero.
     * 
     */

    AML_ASSERT(des.cols == A.cols);
    AML_ASSERT(des.rows == A.rows);

    Aml_Mat2d AT = aml_alloc(A.cols, A.rows);
    Aml_Mat2d ATA = aml_alloc(A.cols, A.cols);
    Aml_Mat2d temp = aml_alloc(ATA.rows, ATA.cols + A.cols);
    Aml_Mat2d temp_des = aml_alloc(des.cols, des.rows);

    aml_transpose(AT, A);
    aml_dot(ATA, AT, A);
    aml_copy_src_to_des_window(temp, ATA, 0, 0, ATA.rows-1, ATA.cols-1);
    aml_copy_src_to_des_window(temp, AT, 0, ATA.cols, AT.rows-1, ATA.cols + AT.cols-1);

    AML_PRINT(temp);

    aml_upper_triangulate(temp, AML_ONES_ON_DIAG);

    aml_copy_src_window_to_des(temp_des, temp, 0, ATA.cols, AT.rows-1, ATA.cols + AT.cols-1);

    aml_transpose(des, temp_des);

    AML_PRINT(temp);

    aml_free(AT);
    aml_free(ATA);
    aml_free(temp);
    aml_free(temp_des);
}

AML_DEF void aml_make_orthogonal_modified_Gram_Schmidt(Aml_Mat2d des, Aml_Mat2d A)
{
    /* https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process */
    AML_ASSERT(des.rows == A.rows);
    AML_ASSERT(des.cols == des.rows);

    size_t num_non_zero_vec = 0;
    for (size_t c = 0; c < A.cols; c++) {
        if (!AML_IS_ZERO(aml_calc_col_norma(A, c))) {
            num_non_zero_vec++;
        }
    }

    aml_rand(des, 1, 2);

    Aml_Mat2d temp_col = aml_alloc(des.rows, 1);
    size_t out_c = 0;
    for (size_t c = 0; c < A.cols; c++) {
        if (!AML_IS_ZERO(aml_calc_col_norma(A, c))) {
            aml_copy_col_from_src_to_des(des, out_c, A, c);
            out_c++;
        }
    }
    for (size_t c = 0; c < des.cols-1; c++) {
        Aml_Mat2d vc = aml_create_col_ref(des, c);
        aml_real vc_vc = aml_inner_product(vc);
        for (size_t k = c+1; k < des.cols; k++) {
            aml_copy(temp_col, vc);
            Aml_Mat2d vk = aml_create_col_ref(des, k);
            aml_real vk_vc = aml_dot_product(vc, vk);
            aml_mult(temp_col, vk_vc / vc_vc);
            aml_sub(vk, temp_col);
        }
        if (!AML_IS_ZERO(aml_calc_norma(vc))) {
            aml_normalize(vc);
        }
    }
    Aml_Mat2d vc = aml_create_col_ref(des, des.cols-1);
    if (!AML_IS_ZERO(aml_calc_norma(vc))) {
        aml_normalize(vc);
    }


    aml_free(temp_col);    
}

AML_DEF bool aml_mat_is_all_digit(Aml_Mat2d m, aml_real digit)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            if (AML_AT(m, i, j) != digit) {
                return false;
            }
        }
    }
    return true;
}

AML_DEF Aml_Mat2d_Minor aml_minor_alloc_fill_from_mat(Aml_Mat2d ref_mat, size_t i, size_t j)
{
    AML_ASSERT(ref_mat.cols == ref_mat.rows && "minor is defined only for square matrix");

    Aml_Mat2d_Minor mm;
    mm.cols = ref_mat.cols-1;
    mm.rows = ref_mat.rows-1;
    mm.stride_r = ref_mat.cols-1;
    mm.cols_list = (size_t*)AML_MALLOC(sizeof(size_t)*(ref_mat.cols-1));
    mm.rows_list = (size_t*)AML_MALLOC(sizeof(size_t)*(ref_mat.rows-1));
    mm.ref_mat = ref_mat;

    AML_ASSERT(mm.cols_list != NULL && mm.rows_list != NULL);

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

AML_DEF Aml_Mat2d_Minor aml_minor_alloc_fill_from_mat_minor(Aml_Mat2d_Minor ref_mm, size_t i, size_t j)
{
    AML_ASSERT(ref_mm.cols == ref_mm.rows && "minor is defined only for square matrix");

    Aml_Mat2d_Minor mm;
    mm.cols = ref_mm.cols-1;
    mm.rows = ref_mm.rows-1;
    mm.stride_r = ref_mm.cols-1;
    mm.cols_list = (size_t*)AML_MALLOC(sizeof(size_t)*(ref_mm.cols-1));
    mm.rows_list = (size_t*)AML_MALLOC(sizeof(size_t)*(ref_mm.rows-1));
    mm.ref_mat = ref_mm.ref_mat;

    AML_ASSERT(mm.cols_list != NULL && mm.rows_list != NULL);

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

AML_DEF aml_real aml_minor_det(Aml_Mat2d_Minor mm)
{
    AML_ASSERT(mm.cols == mm.rows && "should be a square matrix");

    aml_real det = 0;
    /* TODO: finding beast row or col? */
    for (size_t i = 0, j = 0; i < mm.rows; i++) { /* first column */
        if (aml_fabs(AML_MINOR_AT(mm, i, j)) < 1e-10) continue;
        Aml_Mat2d_Minor sub_mm = aml_minor_alloc_fill_from_mat_minor(mm, i, j);
        int factor = (i+j)%2 ? -1 : 1;
        if (sub_mm.cols != 2) {
            AML_ASSERT(sub_mm.cols == sub_mm.rows && "should be a square matrix");
            det += AML_MINOR_AT(mm, i, j) * (factor) * aml_minor_det(sub_mm);
        } else if (sub_mm.cols == 2 && sub_mm.rows == 2) {
            det += AML_MINOR_AT(mm, i, j) * (factor) * aml_det_2x2_mat_minor(sub_mm);;
        }
        aml_minor_free(sub_mm);
    }
    return det;
}

AML_DEF void aml_minor_free(Aml_Mat2d_Minor mm)
{
    AML_FREE(mm.cols_list);
    AML_FREE(mm.rows_list);
}

AML_DEF void aml_minor_print(Aml_Mat2d_Minor mm, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < mm.rows; ++i) {
        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < mm.cols; ++j) {
            printf("%f ", AML_MINOR_AT(mm, i, j));
        }
        printf("\n");
    }
    printf("%*s]\n", (int) padding, "");
}

AML_DEF void aml_mult(Aml_Mat2d m, aml_real factor)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_AT(m, i, j) *= factor;
        }
    }
}

AML_DEF void aml_mult_row(Aml_Mat2d m, size_t r, aml_real factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        AML_AT(m, r, j) *= factor;
    }
}

AML_DEF void aml_normalize(Aml_Mat2d m)
{
    aml_real norma = aml_calc_norma(m);
    if (AML_IS_ZERO(norma)) {
        return;
    }

    aml_mult(m, (aml_real)1 / norma);
}

AML_DEF void aml_normalize_inf(Aml_Mat2d m)
{
    aml_real norma = aml_calc_norma_inf(m);
    if (AML_IS_ZERO(norma)) {
        return;
    }

    aml_mult(m, (aml_real)1 / norma);
}

AML_DEF size_t aml_offset2d(Aml_Mat2d m, size_t i, size_t j)
{
    AML_ASSERT(i < m.rows && j < m.cols);
    return i * m.stride_r + j;
}

AML_DEF size_t aml_offset2d_uint32(Aml_Mat2d_uint32 m, size_t i, size_t j)
{
    AML_ASSERT(i < m.rows && j < m.cols);
    return i * m.stride_r + j;
}

AML_DEF void aml_outer_product(Aml_Mat2d des, Aml_Mat2d v)
{
    AML_ASSERT(des.cols == des.rows);
    AML_ASSERT((1 == v.cols && des.rows == v.rows) || (1 == v.rows && des.cols == v.cols));

    if (1 == v.cols) {
        for (size_t i = 0; i < des.rows; i++) {
            for (size_t j = 0; j < des.cols; j++) {
                AML_AT(des, i, j) = AML_AT(v, i, 0) * AML_AT(v, j, 0);
            }
        }
    } else {
        for (size_t i = 0; i < des.rows; i++) {
            for (size_t j = 0; j < des.cols; j++) {
                AML_AT(des, i, j) = AML_AT(v, 0, i) * AML_AT(v, 0, j);
            }
        }
    }
}

AML_DEF int aml_power_iterate(Aml_Mat2d A, Aml_Mat2d v, aml_real *lambda, aml_real shift, bool norm_inf_v)
{
    /* https://www.youtube.com/watch?v=SkPusgctgpI */

    /** Conditions:
     *      - The eigenvectors must form a basis 
     *      - The largest eigenvalue must be positive and unique
     */ 

    AML_ASSERT(A.cols == A.rows); 
    AML_ASSERT(v.cols == 1);
    AML_ASSERT(v.rows == A.rows);
    AML_ASSERT(aml_calc_norma_inf(v) > 0);

    Aml_Mat2d current_v = aml_alloc(v.rows, v.cols);
    Aml_Mat2d temp_v = aml_alloc(v.rows, v.cols);
    Aml_Mat2d B = aml_alloc(A.rows, A.cols);
    aml_copy(B, A);
    aml_shift(B, shift * -(aml_real)1);

    aml_real temp_lambda = 0;
    aml_real diff = 0;
    aml_real diff1 = 0;
    aml_real diff2 = 0;

    /* Rayleigh quotient */
    aml_dot(temp_v, B, v);
    temp_lambda = aml_dot_product(temp_v, v) / (aml_calc_norma(v) * aml_calc_norma(v));
    int i = 0;
    for (i = 0; i < AML_MAX_POWER_ITERATION; i++) {
        aml_copy(current_v, v);
        aml_dot(v, B, current_v);
        aml_normalize(v);
        aml_mult(v, temp_lambda > 0 ? (aml_real)1 : -(aml_real)11);
        // aml_mult(v, fabs(lambda) / lambda);
        aml_dot(temp_v, B, v);
        temp_lambda = aml_dot_product(temp_v, v);

        aml_sub(current_v, v);
        diff1 = aml_calc_norma_inf(current_v);
        aml_add(current_v, v);
        aml_add(current_v, v);
        diff2 = aml_calc_norma_inf(current_v);
        diff = aml_min(diff1, diff2);
        if (diff < AML_EPS) {
            break;
        }
    }

    aml_free(current_v);
    aml_free(temp_v);
    aml_free(B);

    if (norm_inf_v) aml_normalize_inf(v);
    if (lambda) *lambda = temp_lambda + shift;

    if (diff > AML_EPS) {
        return 1; /* eigenvector alternating between two options */
    } else {
        return 0;
    }
}

AML_DEF void aml_print(Aml_Mat2d m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows; ++i) {
        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < m.cols; ++j) {
            printf("%9.6f ", AML_AT(m, i, j));
        }
        printf("\n");
    }
    printf("%*s]\n", (int) padding, "");
}

AML_DEF void aml_print_uint32(Aml_Mat2d_uint32 m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows; ++i) {
        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < m.cols; ++j) {
            printf("%#10X ", AML_AT(m, i, j));
        }
        printf("\n");
    }
    printf("%*s]\n", (int) padding, "");
}

AML_DEF void aml_print_as_col(Aml_Mat2d m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows*m.cols; ++i) {
            printf("%*s    ", (int) padding, "");
            printf("%f\n", m.elements[i]);
    }
    printf("%*s]\n", (int) padding, "");
}

AML_DEF void aml_project_out_columns(Aml_Mat2d v, Aml_Mat2d basis, size_t used_cols)
{
    /* Gram-Schmidt */
    AML_ASSERT(v.cols == 1);
    AML_ASSERT(basis.rows == v.rows);

    Aml_Mat2d temp = aml_alloc(v.rows, 1);

    for (size_t c = 0; c < used_cols; ++c) {
        Aml_Mat2d bc = aml_create_col_ref(basis, c);
        aml_real alpha = aml_dot_product(v, bc);
        aml_copy(temp, bc);
        aml_mult(temp, alpha);
        aml_sub(v, temp);
    }

    aml_free(temp);
}

AML_DEF void aml_QR_householder_factorization(Aml_Mat2d Q, Aml_Mat2d R, Aml_Mat2d src)
{
    AML_ASSERT(Q.cols == Q.rows);
    AML_ASSERT(R.rows == src.rows);
    AML_ASSERT(R.cols == src.cols);
    AML_ASSERT(Q.cols == R.rows);

    AML_ASSERT(src.rows > 0);
    AML_ASSERT(src.cols > 0);

    Aml_Mat2d vector = aml_alloc(src.rows, 1);
    Aml_Mat2d house  = aml_alloc(Q.rows, Q.cols);
    Aml_Mat2d prev_Q = aml_alloc(Q.rows, Q.cols);
    Aml_Mat2d prev_R = aml_alloc(R.rows, R.cols);

    aml_set_identity(prev_Q);
    aml_copy(prev_R, src);
    aml_set_identity(Q);
    aml_copy(R, src);

    Aml_Mat2d col = {.cols = 1};
    Aml_Mat2d temp_col = {.cols = 1};
    Aml_Mat2d temp_h = {0};
    
    for (size_t i = 0; i < aml_min(src.rows-1, src.cols); i++) {
        col.rows = prev_R.rows - i;
        col.elements = &AML_AT(prev_R, i, i);
        col.stride_r = prev_R.stride_r;

        temp_col.rows = vector.rows - i;
        temp_col.elements = &AML_AT(vector, i, 0);
        temp_col.stride_r = vector.stride_r;
        
        temp_h.rows = house.rows - i;
        temp_h.cols = house.cols - i;
        temp_h.elements = &AML_AT(house, i, i);
        temp_h.stride_r= house.stride_r;

        aml_fill(vector, 1);
        aml_set_identity(house);

        aml_householder_top_element_vector_get(temp_col, col);
        aml_householder_matrix_get(temp_h, temp_col);

        aml_dot(R, house, prev_R);
        aml_dot(Q, prev_Q, house);

        aml_copy(prev_Q, Q);
        aml_copy(prev_R, R);
    }


    aml_free(vector);
    aml_free(house);
    aml_free(prev_Q);
    aml_free(prev_R);
}

AML_DEF void aml_QR_householder_factorization_fast(Aml_Mat2d Q, Aml_Mat2d R, Aml_Mat2d src)
{
    AML_ASSERT(Q.rows == Q.cols);
    AML_ASSERT(R.rows == src.rows);
    AML_ASSERT(R.cols == src.cols);
    AML_ASSERT(Q.rows == src.rows);

    aml_copy(R, src);
    aml_set_identity(Q);

    Aml_Mat2d vbuf = aml_alloc(src.rows, 1);

    size_t steps = aml_min(src.rows - 1, src.cols);
    for (size_t k = 0; k < steps; ++k) {
        Aml_Mat2d x = {
            .rows = R.rows - k,
            .cols = 1,
            .stride_r = R.stride_r,
            .elements = &AML_AT(R, k, k),
        };

        Aml_Mat2d v = {
            .rows = vbuf.rows - k,
            .cols = 1,
            .stride_r = vbuf.stride_r,
            .elements = &AML_AT(vbuf, k, 0),
        };

        aml_householder_top_element_vector_get(v, x);

        if (AML_IS_ZERO(aml_inner_product(v))) {
            continue;
        }

        aml_apply_householder_left(R, k, k, v);
        aml_apply_householder_right(Q, k, v);
    }

    aml_free(vbuf);
}

AML_DEF void aml_rand(Aml_Mat2d m, aml_real low, aml_real high)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_AT(m, i, j) = aml_rand_aml_real()*(high - low) + low;
        }
    }
}

AML_DEF aml_real aml_rand_aml_real(void)
{
    return (aml_real) rand() / (aml_real) RAND_MAX;
}

AML_DEF Aml_Mat2d aml_realloc(Aml_Mat2d m, size_t rows, size_t cols)
{
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (aml_real*)AML_REALLOC(m.elements, sizeof(aml_real)*rows*cols);
    AML_ASSERT(m.elements != NULL);
    
    return m;
}

AML_DEF Aml_Mat2d_uint32 aml_realloc_uint32(Aml_Mat2d_uint32 m, size_t rows, size_t cols)
{
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (uint32_t*)AML_REALLOC(m.elements, sizeof(uint32_t)*rows*cols);
    AML_ASSERT(m.elements != NULL);
    
    return m;
}

AML_DEF size_t aml_reduce(Aml_Mat2d m)
{
    /* preforming Gauss–Jordan reduction to Reduced Row Echelon Form (RREF) */
    /* Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */

    aml_upper_triangulate(m, AML_ONES_ON_DIAG | AML_ROW_SWAPPING);

    size_t rank = 0;

    for (int r = (int)m.rows-1; r >= 0; r--) {
        size_t c = m.cols-1;
        if (!aml_find_first_non_zero_value(m, r, &c)) {
            continue; /* row of zeros */
        }
        for (int i = 0; i < r; i++) {
            aml_real factor = AML_AT(m, i, c);
            aml_sub_row_time_factor_to_row(m, i, r, factor);
        }
        rank++;
    }

    return rank;
}

AML_DEF void aml_rotate_mat_180_deg_inplace(Aml_Mat2d m)
{
    aml_transpose_inplace(m);
    aml_anti_diag_transpose_inplace(m);
}

AML_DEF bool aml_row_is_all_digit(Aml_Mat2d m, aml_real digit, size_t r)
{
    for (size_t j = 0; j < m.cols; ++j) {
        if (AML_AT(m, r, j) != digit) {
            return false;
        }
    }
    return true;
}

AML_DEF void aml_set_DCM_zyx(Aml_Mat2d DCM, float yaw_deg, float pitch_deg, float roll_deg)
{
    Aml_Mat2d RotZ = aml_alloc(3,3);
    aml_set_rot_mat_z(RotZ, yaw_deg);
    Aml_Mat2d RotY = aml_alloc(3,3);
    aml_set_rot_mat_y(RotY, pitch_deg);
    Aml_Mat2d RotX = aml_alloc(3,3);
    aml_set_rot_mat_x(RotX, roll_deg);
    Aml_Mat2d temp = aml_alloc(3,3);

    aml_dot(temp, RotY, RotZ);
    aml_dot(DCM, RotX, temp); /* I have a DCM */

    aml_free(RotZ);
    aml_free(RotY);
    aml_free(RotX);
    aml_free(temp);
}

AML_DEF void aml_set_identity(Aml_Mat2d m)
{
    AML_ASSERT(m.cols == m.rows);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_AT(m, i, j) = i == j ? (aml_real)1 : (aml_real)0;
        }
    }
}

AML_DEF void aml_set_rot_mat_x(Aml_Mat2d m, float angle_deg)
{
    AML_ASSERT(3 == m.cols && 3 == m.rows);

    float angle_rad = angle_deg * (float)AML_PI / 180.0f;
    aml_set_identity(m);
    AML_AT(m, 1, 1) =  aml_cos(angle_rad);
    AML_AT(m, 1, 2) =  aml_sin(angle_rad);
    AML_AT(m, 2, 1) = -aml_sin(angle_rad);
    AML_AT(m, 2, 2) =  aml_cos(angle_rad);
}

AML_DEF void aml_set_rot_mat_y(Aml_Mat2d m, float angle_deg)
{
    AML_ASSERT(3 == m.cols && 3 == m.rows);

    float angle_rad = angle_deg * (float)AML_PI / 180.0f;
    aml_set_identity(m);
    AML_AT(m, 0, 0) =  aml_cos(angle_rad);
    AML_AT(m, 0, 2) = -aml_sin(angle_rad);
    AML_AT(m, 2, 0) =  aml_sin(angle_rad);
    AML_AT(m, 2, 2) =  aml_cos(angle_rad);
}

AML_DEF void aml_set_rot_mat_z(Aml_Mat2d m, float angle_deg)
{
    AML_ASSERT(3 == m.cols && 3 == m.rows);

    float angle_rad = angle_deg * (float)AML_PI / 180.0f;
    aml_set_identity(m);
    AML_AT(m, 0, 0) =  aml_cos(angle_rad);
    AML_AT(m, 0, 1) =  aml_sin(angle_rad);
    AML_AT(m, 1, 0) = -aml_sin(angle_rad);
    AML_AT(m, 1, 1) =  aml_cos(angle_rad);
}

AML_DEF void aml_shift(Aml_Mat2d m, aml_real shift)
{
    AML_ASSERT(m.cols == m.rows); 
    for (size_t i = 0; i < m.rows; i++) {
        AML_AT(m, i, i) += shift;
    }
}

AML_DEF void aml_solve_linear_sys_LUP_decomposition(Aml_Mat2d A, Aml_Mat2d x, Aml_Mat2d B)
{
    AML_ASSERT(A.cols == x.rows);
    AML_ASSERT(1 == x.cols);
    AML_ASSERT(A.rows == B.rows);
    AML_ASSERT(1 == B.cols);

    Aml_Mat2d y     = aml_alloc(x.rows, x.cols);
    Aml_Mat2d l     = aml_alloc(A.rows, A.cols);
    Aml_Mat2d p     = aml_alloc(A.rows, A.cols);
    Aml_Mat2d u     = aml_alloc(A.rows, A.cols);
    Aml_Mat2d inv_l = aml_alloc(l.rows, l.cols);
    Aml_Mat2d inv_u = aml_alloc(u.rows, u.cols);

    aml_LUP_decomposition_with_swap(A, l, p, u);

    aml_invert(inv_l, l);
    aml_invert(inv_u, u);

    aml_fill(x, 0);   /* x here is only a temp mat*/
    aml_fill(y, 0);
    aml_dot(x, p, B);
    aml_dot(y, inv_l, x);

    aml_fill(x, 0);
    aml_dot(x, inv_u, y);

    aml_free(y);
    aml_free(l);
    aml_free(p);
    aml_free(u);
    aml_free(inv_l);
    aml_free(inv_u);
}

AML_DEF void aml_sub(Aml_Mat2d dst, Aml_Mat2d a)
{
    AML_ASSERT(dst.rows == a.rows);
    AML_ASSERT(dst.cols == a.cols);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            AML_AT(dst, i, j) -= AML_AT(a, i, j);
        }
    }
}

AML_DEF void aml_sub_col_to_col(Aml_Mat2d des, size_t des_col, Aml_Mat2d src, size_t src_col)
{
    AML_ASSERT(src_col < src.cols);
    AML_ASSERT(des.rows == src.rows);
    AML_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        AML_AT(des, i, des_col) -= AML_AT(src, i, src_col);
    }
}

AML_DEF void aml_sub_row_to_row(Aml_Mat2d des, size_t des_row, Aml_Mat2d src, size_t src_row)
{
    AML_ASSERT(src_row < src.rows);
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        AML_AT(des, des_row, j) -= AML_AT(src, src_row, j);
    }
}

AML_DEF void aml_sub_row_time_factor_to_row(Aml_Mat2d m, size_t des_r, size_t src_r, aml_real factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        AML_AT(m, des_r, j) -= factor * AML_AT(m, src_r, j);
    }
}

AML_DEF void aml_SVD_full(Aml_Mat2d A, Aml_Mat2d U, Aml_Mat2d S, Aml_Mat2d V, Aml_Mat2d init_vec_u, Aml_Mat2d init_vec_v, bool return_v_transpose)
{
    aml_SVD_thin(A, U, S, V, init_vec_u, init_vec_v, false);

    Aml_Mat2d U_full = aml_alloc(U.rows, U.cols);
    Aml_Mat2d V_full = aml_alloc(V.rows, V.cols);

    aml_make_orthogonal_modified_Gram_Schmidt(U_full, U);
    aml_make_orthogonal_modified_Gram_Schmidt(V_full, V);

    aml_copy(U, U_full);
    if (return_v_transpose) {
        aml_transpose(V, V_full);
    } else {
        aml_copy(V, V_full);
    }

    aml_free(U_full);
    aml_free(V_full);
}

AML_DEF void aml_SVD_thin(Aml_Mat2d A, Aml_Mat2d U, Aml_Mat2d S, Aml_Mat2d V, Aml_Mat2d init_vec_u, Aml_Mat2d init_vec_v, bool return_v_transpose)
{
    /* https://www.youtube.com/watch?v=nbBvuuNVfco */
    /* https://en.wikipedia.org/wiki/Singular_value_decomposition */
    size_t n = A.rows;
    size_t m = A.cols;
    AML_ASSERT(U.rows == n);
    AML_ASSERT(U.cols == n);
    AML_ASSERT(S.rows == n);
    AML_ASSERT(S.cols == m);
    AML_ASSERT(V.rows == m);
    AML_ASSERT(V.cols == m);
    AML_ASSERT(init_vec_u.rows == n);
    AML_ASSERT(init_vec_u.cols == 1);
    AML_ASSERT(init_vec_v.rows == m);
    AML_ASSERT(init_vec_v.cols == 1);

    aml_fill(U, 0);
    aml_fill(S, 0);
    aml_fill(V, 0);

    Aml_Mat2d AT = aml_alloc(m, n);
    aml_transpose(AT, A);

    if (n <= m) {
        Aml_Mat2d AAT = aml_alloc(n, n);
        Aml_Mat2d left_eigenvalues = aml_alloc(n, n);
        Aml_Mat2d left_eigenvectors = aml_alloc(n, n);
        Aml_Mat2d temp_u_vec = aml_alloc(n, 1);
        Aml_Mat2d temp_v_vec = aml_alloc(m, 1);
        aml_dot(AAT, A, AT);
        aml_eig_power_iteration(AAT, left_eigenvalues, left_eigenvectors, init_vec_u, 0);
        /* fill matrix sigma (S) */
        size_t non_zero_n = 0;
        for (size_t i = 0; i < n; i++) {
            if (AML_IS_ZERO(AML_AT(left_eigenvalues, i, i)) || AML_AT(left_eigenvalues, i, i) < 0) {
                AML_AT(S, i, i) = 0; /* AAT is positive definet */
            } else {
                AML_AT(S, i, i) = aml_sqrt(AML_AT(left_eigenvalues, i, i));
                non_zero_n++;
            }
        }
        /** 
         * fill U with the eigenvectors of AAT that have non zero singular value
         * and fill V with the corresponding eigenvector according to:
         *      v_i = A^T*u_i / sigma_i
         */
        for (size_t c = 0; c < non_zero_n; c++) {
            aml_copy_col_from_src_to_des(U, c, left_eigenvectors, c);
            aml_copy_col_from_src_to_des(temp_u_vec, 0, left_eigenvectors, c);
            aml_dot(temp_v_vec, AT, temp_u_vec);
            aml_mult(temp_v_vec, (aml_real)1 / AML_AT(S, c, c));
            aml_copy_col_from_src_to_des(V, c, temp_v_vec, 0);
        }
        aml_free(AAT);
        aml_free(left_eigenvalues);
        aml_free(left_eigenvectors);
        aml_free(temp_u_vec);
        aml_free(temp_v_vec);
    } else {
        Aml_Mat2d ATA = aml_alloc(m, m);
        Aml_Mat2d right_eigenvalues = aml_alloc(m, m);
        Aml_Mat2d right_eigenvectors = aml_alloc(m, m);
        Aml_Mat2d temp_u_vec = aml_alloc(n, 1);
        Aml_Mat2d temp_v_vec = aml_alloc(m, 1);
        aml_dot(ATA, AT, A);
        aml_eig_power_iteration(ATA, right_eigenvalues, right_eigenvectors, init_vec_v, 0);
        /* fill matrix sigma (S) */
        size_t non_zero_m = 0;
        for (size_t i = 0; i < m; i++) {
            if (AML_IS_ZERO(AML_AT(right_eigenvalues, i, i)) || AML_AT(right_eigenvalues, i, i) < 0) {
                AML_AT(S, i, i) = 0; /* ATA is positive definet */
            } else {
                AML_AT(S, i, i) = aml_sqrt(AML_AT(right_eigenvalues, i, i));
                non_zero_m++;
            }
        }
        /** 
         * fill V with the eigenvectors of ATA that have non zero singular value
         * and fill U with the corresponding eigenvector according to:
         *      u_i = A*v_i / sigma_i
         */
        for (size_t c = 0; c < non_zero_m; c++) {
            aml_copy_col_from_src_to_des(V, c, right_eigenvectors, c);
            aml_copy_col_from_src_to_des(temp_v_vec, 0, right_eigenvectors, c);
            aml_dot(temp_u_vec, A, temp_v_vec);
            aml_mult(temp_u_vec, (aml_real)1 / AML_AT(S, c, c));
            aml_copy_col_from_src_to_des(U, c, temp_u_vec, 0);
        }
        aml_free(ATA);
        aml_free(right_eigenvalues);
        aml_free(right_eigenvectors);
        aml_free(temp_u_vec);
        aml_free(temp_v_vec);
    }

    if (return_v_transpose) {
        Aml_Mat2d v_trans = aml_alloc(V.cols, V.rows);
        aml_transpose(v_trans, V);
        aml_copy(V, v_trans);

        aml_free(v_trans);
    }

    aml_free(AT);
}

AML_DEF void aml_swap_rows(Aml_Mat2d m, size_t r1, size_t r2)
{
    for (size_t j = 0; j < m.cols; j++) {
        aml_real temp = AML_AT(m, r1, j);
        AML_AT(m, r1, j) = AML_AT(m, r2, j);
        AML_AT(m, r2, j) = temp;
    }
}

AML_DEF void aml_transpose(Aml_Mat2d des, Aml_Mat2d src)
{
    AML_ASSERT(des.cols == src.rows);
    AML_ASSERT(des.rows == src.cols);

    for (size_t index = 0; index < des.rows; ++index) {
        for (size_t jndex = 0; jndex < des.cols; ++jndex) {
            AML_AT(des, index, jndex) = AML_AT(src, jndex, index);
        }
    }
}

AML_DEF void aml_transpose_inplace(Aml_Mat2d m)
{
    AML_ASSERT(m.cols == m.rows);

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j <= i; j++) {
            aml_real temp = AML_AT(m, i, j);
            AML_AT(m, i, j) = AML_AT(m, j, i);
            AML_AT(m, j, i) = temp;
        }
    }
}

AML_DEF aml_real aml_upper_triangulate(Aml_Mat2d m, uint8_t flags)
{
    /* preforming Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */
    /* returns the factor multiplying the determinant */

    aml_real factor_to_return = 1;

    size_t r = 0;
    for (size_t c = 0; c < m.cols && r < m.rows; c++) {
        if (flags & AML_ROW_SWAPPING) {
            /* finding biggest first number (absolute value); partial pivoting */
            size_t piv = r;
            aml_real best = aml_fabs(AML_AT(m, r, c));
            for (size_t i = r + 1; i < m.rows; i++) {
                aml_real v = aml_fabs(AML_AT(m, i, c));
                if (v > best) {
                    best = v;
                    piv = i;
                }
            }
            if (AML_IS_ZERO(best)) {
                continue; /* move to next column, same pivot row r */
            }
            if (piv != r) {
                aml_swap_rows(m, piv, r);
                factor_to_return *= -(aml_real)1;
            }
        }

        aml_real pivot = AML_AT(m, r, c);
        AML_ASSERT(!AML_IS_ZERO(pivot));

        if (flags & AML_ONES_ON_DIAG) {
            aml_mult_row(m, r, (aml_real)1 / pivot);
            factor_to_return *= pivot;
            pivot = (aml_real)1;
        }

        /* Eliminate entries below pivot in column c */
        for (size_t i = r + 1; i < m.rows; i++) {
            aml_real f = AML_AT(m, i, c) / pivot;
            aml_sub_row_time_factor_to_row(m, i, r, f);
        }
        r++;
    }
    return factor_to_return;
}


#endif // ALMOG_MATRIX_LIBRARY_IMPLEMENTATION