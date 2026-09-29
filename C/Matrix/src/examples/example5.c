// #define AML_SINGLE_PRECISION 
#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#define ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#include "../Almog_Linear_Algebra.h"

#include <time.h>

int main(void)
{
    size_t n = 1000;
    size_t m = n + 40;
    struct Aml_Mat2d A = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d Q = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d R = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d QR = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d diff = aml_mat2d_alloc(n, m);

    // srand((int)time(NULL));
    srand(1);
    aml_set_rand(A, 1, 2);

    ala_QR_householder_decomposition_fast(Q, R, A);

    // AML_PRINT(A);
    // AML_PRINT(Q);
    // AML_PRINT(R);

    aml_dot(QR, Q, R);
    aml_copy(diff, QR);
    aml_sub(diff, A);
    
    // AML_PRINT(QR);
    // AML_PRINT(diff);
    aml_dprintDOUBLE(aml_calc_norma(diff));
    aml_dprintDOUBLE(aml_calc_norma(diff) / aml_calc_norma(A));


    return 0;
}
