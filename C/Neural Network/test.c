#define NN_IMPLEMENTATION
#include "nn.h"

int main(void)
{
    Mat a = mat_alloc(2, 2);
    Mat b = mat_alloc(2, 2);
    Mat ans = mat_alloc(2, 2);

    mat_fill(a, 0);
    MAT_AT(a, 0, 1) = 1;
    mat_fill(b, 10);

    mat_sum(a, b);
    MAT_PRINT(a);
    MAT_PRINT(b);
    MAT_PRINT(ans);
    
}