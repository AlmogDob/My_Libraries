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

Tri_mesh mesh, proj_mesh;
Curve circumcircle, proj_circumcircle;
Curve in_circle, proj_in_circle;
Curve min_containment_circle, proj_min_containment_circle;

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;
    game_state->const_fps = 30;

    Tri_implicit_mesh implicit_mesh = as_tri_implicit_mesh_init();
    Point temp_p = (Point){-2,0,0,0};
    ada_appand(Point, implicit_mesh.points, temp_p);
    temp_p = (Point){2,1,0,0};
    ada_appand(Point, implicit_mesh.points, temp_p);
    temp_p = (Point){1,-1,0,0};
    ada_appand(Point, implicit_mesh.points, temp_p);

    Tri_implicit temp_tri_imp = {.points_index = {0,1,2}};
    ada_appand(Tri_implicit, implicit_mesh.triangles, temp_tri_imp);

    mesh = as_tri_implicit_mesh_to_tri_mesh(implicit_mesh, 1, 0xffffffff);
    ada_init_array(Tri, proj_mesh);


    Point circum_center = {0};
    float circum_r = 0;
    as_tri_get_circumcircle(mesh.elements[0].points[0], mesh.elements[0].points[1], mesh.elements[0].points[2], "xy", &circum_center, &circum_r);
    circumcircle = as_circle_curve_create(circum_center, circum_r, 100, 0xffffffff, "XY");
    proj_circumcircle = as_circle_curve_create(circum_center, circum_r, 100, 0xffffffff, "XY");

    Point in_center = {0};
    float in_r = 0;
    as_tri_get_incircle(mesh.elements[0].points[0], mesh.elements[0].points[1], mesh.elements[0].points[2], "xy", &in_center, &in_r);
    in_circle = as_circle_curve_create(in_center, in_r, 100, 0xff000000, "XY");
    proj_in_circle = as_circle_curve_create(in_center, in_r, 100, 0xff000000, "XY");

    Point min_containment_center = {0};
    float min_containment_r = 0;
    as_tri_get_min_containment_circle(mesh.elements[0].points[0], mesh.elements[0].points[1], mesh.elements[0].points[2], "xy", &min_containment_center, &min_containment_r);
    min_containment_circle = as_circle_curve_create(min_containment_center, min_containment_r, 100, 0xffff0000, "XY");
    proj_min_containment_circle = as_circle_curve_create(min_containment_center, min_containment_r, 100, 0xffff0000, "XY");


    as_tri_implicit_mesh_free(implicit_mesh);
}

void update(game_state_t *game_state)
{
    ae_projection_mat_set(game_state->scene.proj_mat, game_state->scene.camera.aspect_ratio, game_state->scene.camera.fov_deg, game_state->scene.camera.z_near, game_state->scene.camera.z_far);
    ae_view_mat_set(game_state->scene.view_mat, game_state->scene.camera, game_state->scene.up_direction);

    ae_tri_mesh_project_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, &(proj_mesh), mesh, game_state->window_w, game_state->window_h, &(game_state->scene), AE_LIGHTING_FLAT);
    ae_curve_project_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, &proj_circumcircle, circumcircle, game_state->window_w, game_state->window_h, &(game_state->scene));
    ae_curve_project_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, &proj_in_circle, in_circle, game_state->window_w, game_state->window_h, &(game_state->scene));
    ae_curve_project_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, &proj_min_containment_circle, min_containment_circle, game_state->window_w, game_state->window_h, &(game_state->scene));
}

void render(game_state_t *game_state)
{
    adl_tri_mesh_fill_Pinedas_rasterizer(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, proj_mesh, 0xffffffff, ADL_DEFAULT_OFFSET_ZOOM);

    adl_lines_loop_draw(game_state->window_pixels_mat, proj_circumcircle.elements, proj_circumcircle.length, proj_circumcircle.color, ADL_DEFAULT_OFFSET_ZOOM);
    adl_lines_loop_draw(game_state->window_pixels_mat, proj_in_circle.elements, proj_in_circle.length, proj_in_circle.color, ADL_DEFAULT_OFFSET_ZOOM);
    adl_lines_loop_draw(game_state->window_pixels_mat, proj_min_containment_circle.elements, proj_min_containment_circle.length, proj_min_containment_circle.color, ADL_DEFAULT_OFFSET_ZOOM);
}

void destroy(game_state_t *game_state)
{
    free(mesh.elements);
    free(proj_mesh.elements);

    free(circumcircle.elements);
    free(proj_circumcircle.elements);

    free(in_circle.elements);
    free(proj_in_circle.elements);

    free(min_containment_circle.elements);
    free(proj_min_containment_circle.elements);

    (void)game_state;
}
