#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"

int main(void)
{
    Mat2D a = mat2D_alloc(4, 4);
    Mat2D b = mat2D_alloc(3, 3);
    Mat2D m = mat2D_alloc(4 - 3 + 1, 4 - 3 + 1);

    MAT2D_AT(a, 0, 0) = 3;
    MAT2D_AT(a, 0, 1) = 3;
    MAT2D_AT(a, 0, 2) = 2;
    MAT2D_AT(a, 0, 3) = 1;
    MAT2D_AT(a, 1, 0) = 0;
    MAT2D_AT(a, 1, 1) = 0;
    MAT2D_AT(a, 1, 2) = 1;
    MAT2D_AT(a, 1, 3) = 3;
    MAT2D_AT(a, 2, 0) = 3;
    MAT2D_AT(a, 2, 1) = 1;
    MAT2D_AT(a, 2, 2) = 2;
    MAT2D_AT(a, 2, 3) = 2;
    MAT2D_AT(a, 3, 0) = 2;
    MAT2D_AT(a, 3, 1) = 0;
    MAT2D_AT(a, 3, 2) = 0;
    MAT2D_AT(a, 3, 3) = 2;

    MAT2D_AT(b, 0, 0) = 0;
    MAT2D_AT(b, 0, 1) = 1;
    MAT2D_AT(b, 0, 2) = 2;
    MAT2D_AT(b, 1, 0) = 2;
    MAT2D_AT(b, 1, 1) = 2;
    MAT2D_AT(b, 1, 2) = 0;
    MAT2D_AT(b, 2, 0) = 0;
    MAT2D_AT(b, 2, 1) = 1;
    MAT2D_AT(b, 2, 2) = 2;

    mat2D_rotate_mat_180_deg_inplace(b);

    MAT2D_PRINT(a);
    MAT2D_PRINT(b);

    mat2D_convolve(m, a, b);

    MAT2D_PRINT(m);
    

    return 0;
}
