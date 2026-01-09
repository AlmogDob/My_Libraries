#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"

double mat2D_power_iterate(Mat2D A, Mat2D v, bool norm_inf_v)
{
    /* https://www.youtube.com/watch?v=SkPusgctgpI */

    /** Conditions:
     *      - The eigenvectors must form a basis 
     *      - The largest eigenvalue must be positive and unique
     */ 

    MAT2D_ASSERT(A.cols == A.rows); 
    MAT2D_ASSERT(v.cols == 1);
    MAT2D_ASSERT(v.rows == A.rows);

    Mat2D current_v = mat2D_alloc(v.rows, v.cols);
    Mat2D temp_v = mat2D_alloc(v.rows, v.cols);

    int max_num_iteration = 50;
    double lambda = 0;

    /* Rayleigh quotient */
    mat2D_dot(temp_v, A, v);
    lambda = mat2D_dot_product(temp_v, v) / mat2D_calc_norma(v);

    for (int i = 0; i < max_num_iteration; i++) {
        mat2D_copy(current_v, v);
        mat2D_dot(v, A, current_v);
        mat2D_normalize(v);
        mat2D_mult(v, fabs(lambda) / lambda);
        mat2D_dot(temp_v, A, v);
        lambda = mat2D_dot_product(temp_v, v);

        mat2D_sub(current_v, v);
        if (mat2D_calc_norma_inf(current_v) < MAT2D_EPS) {
            break;
        }
    }

    mat2D_free(current_v);
    mat2D_free(temp_v);

    if (norm_inf_v) mat2D_normalize_inf(v);
    return lambda;
}

int main(void)
{
    int n = 2;
    Mat2D A = mat2D_alloc(n, n);   
    Mat2D v = mat2D_alloc(n, 1);   


    MAT2D_AT(v, 0, 0) = 1;
    MAT2D_AT(v, 1, 0) = 0;

    MAT2D_AT(A, 0, 0) = -7;
    MAT2D_AT(A, 1, 0) = -4;
    MAT2D_AT(A, 0, 1) = +8;
    MAT2D_AT(A, 1, 1) = +5;

    // mat2D_rand(v, 0, 1);

    // MAT2D_AT(A, 0, 0) = 2;
    // MAT2D_AT(A, 1, 0) = 0;
    // MAT2D_AT(A, 2, 0) = 0;
    // MAT2D_AT(A, 0, 1) = 0;
    // MAT2D_AT(A, 1, 1) = 3;
    // MAT2D_AT(A, 2, 1) = 4;
    // MAT2D_AT(A, 0, 2) = 0;
    // MAT2D_AT(A, 1, 2) = 4;
    // MAT2D_AT(A, 2, 2) = 9;

    MAT2D_PRINT(A); 

    mat2D_dprintDOUBLE(mat2D_power_iterate(A, v, 1));
    MAT2D_PRINT(v);


    mat2D_free(A);
    mat2D_free(v);

    return 0;
}
