#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"
#include "time.h"


int main(void)
{
    Mat2D m = mat2D_alloc(4,4);
    mat2D_fill_sequence(m, 0, 0.5);

    MAT2D_PRINT(m);
    MAT2D_PRINT_AS_COL(m);

    Mat2D_Minor mm = mat2D_minor_alloc_and_fill(m, 3, 3);
    MAT2D_MINOR_PRINT(mm);
    for (size_t i = 0; i < mm.cols; i++) {
        printf("%zu\n", mm.cols_list[i]);
    }

    mat2D_free(m);

    return 0;
}
