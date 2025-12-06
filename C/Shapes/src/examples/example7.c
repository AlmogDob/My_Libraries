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

    Curve c = as_curve_create_random_points(10, -2, 2, -2, 2, 0,0,time(0));
    // Curve c = as_curve_create_random_points(17, -2, 2, -2, 2, 0,0,20);

    Curve convex_hull = {0};
    ada_init_array(Point, convex_hull);
    as_points_array_convex_hull_Jarvis_march_2D(&convex_hull, c.elements, c.length);
    free(c.elements);

    Tri_edge_implicit_mesh tei_mesh = adt_tri_edge_implicit_mesh_make_Delaunay_triangulation_flip_algorithm(convex_hull.elements, convex_hull.length);
    free(convex_hull.elements);

    // AS_CURVE_PRINT(tei_mesh.points);

    adt_tri_edge_implicit_mesh_set_perimeter_to_segments(tei_mesh);
    adt_tri_edge_implicit_mesh_Delaunay_refinement_Rupperts_algorithm_segments(&tei_mesh, 0.9, true);

    float max_rer = adt_tri_edge_implicit_mesh_calc_max_radius_edge_ratio(tei_mesh); 
    printf("max rer = %5f | min min theta = %5f\n"  , max_rer, adt_radius_edge_ratio_to_theta(max_rer) * 180 / PI);
    float min_rer = adt_tri_edge_implicit_mesh_calc_min_radius_edge_ratio(tei_mesh);
    printf("min rer = %5f | max min theta = %5f\n\n", min_rer, adt_radius_edge_ratio_to_theta(min_rer) * 180 / PI);

    dprintINT(adt_tri_edge_implicit_mesh_any_segment_is_encroach(tei_mesh));
    dprintINT(adt_tri_edge_implicit_mesh_check_Delaunay(tei_mesh));

    mesh = as_tri_edge_implicit_mesh_to_tri_mesh(tei_mesh, 1, 0xffffffff);

    as_tri_edge_implicit_mesh_free(tei_mesh);
}

void update(game_state_t *game_state)
{
    ae_projection_mat_set(game_state->scene.proj_mat, game_state->scene.camera.aspect_ratio, game_state->scene.camera.fov_deg, game_state->scene.camera.z_near, game_state->scene.camera.z_far);
    ae_view_mat_set(game_state->scene.view_mat, game_state->scene.camera, game_state->scene.up_direction);

    ae_tri_mesh_project_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, &(proj_mesh), mesh, game_state->window_w, game_state->window_h, &(game_state->scene), AE_LIGHTING_FLAT);
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
}

void destroy(game_state_t *game_state)
{
    free(mesh.elements);
    free(proj_mesh.elements);

    (void)game_state;
}
