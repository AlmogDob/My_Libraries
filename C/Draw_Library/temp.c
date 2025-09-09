#define SETUP
#define UPDATE
#define RENDER
#include "./display.c"
#define MATRIX2D_IMPLEMENTATION
#include "./Matrix2D.h"
#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "./Almog_Draw_Library.h"


Quad quad1;
Quad quad2;
void setup(game_state_t *game_state)
{
    game_state->const_fps = 30;
    game_state->to_limit_fps = 0;

    quad1.points[0] = (Point){100, 100, 1, 1};
    quad1.points[1] = (Point){600, 50 , 1, 1};
    quad1.points[2] = (Point){300, 700, 1, 1};
    quad1.points[3] = (Point){70 , 300, 1, 1};
    quad1.to_draw = true;
    quad1.light_intensity = 1;
    quad1.colors[0] = 0x0000FF;
    quad1.colors[1] = 0xFFFFFF;
    quad1.colors[2] = 0x0000FF
    quad1.colors[3] = 0xFFFFFF;

    quad2.points[0] = (Point){650 + 100, 100, 1, 1};
    quad2.points[1] = (Point){650 + 600, 50 , 1, 1};
    quad2.points[2] = (Point){650 + 300, 700, 1, 1};
    quad2.points[3] = (Point){650 + 70 , 300, 1, 1};
    quad2.to_draw = true;
    quad2.light_intensity = 1;
    quad2.colors[0] = 0x0000FF;
    quad2.colors[1] = 0xFFFFFF;
    quad2.colors[2] = 0xFFFFFF;
    quad2.colors[3] = 0xFFFFFF;

}

void update(game_state_t *game_state)
{

}

void render(game_state_t *game_state)
{
    // adl_fill_quad_interpolate_color_tri(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, quad1, "02", ADL_DEFAULT_OFFSET_ZOOM);
    // adl_fill_quad_interpolate_color_tri(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, quad2, "02", ADL_DEFAULT_OFFSET_ZOOM);

    // adl_fill_quad(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, quad1, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_fill_quad_interpolate_color(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, quad1, ADL_DEFAULT_OFFSET_ZOOM);

}

