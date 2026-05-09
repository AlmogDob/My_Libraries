#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#include "Almog_Matrix_Library.h"

int main(void)
{
    size_t n = 40, m = 60;
    struct Aml_Mat2d A = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d Q = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d R = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d temp = aml_mat2d_alloc(n, m);

    aml_fill_sequence(A, 0, 1);

    // AML_PRINT(A);

    // aml_QR_householder_factorization(Q, R, A);
    aml_QR_householder_factorization_fast(Q, R, A);

    aml_dot(temp, Q, R);

    AML_PRINT(temp);


    return 0;
}
