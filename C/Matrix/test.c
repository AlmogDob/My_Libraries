#define MATRIX_IMPLEMENTATION
#include "Matrix.h"

int main(void)
{
    Mat m = mat_alloc(2, 2);
    mat_fill(m,30);

    MAT_PRINT(m);
    return 0;
}