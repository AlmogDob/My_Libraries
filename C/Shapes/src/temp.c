#define SETUP
#define UPDATE
#define RENDER
#define DESTROY
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

    // Curve c = as_curve_create_random_points(500, -2, 2, -2, 2, 0, 0, 20);
    Curve c = as_curve_create_random_points(1000, -2, 2, -2, 2, 0, 0, 20);

    Tri_edge_implicit_mesh tei_mesh = as_tri_edge_implicit_mesh_make_Delaunay_triangulation_flip_algorithm(c.elements, c.length);
    // Tri_edge_implicit_mesh tei_mesh = as_tri_edge_implicit_mesh_make_Delaunay_triangulation_flip_algorithm_fixed_iterations(c.elements, c.length);

    // size_t p1_index = 10;
    // size_t p2_index = 950;
    // as_tri_edge_implicit_mesh_insert_segment(&tei_mesh, tei_mesh.points.elements[p1_index], tei_mesh.points.elements[p2_index]);

    AS_TRI_EDGE_IMPLICIT_MESH_PRINT_SEGMENTS(tei_mesh);

    dprintINT(as_tri_edge_implicit_mesh_check_Delaunay(tei_mesh));


    mesh = as_tri_edge_implicit_mesh_to_tri_mesh(tei_mesh, 1, 0xffffffff);



    // mesh = as_tri_implicit_mesh_to_tri_mesh(temp_implicit_mesh, 1, 0xffffffff);

    ada_init_array(Curve, circles);
    ada_init_array(Curve, proj_circles);

    // size_t temp_indexes[] = {p1_index, p2_index, 19, 20};
    // for (int i = 0; i < sizeof(temp_indexes) / sizeof(temp_indexes[0]); i++) {
    //     Curve temp_curve = as_circle_curve_create(tei_mesh.points.elements[temp_indexes[i]], 0.04, 100, 0xffff0000, "xy");
    //     Curve temp_proj_curve = as_circle_curve_create(tei_mesh.points.elements[temp_indexes[i]], 0.02, 100, 0xffff0000, "xy");
    //     ada_appand(Curve, circles, temp_curve);
    //     ada_appand(Curve, proj_circles, temp_proj_curve);
    // }

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

    as_tri_edge_implicit_mesh_free(tei_mesh);
    free(c.elements);
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

    for (size_t i = 0; i < proj_mesh.length; i++) {
        Tri current_tri = proj_mesh.elements[i];
        for (int j = 0; j < 3; j++) {
            adl_circle_fill(game_state->window_pixels_mat, current_tri.points[j].x, current_tri.points[j].y, 4, 0xffff0000, ADL_DEFAULT_OFFSET_ZOOM);
        }
    }

    for (size_t i = 0; i < proj_circles.length; i++) {
        adl_lines_loop_draw(game_state->window_pixels_mat, proj_circles.elements[i].elements, proj_circles.elements[i].length, proj_circles.elements[i].color, ADL_DEFAULT_OFFSET_ZOOM);
    }
}

void destroy(game_state_t *game_state)
{
    free(mesh.elements);
    free(proj_mesh.elements);

    as_curve_ada_free(circles);
    as_curve_ada_free(proj_circles);

    (void)game_state;
}

