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

    Curve convex_hull = {0};
    ada_init_array(Point, convex_hull);

    Point p = {-4,2,0,0};
    ada_appand(Point, convex_hull, p);
    p = (Point){-4,-2,0,0};
    ada_appand(Point, convex_hull, p);
    p = (Point){4,-2,0,0};
    ada_appand(Point, convex_hull, p);
    p = (Point){4,2,0,0};
    ada_appand(Point, convex_hull, p);
    float del = 4.0 / 100;
    for (float i = -2 + del; i < 2 - del; i += del) {
        p = (Point){4,i,0,0};
        ada_appand(Point, convex_hull, p);
    }

    Tri_edge_implicit_mesh tei_mesh = adt_tri_edge_implicit_mesh_make_Delaunay_triangulation_flip_algorithm(convex_hull.elements, convex_hull.length);
    free(convex_hull.elements);

    adt_tri_edge_implicit_mesh_set_perimeter_to_segments(tei_mesh);

    adt_tri_edge_implicit_mesh_Delaunay_refinement_Rupperts_algorithm_segments(&tei_mesh, 0.91, false);

    float max_rer = adt_tri_edge_implicit_mesh_calc_max_radius_edge_ratio(tei_mesh); 
    printf("max rer = %5f | min min theta = %5f\n", max_rer, adt_radius_edge_ratio_to_theta(max_rer) * 180 / PI);
    float min_rer = adt_tri_edge_implicit_mesh_calc_min_radius_edge_ratio(tei_mesh);
    printf("min rer = %5f | max min theta = %5f\n", min_rer, adt_radius_edge_ratio_to_theta(min_rer) * 180 / PI);
    printf("\n");

    dprintINT(adt_tri_edge_implicit_mesh_any_segment_is_encroach(tei_mesh));
    dprintINT(adt_tri_edge_implicit_mesh_check_Delaunay(tei_mesh));






    mesh = as_tri_edge_implicit_mesh_to_tri_mesh(tei_mesh, 1, 0xffffffff);



    ada_init_array(Curve, circles);
    ada_init_array(Curve, proj_circles);

    // size_t temp_indexes[] = {p1_index, p2_index, 19, 20};
    // for (int i = 0; i < sizeof(temp_indexes) / sizeof(temp_indexes[0]); i++) {
    //     Curve temp_curve = as_circle_curve_create(tei_mesh.points.elements[temp_indexes[i]], 0.04, 100, 0xffff0000, "xy");
    //     Curve temp_proj_curve = as_circle_curve_create(tei_mesh.points.elements[temp_indexes[i]], 0.02, 100, 0xffff0000, "xy");
    //     ada_appand(Curve, circles, temp_curve);
    //     ada_appand(Curve, proj_circles, temp_proj_curve);
    // }

    #if 0
    for (size_t i = 0; i < mesh.length; i++) {
        float t = (float)i / ((float)mesh.length - 1);
        float t1 = t < 0.5 ? t * 2 : 1;
        float t2 = t >= 0.5 ? (t-0.5) * 2 : 1;
        Tri tri = mesh.elements[i];
        Point center = {0};
        float r = 0;
        adt_tri_get_circumcircle(tri.points[0], tri.points[1], tri.points[2], "xy", &center, &r);
        Curve temp_curve = as_circle_curve_create(center, r, 500, RGBA_hexARGB(255.0f * t, 255 * (1-t1), 255 * (t2), 255), "xy");
        Curve temp_proj_curve = as_circle_curve_create(center, r, 500, RGBA_hexARGB(255.0f * t, 255 * (1-t1), 255 * (t2), 255), "xy");
        ada_appand(Curve, circles, temp_curve);
        ada_appand(Curve, proj_circles, temp_proj_curve);
    }
    #endif

    as_tri_edge_implicit_mesh_free(tei_mesh);
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

