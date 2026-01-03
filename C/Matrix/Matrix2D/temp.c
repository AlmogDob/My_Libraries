#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"

int main(void)
{
    Mat2D m = mat2D_alloc(5, 4);   

    mat2D_rand(m, 0, 1);

    MAT2D_PRINT(m);

    mat2D_upper_triangulate(m);

    MAT2D_PRINT(m);


    mat2D_free(m);

    return 0;
}
