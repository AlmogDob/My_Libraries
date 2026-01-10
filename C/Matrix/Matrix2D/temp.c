#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"
#include <time.h>

int main(void)
{
    srand(time(0));

    int n = 5;
    Mat2D A = mat2D_alloc(n, n);   
    Mat2D res = mat2D_alloc(n, n);   
    Mat2D eigenvalues = mat2D_alloc(n, n);   
    Mat2D eigenvectors = mat2D_alloc(n, n);   
    Mat2D v = mat2D_alloc(n, 1);   

    MAT2D_AT(v, 0, 0) = 23;
    MAT2D_AT(v, 1, 0) = 13;
    MAT2D_AT(v, 2, 0) = -13;
    MAT2D_AT(v, 3, 0) = -31;
    MAT2D_AT(v, 4, 0) = 3;

    mat2D_rand(v, 0, 1);

    MAT2D_AT(A, 0, 0) = 14;
    MAT2D_AT(A, 1, 0) = -6;
    MAT2D_AT(A, 2, 0) = 14;
    MAT2D_AT(A, 3, 0) = 6;
    MAT2D_AT(A, 4, 0) = 8;

    MAT2D_AT(A, 0, 1) = -6;
    MAT2D_AT(A, 1, 1) = -30;
    MAT2D_AT(A, 2, 1) = 6;
    MAT2D_AT(A, 3, 1) = -30;
    MAT2D_AT(A, 4, 1) = 0;

    MAT2D_AT(A, 0, 2) = 14;
    MAT2D_AT(A, 1, 2) = 6;
    MAT2D_AT(A, 2, 2) = 14;
    MAT2D_AT(A, 3, 2) = -6;
    MAT2D_AT(A, 4, 2) = 8;
    
    MAT2D_AT(A, 0, 3) = 6;
    MAT2D_AT(A, 1, 3) = -30;
    MAT2D_AT(A, 2, 3) = -6;
    MAT2D_AT(A, 3, 3) = -30;
    MAT2D_AT(A, 4, 3) = 0;

    MAT2D_AT(A, 0, 4) = 8;
    MAT2D_AT(A, 1, 4) = 0;
    MAT2D_AT(A, 2, 4) = 8;
    MAT2D_AT(A, 3, 4) = 0;
    MAT2D_AT(A, 4, 4) = 20;

    MAT2D_PRINT(v);
    MAT2D_PRINT(A);

    mat2D_eig_power_iteration(A, eigenvalues, eigenvectors, v, 1);

    MAT2D_PRINT(eigenvalues);
    MAT2D_PRINT(eigenvectors);

    mat2D_eig_check(A, eigenvalues, eigenvectors, res);

    MAT2D_PRINT(res);


    mat2D_dprintDOUBLE(mat2D_det(eigenvectors));



    mat2D_free(A);
    mat2D_free(res);
    mat2D_free(eigenvalues);
    mat2D_free(eigenvectors);
    mat2D_free(v);

    return 0;
}
