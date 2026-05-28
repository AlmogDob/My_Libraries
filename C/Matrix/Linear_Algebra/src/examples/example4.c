// #define AML_SINGLE_PRECISION 
#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#define ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#include "Almog_Linear_Algebra.h"

#include <time.h>

ALA_DEF bool ala_RTR_Cholesky_decomposition(struct Aml_Mat2d R, struct Aml_Mat2d A)
{
    /* This decomposition will succeed only when A is positive definite */
    /* This algorithm is taken from the Fundamentals of Matrix Computations book Pg.37 */

    ALA_ASSERT(A.rows == A.cols);
    ALA_ASSERT(R.rows == R.cols);
    ALA_ASSERT(R.rows == A.rows);
    ALA_ASSERT(A.rows >= 2);
    ALA_ASSERT(aml_is_symmetric(A));

    size_t n = A.rows;

    aml_fill(R, 0);

    aml_dprintINFO("%s", "");
    printf("\33[A");
    for (size_t i = 0; i < n; i++) {
        printf("\n\33[A\33[2K\r       i: %zu / %zu", i, n-1);
        aml_real a_ii = AML_MAT2D_AT(A, i, i);
        aml_real temp_r_ii = 0;
        for (size_t k = 0; k < i; k++) {
            temp_r_ii += AML_MAT2D_AT(R, k, i) * AML_MAT2D_AT(R, k, i);
        }
        if ((a_ii - temp_r_ii) < 0) {
            printf("\n");
            aml_dprintERROR("Matrix A is not positive definite. About to take the root of a negative number: A(%zu, %zu) - sum_k(R(k, %zu)^2) = %g.", i, i, i, a_ii - temp_r_ii);
            return false;
        }
        AML_MAT2D_AT(R, i, i) = aml_sqrt(a_ii - temp_r_ii);
        if (AML_IS_ZERO(AML_MAT2D_AT(R, i, i))) {
            printf("\n");
            aml_dprintERROR("Matrix A is not positive definite. R(%zu, %zu) = %g. Can't divide by zero.", i, i, AML_MAT2D_AT(R, i, i));
            return false;
        }
        for (size_t j = i + 1; j < n; j++) {
            aml_real a_ij = AML_MAT2D_AT(A, i, j);
            aml_real temp_r_ij = 0;
            for (size_t k = 0; k < i; k++) {
                temp_r_ij += AML_MAT2D_AT(R, k, i) * AML_MAT2D_AT(R, k, j);
            }
            AML_MAT2D_AT(R, i, j) = (a_ij - temp_r_ij) / AML_MAT2D_AT(R, i, i);
        }
    }
    printf("\n");

    return true;
}

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

    ala_RTR_Cholesky_decomposition(R, A);

    // AML_PRINT(A);
    // AML_PRINT(R);

    // aml_transpose(A, R);
    // aml_dot(check, A, R);
    
    // AML_PRINT(check);


    return 0;
}
