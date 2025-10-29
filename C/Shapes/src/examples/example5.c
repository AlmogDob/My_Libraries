#define SETUP
#define UPDATE
#define RENDER
#define DESTROY
#include "../include/display.c"
#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "../include/Almog_Draw_Library.h"
#define ALMOG_ENGINE_IMPLEMENTATION
#include "../include/Almog_Engine.h"
#define MATRIX2D_IMPLEMENTATION
#include "../include/Matrix2D.h"
#define ALMOG_DELAUNAY_TRIANGULATION_IMPLEMENTATION
#include "../include/Almog_Delaunay_Triangulation.h"

#include <time.h>

Tri_mesh mesh, proj_mesh;
Curve_ada circles, proj_circles;

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;
    game_state->const_fps = 30;

    ada_init_array(Tri, proj_mesh);

    Curve c = as_curve_create_random_points(1000, -2, 2, -2, 2, 0, 0, time(NULL));

    Tri_edge_implicit_mesh tei_mesh = adt_tri_edge_implicit_mesh_make_Delaunay_triangulation_flip_algorithm(c.elements, c.length);

    Edge_ada edge_list = {0};
    ada_init_array(Edge, edge_list);

    size_t p1_index = 1;
    size_t p2_index = 100;
    Edge edge = (Edge){(Point)tei_mesh.points.elements[p1_index], (Point)tei_mesh.points.elements[p2_index], 0};
    ada_appand(Edge, edge_list, edge);

    p1_index = 100;
    p2_index = 500;
    edge = (Edge){(Point)tei_mesh.points.elements[p1_index], (Point)tei_mesh.points.elements[p2_index], 0};
    ada_appand(Edge, edge_list, edge);

    p1_index = 500;
    p2_index = 900;
    edge = (Edge){(Point)tei_mesh.points.elements[p1_index], (Point)tei_mesh.points.elements[p2_index], 0};
    ada_appand(Edge, edge_list, edge);

    p1_index = 900;
    p2_index = 1;
    edge = (Edge){(Point)tei_mesh.points.elements[p1_index], (Point)tei_mesh.points.elements[p2_index], 0};
    ada_appand(Edge, edge_list, edge);

    adt_tri_edge_implicit_mesh_insert_segment_array(&tei_mesh, edge_list.elements, edge_list.length, AS_EPSILON);

    free(edge_list.elements);

    // AS_TRI_EDGE_IMPLICIT_MESH_PRINT_SEGMENTS(tei_mesh);

    dprintINT(adt_tri_edge_implicit_mesh_check_Delaunay(tei_mesh));


    mesh = as_tri_edge_implicit_mesh_to_tri_mesh(tei_mesh, 1, 0xffffffff);


    ada_init_array(Curve, circles);
    ada_init_array(Curve, proj_circles);


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

