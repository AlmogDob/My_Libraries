// #define AML_SINGLE_PRECISION 
#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#define ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#include "Almog_Linear_Algebra.h"

#include <time.h>

int main(void)
{
    size_t n = 1601;
    size_t m = n - 31;

    // size_t n = 16;
    // size_t m = n - 3;
    struct Aml_Mat2d A = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d l = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d u = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d p = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d pA = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d lu = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d diff = aml_mat2d_alloc(n, m);

    // srand((int)time(NULL));
    srand(1);
    aml_set_rand(A, 1, 2);
    // aml_fill_sequence(A, 1, 1);

    ala_LUP_decomposition_with_swap(A, l, p, u);

    aml_dprintINFO("%s", "Finished LU decomposition.");
    // AML_PRINT(A);
    // AML_PRINT(l);
    // AML_PRINT(u);
    // AML_PRINT(p);

    aml_dot(lu, l, u);
    aml_dot(pA, p, A);
    aml_copy(diff, pA);
    aml_sub(diff, lu);
    
    // AML_PRINT(pA);
    // AML_PRINT(lu);
    // AML_PRINT(diff);
    aml_dprintDOUBLE(aml_calc_norma(diff));
    aml_dprintDOUBLE(aml_calc_norma(diff) / aml_calc_norma(A));


    return 0;
}
