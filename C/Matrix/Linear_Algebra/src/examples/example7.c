// #define AML_SINGLE_PRECISION 
#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#define ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#include "Almog_Linear_Algebra.h"

#include <time.h>

int main(void)
{
    size_t n = 10;
    size_t m = n;
    struct Aml_Mat2d A = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d Q = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d T = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d QTQtrans = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d diff = aml_mat2d_alloc(n, m);

    // srand((int)time(NULL));
    srand(1);
    aml_set_rand(A, -2, 2);

    ala_hessenberg_decomposition_householder(Q, T, A);
    aml_dprintINFO("%s", "Finished hessenberg decomposition.");

    AML_PRINT(A);
    AML_PRINT(Q);
    AML_PRINT(T);

    aml_transpose_inplace(Q);
    aml_dot(diff, T, Q);
    aml_transpose_inplace(Q);
    aml_dot(QTQtrans, Q, diff);
    aml_copy(diff, QTQtrans);
    aml_sub(diff, A);
    
    // AML_PRINT(QR);
    AML_PRINT(diff);
    aml_dprintDOUBLE(aml_calc_norma(diff));
    aml_dprintDOUBLE(aml_calc_norma(diff) / aml_calc_norma(A));


    return 0;
}
