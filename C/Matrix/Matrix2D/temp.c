#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"
#include "time.h"


int main(void)
{
    Mat2D m = mat2D_alloc(4,4);
    MAT2D_AT(m, 0, 0) = 0;
    MAT2D_AT(m, 0, 1) = 1;
    MAT2D_AT(m, 0, 2) = 2;
    MAT2D_AT(m, 0, 3) = 3;
    MAT2D_AT(m, 1, 0) = 4;
    MAT2D_AT(m, 1, 1) = 5;
    MAT2D_AT(m, 1, 2) = 6;
    MAT2D_AT(m, 1, 3) = 7;
    MAT2D_AT(m, 2, 0) = 8;
    MAT2D_AT(m, 2, 1) = 9;
    MAT2D_AT(m, 2, 2) = 10;
    MAT2D_AT(m, 2, 3) = 11;
    MAT2D_AT(m, 3, 0) = 12;
    MAT2D_AT(m, 3, 1) = 13;
    MAT2D_AT(m, 3, 2) = 14;
    MAT2D_AT(m, 3, 3) = 15;

    MAT2D_PRINT(m);
    MAT2D_PRINT_AS_COL(m);

    Mat2D_Minor mm = mat2D_minor_alloc_and_fill(m, 1, 1);
    MAT2D_MINOR_PRINT(mm);
    for (size_t i = 0; i < mm.cols; i++) {
        printf("%zu\n", mm.cols_list[i]);
    }

    mat2D_free(m);

    return 0;
}
