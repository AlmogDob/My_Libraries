#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"

void mat2D_SVD_thin(Mat2D A, Mat2D U, Mat2D S, Mat2D V, Mat2D init_vec_u, Mat2D init_vec_v)
{
    /* https://www.youtube.com/watch?v=nbBvuuNVfco */
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
    mat2D_free(AT);
}

int main(void)
{
    int n = 4;
    int m = 5;

    Mat2D A = mat2D_alloc(n, m);
    Mat2D U = mat2D_alloc(n, n);
    Mat2D S = mat2D_alloc(n, m);
    Mat2D V = mat2D_alloc(m, m);
    Mat2D VT = mat2D_alloc(m, m);
    Mat2D SV = mat2D_alloc(n, m);
    Mat2D USVT = mat2D_alloc(n, m);
    Mat2D init_vec_u = mat2D_alloc(U.rows, 1);
    Mat2D init_vec_v = mat2D_alloc(V.rows, 1);

    mat2D_rand(init_vec_u, 0, 1);
    mat2D_rand(init_vec_v, 0, 1);

    mat2D_fill(A, 0);

    MAT2D_AT(A, 0, 0) = 1;
    MAT2D_AT(A, 0, 4) = 2;
    MAT2D_AT(A, 1, 2) = 3;
    MAT2D_AT(A, 3, 1) = 2;


    mat2D_SVD_thin(A, U, S, V, init_vec_u, init_vec_v);

    MAT2D_PRINT(A);
    MAT2D_PRINT(U);
    MAT2D_PRINT(S);
    MAT2D_PRINT(V);

    mat2D_transpose(VT, V);

    mat2D_dot(SV, S, VT);
    mat2D_dot(USVT, U, SV);

    MAT2D_PRINT(USVT);
    

    mat2D_free(A);
    mat2D_free(U);
    mat2D_free(S);
    mat2D_free(V);
    mat2D_free(SV);
    mat2D_free(USVT);
    mat2D_free(init_vec_u);
    mat2D_free(init_vec_v);

    return 0;
}
