/**
 * @file 
 * @brief Higher-level linear algebra algorithms built on top of Aml_Mat2d.
 *
 * This layer includes decompositions, eigensolvers, SVD routines, Householder
 * and Givens transformations, and linear-system solvers.
 * 
 * Some of the algorithms are inspired by the
 *  - Foundations to Frontiers course on Advanced Linear Algebra: https://www.cs.utexas.edu/~flame/laff/alaff/frontmatter.html
 */

#ifndef ALMOG_LINEAR_ALGEBRA_H_
#define ALMOG_LINEAR_ALGEBRA_H_

#include "Almog_Matrix_Library.h"

#ifndef ALA_ASSERT
#define ALA_ASSERT AML_ASSERT
#endif //ALA_ASSERT

struct Ala_Symmetric_Spectrum_Info {
    aml_real max_abs_eigenvalue;
    aml_real min_abs_eigenvalue;
    aml_real min_abs_significant_eigenvalue;
    aml_real tolerance;
    aml_real reciprocal_condition_number;
    aml_real effective_condition_number;
    size_t numerical_rank;
    bool is_numerically_singular;
};

/**
 * @brief Flags for aml_upper_triangulate()-style elimination routines.
 */
enum Ala_Upper_Triangulate_Flag{
    /**
     * Normalize pivot rows so the diagonal pivot becomes 1.
     */
    ALA_UPPER_TRIANGULATE_ONES_ON_DIAG = 1 << 0,
    /**
     * Enable row swapping / pivot search.
     */
    ALA_UPPER_TRIANGULATE_ROW_SWAPPING = 1 << 1,
};

#ifndef ALA_DEF
    #ifdef ALA_DEF_STATIC
        #define ALA_DEF static
    #else
        #define ALA_DEF extern
    #endif
#endif

#define ALA_MAX_POWER_ITERATION 100
#define ALA_SYMMETRIC_EIG_QR_MAX_ITERATIONS 10000
#define ALA_SYMMETRIC_TRIDIAGONAL_EIG_QR_MAX_ITERATIONS 10000
#define ALA_SYMMETRIC_TRIDIAGONAL_EIG_QR_MAX_ITERATIONS_MULTIPLAYER 150
#define ALA_SYMMETRIC_TRIDIAGONAL_EIG_QR_IMPLICIT_MAX_ITERATIONS_MULTIPLAYER 150
#define ALA_HESSENBERG_SCHUR_DECOMPOSITION_MAX_ITERATIONS_MULTIPLAYER 150

ALA_DEF void                                ala_apply_givens_2x2_left(struct Aml_Mat2d A, size_t i, size_t js, size_t je, aml_real c, aml_real s);
ALA_DEF void                                ala_apply_givens_2x2_right(struct Aml_Mat2d A, size_t j, size_t is, size_t ie, aml_real c, aml_real s);
ALA_DEF aml_real                            ala_householder_beta3(aml_real v0, aml_real v1, aml_real v2);
ALA_DEF void                                ala_apply_householder_left(struct Aml_Mat2d A, size_t row0, size_t col0, struct Aml_Mat2d v);
ALA_DEF void                                ala_apply_householder_left3(struct Aml_Mat2d A, size_t row0, size_t col0, size_t col1, aml_real v0, aml_real v1, aml_real v2, aml_real beta);
ALA_DEF void                                ala_apply_householder_right(struct Aml_Mat2d A, size_t col0, struct Aml_Mat2d v);
ALA_DEF void                                ala_apply_householder_right3(struct Aml_Mat2d A, size_t row0, size_t row1, size_t col0, aml_real v0, aml_real v1, aml_real v2, aml_real beta);

ALA_DEF void                                ala_convolve(struct Aml_Mat2d m, struct Aml_Mat2d a, struct Aml_Mat2d b);

ALA_DEF aml_real                            ala_det(struct Aml_Mat2d m);
ALA_DEF aml_real                            ala_det_2x2_mat(struct Aml_Mat2d m);

ALA_DEF bool                                ala_eig_check(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors, struct Aml_Mat2d res);
ALA_DEF void                                ala_eig_power_iteration(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors, struct Aml_Mat2d init_vector, bool norm_inf_vectors);
ALA_DEF void                                ala_eigenpairs_sort(struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors);
ALA_DEF bool                                ala_eigenvalues_real_2x2(aml_real a, aml_real b, aml_real c, aml_real d);

ALA_DEF aml_real                            ala_givens_2x2_rotations_get_c_and_s(aml_real a11, aml_real a21, aml_real *c, aml_real *s);

ALA_DEF void                                ala_hessenberg_decomposition_householder(struct Aml_Mat2d Q, struct Aml_Mat2d H, struct Aml_Mat2d A);
ALA_DEF void                                ala_hessenberg_calc_double_shift(struct Aml_Mat2d m, size_t last, aml_real *trace, aml_real *det);
ALA_DEF bool                                ala_hessenberg_deflate_tail(struct Aml_Mat2d m, size_t *first, size_t *last, aml_real eps);
ALA_DEF void                                ala_hessenberg_QUQm1_schur_decomposition_given(struct Aml_Mat2d Q, struct Aml_Mat2d U, struct Aml_Mat2d H);
ALA_DEF void                                ala_hessenberg_QUQm1_schur_decomposition_householder(struct Aml_Mat2d Q, struct Aml_Mat2d U, struct Aml_Mat2d H, bool compute_Q);
ALA_DEF void                                ala_hessenberg_QUQm1_schur_decomposition_householder_fast(struct Aml_Mat2d Q, struct Aml_Mat2d U, struct Aml_Mat2d H, bool compute_Q);
ALA_DEF bool                                ala_hessenberg_real_schur_2x2(struct Aml_Mat2d Q, struct Aml_Mat2d U, size_t k);
ALA_DEF void                                ala_householder_matrix_get(struct Aml_Mat2d des, struct Aml_Mat2d v);
ALA_DEF void                                ala_householder_top_element_vector_get(struct Aml_Mat2d v_des, struct Aml_Mat2d x);

ALA_DEF void                                ala_invert(struct Aml_Mat2d des, struct Aml_Mat2d src);

ALA_DEF void                                ala_LUP_decomposition_with_swap(struct Aml_Mat2d src, struct Aml_Mat2d l, struct Aml_Mat2d p, struct Aml_Mat2d u);

ALA_DEF void                                ala_make_orthogonal_Gaussian_elimination(struct Aml_Mat2d des, struct Aml_Mat2d A);
ALA_DEF void                                ala_make_orthogonal_modified_Gram_Schmidt(struct Aml_Mat2d des, struct Aml_Mat2d A);

ALA_DEF bool                                ala_positive_definite_check(struct Aml_Mat2d A);
ALA_DEF bool                                ala_positive_definite_RTR_Cholesky_decomposition(struct Aml_Mat2d R, struct Aml_Mat2d A);
ALA_DEF int                                 ala_power_iterate(struct Aml_Mat2d A, struct Aml_Mat2d v, aml_real *lambda, aml_real shift, bool norm_inf_v);
ALA_DEF void                                ala_project_out_columns(struct Aml_Mat2d v, struct Aml_Mat2d basis, size_t used_cols);

ALA_DEF void                                ala_QR_decomposition_householder(struct Aml_Mat2d Q, struct Aml_Mat2d R, struct Aml_Mat2d src);
ALA_DEF void                                ala_QR_decomposition_householder_fast(struct Aml_Mat2d Q, struct Aml_Mat2d R, struct Aml_Mat2d src);

ALA_DEF size_t                              ala_reduce(struct Aml_Mat2d m);

ALA_DEF aml_real                            ala_schur_residual(struct Aml_Mat2d H0, struct Aml_Mat2d Q, struct Aml_Mat2d U);
ALA_DEF void                                ala_solve_linear_sys_LUP_decomposition(struct Aml_Mat2d A, struct Aml_Mat2d x, struct Aml_Mat2d B);
ALA_DEF void                                ala_SVD_full(struct Aml_Mat2d A, struct Aml_Mat2d U, struct Aml_Mat2d S, struct Aml_Mat2d V, struct Aml_Mat2d init_vec_u, struct Aml_Mat2d init_vec_v, bool return_v_transpose);
ALA_DEF void                                ala_SVD_thin(struct Aml_Mat2d A, struct Aml_Mat2d U, struct Aml_Mat2d S, struct Aml_Mat2d V, struct Aml_Mat2d init_vec_u, struct Aml_Mat2d init_vec_v, bool return_v_transpose);

ALA_DEF void                                ala_symmetric_eig_QR_shift(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors);
ALA_DEF void                                ala_symmetric_eig_QR_tridiagonalize(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors);
ALA_DEF void                                ala_symmetric_eig_QR_tridiagonalize_implicit_shift(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors);
ALA_DEF void                                ala_symmetric_eigen_approximation(struct Aml_Mat2d approx, struct Aml_Mat2d A, size_t order);
ALA_DEF void                                ala_symmetric_eigen_approximation_build(struct Aml_Mat2d approx, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors, size_t order);
ALA_DEF struct Ala_Symmetric_Spectrum_Info  ala_symmetric_spectrum_analyze(struct Aml_Mat2d eigenvalues, aml_real rel_tol_multiplier);
ALA_DEF void                                ala_symmetric_spectrum_info_print(const struct Ala_Symmetric_Spectrum_Info info);

ALA_DEF void                                ala_symmetric_tridiagonalize_householder(struct Aml_Mat2d Q, struct Aml_Mat2d T, struct Aml_Mat2d src);
ALA_DEF aml_real                            ala_symmetric_tridiagonal_calc_shift(struct Aml_Mat2d m, size_t last);
ALA_DEF void                                ala_symmetric_tridiagonal_cleanup(struct Aml_Mat2d T, size_t first, size_t last);
ALA_DEF bool                                ala_symmetric_tridiagonal_deflate_tail(struct Aml_Mat2d m, size_t *first, size_t *last, aml_real eps);
ALA_DEF void                                ala_symmetric_tridiagonal_eig_QR(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors);
ALA_DEF void                                ala_symmetric_tridiagonal_eig_QR_shift(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors);
ALA_DEF void                                ala_symmetric_tridiagonal_eig_QR_implicit_shift(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors);
ALA_DEF void                                ala_symmetric_tridiagonal_QR(struct Aml_Mat2d Q, struct Aml_Mat2d R, struct Aml_Mat2d src);


ALA_DEF aml_real                            ala_upper_triangulate(struct Aml_Mat2d m, enum Ala_Upper_Triangulate_Flag flags);

#endif // ALMOG_LINEAR_ALGEBRA_H_

#ifdef ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#undef ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION


/**
 * @brief Apply a Givens rotation from the left to two adjacent rows.
 *
 * Rows `i` and `i+1` are rotated over the column range `[js, je]` using
 *:
 * `[[c, s], [-s, c]]`.
 *
 * @param A Matrix modified in place.
 * @param i First row of the rotated pair.
 * @param js First affected column.
 * @param je Last affected column.
 * @param c Cosine term.
 * @param s Sine term.
 *
 * Complexity
 * `O(je - js + 1)`.
 */
ALA_DEF void ala_apply_givens_2x2_left(struct Aml_Mat2d A, size_t i, size_t js, size_t je, aml_real c, aml_real s)
{
    ALA_ASSERT(i + 1 < A.rows);
    ALA_ASSERT(js < A.cols || A.cols == 0);
    ALA_ASSERT(je < A.cols);
    ALA_ASSERT(js <= je);

    for (size_t j = js; j <= je; ++j) {
        aml_real x = AML_MAT2D_AT(A, i, j);
        aml_real y = AML_MAT2D_AT(A, i + 1, j);

        AML_MAT2D_AT(A, i, j)     = c * x + s * y;
        AML_MAT2D_AT(A, i + 1, j) = -s * x + c * y;
    }
}

/**
 * @brief Apply a Givens rotation from the right to two adjacent columns.
 *
 * Columns `j` and `j+1` are rotated over the row range `[is, ie]` using the
 * same 2x2 rotation convention as ala_apply_givens_2x2_left().
 *
 * @param A Matrix modified in place.
 * @param j First column of the rotated pair.
 * @param is First affected row.
 * @param ie Last affected row.
 * @param c Cosine term.
 * @param s Sine term.
 *
 * Complexity
 * `O(ie - is + 1)`.
 */
ALA_DEF void ala_apply_givens_2x2_right(struct Aml_Mat2d A, size_t j, size_t is, size_t ie, aml_real c, aml_real s)
{
    ALA_ASSERT(j + 1 < A.cols);
    ALA_ASSERT(is < A.rows || A.rows == 0);
    ALA_ASSERT(ie < A.rows);
    ALA_ASSERT(is <= ie);

    for (size_t i = is; i <= ie; ++i) {
        aml_real x = AML_MAT2D_AT(A, i, j);
        aml_real y = AML_MAT2D_AT(A, i, j + 1);

        AML_MAT2D_AT(A, i, j)     = c * x + s * y;
        AML_MAT2D_AT(A, i, j + 1) = -s * x + c * y;
    }
}

ALA_DEF aml_real ala_householder_beta3(aml_real v0, aml_real v1, aml_real v2)
{
    aml_real vv = v0 * v0 + v1 * v1 + v2 * v2;
    ALA_ASSERT(!AML_IS_ZERO(vv));
    return (aml_real)2 / vv;
}

/**
 * @brief Apply a Householder reflector from the left to a trailing submatrix.
 *
 * Applies `H = I - 2vv^T/(v^Tv)` to rows `row0..row0+v.rows-1` and columns
 * `col0..A.cols-1`, i.e. `A <- H A` on the active block.
 *
 * @param A Matrix modified in place.
 * @param row0 First affected row.
 * @param col0 First affected column.
 * @param v Householder vector.
 *
 * Complexity
 * `O(v.rows * (A.cols - col0))`.
 */
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

ALA_DEF void ala_apply_householder_left3(struct Aml_Mat2d A, size_t row0, size_t col0, size_t col1, aml_real v0, aml_real v1, aml_real v2, aml_real beta)
{
    for (size_t j = col0; j <= col1; ++j) {
        aml_real *a0 = &AML_MAT2D_AT(A, row0 + 0, j);
        aml_real *a1 = &AML_MAT2D_AT(A, row0 + 1, j);
        aml_real *a2 = &AML_MAT2D_AT(A, row0 + 2, j);

        aml_real dot = beta * (v0 * (*a0) + v1 * (*a1) + v2 * (*a2));
        *a0 -= dot * v0;
        *a1 -= dot * v1;
        *a2 -= dot * v2;
    }
}

/**
 * @brief Apply a Householder reflector from the right to a trailing submatrix.
 *
 * Applies `A <- A H` where `H = I - 2vv^T/(v^Tv)` to columns
 * `col0..col0+v.rows-1`.
 *
 * @param A Matrix modified in place.
 * @param col0 First affected column.
 * @param v Householder vector.
 *
 * Complexity
 * `O(A.rows * v.rows)`.
 */
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

ALA_DEF void ala_apply_householder_right3(struct Aml_Mat2d A, size_t row0, size_t row1, size_t col0, aml_real v0, aml_real v1, aml_real v2, aml_real beta)
{
    ALA_ASSERT(col0 + 2 < A.cols);
    ALA_ASSERT(row0 <= row1);
    ALA_ASSERT(row1 < A.rows);

    for (size_t i = row0; i <= row1; ++i) {
        aml_real *a0 = &AML_MAT2D_AT(A, i, col0 + 0);
        aml_real *a1 = &AML_MAT2D_AT(A, i, col0 + 1);
        aml_real *a2 = &AML_MAT2D_AT(A, i, col0 + 2);

        aml_real dot = beta * ((*a0) * v0 + (*a1) * v1 + (*a2) * v2);

        *a0 -= dot * v0;
        *a1 -= dot * v1;
        *a2 -= dot * v2;
    }
}

/**
 * @brief Perform valid 2D convolution of matrix @p a with kernel @p b.
 *
 * The kernel is rotated by 180 degrees in place, the valid convolution is
 * computed, and then the kernel is rotated back, so @p b is restored on return.
 *
 * The implementation requires a square kernel.
 *
 * @param m Output matrix of size `(a.rows-b.rows+1) x (a.cols-b.cols+1)`.
 * @param a Input matrix / image.
 * @param b Square kernel matrix.
 *
 * Complexity
 * `O(m.rows * m.cols * b.rows * b.cols)`.
 */
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

/**
 * @brief Compute the determinant of a square matrix.
 *
 * The implementation:
 * - returns 0 early if it finds an exactly zero row or column,
 * - copies the matrix,
 * - upper-triangularizes it with row swapping,
 * - multiplies the diagonal,
 * - corrects the sign/factor using the elimination return value.
 *
 * @param m Input square matrix.
 * @return Determinant of @p m.
 *
 * Complexity
 * `O(n^3)`.
 *
 * @note This is the general determinant routine.
 */
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
    aml_real factor = ala_upper_triangulate(temp_m, ALA_UPPER_TRIANGULATE_ROW_SWAPPING);
    aml_real diag_mul = 1; 
    for (size_t i = 0; i < temp_m.rows; i++) {
        diag_mul *= AML_MAT2D_AT(temp_m, i, i);
    }
    aml_mat2d_free(temp_m);

    return diag_mul / factor;
}

/**
 * @brief Compute the determinant of a 2x2 matrix directly.
 *
 * Returns `a00*a11 - a01*a10`.
 *
 * @param m Input 2x2 matrix.
 * @return Determinant.
 *
 * Complexity
 * `O(1)`.
 */
ALA_DEF aml_real ala_det_2x2_mat(struct Aml_Mat2d m)
{
    ALA_ASSERT(2 == m.cols && 2 == m.rows && "Not a 2x2 matrix");
    return AML_MAT2D_AT(m, 0, 0) * AML_MAT2D_AT(m, 1, 1) - AML_MAT2D_AT(m, 0, 1) * AML_MAT2D_AT(m, 1, 0);
}

/**
 * @brief Verify a symmetric eigendecomposition.
 *
 * This routine checks:
 * - that @p eigenvalues is diagonal,
 * - orthogonality of `eigenvectors`,
 * - the residual `A V - V D`,
 * - the reconstruction `V D V^T - A`.
 *
 * Workspace is written into @p res.
 *
 * @param A Original square matrix.
 * @param eigenvalues Diagonal matrix `D`.
 * @param eigenvectors Eigenvector matrix `V`.
 * @param res Workspace / residual matrix.
 * @return `true` if all checks pass within a tolerance.
 *
 * Complexity
 * `O(n^3)`.
 *
 * @note This routine assumes an orthonormal eigenbasis and the relation
 * `A = V D V^T`.
 */
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

    aml_real eps = AML_EPS * (aml_real)1e3;
    aml_real norma;

    bool return_value = true;

    if (!aml_is_diagonal(eigenvalues)) {
        aml_dprintWARNING("%s", "eigenvalues matrix is not diagonal.");
        return_value = false;
    }

    /* orthogonality check */
    aml_transpose(VL, eigenvectors);
    aml_dot(res, VL, eigenvectors);
    aml_set_identity(VL);
    aml_sub(res, VL);
    norma = aml_calc_norma(res);
    if (aml_fabs(norma) > eps) {
        aml_dprintWARNING("|V^T V - I| is to big = %g", norma);
        return_value = false;
    }

    /* |res| check */
    aml_dot(res, A, eigenvectors);
    aml_dot(VL, eigenvectors, eigenvalues);
    aml_sub(res, VL);
    norma = aml_calc_norma(res) / aml_calc_norma(A);
    if (aml_fabs(norma) > eps) {
        aml_dprintWARNING("|AV - VD| is to big = %g", norma);
        return_value = false;
    }

    /* reconstruction check */
    aml_transpose(VL, eigenvectors);
    aml_dot(res, eigenvalues, VL);
    aml_copy(VL, res);
    aml_dot(res, eigenvectors, VL);
    aml_sub(res, A);
    norma = aml_calc_norma(res) / aml_calc_norma(A);
    if (aml_fabs(norma) > eps) {
        aml_dprintWARNING("|V D V^T - A| is to big = %g", norma);
        return_value = false;
    }

    aml_mat2d_free(VL);
    return return_value;
}

/**
 * @brief Compute eigenpairs by repeated power iteration with deflation.
 *
 * The routine repeatedly:
 * - seeds the next vector from @p init_vector,
 * - projects out previously found columns,
 * - calls ala_power_iterate(),
 * - stores the eigenpair,
 * - deflates `B <- B - lambda * v v^T`.
 *
 * @param A Input square matrix.
 * @param eigenvalues Output diagonal matrix of eigenvalues.
 * @param eigenvectors Output matrix whose columns are eigenvectors.
 * @param init_vector Initial vector reused for each eigenpair.
 * @param norm_inf_vectors If true, renormalize output eigenvectors with the
 * infinity norm at the end.
 *
 * Complexity
 * Approximately `O(AML_MAX_POWER_ITERATION * n^3)`.
 *
 * @note Best for small educational problems or when only a few dominant
 * eigenpairs are needed and the assumptions roughly hold.
 * @warning The source comment states assumptions that the eigenvectors form an
 * orthonormal basis and that the largest eigenvalue is positive and unique.
 */
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
        if (ala_power_iterate(B, v, &AML_MAT2D_AT(eigenvalues, i, i), (aml_real)shift_value, 0)) { /* norm_inf_v must be zero*/
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

ALA_DEF void ala_eigenpairs_sort(struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors)
{
    for (size_t i = 0; i < eigenvalues.cols - 1; i++) {
        for (size_t j = i + 1; j < eigenvalues.cols; j++) {
            if (aml_fabs(AML_MAT2D_AT(eigenvalues, i, i)) < aml_fabs(AML_MAT2D_AT(eigenvalues, j, j))) {
                aml_cols_swap(eigenvalues, i, j);
                aml_rows_swap(eigenvalues, i, j);
                aml_cols_swap(eigenvectors, i, j);
            }
        }
    }
}

ALA_DEF bool ala_eigenvalues_real_2x2(aml_real a, aml_real b, aml_real c, aml_real d)
{
    aml_real discriminant = (a - d) * (a - d) + 4 * b * c;
    return (discriminant > 0) || (AML_IS_ZERO(discriminant)); 
}

/**
 * @brief Compute Givens rotation parameters that zero the second entry.
 *
 * For inputs `(a11, a21)`, this returns `r = hypot(a11, a21)` and optionally
 * stores `c = a11/r`, `s = a21/r`, so that applying the corresponding rotation
 * from the left maps `[a11, a21]^T` to `[r, 0]^T`.
 *
 * @param a11 Top entry.
 * @param a21 Bottom entry.
 * @param c Optional output cosine.
 * @param s Optional output sine.
 * @return The rotated top value `r`.
 *
 * Complexity
 * `O(1)`.
 */
ALA_DEF aml_real ala_givens_2x2_rotations_get_c_and_s(aml_real a11, aml_real a21, aml_real *c, aml_real *s)
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

ALA_DEF void ala_hessenberg_decomposition_householder(struct Aml_Mat2d Q, struct Aml_Mat2d H, struct Aml_Mat2d A)
{
    ALA_ASSERT(A.rows == A.cols);
    ALA_ASSERT(Q.rows == Q.cols);
    ALA_ASSERT(H.rows == H.cols);
    ALA_ASSERT(Q.rows == A.rows);
    ALA_ASSERT(H.rows == A.rows);

    size_t n = A.rows;
    aml_copy(H, A);
    aml_set_identity(Q);

    if (n <= 2) return;

    struct Aml_Mat2d vbuf = aml_mat2d_alloc(n, 1);

    for (size_t k = 0; k < n - 2; ++k) {
        struct Aml_Mat2d x = {
            .rows = n - (k + 1),
            .cols = 1,
            .stride_r = H.stride_r,
            .elements = &AML_MAT2D_AT(H, k + 1, k),
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

        ala_apply_householder_left(H, k + 1, k, v);
        ala_apply_householder_right(H, k + 1, v);
        ala_apply_householder_right(Q, k + 1, v);

        for (size_t i = k + 2; i < n; ++i) {
            AML_MAT2D_AT(H, i, k) = 0;
        }
    }

    aml_mat2d_free(vbuf);
}

ALA_DEF void ala_hessenberg_calc_double_shift(struct Aml_Mat2d m, size_t last, aml_real *trace, aml_real *det)
{
    ALA_ASSERT(aml_is_hessenberg(m));
    ALA_ASSERT(m.rows > 0); 
    ALA_ASSERT(last < m.rows); 
    ALA_ASSERT(last > 0);

    aml_real a, b, c, d;
    
    a = AML_MAT2D_AT(m, last - 1, last - 1);
    b = AML_MAT2D_AT(m, last - 1, last);
    c = AML_MAT2D_AT(m, last, last - 1);
    d = AML_MAT2D_AT(m, last, last);

    if (trace) *trace = a + d;
    if (det) *det = a * d - b * c;
}

ALA_DEF bool ala_hessenberg_deflate_tail(struct Aml_Mat2d m, size_t *first, size_t *last, aml_real eps)
{
    ALA_ASSERT(aml_is_hessenberg(m));
    ALA_ASSERT(m.rows == m.cols);
    ALA_ASSERT(m.rows > 0);

    size_t l = m.rows - 1;

    while (l > 1) {
        aml_real sub_l = aml_fabs(AML_MAT2D_AT(m, l, l - 1));
        aml_real sub_lm1 = aml_fabs(AML_MAT2D_AT(m, l - 1, l - 2));
        aml_real dlm2 = aml_fabs(AML_MAT2D_AT(m, l - 2, l - 2));
        aml_real dlm1 = aml_fabs(AML_MAT2D_AT(m, l - 1, l - 1));
        aml_real dl = aml_fabs(AML_MAT2D_AT(m, l, l));

        /* first checking the 1x1 block and then the 2x2 block */
        if (sub_l <= eps * (dl + dlm1)) {
            AML_MAT2D_AT(m, l, l - 1) = 0;
            --l;
            continue;
        }
        // aml_real a = AML_MAT2D_AT(m, l - 1, l - 1);
        // aml_real b = AML_MAT2D_AT(m, l - 1, l);
        // aml_real c = AML_MAT2D_AT(m, l, l - 1);
        // aml_real d = AML_MAT2D_AT(m, l, l);
        // if (sub_lm1 <= eps * (dl + dlm1 + dlm2) && !ala_eigenvalues_real_2x2(a, b, c, d)) {
        if (sub_lm1 <= eps * (dl + dlm1 + dlm2)) {
            AML_MAT2D_AT(m, l - 1, l - 2) = 0;
            l -= 2;
            continue;
        }

        break;
    }

    if (l == 0 || l == 1) {
        if (first) *first = 0;
        if (last)  *last = l;
        return true;
    }

    size_t f = 0;
    for (size_t k = l; k > 1; --k) {
        aml_real sub_k = aml_fabs(AML_MAT2D_AT(m, k, k - 1));
        aml_real sub_km1 = aml_fabs(AML_MAT2D_AT(m, k - 1, k - 2));
        aml_real dkm2 = aml_fabs(AML_MAT2D_AT(m, k - 2, k - 2));
        aml_real dkm1 = aml_fabs(AML_MAT2D_AT(m, k - 1, k - 1));
        aml_real dk = aml_fabs(AML_MAT2D_AT(m, k, k));

        /* first checking the 1x1 block and then the 2x2 block */
        if (sub_k <= eps * (dk + dkm1)) {
            AML_MAT2D_AT(m, k, k - 1) = 0;
            f = k;
            break;
        }
        // aml_real a = AML_MAT2D_AT(m, k - 1, k - 1);
        // aml_real b = AML_MAT2D_AT(m, k - 1, k);
        // aml_real c = AML_MAT2D_AT(m, k, k - 1);
        // aml_real d = AML_MAT2D_AT(m, k, k);
        // if (sub_km1 <= eps * (dk + dkm1 + dkm2) && !ala_eigenvalues_real_2x2(a, b, c, d)) {
        if (sub_km1 <= eps * (dk + dkm1 + dkm2)) {
            AML_MAT2D_AT(m, k - 1, k - 2) = 0;
            f = k - 1;
            break;
        }
    }

    if (first) *first = f;
    if (last)  *last = l;
    return ((l - f + 1) <= 2) && (f == 0);
}

ALA_DEF void ala_hessenberg_QUQm1_schur_decomposition_given(struct Aml_Mat2d Q, struct Aml_Mat2d U, struct Aml_Mat2d H)
{
    /**
     * https://en.wikipedia.org/wiki/Schur_decomposition
     * https://www.mathworks.com/help/matlab/ref/schur.html
     */
    ALA_ASSERT(H.rows == H.cols);
    ALA_ASSERT(Q.rows == Q.cols);
    ALA_ASSERT(U.rows == U.cols);
    ALA_ASSERT(Q.rows == H.rows);
    ALA_ASSERT(U.rows == H.rows);
    ALA_ASSERT(aml_is_hessenberg(H));

    size_t n = H.rows;
    aml_copy(U, H);
    aml_set_identity(Q);

    bool converged = false;
    bool deflated = false;

    aml_dprintINFO("%s", "");
    // printf("\33[A");
    for (size_t i = 0; i < ALA_HESSENBERG_SCHUR_DECOMPOSITION_MAX_ITERATIONS_MULTIPLAYER * n; i++) {
        printf("\33[A\33[2K\r       i: %zu\n", i);
        size_t last = 0, first = 0;
        deflated = ala_hessenberg_deflate_tail(U, &first, &last, aml_min(AML_EPS * 100, (aml_real)1e-4));
        if (deflated || last - first < 2) {
            converged = true;
            aml_dprintINFO("converged on iteration %zu", i);
            break;
        }

        // aml_dprintINFO("first = %zu | last = %zu\n", first, last);

        aml_real trace = 0, det = 0;
        ala_hessenberg_calc_double_shift(U, last, &trace, &det);

        aml_real u_k_k     = AML_MAT2D_AT(U, first    , first    );
        aml_real u_kp1_k   = AML_MAT2D_AT(U, first + 1, first    );
        aml_real u_k_kp1   = AML_MAT2D_AT(U, first    , first + 1);
        aml_real u_kp1_kp1 = AML_MAT2D_AT(U, first + 1, first + 1);
        aml_real u_kp2_kp1 = AML_MAT2D_AT(U, first + 2, first + 1);

        aml_real x = u_k_k * u_k_k + u_k_kp1 * u_kp1_k - trace * u_k_k + det;
        aml_real y = u_kp1_k * (u_k_k + u_kp1_kp1 - trace);
        aml_real z = u_kp1_k * u_kp2_kp1;

        for (size_t k = first; k + 1 <= last; ++k) {
            if (k + 2 <= last) {
                aml_real c1 = 0, s1 = 0, c2 = 0, s2 = 0;
                ala_givens_2x2_rotations_get_c_and_s(y, z, &c1, &s1);
                ala_givens_2x2_rotations_get_c_and_s(x, aml_hypot(y, z), &c2, &s2);

                size_t begin = (k == first) ? first : (k - 1);
                ala_apply_givens_2x2_left(U, k+1, begin, n - 1, c1, s1);
                ala_apply_givens_2x2_left(U, k, begin, n - 1, c2, s2);

                // printf("-------left k = %zu ---------\n", k);
                // AML_PRINT(U);

                ala_apply_givens_2x2_right(U, k+1, 0, n - 1, c1, s1);
                ala_apply_givens_2x2_right(U, k, 0, n - 1, c2, s2);

                // if (k > first) {
                //     AML_MAT2D_AT(U, k + 1, k - 1) = 0;
                //     AML_MAT2D_AT(U, k + 2, k - 1) = 0;
                // }


                // printf("-------right k = %zu ---------\n", k);
                // AML_PRINT(U);

                ala_apply_givens_2x2_right(Q, k+1, 0, n - 1, c1, s1);
                ala_apply_givens_2x2_right(Q, k, 0, n - 1, c2, s2);

                // aml_dprintDOUBLE(ala_schur_residual(H, Q, U));
                
                if (k + 2 < last) {
                    x = AML_MAT2D_AT(U, k + 1, k);
                    y = AML_MAT2D_AT(U, k + 2, k);
                    z = AML_MAT2D_AT(U, k + 3, k);
                }
            } else {
                aml_real c1 = 0, s1 = 0;
                ALA_ASSERT(k);
                x = AML_MAT2D_AT(U, k, k - 1);
                y = AML_MAT2D_AT(U, k + 1, k - 1);
                ala_givens_2x2_rotations_get_c_and_s(x, y, &c1, &s1);

                size_t begin = (k == first) ? first : (k - 1);
                ala_apply_givens_2x2_left(U, k, begin, n - 1, c1, s1);

                // AML_MAT2D_AT(U, k + 1, k - 1) = 0;

                // printf("-------left 2x2 k = %zu ---------\n", k);
                // AML_PRINT(U);

                ala_apply_givens_2x2_right(U, k, 0, n - 1, c1, s1);

                // printf("-------right 2x2 k = %zu ---------\n", k);
                // AML_PRINT(U);

                ala_apply_givens_2x2_right(Q, k, 0, n - 1, c1, s1);

                // aml_dprintDOUBLE(ala_schur_residual(H, Q, U));
            }
            
        }

        // aml_dprintDOUBLE(ala_schur_residual(H, Q, U));

        aml_make_upper_hessenberg_range(U, first, last);

        // printf("--------%zu-----------\n", i);
        // AML_PRINT(U);

        // if (i == 1) exit(1);
    }

    if (!converged) {
        aml_dprintWARNING("Did not converged after %zu iterations.", ALA_HESSENBERG_SCHUR_DECOMPOSITION_MAX_ITERATIONS_MULTIPLAYER * n);
    }
}

ALA_DEF void ala_hessenberg_QUQm1_schur_decomposition_householder(struct Aml_Mat2d Q, struct Aml_Mat2d U, struct Aml_Mat2d H, bool compute_Q)
{
    /* works better than the Given's rotations one. */
    /**
     * https://en.wikipedia.org/wiki/Schur_decomposition
     * https://www.mathworks.com/help/matlab/ref/schur.html
     */
    ALA_ASSERT(H.rows == H.cols);
    ALA_ASSERT(Q.rows == Q.cols);
    ALA_ASSERT(U.rows == U.cols);
    ALA_ASSERT(Q.rows == H.rows);
    ALA_ASSERT(U.rows == H.rows);
    ALA_ASSERT(H.rows > 2);
    ALA_ASSERT(aml_is_hessenberg(H));

    size_t n = H.rows;
    aml_copy(U, H);
    aml_set_identity(Q);
    struct Aml_Mat2d v3buf = aml_mat2d_alloc(3, 1);
    struct Aml_Mat2d x3buf = aml_mat2d_alloc(3, 1);
    struct Aml_Mat2d v2buf = aml_mat2d_alloc(2, 1);
    struct Aml_Mat2d x2buf = aml_mat2d_alloc(2, 1);


    bool converged = false;

    aml_dprintINFO("%s", "");
    printf("\33[A");
    for (size_t i = 0; i < ALA_HESSENBERG_SCHUR_DECOMPOSITION_MAX_ITERATIONS_MULTIPLAYER * n; i++) {
        printf("\n\33[A\33[2K\r       i: %zu", i);
        size_t last = 0, first = 0;
        if (ala_hessenberg_deflate_tail(U, &first, &last, aml_min(AML_EPS, (aml_real)1e-4))) {
            converged = true;
            printf("\n");
            aml_dprintINFO("converged on iteration %zu", i);
            break;
        }

        // aml_dprintINFO("first = %zu | last = %zu", first, last);
        
        aml_real trace = 0, det = 0;
        ala_hessenberg_calc_double_shift(U, last, &trace, &det);

        {
            aml_real u00 = AML_MAT2D_AT(U, first, first);
            aml_real u01 = AML_MAT2D_AT(U, first, first + 1);
            aml_real u10 = AML_MAT2D_AT(U, first + 1, first);
            aml_real u11 = AML_MAT2D_AT(U, first + 1, first + 1);
            aml_real u21 = AML_MAT2D_AT(U, first + 2, first + 1);

            aml_real x = u00 * u00 + u01 * u10 - trace * u00 + det;
            aml_real y = u10 * (u00 + u11 - trace);
            aml_real z = u10 * u21;

            AML_MAT2D_AT(x3buf, 0, 0) = x;
            AML_MAT2D_AT(x3buf, 1, 0) = y;
            AML_MAT2D_AT(x3buf, 2, 0) = z;

            ala_householder_top_element_vector_get(v3buf, x3buf);

            if (!AML_IS_ZERO(aml_inner_product(v3buf))) {
                /* initial step acts on first:first+2 */
                ala_apply_householder_left(U, first, first, v3buf);
                // printf("-------left first ---------\n");
                // AML_PRINT(U);

                ala_apply_householder_right(U, first, v3buf);
                // printf("-------right first ---------\n");
                // AML_PRINT(U);

                if (compute_Q) ala_apply_householder_right(Q, first, v3buf);
            }
        }

        /* chase the bulge with 3x3 reflectors */
        for (size_t k = first; k + 3 <= last; ++k) {
            AML_MAT2D_AT(x3buf, 0, 0) = AML_MAT2D_AT(U, k + 1, k);
            AML_MAT2D_AT(x3buf, 1, 0) = AML_MAT2D_AT(U, k + 2, k);
            AML_MAT2D_AT(x3buf, 2, 0) = AML_MAT2D_AT(U, k + 3, k);

            ala_householder_top_element_vector_get(v3buf, x3buf);

            if (AML_IS_ZERO(aml_inner_product(v3buf))) {
                continue;
            }

            ala_apply_householder_left(U, k + 1, k, v3buf);
            // printf("-------left k = %zu ---------\n", k);
            // AML_PRINT(U);
            
            ala_apply_householder_right(U, k + 1, v3buf);
            // printf("-------right k = %zu ---------\n", k);
            // AML_PRINT(U);
            
            if (compute_Q) ala_apply_householder_right(Q, k + 1, v3buf);

            AML_MAT2D_AT(U, k + 2, k) = 0;
            AML_MAT2D_AT(U, k + 3, k) = 0;
        }

        /* final 2x2 */
        if (last >= first + 2) {
            size_t k = last - 2;

            AML_MAT2D_AT(x2buf, 0, 0) = AML_MAT2D_AT(U, k + 1, k);
            AML_MAT2D_AT(x2buf, 1, 0) = AML_MAT2D_AT(U, k + 2, k);

            ala_householder_top_element_vector_get(v2buf, x2buf);

            if (!AML_IS_ZERO(aml_inner_product(v2buf))) {
                ala_apply_householder_left(U, k + 1, k, v2buf);
                // printf("-------left k 2x2 = %zu ---------\n", k);
                // AML_PRINT(U);

                ala_apply_householder_right(U, k + 1, v2buf);
                // printf("-------right k 2x2 = %zu ---------\n", k);
                // AML_PRINT(U);

                if (compute_Q) ala_apply_householder_right(Q, k + 1, v2buf);

                AML_MAT2D_AT(U, k + 2, k) = 0;
            }
        }

        // printf("--------%zu-----------\n", i);
        // AML_PRINT(U);


        aml_make_upper_hessenberg_range(U, first, last);

        // if (i == 1) exit(1);
    }
    printf("\n");

    if (!converged) {
        aml_dprintWARNING("Did not converged after %zu iterations.", ALA_HESSENBERG_SCHUR_DECOMPOSITION_MAX_ITERATIONS_MULTIPLAYER * n);
    }

    aml_mat2d_free(v3buf);
    aml_mat2d_free(x3buf);
    aml_mat2d_free(v2buf);
    aml_mat2d_free(x2buf);
}

ALA_DEF void ala_hessenberg_QUQm1_schur_decomposition_householder_fast(struct Aml_Mat2d Q, struct Aml_Mat2d U, struct Aml_Mat2d H, bool compute_Q)
{
    /** works better than the Given's rotations one.
     * This version is slightly faster by using specialized householder rotations
     */
    /**
     * https://en.wikipedia.org/wiki/Schur_decomposition
     * https://www.mathworks.com/help/matlab/ref/schur.html
     */
    ALA_ASSERT(H.rows == H.cols);
    ALA_ASSERT(Q.rows == Q.cols);
    ALA_ASSERT(U.rows == U.cols);
    ALA_ASSERT(Q.rows == H.rows);
    ALA_ASSERT(U.rows == H.rows);
    ALA_ASSERT(H.rows > 2);
    ALA_ASSERT(aml_is_hessenberg(H));

    size_t n = H.rows;
    aml_copy(U, H);
    aml_set_identity(Q);
    struct Aml_Mat2d v3buf = aml_mat2d_alloc(3, 1);
    struct Aml_Mat2d x3buf = aml_mat2d_alloc(3, 1);
    struct Aml_Mat2d v2buf = aml_mat2d_alloc(2, 1);
    struct Aml_Mat2d x2buf = aml_mat2d_alloc(2, 1);


    bool converged = false;

    aml_dprintINFO("%s", "");
    printf("\33[A");
    for (size_t i = 0; i < ALA_HESSENBERG_SCHUR_DECOMPOSITION_MAX_ITERATIONS_MULTIPLAYER * n; i++) {
        printf("\n\33[A\33[2K\r       i: %zu", i);
        size_t last = 0, first = 0;
        if (ala_hessenberg_deflate_tail(U, &first, &last, aml_min(AML_EPS, (aml_real)1e-4))) {
            converged = true;
            printf("\n");
            aml_dprintINFO("converged on iteration %zu", i);
            break;
        }

        // aml_dprintINFO("first = %zu | last = %zu", first, last);
        
        aml_real trace = 0, det = 0;
        ala_hessenberg_calc_double_shift(U, last, &trace, &det);

        {
            aml_real u00 = AML_MAT2D_AT(U, first, first);
            aml_real u01 = AML_MAT2D_AT(U, first, first + 1);
            aml_real u10 = AML_MAT2D_AT(U, first + 1, first);
            aml_real u11 = AML_MAT2D_AT(U, first + 1, first + 1);
            aml_real u21 = AML_MAT2D_AT(U, first + 2, first + 1);

            aml_real x = u00 * u00 + u01 * u10 - trace * u00 + det;
            aml_real y = u10 * (u00 + u11 - trace);
            aml_real z = u10 * u21;

            AML_MAT2D_AT(x3buf, 0, 0) = x;
            AML_MAT2D_AT(x3buf, 1, 0) = y;
            AML_MAT2D_AT(x3buf, 2, 0) = z;

            ala_householder_top_element_vector_get(v3buf, x3buf);

            if (!AML_IS_ZERO(aml_inner_product(v3buf))) {
                aml_real v0 = AML_MAT2D_AT(v3buf, 0, 0);
                aml_real v1 = AML_MAT2D_AT(v3buf, 1, 0);
                aml_real v2 = AML_MAT2D_AT(v3buf, 2, 0);
                aml_real beta = ala_householder_beta3(v0, v1, v2);

                ala_apply_householder_left3(U, first, first, n-1, v0, v1, v2, beta);
                // printf("-------left first ---------\n");
                // AML_PRINT(U);

                ala_apply_householder_right3(U, 0, aml_min(last, first + 3), first, v0, v1, v2, beta);
                // printf("-------right first ---------\n");
                // AML_PRINT(U);

                if (compute_Q) ala_apply_householder_right3(Q, 0, n - 1, first, v0, v1, v2, beta);
            }
        }

        /* chase the bulge with 3x3 reflectors */
        for (size_t k = first; k + 3 <= last; ++k) {
            AML_MAT2D_AT(x3buf, 0, 0) = AML_MAT2D_AT(U, k + 1, k);
            AML_MAT2D_AT(x3buf, 1, 0) = AML_MAT2D_AT(U, k + 2, k);
            AML_MAT2D_AT(x3buf, 2, 0) = AML_MAT2D_AT(U, k + 3, k);

            ala_householder_top_element_vector_get(v3buf, x3buf);

            aml_real v0 = AML_MAT2D_AT(v3buf, 0, 0);
            aml_real v1 = AML_MAT2D_AT(v3buf, 1, 0);
            aml_real v2 = AML_MAT2D_AT(v3buf, 2, 0);
            
            if (AML_IS_ZERO(aml_inner_product(v3buf))) {
                continue;
            }

            aml_real beta = ala_householder_beta3(v0, v1, v2);

            ala_apply_householder_left3(U, k + 1, k, n-1, v0, v1, v2, beta);
            // printf("-------left k = %zu ---------\n", k);
            // AML_PRINT(U);
            
            ala_apply_householder_right3(U, 0, aml_min(last, k + 4), k + 1, v0, v1, v2, beta);
            // printf("-------right k = %zu ---------\n", k);
            // AML_PRINT(U);
            
            if (compute_Q) ala_apply_householder_right3(Q, 0, n - 1, k + 1, v0, v1, v2, beta);

            // AML_MAT2D_AT(U, k + 2, k) = 0;
            // AML_MAT2D_AT(U, k + 3, k) = 0;
        }

        /* final 2x2 */
        if (last >= first + 2) {
            size_t k = last - 2;

            AML_MAT2D_AT(x2buf, 0, 0) = AML_MAT2D_AT(U, k + 1, k);
            AML_MAT2D_AT(x2buf, 1, 0) = AML_MAT2D_AT(U, k + 2, k);

            ala_householder_top_element_vector_get(v2buf, x2buf);

            if (!AML_IS_ZERO(aml_inner_product(v2buf))) {
                ala_apply_householder_left(U, k + 1, k, v2buf);
                // printf("-------left k 2x2 = %zu ---------\n", k);
                // AML_PRINT(U);

                ala_apply_householder_right(U, k + 1, v2buf);
                // printf("-------right k 2x2 = %zu ---------\n", k);
                // AML_PRINT(U);

                if (compute_Q) ala_apply_householder_right(Q, k + 1, v2buf);

                // AML_MAT2D_AT(U, k + 2, k) = 0;
            }
        }
        // printf("--------%zu-----------\n", i);
        // AML_PRINT(U);

        aml_make_upper_hessenberg_range(U, first, last);
    }
    printf("\n");

    if (!converged) {
        aml_dprintWARNING("Did not converged after %zu iterations.", ALA_HESSENBERG_SCHUR_DECOMPOSITION_MAX_ITERATIONS_MULTIPLAYER * n);
    }

    aml_mat2d_free(v3buf);
    aml_mat2d_free(x3buf);
    aml_mat2d_free(v2buf);
    aml_mat2d_free(x2buf);
}

ALA_DEF bool ala_hessenberg_real_schur_2x2(struct Aml_Mat2d Q, struct Aml_Mat2d U, size_t k)
{
    aml_real a = AML_MAT2D_AT(U, k, k);
    aml_real b = AML_MAT2D_AT(U, k, k + 1);
    aml_real c = AML_MAT2D_AT(U, k + 1, k);
    aml_real d = AML_MAT2D_AT(U, k + 1, k + 1);

    if (!ala_eigenvalues_real_2x2(a, b, c, d)) {
        return false;
    }

    aml_real disc = aml_sqrt((a - d) * (a - d) + 4 * b * c);
    aml_real lambda = ((a + d) + (a >= d ? disc : -disc)) * (aml_real)0.5;

    aml_real x = b;
    aml_real y = lambda - a;

    if (AML_IS_ZERO(x) && AML_IS_ZERO(y)) {
        x = lambda - d;
        y = c;
    }

    aml_real cs = 1;
    aml_real sn = 0;
    ala_givens_2x2_rotations_get_c_and_s(x, y, &cs, &sn);

    ala_apply_givens_2x2_left(U, k, k, U.cols - 1, cs, sn);
    ala_apply_givens_2x2_right(U, k, 0, U.rows - 1, cs, sn);
    ala_apply_givens_2x2_right(Q, k, 0, Q.rows - 1, cs, sn);

    AML_MAT2D_AT(U, k + 1, k) = 0;
    return true;
}

/**
 * @brief Build an explicit Householder matrix from a vector.
 *
 * Forms:
 * `H = I - 2 * (v v^T) / (v^T v)`.
 *
 * @param des Output square matrix.
 * @param v Householder vector.
 *
 * Complexity
 * `O(n^2)`.
 *
 * @note Useful for teaching and debugging, but applying explicit Householder
 * matrices is usually slower and less numerically attractive than applying the
 * reflector implicitly.
 */
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

/**
 * @brief Form the standard Householder vector that zeros entries below the top.
 *
 * Copies `x` into `v_des` and modifies the first entry by `±||x||`, choosing
 * the sign according to the implementation's branch on `x(0)` to avoid catastrophic cancellation.
 *
 * @param v_des Output vector.
 * @param x Input column vector.
 *
 * Complexity
 * `O(n)`.
 */
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

/**
 * @brief Invert a square matrix by Gauss-Jordan elimination.
 *
 * The implementation forms `[src | I]`, reduces it to RREF using ala_reduce(),
 * and copies the right block into @p des.
 *
 * @param des Output inverse matrix.
 * @param src Input square matrix.
 *
 * Complexity
 * `O(n^3)`.
 *
 * @warning No explicit singularity handling beyond what the reduction routine
 * naturally produces.
 */
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

/**
 * @brief Compute an LUP decomposition with row swaps when a pivot is zero.
 *
 * On return, `src` has been decomposed into permutation matrix `p`, unit-lower
 * triangular `l`, and upper triangular `u`.
 *
 * @param src Input matrix.
 * @param l Output lower-triangular factor with unit diagonal.
 * @param p Output permutation matrix.
 * @param u Output upper-triangular factor.
 *
 * Complexity
 * `O(n^3)` for square matrices.
 *
 * @note This implementation only searches for a pivot row when the current
 * diagonal pivot is numerically zero; it is not full partial pivoting at every
 * step.
 * @note Best as a simple/reference LU decomposition. For difficult numerical
 * problems, always-on pivoting would be better.
 */
ALA_DEF void ala_LUP_decomposition_with_swap(struct Aml_Mat2d src, struct Aml_Mat2d l, struct Aml_Mat2d p, struct Aml_Mat2d u)
{
    /* performing LU decomposition Following the Wikipedia page: https://en.wikipedia.org/wiki/LU_decomposition */
    /**
     * Rectangular LUP decomposition with partial pivoting:
     *
     *     P * A = L * U
     *
     * Shapes:
     *     A : m x n
     *     P : m x m
     *     L : m x m   (unit lower triangular)
     *     U : m x n   (upper trapezoidal)
     */
    ALA_ASSERT(p.rows == p.cols);
    ALA_ASSERT(p.rows == src.rows);
    ALA_ASSERT(l.rows == l.cols);
    ALA_ASSERT(l.rows == src.rows);
    ALA_ASSERT(u.rows == l.cols);
    ALA_ASSERT(u.cols == src.cols);

    aml_copy(u, src);
    aml_set_identity(p);
    aml_set_identity(l);

    aml_real scale = aml_calc_norma_inf(src);
    if (AML_IS_ZERO(scale)) return;

    aml_real tol = AML_EPS * scale;
    size_t steps = aml_min(src.rows, src.cols);
    for (size_t i = 0; i < steps; i++) {
        /* partial pivoting: find largest entry in column i from row i downward */
        size_t pivot_r = i;
        aml_real best = aml_fabs(AML_MAT2D_AT(u, i, i));
        for (size_t r = i + 1; r < u.rows; r++) {
            aml_real v = aml_fabs(AML_MAT2D_AT(u, r, i));
            if (v > best) {
                best = v;
                pivot_r = r;
            }
        }
        /* if whole pivot column below i is zero, skip this column */
        if (AML_IS_ZERO(best)) {
            for (size_t temp_r = i + 1; temp_r < u.rows; temp_r++) {
                AML_MAT2D_AT(u, temp_r, i) = 0;
            }
            continue;
        }
        if (pivot_r != i) {
            aml_rows_swap(u, i, pivot_r);
            aml_rows_swap(p, i, pivot_r);
            /* Swap only the already computed multipliers in L: columns [0, i) */
            for (size_t c = 0; c < i; c++) {
                aml_real tmp = AML_MAT2D_AT(l, i, c);
                AML_MAT2D_AT(l, i, c) = AML_MAT2D_AT(l, pivot_r, c);
                AML_MAT2D_AT(l, pivot_r, c) = tmp;
            }
        }
        aml_real pivot = AML_MAT2D_AT(u, i, i);
        for (size_t temp_r = i + 1; temp_r < u.rows; temp_r++) {
            aml_real a = AML_MAT2D_AT(u, temp_r, i);
            if (aml_fabs(a) <= tol) {
                AML_MAT2D_AT(u, temp_r, i) = 0;
                continue;
            }
            aml_real mult = a / pivot;
            AML_MAT2D_AT(l, temp_r, i) = mult;
            aml_sub_src_row_time_factor_from_des_row_range(u, temp_r, i, mult, i, u.cols-1);
            AML_MAT2D_AT(u, temp_r, i) = 0;
        }
    }
}

/**
 * @brief Experimental orthogonalization-like routine based on elimination.
 *
 * The implementation forms `A^T A`, builds the augmented matrix
 * `[A^T A | A^T]`, performs forward elimination with unit pivots, copies the
 * transformed right block, and transposes it into @p des.
 *
 * @param des Output matrix.
 * @param A Input matrix.
 *
 * Complexity
 * Roughly `O(A.rows * A.cols^2 + A.cols^3)`.
 *
 * @warning This is not a standard production orthogonalization routine. It is best treated as a reference/experimental
 * helper matching this specific implementation rather than as a canonical QR or
 * Gram-Schmidt replacement.
 */
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

    ala_upper_triangulate(temp, ALA_UPPER_TRIANGULATE_ONES_ON_DIAG);

    aml_copy_src_window_to_des(temp_des, temp, 0, ATA.cols, AT.rows-1, ATA.cols + AT.cols-1);

    aml_transpose(des, temp_des);

    AML_PRINT(temp);

    aml_mat2d_free(AT);
    aml_mat2d_free(ATA);
    aml_mat2d_free(temp);
    aml_mat2d_free(temp_des);
}

/**
 * @brief Build an orthogonal basis using modified Gram-Schmidt.
 *
 * The function:
 * - copies all non-zero columns of @p A into the first columns of @p des,
 * - fills the remaining columns of @p des with random values,
 * - runs a modified Gram-Schmidt process across all columns,
 * - normalizes each resulting column.
 *
 * @param des Output square matrix whose columns become an orthogonal basis.
 * @param A Input matrix whose non-zero columns are used first.
 *
 * Complexity
 * Roughly `O(des.cols^2 * des.rows)`, i.e. `O(n^3)` for square `n x n` output.
 *
 * @note Best when you want a simple full orthogonal basis completion from a set
 * of columns.
 * @note Compared with classical Gram-Schmidt, modified Gram-Schmidt is usually
 * more numerically stable and is a better default for practical
 * orthogonalization.
 * @warning The function asserts `des.rows == A.rows` and `des.cols == des.rows`,
 * so the output is always square.
 */
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

ALA_DEF bool ala_positive_definite_check(struct Aml_Mat2d A)
{
    ALA_ASSERT(A.rows == A.cols);
    ALA_ASSERT(A.rows >= 1);
    ALA_ASSERT(aml_is_symmetric(A));

    struct Aml_Mat2d R = aml_mat2d_alloc(A.rows, A.cols);

    bool res = true;
    if (!ala_positive_definite_RTR_Cholesky_decomposition(R, A)) {
        res = false;
        aml_dprintERROR("%s", "Matrix A failed the Cholesky decomposition hanse it is not positive definite.");
    }

    aml_mat2d_free(R);
    return res;
}

ALA_DEF bool ala_positive_definite_RTR_Cholesky_decomposition(struct Aml_Mat2d R, struct Aml_Mat2d A)
{
    /* This decomposition will succeed only when A is positive definite */
    /* This algorithm is taken from the Fundamentals of Matrix Computations book Pg.37 */

    ALA_ASSERT(A.rows == A.cols);
    ALA_ASSERT(R.rows == R.cols);
    ALA_ASSERT(R.rows == A.rows);
    ALA_ASSERT(A.rows >= 1);
    ALA_ASSERT(aml_is_symmetric(A));

    size_t n = A.rows;

    aml_fill(R, 0);

    for (size_t i = 0; i < n; i++) {
        aml_real a_ii = AML_MAT2D_AT(A, i, i);
        aml_real temp_r_ii = 0;
        for (size_t k = 0; k < i; k++) {
            temp_r_ii += AML_MAT2D_AT(R, k, i) * AML_MAT2D_AT(R, k, i);
        }
        if ((a_ii - temp_r_ii) < 0) {
            printf("\n");
            aml_dprintERROR("Matrix A is not positive definite. About to take the root of a negative number: A(%zu, %zu) - sum_k(R(k, %zu)^2) = %g.", i, i, i, a_ii - temp_r_ii);
            return false;
        }
        AML_MAT2D_AT(R, i, i) = aml_sqrt(a_ii - temp_r_ii);
        if (AML_IS_ZERO(AML_MAT2D_AT(R, i, i))) {
            printf("\n");
            aml_dprintERROR("Matrix A is not positive definite. R(%zu, %zu) = %g. Can't divide by zero.", i, i, AML_MAT2D_AT(R, i, i));
            return false;
        }
        for (size_t j = i + 1; j < n; j++) {
            aml_real a_ij = AML_MAT2D_AT(A, i, j);
            aml_real temp_r_ij = 0;
            for (size_t k = 0; k < i; k++) {
                temp_r_ij += AML_MAT2D_AT(R, k, i) * AML_MAT2D_AT(R, k, j);
            }
            AML_MAT2D_AT(R, i, j) = (a_ij - temp_r_ij) / AML_MAT2D_AT(R, i, i);
        }
    }

    return true;
}

/**
 * @brief Run power iteration on `A - shift * I`.
 *
 * The routine repeatedly multiplies the vector by the shifted matrix, normalizes
 * it, estimates the eigenvalue with a Rayleigh-like quotient, and stops when
 * the vector change becomes small up to sign.
 *
 * @param A Input square matrix.
 * @param v On entry: initial guess. On return: approximated eigenvector.
 * @param lambda Optional output eigenvalue estimate for the original matrix.
 * @param shift Spectral shift subtracted before iteration and added back to the
 * returned eigenvalue.
 * @param norm_inf_v If true, renormalize the output vector by the infinity norm.
 * @return `0` on apparent convergence, `1` if the iteration did not satisfy the
 * convergence test and appears to alternate.
 *
 * Complexity
 * `O(ALA_MAX_POWER_ITERATION * n^2)`.
 *
 * @note Best for the dominant eigenpair when that eigenvalue is well separated.
 * @note Shifts can help target different parts of the spectrum, but this is
 * still a basic power-iteration scheme.
 */
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
    for (i = 0; i < ALA_MAX_POWER_ITERATION; i++) {
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

/**
 * @brief Orthogonally project a vector away from previously used basis columns.
 *
 * Performs a simple Gram-Schmidt projection:
 * `v <- v - sum_c <v, b_c> b_c`
 * over columns `0 .. used_cols-1` of @p basis.
 *
 * @param v Vector modified in place.
 * @param basis Matrix whose leading columns are treated as basis vectors.
 * @param used_cols Number of basis columns to remove from @p v.
 *
 * Complexity
 * `O(v.rows * used_cols)`.
 *
 * @note This assumes the used basis columns are already orthonormal if you want
 * the projection to be mathematically exact in one pass.
 */
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

/**
 * @brief Compute a QR decomposition using explicit Householder matrices.
 *
 * This textbook-style implementation constructs each Householder matrix
 * explicitly and updates:
 * - `R <- H * R`
 * - `Q <- Q * H`
 *
 * @param Q Output orthogonal matrix.
 * @param R Output upper-triangular matrix.
 * @param src Input matrix.
 *
 * Complexity
 * Roughly `O(n^4)` for square matrices because full Householder matrices are
 * formed and multiplied explicitly.
 *
 * @note Best for learning, debugging, and verifying the algorithm step by step.
 * @note This explicit form is easier to inspect and often feels more
 * "mathematically correct" because it mirrors the derivation directly.
 * @warning It is slower and usually less practical than the fast implicit
 * version.
 */
ALA_DEF void ala_QR_decomposition_householder(struct Aml_Mat2d Q, struct Aml_Mat2d R, struct Aml_Mat2d src)
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

/**
 * @brief Compute a QR decomposition using implicit Householder applications.
 *
 * Instead of forming each Householder matrix explicitly, this version computes
 * the reflector vector and applies it directly to `R` and `Q`.
 *
 * @param Q Output orthogonal matrix.
 * @param R Output upper-triangular matrix.
 * @param src Input matrix.
 *
 * Complexity
 * Roughly `O(n^3)` for square matrices.
 *
 * @note This is the practical version to use for performance.
 * @note It is typically preferable to the explicit version for larger matrices
 * because it avoids forming dense reflector matrices.
 * @note The implementation updates `Q` by right-applying the reflectors, so the
 * returned `Q` matches the decomposition used by this code path.
 */
ALA_DEF void ala_QR_decomposition_householder_fast(struct Aml_Mat2d Q, struct Aml_Mat2d R, struct Aml_Mat2d src)
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

/**
 * @brief Reduce a matrix to reduced row echelon form.
 *
 * The function first performs forward elimination via ala_upper_triangulate()
 * using row swapping and unit pivots, then performs the backward elimination
 * phase to obtain RREF.
 *
 * @param m Matrix modified in place.
 * @return Rank estimate equal to the number of pivot rows found during the
 * backward pass.
 *
 * Complexity
 * `O(rows * cols * min(rows, cols))`, i.e. `O(n^3)` for square matrices.
 *
 * @note Best for solving small dense systems, computing rank, or educational
 * row-reduction tasks.
 */
ALA_DEF size_t ala_reduce(struct Aml_Mat2d m)
{
    /* preforming Gauss–Jordan reduction to Reduced Row Echelon Form (RREF) */
    /* Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */

    ala_upper_triangulate(m, ALA_UPPER_TRIANGULATE_ONES_ON_DIAG | ALA_UPPER_TRIANGULATE_ROW_SWAPPING);

    size_t rank = 0;

    for (int r = (int)m.rows-1; r >= 0; r--) {
        size_t c = m.cols-1;
        if (!aml_row_find_first_non_zero_value(m, r, &c)) {
            continue; /* row of zeros */
        }
        for (int i = 0; i < r; i++) {
            aml_real factor = AML_MAT2D_AT(m, i, c);
            aml_sub_src_row_time_factor_from_des_row(m, i, r, factor);
        }
        rank++;
    }

    return rank;
}

ALA_DEF aml_real ala_schur_residual(struct Aml_Mat2d H0, struct Aml_Mat2d Q, struct Aml_Mat2d U)
{
    struct Aml_Mat2d t1 = aml_mat2d_alloc(H0.rows, H0.cols);
    struct Aml_Mat2d t2 = aml_mat2d_alloc(H0.rows, H0.cols);

    aml_transpose(t1, Q);
    aml_dot(t2, t1, H0);
    aml_dot(t1, t2, Q);
    aml_sub(t1, U);

    aml_real r = aml_calc_norma(t1) / aml_calc_norma(H0);

    aml_mat2d_free(t1);
    aml_mat2d_free(t2);
    return r;
}

/**
 * @brief Solve a linear system using LUP decomposition.
 *
 * The implementation computes `P`, `L`, and `U`, explicitly inverts `L` and
 * `U`, and then evaluates:
 * - `x_tmp = P * B`
 * - `y = inv(L) * x_tmp`
 * - `x = inv(U) * y`
 *
 * @param A System matrix.
 * @param x Output solution vector.
 * @param B Right-hand side vector.
 *
 * Complexity
 * `O(n^3)`.
 *
 * @note This matches the implementation, but inverting `L` and `U` explicitly
 * is not the most efficient way to solve triangular systems.
 * @note Best as a simple reference implementation.
 */
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

/**
 * @brief Compute a full SVD by first computing the thin SVD and then completing
 * orthogonal bases.
 *
 * This function first calls ala_SVD_thin(), then expands `U` and `V` into full
 * orthogonal matrices using ala_make_orthogonal_modified_Gram_Schmidt().
 *
 * @param A Input matrix.
 * @param U Output full left singular-vector matrix.
 * @param S Output rectangular diagonal singular-value matrix.
 * @param V Output full right singular-vector matrix, or `V^T` if requested.
 * @param init_vec_u Initial vector for the left-side power iteration path.
 * @param init_vec_v Initial vector for the right-side power iteration path.
 * @param return_v_transpose If true, return `V^T` in @p V.
 *
 * Complexity
 * Dominated by repeated power iterations and basis completion; roughly
 * `O(k n^3)` for square problems, where `k` is the number of power-iteration
 * steps used internally.
 *
 * @note Best when you specifically need full square `U` and `V`.
 * @note Since this implementation is based on eigendecomposition plus basis
 * completion, it is more educational than industrial-strength.
 */
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

/**
 * @brief Compute an SVD using eigendecomposition of `A A^T` or `A^T A`.
 *
 * If `n <= m`, the function diagonalizes `A A^T` to get left singular vectors,
 * then computes right singular vectors from `A^T u_i / sigma_i`.
 *
 * If `n > m`, it diagonalizes `A^T A` to get right singular vectors, then
 * computes left singular vectors from `A v_i / sigma_i`.
 *
 * @param A Input matrix of size `n x m`.
 * @param U Output left singular-vector matrix buffer.
 * @param S Output singular-value matrix buffer.
 * @param V Output right singular-vector matrix buffer, or `V^T` if requested.
 * @param init_vec_u Initial vector for the `A A^T` eigenpath.
 * @param init_vec_v Initial vector for the `A^T A` eigenpath.
 * @param return_v_transpose If true, transpose `V` before returning it.
 *
 * Complexity
 * Dominated by the eigensolver: roughly `O(k n^3)` or `O(k m^3)` depending on
 * which side is diagonalized.
 *
 * @note Despite the name, the implementation expects full-size `U`, `S`, and
 * `V` buffers and fills the leading meaningful columns/diagonal entries.
 * @note Best for small to medium matrices when a simple SVD construction is
 * acceptable.
 * @warning Since this relies on repeated power iteration, accuracy and ordering
 * depend on the spectral structure and initialization.
 */
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

/**
 * @brief Compute eigenvalues and eigenvectors of a symmetric matrix by shifted
 * QR iteration.
 *
 * The algorithm repeatedly:
 * - chooses the last diagonal element as a shift,
 * - performs QR decomposition of `A - mu I`,
 * - forms the next iterate `R Q + mu I`,
 * - accumulates eigenvectors.
 *
 * @param A Input symmetric matrix.
 * @param eigenvalues Output matrix converging toward diagonal form.
 * @param eigenvectors Output orthogonal eigenvector matrix.
 *
 * Complexity
 * Roughly `O(iterations * n^3)`.
 *
 * @note Best for symmetric dense matrices when a direct QR eigensolver is
 * desired.
 * @note This avoids the tridiagonal reduction step, so it is conceptually
 * simple but slower than the tridiagonalized approach.
 */
ALA_DEF void ala_symmetric_eig_QR_shift(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors)
{
    /**
     * https://youtu.be/HIxSCrFX1Ls?list=PLfNiIduhuYeBfgM2kuZCofXxWHx_bkAr1
     */
    /** Left and Right eigenvectors
     * - right eigenvectors accumulate as V = V * Q_k
     * - left eigenvectors accumulate as W^T = Q_k^(-1) * W^T,
     *   or, if stored as columns, W = W * Q_k^(-1)^T
     * 
     * for orthogonal Q:
     *   W = W * Q_k
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
        ala_QR_decomposition_householder_fast(Q, R, eigenvalues);
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

/**
 * @brief Compute eigenpairs of a symmetric matrix by first tridiagonalizing it.
 *
 * The function:
 * - reduces `A` to `T = Q^T A Q` using Householder reflectors,
 * - solves the symmetric tridiagonal eigenproblem,
 * - maps eigenvectors back with `Q`.
 *
 * @param A Input symmetric matrix.
 * @param eigenvalues Output eigenvalue matrix.
 * @param eigenvectors Output eigenvector matrix.
 *
 * Complexity
 * `O(n^3)` overall.
 *
 * @note This is generally the preferred structure for symmetric eigensolvers:
 * dense-to-tridiagonal reduction first, then a specialized tridiagonal method.
 * @note In this implementation the tridiagonal solve is done by
 * ala_symmetric_tridiagonal_eig_QR_explicit_shift().
 */
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
    ala_symmetric_tridiagonal_cleanup(T, 0, T.cols-1);
    aml_dprintINFO("%s", "made tridiagonal");

    ala_symmetric_tridiagonal_eig_QR_shift(T, eigenvalues, semi_eigenvectors);

    aml_dot(eigenvectors, Q, semi_eigenvectors);

    aml_mat2d_free(Q);
    aml_mat2d_free(T);
    aml_mat2d_free(semi_eigenvectors);
}

ALA_DEF void ala_symmetric_eig_QR_tridiagonalize_implicit_shift(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors)
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
    ala_symmetric_tridiagonal_cleanup(T, 0, T.cols-1);
    aml_dprintINFO("%s", "made tridiagonal");

    ala_symmetric_tridiagonal_eig_QR_implicit_shift(T, eigenvalues, semi_eigenvectors);

    aml_dot(eigenvectors, Q, semi_eigenvectors);

    aml_mat2d_free(Q);
    aml_mat2d_free(T);
    aml_mat2d_free(semi_eigenvectors);
}

ALA_DEF void ala_symmetric_eigen_approximation(struct Aml_Mat2d approx, struct Aml_Mat2d A, size_t order)
{
    AML_ASSERT(order <= A.rows);
    AML_ASSERT(A.cols == A.rows);
    AML_ASSERT(approx.cols == approx.rows);
    AML_ASSERT(aml_is_symmetric(A));

    struct Aml_Mat2d eigenvalues  = aml_mat2d_alloc(A.rows, A.cols);
    struct Aml_Mat2d eigenvectors = aml_mat2d_alloc(A.rows, A.cols);

    ala_symmetric_eig_QR_tridiagonalize_implicit_shift(A, eigenvalues, eigenvectors);
    aml_dprintINT(ala_eig_check(A, eigenvalues, eigenvectors, approx));
    ala_symmetric_spectrum_info_print(ala_symmetric_spectrum_analyze(eigenvalues, AML_EPS));
    aml_make_diagonal(eigenvalues);
    ala_eigenpairs_sort(eigenvalues, eigenvectors);

    ala_symmetric_eigen_approximation_build(approx, eigenvalues, eigenvectors, order);

    aml_mat2d_free(eigenvalues);
    aml_mat2d_free(eigenvectors);
}

ALA_DEF void ala_symmetric_eigen_approximation_build(struct Aml_Mat2d approx, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors, size_t order)
{
    AML_ASSERT(order <= eigenvalues.rows);
    AML_ASSERT(eigenvalues.rows == eigenvalues.cols);
    AML_ASSERT(approx.rows == approx.cols);
    AML_ASSERT(eigenvectors.cols == eigenvectors.rows);
    AML_ASSERT(eigenvalues.rows == eigenvectors.rows);
    AML_ASSERT(eigenvalues.rows == approx.rows);

    struct Aml_Mat2d temp         = aml_mat2d_alloc(eigenvalues.rows, eigenvalues.cols);
    struct Aml_Mat2d n_values     = aml_mat2d_alloc(eigenvalues.rows, eigenvalues.cols);
    struct Aml_Mat2d n_vectors    = aml_mat2d_alloc(eigenvalues.rows, eigenvalues.cols);

    size_t n = order == 0 ? eigenvalues.rows : order;

    aml_fill(approx, 0);
    aml_fill(temp, 0);
    aml_fill(n_values, 0);
    aml_fill(n_vectors, 0);

    for (size_t i = 0; i < n; i++) {
        AML_MAT2D_AT(n_values, i, i) = AML_MAT2D_AT(eigenvalues, i, i);
        aml_copy_col_from_src_to_des(n_vectors, i, eigenvectors, i);
    }
    aml_transpose_inplace(n_vectors);
    aml_dot(temp, n_values, n_vectors);
    aml_transpose_inplace(n_vectors);
    aml_dot(approx, n_vectors, temp);

    aml_mat2d_free(temp);
    aml_mat2d_free(n_values);
    aml_mat2d_free(n_vectors);
}

ALA_DEF struct Ala_Symmetric_Spectrum_Info ala_symmetric_spectrum_analyze(struct Aml_Mat2d eigenvalues, aml_real rel_tol_multiplier)
{
    /** Warning
     * The use of eigenvalues to calculate the condition number is acceptable only for symmetric matrices.
     * The real definition of the condition number is the ratio between the largest and smallest singular value.
     */
    /** Explanation
     * The reported effective condition number estimates the conditioning of the matrix as a linear operator
     * on its numerically significant subspace. It is a matrix inversion / linear solve / pseudoinverse style condition number.
     */

    ALA_ASSERT(eigenvalues.rows == eigenvalues.cols);
    ALA_ASSERT(aml_is_diagonal(eigenvalues));

    struct Ala_Symmetric_Spectrum_Info info = {0};

    size_t n = eigenvalues.rows;

    info.max_abs_eigenvalue = 0;
    info.min_abs_eigenvalue = 0;
    info.min_abs_significant_eigenvalue = 0;
    info.tolerance = 0;
    info.reciprocal_condition_number = 0;
    info.effective_condition_number = HUGE_VAL;
    info.numerical_rank = 0;
    info.is_numerically_singular = true;

    if (n == 0) {
        return info;
    }

    aml_real max_abs = 0;
    aml_real min_abs = aml_fabs(AML_MAT2D_AT(eigenvalues, 0, 0));

    for (size_t i = 0; i < n; ++i) {
        aml_real a = aml_fabs(AML_MAT2D_AT(eigenvalues, i, i));
        if (a > max_abs) {
            max_abs = a;
        }
        if (a < min_abs) {
            min_abs = a;
        }
    }

    info.max_abs_eigenvalue = max_abs;
    info.min_abs_eigenvalue = min_abs;

    if (AML_IS_ZERO(max_abs)) {
        info.tolerance = 0;
        info.numerical_rank = 0;
        info.is_numerically_singular = true;
        info.reciprocal_condition_number = 0;
        info.effective_condition_number = HUGE_VAL;
        return info;
    }

    aml_real mult = rel_tol_multiplier > 0 ? rel_tol_multiplier : (aml_real)100;
    aml_real tol = mult * (aml_real)n * AML_EPS * max_abs;

    size_t rank = 0;
    aml_real min_sig = HUGE_VAL;

    for (size_t i = 0; i < n; ++i) {
        aml_real a = aml_fabs(AML_MAT2D_AT(eigenvalues, i, i));
        if (a > tol) {
            rank++;
            if (a < min_sig) {
                min_sig = a;
            }
        }
    }

    info.tolerance = tol;
    info.numerical_rank = rank;
    info.is_numerically_singular = rank < n;

    if (rank == 0) {
        info.min_abs_significant_eigenvalue = 0;
        info.reciprocal_condition_number = 0;
        info.effective_condition_number = HUGE_VAL;
        return info;
    }

    info.min_abs_significant_eigenvalue = min_sig;
    info.reciprocal_condition_number = min_sig / max_abs;
    info.effective_condition_number = max_abs / min_sig;

    return info;
}

ALA_DEF void ala_symmetric_spectrum_info_print(const struct Ala_Symmetric_Spectrum_Info info)
{
    printf("Ala_Symmetric_Spectrum_Info = {\n");
    printf("    max_abs_eigenvalue             = %g\n", (double)info.max_abs_eigenvalue);
    printf("    min_abs_eigenvalue             = %g\n", (double)info.min_abs_eigenvalue);
    printf("    min_abs_significant_eigenvalue = %g\n", (double)info.min_abs_significant_eigenvalue);
    printf("    tolerance                      = %g\n", (double)info.tolerance);
    printf("    reciprocal_condition_number    = %g\n", (double)info.reciprocal_condition_number);
    printf("    effective_condition_number     = %g\n", (double)info.effective_condition_number);
    printf("    numerical_rank                 = %zu\n", info.numerical_rank);
    printf("    is_numerically_singular        = %s\n", info.is_numerically_singular ? "true" : "false");
    printf("}\n");
}

/**
 * @brief Reduce a symmetric matrix to symmetric tridiagonal form using
 * Householder reflectors.
 *
 * On return, `T` contains the tridiagonalized matrix and `Q` accumulates the
 * orthogonal similarity transforms such that `T = Q^T * src * Q`.
 *
 * @param Q Output orthogonal accumulator.
 * @param T Output tridiagonal matrix.
 * @param src Input symmetric matrix.
 *
 * Complexity
 * `O(n^3)`.
 *
 * @note Best as the preprocessing stage for symmetric QR eigensolvers.
 */
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

/**
 * @brief Compute the Wilkinson-style shift for the trailing 2x2 block of a
 * symmetric tridiagonal matrix.
 *
 * The function uses the last two diagonal entries and the last subdiagonal
 * entry to compute a scalar shift.
 *
 * @param m Symmetric tridiagonal matrix.
 * @param last Last active index.
 * @return Shift value.
 *
 * Complexity
 * `O(1)`.
 *
 * @note Best used inside shifted QR on symmetric tridiagonal matrices.
 */
ALA_DEF aml_real ala_symmetric_tridiagonal_calc_shift(struct Aml_Mat2d m, size_t last)
{
    ALA_ASSERT(aml_is_symmetric_relative(m));
    ALA_ASSERT(aml_is_tridiagonal(m));
    ALA_ASSERT(m.cols == m.rows); 
    ALA_ASSERT(m.rows > 0); 

    aml_real e, d0, d1;

    e  = AML_MAT2D_AT(m, last, last - 1);
    d0 = AML_MAT2D_AT(m, last - 1, last - 1);
    d1 = AML_MAT2D_AT(m, last, last);

    aml_real delta = (d0 - d1) * (aml_real)0.5;
    int sign = delta >= 0 ? 1 : -1;
    aml_real shift = d1 - (aml_real)sign * e * e / (aml_fabs(delta) + aml_hypot(delta, e));

    return shift;
}

/**
 * @brief Restore exact tridiagonal symmetry on an active block.
 *
 * The function:
 * - zeros all entries outside the tridiagonal band in `[first, last]`,
 * - replaces each upper/lower off-diagonal pair by their average.
 *
 * @param T Matrix modified in place.
 * @param first First active index.
 * @param last Last active index.
 *
 * Complexity
 * `O((last - first + 1)^2)`.
 *
 * @note Best as a cleanup step after finite-precision updates that should have
 * preserved symmetric tridiagonal structure.
 */
ALA_DEF void ala_symmetric_tridiagonal_cleanup(struct Aml_Mat2d T, size_t first, size_t last)
{
    for (size_t i = first; i <= last; ++i) {
        for (size_t j = first; j <= last; ++j) {
            if (i + 1 < j || j + 1 < i) {
                AML_MAT2D_AT(T, i, j) = (aml_real)0;
            }
        }
    }

    for (size_t i = first; i < last; ++i) {
        aml_real e = ((aml_real)0.5) *
            (AML_MAT2D_AT(T, i, i + 1) + AML_MAT2D_AT(T, i + 1, i));
        AML_MAT2D_AT(T, i, i + 1) = e;
        AML_MAT2D_AT(T, i + 1, i) = e;
    }
} 

/**
 * @brief Deflate converged trailing entries of a symmetric tridiagonal matrix.
 *
 * Small subdiagonal entries are compared against a relative threshold and set
 * to zero. The function returns the active unreduced block as `[first, last]`.
 *
 * @param m Symmetric tridiagonal matrix modified in place.
 * @param first Optional output first active index.
 * @param last Optional output last active index.
 * @param eps Relative tolerance scale.
 * @return `true` if the whole matrix has deflated to 1x1 blocks.
 *
 * Complexity
 * `O(n)`.
 *
 * @note Best used between QR iterations to isolate the current active block.
 */
ALA_DEF bool ala_symmetric_tridiagonal_deflate_tail(struct Aml_Mat2d m, size_t *first, size_t *last, aml_real eps)
{
    ALA_ASSERT(m.rows == m.cols);
    ALA_ASSERT(m.rows > 0);

    size_t l = m.rows - 1;

    while (l > 0) {
        aml_real e = aml_fabs(AML_MAT2D_AT(m, l, l - 1));
        aml_real d0 = aml_fabs(AML_MAT2D_AT(m, l - 1, l - 1));
        aml_real d1 = aml_fabs(AML_MAT2D_AT(m, l, l));

        if (d1 < eps) {
            AML_MAT2D_AT(m, l, l - 1) = 0;
            AML_MAT2D_AT(m, l - 1, l) = 0;
            --l;
            continue;
        }

        if (e > eps * (d0 + d1)) {
            break;
        }

        AML_MAT2D_AT(m, l, l - 1) = 0;
        AML_MAT2D_AT(m, l - 1, l) = 0;
        --l;
    }

    if (l == 0) {
        if (first) *first = 0;
        if (last)  *last = 0;
        return true;
    }

    size_t f = 0;
    for (size_t k = l; k > 0; --k) {
        aml_real e = aml_fabs(AML_MAT2D_AT(m, k, k - 1));
        aml_real d0 = aml_fabs(AML_MAT2D_AT(m, k - 1, k - 1));
        aml_real d1 = aml_fabs(AML_MAT2D_AT(m, k, k));

        if (e <= eps * (d0 + d1)) {
            AML_MAT2D_AT(m, k, k - 1) = 0;
            AML_MAT2D_AT(m, k - 1, k) = 0;
            f = k;
            break;
        }
    }

    if (first) *first = f;
    if (last)  *last = l;
    return false;
}

/**
 * @brief Compute eigenpairs of a symmetric tridiagonal matrix by unshifted
 * explicit QR iteration.
 *
 * This version explicitly performs the QR step using stored Givens rotations:
 * first left-applying them to form `Q^T A`, then right-applying them to form
 * `R Q`, while accumulating eigenvectors.
 *
 * @param A Input symmetric tridiagonal matrix.
 * @param eigenvalues Output matrix converging toward diagonal form.
 * @param eigenvectors Output orthogonal eigenvector matrix.
 *
 * Complexity
 * Roughly `O(iterations * n^2)`.
 *
 * @note Best as the clearest reference implementation of QR on a tridiagonal
 * matrix.
 * @note Because it is explicit and unshifted, it is usually slower and can
 * converge more poorly than shifted variants.
 * @note This is the most transparent "textbook" form if you want to inspect
 * each QR step directly.
 */
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
            ala_givens_2x2_rotations_get_c_and_s(a11, a21, &c, &s);
            AML_MAT2D_AT(given_c_and_s, k, 0) = c;
            AML_MAT2D_AT(given_c_and_s, k, 1) = s;
            ala_apply_givens_2x2_left(eigenvalues, k, k, eigenvalues.cols-1, c, s);
            ala_apply_givens_2x2_right(eigenvectors, k, 0, eigenvectors.rows-1, c, s);
        }

        for (size_t k = 0; k < eigenvalues.rows - 1; ++k) {
            aml_real c = AML_MAT2D_AT(given_c_and_s, k, 0);
            aml_real s = AML_MAT2D_AT(given_c_and_s, k, 1);
            ala_apply_givens_2x2_right(eigenvalues, k, 0, eigenvalues.rows-1, c, s);
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

/**
 * @brief Compute eigenpairs of a symmetric tridiagonal matrix by shifted
 * explicit QR iteration.
 *
 * The algorithm:
 * - finds the current active trailing block,
 * - computes a shift,
 * - explicitly applies Givens rotations for the QR decomposition of the shifted
 * block,
 * - explicitly forms the next iterate,
 * - periodically cleans the tridiagonal structure.
 *
 * @param A Input symmetric tridiagonal matrix.
 * @param eigenvalues Output matrix converging toward diagonal form.
 * @param eigenvectors Output orthogonal eigenvector matrix.
 *
 * Complexity
 * Roughly `O(iterations * n^2)`.
 *
 * @note Best when you want a step-by-step explicit shifted QR algorithm that
 * closely follows the mathematical derivation.
 * @note In many educational/debug settings, this explicit version can produce a
 * more directly "correct-looking" result because every transform is exposed and
 * accumulated plainly.
 * @warning It is usually less stable and less efficient than a well-implemented
 * implicit shifted QR method.
 */
ALA_DEF void ala_symmetric_tridiagonal_eig_QR_shift(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors)
{
    /**
     * https://www.youtube.com/watch?v=_IgDCL7OPdU&list=PLfNiIduhuYeBfgM2kuZCofXxWHx_bkAr1&index=17&pp=iAQB
     */

    ALA_ASSERT(aml_is_symmetric_relative(A));
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

    // AML_PRINT(eigenvalues);

    bool converged = false;
    for (size_t i = 0; i < ALA_SYMMETRIC_TRIDIAGONAL_EIG_QR_MAX_ITERATIONS_MULTIPLAYER * A.rows; i++) {

        size_t last, first;
        if (ala_symmetric_tridiagonal_deflate_tail(eigenvalues, &first, &last, AML_EPS)) {
            converged = true;
            aml_dprintINFO("converged on iteration %zu", i);
            break;
        }
        
        aml_real shift = ala_symmetric_tridiagonal_calc_shift(eigenvalues, last);
        aml_shift_specific(eigenvalues, - shift, first, last);

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
            ala_givens_2x2_rotations_get_c_and_s(a11, a21, &c, &s);
            AML_MAT2D_AT(given_c_and_s, k, 0) = c;
            AML_MAT2D_AT(given_c_and_s, k, 1) = s;
            ala_apply_givens_2x2_left(eigenvalues, k, k, last, c, s);
            // printf("-------left---------\n");
            // AML_PRINT(eigenvalues);
            AML_MAT2D_AT(eigenvalues, k+1, k) = 0;
            ala_apply_givens_2x2_right(eigenvectors, k, 0, eigenvectors.rows-1, c, s);
        }


        for (size_t k = first; k < last; ++k) {
            aml_real c = AML_MAT2D_AT(given_c_and_s, k, 0);
            aml_real s = AML_MAT2D_AT(given_c_and_s, k, 1);
            ala_apply_givens_2x2_right(eigenvalues, k, first, k + 1, c, s);
            // printf("-------right--------\n");
            // AML_PRINT(eigenvalues);
        }

        aml_shift_specific(eigenvalues, shift, first, last);

        if (!(i % 10)) ala_symmetric_tridiagonal_cleanup(eigenvalues, first, last);

        // AML_PRINT(eigenvalues);
        // printf("---------%zu----------\n", i);
    }

    if (!converged) {
        aml_dprintWARNING("Did not converged after %zu iterations.", ALA_SYMMETRIC_TRIDIAGONAL_EIG_QR_MAX_ITERATIONS_MULTIPLAYER * A.rows);
    }


    aml_mat2d_free(given_c_and_s);
}

/**
 * @brief Compute eigenpairs of a symmetric tridiagonal matrix by implicit
 * shifted QR iteration with bulge chasing.
 *
 * This routine computes a shift from the trailing block, introduces the bulge
 * at the leading edge of the active block, and chases it down the matrix using
 * Givens rotations while accumulating eigenvectors.
 *
 * @param A Input symmetric tridiagonal matrix.
 * @param eigenvalues Output matrix converging toward diagonal form.
 * @param eigenvectors Output orthogonal eigenvector matrix.
 *
 * Complexity
 * Roughly `O(iterations * n^2)`.
 *
 * @note This is the flavor that is normally preferred in practical symmetric
 * tridiagonal QR eigensolvers because it preserves structure efficiently.
 * @note It is generally the more efficient and more sophisticated method than
 * the explicit shifted version.
 * @warning The implementation itself contains a comment noting convergence
 * problems on large matrices, so in this codebase the explicit shifted version
 * may currently give more trustworthy results for some cases. The problem is that for high precision,
 * the implicit method takes too long to converge. The bulge is too small.
 * For fast convergence you can relax the accuracy requirements by multiplying the epsilon.
 */
ALA_DEF void ala_symmetric_tridiagonal_eig_QR_implicit_shift(struct Aml_Mat2d A, struct Aml_Mat2d eigenvalues, struct Aml_Mat2d eigenvectors)
{
    /** TODO:
     * There is a convergence problem!!!!! I don't converge for large matrices. Check!
     */
    /**
     * https://youtu.be/RSm_Mqi0aSA?list=PLfNiIduhuYeBfgM2kuZCofXxWHx_bkAr1
     */
    ALA_ASSERT(aml_is_symmetric_relative(A));
    ALA_ASSERT(aml_is_tridiagonal(A));
    ALA_ASSERT(A.rows >= 2);
    ALA_ASSERT(A.cols == A.rows); 
    ALA_ASSERT(eigenvalues.cols == A.cols);
    ALA_ASSERT(eigenvalues.rows == A.rows);
    ALA_ASSERT(eigenvectors.cols == A.cols);
    ALA_ASSERT(eigenvectors.rows == A.rows);

    aml_copy(eigenvalues, A);
    aml_set_identity(eigenvectors);

    bool converged = false;

    aml_dprintINFO("%s", "");
    printf("\33[A");
    for (size_t i = 0; i < ALA_SYMMETRIC_TRIDIAGONAL_EIG_QR_IMPLICIT_MAX_ITERATIONS_MULTIPLAYER * eigenvalues.rows; i++) {
        printf("\n\33[A\33[2K\r       i: %zu", i);
        size_t last, first;
        if (ala_symmetric_tridiagonal_deflate_tail(eigenvalues, &first, &last, aml_min(AML_EPS * 100, (aml_real)1e-4))) {
            converged = true;
            printf("\n");
            aml_dprintINFO("converged on iteration %zu", i);
            break;
        }

        aml_real shift = ala_symmetric_tridiagonal_calc_shift(eigenvalues, last);

        aml_real x = AML_MAT2D_AT(eigenvalues, first, first) - shift;
        aml_real z = AML_MAT2D_AT(eigenvalues, first + 1, first);

        for (size_t k = first; k < last; ++k) {
            aml_real c = 0;
            aml_real s = 0;
            ala_givens_2x2_rotations_get_c_and_s(x, z, &c, &s);

            size_t begin = (k == first) ? first : (k - 1);
            size_t end = aml_min(k + 2, last);
            ala_apply_givens_2x2_left(eigenvalues, k, begin, end, c, s);
            if (k > first) {
                AML_MAT2D_AT(eigenvalues, k + 1, k - 1) = 0;
            }
            // printf("-------left---------\n");
            // AML_PRINT(eigenvalues);
            ala_apply_givens_2x2_right(eigenvalues, k, begin, end, c, s);
            if (k > first) {
                AML_MAT2D_AT(eigenvalues, k - 1, k + 1) = 0;
            }
            // printf("-------right--------\n");
            // AML_PRINT(eigenvalues);
            ala_apply_givens_2x2_right(eigenvectors, k, 0, eigenvectors.rows - 1, c, s);

            
            if (k + 1 < last) {
                x = AML_MAT2D_AT(eigenvalues, k + 1, k);
                z = AML_MAT2D_AT(eigenvalues, k + 2, k);
            }
        }

        #if 0
        if (!aml_is_symmetric_relative(eigenvalues)) {
            aml_dprintERROR("%s in iteration %zu", "lost symmetry", i);
            // break;
        }
        if (!aml_is_tridiagonal(eigenvalues)) {
            aml_dprintERROR("%s in iteration %zu", "lost tridiagonal structure", i);
            // break;
        }
        #ifdef AML_SINGLE_PRECISION
        if (!(i % 5)) ala_symmetric_tridiagonal_cleanup(eigenvalues, first, last);
        #else
        if (!(i % 10)) ala_symmetric_tridiagonal_cleanup(eigenvalues, first, last);
        #endif
        #else
        ala_symmetric_tridiagonal_cleanup(eigenvalues, first, last);
        #endif

        // printf("--------%zu-----------\n", i);
        // printf("shitf: %g\n", shift);
        // AML_PRINT(eigenvalues);

        // if (i == 10) exit(1);
    }
    printf("\n");

    if (!converged) {
        aml_dprintWARNING("Did not converged after %zu iterations.", ALA_SYMMETRIC_TRIDIAGONAL_EIG_QR_IMPLICIT_MAX_ITERATIONS_MULTIPLAYER * eigenvalues.rows);
    }

}

/**
 * @brief Compute a QR decomposition specialized for symmetric tridiagonal
 * matrices.
 *
 * The function annihilates each subdiagonal entry with a Givens rotation,
 * updates the matrix from the left, zeros the eliminated entry explicitly, and
 * accumulates the orthogonal factor.
 *
 * @param Q Output orthogonal matrix.
 * @param R Output upper-triangular matrix.
 * @param src Input symmetric tridiagonal matrix.
 *
 * Complexity
 * `O(n^2)`.
 *
 * @note Best as a building block for explicit QR iteration on tridiagonal
 * matrices.
 */
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
        ala_givens_2x2_rotations_get_c_and_s(a11, a21, &c, &s);

        ala_apply_givens_2x2_left(R, k, k, R.rows-1, c, s);
        AML_MAT2D_AT(R, k + 1, k) = 0;

        ala_apply_givens_2x2_right(Q, k, 0, Q.cols-1, c, s);
    }
}

/**
 * @brief Perform forward Gaussian elimination to upper-triangular form.
 *
 * The function scans pivot columns from left to right and pivot rows from top to
 * bottom. Depending on @p flags, it can:
 * - swap rows using partial pivot search,
 * - normalize pivot rows to put ones on the diagonal.
 *
 * It returns the cumulative determinant scaling introduced by row swaps and
 * optional row normalization.
 *
 * @param m Matrix modified in place.
 * @param flags Bitwise OR of enum aml_upper_triangulate_flag values.
 * @return Factor by which the determinant must be corrected after elimination.
 *
 * Complexity
 * `O(rows * cols * min(rows, cols))`, i.e. `O(n^3)` for square matrices.
 *
 * @note Best as a reusable elimination primitive for determinant, RREF, and
 * related routines.
 * @note With `AML_ROW_SWAPPING`, the routine uses partial pivoting by choosing
 * the largest absolute entry in the active column.
 */
ALA_DEF aml_real ala_upper_triangulate(struct Aml_Mat2d m, enum Ala_Upper_Triangulate_Flag flags)
{
    /* preforming Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */
    /* returns the factor multiplying the determinant */

    aml_real factor_to_return = 1;

    size_t r = 0;
    for (size_t c = 0; c < m.cols && r < m.rows; c++) {
        if (flags & ALA_UPPER_TRIANGULATE_ROW_SWAPPING) {
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
                aml_rows_swap(m, piv, r);
                factor_to_return *= -(aml_real)1;
            }
        }

        aml_real pivot = AML_MAT2D_AT(m, r, c);
        AML_ASSERT(!AML_IS_ZERO(pivot));

        if (flags & ALA_UPPER_TRIANGULATE_ONES_ON_DIAG) {
            aml_mult_row(m, r, (aml_real)1 / pivot);
            factor_to_return *= pivot;
            pivot = (aml_real)1;
        }

        /* Eliminate entries below pivot in column c */
        for (size_t i = r + 1; i < m.rows; i++) {
            aml_real f = AML_MAT2D_AT(m, i, c) / pivot;
            aml_sub_src_row_time_factor_from_des_row(m, i, r, f);
        }
        r++;
    }
    return factor_to_return;
}




#endif // ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION

