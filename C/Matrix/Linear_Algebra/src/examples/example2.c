#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#define ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#include "Almog_Linear_Algebra.h"

#include <time.h>

int main(void)
{
    size_t n = 1000;
    // size_t n = 5;
    size_t m = n;
    struct Aml_Mat2d A = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d res = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d eigvalues = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d eigvectors = aml_mat2d_alloc(n, m);

    // srand((int)time(NULL));
    aml_set_rand_symmetric(A, -2, 2);

    ala_symmetric_eig_QR_tridiagonalize(A, eigvalues, eigvectors);

    // AML_PRINT(A);
    // AML_PRINT(eigvalues);
    // AML_PRINT(eigvectors);

    aml_dprintINT(ala_eig_check(A, eigvalues, eigvectors, res));
    // AML_PRINT(res);


    return 0;
}