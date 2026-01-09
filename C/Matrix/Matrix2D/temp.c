#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"

int main(void)
{
    int n = 3;
    Mat2D A = mat2D_alloc(n, n);   

    MAT2D_AT(A, 0, 0) = 2;
    MAT2D_AT(A, 1, 0) = 0;
    MAT2D_AT(A, 2, 0) = 0;
    MAT2D_AT(A, 0, 1) = 0;
    MAT2D_AT(A, 1, 1) = 3;
    MAT2D_AT(A, 2, 1) = 4;
    MAT2D_AT(A, 0, 2) = 0;
    MAT2D_AT(A, 1, 2) = 4;
    MAT2D_AT(A, 2, 2) = 9;

    MAT2D_PRINT(A); 

    mat2D_free(A);

    return 0;
}
