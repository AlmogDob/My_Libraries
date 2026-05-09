#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#include "Almog_Matrix_Library.h"

int main(void)
{
    size_t n = 40, m = 60;
    Mat2D A = aml_alloc(n, m);
    Mat2D Q = aml_alloc(n, n);
    Mat2D R = aml_alloc(n, m);
    Mat2D temp = aml_alloc(n, m);

    aml_fill_sequence(A, 0, 1);

    // AML_PRINT(A);

    // aml_QR_householder_factorization(Q, R, A);
    aml_QR_householder_factorization_fast(Q, R, A);

    aml_dot(temp, Q, R);

    AML_PRINT(temp);


    return 0;
}
