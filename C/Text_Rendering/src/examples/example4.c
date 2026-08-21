#include <stdio.h>
#include <stdbool.h>

#define APL_SETUP
#define APL_UPDATE
#define APL_RENDER
#define APL_INPUT
#define APL_DESTROY

#define AMD_MEMORY_DEBUG
#define ALMOG_MEMORY_DEBUG_IMPLEMENTATION
#include "../includes/Almog_Memory_Debug.h"

#define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#include "../includes/Almog_Platform_Library.h"

#define ADL_ASSERT APL_ASSERT
#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "../includes/Almog_Draw_Library.h"

#define ATR_ASSERT APL_ASSERT
#define ALMOG_TEXT_RENDERING_IMPLEMENTATION
#include "../includes/Almog_Text_Rendering.h"

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
struct Atr_Font font = {0};

enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
    ws->to_limit_fps = false;
    offzoom = ADL_DEFAULT_OFFSET_ZOOM;

    // char font_file_name[] = "../src/fonts/nehama-webfont.ttf";

    char font_file_name[] = "../src/fonts/MPLUSU-VariableFont_wght.ttf";

    /* english */
    // char font_file_name[] = "../src/fonts/BLKCHCRY.ttf";
    // char font_file_name[] = "../src/fonts/Canterbury.ttf";
    // char font_file_name[] = "../src/fonts/Inconsolata-Regular.ttf";
    // char font_file_name[] = "../src/fonts/Symbola.ttf";
    // char font_file_name[] = "../src/fonts/waltographUI.ttf";

    if (ATR_FAIL == atr_font_load_from_file_name(&font, font_file_name)) {
        atr_dprintERROR("Failed to load font from file '%s'.", font_file_name);
        return APL_FAIL;
    }

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
    struct Atr_Pixel_Buffer font_pixels = adl_pixel_buffer_as_atr_pixel_buffer(pixels);

    char str1[] = "the quick brown fox jumps over the lazy dog! @#$%^&*:\"{}[]?><\\/';.()_+-";
    // char str1[] = "אלמוג";
    // char str1[] = "いろはにほへと ちりぬるを わかよたれそ つねならむ うゐのおくやま けふこえて あさきゆめみし ゑひもせす";

    atr_real top_left_x = 10, top_left_y = 10, letter_hight = 50, spacing = 10;
    struct Atr_Vec2 bounding_box1 = atr_text_line_draw_no_antialiasing(font_pixels, &font, (uint8_t *)str1, top_left_x, top_left_y, letter_hight, spacing, ADL_COLOR_WHITE_hexARGB, -1, adl_offset_zoom_to_atr_offset_zoom(offzoom));
    // atr_text_line_draw_outline(font_pixels, &font, (uint8_t *)str1, top_left_x, top_left_y, letter_hight, spacing, ADL_COLOR_RED_hexARGB, -1, adl_offset_zoom_to_atr_offset_zoom(offzoom));
    adl_rectangle_draw_min_max(pixels, top_left_x, top_left_x + bounding_box1.x, top_left_y, top_left_y + bounding_box1.y, ADL_COLOR_WHITE_hexARGB, offzoom);

    // return APL_FAIL;

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

enum Apl_Return_Types apl_destroy(struct Apl_Window_State *ws)
{
    atr_font_free(&font);
    free(ws->window_pixels_mat.elements);
    free(ws->inv_z_buffer_mat.elements);

    if (AMD_FAIL == amd_debug_mem()) {
        amd_dprintERROR("%s", "Corrupted memory detected.");
        return APL_FAIL;
    }
    // amd_debug_mem_print(0);
    amd_debug_mem_reset();


    return APL_SUCCESS;
}
