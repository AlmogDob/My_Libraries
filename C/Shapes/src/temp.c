#define ALMOG_SHAPES_IMPLEMENTATION
#include "./include/Almog_Shapes.h"
#define MATRIX2D_IMPLEMENTATION
#include "./include/Matrix2D.h"
#if 1
int main()
{
    Curve c = {0};
    ada_init_array(Point, c);

    Point temp_p = (Point){-2,0,0,0};
    ada_appand(Point, c, temp_p);
    temp_p = (Point){-1,0,0,0};
    ada_appand(Point, c, temp_p);
    temp_p = (Point){0,0,0,0};
    ada_appand(Point, c, temp_p);
    temp_p = (Point){2,1,0,0};
    ada_appand(Point, c, temp_p);
    temp_p = (Point){2,-1,0,0};
    ada_appand(Point, c, temp_p);
    temp_p = (Point){1,1,0,0};
    ada_appand(Point, c, temp_p);
    temp_p = (Point){1,-1,0,0};
    ada_appand(Point, c, temp_p);

    AS_CURVE_PRINT(c);
    printf("------------------------------\n");

    Tri_implicit_mesh implicit_mesh = as_points_array_get_lexicographic_triangulation(c.elements, c.length);

    AS_TRI_IMPLICIT_MESH_PRINT(implicit_mesh);

}
#else
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

Tri_mesh temp_mesh, mesh, proj_mesh;
Tri_implicit_mesh temp_implicit_mesh;

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;
    game_state->const_fps = 30;

    ada_init_array(Tri, proj_mesh);

    temp_mesh = as_circle_tri_mesh_create_simple((Point){0,0,0,0}, 1, 10, 0xffffffff, 1, "xy");
    temp_implicit_mesh = as_tri_mesh_to_tri_implicit_mesh(temp_mesh);



    mesh = as_tri_implicit_mesh_to_tri_mesh(temp_implicit_mesh, 1, 0xffffffff);

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
#endif