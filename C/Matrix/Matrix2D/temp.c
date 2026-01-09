#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"

int main(void)
{
    int n = 3;
    Mat2D src = mat2D_alloc(n, n);   
    Mat2D des = mat2D_alloc(n, n);   

    mat2D_rand(src, 0, 1);
    // MAT2D_AT(src, 0, 0) = 2;
    // MAT2D_AT(src, 1, 0) = 0;
    // MAT2D_AT(src, 2, 0) = 1;
    // MAT2D_AT(src, 0, 1) = 1;
    // MAT2D_AT(src, 1, 1) = 2;
    // MAT2D_AT(src, 2, 1) = 1;
    // MAT2D_AT(src, 0, 2) = 3;
    // MAT2D_AT(src, 1, 2) = 4;
    // MAT2D_AT(src, 2, 2) = 2;

    MAT2D_PRINT(src); 

    mat2D_invert(des, src);

    MAT2D_PRINT(des); 



    mat2D_free(src);
    mat2D_free(des);

    return 0;
}
