// #define AML_SINGLE_PRECISION 
#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#define ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#include "Almog_Linear_Algebra.h"

#include <time.h>

int main(void)
{
    // size_t n = 3;
    size_t n = 1200;
    struct Aml_Mat2d A = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d Q = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d Q_h = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d H = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d U = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d Q_u = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d diff = aml_mat2d_alloc(n, n);

    // srand((int)time(NULL));
    srand(1);
    aml_set_rand(A, -2, 2);
    // aml_set_rand_symmetric(A, -2, 2);

    // AML_MAT2D_AT(A, 0, 0) = -149; AML_MAT2D_AT(A, 0, 1) = -50 ; AML_MAT2D_AT(A, 0, 2) = -154;
    // AML_MAT2D_AT(A, 1, 0) =  537; AML_MAT2D_AT(A, 1, 1) =  180; AML_MAT2D_AT(A, 1, 2) =  546;
    // AML_MAT2D_AT(A, 2, 0) = -27 ; AML_MAT2D_AT(A, 2, 1) = -9  ; AML_MAT2D_AT(A, 2, 2) = -25 ;

    // AML_MAT2D_AT(A, 0, 0) =  8; AML_MAT2D_AT(A, 0, 1) = 1; AML_MAT2D_AT(A, 0, 2) = 6;
    // AML_MAT2D_AT(A, 1, 0) =  3; AML_MAT2D_AT(A, 1, 1) = 5; AML_MAT2D_AT(A, 1, 2) = 7;
    // AML_MAT2D_AT(A, 2, 0) =  4; AML_MAT2D_AT(A, 2, 1) = 9; AML_MAT2D_AT(A, 2, 2) = 2;

    aml_dprintINFO("n = %zu", n);
    ala_hessenberg_decomposition_householder(Q_h, H, A);
    aml_dprintINFO("%s", "Finished hessenberg decomposition.");

    // ala_hessenberg_QUQm1_schur_decomposition_given(Q_u, U, H);
    // ala_hessenberg_QUQm1_schur_decomposition_householder(Q_u, U, H, true);
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
