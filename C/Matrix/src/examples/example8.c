// #define AML_SINGLE_PRECISION 
#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#define ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#include "../Almog_Linear_Algebra.h"

int main(void)
{
    size_t n = 1000;
    struct Aml_Mat2d A = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d Q = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d Q_h = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d H = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d U = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d Q_u = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d diff = aml_mat2d_alloc(n, n);

    aml_set_rand(A, -2, 2);

    aml_dprintINFO("n = %zu", n);
    ala_hessenberg_decomposition_householder(Q_h, H, A);
    aml_dprintINFO("%s", "Finished hessenberg decomposition.");

    ala_hessenberg_QUQm1_schur_decomposition_householder_fast(Q_u, U, H, true);

    // AML_PRINT(A);
    // AML_PRINT(Q_h);
    // AML_PRINT(H);
    // AML_PRINT(Q_u);
    // AML_PRINT(U);

    aml_dot(Q, Q_h, Q_u);
    aml_transpose_inplace(Q);
    aml_dot(Q_h, U, Q);
    aml_transpose_inplace(Q);
    aml_dot(diff, Q, Q_h);
    aml_sub(diff, A);
    
    aml_dprintDOUBLE(aml_calc_norma(diff));
    aml_dprintDOUBLE(aml_calc_norma(diff) / aml_calc_norma(A));

    return 0;
}
