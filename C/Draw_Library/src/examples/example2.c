#include <stdio.h>
#include <stdbool.h>

#define APL_SETUP
#define APL_INPUT
#define APL_UPDATE
#define APL_RENDER

#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "../includes/Almog_Draw_Library.h"

#define apl_real adl_real
#define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#include "../includes/Almog_Platform_Library.h"


struct Adl_Pixel_Buffer apl_pixel_buffer_as_adl_pixel_buffer(struct Apl_Pixel_Buffer apl_b) 
{
    struct Adl_Pixel_Buffer adl_b = {
        .cols = apl_b.cols,
        .rows = apl_b.rows,
        .stride_r = apl_b.stride_r,
        .elements = apl_b.elements,
    };

    return adl_b;
}

struct Adl_Offset_Zoom offzoom = {0};

enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
    ws->to_limit_fps = true;
    ws->wanted_fps = 60;
    offzoom = ADL_DEFAULT_OFFSET_ZOOM;


    return APL_SUCCESS;
}

enum Apl_Return_Types apl_update(struct Apl_Window_State *ws)
{
    APL_UNUSED(ws);

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_render(struct Apl_Window_State *ws)
{
    struct Adl_Pixel_Buffer pixels = apl_pixel_buffer_as_adl_pixel_buffer(ws->window_pixels_mat);


    struct Adl_Point p0 = {.x = 200, .y = 100};
    struct Adl_Point p1 = {.x = 200 + ((adl_real)ws->elapsed_time_micro_sec / 50 / 1000), .y = 250};
    struct Adl_Point p2 = {.x = 340, .y = 300};
    adl_tri_draw(pixels, p0, p1, p2, ADL_COLOR_WHITE_hexARGB, offzoom);

    struct Adl_Point p0_ = {.x = 200, .y = 300};
    struct Adl_Point p1_ = {.x = 200 + ((adl_real)ws->elapsed_time_micro_sec / 50 / 1000), .y = 450};
    struct Adl_Point p2_ = {.x = 340, .y = 500};
    adl_tri_draw_no_antialiasing(pixels, p0_, p1_, p2_, ADL_COLOR_WHITE_hexARGB, offzoom);


    return APL_SUCCESS;
}

enum Apl_Return_Types apl_input(struct Apl_Window_State *ws)
{
    if (ws->buttons.e_is_pressed) {
        offzoom.zoom_multiplier *= 1.1f;
        ws->to_render = true;
    } else if (ws->buttons.q_is_pressed) {
        offzoom.zoom_multiplier /= 1.1f;
        ws->to_render = true;
    } else if (ws->buttons.r_is_pressed) {
        offzoom = ADL_DEFAULT_OFFSET_ZOOM;
        ws->to_render = true;
    } else if (ws->buttons.d_is_pressed) {
        offzoom.offset_x -= 1 / offzoom.zoom_multiplier * ws->window_pixels_mat.cols / 100;
        ws->to_render = true;
    } else if (ws->buttons.a_is_pressed) {
        offzoom.offset_x += 1 / offzoom.zoom_multiplier * ws->window_pixels_mat.cols / 100;
        ws->to_render = true;
    } else if (ws->buttons.s_is_pressed) {
        offzoom.offset_y -= 1 / offzoom.zoom_multiplier * ws->window_pixels_mat.rows / 100;
        ws->to_render = true;
    } else if (ws->buttons.w_is_pressed) {
        offzoom.offset_y += 1 / offzoom.zoom_multiplier * ws->window_pixels_mat.rows / 100;
        ws->to_render = true;
    }
    APL_UNUSED(ws);

    return APL_SUCCESS;
}
