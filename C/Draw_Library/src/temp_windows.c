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

Quad quad1;
Tri tri;

Offset_zoom_param offzoom = {0};
enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
    ws->to_limit_fps = 0;
    offzoom = ADL_DEFAULT_OFFSET_ZOOM;

    quad1.points[3] = (Point){200, 100, 1, 1};
    quad1.points[2] = (Point){600, 50 , 1, 1};
    quad1.points[1] = (Point){200, 700, 1, 1};
    quad1.points[0] = (Point){100, 300, 1, 1};
    quad1.to_draw = true;
    quad1.light_intensity[0] = 1;
    quad1.light_intensity[1] = 1;
    quad1.light_intensity[2] = 1;
    quad1.light_intensity[3] = 1;
    quad1.colors[0] = 0xFFFFFFFF;
    quad1.colors[1] = 0xFF0000FF;
    quad1.colors[2] = 0xFF00FF00;
    quad1.colors[3] = 0xFFFF0000;

    tri.points[2] = (Point){750 , 100, 2, 1};
    tri.points[1] = (Point){1250, 700, 2, 1};
    tri.points[0] = (Point){650 , 500, 0.5, 1};
    tri.to_draw = true;
    tri.light_intensity[0] = 1;
    tri.light_intensity[1] = 1;
    tri.light_intensity[2] = 1;
    tri.colors[0] = 0xFFFFFFFF;
    tri.colors[1] = 0xFF0000FF;
    tri.colors[2] = 0xFF00FF00;

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_input(struct Apl_Window_State *ws)
{
    if (ws->buttons.e_is_pressed) {
        offzoom.zoom_multiplier *= 1.1;
        ws->to_render = true;
    } else if (ws->buttons.q_is_pressed) {
        offzoom.zoom_multiplier /= 1.1;
        ws->to_render = true;
    } else if (ws->buttons.r_is_pressed) {
        offzoom = ADL_DEFAULT_OFFSET_ZOOM;
        ws->to_render = true;
    } else if (ws->buttons.d_is_pressed) {
        offzoom.offset_x -= ws->window_pixels_mat.cols / 100;
        ws->to_render = true;
    } else if (ws->buttons.a_is_pressed) {
        offzoom.offset_x += ws->window_pixels_mat.cols / 100;
        ws->to_render = true;
    } else if (ws->buttons.s_is_pressed) {
        offzoom.offset_y -= ws->window_pixels_mat.rows / 100;
        ws->to_render = true;
    } else if (ws->buttons.w_is_pressed) {
        offzoom.offset_y += ws->window_pixels_mat.rows / 100;
        ws->to_render = true;
    }
    APL_UNUSED(ws);

    return APL_SUCCESS;

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_update(struct Apl_Window_State *ws)
{
    APL_UNUSED(ws);

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_render(struct Apl_Window_State *ws)
{
    adl_quad_fill_interpolate_color_mean_value(ws->window_pixels_mat, ws->inv_z_buffer_mat, quad1, offzoom);
    adl_quad_draw(ws->window_pixels_mat, ws->inv_z_buffer_mat, quad1, 0xFF000000, offzoom);

    adl_tri_fill_Pinedas_rasterizer_interpolate_color(ws->window_pixels_mat, ws->inv_z_buffer_mat, tri, offzoom);
    adl_tri_draw(ws->window_pixels_mat, tri, 0xff000000, offzoom);

    return APL_SUCCESS;
}
