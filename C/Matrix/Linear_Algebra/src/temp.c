#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#define ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#include "Almog_Linear_Algebra.h"

#include <time.h>

int main(void)
{
    size_t n = 500, m = 500;
    // size_t n = 5, m = 5;
    struct Aml_Mat2d A = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d Q = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d T = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d R = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d res = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d eigvalues = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d eigvectors = aml_mat2d_alloc(n, m);

    // srand(time(0));
    aml_set_rand_symmetric(A, 1, 2);
    // AML_MAT2D_AT(A, 0, 0) = 1; AML_MAT2D_AT(A, 0, 1) = 2; AML_MAT2D_AT(A, 0, 2) = 3; AML_MAT2D_AT(A, 0, 3) = 4;
    // AML_MAT2D_AT(A, 1, 0) = 2; AML_MAT2D_AT(A, 1, 1) = 0; AML_MAT2D_AT(A, 1, 2) = 5; AML_MAT2D_AT(A, 1, 3) = 0;
    // AML_MAT2D_AT(A, 2, 0) = 3; AML_MAT2D_AT(A, 2, 1) = 5; AML_MAT2D_AT(A, 2, 2) = 10; AML_MAT2D_AT(A, 2, 3) = 1;
    // AML_MAT2D_AT(A, 3, 0) = 4; AML_MAT2D_AT(A, 3, 1) = 0; AML_MAT2D_AT(A, 3, 2) = 1; AML_MAT2D_AT(A, 3, 3) = -2;

    ala_symmetric_eig_QR_tridiagonalize(A, eigvalues, eigvectors);
    // ala_symmetric_eig_QR_shift(A, eigvalues, eigvectors);

    // AML_PRINT(A);

    AML_PRINT(eigvalues);
    // AML_PRINT(eigvectors);

    aml_dprintINT(ala_eig_check(A, eigvalues, eigvectors, res));
    // AML_PRINT(res);


    return 0;
}
