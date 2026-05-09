#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#include "Almog_Matrix_Library.h"

int main(void)
{
    size_t n = 40, m = 60;
    Mat2D A = mat2D_alloc(n, m);
    Mat2D Q = mat2D_alloc(n, n);
    Mat2D R = mat2D_alloc(n, m);
    Mat2D temp = mat2D_alloc(n, m);

    mat2D_fill_sequence(A, 0, 1);

    // MAT2D_PRINT(A);

    // mat2D_QR_householder_factorization(Q, R, A);
    mat2D_QR_householder_factorization_fast(Q, R, A);

    mat2D_dot(temp, Q, R);

    MAT2D_PRINT(temp);


    return 0;
}
