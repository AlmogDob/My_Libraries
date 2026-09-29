#include <stdio.h>
#include <stdbool.h>

#define APL_SETUP
#define APL_INPUT
#define APL_UPDATE
#define APL_RENDER
#define MATRIX2D_IMPLEMENTATION
#define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#include "../../src/Almog_Platform_Library.h"
#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "../../src/Almog_Draw_Library.h"

Quad quad1;
Tri tri;

enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
    // ws->to_limit_fps = 0;

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
    adl_quad_fill_interpolate_color_mean_value(ws->window_pixels_mat, ws->inv_z_buffer_mat, quad1, ADL_DEFAULT_OFFSET_ZOOM);
    adl_quad_draw(ws->window_pixels_mat, ws->inv_z_buffer_mat, quad1, 0xFF000000, ADL_DEFAULT_OFFSET_ZOOM);

    adl_tri_fill_Pinedas_rasterizer_interpolate_color(ws->window_pixels_mat, ws->inv_z_buffer_mat, tri, ADL_DEFAULT_OFFSET_ZOOM);
    adl_tri_draw(ws->window_pixels_mat, tri, 0xff000000, ADL_DEFAULT_OFFSET_ZOOM);

    #if 1
    Mat2D inv_z_buffer = ws->inv_z_buffer_mat;
    double max_inv_z = 0;
    double min_inv_z = DBL_MAX;
    for (size_t i = 0; i < inv_z_buffer.rows; i++) {
        for (size_t j = 0; j < inv_z_buffer.cols; j++) {
            if (MAT2D_AT(inv_z_buffer, i, j) > max_inv_z) {
                max_inv_z = MAT2D_AT(inv_z_buffer, i, j);
            }
            if (MAT2D_AT(inv_z_buffer, i, j) < min_inv_z && MAT2D_AT(inv_z_buffer, i, j) > 0) {
                min_inv_z = MAT2D_AT(inv_z_buffer, i, j);
            }
        }
    }
    for (size_t i = 0; i < inv_z_buffer.rows; i++) {
        for (size_t j = 0; j < inv_z_buffer.cols; j++) {
            double z_fraq = MAT2D_AT(inv_z_buffer, i, j);
            z_fraq = fmax(z_fraq, min_inv_z);
            z_fraq = adl_linear_map(z_fraq, min_inv_z, max_inv_z, 0.1, 1);
            uint32_t color = ADL_RGB_hexRGB(0xFF*z_fraq, 0xFF*z_fraq, 0xFF*z_fraq); 
            MAT2D_AT(ws->window_pixels_mat, i, j) = color;
        }
    }
    #endif

    return APL_SUCCESS;
}
