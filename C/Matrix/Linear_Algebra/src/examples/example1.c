#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#define ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#include "Almog_Linear_Algebra.h"

int main(void)
{
    size_t n = 4, m = 4;
    struct Aml_Mat2d A = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d Q = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d QT = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d T = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d temp = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d res = aml_mat2d_alloc(n, m);

    aml_set_rand_symmetric(A, 1, 2);

    ala_symmetric_tridiagonalize_householder(Q, T, A);

    aml_transpose(QT, Q);

    aml_dot(temp, T, QT);
    aml_dot(res, Q, temp);

    AML_PRINT(A);
    AML_PRINT(Q);
    AML_PRINT(T);
    AML_PRINT(res);


    return 0;
}
