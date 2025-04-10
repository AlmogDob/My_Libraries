#define SETUP
#define UPDATE
#define RENDER
#include "display.c"

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;

    game_state->window_surface = SDL_GetWindowSurface(game_state->window);

    game_state->window_pixels_mat = mat2D_alloc(game_state->window_h, game_state->window_w);


}

void update(game_state_t *game_state)
{
    for (size_t j = 0; j < game_state->window_pixels_mat.cols; j++) {
        float t = (float)j/(game_state->window_pixels_mat.cols-1);
        for (size_t i = 0; i < game_state->window_pixels_mat.rows; i++) {
            float n = (float)i/(game_state->window_pixels_mat.rows-1);
            float m = (float)(i + j) / (game_state->window_pixels_mat.rows + game_state->window_pixels_mat.cols - 2);
            uint32_t color = RGB_hexRGB((int)(255*t), (int)(255*(game_state->elapsed_time/5)*(1-m)), (int)(255*(n)));

            // float n = (float)i/(game_state->window_pixels_mat.rows-1);
            // uint32_t color = RGB_hexRGB((int)(255*(1-n)), (int)(255*(t)), 0);

            MAT2D_AT(game_state->window_pixels_mat, i, j) = color;
        }
    }
}

void render(game_state_t *game_state)
{
    copy_mat_to_surface_RGB(game_state);
    SDL_UpdateWindowSurface(game_state->window);
}
