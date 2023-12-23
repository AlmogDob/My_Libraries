#include <time.h>

#define MATRIX_IMPLEMENTATION
#include "Matrix.h"

int main(void)
{
    Mat a = mat_alloc(2, 2);
    mat_fill(a,1);

    Mat b = mat_alloc(2, 2);
    mat_fill(b,1);

    Mat afsdf = mat_alloc(2, 2);

    MAT_AT(b, 1, 1) = 0;
    MAT_AT(a, 1, 1) = 0;

    mat_dot(afsdf, a, b);

    MAT_PRINT(afsdf);
    MAT_PRINT(a);
    
    return 0;
}
