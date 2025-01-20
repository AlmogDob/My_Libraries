#define MATRIX_IMPLEMENTATION
#include "Matrix.h"
#include "time.h"

int main(void)
{
    Mat m = mat_alloc(2, 2);
    srand(time(0));
    mat_rand(m, 0, 1);

    MAT_PRINT(m);
    return 0;
}