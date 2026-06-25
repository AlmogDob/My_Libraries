// #define AML_SINGLE_PRECISION 
#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#define ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#include "Almog_Linear_Algebra.h"

int main(void)
{
    size_t n = 1000;
    struct Aml_Mat2d A = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d B = aml_mat2d_alloc(n, n);
    struct Aml_Mat2d C = aml_mat2d_alloc(n, n);

    aml_fill(A, 1);
    aml_fill(B, 1);

    aml_dot(C, A, B);

    return 0;
}
