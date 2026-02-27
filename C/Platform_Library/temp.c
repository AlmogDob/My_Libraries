#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define APL_SETUP
#define APL_UPDATE
#define APL_RENDER
#define APL_DEFINE_ALL_IMPLEMENTATIONS
#include "Almog_Platform_Library.h"


enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
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
    APL_UNUSED(ws);

    return APL_SUCCESS;
}
