#include <stdio.h>
#define MATRIX2D_IMPLEMENTATION
#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "Almog_Draw_Library.h"

int main()
{
    Point points[] = {{.x = 20, .y = 20, .z = 0}, {.x = 20, .y = 80, .z = 0}, {.x = 70, .y = 20, .z = 0}};
    int len = 3;
    Mat2D_uint32 screen = mat2D_alloc_uint32(100, 100);

    adl_draw_lines(screen, points, len, 0xFFFFFFFF);

    MAT2D_UINT32_PRINT(screen);

    return 0;
}
