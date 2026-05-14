#ifndef ALMOG_LINEAR_ALGEBRA_H_
#define ALMOG_LINEAR_ALGEBRA_H_

#include "Almog_Matrix_Library.h"

#ifndef ALA_ASSERT
#include <assert.h>
#define ALA_ASSERT AML_ASSERT
#endif //ALA_ASSERT

#ifndef ALA_DEF
    #ifdef ALA_DEF_STATIC
        #define ALA_DEF static
    #else
        #define ALA_DEF extern
    #endif
#endif

#define ALA_SYMMETRIC_EIG_QR_MAX_ITERATIONS 10000
#define ALA_SYMMETRIC_TRIDIAGONAL_EIG_QR_MAX_ITERATIONS 500000

ALA_DEF void        ala_apply_givens_left(struct Aml_Mat2d A, size_t i, size_t js, size_t je, aml_real c, aml_real s);
ALA_DEF void        ala_apply_givens_right(struct Aml_Mat2d A, size_t j, size_t is, size_t ie, aml_real c, aml_real s);
ALA_DEF void        ala_apply_householder_left(struct Aml_Mat2d A, size_t row0, size_t col0, struct Aml_Mat2d v);
ALA_DEF void        ala_apply_householder_right(struct Aml_Mat2d A, size_t col0, struct Aml_Mat2d v);

ALA_DEF void        ala_convolve(struct Aml_Mat2d m, struct Aml_Mat2d a, struct Aml_Mat2d b);

ALA_DEF aml_real    ala_det(struct Aml_Mat2d m);
ALA_DEF aml_real    ala_det_2x2_mat(struct Aml_Mat2d m);

ALA_DEF bool        ala_eig_check(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors, struct Aml_Mat2d res);
ALA_DEF void        ala_eig_power_iteration(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors, struct Aml_Mat2d init_vector, bool norm_inf_vectors);

ALA_DEF aml_real    ala_givens_rotations_get_c_and_s(aml_real a11, aml_real a21, aml_real *c, aml_real *s);

ALA_DEF void        ala_householder_matrix_get(struct Aml_Mat2d des, struct Aml_Mat2d v);
ALA_DEF void        ala_householder_top_element_vector_get(struct Aml_Mat2d v_des, struct Aml_Mat2d x);

ALA_DEF void        ala_invert(struct Aml_Mat2d des, struct Aml_Mat2d src);

ALA_DEF void        ala_LUP_decomposition_with_swap(struct Aml_Mat2d src, struct Aml_Mat2d l, struct Aml_Mat2d p, struct Aml_Mat2d u);

ALA_DEF void        ala_make_orthogonal_Gaussian_elimination(struct Aml_Mat2d des, struct Aml_Mat2d A);
ALA_DEF void        ala_make_orthogonal_modified_Gram_Schmidt(struct Aml_Mat2d des, struct Aml_Mat2d A);

ALA_DEF int         ala_power_iterate(struct Aml_Mat2d A, struct Aml_Mat2d v, aml_real *lambda, aml_real shift, bool norm_inf_v);
ALA_DEF void        ala_project_out_columns(struct Aml_Mat2d v, struct Aml_Mat2d basis, size_t used_cols);

ALA_DEF void        ala_QR_householder_factorization(struct Aml_Mat2d Q, struct Aml_Mat2d R, struct Aml_Mat2d src);
ALA_DEF void        ala_QR_householder_factorization_fast(struct Aml_Mat2d Q, struct Aml_Mat2d R, struct Aml_Mat2d src);

ALA_DEF size_t      ala_reduce(struct Aml_Mat2d m);

ALA_DEF void        ala_solve_linear_sys_LUP_decomposition(struct Aml_Mat2d A, struct Aml_Mat2d x, struct Aml_Mat2d B);
ALA_DEF void        ala_SVD_full(struct Aml_Mat2d A, struct Aml_Mat2d U, struct Aml_Mat2d S, struct Aml_Mat2d V, struct Aml_Mat2d init_vec_u, struct Aml_Mat2d init_vec_v, bool return_v_transpose);
ALA_DEF void        ala_SVD_thin(struct Aml_Mat2d A, struct Aml_Mat2d U, struct Aml_Mat2d S, struct Aml_Mat2d V, struct Aml_Mat2d init_vec_u, struct Aml_Mat2d init_vec_v, bool return_v_transpose);
ALA_DEF void        ala_symmetric_eig_QR_shift(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors);
ALA_DEF void        ala_symmetric_eig_QR_tridiagonalize(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors);
ALA_DEF void        ala_symmetric_tridiagonalize_householder(struct Aml_Mat2d Q, struct Aml_Mat2d T, struct Aml_Mat2d src);
ALA_DEF aml_real    ala_symmetric_tridiagonal_calc_shift(struct Aml_Mat2d m, size_t *last, size_t *first);
ALA_DEF void        ala_symmetric_tridiagonal_eig_QR(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors);
ALA_DEF void        ala_symmetric_tridiagonal_eig_QR_shift(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors);
ALA_DEF void        ala_symmetric_tridiagonal_eig_QR_implicit_shift(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors);
ALA_DEF void        ala_symmetric_tridiagonal_QR(struct Aml_Mat2d Q, struct Aml_Mat2d R, struct Aml_Mat2d src);

ALA_DEF aml_real    ala_upper_triangulate(struct Aml_Mat2d m, uint8_t flags);

#endif // ALMOG_LINEAR_ALGEBRA_H_

#ifdef ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#undef ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION

ALA_DEF void ala_apply_givens_left(struct Aml_Mat2d A, size_t i, size_t js, size_t je, aml_real c, aml_real s)
{
    ALA_ASSERT(i + 1 < A.rows);
    ALA_ASSERT(js < A.cols || A.cols == 0);

    for (size_t j = js; j <= je; ++j) {
        aml_real x = AML_MAT2D_AT(A, i, j);
        aml_real y = AML_MAT2D_AT(A, i + 1, j);

        AML_MAT2D_AT(A, i, j)     = c * x + s * y;
        AML_MAT2D_AT(A, i + 1, j) = -s * x + c * y;
    }
}

ALA_DEF void ala_apply_givens_right(struct Aml_Mat2d A, size_t j, size_t is, size_t ie, aml_real c, aml_real s)
{
    ALA_ASSERT(j + 1 < A.cols);
    ALA_ASSERT(is < A.rows || A.rows == 0);

    for (size_t i = is; i <= ie; ++i) {
        aml_real x = AML_MAT2D_AT(A, i, j);
        aml_real y = AML_MAT2D_AT(A, i, j + 1);

        AML_MAT2D_AT(A, i, j)     = c * x + s * y;
        AML_MAT2D_AT(A, i, j + 1) = -s * x + c * y;
    }
}

ALA_DEF void ala_apply_householder_left(struct Aml_Mat2d A, size_t row0, size_t col0, struct Aml_Mat2d v)
{
    aml_real vv = aml_inner_product(v);
    ALA_ASSERT(!AML_IS_ZERO(vv));

    aml_real beta = (aml_real)2 / vv;

    for (size_t j = col0; j < A.cols; ++j) {
        aml_real dot = 0;
        for (size_t i = 0; i < v.rows; ++i) {
            dot += AML_MAT2D_AT(v, i, 0) * AML_MAT2D_AT(A, row0 + i, j);
        }
        dot *= beta;

        for (size_t i = 0; i < v.rows; ++i) {
            AML_MAT2D_AT(A, row0 + i, j) -= dot * AML_MAT2D_AT(v, i, 0);
        }
    }
}

ALA_DEF void ala_apply_householder_right(struct Aml_Mat2d A, size_t col0, struct Aml_Mat2d v)
{
    aml_real vv = aml_inner_product(v);
    ALA_ASSERT(!AML_IS_ZERO(vv));

    aml_real beta = (aml_real)2 / vv;

    for (size_t i = 0; i < A.rows; ++i) {
        aml_real dot = 0;
        for (size_t j = 0; j < v.rows; ++j) {
            dot += AML_MAT2D_AT(A, i, col0 + j) * AML_MAT2D_AT(v, j, 0);
        }
        dot *= beta;

        for (size_t j = 0; j < v.rows; ++j) {
            AML_MAT2D_AT(A, i, col0 + j) -= dot * AML_MAT2D_AT(v, j, 0);
        }
    }
}

ALA_DEF void ala_convolve(struct Aml_Mat2d m, struct Aml_Mat2d a, struct Aml_Mat2d b) 
{
    ALA_ASSERT(a.cols >= b.cols);
    ALA_ASSERT(a.rows >= b.rows);
    ALA_ASSERT(m.rows == (a.rows - b.rows + 1));
    ALA_ASSERT(m.cols == (a.cols - b.cols + 1));
    ALA_ASSERT(b.cols == b.rows);

    aml_rotate_mat_180_deg_inplace(b);

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            struct Aml_Mat2d window = {
                .cols = b.cols,
                .rows = b.rows,
                .stride_r = a.stride_r,
                .elements = &(AML_MAT2D_AT(a, i, j))
            };
            AML_MAT2D_AT(m, i, j) = aml_dot_product(window, b);
        }
    }
    aml_rotate_mat_180_deg_inplace(b);
}

ALA_DEF aml_real ala_det(struct Aml_Mat2d m)
{
    ALA_ASSERT(m.cols == m.rows && "should be a square matrix");

    /* checking if there is a row or column with all zeros */
    /* checking rows */
    for (size_t i = 0; i < m.rows; i++) {
        if (aml_row_is_all_the_same(m, 0, i)) {
            return 0;
        }
    }
    /* checking cols */
    for (size_t j = 0; j < m.rows; j++) {
        if (aml_col_is_all_the_same(m, 0, j)) {
            return 0;
        }
    }

    struct Aml_Mat2d temp_m = aml_mat2d_alloc(m.rows, m.cols);
    aml_copy(temp_m, m);
    aml_real factor = ala_upper_triangulate(temp_m, AML_ROW_SWAPPING);
    aml_real diag_mul = 1; 
    for (size_t i = 0; i < temp_m.rows; i++) {
        diag_mul *= AML_MAT2D_AT(temp_m, i, i);
    }
    aml_mat2d_free(temp_m);

    return diag_mul / factor;
}

ALA_DEF aml_real ala_det_2x2_mat(struct Aml_Mat2d m)
{
    ALA_ASSERT(2 == m.cols && 2 == m.rows && "Not a 2x2 matrix");
    return AML_MAT2D_AT(m, 0, 0) * AML_MAT2D_AT(m, 1, 1) - AML_MAT2D_AT(m, 0, 1) * AML_MAT2D_AT(m, 1, 0);
}

ALA_DEF bool ala_eig_check(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors, struct Aml_Mat2d res)
{
    ALA_ASSERT(A.cols == A.rows); 
    ALA_ASSERT(eigenvalues.cols == A.cols);
    ALA_ASSERT(eigenvalues.rows == A.rows);
    ALA_ASSERT(eigenvectors.cols == A.cols);
    ALA_ASSERT(eigenvectors.rows == A.rows);
    ALA_ASSERT(res.cols == A.cols);
    ALA_ASSERT(res.rows == A.rows);

    struct Aml_Mat2d VL = aml_mat2d_alloc(A.rows, A.cols);

    aml_real eps = AML_EPS * 1e2;

    /* |res| check */
    aml_dot(res, A, eigenvectors);
    aml_dot(VL, eigenvectors, eigenvalues);
    aml_sub(res, VL);
    aml_real norma = aml_calc_norma(res);
    if (aml_fabs(norma) > eps) {
        aml_dprintWARNING("|AV - VD| is to big = %g", norma);
        aml_mat2d_free(VL);
        return false;
    }

    /* orthogonality check */
    aml_transpose(VL, eigenvectors);
    aml_dot(res, VL, eigenvectors);
    aml_set_identity(VL);
    aml_sub(res, VL);
    norma = aml_calc_norma(res);
    if (aml_fabs(norma) > eps) {
        aml_dprintWARNING("|V^T V - I| is to big = %g", norma);
        aml_mat2d_free(VL);
        return false;
    }

    /* reconstruction check */
    aml_transpose(VL, eigenvectors);
    aml_dot(res, eigenvalues, VL);
    aml_copy(VL, res);
    aml_dot(res, eigenvectors, VL);
    aml_sub(res, A);
    norma = aml_calc_norma(res);
    if (aml_fabs(norma) > eps) {
        aml_dprintWARNING("|V D V^T - A| is to big = %g", norma);
        aml_mat2d_free(VL);
        return false;
    }

    for (size_t i = 0; i < res.rows; i++) {
        for (size_t j = 0; j < res.cols; j++) {
            if (!AML_IS_ZERO(AML_MAT2D_AT(res, i, j))) {
                aml_dprintINFO("(%zu, %zu) = %g", i, j, AML_MAT2D_AT(res, i, j));
                aml_mat2d_free(VL);
                return false;
            }
        }
    }

    aml_mat2d_free(VL);
    return true;
}

ALA_DEF void ala_eig_power_iteration(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors, struct Aml_Mat2d init_vector, bool norm_inf_vectors)
{
    /* https://www.youtube.com/watch?v=c8DIOzuZqBs */

    /** Conditions:
     *      - The eigenvectors must form an orthonormal basis 
     *      - The largest eigenvalue must be positive and unique
     */ 

    ALA_ASSERT(A.cols == A.rows); 
    ALA_ASSERT(eigenvalues.cols == A.cols);
    ALA_ASSERT(eigenvalues.rows == A.rows);
    ALA_ASSERT(eigenvectors.cols == A.cols);
    ALA_ASSERT(eigenvectors.rows == A.rows);
    ALA_ASSERT(init_vector.cols == 1);
    ALA_ASSERT(init_vector.rows == A.rows);
    ALA_ASSERT(aml_calc_norma_inf(init_vector) > 0);

    aml_fill(eigenvalues, 0);
    struct Aml_Mat2d B = aml_mat2d_alloc(A.rows, A.cols);
    struct Aml_Mat2d temp_mat = aml_mat2d_alloc(A.rows, A.cols);
    aml_copy(B, A);

    for (int i = 0, shift_value = 0; i < (int)A.rows; i++) {
        aml_copy_src_to_des_window(eigenvectors, init_vector, 0, i, init_vector.rows-1, i);
        struct Aml_Mat2d v = {.cols = init_vector.cols,
                   .elements = &AML_MAT2D_AT(eigenvectors, 0, i),
                   .rows = init_vector.rows,
                   .stride_r = eigenvectors.stride_r};
        ala_project_out_columns(v, eigenvectors, i);
        if (!AML_IS_ZERO(aml_calc_norma(v))) {
            aml_normalize(v);
        }
        if (ala_power_iterate(B, v, &AML_MAT2D_AT(eigenvalues, i, i), shift_value, 0)) { /* norm_inf_v must be zero*/
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
        aml_mult(temp_mat, AML_MAT2D_AT(eigenvalues, i, i));
        aml_sub(B, temp_mat);
    }

    if (norm_inf_vectors) {
        for (size_t c = 0; c < eigenvectors.cols; c++) {
            struct Aml_Mat2d v = {.cols = init_vector.cols,
                    .elements = &AML_MAT2D_AT(eigenvectors, 0, c),
                    .rows = init_vector.rows,
                    .stride_r = eigenvectors.stride_r};
            aml_normalize_inf(v);
        }
    }

    aml_mat2d_free(B);
    aml_mat2d_free(temp_mat);
}

ALA_DEF aml_real ala_givens_rotations_get_c_and_s(aml_real a11, aml_real a21, aml_real *c, aml_real *s)
{
    if (AML_IS_ZERO(a21)) {
        if (c) *c = (aml_real)1;
        if (s) *s = (aml_real)0;
        return a11;
    }

    aml_real r = aml_hypot(a11, a21);
    if (c) *c = a11 / r;
    if (s) *s = a21 / r;
    return r;
}

ALA_DEF void ala_householder_matrix_get(struct Aml_Mat2d des, struct Aml_Mat2d v)
{
    ALA_ASSERT(v.cols == 1);
    ALA_ASSERT(des.cols == des.rows);
    ALA_ASSERT(des.cols == v.rows);

    aml_outer_product(des, v);
    aml_real inner_product = aml_inner_product(v);
    aml_mult(des, -(aml_real)2 / inner_product);
    
    aml_shift(des, 1);
}

ALA_DEF void ala_householder_top_element_vector_get(struct Aml_Mat2d v_des, struct Aml_Mat2d x)
{
    ALA_ASSERT(x.cols == 1);
    ALA_ASSERT(v_des.cols == x.cols);
    ALA_ASSERT(v_des.rows == x.rows);

    aml_real norm = aml_calc_norma(x);

    aml_copy(v_des, x);
    
    if (AML_MAT2D_AT(x, 0, 0) > 0) {
        AML_MAT2D_AT(v_des, 0, 0) += norm;
    } else {
        AML_MAT2D_AT(v_des, 0, 0) -= norm;
    }
}

ALA_DEF void ala_invert(struct Aml_Mat2d des, struct Aml_Mat2d src)
{
    ALA_ASSERT(src.cols == src.rows && "Must be an NxN matrix");
    ALA_ASSERT(des.cols == src.cols && des.rows == des.cols);

    struct Aml_Mat2d m = aml_mat2d_alloc(src.rows, src.cols * 2);
    aml_copy_src_to_des_window(m, src, 0, 0, src.rows-1, src.cols-1);

    aml_set_identity(des);
    aml_copy_src_to_des_window(m, des, 0, src.cols, des.rows-1, 2 * des.cols-1);
    
    ala_reduce(m);

    aml_copy_src_window_to_des(des, m, 0, src.cols, des.rows-1, 2 * des.cols-1);

    aml_mat2d_free(m);
}

ALA_DEF void ala_LUP_decomposition_with_swap(struct Aml_Mat2d src, struct Aml_Mat2d l, struct Aml_Mat2d p, struct Aml_Mat2d u)
{
    /* performing LU decomposition Following the Wikipedia page: https://en.wikipedia.org/wiki/LU_decomposition */

    aml_copy(u, src);
    aml_set_identity(p);
    aml_fill(l, 0);

    for (size_t i = 0; i < (size_t)aml_min(u.rows-1, u.cols); i++) {
        if (AML_IS_ZERO(AML_MAT2D_AT(u, i, i))) {   /* swapping only if it is zero */
            /* finding biggest first number (absolute value) */
            size_t biggest_r = i;
            for (size_t index = i; index < u.rows; index++) {
                if (aml_fabs(AML_MAT2D_AT(u, index, i)) > aml_fabs(AML_MAT2D_AT(u, biggest_r, i))) {
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
            aml_real factor = 1 / AML_MAT2D_AT(u, i, i);
            if (!isfinite(factor)) {
                printf("%s:%d:\n%s:\n[Error] unable to transfrom into uper triangular matrix. Probably some of the rows are not independent.\n", __FILE__, __LINE__, __func__);
            }
            aml_real mat_value = AML_MAT2D_AT(u, j, i);
            aml_sub_row_time_factor_to_row(u, j, i, mat_value * factor);
            AML_MAT2D_AT(l, j, i) = mat_value * factor;
        }
        AML_MAT2D_AT(l, i, i) = 1;
    }
    AML_MAT2D_AT(l, l.rows-1, l.cols-1) = 1;
}

ALA_DEF void ala_make_orthogonal_Gaussian_elimination(struct Aml_Mat2d des, struct Aml_Mat2d A)
{
    /* https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process */
    /** 
     * A^TA must be fully ranked, i.e. 
     * columns must be linearly independent and non zero.
     * 
     */

    ALA_ASSERT(des.cols == A.cols);
    ALA_ASSERT(des.rows == A.rows);

    struct Aml_Mat2d AT = aml_mat2d_alloc(A.cols, A.rows);
    struct Aml_Mat2d ATA = aml_mat2d_alloc(A.cols, A.cols);
    struct Aml_Mat2d temp = aml_mat2d_alloc(ATA.rows, ATA.cols + A.cols);
    struct Aml_Mat2d temp_des = aml_mat2d_alloc(des.cols, des.rows);

    aml_transpose(AT, A);
    aml_dot(ATA, AT, A);
    aml_copy_src_to_des_window(temp, ATA, 0, 0, ATA.rows-1, ATA.cols-1);
    aml_copy_src_to_des_window(temp, AT, 0, ATA.cols, AT.rows-1, ATA.cols + AT.cols-1);

    AML_PRINT(temp);

    ala_upper_triangulate(temp, AML_ONES_ON_DIAG);

    aml_copy_src_window_to_des(temp_des, temp, 0, ATA.cols, AT.rows-1, ATA.cols + AT.cols-1);

    aml_transpose(des, temp_des);

    AML_PRINT(temp);

    aml_mat2d_free(AT);
    aml_mat2d_free(ATA);
    aml_mat2d_free(temp);
    aml_mat2d_free(temp_des);
}

ALA_DEF void ala_make_orthogonal_modified_Gram_Schmidt(struct Aml_Mat2d des, struct Aml_Mat2d A)
{
    /* https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process */
    ALA_ASSERT(des.rows == A.rows);
    ALA_ASSERT(des.cols == des.rows);

    size_t num_non_zero_vec = 0;
    for (size_t c = 0; c < A.cols; c++) {
        if (!AML_IS_ZERO(aml_calc_col_norma(A, c))) {
            num_non_zero_vec++;
        }
    }

    aml_set_rand(des, 1, 2);

    struct Aml_Mat2d temp_col = aml_mat2d_alloc(des.rows, 1);
    size_t out_c = 0;
    for (size_t c = 0; c < A.cols; c++) {
        if (!AML_IS_ZERO(aml_calc_col_norma(A, c))) {
            aml_copy_col_from_src_to_des(des, out_c, A, c);
            out_c++;
        }
    }
    for (size_t c = 0; c < des.cols-1; c++) {
        struct Aml_Mat2d vc = aml_create_col_ref(des, c);
        aml_real vc_vc = aml_inner_product(vc);
        for (size_t k = c+1; k < des.cols; k++) {
            aml_copy(temp_col, vc);
            struct Aml_Mat2d vk = aml_create_col_ref(des, k);
            aml_real vk_vc = aml_dot_product(vc, vk);
            aml_mult(temp_col, vk_vc / vc_vc);
            aml_sub(vk, temp_col);
        }
        if (!AML_IS_ZERO(aml_calc_norma(vc))) {
            aml_normalize(vc);
        }
    }
    struct Aml_Mat2d vc = aml_create_col_ref(des, des.cols-1);
    if (!AML_IS_ZERO(aml_calc_norma(vc))) {
        aml_normalize(vc);
    }


    aml_mat2d_free(temp_col);    
}

ALA_DEF int ala_power_iterate(struct Aml_Mat2d A, struct Aml_Mat2d v, aml_real *lambda, aml_real shift, bool norm_inf_v)
{
    /* https://www.youtube.com/watch?v=SkPusgctgpI */

    /** Conditions:
     *      - The eigenvectors must form a basis 
     *      - The largest eigenvalue must be positive and unique
     */ 

    ALA_ASSERT(A.cols == A.rows); 
    ALA_ASSERT(v.cols == 1);
    ALA_ASSERT(v.rows == A.rows);
    ALA_ASSERT(aml_calc_norma_inf(v) > 0);

    struct Aml_Mat2d current_v = aml_mat2d_alloc(v.rows, v.cols);
    struct Aml_Mat2d temp_v = aml_mat2d_alloc(v.rows, v.cols);
    struct Aml_Mat2d B = aml_mat2d_alloc(A.rows, A.cols);
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
        aml_mult(v, temp_lambda > 0 ? (aml_real)1 : -(aml_real)1);
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

    aml_mat2d_free(current_v);
    aml_mat2d_free(temp_v);
    aml_mat2d_free(B);

    if (norm_inf_v) aml_normalize_inf(v);
    if (lambda) *lambda = temp_lambda + shift;

    if (diff > AML_EPS) {
        return 1; /* eigenvector alternating between two options */
    } else {
        return 0;
    }
}

ALA_DEF void ala_project_out_columns(struct Aml_Mat2d v, struct Aml_Mat2d basis, size_t used_cols)
{
    /* Gram-Schmidt */
    ALA_ASSERT(v.cols == 1);
    ALA_ASSERT(basis.rows == v.rows);

    struct Aml_Mat2d temp = aml_mat2d_alloc(v.rows, 1);

    for (size_t c = 0; c < used_cols; ++c) {
        struct Aml_Mat2d bc = aml_create_col_ref(basis, c);
        aml_real alpha = aml_dot_product(v, bc);
        aml_copy(temp, bc);
        aml_mult(temp, alpha);
        aml_sub(v, temp);
    }

    aml_mat2d_free(temp);
}

ALA_DEF void ala_QR_householder_factorization(struct Aml_Mat2d Q, struct Aml_Mat2d R, struct Aml_Mat2d src)
{
    /** 
     * https://youtu.be/5MeeuSoFBdY?list=PLfNiIduhuYeAhoLm2NboJmzpHNaqgnx0n
     */
    ALA_ASSERT(Q.cols == Q.rows);
    ALA_ASSERT(R.rows == src.rows);
    ALA_ASSERT(R.cols == src.cols);
    ALA_ASSERT(Q.cols == R.rows);

    ALA_ASSERT(src.rows > 0);
    ALA_ASSERT(src.cols > 0);

    struct Aml_Mat2d vector = aml_mat2d_alloc(src.rows, 1);
    struct Aml_Mat2d house  = aml_mat2d_alloc(Q.rows, Q.cols);
    struct Aml_Mat2d prev_Q = aml_mat2d_alloc(Q.rows, Q.cols);
    struct Aml_Mat2d prev_R = aml_mat2d_alloc(R.rows, R.cols);

    aml_set_identity(prev_Q);
    aml_copy(prev_R, src);
    aml_set_identity(Q);
    aml_copy(R, src);

    struct Aml_Mat2d col = {.cols = 1};
    struct Aml_Mat2d temp_col = {.cols = 1};
    struct Aml_Mat2d temp_h = {0};
    
    for (size_t i = 0; i < aml_min(src.rows-1, src.cols); i++) {
        col.rows = prev_R.rows - i;
        col.elements = &AML_MAT2D_AT(prev_R, i, i);
        col.stride_r = prev_R.stride_r;

        temp_col.rows = vector.rows - i;
        temp_col.elements = &AML_MAT2D_AT(vector, i, 0);
        temp_col.stride_r = vector.stride_r;
        
        temp_h.rows = house.rows - i;
        temp_h.cols = house.cols - i;
        temp_h.elements = &AML_MAT2D_AT(house, i, i);
        temp_h.stride_r= house.stride_r;

        aml_fill(vector, 1);
        aml_set_identity(house);

        ala_householder_top_element_vector_get(temp_col, col);
        if (AML_IS_ZERO(aml_inner_product(col))) {
            continue;
        }
        ala_householder_matrix_get(temp_h, temp_col);

        aml_dot(R, house, prev_R);
        aml_dot(Q, prev_Q, house);

        aml_copy(prev_Q, Q);
        aml_copy(prev_R, R);
    }


    aml_mat2d_free(vector);
    aml_mat2d_free(house);
    aml_mat2d_free(prev_Q);
    aml_mat2d_free(prev_R);
}

ALA_DEF void ala_QR_householder_factorization_fast(struct Aml_Mat2d Q, struct Aml_Mat2d R, struct Aml_Mat2d src)
{
    /** 
     * https://youtu.be/5MeeuSoFBdY?list=PLfNiIduhuYeAhoLm2NboJmzpHNaqgnx0n
     */
    ALA_ASSERT(Q.rows == Q.cols);
    ALA_ASSERT(R.rows == src.rows);
    ALA_ASSERT(R.cols == src.cols);
    ALA_ASSERT(Q.rows == src.rows);

    aml_copy(R, src);
    aml_set_identity(Q);

    struct Aml_Mat2d vbuf = aml_mat2d_alloc(src.rows, 1);

    size_t steps = aml_min(src.rows - 1, src.cols);
    for (size_t k = 0; k < steps; ++k) {
        struct Aml_Mat2d x = {
            .rows = R.rows - k,
            .cols = 1,
            .stride_r = R.stride_r,
            .elements = &AML_MAT2D_AT(R, k, k),
        };

        struct Aml_Mat2d v = {
            .rows = vbuf.rows - k,
            .cols = 1,
            .stride_r = vbuf.stride_r,
            .elements = &AML_MAT2D_AT(vbuf, k, 0),
        };

        ala_householder_top_element_vector_get(v, x);

        if (AML_IS_ZERO(aml_inner_product(v))) {
            continue;
        }

        ala_apply_householder_left(R, k, k, v);
        ala_apply_householder_right(Q, k, v);
    }

    aml_mat2d_free(vbuf);
}

ALA_DEF size_t ala_reduce(struct Aml_Mat2d m)
{
    /* preforming Gauss–Jordan reduction to Reduced Row Echelon Form (RREF) */
    /* Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */

    ala_upper_triangulate(m, AML_ONES_ON_DIAG | AML_ROW_SWAPPING);

    size_t rank = 0;

    for (int r = (int)m.rows-1; r >= 0; r--) {
        size_t c = m.cols-1;
        if (!aml_row_find_first_non_zero_value(m, r, &c)) {
            continue; /* row of zeros */
        }
        for (int i = 0; i < r; i++) {
            aml_real factor = AML_MAT2D_AT(m, i, c);
            aml_sub_row_time_factor_to_row(m, i, r, factor);
        }
        rank++;
    }

    return rank;
}

ALA_DEF void ala_solve_linear_sys_LUP_decomposition(struct Aml_Mat2d A, struct Aml_Mat2d x, struct Aml_Mat2d B)
{
    ALA_ASSERT(A.cols == x.rows);
    ALA_ASSERT(1 == x.cols);
    ALA_ASSERT(A.rows == B.rows);
    ALA_ASSERT(1 == B.cols);

    struct Aml_Mat2d y     = aml_mat2d_alloc(x.rows, x.cols);
    struct Aml_Mat2d l     = aml_mat2d_alloc(A.rows, A.cols);
    struct Aml_Mat2d p     = aml_mat2d_alloc(A.rows, A.cols);
    struct Aml_Mat2d u     = aml_mat2d_alloc(A.rows, A.cols);
    struct Aml_Mat2d inv_l = aml_mat2d_alloc(l.rows, l.cols);
    struct Aml_Mat2d inv_u = aml_mat2d_alloc(u.rows, u.cols);

    ala_LUP_decomposition_with_swap(A, l, p, u);

    ala_invert(inv_l, l);
    ala_invert(inv_u, u);

    aml_fill(x, 0);   /* x here is only a temp mat*/
    aml_fill(y, 0);
    aml_dot(x, p, B);
    aml_dot(y, inv_l, x);

    aml_fill(x, 0);
    aml_dot(x, inv_u, y);

    aml_mat2d_free(y);
    aml_mat2d_free(l);
    aml_mat2d_free(p);
    aml_mat2d_free(u);
    aml_mat2d_free(inv_l);
    aml_mat2d_free(inv_u);
}

ALA_DEF void ala_SVD_full(struct Aml_Mat2d A, struct Aml_Mat2d U, struct Aml_Mat2d S, struct Aml_Mat2d V, struct Aml_Mat2d init_vec_u, struct Aml_Mat2d init_vec_v, bool return_v_transpose)
{
    ala_SVD_thin(A, U, S, V, init_vec_u, init_vec_v, false);

    struct Aml_Mat2d U_full = aml_mat2d_alloc(U.rows, U.cols);
    struct Aml_Mat2d V_full = aml_mat2d_alloc(V.rows, V.cols);

    ala_make_orthogonal_modified_Gram_Schmidt(U_full, U);
    ala_make_orthogonal_modified_Gram_Schmidt(V_full, V);

    aml_copy(U, U_full);
    if (return_v_transpose) {
        aml_transpose(V, V_full);
    } else {
        aml_copy(V, V_full);
    }

    aml_mat2d_free(U_full);
    aml_mat2d_free(V_full);
}

ALA_DEF void ala_SVD_thin(struct Aml_Mat2d A, struct Aml_Mat2d U, struct Aml_Mat2d S, struct Aml_Mat2d V, struct Aml_Mat2d init_vec_u, struct Aml_Mat2d init_vec_v, bool return_v_transpose)
{
    /* https://www.youtube.com/watch?v=nbBvuuNVfco */
    /* https://en.wikipedia.org/wiki/Singular_value_decomposition */
    size_t n = A.rows;
    size_t m = A.cols;
    ALA_ASSERT(U.rows == n);
    ALA_ASSERT(U.cols == n);
    ALA_ASSERT(S.rows == n);
    ALA_ASSERT(S.cols == m);
    ALA_ASSERT(V.rows == m);
    ALA_ASSERT(V.cols == m);
    ALA_ASSERT(init_vec_u.rows == n);
    ALA_ASSERT(init_vec_u.cols == 1);
    ALA_ASSERT(init_vec_v.rows == m);
    ALA_ASSERT(init_vec_v.cols == 1);

    aml_fill(U, 0);
    aml_fill(S, 0);
    aml_fill(V, 0);

    struct Aml_Mat2d AT = aml_mat2d_alloc(m, n);
    aml_transpose(AT, A);

    if (n <= m) {
        struct Aml_Mat2d AAT = aml_mat2d_alloc(n, n);
        struct Aml_Mat2d left_eigenvalues = aml_mat2d_alloc(n, n);
        struct Aml_Mat2d left_eigenvectors = aml_mat2d_alloc(n, n);
        struct Aml_Mat2d temp_u_vec = aml_mat2d_alloc(n, 1);
        struct Aml_Mat2d temp_v_vec = aml_mat2d_alloc(m, 1);
        aml_dot(AAT, A, AT);
        ala_eig_power_iteration(AAT, left_eigenvalues, left_eigenvectors, init_vec_u, 0);
        /* fill matrix sigma (S) */
        size_t non_zero_n = 0;
        for (size_t i = 0; i < n; i++) {
            if (AML_IS_ZERO(AML_MAT2D_AT(left_eigenvalues, i, i)) || AML_MAT2D_AT(left_eigenvalues, i, i) < 0) {
                AML_MAT2D_AT(S, i, i) = 0; /* AAT is positive definet */
            } else {
                AML_MAT2D_AT(S, i, i) = aml_sqrt(AML_MAT2D_AT(left_eigenvalues, i, i));
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
            aml_mult(temp_v_vec, (aml_real)1 / AML_MAT2D_AT(S, c, c));
            aml_copy_col_from_src_to_des(V, c, temp_v_vec, 0);
        }
        aml_mat2d_free(AAT);
        aml_mat2d_free(left_eigenvalues);
        aml_mat2d_free(left_eigenvectors);
        aml_mat2d_free(temp_u_vec);
        aml_mat2d_free(temp_v_vec);
    } else {
        struct Aml_Mat2d ATA = aml_mat2d_alloc(m, m);
        struct Aml_Mat2d right_eigenvalues = aml_mat2d_alloc(m, m);
        struct Aml_Mat2d right_eigenvectors = aml_mat2d_alloc(m, m);
        struct Aml_Mat2d temp_u_vec = aml_mat2d_alloc(n, 1);
        struct Aml_Mat2d temp_v_vec = aml_mat2d_alloc(m, 1);
        aml_dot(ATA, AT, A);
        ala_eig_power_iteration(ATA, right_eigenvalues, right_eigenvectors, init_vec_v, 0);
        /* fill matrix sigma (S) */
        size_t non_zero_m = 0;
        for (size_t i = 0; i < m; i++) {
            if (AML_IS_ZERO(AML_MAT2D_AT(right_eigenvalues, i, i)) || AML_MAT2D_AT(right_eigenvalues, i, i) < 0) {
                AML_MAT2D_AT(S, i, i) = 0; /* ATA is positive definet */
            } else {
                AML_MAT2D_AT(S, i, i) = aml_sqrt(AML_MAT2D_AT(right_eigenvalues, i, i));
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
            aml_mult(temp_u_vec, (aml_real)1 / AML_MAT2D_AT(S, c, c));
            aml_copy_col_from_src_to_des(U, c, temp_u_vec, 0);
        }
        aml_mat2d_free(ATA);
        aml_mat2d_free(right_eigenvalues);
        aml_mat2d_free(right_eigenvectors);
        aml_mat2d_free(temp_u_vec);
        aml_mat2d_free(temp_v_vec);
    }

    if (return_v_transpose) {
        struct Aml_Mat2d v_trans = aml_mat2d_alloc(V.cols, V.rows);
        aml_transpose(v_trans, V);
        aml_copy(V, v_trans);

        aml_mat2d_free(v_trans);
    }

    aml_mat2d_free(AT);
}

ALA_DEF void ala_symmetric_eig_QR_shift(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors)
{
    /**
     * https://youtu.be/HIxSCrFX1Ls?list=PLfNiIduhuYeBfgM2kuZCofXxWHx_bkAr1
     */
    ALA_ASSERT(aml_is_symmetric(A));
    ALA_ASSERT(A.cols == A.rows); 
    ALA_ASSERT(eigenvalues.cols == A.cols);
    ALA_ASSERT(eigenvalues.rows == A.rows);
    ALA_ASSERT(eigenvectors.cols == A.cols);
    ALA_ASSERT(eigenvectors.rows == A.rows);

    struct Aml_Mat2d Q = aml_mat2d_alloc(A.rows, A.cols);
    struct Aml_Mat2d R = aml_mat2d_alloc(A.rows, A.cols);
    
    aml_copy(eigenvalues, A);
    aml_set_identity(eigenvectors);

    bool converged = true;
    for (size_t i = 0, n = A.rows; i < ALA_SYMMETRIC_EIG_QR_MAX_ITERATIONS; i++) {
        converged = false;
        aml_real mu = AML_MAT2D_AT(eigenvalues, n - 1, n - 1);

        aml_shift(eigenvalues, -mu);
        ala_QR_householder_factorization_fast(Q, R, eigenvalues);
        aml_dot(eigenvalues, R, Q);
        aml_shift(eigenvalues, mu);
        
        aml_copy(R, eigenvectors);
        aml_dot(eigenvectors, R, Q);

        if (!(i % 1)) {
            converged = true;
            for (size_t conv_index = 1; conv_index < eigenvalues.rows; conv_index++) {
                if (!AML_IS_ZERO(AML_MAT2D_AT(eigenvalues, conv_index, conv_index - 1))) {
                    converged = false;
                    break;
                }
            }
        }

        if (converged) {
            aml_dprintINFO("converged on iteration %zu", i);
            break;
        }
    }

    if (!converged) {
        aml_dprintWARNING("Did not converged after %d iterations.", ALA_SYMMETRIC_EIG_QR_MAX_ITERATIONS);
    }

    aml_mat2d_free(Q);
    aml_mat2d_free(R);
}

ALA_DEF void ala_symmetric_eig_QR_tridiagonalize(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors)
{
    ALA_ASSERT(aml_is_symmetric(A));
    ALA_ASSERT(A.cols == A.rows); 
    ALA_ASSERT(eigenvalues.cols == A.cols);
    ALA_ASSERT(eigenvalues.rows == A.rows);
    ALA_ASSERT(eigenvectors.cols == A.cols);
    ALA_ASSERT(eigenvectors.rows == A.rows);

    struct Aml_Mat2d Q = aml_mat2d_alloc(A.rows, A.cols);
    struct Aml_Mat2d T = aml_mat2d_alloc(A.rows, A.cols);
    struct Aml_Mat2d semi_eigenvectors = aml_mat2d_alloc(A.rows, A.cols);

    ala_symmetric_tridiagonalize_householder(Q, T, A);
    aml_dprintINFO("%s", "made tridiagonal");

    // ala_symmetric_tridiagonal_eig_QR(T, eigenvalues, semi_eigenvectors);
    ala_symmetric_tridiagonal_eig_QR_shift(T, eigenvalues, semi_eigenvectors);
    // ala_symmetric_tridiagonal_eig_QR_implicit_shift(T, eigenvalues, semi_eigenvectors);

    aml_dot(eigenvectors, Q, semi_eigenvectors);

    aml_mat2d_free(Q);
    aml_mat2d_free(T);
    aml_mat2d_free(semi_eigenvectors);
}

ALA_DEF void ala_symmetric_tridiagonalize_householder(struct Aml_Mat2d Q, struct Aml_Mat2d T, struct Aml_Mat2d src)
{
    /** 
     * https://youtu.be/ETQbxnweKok
     */
    ALA_ASSERT(aml_is_symmetric(src));
    ALA_ASSERT(src.rows == src.cols);
    ALA_ASSERT(Q.rows == Q.cols);
    ALA_ASSERT(T.rows == T.cols);
    ALA_ASSERT(Q.rows == src.rows);
    ALA_ASSERT(T.rows == src.rows);

    size_t n = src.rows;
    aml_copy(T, src);
    aml_set_identity(Q);

    if (n <= 2) return;

    struct Aml_Mat2d vbuf = aml_mat2d_alloc(n, 1);

    for (size_t k = 0; k < n - 2; ++k) {
        struct Aml_Mat2d x = {
            .rows = n - (k + 1),
            .cols = 1,
            .stride_r = T.stride_r,
            .elements = &AML_MAT2D_AT(T, k + 1, k),
        };

        struct Aml_Mat2d v = {
            .rows = x.rows,
            .cols = 1,
            .stride_r = vbuf.stride_r,
            .elements = &AML_MAT2D_AT(vbuf, k + 1, 0),
        };

        ala_householder_top_element_vector_get(v, x);

        if (AML_IS_ZERO(aml_inner_product(v))) {
            continue;
        }

        ala_apply_householder_left(T, k + 1, k, v);
        ala_apply_householder_right(T, k + 1, v);

        ala_apply_householder_right(Q, k + 1, v);

        for (size_t i = k + 2; i < n; ++i) {
            AML_MAT2D_AT(T, i, k) = 0;
            AML_MAT2D_AT(T, k, i) = 0;
        }
    }

    aml_mat2d_free(vbuf);

}

ALA_DEF aml_real ala_symmetric_tridiagonal_calc_shift(struct Aml_Mat2d m, size_t *last, size_t *first)
{
    ALA_ASSERT(aml_is_symmetric(m));
    ALA_ASSERT(aml_is_tridiagonal(m));
    ALA_ASSERT(m.cols == m.rows); 
    ALA_ASSERT(m.rows > 0); 

    size_t temp_last = m.rows - 1;
    aml_real e, d0, d1;
    while (temp_last > 0) {
        e  = aml_fabs(AML_MAT2D_AT(m, temp_last - 1, temp_last));
        d0 = aml_fabs(AML_MAT2D_AT(m, temp_last - 1, temp_last - 1));
        d1 = aml_fabs(AML_MAT2D_AT(m, temp_last, temp_last));
        if (e < AML_EPS * (d0 + d1)) {
            temp_last--;
        } else {
            break;
        }
    }

    if (temp_last == 0) {
        return AML_MAT2D_AT(m, 0, 0);
    }

    e  = AML_MAT2D_AT(m, temp_last - 1, temp_last);
    d0 = AML_MAT2D_AT(m, temp_last - 1, temp_last - 1);
    d1 = AML_MAT2D_AT(m, temp_last, temp_last);

    aml_real delta = (d0 - d1) * (aml_real)0.5;
    aml_real sign = delta >= 0 ? 1 : -1;
    aml_real shift = d1 - sign * e * e / (aml_fabs(delta) + aml_hypot(delta, e));

    if (last) *last = temp_last;

    size_t temp_first = 0;
    for (size_t k = temp_last; k > 0; --k) {
        e  = aml_fabs(AML_MAT2D_AT(m, k, k - 1));
        d0 = aml_fabs(AML_MAT2D_AT(m, k - 1, k - 1));
        d1 = aml_fabs(AML_MAT2D_AT(m, k, k));
        if (e < AML_EPS * (d0 + d1)) {
            AML_MAT2D_AT(m, k, k - 1) = 0;
            AML_MAT2D_AT(m, k - 1, k) = 0;
            temp_first = k;
            break;
        }
    }

    if (first) *first = temp_first;

    return shift;
}

ALA_DEF void ala_symmetric_tridiagonal_eig_QR(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors)
{
    /**
     * https://www.youtube.com/watch?v=_IgDCL7OPdU&list=PLfNiIduhuYeBfgM2kuZCofXxWHx_bkAr1&index=17&pp=iAQB
     */

    ALA_ASSERT(aml_is_symmetric(A));
    ALA_ASSERT(aml_is_tridiagonal(A));
    ALA_ASSERT(A.cols == A.rows); 
    ALA_ASSERT(eigenvalues.cols == A.cols);
    ALA_ASSERT(eigenvalues.rows == A.rows);
    ALA_ASSERT(eigenvectors.cols == A.cols);
    ALA_ASSERT(eigenvectors.rows == A.rows);

    struct Aml_Mat2d given_c_and_s = aml_mat2d_alloc(eigenvalues.rows-1, 2);
    
    aml_copy(eigenvalues, A);
    aml_set_identity(eigenvectors);

    bool converged = true;
    for (size_t i = 0; i < ALA_SYMMETRIC_TRIDIAGONAL_EIG_QR_MAX_ITERATIONS; i++) {
        converged = false;

        for (size_t k = 0; k < eigenvalues.rows - 1; ++k) {
            aml_real a11 = AML_MAT2D_AT(eigenvalues, k, k);
            aml_real a21 = AML_MAT2D_AT(eigenvalues, k + 1, k);
            if (AML_IS_ZERO(a21)) {
                AML_MAT2D_AT(given_c_and_s, k, 0) = 1;
                AML_MAT2D_AT(given_c_and_s, k, 1) = 0;
                continue;
            }

            aml_real c = 0;
            aml_real s = 0;
            ala_givens_rotations_get_c_and_s(a11, a21, &c, &s);
            AML_MAT2D_AT(given_c_and_s, k, 0) = c;
            AML_MAT2D_AT(given_c_and_s, k, 1) = s;
            ala_apply_givens_left(eigenvalues, k, k, eigenvalues.rows-1, c, s);
            ala_apply_givens_right(eigenvectors, k, 0, eigenvectors.cols-1, c, s);
        }

        for (size_t k = 0; k < eigenvalues.rows - 1; ++k) {
            aml_real c = AML_MAT2D_AT(given_c_and_s, k, 0);
            aml_real s = AML_MAT2D_AT(given_c_and_s, k, 1);
            ala_apply_givens_right(eigenvalues, k, 0, eigenvalues.cols-1, c, s);
        }

        if (!(i % 10)) {
            converged = true;
            aml_real i_i, i_im1, im1_im1;
            for (size_t conv_index = 1; conv_index < eigenvalues.rows; conv_index++) {
                i_i   = aml_fabs(AML_MAT2D_AT(eigenvalues, conv_index, conv_index));
                i_im1 = aml_fabs(AML_MAT2D_AT(eigenvalues, conv_index, conv_index - 1));
                im1_im1 = aml_fabs(AML_MAT2D_AT(eigenvalues, conv_index - 1, conv_index - 1));
                if (i_im1 > AML_EPS * (i_i + im1_im1) && !AML_IS_ZERO(i_im1)) {
                    // aml_dprintWARNING("(%zu)(%zu) i_im1: %g > %g", i, conv_index, i_im1, AML_EPS * (i_i + im1_im1));
                    converged = false;
                    break;
                } 
            }
        }

        if (converged) {
            aml_dprintINFO("converged on iteration %zu", i);
            break;
        }
    }

    if (!converged) {
        aml_dprintWARNING("Did not converged after %d iterations.", ALA_SYMMETRIC_TRIDIAGONAL_EIG_QR_MAX_ITERATIONS);
    }


    aml_mat2d_free(given_c_and_s);
}

ALA_DEF void ala_symmetric_tridiagonal_eig_QR_shift(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors)
{
    /**
     * https://www.youtube.com/watch?v=_IgDCL7OPdU&list=PLfNiIduhuYeBfgM2kuZCofXxWHx_bkAr1&index=17&pp=iAQB
     */

    ALA_ASSERT(aml_is_symmetric(A));
    ALA_ASSERT(aml_is_tridiagonal(A));
    ALA_ASSERT(A.cols == A.rows); 
    ALA_ASSERT(eigenvalues.cols == A.cols);
    ALA_ASSERT(eigenvalues.rows == A.rows);
    ALA_ASSERT(eigenvectors.cols == A.cols);
    ALA_ASSERT(eigenvectors.rows == A.rows);
    ALA_ASSERT(eigenvalues.rows > 1);

    struct Aml_Mat2d given_c_and_s = aml_mat2d_alloc(eigenvalues.rows-1, 2);
    
    aml_copy(eigenvalues, A);
    aml_set_identity(eigenvectors);

    bool converged = true;
    for (size_t i = 0; i < ALA_SYMMETRIC_TRIDIAGONAL_EIG_QR_MAX_ITERATIONS; i++) {
        converged = false;

        size_t last, first;
        aml_real shift = ala_symmetric_tridiagonal_calc_shift(eigenvalues, &last, &first);

        aml_shift(eigenvalues, - shift);

        for (size_t k = first; k < last; ++k) {
            aml_real a11 = AML_MAT2D_AT(eigenvalues, k, k);
            aml_real a21 = AML_MAT2D_AT(eigenvalues, k + 1, k);
            if (AML_IS_ZERO(a21)) {
                AML_MAT2D_AT(given_c_and_s, k, 0) = 1;
                AML_MAT2D_AT(given_c_and_s, k, 1) = 0;
                continue;
            }

            aml_real c = 0;
            aml_real s = 0;
            ala_givens_rotations_get_c_and_s(a11, a21, &c, &s);
            AML_MAT2D_AT(given_c_and_s, k, 0) = c;
            AML_MAT2D_AT(given_c_and_s, k, 1) = s;
            ala_apply_givens_left(eigenvalues, k, k, eigenvalues.rows-1, c, s);
            AML_MAT2D_AT(eigenvalues, k+1, k) = 0;
            ala_apply_givens_right(eigenvectors, k, 0, eigenvectors.cols-1, c, s);
        }

        for (size_t k = first; k < last; ++k) {
            aml_real c = AML_MAT2D_AT(given_c_and_s, k, 0);
            aml_real s = AML_MAT2D_AT(given_c_and_s, k, 1);
            ala_apply_givens_right(eigenvalues, k, 0, k+1, c, s);
        }

        aml_shift(eigenvalues, shift);

        // aml_make_tridiagonal(eigenvalues);

        if (!(i % 10)) {
            converged = true;
            aml_real i_i, i_im1, im1_im1;
            for (size_t conv_index = 1; conv_index < eigenvalues.rows; conv_index++) {
                i_i   = aml_fabs(AML_MAT2D_AT(eigenvalues, conv_index, conv_index));
                i_im1 = aml_fabs(AML_MAT2D_AT(eigenvalues, conv_index, conv_index - 1));
                im1_im1 = aml_fabs(AML_MAT2D_AT(eigenvalues, conv_index - 1, conv_index - 1));
                if (i_im1 > AML_EPS * (i_i + im1_im1) && !AML_IS_ZERO(i_im1)) {
                    // aml_dprintWARNING("(%zu)(%zu) i_im1: %g > %g", i, conv_index, i_im1, AML_EPS * (i_i + im1_im1));
                    converged = false;
                    break;
                } 
            }
        }

        if (converged) {
            aml_dprintINFO("converged on iteration %zu", i);
            break;
        }
    }

    if (!converged) {
        aml_dprintWARNING("Did not converged after %d iterations.", ALA_SYMMETRIC_TRIDIAGONAL_EIG_QR_MAX_ITERATIONS);
    }


    aml_mat2d_free(given_c_and_s);
}

ALA_DEF void ala_symmetric_tridiagonal_eig_QR_implicit_shift(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors)
{
    /**
     * https://youtu.be/RSm_Mqi0aSA?list=PLfNiIduhuYeBfgM2kuZCofXxWHx_bkAr1
     */
    ALA_ASSERT(aml_is_symmetric(A));
    ALA_ASSERT(aml_is_tridiagonal(A));
    ALA_ASSERT(A.rows >= 2);
    ALA_ASSERT(A.cols == A.rows); 
    ALA_ASSERT(eigenvalues.cols == A.cols);
    ALA_ASSERT(eigenvalues.rows == A.rows);
    ALA_ASSERT(eigenvectors.cols == A.cols);
    ALA_ASSERT(eigenvectors.rows == A.rows);

    struct Aml_Mat2d given_c_and_s = aml_mat2d_alloc(eigenvalues.rows-1, 2);

    aml_copy(eigenvalues, A);
    aml_set_identity(eigenvectors);

    bool converged = true;
    size_t i = 0;
    for (; i < ALA_SYMMETRIC_TRIDIAGONAL_EIG_QR_MAX_ITERATIONS; i++) {
        converged = false;

        // printf("---------------------\n");

        size_t last = eigenvalues.rows - 1;
        aml_real e, d0, d1;
        while (last > 0) {
            e  = aml_fabs(AML_MAT2D_AT(eigenvalues, last - 1, last));
            d0 = aml_fabs(AML_MAT2D_AT(eigenvalues, last - 1, last - 1));
            d1 = aml_fabs(AML_MAT2D_AT(eigenvalues, last, last));
            if (e < AML_EPS * (d0 + d1)) {
                last--;
            } else {
                break;
            }
        }

        if (last == 0) {
            converged = true;
            break;
        }

        size_t first = 0;
        for (size_t k = last; k > 0; --k) {
            e  = aml_fabs(AML_MAT2D_AT(eigenvalues, k, k - 1));
            d0 = aml_fabs(AML_MAT2D_AT(eigenvalues, k - 1, k - 1));
            d1 = aml_fabs(AML_MAT2D_AT(eigenvalues, k, k));
            if (e < AML_EPS * (d0 + d1)) {
                AML_MAT2D_AT(eigenvalues, k, k - 1) = 0;
                AML_MAT2D_AT(eigenvalues, k - 1, k) = 0;
                first = k;
                break;
            }
        }



        e  = AML_MAT2D_AT(eigenvalues, last - 1, last);
        d0 = AML_MAT2D_AT(eigenvalues, last - 1, last - 1);
        d1 = AML_MAT2D_AT(eigenvalues, last, last);

        aml_real delta = (d0 - d1) * (aml_real)0.5;
        aml_real sign = delta >= 0 ? 1 : -1;
        aml_real shift = d1 - sign * e * e / (aml_fabs(delta) + aml_hypot(delta, e));
        aml_real a11 = AML_MAT2D_AT(eigenvalues, first, first) - shift;
        aml_real a21 = AML_MAT2D_AT(eigenvalues, first + 1, first);

        aml_real c = 0;
        aml_real s = 0;
        ala_givens_rotations_get_c_and_s(a11, a21, &c, &s);

        // AML_PRINT(eigenvalues);
        ala_apply_givens_left(eigenvalues, first, first, eigenvalues.rows-1, c, s);
        // AML_PRINT(eigenvalues);
        ala_apply_givens_right(eigenvalues, first, 0, eigenvalues.cols-1, c, s);

        ala_apply_givens_right(eigenvectors, first, 0, eigenvectors.cols-1, c, s);
        
        // AML_MAT2D_AT(eigenvalues, 2, 0) = 0;
        // AML_MAT2D_AT(eigenvalues, 0, 2) = 0;

        
        // for (size_t k = 0; k + 1 < eigenvalues.rows-1; ++k) {
        for (size_t k = first; k + 1 < last; ++k) {
            // aml_dprintSIZE_T(k);
            a11 = AML_MAT2D_AT(eigenvalues, k + 1, k);
            a21 = AML_MAT2D_AT(eigenvalues, k + 2, k);
            c = 0;
            s = 0;
            ala_givens_rotations_get_c_and_s(a11, a21, &c, &s);

            // AML_PRINT(eigenvalues);
            ala_apply_givens_left(eigenvalues, k+1, k, eigenvalues.rows-1, c, s);
            // AML_PRINT(eigenvalues);
            ala_apply_givens_right(eigenvalues, k+1, 0, eigenvalues.cols-1, c, s);

            ala_apply_givens_right(eigenvectors, k+1, 0, eigenvectors.cols-1, c, s);

            if (k + 2 < eigenvalues.rows) {
                AML_MAT2D_AT(eigenvalues, k + 2, k) = 0;
                AML_MAT2D_AT(eigenvalues, k, k + 2) = 0;
            }
        }

        // AML_PRINT(eigenvalues);

        // if (i == 107) {
        //     exit(1);
        // }

        if (!(i % 10)) {
            converged = true;
            aml_real i_i, i_im1, im1_im1;
            for (size_t conv_index = 1; conv_index < eigenvalues.rows; conv_index++) {
                i_i   = aml_fabs(AML_MAT2D_AT(eigenvalues, conv_index, conv_index));
                i_im1 = aml_fabs(AML_MAT2D_AT(eigenvalues, conv_index, conv_index - 1));
                im1_im1 = aml_fabs(AML_MAT2D_AT(eigenvalues, conv_index - 1, conv_index - 1));
                if (i_im1 > AML_EPS * (i_i + im1_im1) && !AML_IS_ZERO(i_im1)) {
                    // aml_dprintWARNING("(%zu)(%zu) i_im1: %g > %g", i, conv_index, i_im1, AML_EPS * (i_i + im1_im1));
                    converged = false;
                    break;
                } 
            }
        }

        if (converged) {
            break;
        }

        // aml_make_tridiagonal(eigenvalues);
        // aml_make_symmetric(eigenvalues);
    }

    if (!converged) {
        aml_dprintWARNING("Did not converged after %d iterations.", ALA_SYMMETRIC_TRIDIAGONAL_EIG_QR_MAX_ITERATIONS);
    } else {
        aml_dprintINFO("converged on iteration %zu", i);
    }

    aml_mat2d_free(given_c_and_s);

}

ALA_DEF void ala_symmetric_tridiagonal_QR(struct Aml_Mat2d Q, struct Aml_Mat2d R, struct Aml_Mat2d src)
{
    /**
     * https://youtu.be/_IgDCL7OPdU?list=PLfNiIduhuYeBfgM2kuZCofXxWHx_bkAr1
     */

    ALA_ASSERT(aml_is_symmetric(src));
    ALA_ASSERT(aml_is_tridiagonal(src));
    ALA_ASSERT(Q.rows == Q.cols);
    ALA_ASSERT(R.rows == src.rows);
    ALA_ASSERT(R.cols == src.cols);
    ALA_ASSERT(Q.rows == src.rows);

    aml_copy(R, src);
    aml_set_identity(Q);

    size_t n = src.rows;
    if (n <= 1) {
        return;
    }

    for (size_t k = 0; k < n - 1; ++k) {
        aml_real a11 = AML_MAT2D_AT(R, k, k);
        aml_real a21 = AML_MAT2D_AT(R, k + 1, k);

        if (AML_IS_ZERO(a21)) {
            continue;
        }

        aml_real c = 0;
        aml_real s = 0;
        ala_givens_rotations_get_c_and_s(a11, a21, &c, &s);

        ala_apply_givens_left(R, k, k, R.rows-1, c, s);
        AML_MAT2D_AT(R, k + 1, k) = 0;

        ala_apply_givens_right(Q, k, 0, Q.cols-1, c, s);
    }
}

ALA_DEF aml_real ala_upper_triangulate(struct Aml_Mat2d m, uint8_t flags)
{
    /* preforming Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */
    /* returns the factor multiplying the determinant */

    aml_real factor_to_return = 1;

    size_t r = 0;
    for (size_t c = 0; c < m.cols && r < m.rows; c++) {
        if (flags & AML_ROW_SWAPPING) {
            /* finding biggest first number (absolute value); partial pivoting */
            size_t piv = r;
            aml_real best = aml_fabs(AML_MAT2D_AT(m, r, c));
            for (size_t i = r + 1; i < m.rows; i++) {
                aml_real v = aml_fabs(AML_MAT2D_AT(m, i, c));
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

        aml_real pivot = AML_MAT2D_AT(m, r, c);
        AML_ASSERT(!AML_IS_ZERO(pivot));

        if (flags & AML_ONES_ON_DIAG) {
            aml_mult_row(m, r, (aml_real)1 / pivot);
            factor_to_return *= pivot;
            pivot = (aml_real)1;
        }

        /* Eliminate entries below pivot in column c */
        for (size_t i = r + 1; i < m.rows; i++) {
            aml_real f = AML_MAT2D_AT(m, i, c) / pivot;
            aml_sub_row_time_factor_to_row(m, i, r, f);
        }
        r++;
    }
    return factor_to_return;
}




#endif // ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION

