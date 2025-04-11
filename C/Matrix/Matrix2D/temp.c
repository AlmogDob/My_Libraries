#include "time.h"
#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"

int main(void)
{
    Mat2D m = mat2D_alloc(5, 5);
    Mat2D window_m = mat2D_alloc(4, 3);

    mat2D_fill_sequence(m, 0, 1);
    
    mat2D_copy_mat_to_mat_at_window(window_m, m, 1, 1, 4, 3);

    MAT2D_PRINT(m);
    MAT2D_PRINT(window_m);

    mat2D_free(m);
    mat2D_free(window_m);

    return 0;
}
