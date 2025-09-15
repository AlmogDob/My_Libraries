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

Quad_mesh quad_mesh;
Quad_mesh proj_quad_mesh;
Tri_mesh tri_mesh;
Tri_mesh proj_tri_mesh;

void setup(game_state_t *game_state)
{
    game_state->const_fps = 30;
    game_state->to_limit_fps = 0;

    ada_init_array(Quad, quad_mesh);
    ada_init_array(Tri, tri_mesh);
    ada_init_array(Quad, proj_quad_mesh);
    ada_init_array(Tri, proj_tri_mesh);
    Quad quad;
    Tri tri;

    quad.points[3] = (Point){-2  , -1  , 1, 1};
    quad.points[2] = (Point){-1  , -0.5, 1, 1};
    quad.points[1] = (Point){-1.5, 1   , 1, 1};
    quad.points[0] = (Point){-2.5, 0.5 , 1, 1};
    quad.to_draw = true;
    quad.light_intensity = 1;
    quad.colors[0] = 0xFFFFFF;
    quad.colors[1] = 0x0000FF;
    quad.colors[2] = 0x00FF00;
    quad.colors[3] = 0xFF0000;

    ada_appand(Quad, quad_mesh, quad);

    tri.points[2] = (Point){1  , -1  , 1, 1};
    tri.points[1] = (Point){2  , -0.5, 1, 1};
    tri.points[0] = (Point){0.5, 1   , 1, 1};
    tri.to_draw = true;
    tri.light_intensity = 1;
    tri.colors[0] = 0xFFFFFF;
    tri.colors[1] = 0x0000FF;
    tri.colors[2] = 0x00FF00;
    // tri.colors[0] = 0xFFFFFF;
    // tri.colors[1] = 0xFFFFFF;
    // tri.colors[2] = 0xFFFFFF;

    ada_appand(Tri, tri_mesh, tri);

}

void update(game_state_t *game_state)
{
    ae_set_projection_mat(game_state->scene.proj_mat, game_state->scene.camera.aspect_ratio, game_state->scene.camera.fov_deg, game_state->scene.camera.z_near, game_state->scene.camera.z_far);
    ae_set_view_mat(game_state->scene.view_mat, game_state->scene.camera, game_state->scene.up_direction);

    ae_project_quad_mesh_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, &proj_quad_mesh, quad_mesh, game_state->window_w, game_state->window_h, game_state->scene.light_direction, &(game_state->scene));
    ae_project_tri_mesh_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, &proj_tri_mesh, tri_mesh, game_state->window_w, game_state->window_h, game_state->scene.light_direction, &(game_state->scene));
}

void render(game_state_t *game_state)
{

    adl_fill_quad_mesh_interpolate_color(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, proj_quad_mesh, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_quad_mesh(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, proj_quad_mesh, 0x0, ADL_DEFAULT_OFFSET_ZOOM);

    adl_fill_tri_mesh_Pinedas_rasterizer_interpolate_color(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, proj_tri_mesh, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_tri_mesh(game_state->window_pixels_mat, proj_tri_mesh, 0x0, ADL_DEFAULT_OFFSET_ZOOM);

    // adl_fill_quad_mesh_interpolate_color(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, proj_quad_mesh, ADL_DEFAULT_OFFSET_ZOOM);
    // adl_fill_tri_mesh_Pinedas_rasterizer_interpolate_color(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, proj_tri_mesh, ADL_DEFAULT_OFFSET_ZOOM);

    proj_tri_mesh.length  = 0;
    proj_quad_mesh.length = 0;
    
}

