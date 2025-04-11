#include "time.h"
#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"

int main(void)
{
    Mat2D m = mat2D_alloc(5, 5);
    Mat2D inv_m = mat2D_alloc(m.rows, m.cols);
    srand(2);
    
    mat2D_set_identity(m);
    mat2D_mult(m, 4);

    mat2D_invert(inv_m, m);

    MAT2D_PRINT(m);
    MAT2D_PRINT(inv_m);

    mat2D_free(m);
    mat2D_free(inv_m);

    return 0;
}
