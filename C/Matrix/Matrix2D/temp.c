#include "time.h"
#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"

void mat2D_LU_decomposition_with_swap(Mat2D src, Mat2D l, Mat2D p, Mat2D u)
{
    /* performing LU decomposition Following the Wikipedia page: https://en.wikipedia.org/wiki/LU_decomposition */

    mat2D_copy(u, src);
    mat2D_set_identity(p);

    for (size_t i = 0; i < (size_t)fmin(u.rows-1, u.cols); i++) {
        if (!MAT2D_AT(u, i, i)) {   /* swapping only if it is zero */
            /* finding biggest first number (absolute value) */
            size_t biggest_r = i;
            for (size_t index = i; index < u.rows; index++) {
                if (fabs(MAT2D_AT(u, index, i)) > fabs(MAT2D_AT(u, biggest_r, i))) {
                    biggest_r = index;
                }
            }
            if (i != biggest_r) {
                mat2D_swap_rows(u, i, biggest_r);
                mat2D_swap_rows(p, i, biggest_r);
            }
        }
        for (size_t j = i+1; j < u.cols; j++) {
            double factor = 1 / MAT2D_AT(u, i, i);
            if (!isfinite(factor)) {
                printf("%s:%d: [Error] unable to transfrom into uperr triangular matrix. Probably some of the rows are not independent.\n", __FILE__, __LINE__);
            }
            double mat_value = MAT2D_AT(u, j, i);
            mat2D_sub_row_time_factor_to_row(u, j, i, mat_value * factor);
        }
    }
}

int main(void)
{
    Mat2D m = mat2D_alloc(5, 5);
    Mat2D l = mat2D_alloc(5, 5);
    Mat2D p = mat2D_alloc(5, 5);
    Mat2D u = mat2D_alloc(5, 5);

    mat2D_rand(m, 0, 1);

    mat2D_LU_decomposition_with_swap(m, l, p, u);

    MAT2D_PRINT(m);
    MAT2D_PRINT(u);
    MAT2D_PRINT(p);
    // MAT2D_PRINT(l);

    mat2D_free(m);
    mat2D_free(l);
    mat2D_free(p);
    mat2D_free(u);

    return 0;
}
