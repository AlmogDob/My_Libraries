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
#define ALMOG_SHAPES_IMPLEMENTATION
#include "./include/Almog_Shapes.h"

Curve circle;
Curve proj_circle;

void setup(game_state_t *game_state)
{
    // game_state->to_limit_fps = 0;
    game_state->const_fps = 30;

    circle = as_circle_create((Point){0,0,0,0}, 1, 100, 0xffffffff, "XY");
    proj_circle = as_circle_create((Point){0,0,0,0}, 1, 100, 0xffffffff, "XY");

    // ae_curve_copy(&proj_circle, circle);    

    // AS_CURVE_PRINT(circle);
    // AS_CURVE_PRINT(proj_circle);
}

void update(game_state_t *game_state)
{
    ae_projection_mat_set(game_state->scene.proj_mat, game_state->scene.camera.aspect_ratio, game_state->scene.camera.fov_deg, game_state->scene.camera.z_near, game_state->scene.camera.z_far);
    ae_view_mat_set(game_state->scene.view_mat, game_state->scene.camera, game_state->scene.up_direction);

    ae_curve_project_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, &proj_circle, circle, game_state->window_w, game_state->window_h, &(game_state->scene));

}

void render(game_state_t *game_state)
{
    adl_lines_loop_draw(game_state->window_pixels_mat, proj_circle.elements, proj_circle.length, proj_circle.color, ADL_DEFAULT_OFFSET_ZOOM);

    // adl_tri_mesh_fill_Pinedas_rasterizer_interpolate_normal(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, proj_cube, 0xffffffff, ADL_DEFAULT_OFFSET_ZOOM);

    proj_circle.length = 0;
}