#define SETUP
#define UPDATE
#define RENDER
#include "./include/display.c"
#define MATRIX2D_IMPLEMENTATION
#include "./include/Matrix2D.h"
#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "./include/Almog_Draw_Library.h"
#define ALMOG_ENGINE_IMPLEMENTATION
#include "./include/Almog_Engine.h"

Grid grid;
Grid grid_proj;
void setup(game_state_t *game_state)
{
    // game_state->to_limit_fps = 0;
    game_state->const_fps = 500;

    grid      = adl_cartesian_grid_create(-1, 1, -2, 2, 10, 20, "XZ", 1);
    grid_proj = adl_cartesian_grid_create(-1, 1, -2, 2, 10, 20, "XZ", 1);
}

void update(game_state_t *game_state)
{
    ae_projection_mat_set(game_state->scene.proj_mat, game_state->scene.camera.aspect_ratio, game_state->scene.camera.fov_deg, game_state->scene.camera.z_near, game_state->scene.camera.z_far);
    ae_view_mat_set(game_state->scene.view_mat, game_state->scene.camera, game_state->scene.up_direction);

    ae_grid_project_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, grid_proj, grid, game_state->window_w, game_state->window_h, &(game_state->scene));

}

void render(game_state_t *game_state)
{
    adl_grid_draw(game_state->window_pixels_mat, grid_proj, 0xffffffff, ADL_DEFAULT_OFFSET_ZOOM);

}
