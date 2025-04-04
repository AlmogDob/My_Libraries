#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"
#include "time.h"

int main(void)
{
    Mat2D m = mat2D_alloc(5, 5);
    Mat2D inv_m = mat2D_alloc(m.rows, m.cols);
    srand(2);
    mat2D_rand(m, -10, 10);
    // mat2D_fill_sequence(m, 0, 1);
    mat2D_copy(inv_m, m);
    
    printf("%f\n", mat2D_det(m));
    printf("%f\n", 1/mat2D_make_identity(inv_m));

    // mat2D_invert(inv_m, m);

    MAT2D_PRINT(m);
    MAT2D_PRINT(inv_m);

    mat2D_free(m);
    mat2D_free(inv_m);

    return 0;
}
