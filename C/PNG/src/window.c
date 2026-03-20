#include <stdio.h>
#include <stdbool.h>

#define APL_SETUP
#define APL_INPUT
#define APL_UPDATE
#define APL_RENDER
#define MATRIX2D_IMPLEMENTATION
#define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#include "include/Almog_Platform_Library.h"

#define ADL_ASSERT APL_ASSERT
#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "include/Almog_Draw_Library.h"

#define APNG_ASSERT APL_ASSERT
#define ALMOG_PNG_IMPLEMENTATION
#include "include/Almog_PNG.h"

enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
    ws->wanted_fps = 40;
    ws->to_limit_fps = false;
    APL_UNUSED(ws);


    char file_name[] = "../src/test-png.png";
    printf("Loading PNG '%s'.\n", file_name);

    struct Apng_Bin_String file = apng_bin_file_read(file_name);
    apng_decode_png(file);


    apng_bin_string_free(file);

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_update(struct Apl_Window_State *ws)
{
    APL_UNUSED(ws);

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_render(struct Apl_Window_State *ws)
{
    adl_circle_fill(ws->window_pixels_mat, 100, 100, 100, APL_COLOR_WHITE_hexARGB, ADL_DEFAULT_OFFSET_ZOOM);
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
