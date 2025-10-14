#define SETUP
#define UPDATE
#define RENDER
#include "./include/display.c"
#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "./include/Almog_Draw_Library.h"
#define ALMOG_ENGINE_IMPLEMENTATION
#include "./include/Almog_Engine.h"
#define MATRIX2D_IMPLEMENTATION
#include "./include/Matrix2D.h"
#define ALMOG_SHAPES_IMPLEMENTATION
#include "./include/Almog_Shapes.h"

#include <time.h>

Tri_mesh mesh, proj_mesh;
Curve_ada circles, proj_circles;

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;
    game_state->const_fps = 30;

    ada_init_array(Tri, proj_mesh);

    Curve c = as_curve_create_random_points(5, -2, 2, -2, 2, 0, 0, 1);

    Tri_implicit_mesh temp_implicit_mesh = as_points_array_get_lexicographic_triangulation(c.elements, c.length);

    as_tri_implicit_mesh_set_Delaunay_triangulation_flip_algorithm(temp_implicit_mesh);





    mesh = as_tri_implicit_mesh_to_tri_mesh(temp_implicit_mesh, 1, 0xffffffff);

    ada_init_array(Curve, circles);
    ada_init_array(Curve, proj_circles);
    // for (size_t i = 0; i < mesh.length; i++) {
    //     float t = (float)i / ((float)mesh.length - 1);
    //     float t1 = t < 0.5 ? t * 2 : 1;
    //     float t2 = t >= 0.5 ? (t-0.5) * 2 : 1;
    //     Tri tri = mesh.elements[i];
    //     Point center = {0};
    //     float r = 0;
    //     as_tri_get_circumcircle(tri.points[0], tri.points[1], tri.points[2], "xy", &center, &r);
    //     Curve temp_curve = as_circle_curve_create(center, r, 500, RGBA_hexARGB(255.0f * t, 255 * (1-t1), 255 * (t2), 255), "xy");
    //     Curve temp_proj_curve = as_circle_curve_create(center, r, 500, RGBA_hexARGB(255.0f * t, 255 * (1-t1), 255 * (t2), 255), "xy");
    //     ada_appand(Curve, circles, temp_curve);
    //     ada_appand(Curve, proj_circles, temp_proj_curve);
    // }

}

void update(game_state_t *game_state)
{
    ae_projection_mat_set(game_state->scene.proj_mat, game_state->scene.camera.aspect_ratio, game_state->scene.camera.fov_deg, game_state->scene.camera.z_near, game_state->scene.camera.z_far);
    ae_view_mat_set(game_state->scene.view_mat, game_state->scene.camera, game_state->scene.up_direction);

    ae_tri_mesh_project_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, &(proj_mesh), mesh, game_state->window_w, game_state->window_h, &(game_state->scene), AE_LIGHTING_FLAT);

    ae_curve_ada_project_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, &proj_circles, circles, game_state->window_w, game_state->window_h, &(game_state->scene));
}

void render(game_state_t *game_state)
{
    adl_tri_mesh_fill_Pinedas_rasterizer(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, proj_mesh, 0xffffffff, ADL_DEFAULT_OFFSET_ZOOM);
    adl_tri_mesh_draw(game_state->window_pixels_mat, proj_mesh, 0xff000000, ADL_DEFAULT_OFFSET_ZOOM);

    for (size_t i = 0; i < proj_circles.length; i++) {
        adl_lines_loop_draw(game_state->window_pixels_mat, proj_circles.elements[i].elements, proj_circles.elements[i].length, proj_circles.elements[i].color, ADL_DEFAULT_OFFSET_ZOOM);
    }
}

