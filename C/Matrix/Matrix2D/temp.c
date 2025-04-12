#include "time.h"
#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"

int main(void)
{
    Mat2D m  = mat2D_alloc(5, 5);
    Mat2D l  = mat2D_alloc(5, 5);
    Mat2D p  = mat2D_alloc(5, 5);
    Mat2D u  = mat2D_alloc(5, 5);
    Mat2D lu = mat2D_alloc(5, 5);
    Mat2D pa = mat2D_alloc(5, 5);

    srand(time(0));
    mat2D_rand(m, 0, 1);

    mat2D_LU_decomposition_with_swap(m, l, p, u);
    mat2D_dot(pa, p, m);
    mat2D_dot(lu, l, u);

    MAT2D_PRINT(m);
    MAT2D_PRINT(u);
    MAT2D_PRINT(p);
    MAT2D_PRINT(l);
    MAT2D_PRINT(lu);
    MAT2D_PRINT(pa);

    mat2D_free(m);
    mat2D_free(l);
    mat2D_free(p);
    mat2D_free(u);
    mat2D_free(lu);
    mat2D_free(pa);

    return 0;
}
