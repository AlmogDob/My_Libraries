#include "time.h"
#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"

int main(void)
{
    Mat2D a     = mat2D_alloc(3, 3);
    Mat2D b     = mat2D_alloc(3, 1);
    Mat2D x     = mat2D_alloc(3, 1);

    // srand(time(0));
    // mat2D_rand(a, 0, 1);
    MAT2D_AT(a, 0, 0) =  1;
    MAT2D_AT(a, 0, 1) =  3;
    MAT2D_AT(a, 0, 2) = -2;

    MAT2D_AT(a, 1, 0) =  3;
    MAT2D_AT(a, 1, 1) =  5;
    MAT2D_AT(a, 1, 2) =  6;

    MAT2D_AT(a, 2, 0) =  2;
    MAT2D_AT(a, 2, 1) =  4;
    MAT2D_AT(a, 2, 2) =  3;

    MAT2D_AT(b, 0, 0) =  5;
    MAT2D_AT(b, 1, 0) =  7;
    MAT2D_AT(b, 2, 0) =  8;

    MAT2D_PRINT(a);
    MAT2D_PRINT(b);

    mat2D_solve_linear_sys_LUP_decomposition(a, x, b);
    MAT2D_PRINT(x);

    mat2D_free(a);
    mat2D_free(b);
    mat2D_free(x);

    return 0;
}
