#define SETUP
#define UPDATE
#define RENDER
#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "./Almog_Draw_Library.h"
#include "./display.c"
#define MATRIX2D_IMPLEMENTATION
#include "./Matrix2D.h"

Quad quad1;
Tri tri;

void setup(game_state_t *game_state)
{
    // game_state->const_fps = 30;
    game_state->to_limit_fps = 0;

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

    tri.points[2] = (Point){750 , 100, 1, 1};
    tri.points[1] = (Point){1250, 700, 1, 1};
    tri.points[0] = (Point){650 , 500, 1, 1};
    tri.to_draw = true;
    tri.light_intensity[0] = 1;
    tri.light_intensity[1] = 1;
    tri.light_intensity[2] = 1;
    tri.colors[0] = 0xFFFFFFFF;
    tri.colors[1] = 0xFF0000FF;
    tri.colors[2] = 0xFF00FF00;

}

void update(game_state_t *game_state)
{
    SDL_Delay(1);
}

void render(game_state_t *game_state)
{
    adl_quad_fill_interpolate_color_mean_value(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, quad1, game_state->offset_zoom_param);
    adl_quad_draw(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, quad1, 0xFF000000, game_state->offset_zoom_param);

    adl_tri_fill_Pinedas_rasterizer_interpolate_color(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, tri, game_state->offset_zoom_param);
    adl_tri_draw(game_state->window_pixels_mat, tri, 0xff000000, game_state->offset_zoom_param);

    #if 0
    Mat2D inv_z_buffer = game_state->inv_z_buffer_mat;
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
            uint32_t color = RGB_hexRGB(0xFF*z_fraq, 0xFF*z_fraq, 0xFF*z_fraq); 
            MAT2D_AT_UINT32(game_state->window_pixels_mat, i, j) = color;
        }
    }
    #endif
}

