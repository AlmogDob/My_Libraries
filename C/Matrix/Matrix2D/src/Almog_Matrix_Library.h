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
    typedef float mat2D_real;
    #define AML_EPS 1e-5
    #define mat2D_fmin  fminf
    #define mat2D_fmax  fmaxf
    #define mat2D_fabs  fabsf
    #define mat2D_sqrt  sqrtf
    #define mat2D_sin   sinf
    #define mat2D_cos   cosf
    #define mat2D_exp   expf
    #define mat2D_ceil  ceilf
#else 
    typedef double mat2D_real;
    #define AML_EPS 1e-15
    #define mat2D_fmin  fmin
    #define mat2D_fmax  fmax
    #define mat2D_fabs  fabs
    #define mat2D_sqrt  sqrt
    #define mat2D_sin   sin
    #define mat2D_cos   cos
    #define mat2D_exp   exp
    #define mat2D_ceil  ceil
#endif

typedef struct {
    size_t rows;
    size_t cols;
    size_t stride_r; /* elements to traverse to reach the next row */
    mat2D_real *elements;
} Mat2D;

typedef struct {
    size_t rows;
    size_t cols;
    size_t stride_r; /* elements to traverse to reach the next row */
    uint32_t *elements;
} Mat2D_uint32;

typedef struct {
    size_t rows;
    size_t cols;
    size_t stride_r; /* logical stride for the minor shape (not used for access) */
    size_t *rows_list;
    size_t *cols_list;
    Mat2D ref_mat;
} Mat2D_Minor;

#define AML_AT(m, i, j) (m).elements[(AML_ASSERT((i) < (m).rows && (j) < (m).cols), (i) * (m).stride_r + (j))]

#define AML_PI 3.14159265358979323846
#define AML_MAX_POWER_ITERATION 100


#define AML_IS_ZERO(x) (mat2D_fabs(x) < AML_EPS)

#define AML_MINOR_AT(mm, i, j) AML_AT((mm).ref_mat, (mm).rows_list[i], (mm).cols_list[j])

#define AML_PRINT(m) mat2D_print(m, #m, 0)

#define AML_PRINT_UINT32(m) mat2D_print_uint32(m, #m, 0)

#define AML_PRINT_AS_COL(m) mat2D_print_as_col(m, #m, 0)

#define AML_MINOR_PRINT(mm) mat2D_minor_print(mm, #mm, 0)

#define mat2D_min(a, b) ((a) < (b) ? (a) : (b))
#define mat2D_max(a, b) ((a) > (b) ? (a) : (b))

#define mat2D_dprintDOUBLE(expr) printf(#expr " = %#g\n", expr)

#define mat2D_dprintSIZE_T(expr) printf(#expr " = %zu\n", expr)

#define mat2D_dprintINT(expr) printf(#expr " = %d\n", expr)

enum mat2D_upper_triangulate_flag{
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

AML_DEF void          mat2D_add(Mat2D dst, Mat2D a);
AML_DEF void          mat2D_add_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col);
AML_DEF void          mat2D_add_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row);
AML_DEF void          mat2D_add_row_time_factor_to_row(Mat2D m, size_t des_r, size_t src_r, mat2D_real factor);
AML_DEF void          mat2D_add_scalar(Mat2D m, mat2D_real x);
AML_DEF Mat2D         mat2D_alloc(size_t rows, size_t cols);
AML_DEF Mat2D_uint32  mat2D_alloc_uint32(size_t rows, size_t cols);
AML_DEF void          mat2D_anti_diag_transpose_inplace(Mat2D m);
AML_DEF void          mat2D_apply_householder_left(Mat2D A, size_t row0, size_t col0, Mat2D v);
AML_DEF void          mat2D_apply_householder_right(Mat2D A, size_t col0, Mat2D v);
AML_DEF mat2D_real    mat2D_calc_col_norma(Mat2D m, size_t c);
AML_DEF mat2D_real    mat2D_calc_norma(Mat2D m);
AML_DEF mat2D_real    mat2D_calc_norma_inf(Mat2D m);
AML_DEF bool          mat2D_col_is_all_digit(Mat2D m, mat2D_real digit, size_t c);
AML_DEF void          mat2D_convolve(Mat2D m, Mat2D a, Mat2D b);
AML_DEF void          mat2D_copy(Mat2D des, Mat2D src);
AML_DEF void          mat2D_copy_uint32(Mat2D_uint32 des, Mat2D_uint32 src);
AML_DEF void          mat2D_copy_col_from_src_to_des(Mat2D des, size_t des_col, Mat2D src, size_t src_col);
AML_DEF void          mat2D_copy_row_from_src_to_des(Mat2D des, size_t des_row, Mat2D src, size_t src_row);
AML_DEF void          mat2D_copy_src_to_des_window(Mat2D des, Mat2D src, size_t is, size_t js, size_t ie, size_t je);
AML_DEF void          mat2D_copy_src_window_to_des(Mat2D des, Mat2D src, size_t is, size_t js, size_t ie, size_t je);
AML_DEF Mat2D         mat2D_create_col_ref(Mat2D src, size_t c);
AML_DEF void          mat2D_cross(Mat2D dst, Mat2D v1, Mat2D v2);

AML_DEF void          mat2D_dot(Mat2D dst, Mat2D a, Mat2D b);
AML_DEF mat2D_real    mat2D_dot_product(Mat2D v1, Mat2D v2);
AML_DEF mat2D_real    mat2D_det(Mat2D m);
AML_DEF mat2D_real    mat2D_det_2x2_mat(Mat2D m);
AML_DEF mat2D_real    mat2D_det_2x2_mat_minor(Mat2D_Minor mm);

AML_DEF void          mat2D_eig_check(Mat2D A, Mat2D eigenvalues, Mat2D eigenvectors, Mat2D res);
AML_DEF void          mat2D_eig_power_iteration(Mat2D A, Mat2D eigenvalues, Mat2D eigenvectors, Mat2D init_vector, bool norm_inf_vectors);
AML_DEF mat2D_real    mat2D_elements_sum(Mat2D m);

AML_DEF void          mat2D_fill(Mat2D m, mat2D_real x);
AML_DEF void          mat2D_fill_sequence(Mat2D m, mat2D_real start, mat2D_real step);
AML_DEF void          mat2D_fill_uint32(Mat2D_uint32 m, uint32_t x);
AML_DEF bool          mat2D_find_first_non_zero_value(Mat2D m, size_t r, size_t *non_zero_col);
AML_DEF void          mat2D_free(Mat2D m);
AML_DEF void          mat2D_free_uint32(Mat2D_uint32 m);

AML_DEF void          mat2D_householder_matrix_get(Mat2D des, Mat2D v);
AML_DEF void          mat2D_householder_top_element_vector_get(Mat2D v_des, Mat2D x);

AML_DEF mat2D_real    mat2D_inner_product(Mat2D v);
AML_DEF void          mat2D_invert(Mat2D des, Mat2D src);
AML_DEF bool          mat2D_is_symmetric(Mat2D m);

AML_DEF void          mat2D_LUP_decomposition_with_swap(Mat2D src, Mat2D l, Mat2D p, Mat2D u);

AML_DEF void          mat2D_make_orthogonal_Gaussian_elimination(Mat2D des, Mat2D A);
AML_DEF void          mat2D_make_orthogonal_modified_Gram_Schmidt(Mat2D des, Mat2D A);
AML_DEF bool          mat2D_mat_is_all_digit(Mat2D m, mat2D_real digit);
AML_DEF Mat2D_Minor   mat2D_minor_alloc_fill_from_mat(Mat2D ref_mat, size_t i, size_t j);
AML_DEF Mat2D_Minor   mat2D_minor_alloc_fill_from_mat_minor(Mat2D_Minor ref_mm, size_t i, size_t j);
AML_DEF mat2D_real    mat2D_minor_det(Mat2D_Minor mm);
AML_DEF void          mat2D_minor_free(Mat2D_Minor mm);
AML_DEF void          mat2D_minor_print(Mat2D_Minor mm, const char *name, size_t padding);
AML_DEF void          mat2D_mult(Mat2D m, mat2D_real factor);
AML_DEF void          mat2D_mult_row(Mat2D m, size_t r, mat2D_real factor);

AML_DEF void          mat2D_normalize(Mat2D m);
AML_DEF void          mat2D_normalize_inf(Mat2D m);

AML_DEF size_t        mat2D_offset2d(Mat2D m, size_t i, size_t j);
AML_DEF size_t        mat2D_offset2d_uint32(Mat2D_uint32 m, size_t i, size_t j);
AML_DEF void          mat2D_outer_product(Mat2D des, Mat2D v);

AML_DEF int           mat2D_power_iterate(Mat2D A, Mat2D v, mat2D_real *lambda, mat2D_real shift, bool norm_inf_v);
AML_DEF void          mat2D_print(Mat2D m, const char *name, size_t padding);
AML_DEF void          mat2D_print_uint32(Mat2D_uint32 m, const char *name, size_t padding);
AML_DEF void          mat2D_print_as_col(Mat2D m, const char *name, size_t padding);
AML_DEF void          mat2D_project_out_columns(Mat2D v, Mat2D basis, size_t used_cols);

AML_DEF void          mat2D_QR_householder_factorization(Mat2D Q, Mat2D R, Mat2D src);
AML_DEF void          mat2D_QR_householder_factorization_fast(Mat2D Q, Mat2D R, Mat2D src);

AML_DEF void          mat2D_rand(Mat2D m, mat2D_real low, mat2D_real high);
AML_DEF mat2D_real    mat2D_rand_mat2D_real(void);
AML_DEF Mat2D         mat2D_realloc(Mat2D m, size_t rows, size_t cols);
AML_DEF Mat2D_uint32  mat2D_realloc_uint32(Mat2D_uint32 m, size_t rows, size_t cols);
AML_DEF size_t        mat2D_reduce(Mat2D m);
AML_DEF void          mat2D_rotate_mat_180_deg_inplace(Mat2D m);
AML_DEF bool          mat2D_row_is_all_digit(Mat2D m, mat2D_real digit, size_t r);

AML_DEF void          mat2D_set_DCM_zyx(Mat2D DCM, float yaw_deg, float pitch_deg, float roll_deg);
AML_DEF void          mat2D_set_identity(Mat2D m);
AML_DEF void          mat2D_set_rot_mat_x(Mat2D m, float angle_deg);
AML_DEF void          mat2D_set_rot_mat_y(Mat2D m, float angle_deg);
AML_DEF void          mat2D_set_rot_mat_z(Mat2D m, float angle_deg);
AML_DEF void          mat2D_shift(Mat2D m, mat2D_real shift);
AML_DEF void          mat2D_solve_linear_sys_LUP_decomposition(Mat2D A, Mat2D x, Mat2D B);
AML_DEF void          mat2D_sub(Mat2D dst, Mat2D a);
AML_DEF void          mat2D_sub_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col);
AML_DEF void          mat2D_sub_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row);
AML_DEF void          mat2D_sub_row_time_factor_to_row(Mat2D m, size_t des_r, size_t src_r, mat2D_real factor);
AML_DEF void          mat2D_SVD_full(Mat2D A, Mat2D U, Mat2D S, Mat2D V, Mat2D init_vec_u, Mat2D init_vec_v, bool return_v_transpose);
AML_DEF void          mat2D_SVD_thin(Mat2D A, Mat2D U, Mat2D S, Mat2D V, Mat2D init_vec_u, Mat2D init_vec_v, bool return_v_transpose);
AML_DEF void          mat2D_swap_rows(Mat2D m, size_t r1, size_t r2);

AML_DEF void          mat2D_transpose(Mat2D des, Mat2D src);
AML_DEF void          mat2D_transpose_inplace(Mat2D m);

AML_DEF mat2D_real    mat2D_upper_triangulate(Mat2D m, uint8_t flags);

#endif // ALMOG_MATRIX_LIBRARY_H_

#ifdef ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#undef ALMOG_MATRIX_LIBRARY_IMPLEMENTATION


AML_DEF void mat2D_add(Mat2D dst, Mat2D a)
{
    AML_ASSERT(dst.rows == a.rows);
    AML_ASSERT(dst.cols == a.cols);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            AML_AT(dst, i, j) += AML_AT(a, i, j);
        }
    }
}

AML_DEF void mat2D_add_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col)
{
    AML_ASSERT(src_col < src.cols);
    AML_ASSERT(des.rows == src.rows);
    AML_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        AML_AT(des, i, des_col) += AML_AT(src, i, src_col);
    }
}

AML_DEF void mat2D_add_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row)
{
    AML_ASSERT(src_row < src.rows);
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        AML_AT(des, des_row, j) += AML_AT(src, src_row, j);
    }
}

AML_DEF void mat2D_add_row_time_factor_to_row(Mat2D m, size_t des_r, size_t src_r, mat2D_real factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        AML_AT(m, des_r, j) += factor * AML_AT(m, src_r, j);
    }
}

AML_DEF void mat2D_add_scalar(Mat2D m, mat2D_real x)
{
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            AML_AT(m, i, j) += x;
        }
    }
}

AML_DEF Mat2D mat2D_alloc(size_t rows, size_t cols)
{
    Mat2D m;
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (mat2D_real*)AML_MALLOC(sizeof(mat2D_real)*rows*cols);
    AML_ASSERT(m.elements != NULL);
    
    return m;
}

AML_DEF Mat2D_uint32 mat2D_alloc_uint32(size_t rows, size_t cols)
{
    Mat2D_uint32 m;
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (uint32_t*)AML_MALLOC(sizeof(uint32_t)*rows*cols);
    AML_ASSERT(m.elements != NULL);
    
    return m;
}

AML_DEF void mat2D_anti_diag_transpose_inplace(Mat2D m)
{
    AML_ASSERT(m.cols == m.rows);

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols - i; j++) {
            mat2D_real temp = AML_AT(m, i, j);
            AML_AT(m, i, j) = AML_AT(m, m.rows - j - 1, m.cols - i - 1);
            AML_AT(m, m.rows - j - 1, m.cols - i - 1) = temp;
        }
    }
}

AML_DEF void mat2D_apply_householder_left(Mat2D A, size_t row0, size_t col0, Mat2D v)
{
    mat2D_real vv = mat2D_inner_product(v);
    AML_ASSERT(!AML_IS_ZERO(vv));

    mat2D_real beta = (mat2D_real)2 / vv;

    for (size_t j = col0; j < A.cols; ++j) {
        mat2D_real dot = 0;
        for (size_t i = 0; i < v.rows; ++i) {
            dot += AML_AT(v, i, 0) * AML_AT(A, row0 + i, j);
        }
        dot *= beta;

        for (size_t i = 0; i < v.rows; ++i) {
            AML_AT(A, row0 + i, j) -= dot * AML_AT(v, i, 0);
        }
    }
}

AML_DEF void mat2D_apply_householder_right(Mat2D A, size_t col0, Mat2D v)
{
    mat2D_real vv = mat2D_inner_product(v);
    AML_ASSERT(!AML_IS_ZERO(vv));

    mat2D_real beta = (mat2D_real)2 / vv;

    for (size_t i = 0; i < A.rows; ++i) {
        mat2D_real dot = 0;
        for (size_t j = 0; j < v.rows; ++j) {
            dot += AML_AT(A, i, col0 + j) * AML_AT(v, j, 0);
        }
        dot *= beta;

        for (size_t j = 0; j < v.rows; ++j) {
            AML_AT(A, i, col0 + j) -= dot * AML_AT(v, j, 0);
        }
    }
}

AML_DEF mat2D_real mat2D_calc_col_norma(Mat2D m, size_t c)
{
    AML_ASSERT(c < m.cols);

    mat2D_real sum = 0;
    for (size_t i = 0; i < m.rows; ++i) {
        sum += AML_AT(m, i, c) * AML_AT(m, i, c);
    }
    return mat2D_sqrt(sum);
}

AML_DEF mat2D_real mat2D_calc_norma(Mat2D m)
{
    mat2D_real sum = 0;

    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            sum += AML_AT(m, i, j) * AML_AT(m, i, j);
        }
    }
    return mat2D_sqrt(sum);
}
 
AML_DEF mat2D_real mat2D_calc_norma_inf(Mat2D m)
{
    mat2D_real max = 0;
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            mat2D_real current = mat2D_fabs(AML_AT(m, i, j));
            if (current > max) {
                max = current;
            }
        }
    }

    return max;
}

AML_DEF bool mat2D_col_is_all_digit(Mat2D m, mat2D_real digit, size_t c)
{
    for (size_t i = 0; i < m.rows; ++i) {
        if (AML_AT(m, i, c) != digit) {
            return false;
        }
    }
    return true;
}

void mat2D_convolve(Mat2D m, Mat2D a, Mat2D b) 
{
    AML_ASSERT(a.cols >= b.cols);
    AML_ASSERT(a.rows >= b.rows);
    AML_ASSERT(m.rows == (a.rows - b.rows + 1));
    AML_ASSERT(m.cols == (a.cols - b.cols + 1));
    AML_ASSERT(b.cols == b.rows);

    mat2D_rotate_mat_180_deg_inplace(b);

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            Mat2D window = {
                .cols = b.cols,
                .rows = b.rows,
                .stride_r = a.stride_r,
                .elements = &(AML_AT(a, i, j))
            };

            AML_AT(m, i, j) = mat2D_dot_product(window, b);
        }
    }

}

AML_DEF void mat2D_copy(Mat2D des, Mat2D src)
{
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des.rows == src.rows);

    for (size_t i = 0; i < des.rows; ++i) {
        for (size_t j = 0; j < des.cols; ++j) {
            AML_AT(des, i, j) = AML_AT(src, i, j);
        }
    }
}

AML_DEF void mat2D_copy_uint32(Mat2D_uint32 des, Mat2D_uint32 src)
{
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des.rows == src.rows);

    for (size_t i = 0; i < des.rows; ++i) {
        for (size_t j = 0; j < des.cols; ++j) {
            AML_AT(des, i, j) = AML_AT(src, i, j);
        }
    }
}

AML_DEF void mat2D_copy_col_from_src_to_des(Mat2D des, size_t des_col, Mat2D src, size_t src_col)
{
    AML_ASSERT(src_col < src.cols);
    AML_ASSERT(des.rows == src.rows);
    AML_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        AML_AT(des, i, des_col) = AML_AT(src, i, src_col);
    }
}

AML_DEF void mat2D_copy_row_from_src_to_des(Mat2D des, size_t des_row, Mat2D src, size_t src_row)
{
    AML_ASSERT(src_row < src.rows);
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        AML_AT(des, des_row, j) = AML_AT(src, src_row, j);
    }
}

AML_DEF void mat2D_copy_src_to_des_window(Mat2D des, Mat2D src, size_t is, size_t js, size_t ie, size_t je)
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

AML_DEF void mat2D_copy_src_window_to_des(Mat2D des, Mat2D src, size_t is, size_t js, size_t ie, size_t je)
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

AML_DEF Mat2D mat2D_create_col_ref(Mat2D src, size_t c)
{
    AML_ASSERT(c < src.cols);

    Mat2D col = {.cols = 1,
                 .rows = src.rows,
                 .stride_r = src.stride_r,
                 .elements = &(AML_AT(src, 0, c))};

    return col;
}

void mat2D_cross(Mat2D dst, Mat2D v1, Mat2D v2)
{
    AML_ASSERT(3 == dst.rows && 1 == dst.cols);
    AML_ASSERT(3 == v1.rows && 1 == v1.cols);
    AML_ASSERT(3 == v2.rows && 1 == v2.cols);

    AML_AT(dst, 0, 0) = AML_AT(v1, 1, 0) * AML_AT(v2, 2, 0) - AML_AT(v1, 2, 0) * AML_AT(v2, 1, 0);
    AML_AT(dst, 1, 0) = AML_AT(v1, 2, 0) * AML_AT(v2, 0, 0) - AML_AT(v1, 0, 0) * AML_AT(v2, 2, 0);
    AML_AT(dst, 2, 0) = AML_AT(v1, 0, 0) * AML_AT(v2, 1, 0) - AML_AT(v1, 1, 0) * AML_AT(v2, 0, 0);
}

AML_DEF void mat2D_dot(Mat2D dst, Mat2D a, Mat2D b)
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

AML_DEF mat2D_real mat2D_dot_product(Mat2D m1, Mat2D m2)
{
    AML_ASSERT(m1.rows == m2.rows);
    AML_ASSERT(m1.cols == m2.cols);

    mat2D_real dot_product = 0;

    for (size_t i = 0; i < m1.rows; i++) {
        for (size_t j = 0; j < m1.cols; j++) {
            dot_product += AML_AT(m1, i, j) * AML_AT(m2, i, j);
        }
    }
    
    return dot_product;
}

AML_DEF mat2D_real mat2D_det(Mat2D m)
{
    AML_ASSERT(m.cols == m.rows && "should be a square matrix");

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

    Mat2D temp_m = mat2D_alloc(m.rows, m.cols);
    mat2D_copy(temp_m, m);
    mat2D_real factor = mat2D_upper_triangulate(temp_m, AML_ROW_SWAPPING);
    mat2D_real diag_mul = 1; 
    for (size_t i = 0; i < temp_m.rows; i++) {
        diag_mul *= AML_AT(temp_m, i, i);
    }
    mat2D_free(temp_m);

    return diag_mul / factor;
}

AML_DEF mat2D_real mat2D_det_2x2_mat(Mat2D m)
{
    AML_ASSERT(2 == m.cols && 2 == m.rows && "Not a 2x2 matrix");
    return AML_AT(m, 0, 0) * AML_AT(m, 1, 1) - AML_AT(m, 0, 1) * AML_AT(m, 1, 0);
}

AML_DEF mat2D_real mat2D_det_2x2_mat_minor(Mat2D_Minor mm)
{
    AML_ASSERT(2 == mm.cols && 2 == mm.rows && "Not a 2x2 matrix");
    return AML_MINOR_AT(mm, 0, 0) * AML_MINOR_AT(mm, 1, 1) - AML_MINOR_AT(mm, 0, 1) * AML_MINOR_AT(mm, 1, 0);
}

AML_DEF void mat2D_eig_check(Mat2D A, Mat2D eigenvalues, Mat2D eigenvectors, Mat2D res)
{
    AML_ASSERT(A.cols == A.rows); 
    AML_ASSERT(eigenvalues.cols == A.cols);
    AML_ASSERT(eigenvalues.rows == A.rows);
    AML_ASSERT(eigenvectors.cols == A.cols);
    AML_ASSERT(eigenvectors.rows == A.rows);
    AML_ASSERT(res.cols == A.cols);
    AML_ASSERT(res.rows == A.rows);

    mat2D_dot(res, A, eigenvectors);
    Mat2D VL = mat2D_alloc(A.rows, A.cols);
    mat2D_dot(VL, eigenvectors, eigenvalues);

    mat2D_sub(res, VL);

    mat2D_free(VL);
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

    AML_ASSERT(A.cols == A.rows); 
    AML_ASSERT(eigenvalues.cols == A.cols);
    AML_ASSERT(eigenvalues.rows == A.rows);
    AML_ASSERT(eigenvectors.cols == A.cols);
    AML_ASSERT(eigenvectors.rows == A.rows);
    AML_ASSERT(init_vector.cols == 1);
    AML_ASSERT(init_vector.rows == A.rows);
    AML_ASSERT(mat2D_calc_norma_inf(init_vector) > 0);

    mat2D_fill(eigenvalues, 0);
    Mat2D B = mat2D_alloc(A.rows, A.cols);
    Mat2D temp_mat = mat2D_alloc(A.rows, A.cols);
    mat2D_copy(B, A);

    for (int i = 0, shift_value = 0; i < (int)A.rows; i++) {
        mat2D_copy_src_to_des_window(eigenvectors, init_vector, 0, i, init_vector.rows-1, i);
        Mat2D v = {.cols = init_vector.cols,
                   .elements = &AML_AT(eigenvectors, 0, i),
                   .rows = init_vector.rows,
                   .stride_r = eigenvectors.stride_r};
        mat2D_project_out_columns(v, eigenvectors, i);
        if (!AML_IS_ZERO(mat2D_calc_norma(v))) {
            mat2D_normalize(v);
        }
        if (mat2D_power_iterate(B, v, &AML_AT(eigenvalues, i, i), shift_value, 0)) { /* norm_inf_v must be zero*/
            shift_value++;
            i--;
            continue;
        } else {
            shift_value = 0;
        }
        if (!AML_IS_ZERO(mat2D_calc_norma(v))) {
            mat2D_normalize(v);
        }
        mat2D_outer_product(temp_mat, v);
        mat2D_mult(temp_mat, AML_AT(eigenvalues, i, i));
        mat2D_sub(B, temp_mat);
    }

    if (norm_inf_vectors) {
        for (size_t c = 0; c < eigenvectors.cols; c++) {
            Mat2D v = {.cols = init_vector.cols,
                    .elements = &AML_AT(eigenvectors, 0, c),
                    .rows = init_vector.rows,
                    .stride_r = eigenvectors.stride_r};
            mat2D_normalize_inf(v);
        }
    }

    mat2D_free(B);
    mat2D_free(temp_mat);
}

AML_DEF mat2D_real mat2D_elements_sum(Mat2D m)
{
    mat2D_real sum = 0;

    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            sum += AML_AT(m, i, j);
        }
    }
    return sum;
}

AML_DEF void mat2D_fill(Mat2D m, mat2D_real x)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_AT(m, i, j) = x;
        }
    }
}

AML_DEF void mat2D_fill_sequence(Mat2D m, mat2D_real start, mat2D_real step) {
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            AML_AT(m, i, j) = start + step * mat2D_offset2d(m, i, j);
        }
    }
}

AML_DEF void mat2D_fill_uint32(Mat2D_uint32 m, uint32_t x)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_AT(m, i, j) = x;
        }
    }
}

AML_DEF bool mat2D_find_first_non_zero_value(Mat2D m, size_t r, size_t *non_zero_col)
{
    for (size_t c = 0; c < m.cols; ++c) {
        if (!AML_IS_ZERO(AML_AT(m, r, c))) {
            *non_zero_col = c;
            return true;
        }
    }
    return false;
}

AML_DEF void mat2D_free(Mat2D m)
{
    AML_FREE(m.elements);
}

AML_DEF void mat2D_free_uint32(Mat2D_uint32 m)
{
    AML_FREE(m.elements);
}

AML_DEF void mat2D_householder_matrix_get(Mat2D des, Mat2D v)
{
    AML_ASSERT(v.cols == 1);
    AML_ASSERT(des.cols == des.rows);
    AML_ASSERT(des.cols == v.rows);

    Mat2D outer_product = mat2D_alloc(v.rows, v.rows);
    mat2D_outer_product(outer_product, v);
    mat2D_real inner_product = mat2D_inner_product(v);
    mat2D_mult(outer_product, (mat2D_real)2 / inner_product);
    
    mat2D_set_identity(des);

    mat2D_sub(des, outer_product);

    mat2D_free(outer_product);
}

AML_DEF void mat2D_householder_top_element_vector_get(Mat2D v_des, Mat2D x)
{
    AML_ASSERT(x.cols == 1);
    AML_ASSERT(v_des.cols == x.cols);
    AML_ASSERT(v_des.rows == x.rows);

    mat2D_real norm = mat2D_calc_norma(x);

    mat2D_copy(v_des, x);
    
    if (AML_AT(x, 0, 0) > 0) {
        AML_AT(v_des, 0, 0) += norm;
    } else {
        AML_AT(v_des, 0, 0) -= norm;
    }
}

AML_DEF mat2D_real mat2D_inner_product(Mat2D v)
{
    AML_ASSERT((1 == v.cols) || (1 == v.rows));

    mat2D_real dot_product = 0;

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

AML_DEF void mat2D_invert(Mat2D des, Mat2D src)
{
    AML_ASSERT(src.cols == src.rows && "Must be an NxN matrix");
    AML_ASSERT(des.cols == src.cols && des.rows == des.cols);

    Mat2D m = mat2D_alloc(src.rows, src.cols * 2);
    mat2D_copy_src_to_des_window(m, src, 0, 0, src.rows-1, src.cols-1);

    mat2D_set_identity(des);
    mat2D_copy_src_to_des_window(m, des, 0, src.cols, des.rows-1, 2 * des.cols-1);
    
    mat2D_reduce(m);

    mat2D_copy_src_window_to_des(des, m, 0, src.cols, des.rows-1, 2 * des.cols-1);

    mat2D_free(m);
}

AML_DEF bool mat2D_is_symmetric(Mat2D m)
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

AML_DEF void mat2D_LUP_decomposition_with_swap(Mat2D src, Mat2D l, Mat2D p, Mat2D u)
{
    /* performing LU decomposition Following the Wikipedia page: https://en.wikipedia.org/wiki/LU_decomposition */

    mat2D_copy(u, src);
    mat2D_set_identity(p);
    mat2D_fill(l, 0);

    for (size_t i = 0; i < (size_t)mat2D_min(u.rows-1, u.cols); i++) {
        if (AML_IS_ZERO(AML_AT(u, i, i))) {   /* swapping only if it is zero */
            /* finding biggest first number (absolute value) */
            size_t biggest_r = i;
            for (size_t index = i; index < u.rows; index++) {
                if (mat2D_fabs(AML_AT(u, index, i)) > mat2D_fabs(AML_AT(u, biggest_r, i))) {
                    biggest_r = index;
                }
            }
            if (i != biggest_r) {
                mat2D_swap_rows(u, i, biggest_r);
                mat2D_swap_rows(p, i, biggest_r);
                mat2D_swap_rows(l, i, biggest_r);
            }
        }
        for (size_t j = i+1; j < u.rows; j++) {
            mat2D_real factor = 1 / AML_AT(u, i, i);
            if (!isfinite(factor)) {
                printf("%s:%d:\n%s:\n[Error] unable to transfrom into uper triangular matrix. Probably some of the rows are not independent.\n", __FILE__, __LINE__, __func__);
            }
            mat2D_real mat_value = AML_AT(u, j, i);
            mat2D_sub_row_time_factor_to_row(u, j, i, mat_value * factor);
            AML_AT(l, j, i) = mat_value * factor;
        }
        AML_AT(l, i, i) = 1;
    }
    AML_AT(l, l.rows-1, l.cols-1) = 1;
}

AML_DEF void mat2D_make_orthogonal_Gaussian_elimination(Mat2D des, Mat2D A)
{
    /* https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process */
    /** 
     * A^TA must be fully ranked, i.e. 
     * columns must be linearly independent and non zero.
     * 
     */

    AML_ASSERT(des.cols == A.cols);
    AML_ASSERT(des.rows == A.rows);

    Mat2D AT = mat2D_alloc(A.cols, A.rows);
    Mat2D ATA = mat2D_alloc(A.cols, A.cols);
    Mat2D temp = mat2D_alloc(ATA.rows, ATA.cols + A.cols);
    Mat2D temp_des = mat2D_alloc(des.cols, des.rows);

    mat2D_transpose(AT, A);
    mat2D_dot(ATA, AT, A);
    mat2D_copy_src_to_des_window(temp, ATA, 0, 0, ATA.rows-1, ATA.cols-1);
    mat2D_copy_src_to_des_window(temp, AT, 0, ATA.cols, AT.rows-1, ATA.cols + AT.cols-1);

    AML_PRINT(temp);

    mat2D_upper_triangulate(temp, AML_ONES_ON_DIAG);

    mat2D_copy_src_window_to_des(temp_des, temp, 0, ATA.cols, AT.rows-1, ATA.cols + AT.cols-1);

    mat2D_transpose(des, temp_des);

    AML_PRINT(temp);

    mat2D_free(AT);
    mat2D_free(ATA);
    mat2D_free(temp);
    mat2D_free(temp_des);
}

AML_DEF void mat2D_make_orthogonal_modified_Gram_Schmidt(Mat2D des, Mat2D A)
{
    /* https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process */
    AML_ASSERT(des.rows == A.rows);
    AML_ASSERT(des.cols == des.rows);

    size_t num_non_zero_vec = 0;
    for (size_t c = 0; c < A.cols; c++) {
        if (!AML_IS_ZERO(mat2D_calc_col_norma(A, c))) {
            num_non_zero_vec++;
        }
    }

    mat2D_rand(des, 1, 2);

    Mat2D temp_col = mat2D_alloc(des.rows, 1);
    size_t out_c = 0;
    for (size_t c = 0; c < A.cols; c++) {
        if (!AML_IS_ZERO(mat2D_calc_col_norma(A, c))) {
            mat2D_copy_col_from_src_to_des(des, out_c, A, c);
            out_c++;
        }
    }
    for (size_t c = 0; c < des.cols-1; c++) {
        Mat2D vc = mat2D_create_col_ref(des, c);
        mat2D_real vc_vc = mat2D_inner_product(vc);
        for (size_t k = c+1; k < des.cols; k++) {
            mat2D_copy(temp_col, vc);
            Mat2D vk = mat2D_create_col_ref(des, k);
            mat2D_real vk_vc = mat2D_dot_product(vc, vk);
            mat2D_mult(temp_col, vk_vc / vc_vc);
            mat2D_sub(vk, temp_col);
        }
        if (!AML_IS_ZERO(mat2D_calc_norma(vc))) {
            mat2D_normalize(vc);
        }
    }
    Mat2D vc = mat2D_create_col_ref(des, des.cols-1);
    if (!AML_IS_ZERO(mat2D_calc_norma(vc))) {
        mat2D_normalize(vc);
    }


    mat2D_free(temp_col);    
}

AML_DEF bool mat2D_mat_is_all_digit(Mat2D m, mat2D_real digit)
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

AML_DEF Mat2D_Minor mat2D_minor_alloc_fill_from_mat(Mat2D ref_mat, size_t i, size_t j)
{
    AML_ASSERT(ref_mat.cols == ref_mat.rows && "minor is defined only for square matrix");

    Mat2D_Minor mm;
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

AML_DEF Mat2D_Minor mat2D_minor_alloc_fill_from_mat_minor(Mat2D_Minor ref_mm, size_t i, size_t j)
{
    AML_ASSERT(ref_mm.cols == ref_mm.rows && "minor is defined only for square matrix");

    Mat2D_Minor mm;
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

AML_DEF mat2D_real mat2D_minor_det(Mat2D_Minor mm)
{
    AML_ASSERT(mm.cols == mm.rows && "should be a square matrix");

    mat2D_real det = 0;
    /* TODO: finding beast row or col? */
    for (size_t i = 0, j = 0; i < mm.rows; i++) { /* first column */
        if (mat2D_fabs(AML_MINOR_AT(mm, i, j)) < 1e-10) continue;
        Mat2D_Minor sub_mm = mat2D_minor_alloc_fill_from_mat_minor(mm, i, j);
        int factor = (i+j)%2 ? -1 : 1;
        if (sub_mm.cols != 2) {
            AML_ASSERT(sub_mm.cols == sub_mm.rows && "should be a square matrix");
            det += AML_MINOR_AT(mm, i, j) * (factor) * mat2D_minor_det(sub_mm);
        } else if (sub_mm.cols == 2 && sub_mm.rows == 2) {
            det += AML_MINOR_AT(mm, i, j) * (factor) * mat2D_det_2x2_mat_minor(sub_mm);;
        }
        mat2D_minor_free(sub_mm);
    }
    return det;
}

AML_DEF void mat2D_minor_free(Mat2D_Minor mm)
{
    AML_FREE(mm.cols_list);
    AML_FREE(mm.rows_list);
}

AML_DEF void mat2D_minor_print(Mat2D_Minor mm, const char *name, size_t padding)
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

AML_DEF void mat2D_mult(Mat2D m, mat2D_real factor)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_AT(m, i, j) *= factor;
        }
    }
}

AML_DEF void mat2D_mult_row(Mat2D m, size_t r, mat2D_real factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        AML_AT(m, r, j) *= factor;
    }
}

AML_DEF void mat2D_normalize(Mat2D m)
{
    mat2D_real norma = mat2D_calc_norma(m);
    if (AML_IS_ZERO(norma)) {
        return;
    }

    mat2D_mult(m, (mat2D_real)1 / norma);
}

AML_DEF void mat2D_normalize_inf(Mat2D m)
{
    mat2D_real norma = mat2D_calc_norma_inf(m);
    if (AML_IS_ZERO(norma)) {
        return;
    }

    mat2D_mult(m, (mat2D_real)1 / norma);
}

AML_DEF size_t mat2D_offset2d(Mat2D m, size_t i, size_t j)
{
    AML_ASSERT(i < m.rows && j < m.cols);
    return i * m.stride_r + j;
}

AML_DEF size_t mat2D_offset2d_uint32(Mat2D_uint32 m, size_t i, size_t j)
{
    AML_ASSERT(i < m.rows && j < m.cols);
    return i * m.stride_r + j;
}

AML_DEF void mat2D_outer_product(Mat2D des, Mat2D v)
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

AML_DEF int mat2D_power_iterate(Mat2D A, Mat2D v, mat2D_real *lambda, mat2D_real shift, bool norm_inf_v)
{
    /* https://www.youtube.com/watch?v=SkPusgctgpI */

    /** Conditions:
     *      - The eigenvectors must form a basis 
     *      - The largest eigenvalue must be positive and unique
     */ 

    AML_ASSERT(A.cols == A.rows); 
    AML_ASSERT(v.cols == 1);
    AML_ASSERT(v.rows == A.rows);
    AML_ASSERT(mat2D_calc_norma_inf(v) > 0);

    Mat2D current_v = mat2D_alloc(v.rows, v.cols);
    Mat2D temp_v = mat2D_alloc(v.rows, v.cols);
    Mat2D B = mat2D_alloc(A.rows, A.cols);
    mat2D_copy(B, A);
    mat2D_shift(B, shift * -(mat2D_real)1);

    mat2D_real temp_lambda = 0;
    mat2D_real diff = 0;
    mat2D_real diff1 = 0;
    mat2D_real diff2 = 0;

    /* Rayleigh quotient */
    mat2D_dot(temp_v, B, v);
    temp_lambda = mat2D_dot_product(temp_v, v) / (mat2D_calc_norma(v) * mat2D_calc_norma(v));
    int i = 0;
    for (i = 0; i < AML_MAX_POWER_ITERATION; i++) {
        mat2D_copy(current_v, v);
        mat2D_dot(v, B, current_v);
        mat2D_normalize(v);
        mat2D_mult(v, temp_lambda > 0 ? (mat2D_real)1 : -(mat2D_real)11);
        // mat2D_mult(v, fabs(lambda) / lambda);
        mat2D_dot(temp_v, B, v);
        temp_lambda = mat2D_dot_product(temp_v, v);

        mat2D_sub(current_v, v);
        diff1 = mat2D_calc_norma_inf(current_v);
        mat2D_add(current_v, v);
        mat2D_add(current_v, v);
        diff2 = mat2D_calc_norma_inf(current_v);
        diff = mat2D_min(diff1, diff2);
        if (diff < AML_EPS) {
            break;
        }
    }

    mat2D_free(current_v);
    mat2D_free(temp_v);
    mat2D_free(B);

    if (norm_inf_v) mat2D_normalize_inf(v);
    if (lambda) *lambda = temp_lambda + shift;

    if (diff > AML_EPS) {
        return 1; /* eigenvector alternating between two options */
    } else {
        return 0;
    }
}

AML_DEF void mat2D_print(Mat2D m, const char *name, size_t padding)
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

AML_DEF void mat2D_print_uint32(Mat2D_uint32 m, const char *name, size_t padding)
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

AML_DEF void mat2D_print_as_col(Mat2D m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows*m.cols; ++i) {
            printf("%*s    ", (int) padding, "");
            printf("%f\n", m.elements[i]);
    }
    printf("%*s]\n", (int) padding, "");
}

AML_DEF void mat2D_project_out_columns(Mat2D v, Mat2D basis, size_t used_cols)
{
    /* Gram-Schmidt */
    AML_ASSERT(v.cols == 1);
    AML_ASSERT(basis.rows == v.rows);

    Mat2D temp = mat2D_alloc(v.rows, 1);

    for (size_t c = 0; c < used_cols; ++c) {
        Mat2D bc = mat2D_create_col_ref(basis, c);
        mat2D_real alpha = mat2D_dot_product(v, bc);
        mat2D_copy(temp, bc);
        mat2D_mult(temp, alpha);
        mat2D_sub(v, temp);
    }

    mat2D_free(temp);
}

AML_DEF void mat2D_QR_householder_factorization(Mat2D Q, Mat2D R, Mat2D src)
{
    AML_ASSERT(Q.cols == Q.rows);
    AML_ASSERT(R.rows == src.rows);
    AML_ASSERT(R.cols == src.cols);
    AML_ASSERT(Q.cols == R.rows);

    AML_ASSERT(src.rows > 0);
    AML_ASSERT(src.cols > 0);

    Mat2D vector = mat2D_alloc(src.rows, 1);
    Mat2D house  = mat2D_alloc(Q.rows, Q.cols);
    Mat2D prev_Q = mat2D_alloc(Q.rows, Q.cols);
    Mat2D prev_R = mat2D_alloc(R.rows, R.cols);

    mat2D_set_identity(prev_Q);
    mat2D_copy(prev_R, src);
    mat2D_set_identity(Q);
    mat2D_copy(R, src);

    Mat2D col = {.cols = 1};
    Mat2D temp_col = {.cols = 1};
    Mat2D temp_h = {0};
    
    for (size_t i = 0; i < mat2D_min(src.rows-1, src.cols); i++) {
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

        mat2D_fill(vector, 1);
        mat2D_set_identity(house);

        mat2D_householder_top_element_vector_get(temp_col, col);
        mat2D_householder_matrix_get(temp_h, temp_col);

        mat2D_dot(R, house, prev_R);
        mat2D_dot(Q, prev_Q, house);

        mat2D_copy(prev_Q, Q);
        mat2D_copy(prev_R, R);
    }


    mat2D_free(vector);
    mat2D_free(house);
    mat2D_free(prev_Q);
    mat2D_free(prev_R);
}

AML_DEF void mat2D_QR_householder_factorization_fast(Mat2D Q, Mat2D R, Mat2D src)
{
    AML_ASSERT(Q.rows == Q.cols);
    AML_ASSERT(R.rows == src.rows);
    AML_ASSERT(R.cols == src.cols);
    AML_ASSERT(Q.rows == src.rows);

    mat2D_copy(R, src);
    mat2D_set_identity(Q);

    Mat2D vbuf = mat2D_alloc(src.rows, 1);

    size_t steps = mat2D_min(src.rows - 1, src.cols);
    for (size_t k = 0; k < steps; ++k) {
        Mat2D x = {
            .rows = R.rows - k,
            .cols = 1,
            .stride_r = R.stride_r,
            .elements = &AML_AT(R, k, k),
        };

        Mat2D v = {
            .rows = vbuf.rows - k,
            .cols = 1,
            .stride_r = vbuf.stride_r,
            .elements = &AML_AT(vbuf, k, 0),
        };

        mat2D_householder_top_element_vector_get(v, x);

        if (AML_IS_ZERO(mat2D_inner_product(v))) {
            continue;
        }

        mat2D_apply_householder_left(R, k, k, v);
        mat2D_apply_householder_right(Q, k, v);
    }

    mat2D_free(vbuf);
}

AML_DEF void mat2D_rand(Mat2D m, mat2D_real low, mat2D_real high)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_AT(m, i, j) = mat2D_rand_mat2D_real()*(high - low) + low;
        }
    }
}

AML_DEF mat2D_real mat2D_rand_mat2D_real(void)
{
    return (mat2D_real) rand() / (mat2D_real) RAND_MAX;
}

AML_DEF Mat2D mat2D_realloc(Mat2D m, size_t rows, size_t cols)
{
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (mat2D_real*)AML_REALLOC(m.elements, sizeof(mat2D_real)*rows*cols);
    AML_ASSERT(m.elements != NULL);
    
    return m;
}

AML_DEF Mat2D_uint32 mat2D_realloc_uint32(Mat2D_uint32 m, size_t rows, size_t cols)
{
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (uint32_t*)AML_REALLOC(m.elements, sizeof(uint32_t)*rows*cols);
    AML_ASSERT(m.elements != NULL);
    
    return m;
}

AML_DEF size_t mat2D_reduce(Mat2D m)
{
    /* preforming Gauss–Jordan reduction to Reduced Row Echelon Form (RREF) */
    /* Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */

    mat2D_upper_triangulate(m, AML_ONES_ON_DIAG | AML_ROW_SWAPPING);

    size_t rank = 0;

    for (int r = (int)m.rows-1; r >= 0; r--) {
        size_t c = m.cols-1;
        if (!mat2D_find_first_non_zero_value(m, r, &c)) {
            continue; /* row of zeros */
        }
        for (int i = 0; i < r; i++) {
            mat2D_real factor = AML_AT(m, i, c);
            mat2D_sub_row_time_factor_to_row(m, i, r, factor);
        }
        rank++;
    }

    return rank;
}

AML_DEF void mat2D_rotate_mat_180_deg_inplace(Mat2D m)
{
    mat2D_transpose_inplace(m);
    mat2D_anti_diag_transpose_inplace(m);
}

AML_DEF bool mat2D_row_is_all_digit(Mat2D m, mat2D_real digit, size_t r)
{
    for (size_t j = 0; j < m.cols; ++j) {
        if (AML_AT(m, r, j) != digit) {
            return false;
        }
    }
    return true;
}

AML_DEF void mat2D_set_DCM_zyx(Mat2D DCM, float yaw_deg, float pitch_deg, float roll_deg)
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

AML_DEF void mat2D_set_identity(Mat2D m)
{
    AML_ASSERT(m.cols == m.rows);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            AML_AT(m, i, j) = i == j ? (mat2D_real)1 : (mat2D_real)0;
        }
    }
}

AML_DEF void mat2D_set_rot_mat_x(Mat2D m, float angle_deg)
{
    AML_ASSERT(3 == m.cols && 3 == m.rows);

    float angle_rad = angle_deg * (float)AML_PI / 180.0f;
    mat2D_set_identity(m);
    AML_AT(m, 1, 1) =  mat2D_cos(angle_rad);
    AML_AT(m, 1, 2) =  mat2D_sin(angle_rad);
    AML_AT(m, 2, 1) = -mat2D_sin(angle_rad);
    AML_AT(m, 2, 2) =  mat2D_cos(angle_rad);
}

AML_DEF void mat2D_set_rot_mat_y(Mat2D m, float angle_deg)
{
    AML_ASSERT(3 == m.cols && 3 == m.rows);

    float angle_rad = angle_deg * (float)AML_PI / 180.0f;
    mat2D_set_identity(m);
    AML_AT(m, 0, 0) =  mat2D_cos(angle_rad);
    AML_AT(m, 0, 2) = -mat2D_sin(angle_rad);
    AML_AT(m, 2, 0) =  mat2D_sin(angle_rad);
    AML_AT(m, 2, 2) =  mat2D_cos(angle_rad);
}

AML_DEF void mat2D_set_rot_mat_z(Mat2D m, float angle_deg)
{
    AML_ASSERT(3 == m.cols && 3 == m.rows);

    float angle_rad = angle_deg * (float)AML_PI / 180.0f;
    mat2D_set_identity(m);
    AML_AT(m, 0, 0) =  mat2D_cos(angle_rad);
    AML_AT(m, 0, 1) =  mat2D_sin(angle_rad);
    AML_AT(m, 1, 0) = -mat2D_sin(angle_rad);
    AML_AT(m, 1, 1) =  mat2D_cos(angle_rad);
}

AML_DEF void mat2D_shift(Mat2D m, mat2D_real shift)
{
    AML_ASSERT(m.cols == m.rows); 
    for (size_t i = 0; i < m.rows; i++) {
        AML_AT(m, i, i) += shift;
    }
}

AML_DEF void mat2D_solve_linear_sys_LUP_decomposition(Mat2D A, Mat2D x, Mat2D B)
{
    AML_ASSERT(A.cols == x.rows);
    AML_ASSERT(1 == x.cols);
    AML_ASSERT(A.rows == B.rows);
    AML_ASSERT(1 == B.cols);

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

AML_DEF void mat2D_sub(Mat2D dst, Mat2D a)
{
    AML_ASSERT(dst.rows == a.rows);
    AML_ASSERT(dst.cols == a.cols);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            AML_AT(dst, i, j) -= AML_AT(a, i, j);
        }
    }
}

AML_DEF void mat2D_sub_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col)
{
    AML_ASSERT(src_col < src.cols);
    AML_ASSERT(des.rows == src.rows);
    AML_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        AML_AT(des, i, des_col) -= AML_AT(src, i, src_col);
    }
}

AML_DEF void mat2D_sub_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row)
{
    AML_ASSERT(src_row < src.rows);
    AML_ASSERT(des.cols == src.cols);
    AML_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        AML_AT(des, des_row, j) -= AML_AT(src, src_row, j);
    }
}

AML_DEF void mat2D_sub_row_time_factor_to_row(Mat2D m, size_t des_r, size_t src_r, mat2D_real factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        AML_AT(m, des_r, j) -= factor * AML_AT(m, src_r, j);
    }
}

AML_DEF void mat2D_SVD_full(Mat2D A, Mat2D U, Mat2D S, Mat2D V, Mat2D init_vec_u, Mat2D init_vec_v, bool return_v_transpose)
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

AML_DEF void mat2D_SVD_thin(Mat2D A, Mat2D U, Mat2D S, Mat2D V, Mat2D init_vec_u, Mat2D init_vec_v, bool return_v_transpose)
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
            if (AML_IS_ZERO(AML_AT(left_eigenvalues, i, i)) || AML_AT(left_eigenvalues, i, i) < 0) {
                AML_AT(S, i, i) = 0; /* AAT is positive definet */
            } else {
                AML_AT(S, i, i) = mat2D_sqrt(AML_AT(left_eigenvalues, i, i));
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
            mat2D_mult(temp_v_vec, (mat2D_real)1 / AML_AT(S, c, c));
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
            if (AML_IS_ZERO(AML_AT(right_eigenvalues, i, i)) || AML_AT(right_eigenvalues, i, i) < 0) {
                AML_AT(S, i, i) = 0; /* ATA is positive definet */
            } else {
                AML_AT(S, i, i) = mat2D_sqrt(AML_AT(right_eigenvalues, i, i));
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
            mat2D_mult(temp_u_vec, (mat2D_real)1 / AML_AT(S, c, c));
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

AML_DEF void mat2D_swap_rows(Mat2D m, size_t r1, size_t r2)
{
    for (size_t j = 0; j < m.cols; j++) {
        mat2D_real temp = AML_AT(m, r1, j);
        AML_AT(m, r1, j) = AML_AT(m, r2, j);
        AML_AT(m, r2, j) = temp;
    }
}

AML_DEF void mat2D_transpose(Mat2D des, Mat2D src)
{
    AML_ASSERT(des.cols == src.rows);
    AML_ASSERT(des.rows == src.cols);

    for (size_t index = 0; index < des.rows; ++index) {
        for (size_t jndex = 0; jndex < des.cols; ++jndex) {
            AML_AT(des, index, jndex) = AML_AT(src, jndex, index);
        }
    }
}

AML_DEF void mat2D_transpose_inplace(Mat2D m)
{
    AML_ASSERT(m.cols == m.rows);

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j <= i; j++) {
            mat2D_real temp = AML_AT(m, i, j);
            AML_AT(m, i, j) = AML_AT(m, j, i);
            AML_AT(m, j, i) = temp;
        }
    }
}

AML_DEF mat2D_real mat2D_upper_triangulate(Mat2D m, uint8_t flags)
{
    /* preforming Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */
    /* returns the factor multiplying the determinant */

    mat2D_real factor_to_return = 1;

    size_t r = 0;
    for (size_t c = 0; c < m.cols && r < m.rows; c++) {
        if (flags & AML_ROW_SWAPPING) {
            /* finding biggest first number (absolute value); partial pivoting */
            size_t piv = r;
            mat2D_real best = mat2D_fabs(AML_AT(m, r, c));
            for (size_t i = r + 1; i < m.rows; i++) {
                mat2D_real v = mat2D_fabs(AML_AT(m, i, c));
                if (v > best) {
                    best = v;
                    piv = i;
                }
            }
            if (AML_IS_ZERO(best)) {
                continue; /* move to next column, same pivot row r */
            }
            if (piv != r) {
                mat2D_swap_rows(m, piv, r);
                factor_to_return *= -(mat2D_real)1;
            }
        }

        mat2D_real pivot = AML_AT(m, r, c);
        AML_ASSERT(!AML_IS_ZERO(pivot));

        if (flags & AML_ONES_ON_DIAG) {
            mat2D_mult_row(m, r, (mat2D_real)1 / pivot);
            factor_to_return *= pivot;
            pivot = (mat2D_real)1;
        }

        /* Eliminate entries below pivot in column c */
        for (size_t i = r + 1; i < m.rows; i++) {
            mat2D_real f = AML_AT(m, i, c) / pivot;
            mat2D_sub_row_time_factor_to_row(m, i, r, f);
        }
        r++;
    }
    return factor_to_return;
}


#endif // ALMOG_MATRIX_LIBRARY_IMPLEMENTATION