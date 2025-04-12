#include "time.h"
#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"

int main(void)
{
    Mat2D m = mat2D_alloc(5, 5);
    Mat2D inv_m = mat2D_alloc(5, 5);

    // mat2D_fill_sequence(m, 0, 1);
    // mat2D_rand(m, 0, 10);
    mat2D_set_identity(m);
    mat2D_mult(m, 4);

    MAT2D_AT(m, 0, 0) = 0;
    MAT2D_AT(m, 1, 0) = 1;
    MAT2D_AT(m, 0, 1) = 2;

    MAT2D_PRINT(m);
    
    mat2D_invert(inv_m, m);

    MAT2D_PRINT(inv_m);

    mat2D_triangulate(m);

    MAT2D_PRINT(m);

    mat2D_free(m);
    mat2D_free(inv_m);

    return 0;
}
