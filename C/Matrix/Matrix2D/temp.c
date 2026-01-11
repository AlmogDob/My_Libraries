#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"

void mat2D_SVD(Mat2D A, Mat2D U, Mat2D S, Mat2D V, Mat2D init_vec_u, Mat2D init_vec_v)
{
    /* https://www.youtube.com/watch?v=nbBvuuNVfco */
    MAT2D_ASSERT(U.rows == A.rows);
    MAT2D_ASSERT(U.cols == A.rows);
    MAT2D_ASSERT(S.rows == A.rows);
    MAT2D_ASSERT(S.cols == A.cols);
    MAT2D_ASSERT(V.rows == A.cols);
    MAT2D_ASSERT(V.cols == A.cols);

    Mat2D AT = mat2D_alloc(A.cols, A.rows);
    Mat2D ATA = mat2D_alloc(A.cols, A.cols);
    Mat2D AAT = mat2D_alloc(A.rows, A.rows);
    Mat2D left_eigenvalues = mat2D_alloc(AAT.rows, AAT.cols);
    Mat2D left_eigenvectors = mat2D_alloc(AAT.rows, AAT.cols);
    Mat2D right_eigenvalues = mat2D_alloc(ATA.rows, ATA.cols);
    Mat2D right_eigenvectors = mat2D_alloc(ATA.rows, ATA.cols);

    mat2D_transpose(AT, A);
    mat2D_dot(ATA, AT, A);
    mat2D_dot(AAT, A, AT);

    mat2D_eig_power_iteration(AAT, left_eigenvalues, left_eigenvectors, init_vec_u, 0);
    mat2D_eig_power_iteration(ATA, right_eigenvalues, right_eigenvectors, init_vec_v, 0);

    mat2D_copy(U, left_eigenvectors);
    mat2D_transpose(V, right_eigenvectors);
    mat2D_fill(S, 0);

    if (AAT.rows < ATA.rows) {
        for (size_t i = 0; i < AAT.rows; i++) {
            MAT2D_AT(S, i, i) = sqrt(MAT2D_AT(left_eigenvalues, i, i));
        }
    } else {
        for (size_t i = 0; i < ATA.rows; i++) {
            MAT2D_AT(S, i, i) = sqrt(MAT2D_AT(right_eigenvalues, i, i));
        }
    }

    mat2D_free(AT);
    mat2D_free(ATA);
    mat2D_free(AAT);
    mat2D_free(left_eigenvalues);
    mat2D_free(left_eigenvectors);
    mat2D_free(right_eigenvalues);
    mat2D_free(right_eigenvectors);
}

int main(void)
{
    int n = 3;
    int m = 2;

    Mat2D A = mat2D_alloc(n, m);
    Mat2D U = mat2D_alloc(n, n);
    Mat2D S = mat2D_alloc(n, m);
    Mat2D V = mat2D_alloc(m, m);
    Mat2D VT = mat2D_alloc(m, m);
    Mat2D SV = mat2D_alloc(n, m);
    Mat2D USV = mat2D_alloc(n, m);
    Mat2D init_vec_u = mat2D_alloc(U.rows, 1);
    Mat2D init_vec_v = mat2D_alloc(V.rows, 1);

    mat2D_rand(init_vec_u, 0, 1);
    mat2D_rand(init_vec_v, 0, 1);

    MAT2D_AT(A, 0, 0) = 3;
    MAT2D_AT(A, 0, 1) = 2;
    MAT2D_AT(A, 1, 0) = 2;
    MAT2D_AT(A, 1, 1) = 3;
    MAT2D_AT(A, 2, 0) = 2;
    MAT2D_AT(A, 2, 1) = -2;


    mat2D_SVD(A, U, S, V, init_vec_u, init_vec_v);

    MAT2D_PRINT(A);
    MAT2D_PRINT(U);
    MAT2D_PRINT(S);
    MAT2D_PRINT(V);

    mat2D_transpose(VT, V);

    mat2D_dot(SV, S, VT);
    mat2D_dot(USV, U, SV);

    MAT2D_PRINT(USV);
    

    mat2D_free(A);
    mat2D_free(U);
    mat2D_free(S);
    mat2D_free(V);
    mat2D_free(SV);
    mat2D_free(USV);
    mat2D_free(init_vec_u);
    mat2D_free(init_vec_v);

    return 0;
}
