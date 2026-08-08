#include <stdio.h>
#include <stdbool.h>

#define APL_SETUP
#define APL_INPUT
#define APL_UPDATE
#define APL_RENDER

#define ALMOG_TEXT_RENDERING_IMPLEMENTATION
#include "includes/Almog_Text_Rendering.h"

#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "includes/Almog_Draw_Library.h"

#define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#include "includes/Almog_Platform_Library.h"

struct Atr_Pixel_Buffer adl_pixel_buffer_as_atr_pixel_buffer(struct Adl_Pixel_Buffer adl_b) 
{
    struct Atr_Pixel_Buffer atr_b = {
        .cols = adl_b.cols,
        .rows = adl_b.rows,
        .stride_r = adl_b.stride_r,
        .elements = adl_b.elements,
    };

    return atr_b;
}

struct Atr_Offset_Zoom adl_offset_zoom_to_atr_offset_zoom(struct Adl_Offset_Zoom adl_offzoom)
{
    return (struct Atr_Offset_Zoom){
        .offset_x = adl_offzoom.offset_x,
        .offset_y = adl_offzoom.offset_y,
        .zoom_multiplier = adl_offzoom.zoom_multiplier,
    };
}

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
    ws->to_limit_fps = false;
    offzoom = ADL_DEFAULT_OFFSET_ZOOM;


    struct Atr_Font font = {0};
    char font_file_name[] = "../src/fonts/Inconsolata-Regular.ttf";
    if (ATR_FAIL == atr_font_load_from_file_name(&font, font_file_name)) {
        atr_dprintERROR("Failed to load font from file '%s'.", font_file_name);
    }




    ws->running = false;

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

    adl_real x = 200, y = 200, r = 150;

    adl_circle_fill(pixels, x, y, r, ADL_COLOR_WHITE_hexARGB, offzoom);

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_input(struct Apl_Window_State *ws)
{
    size_t time_delay = 60;
    if (ws->buttons.e_is_pressed) {
        offzoom.zoom_multiplier *= 1.1f;
        ws->to_render = true;
        apl_sleep(time_delay);
    } else if (ws->buttons.q_is_pressed) {
        offzoom.zoom_multiplier /= 1.1f;
        ws->to_render = true;
        apl_sleep(time_delay);
    } else if (ws->buttons.r_is_pressed) {
        offzoom = ADL_DEFAULT_OFFSET_ZOOM;
        ws->to_render = true;
        apl_sleep(time_delay);
    } else if (ws->buttons.d_is_pressed) {
        offzoom.offset_x -= 1 / offzoom.zoom_multiplier * ws->window_pixels_mat.cols / 100;
        ws->to_render = true;
        apl_sleep(time_delay);
    } else if (ws->buttons.a_is_pressed) {
        offzoom.offset_x += 1 / offzoom.zoom_multiplier * ws->window_pixels_mat.cols / 100;
        ws->to_render = true;
        apl_sleep(time_delay);
    } else if (ws->buttons.s_is_pressed) {
        offzoom.offset_y -= 1 / offzoom.zoom_multiplier * ws->window_pixels_mat.rows / 100;
        ws->to_render = true;
        apl_sleep(time_delay);
    } else if (ws->buttons.w_is_pressed) {
        offzoom.offset_y += 1 / offzoom.zoom_multiplier * ws->window_pixels_mat.rows / 100;
        ws->to_render = true;
        apl_sleep(time_delay);
    } else if (ws->buttons.space_bar_is_pressed) {
        ws->to_update = !ws->to_update;
        apl_sleep(time_delay * 2000);
    }


    return APL_SUCCESS;
}
