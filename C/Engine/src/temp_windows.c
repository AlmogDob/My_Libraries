#include <stdio.h>
#include <stdbool.h>

#define APL_SETUP
#define APL_INPUT
#define APL_UPDATE
#define APL_RENDER
#define MATRIX2D_IMPLEMENTATION
#define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#include "./include/Almog_Platform_Library.h"

#define ADL_ASSERT APL_ASSERT
#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "./include/Almog_Draw_Library.h"

#define AE_ASSERT APL_ASSERT
#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#define ALMOG_ENGINE_IMPLEMENTATION
#include "./include/Almog_Engine.h"

enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
    ws->to_limit_fps = 0;

    static Scene main_scene;
    main_scene = ae_scene_init(ws->window_h, ws->window_w);
    ws->user_data = (void *)&main_scene;

    Scene * scene = ws->user_data;
    ada_init_array(Tri_mesh, scene->original_tri_meshes);
    ada_init_array(Tri_mesh, scene->in_world_tri_meshes);
    ada_init_array(Tri_mesh, scene->projected_tri_meshes);

    char file_path[ASM_MAX_LEN_LINE];
    strncpy(file_path, "../src/assets/stl/teapot.stl", ASM_MAX_LEN_LINE);

    Tri_mesh tri_mesh = ae_tri_mesh_get_from_file(file_path);

    // ae_tri_mesh_flip_normals(tri_mesh);

    ada_appand(Tri_mesh, scene->original_tri_meshes, tri_mesh);

    printf("[INFO] number of meshes: %zu\n", scene->original_tri_meshes.length);
    size_t sum = 0;
    for (size_t i = 0; i < scene->original_tri_meshes.length; i++) {
        printf("[INFO] mesh number %zu: %zu\n", i, scene->original_tri_meshes.elements[i].length);
        sum += scene->original_tri_meshes.elements[i].length;
    }
    printf("[INFO] total number of triangles: %zu\n", sum);


    for (size_t i = 0; i < scene->original_tri_meshes.length; i++) {
        ae_tri_mesh_normalize(scene->original_tri_meshes.elements[i]);
    }
    for (size_t i = 0; i < scene->original_tri_meshes.length; i++) {
        ae_tri_mesh_appand_copy(&(scene->in_world_tri_meshes), scene->original_tri_meshes.elements[i]);
        ae_tri_mesh_appand_copy(&(scene->projected_tri_meshes), scene->original_tri_meshes.elements[i]);
        scene->projected_tri_meshes.elements[i].length = 0;
    }

    ae_tri_mesh_rotate_Euler_xyz(scene->in_world_tri_meshes.elements[0], -90, 0, 180);

    // ae_tri_mesh_translate(scene->in_world_tri_meshes.elements[0], 0, 0, 6);

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_update(struct Apl_Window_State *ws)
{
    Scene * scene = ws->user_data;
    // MAT2D_PRINT(scene.camera.current_position);
    // MAT2D_PRINT(scene.light_direction);

    ae_projection_mat_set(scene->proj_mat, scene->camera.aspect_ratio, scene->camera.fov_deg, scene->camera.z_near, scene->camera.z_far);
    ae_view_mat_set(scene->view_mat, scene->camera, scene->up_direction);

    for (size_t i = 0; i < scene->in_world_tri_meshes.length; i++) {
        ae_tri_mesh_project_world2screen(scene->proj_mat, scene->view_mat, &(scene->projected_tri_meshes.elements[i]), scene->in_world_tri_meshes.elements[i], ws->window_w, ws->window_h, scene, AE_LIGHTING_SMOOTH);
    }

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_render(struct Apl_Window_State *ws)
{
    if (!ws->user_data) {
        return APL_OK;
    }
    Scene * scene = ws->user_data;

    // apl_dprintSIZE_T(scene->projected_tri_meshes.length);

    for (size_t i = 0; i < scene->projected_tri_meshes.length; i++) {
        adl_tri_mesh_fill_Pinedas_rasterizer_interpolate_normal(ws->window_pixels_mat, ws->inv_z_buffer_mat, scene->projected_tri_meshes.elements[i], 0xffffffff, ADL_DEFAULT_OFFSET_ZOOM);
        // adl_tri_mesh_draw(ws->window_pixels_mat, scene->projected_tri_meshes.elements[i], 0xffffffff, ADL_DEFAULT_OFFSET_ZOOM);
    }

    // adl_circle_fill(ws->window_pixels_mat, 100, 100, 100, 0xffffffff, ADL_DEFAULT_OFFSET_ZOOM);

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_input(struct Apl_Window_State *ws)
{
    Scene * scene = ws->user_data;

    if (ws->buttons.w_is_pressed) {
        MAT2D_AT(scene->camera.offset_position, 1, 0) -= 1 * ws->delta_time_sec;
    }
    if (ws->buttons.s_is_pressed) {
        MAT2D_AT(scene->camera.offset_position, 1, 0) += 1 * ws->delta_time_sec;
    }
    if (ws->buttons.a_is_pressed) {
        MAT2D_AT(scene->camera.offset_position, 0, 0) -= 1 * ws->delta_time_sec;
    }
    if (ws->buttons.d_is_pressed) {
        MAT2D_AT(scene->camera.offset_position, 0, 0) += 1 * ws->delta_time_sec;
    }
    if (ws->buttons.q_is_pressed) {
        MAT2D_AT(scene->camera.offset_position, 2, 0) -= 1 * ws->delta_time_sec;
    }
    if (ws->buttons.e_is_pressed) {
        MAT2D_AT(scene->camera.offset_position, 2, 0) += 1 * ws->delta_time_sec;
    }
    if (ws->buttons.left_is_pressed) {
        scene->camera.pitch_offset_deg -= 15 * ws->delta_time_sec;
    }
    if (ws->buttons.right_is_pressed) {
        scene->camera.pitch_offset_deg += 15 * ws->delta_time_sec;
    }
    if (ws->buttons.up_is_pressed) {
        scene->camera.roll_offset_deg += 15 * ws->delta_time_sec;
        if (scene->camera.roll_offset_deg > 89) {
            scene->camera.roll_offset_deg = 89;
        }
    }
    if (ws->buttons.down_is_pressed) {
        scene->camera.roll_offset_deg -= 15 * ws->delta_time_sec;
        if (scene->camera.roll_offset_deg < -89) {
            scene->camera.roll_offset_deg = -89;
        }
    }
    if (ws->buttons.r_is_pressed) {
        ae_camera_reset_pos(scene);
    }

    scene->camera.aspect_ratio = (float)(ws->window_h) / (float)(ws->window_w);

    return APL_SUCCESS;
}
