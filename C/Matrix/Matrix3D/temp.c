#define MATRIX3D_IMPLEMENTATION
#include "Matrix3D.h"
#include "time.h"

int main(void)
{
    Mat3D m1 = mat3D_alloc(3, 3, 3);
    Mat3D m2 = mat3D_alloc(3, 3, 3);

    mat3D_fill(m1, 0);
    mat3D_fill(m2, 1);

    MAT3D_AT(m1, 1, 1, 2) = 1;

    mat3D_sum(m1, m2);
    mat3D_identity_mat(m2);

    MAT3D_AT(m2, 2, 2, 0) = 1123;

    MAT3D_PRINT(m1);
    MAT3D_PRINT(m2);

    mat3D_free(m1);
    mat3D_free(m2);
    return 0;
}