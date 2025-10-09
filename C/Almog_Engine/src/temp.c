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

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;

    ada_init_array(Tri_mesh, game_state->scene.original_tri_meshes);
    ada_init_array(Tri_mesh, game_state->scene.in_world_tri_meshes);
    ada_init_array(Tri_mesh, game_state->scene.projected_tri_meshes);

    char file_path[MAX_LEN_LINE];
    strncpy(file_path, "./teapot.stl", MAX_LEN_LINE);

    Tri_mesh tri_mesh = ae_tri_mesh_get_from_file(file_path);
    // ae_tri_mesh_flip_normals(tri_mesh);

    ada_appand(Tri_mesh, game_state->scene.original_tri_meshes, tri_mesh);

    printf("[INFO] number of meshes: %zu\n", game_state->scene.original_tri_meshes.length);
    size_t sum = 0;
    for (size_t i = 0; i < game_state->scene.original_tri_meshes.length; i++) {
        printf("[INFO] mesh number %zu: %zu\n", i, game_state->scene.original_tri_meshes.elements[i].length);
        sum += game_state->scene.original_tri_meshes.elements[i].length;
    }
    printf("[INFO] total number of triangles: %zu\n", sum);


    for (size_t i = 0; i < game_state->scene.original_tri_meshes.length; i++) {
        ae_tri_mesh_normalize(game_state->scene.original_tri_meshes.elements[i]);
    }
    for (size_t i = 0; i < game_state->scene.original_tri_meshes.length; i++) {
        ae_tri_mesh_appand_copy(&(game_state->scene.in_world_tri_meshes), game_state->scene.original_tri_meshes.elements[i]);
        ae_tri_mesh_appand_copy(&(game_state->scene.projected_tri_meshes), game_state->scene.original_tri_meshes.elements[i]);
        game_state->scene.projected_tri_meshes.elements[i].length = 0;
    }

    ae_tri_mesh_rotate_Euler_xyz(game_state->scene.in_world_tri_meshes.elements[0], -90, 0, 180);

}

void update(game_state_t *game_state)
{
    // MAT2D_PRINT(game_state->scene.camera.current_position);
    // MAT2D_PRINT(game_state->scene.light_direction);

    ae_projection_mat_set(game_state->scene.proj_mat, game_state->scene.camera.aspect_ratio, game_state->scene.camera.fov_deg, game_state->scene.camera.z_near, game_state->scene.camera.z_far);
    ae_view_mat_set(game_state->scene.view_mat, game_state->scene.camera, game_state->scene.up_direction);

    for (size_t i = 0; i < game_state->scene.in_world_tri_meshes.length; i++) {
        ae_tri_mesh_project_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, &(game_state->scene.projected_tri_meshes.elements[i]), game_state->scene.in_world_tri_meshes.elements[i], game_state->window_w, game_state->window_h, &(game_state->scene), AE_LIGHTING_FLAT);
    }

}

void render(game_state_t *game_state)
{
    for (size_t i = 0; i < game_state->scene.projected_tri_meshes.length; i++) {
        adl_tri_mesh_fill_Pinedas_rasterizer_interpolate_normal(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, game_state->scene.projected_tri_meshes.elements[i], 0xffffffff, ADL_DEFAULT_OFFSET_ZOOM);
    }
}