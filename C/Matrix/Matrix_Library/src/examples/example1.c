#define MATRIX2D_IMPLEMENTATION
#include "../Matrix2D.h"

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

    mat2D_SVD_thin(A, U, S, V, init_vec_u, init_vec_v, 0);

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
