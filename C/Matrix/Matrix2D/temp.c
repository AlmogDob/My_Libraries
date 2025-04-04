#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"
#include "time.h"

int main(void)
{
    Mat2D m = mat2D_alloc(50, 50);
    Mat2D inv_m = mat2D_alloc(m.rows, m.cols);
    srand(1);
    mat2D_rand(m, 0, 1);
    // mat2D_fill_sequence(m, 0, 1);
    
    printf("%f\n", mat2D_det(m));
    mat2D_invert(inv_m, m);

    MAT2D_PRINT(m);
    MAT2D_PRINT(inv_m);

    mat2D_free(m);
    mat2D_free(inv_m);

    return 0;
}
