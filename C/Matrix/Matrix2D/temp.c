#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"
#include "time.h"

int main(void)
{
    Mat2D m = mat2D_alloc(10, 10);
    srand(time(0));
    mat2D_rand(m, 0, 1);

    MAT2D_PRINT(m);

    mat2D_print(m, "m", 10);

    mat2D_free(m);
    return 0;
}