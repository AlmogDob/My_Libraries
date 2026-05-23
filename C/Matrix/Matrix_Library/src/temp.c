#define ALMOG_MATRIX_LIBRARY_IMPLEMENTATION
#include "Almog_Matrix_Library.h"

int main(void)
{
    size_t n = 40, m = 60;
    struct Aml_Mat2d A = aml_mat2d_alloc(n, m);

    aml_fill_sequence(A, 0, 1);

    AML_PRINT(A);

    return 0;
}
