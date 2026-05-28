// #define AML_SINGLE_PRECISION 
#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#define ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#include "Almog_Linear_Algebra.h"

#include <time.h>

int main(void)
{
    size_t n = 1500;
    size_t m = n;
    struct Aml_Mat2d A = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d R = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d check = aml_mat2d_alloc(n, m);

    // srand((int)time(NULL));
    srand(1);
    aml_set_rand(R, 1, 2);
    aml_transpose(check, R);
    aml_dot(A, check, R);

    ala_positive_definite_RTR_Cholesky_decomposition(R, A);

    // AML_PRINT(A);
    // AML_PRINT(R);

    // aml_transpose(A, R);
    // aml_dot(check, A, R);
    
    // AML_PRINT(check);


    return 0;
}
