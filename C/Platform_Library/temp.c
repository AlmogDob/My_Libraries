#include <stdio.h>
#include <stdbool.h>

#define APL_SETUP
#define APL_INPUT
#define APL_UPDATE
#define APL_RENDER
#define MATRIX2D_IMPLEMENTATION
#define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#include "Almog_Platform_Library.h"
#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "Almog_Draw_Library.h"

Mat2D apl_depth_buffer_as_mat2d(struct Apl_Depth_Buffer b)
{
    Mat2D m = {
        .rows = b.rows,
        .cols = b.cols,
        .stride_r = b.stride_r,
        .elements = (mat2D_real *)b.elements,
    };
    return m;
}

Mat2D_uint32 apl_pixel_buffer_mat2d_u32(struct Apl_Pixel_Buffer b)
{
    Mat2D_uint32 m = {
        .rows = b.rows,
        .cols = b.cols,
        .stride_r = b.stride_r,
        .elements = b.elements,
    };
    return m;
}

enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
    ws->to_limit_fps = false;
    APL_UNUSED(ws);

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_input(struct Apl_Window_State *ws)
{
    if (ws->buttons.w_is_pressed) {
        printf("w");
    }
    if (ws->buttons.a_is_pressed) {
        printf("a");
    }
    if (ws->buttons.s_is_pressed) {
        printf("s");
    }
    if (ws->buttons.d_is_pressed) {
        printf("d");
    }
    if (ws->buttons.space_bar_is_pressed) {
        printf("\n");
    }
    APL_UNUSED(ws);

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_update(struct Apl_Window_State *ws)
{
    APL_UNUSED(ws);

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_render(struct Apl_Window_State *ws)
{
    Mat2D_uint32 pixel_mat2D = apl_pixel_buffer_mat2d_u32(ws->window_pixels_mat);
    adl_circle_fill(pixel_mat2D, 100, 100, 100, APL_COLOR_WHITE_hexARGB, ADL_DEFAULT_OFFSET_ZOOM);
    adl_circle_fill(pixel_mat2D, 300, 500, 100, APL_COLOR_WHITE_hexARGB, ADL_DEFAULT_OFFSET_ZOOM);

    APL_UNUSED(ws);

    return APL_SUCCESS;
}
