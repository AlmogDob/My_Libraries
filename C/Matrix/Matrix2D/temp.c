#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"
#include "time.h"

double mat2D_det(Mat2D m)
{
    double det = 0;
    /* TODO: finding beast row or col? */
    for (size_t i = 0, j = 0; i < m.rows; i++) { /* first column */
        Mat2D_Minor sub_mm = mat2D_minor_alloc_fill_from_mat(m, i, j);
        int factor = (i+j)%2 ? -1 : 1;
        if (sub_mm.cols != 2) {
            MATRIX2D_ASSERT(sub_mm.cols == sub_mm.rows && "should be a square matrix");
            det += MAT2D_AT(m, i, j) * (factor) * mat2D_minor_det(sub_mm);
        } else if (sub_mm.cols == 2 && sub_mm.rows == 2) {
            det += MAT2D_AT(m, i, j) * (factor) * mat2D_det_2x2_mat_minor(sub_mm);;
        }
        mat2D_minor_free(sub_mm);
    }
    return det;
}


int main(void)
{
    Mat2D m = mat2D_alloc(12,12);
    srand(time(0));
    mat2D_rand(m, 0, 1);

    MAT2D_PRINT(m);

    printf("%.10f\n", mat2D_det(m));

    mat2D_free(m);

    return 0;
}
