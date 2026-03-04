#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define APL_SETUP
#define APL_INPUT
#define APL_UPDATE
#define APL_RENDER
#define APL_DEFINE_ALL_IMPLEMENTATIONS
#include "Almog_Platform_Library.h"


enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
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
    adl_circle_fill(ws->window_pixels_mat, 100, 100, 100, APL_COLOR_WHITE_hexARGB, ADL_DEFAULT_OFFSET_ZOOM);

    APL_UNUSED(ws);

    return APL_SUCCESS;
}
