#include "time.h"
#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"

void mat2D_solve_linear_sys_LUP_decomposition(Mat2D A, Mat2D x, Mat2D B)
{
    MATRIX2D_ASSERT(A.cols == x.rows);
    MATRIX2D_ASSERT(1 == x.cols);
    MATRIX2D_ASSERT(A.rows == B.rows);
    MATRIX2D_ASSERT(1 == B.cols);

    Mat2D y        = mat2D_alloc(x.rows, x.cols);
    Mat2D l        = mat2D_alloc(A.rows, A.cols);
    Mat2D p        = mat2D_alloc(A.rows, A.cols);
    Mat2D u        = mat2D_alloc(A.rows, A.cols);
    Mat2D inv_l    = mat2D_alloc(l.rows, l.cols);
    Mat2D inv_u    = mat2D_alloc(u.rows, u.cols);

    mat2D_LUP_decomposition_with_swap(A, l, p, u);

    mat2D_invert(inv_l, l);
    mat2D_invert(inv_u, u);

    mat2D_fill(x, 0);   /* x here is only a temp mat*/
    mat2D_fill(y, 0);
    mat2D_dot(x, p, B);
    mat2D_dot(y, inv_l, x);

    mat2D_fill(x, 0);
    mat2D_dot(x, inv_u, y);

    mat2D_free(y);
    mat2D_free(l);
    mat2D_free(p);
    mat2D_free(u);
    mat2D_free(inv_l);
    mat2D_free(inv_u);
}

int main(void)
{
    Mat2D a     = mat2D_alloc(3, 3);
    Mat2D b     = mat2D_alloc(3, 1);
    Mat2D x     = mat2D_alloc(3, 1);

    // srand(time(0));
    // mat2D_rand(a, 0, 1);
    MAT2D_AT(a, 0, 0) =  1;
    MAT2D_AT(a, 0, 1) =  3;
    MAT2D_AT(a, 0, 2) = -2;

    MAT2D_AT(a, 1, 0) =  3;
    MAT2D_AT(a, 1, 1) =  5;
    MAT2D_AT(a, 1, 2) =  6;

    MAT2D_AT(a, 2, 0) =  2;
    MAT2D_AT(a, 2, 1) =  4;
    MAT2D_AT(a, 2, 2) =  3;

    MAT2D_AT(b, 0, 0) =  5;
    MAT2D_AT(b, 1, 0) =  7;
    MAT2D_AT(b, 2, 0) =  8;

    MAT2D_PRINT(a);
    MAT2D_PRINT(b);

    mat2D_solve_linear_sys_LUP_decomposition(a, x, b);
    MAT2D_PRINT(x);

    mat2D_free(a);
    mat2D_free(b);
    mat2D_free(x);

    return 0;
}
