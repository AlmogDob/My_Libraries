#include <stdio.h>
#include <stdbool.h>

#define APL_SETUP
#define APL_INPUT
#define APL_UPDATE
#define APL_RENDER

#define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#include "includes/Almog_Platform_Library.h"

#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "includes/Almog_Draw_Library.h"

void apl_depth_buffer_to_adl_depth_buffer(struct Adl_Depth_Buffer adl_b, struct Apl_Depth_Buffer apl_b)
{
    ADL_ASSERT(adl_b.rows == apl_b.rows);
    ADL_ASSERT(adl_b.cols == apl_b.cols);
    ADL_ASSERT(adl_b.stride_r == apl_b.stride_r);

    for (size_t i = 0; i < apl_b.rows; i++) {
        for (size_t j = 0; j < apl_b.cols; j++) {
            ADL_BUFFER_AT(adl_b, i, j) = APL_BUFFER_AT(apl_b, i, j);
        }
    }
}

void adl_depth_buffer_to_apl_depth_buffer(struct Apl_Depth_Buffer apl_b, struct Adl_Depth_Buffer adl_b)
{
    APL_ASSERT(adl_b.rows == apl_b.rows);
    APL_ASSERT(adl_b.cols == apl_b.cols);
    APL_ASSERT(adl_b.stride_r == apl_b.stride_r);

    for (size_t i = 0; i < adl_b.rows; i++) {
        for (size_t j = 0; j < adl_b.cols; j++) {
            APL_BUFFER_AT(apl_b, i, j) = ADL_BUFFER_AT(adl_b, i, j);
        }
    }
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
