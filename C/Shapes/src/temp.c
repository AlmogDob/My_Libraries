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

Tri_mesh mesh, proj_mesh;

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;
    game_state->const_fps = 30;

#if 0
    Tri_implicit_mesh implicit_mesh = as_Tri_implicit_mesh_init();
    Point temp_p = (Point){0,0,0,0};
    ada_appand(Point, implicit_mesh.points, temp_p);
    temp_p = (Point){2,0,0,0};
    ada_appand(Point, implicit_mesh.points, temp_p);
    temp_p = (Point){1,-2,0,0};
    ada_appand(Point, implicit_mesh.points, temp_p);

    Tri_implicit temp_tri_imp = {.points_index = {0,1,2}};
    ada_appand(Tri_implicit, implicit_mesh.triangles, temp_tri_imp);

    mesh = as_tri_implicit_mesh_to_tri_mesh(implicit_mesh, 1, 0xffffffff);
    ada_init_array(Tri, proj_mesh);

    AS_TRI_IMPLICIT_MESH_PRINT(implicit_mesh);
    AS_TRI_MESH_PRINT(mesh);
#else
    Tri_mesh temp_mesh = {0};
    ada_init_array(Tri, temp_mesh);
    ada_init_array(Tri, mesh);
    ada_init_array(Tri, proj_mesh);

    Tri tri = {0};
    tri.points[0] = (Point){0,0,0,0};
    tri.points[1] = (Point){2,0,0,0};
    tri.points[2] = (Point){1,-2,0,0};
    as_tri_set_normals(&tri);

    ada_appand(Tri, temp_mesh, tri);

    Tri_implicit_mesh implicit_mesh = as_tri_mesh_to_tri_implicit_mesh(temp_mesh);

    mesh = as_tri_implicit_mesh_to_tri_mesh(implicit_mesh, 1, 0xffffffff);

    AS_TRI_MESH_PRINT(temp_mesh);
    AS_TRI_IMPLICIT_MESH_PRINT(implicit_mesh);
    AS_TRI_MESH_PRINT(mesh);

#endif

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
}
