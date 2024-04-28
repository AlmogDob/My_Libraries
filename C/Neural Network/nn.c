#define NN_IMPLEMENTATION
#include "nn.h"
#include "time.h"

int main(void)
{
    srand(time(0));
    Mat a = mat_alloc(3, 3);
    mat_identity_mat(a);
    mat_mult(a, 2);

    Mat b = mat_alloc(3, 3);
    mat_fill(b, 4);

    MAT_PRINT(a);
    MAT_PRINT(b);
    printf("-------------------------------\n");
    Mat dst = mat_alloc(3, 3);
    mat_dot(dst, a, b);
    
    MAT_PRINT(dst);
    return 0;
}