#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#define ALMOG_LINEAR_ALGEBRA_IMPLEMENTATION
#include "Almog_Linear_Algebra.h"

#include <time.h>

int main(void)
{
    size_t n = 12;
    size_t m = n;
    struct Aml_Mat2d A = aml_mat2d_alloc(n, m);
    struct Aml_Mat2d res = aml_mat2d_alloc(n, m);

    // srand((int)time(NULL));
    aml_set_rand_symmetric(A, -2, 2);

    ala_symmetric_eigen_approximation(res, A, 1);

    AML_PRINT(A);
    AML_PRINT(res);



    return 0;
}
