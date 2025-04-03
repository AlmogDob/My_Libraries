#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"
#include "time.h"


int main(void)
{
    Mat2D m = mat2D_alloc(5, 5);

    Mat2D a = mat2D_alloc(3, 4);
    mat2D_fill(a, 1);

    mat2D_copy_mat_to_mat_at_ij(m, a, 1, 0);

    MAT2D_PRINT(m);

    mat2D_free(m);

    return 0;
}
