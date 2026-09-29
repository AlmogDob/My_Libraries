#include <stdio.h>
#include <stdbool.h>

#define APL_SETUP
#define APL_INPUT
#define APL_UPDATE
#define APL_RENDER

#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#define APL_ADL_BRIDGE_IMPLEMENTATION
#include "includes/APL_ADL_Bridge.h"


struct Adl_Offset_Zoom offzoom = {0};

enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
    ws->to_limit_fps = true;
    // ws->to_limit_fps = false;
    ws->wanted_fps = 60;
    offzoom = ADL_DEFAULT_OFFSET_ZOOM;


    return APL_SUCCESS;
}

enum Apl_Return_Types apl_update(struct Apl_Window_State *ws)
{
    APL_UNUSED(ws);

    return APL_SUCCESS;
}

adl_real xe = 0, ye = 0, r = 1;

enum Apl_Return_Types apl_render(struct Apl_Window_State *ws)
{
    struct Adl_Pixel_Buffer pixels = apl_pixel_buffer_as_adl_pixel_buffer(ws->window_pixels_mat);

    adl_line_draw_no_antialiasing(pixels, 50, 50, xe, ye, ADL_COLOR_CYAN_hexARGB, offzoom);
    adl_line_draw(pixels, 600, 100, xe, ye, ADL_COLOR_CYAN_hexARGB, offzoom);
    adl_line_draw_width(pixels, 400, 500, xe, ye, r, ADL_COLOR_CYAN_hexARGB, offzoom);

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
    } else if (ws->buttons.up_is_pressed) {
        r++;
    } else if (ws->buttons.down_is_pressed) {
        r--;
        r = adl_max(r, 1);
    }
    xe = ws->mouse.mouse_x;
    ye = ws->mouse.mouse_y;

    return APL_SUCCESS;
}
