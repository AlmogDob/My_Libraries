#include <stdio.h>
#define MATRIX2D_IMPLEMENTATION
#define ALMOG_RENDER_SHAPES_IMPLEMENTATION
#include "Almog_Render_Shapes.h"

int main()
{
    Mat2D_uint32 screen = mat2D_alloc_uint32(10, 10);
    ars_draw_circle(screen, 5,5,2,0xFFFFFF);

    MAT2D_UINT32_PRINT(screen);

    return 0;
}
