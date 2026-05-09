#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#define ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#include "Almog_Linear_Algebra.h"

int main(void)
{
    size_t n = 60, m = 60;
    struct Aml_Mat2d A = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d res = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d eigvalues = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d eigvectors = aml_mat2d_alloc(n, m);

    aml_set_rand_symmetric(A, 1, 2);
    // AML_MAT2D_AT(A, 0, 0) = 1; AML_MAT2D_AT(A, 0, 1) = 2; AML_MAT2D_AT(A, 0, 2) = 3;
    // AML_MAT2D_AT(A, 1, 0) = 2; AML_MAT2D_AT(A, 1, 1) = 2; AML_MAT2D_AT(A, 1, 2) = 1;
    // AML_MAT2D_AT(A, 2, 0) = 3; AML_MAT2D_AT(A, 2, 1) = 1; AML_MAT2D_AT(A, 2, 2) = 3;

    ala_symmetric_eig_QR(A, eigvalues, eigvectors);

    ala_eig_check(A, eigvalues, eigvectors, res);

    // AML_PRINT(A);
    // AML_PRINT(eigvalues);
    // AML_PRINT(eigvectors);
    // AML_PRINT(res);

    return 0;
}
