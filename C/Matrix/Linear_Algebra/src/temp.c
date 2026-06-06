// #define AML_SINGLE_PRECISION 
#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#define ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#include "Almog_Linear_Algebra.h"

#include <time.h>

int main(void)
{
    size_t n = 6;
    size_t m = n;
    struct Aml_Mat2d A = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d Q_h = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d H = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d U = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d Q_u = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d diff = aml_mat2d_alloc(n, m);

    // srand((int)time(NULL));
    srand(1);
    aml_set_rand(A, -2, 2);

    ala_hessenberg_decomposition_householder(Q_h, H, A);
    aml_dprintINFO("%s", "Finished hessenberg decomposition.");
    ala_hessenberg_QUQm1_schur_decomposition(Q_u, U, H);

    // AML_PRINT(A);
    // AML_PRINT(Q_h);
    // AML_PRINT(H);

    
    // AML_PRINT(diff);
    // aml_dprintDOUBLE(aml_calc_norma(diff));
    // aml_dprintDOUBLE(aml_calc_norma(diff) / aml_calc_norma(A));


    return 0;
}
