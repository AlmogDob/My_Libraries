#include "time.h"
#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"

int main(void)
{
    int n = 3;
    Mat2D a     = mat2D_alloc(n, n);
    Mat2D l     = mat2D_alloc(n, n);
    Mat2D p     = mat2D_alloc(n, n);
    Mat2D u     = mat2D_alloc(n, n);
    Mat2D current_A  = mat2D_alloc(n, n);
    Mat2D previous_A = mat2D_alloc(n, n);
    Mat2D diff       = mat2D_alloc(n, n);
    Mat2D x          = mat2D_alloc(n, 1);
    Mat2D B          = mat2D_alloc(n, 1);

    // srand(time(0));
    // mat2D_rand(a, 0, 1);
    mat2D_fill_sequence(a, 1, 1);
    MAT2D_PRINT(a);

    mat2D_LUP_decomposition_with_swap(a, l, p, u);

    MAT2D_PRINT(l);
    MAT2D_PRINT(p);
    MAT2D_PRINT(u);

    mat2D_dot(current_A, l, u);
    MAT2D_PRINT(current_A);

    for (int i = 0; i < 25; i++) {
        mat2D_dot(current_A, l, u);
        mat2D_dot(previous_A, u, l);
        mat2D_LUP_decomposition_with_swap(previous_A, l, p, u);
        mat2D_copy(diff, current_A);
        mat2D_sub(diff, previous_A);
    }
    MAT2D_PRINT(diff);
    mat2D_copy(current_A, previous_A);

    mat2D_set_identity(previous_A);
    mat2D_mult(previous_A, MAT2D_AT(current_A, 1, 1));
    mat2D_sub(a, previous_A);

    mat2D_fill(B, 0);
    mat2D_solve_linear_sys_LUP_decomposition(a, x, B);

    MAT2D_PRINT(a);
    MAT2D_PRINT(x);
    printf("\n%g\n", mat2D_calc_norma(diff));

    return 0;
}
