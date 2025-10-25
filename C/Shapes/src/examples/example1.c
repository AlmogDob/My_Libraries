#define SETUP
#define UPDATE
#define RENDER
#define DESTROY
#include "../include/display.c"
#define MATRIX2D_IMPLEMENTATION
#include "../include/Matrix2D.h"
#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "../include/Almog_Draw_Library.h"
#define ALMOG_ENGINE_IMPLEMENTATION
#include "../include/Almog_Engine.h"
#define ALMOG_SHAPES_IMPLEMENTATION
#include "../include/Almog_Shapes.h"

Curve circle;
Curve proj_circle;

Curve_ada sphere;
Curve_ada proj_sphere;

void setup(game_state_t *game_state)
{
    // game_state->to_limit_fps = 0;
    game_state->const_fps = 30;

    circle = as_circle_curve_create((Point){0,0,0,0}, 1.5, 100, 0xffffffff, "XY");
    proj_circle = as_circle_curve_create((Point){0,0,0,0}, 1, 100, 0xffffffff, "XY");


    int temp = 30;
    sphere = as_sphere_curve_ada_create((Point){0,0,0,0}, 1, temp, temp, 0xffffffff);
    proj_sphere = as_sphere_curve_ada_create((Point){0,0,0,0}, 1, temp, temp, 0xffffffff);
}

void update(game_state_t *game_state)
{
    ae_projection_mat_set(game_state->scene.proj_mat, game_state->scene.camera.aspect_ratio, game_state->scene.camera.fov_deg, game_state->scene.camera.z_near, game_state->scene.camera.z_far);
    ae_view_mat_set(game_state->scene.view_mat, game_state->scene.camera, game_state->scene.up_direction);



    ae_curve_project_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, &proj_circle, circle, game_state->window_w, game_state->window_h, &(game_state->scene));

    ae_curve_ada_project_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, &proj_sphere, sphere, game_state->window_w, game_state->window_h, &(game_state->scene));

}

void render(game_state_t *game_state)
{
    adl_lines_loop_draw(game_state->window_pixels_mat, proj_circle.elements, proj_circle.length, proj_circle.color, ADL_DEFAULT_OFFSET_ZOOM);

    for (size_t i = 0; i < sphere.length; i++) {
        adl_lines_loop_draw(game_state->window_pixels_mat, proj_sphere.elements[i].elements, proj_sphere.elements[i].length, proj_sphere.elements[i].color, ADL_DEFAULT_OFFSET_ZOOM);
    }

}

void destroy(game_state_t *game_state)
{
    free(circle.elements);
    free(proj_circle.elements);

    as_curve_ada_free(sphere);
    as_curve_ada_free(proj_sphere);

    (void)game_state;
}