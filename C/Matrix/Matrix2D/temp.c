#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"
#include "time.h"

int main(void)
{
    Mat2D m = mat2D_alloc(50, 50);
    Mat2D inv_m = mat2D_alloc(m.rows, m.cols);
    srand(1);
    mat2D_rand(m, 0, 1);
    // mat2D_fill_sequence(m, 0, 1);
    
    printf("%f\n", mat2D_det(m));
    mat2D_invert(inv_m, m);

    MAT2D_PRINT(m);
    MAT2D_PRINT(inv_m);

    mat2D_free(m);
    mat2D_free(inv_m);

    return 0;
}

void mat2D_invert(Mat2D des, Mat2D src)
{
    MATRIX2D_ASSERT(src.cols == src.rows && "should be an NxN matrix");
    MATRIX2D_ASSERT(des.cols == src.cols && des.rows == des.cols);

    Mat2D m = mat2D_alloc(src.rows, src.cols);
    mat2D_copy(m, src);

    mat2D_set_identity(des);

    for (size_t i = 0; i < (size_t)fmin(m.rows-1, m.cols); i++) {
        if (0 == MAT2D_AT(m, i, i)) {
            /* find row with non zero element at index i */
            for (size_t j = i+1; j < m.cols; j++) {
                if (0 != MAT2D_AT(m, j, i)) {
                    mat2D_swap_rows(m, j, i);
                    mat2D_swap_rows(m, j, m.rows-1);

                    mat2D_swap_rows(des, j, i);
                    mat2D_swap_rows(des, j, des.rows-1);
                    break;
                }
                i++;
            }
        }
        for (size_t j = i+1; j < m.cols; j++) {
            double factor = 1 / MAT2D_AT(m, i, i);
            double mat_value = MAT2D_AT(m, j, i);
            mat2D_sub_row_time_factor_to_row(m, j, i, mat_value * factor);
            mat2D_mult_row(m, i, factor);

            mat2D_sub_row_time_factor_to_row(des, j, i, mat_value * factor);
            mat2D_mult_row(des, i, factor);
        }

        if (0 == i) {
            // MAT2D_PRINT(m);
            // MAT2D_PRINT(m);
            // return;
        }
    }
    double factor = 1 / MAT2D_AT(m, m.rows-1, m.cols-1);
    mat2D_mult_row(m, m.rows-1, factor);
    mat2D_mult_row(des, des.rows-1, factor);
    for (size_t c = m.cols-1; c > 0; c--) {
        for (int r = c-1; r >= 0; r--) {
            double factor = 1 / MAT2D_AT(m, c, c);
            double mat_value = MAT2D_AT(m, r, c);
            mat2D_sub_row_time_factor_to_row(m, r, c, mat_value * factor);
            mat2D_sub_row_time_factor_to_row(des, r, c, mat_value * factor);
        }
    }

    mat2D_free(m);
}