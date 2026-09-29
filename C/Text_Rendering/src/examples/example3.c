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

#define APL_ATR_SINGLE_PRECISION
#define APL_ATR_BRIDGE_IMPLEMENTATION
#include "../includes/APL_ATR_BRIDGE.h"
#define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#include "../includes/Almog_Platform_Library.h"
#define ALMOG_TEXT_RENDERING_IMPLEMENTATION
#include "../includes/Almog_Text_Rendering.h"


struct Atr_Offset_Zoom offzoom = {0};
struct Atr_Font font = {0};

enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
    ws->to_limit_fps = false;
    offzoom = ATR_DEFAULT_OFFSET_ZOOM;


    /* hebrew */
    // char font_file_name[] = "../src/fonts/ankaclm-bold-webfont.ttf";
    // char font_file_name[] = "../src/fonts/stamashkenazclm-webfont.ttf";
    // char font_file_name[] = "../src/fonts/nehama-webfont.ttf";

    /* hebrew + english */
    // char font_file_name[] = "../src/fonts/VarelaRound-Regular.ttf";

    /* english */
    // char font_file_name[] = "../src/fonts/BLKCHCRY.ttf";
    // char font_file_name[] = "../src/fonts/Canterbury.ttf";
    // char font_file_name[] = "../src/fonts/ChopinScript.ttf";
    // char font_file_name[] = "../src/fonts/Inconsolata-Regular.ttf";
    // char font_file_name[] = "../src/fonts/Montague.ttf";
    // char font_file_name[] = "../src/fonts/MotionPicture_PersonalUseOnly.ttf";
    // char font_file_name[] = "../src/fonts/Motterdam-K74zp.ttf";
    // char font_file_name[] = "../src/fonts/PlayfairDisplay-Regular.ttf";
    // char font_file_name[] = "../src/fonts/Scabber-q2Mn0.ttf";
    // char font_file_name[] = "../src/fonts/Symbola.ttf";
    // char font_file_name[] = "../src/fonts/waltographUI.ttf";

    /* japanese + english */
    // char font_file_name[] = "../src/fonts/MPLUSU-VariableFont_wght.ttf";
    char font_file_name[] = "../src/fonts/KaiseiTokumin-Regular.ttf";

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
    struct Atr_Pixel_Buffer font_pixels = apl_pixel_buffer_as_atr_pixel_buffer(ws->window_pixels_mat);

    char str1[] = "いろはにほへと ちりぬるを わかよたれそ つねならむ うゐのおくやま けふこえて あさきゆめみし ゑひもせす";
    char str2[] = "the quick brown fox jumps over the lazy dog! @#$%^&*:\"{}[]?><\\/';.()_+-";

    atr_real top_left_x = 10, top_left_y = 10, letter_hight = 100, spacing = 10;
    struct Atr_Vec2 bounding_box1 = atr_text_line_draw_outline(font_pixels, &font, (uint8_t *)str1, top_left_x, top_left_y, letter_hight, spacing, 0xFFFFFFFF, -1, (offzoom));
    atr_text_line_draw_outline(font_pixels, &font, (uint8_t *)str2, top_left_x, top_left_y + bounding_box1.y + top_left_y, letter_hight, spacing, 0xFFFFFFFF, -1, (offzoom));

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
        offzoom = ATR_DEFAULT_OFFSET_ZOOM;
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
    amd_debug_mem_print(0);
    amd_debug_mem_reset();


    return APL_SUCCESS;
}
