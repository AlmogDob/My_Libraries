/* This code is heavily inspired by the 3D engine of 'OneLoneCoder' in C++.
you can find the source code in:
https://github.com/OneLoneCoder/Javidx9/tree/master/ConsoleGameEngine/BiggerProjects/Engine3D .
featured in this video of his:
https://youtu.be/ih20l3pJoeU?si=CzQ8rjk5ZEOlqEHN. */ 


#ifndef ALMOG_ENGINE_H_
#define ALMOG_ENGINE_H_

#include "./Almog_Dynamic_Array.h"
#include "./Matrix2D.h"
#include "./Almog_Draw_Library.h"

#ifndef ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#endif
#include "./Almog_String_Manipulation.h"

#ifndef AE_ASSERT
#include <assert.h>
#define AE_ASSERT assert
#endif

#include <math.h>
#include <stdbool.h>
#include <float.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#ifndef PI
#define PI M_PI
#endif

#ifndef STL_HEADER_SIZE
#define STL_HEADER_SIZE 80
#endif

#ifndef STL_NUM_SIZE
#define STL_NUM_SIZE 4
#endif

#ifndef STL_SIZE_FOREACH_TRI
#define STL_SIZE_FOREACH_TRI 50
#endif

#ifndef STL_ATTRIBUTE_BITS_SIZE
#define STL_ATTRIBUTE_BITS_SIZE 2
#endif

#ifndef HexARGB_RGBA
#define HexARGB_RGBA(x) ((x)>>(8*2)&0xFF), ((x)>>(8*1)&0xFF), ((x)>>(8*0)&0xFF), ((x)>>(8*3)&0xFF)
#endif
#ifndef HexARGB_RGB_VAR
#define HexARGB_RGB_VAR(x, r, g, b) r = ((x)>>(8*2)&0xFF); g = ((x)>>(8*1)&0xFF); b = ((x)>>(8*0)&0xFF);
#endif
#ifndef HexARGB_RGBA_VAR
#define HexARGB_RGBA_VAR(x, r, g, b, a) r = ((x)>>(8*2)&0xFF); g = ((x)>>(8*1)&0xFF); b = ((x)>>(8*0)&0xFF); a = ((x)>>(8*3)&0xFF)
#endif
#define ARGB_hexARGB(a, r, g, b) 0x01000000l*(uint8_t)(a) + 0x00010000*(uint8_t)(r) + 0x00000100*(uint8_t)(g) + 0x00000001*(uint8_t)(b)
#ifndef RGB_hexRGB
#define RGB_hexRGB(r, g, b) (int)(0x010000*(int)(r) + 0x000100*(int)(g) + 0x000001*(int)(b))
#endif

#define AE_MAX_POINT_VAL 1e5
#define ae_assert_point_is_valid(p) AE_ASSERT(isfinite((p).x) && isfinite((p).y) && isfinite((p).z) && isfinite((p).w));    \
        AE_ASSERT((p).x > -AE_MAX_POINT_VAL && (p).x < AE_MAX_POINT_VAL);                                                                  \
        AE_ASSERT((p).y > -AE_MAX_POINT_VAL && (p).y < AE_MAX_POINT_VAL);                                                                  \
        AE_ASSERT((p).z > -AE_MAX_POINT_VAL && (p).z < AE_MAX_POINT_VAL);                                                                  \
        AE_ASSERT((p).w > -AE_MAX_POINT_VAL && (p).w < AE_MAX_POINT_VAL);
#define ae_assert_tri_is_valid(tri) ae_assert_point_is_valid((tri).points[0]);  \
        ae_assert_point_is_valid((tri).points[1]);                              \
        ae_assert_point_is_valid((tri).points[2])
#define ae_assert_quad_is_valid(quad) ae_assert_point_is_valid((quad).points[0]);   \
        ae_assert_point_is_valid((quad).points[1]);                                 \
        ae_assert_point_is_valid((quad).points[2]);                                 \
        ae_assert_point_is_valid((quad).points[3])
#define ae_point_normalize_xyz_norma(p, norma) (p).x = (p).x / norma;  \
        (p).y = (p).y / norma;                        \
        (p).z = (p).z / norma 
#define ae_point_calc_norma(p) sqrt(((p).x * (p).x) + ((p).y * (p).y) + ((p).z * (p).z))
#define ae_point_add_point(p, p1, p2) (p).x = (p1).x + (p2).x;  \
        (p).y = (p1).y + (p2).y;                                \
        (p).z = (p1).z + (p2).z;                                \
        (p).w = (p1).w + (p2).w
#define ae_point_sub_point(p, p1, p2) (p).x = (p1).x - (p2).x;  \
        (p).y = (p1).y - (p2).y;                                \
        (p).z = (p1).z - (p2).z;                                \
        (p).w = (p1).w - (p2).w
#define ae_point_dot_point(p1, p2) (((p1).x * (p2).x) + ((p1).y * (p2).y) + ((p1).z * (p2).z))
#define ae_point_mult(p, const) (p).x *= const; \
        (p).y *= const;                         \
        (p).z *= const
#define ae_points_equal(p1, p2) (p1).x == (p2).x && (p1).y == (p2).y && (p1).z == (p2).z


typedef enum {
    AE_LIGHTING_FLAT,
    AE_LIGHTING_SMOOTH,
    AE_LIGHTING_MODE_LENGTH
} Lighting_mode;

#ifndef TRI_MESH_ARRAY
#define TRI_MESH_ARRAY
typedef struct {
    size_t length;
    size_t capacity;
    Tri_mesh *elements;
} Tri_mesh_array; /* Tri_mesh ada array */
#endif

#ifndef QUAD_MESH_ARRAY
#define QUAD_MESH_ARRAY
typedef struct {
    size_t length;
    size_t capacity;
    Quad_mesh *elements;
} Quad_mesh_array; /* Quad_mesh ada array */
#endif

typedef struct {
    Mat2D init_position;
    Mat2D current_position;
    Mat2D offset_position;
    Mat2D direction;
    float z_near;
    float z_far;
    float fov_deg;
    float aspect_ratio;
    float roll_offset_deg;
    float pitch_offset_deg;
    float yaw_offset_deg;
    Mat2D camera_x;
    Mat2D camera_y;
    Mat2D camera_z;
} Camera;

typedef struct {
    Point light_direction_or_pos;
    float light_intensity;
} Light_source;

typedef struct {
    float specular_power_alpha;
    float c_ambi;
    float c_diff;
    float c_spec;
} Material;

typedef struct {
    Tri_mesh_array in_world_tri_meshes;
    Tri_mesh_array projected_tri_meshes;
    Tri_mesh_array original_tri_meshes;

    Quad_mesh_array in_world_quad_meshes;
    Quad_mesh_array projected_quad_meshes;
    Quad_mesh_array original_quad_meshes;

    Camera camera;
    Mat2D up_direction;
    Mat2D proj_mat;
    Mat2D view_mat;

    Light_source light_source0;
    Material material0;
} Scene;

Tri         ae_tri_create(Point p1, Point p2, Point p3);
void        ae_tri_mesh_create_copy(Tri_mesh *des, Tri *src_elements, size_t len);
Tri_mesh    ae_cube_create_tri_mesh(const size_t len, uint32_t color);

void        ae_camera_init(Scene *scene, int window_h, int window_w);
Scene       ae_scene_init(int window_h, int window_w);
void        ae_camera_reset_pos(Scene *scene);

void        ae_point_to_mat2D(Point p, Mat2D m);
Point       ae_mat2D_to_point(Mat2D m);

Tri_mesh    ae_tri_mesh_get_from_obj_file(char *file_path);
Tri_mesh    ae_tri_mesh_get_from_stl_file(char *file_path);
Tri_mesh    ae_tri_mesh_get_from_file(char *file_path);
void        ae_tri_mesh_appand_copy(Tri_mesh_array *mesh_array, Tri_mesh mesh);
Tri_mesh    ae_tri_mesh_get_from_quad_mesh(Quad_mesh q_mesh);

void        ae_print_points(Curve p);
void        ae_print_tri(Tri tri, char *name, size_t padding);
void        ae_print_tri_mesh(Tri_mesh mesh, char *name, size_t padding);

Point       ae_point_normalize_xyz(Point p);
void        ae_tri_set_normals(Tri *tri);
Point       ae_tri_get_average_normal(Tri tri);
Point       ae_tri_get_average_point(Tri tri);
void        ae_tri_calc_normal(Mat2D normal, Tri tri);
void        ae_tri_mesh_translate(Tri_mesh mesh, float x, float y, float z);
void        ae_tri_mesh_rotate_Euler_xyz(Tri_mesh mesh, float phi_deg, float theta_deg, float psi_deg);
void        ae_tri_mesh_set_bounding_box(Tri_mesh mesh, float *x_min, float *x_max, float *y_min, float *y_max, float *z_min, float *z_max);
void        ae_tri_set_center_zmin_zmax(Tri *tri);
void        ae_tri_mesh_normalize(Tri_mesh mesh);
void        ae_tri_mesh_flip_normals(Tri_mesh mesh);
void        ae_quad_set_normals(Quad *quad);
Point       ae_quad_get_average_normal(Quad quad);
Point       ae_quad_get_average_point(Quad quad);
void        ae_quad_calc_normal(Mat2D normal, Quad quad);
void        ae_curve_copy(Curve *des, Curve src);

void        ae_tri_calc_light_intensity(Tri *tri, Scene *scene, Lighting_mode lighting_mode);
void        ae_quad_calc_light_intensity(Quad *quad, Scene *scene, Lighting_mode lighting_mode);

Point       ae_line_itersect_plane(Mat2D plane_p, Mat2D plane_n, Mat2D line_start, Mat2D line_end, float *t);
int         ae_line_clip_with_plane(Point start_in, Point end_in, Mat2D plane_p, Mat2D plane_n, Point *start_out, Point *end_out);
float       ae_signed_dist_point_and_plane(Point p, Mat2D plane_p, Mat2D plane_n);
int         ae_tri_clip_with_plane(Tri tri_in, Mat2D plane_p, Mat2D plane_n, Tri *tri_out1, Tri *tri_out2);
int         ae_quad_clip_with_plane(Quad quad_in, Mat2D plane_p, Mat2D plane_n, Quad *quad_out1, Quad *quad_out2);

void        ae_projection_mat_set(Mat2D proj_mat,float aspect_ratio, float FOV_deg, float z_near, float z_far);
void        ae_view_mat_set(Mat2D view_mat, Camera camera, Mat2D up);
Point       ae_point_project_world2screen(Mat2D view_mat, Mat2D proj_mat, Point src, int window_w, int window_h);
Point       ae_point_project_world2view(Mat2D view_mat, Point src);
Point       ae_point_project_view2screen(Mat2D proj_mat, Point src, int window_w, int window_h);
void        ae_line_project_world2screen(Mat2D view_mat, Mat2D proj_mat, Point start_src, Point end_src, int window_w, int window_h, Point *start_des, Point *end_des, Scene *scene);
Tri         ae_tri_transform_to_view(Mat2D view_mat, Tri tri);
Tri_mesh    ae_tri_project_world2screen(Mat2D proj_mat, Mat2D view_mat, Tri tri, int window_w, int window_h, Scene *scene, Lighting_mode lighting_mode);
void        ae_tri_mesh_project_world2screen(Mat2D proj_mat, Mat2D view_mat, Tri_mesh *des, Tri_mesh src, int window_w, int window_h, Scene *scene, Lighting_mode lighting_mode);
Quad        ae_quad_transform_to_view(Mat2D view_mat, Quad quad);
Quad_mesh   ae_quad_project_world2screen(Mat2D proj_mat, Mat2D view_mat, Quad quad, int window_w, int window_h, Scene *scene, Lighting_mode lighting_mode);
void        ae_quad_mesh_project_world2screen(Mat2D proj_mat, Mat2D view_mat, Quad_mesh *des, Quad_mesh src, int window_w, int window_h, Scene *scene, Lighting_mode lighting_mode);
void        ae_curve_project_world2screen(Mat2D proj_mat, Mat2D view_mat, Curve *des, Curve src, int window_w, int window_h, Scene *scene);
void        ae_curve_ada_project_world2screen(Mat2D proj_mat, Mat2D view_mat, Curve_ada *des, Curve_ada src, int window_w, int window_h, Scene *scene);
void        ae_grid_project_world2screen(Mat2D proj_mat, Mat2D view_mat, Grid des, Grid src, int window_w, int window_h, Scene *scene);

void        ae_tri_swap(Tri *v, int i, int j);
bool        ae_tri_compare(Tri t1, Tri t2);
void        ae_tri_qsort(Tri *v, int left, int right);
double      ae_linear_map(double s, double min_in, double max_in, double min_out, double max_out);
void        ae_z_buffer_copy_to_screen(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer);

#endif /* ALMOG_ENGINE_H_ */

#ifdef ALMOG_ENGINE_IMPLEMENTATION
#undef ALMOG_ENGINE_IMPLEMENTATION

#define AE_PRINT_TRI(tri) ae_print_tri(tri, #tri, 0)
#define AE_PRINT_MESH(mesh) ae_print_tri_mesh(mesh, #mesh, 0)

Tri ae_tri_create(Point p1, Point p2, Point p3)
{
    Tri tri;

    tri.points[0] = p1;
    tri.points[1] = p2;
    tri.points[2] = p3;

    return tri;
}

void ae_tri_mesh_create_copy(Tri_mesh *des, Tri *src_elements, size_t len)
{
    Tri_mesh temp_des = *des;
    for (size_t i = 0; i < len; i++) {
        ada_appand(Tri, temp_des, src_elements[i]);
    }
    *des = temp_des;
}

Tri_mesh ae_cube_create_tri_mesh(const size_t len, uint32_t color)
{
    Tri_mesh cube;

    ada_init_array(Tri, cube);

    Tri tri1 = { /* south */
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = 0,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = 0,
        .to_draw = true,
        .colors = {color, color, color},
    };
    ae_tri_set_normals(&tri1);
    ada_appand(Tri, cube, tri1);
    Tri tri2 = {
        .points[0].x = len,
        .points[0].y = len,
        .points[0].z = 0,

        .points[1].x = len,
        .points[1].y = 0,
        .points[1].z = 0,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = 0,
        .to_draw = true,
        .colors = {color, color, color},
    };
    ae_tri_set_normals(&tri2);
    ada_appand(Tri, cube, tri2);
    Tri tri3 = { /* north */
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = len,
        .points[2].z = len,
        .to_draw = true,
        .colors = {color, color, color},
    };
    ae_tri_set_normals(&tri3);
    ada_appand(Tri, cube, tri3);
    Tri tri4 = {
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = len,
        .to_draw = true,
        .colors = {color, color, color},
    };
    ae_tri_set_normals(&tri4);
    ada_appand(Tri, cube, tri4);
    Tri tri5 = { /* east */
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = 0,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = len,
        .to_draw = true,
        .colors = {color, color, color},
    };
    ae_tri_set_normals(&tri5);
    ada_appand(Tri, cube, tri5);
    Tri tri6 = {
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = 0,
        .points[2].z = len,
        .to_draw = true,
        .colors = {color, color, color},
    };
    ae_tri_set_normals(&tri6);
    ada_appand(Tri, cube, tri6);
    Tri tri7 = { /* west */
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = len,
        .points[2].z = 0,
        .to_draw = true,
        .colors = {color, color, color},
    };
    ae_tri_set_normals(&tri7);
    ada_appand(Tri, cube, tri7);
    Tri tri8 = {
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = 0,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = 0,
        .to_draw = true,
        .colors = {color, color, color},
    };
    ae_tri_set_normals(&tri8);
    ada_appand(Tri, cube, tri8);
    Tri tri9 = { /* top */
        .points[0].x = 0,
        .points[0].y = len,
        .points[0].z = 0,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = len,
        .to_draw = true,
        .colors = {color, color, color},
    };
    ae_tri_set_normals(&tri9);
    ada_appand(Tri, cube, tri9);
    Tri tri10 = {
        .points[0].x = 0,
        .points[0].y = len,
        .points[0].z = 0,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = 0,
        .to_draw = true,
        .colors = {color, color, color},
    };
    ae_tri_set_normals(&tri10);
    ada_appand(Tri, cube, tri10);
    Tri tri11 = { /* bottom */
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = 0,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = 0,
        .to_draw = true,
        .colors = {color, color, color},
    };
    ae_tri_set_normals(&tri11);
    ada_appand(Tri, cube, tri11);
    Tri tri12 = {
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = 0,
        .points[1].z = 0,

        .points[2].x = len,
        .points[2].y = 0,
        .points[2].z = 0,
        .to_draw = true,
        .colors = {color, color, color},
    };
    ae_tri_set_normals(&tri12);
    ada_appand(Tri, cube, tri12);
    
    return cube;
}

void ae_camera_init(Scene *scene, int window_h, int window_w)
{
    scene->camera.z_near       = 0.1;
    scene->camera.z_far        = 1000;
    scene->camera.fov_deg      = 60;
    scene->camera.aspect_ratio = (float)window_h / (float)window_w;

    scene->camera.init_position = mat2D_alloc(3, 1);
    mat2D_fill(scene->camera.init_position, 0);
    MAT2D_AT(scene->camera.init_position, 2, 0) = -4;

    scene->camera.current_position = mat2D_alloc(3, 1);
    mat2D_copy(scene->camera.current_position, scene->camera.init_position);

    scene->camera.offset_position = mat2D_alloc(3, 1);
    mat2D_fill(scene->camera.offset_position, 0);

    scene->camera.roll_offset_deg = 0;
    scene->camera.pitch_offset_deg = 0;
    scene->camera.yaw_offset_deg = 0;

    scene->camera.direction = mat2D_alloc(3, 1);
    mat2D_fill(scene->camera.direction, 0);
    MAT2D_AT(scene->camera.direction, 2, 0) = 1;

    scene->camera.camera_x = mat2D_alloc(3, 1);
    mat2D_fill(scene->camera.camera_x, 0);
    MAT2D_AT(scene->camera.camera_x, 0, 0) = 1;
    
    scene->camera.camera_y = mat2D_alloc(3, 1);
    mat2D_fill(scene->camera.camera_y, 0);
    MAT2D_AT(scene->camera.camera_y, 1, 0) = 1;

    scene->camera.camera_z = mat2D_alloc(3, 1);
    mat2D_fill(scene->camera.camera_z, 0);
    MAT2D_AT(scene->camera.camera_z, 2, 0) = 1;
}

Scene ae_scene_init(int window_h, int window_w)
{
    Scene scene;
    ae_camera_init(&(scene), window_h, window_w);

    scene.up_direction = mat2D_alloc(3, 1);
    mat2D_fill(scene.up_direction, 0);
    MAT2D_AT(scene.up_direction, 1, 0) = 1;

    scene.light_source0.light_direction_or_pos.x = 0.5;
    scene.light_source0.light_direction_or_pos.y = 1;
    scene.light_source0.light_direction_or_pos.z = 1;
    scene.light_source0.light_direction_or_pos.w = 0;
    scene.light_source0.light_direction_or_pos = ae_point_normalize_xyz(scene.light_source0.light_direction_or_pos);
    scene.light_source0.light_intensity = 1;

    scene.material0.specular_power_alpha = 1;
    scene.material0.c_ambi = 0.2;
    scene.material0.c_diff = 0.6;
    scene.material0.c_spec = 0.2;

    scene.proj_mat = mat2D_alloc(4, 4);
    ae_projection_mat_set(scene.proj_mat, scene.camera.aspect_ratio, scene.camera.fov_deg, scene.camera.z_near, scene.camera.z_far);

    scene.view_mat = mat2D_alloc(4, 4);
    ae_view_mat_set(scene.view_mat, scene.camera, scene.up_direction);

    return scene;
}

void ae_camera_reset_pos(Scene *scene)
{
    scene->camera.roll_offset_deg = 0;
    scene->camera.pitch_offset_deg = 0;
    scene->camera.yaw_offset_deg = 0;

    mat2D_fill(scene->camera.offset_position, 0);

    mat2D_fill(scene->camera.camera_x, 0);
    MAT2D_AT(scene->camera.camera_x, 0, 0) = 1;
    mat2D_fill(scene->camera.camera_y, 0);
    MAT2D_AT(scene->camera.camera_y, 1, 0) = 1;
    mat2D_fill(scene->camera.camera_z, 0);
    MAT2D_AT(scene->camera.camera_z, 2, 0) = 1;

    mat2D_copy(scene->camera.current_position, scene->camera.init_position);
}


void ae_point_to_mat2D(Point p, Mat2D m)
{
    MATRIX2D_ASSERT((3 == m.rows && 1 == m.cols) || (1 == m.rows && 3 == m.cols));
    
    if (3 == m.rows) {
        MAT2D_AT(m, 0, 0) = p.x;
        MAT2D_AT(m, 1, 0) = p.y;
        MAT2D_AT(m, 2, 0) = p.z;
    }
    if (3 == m.cols) {
        MAT2D_AT(m, 0, 0) = p.x;
        MAT2D_AT(m, 0, 1) = p.y;
        MAT2D_AT(m, 0, 2) = p.z;
    }
}

Point ae_mat2D_to_point(Mat2D m)
{
    Point res = {.x = MAT2D_AT(m, 0, 0), .y = MAT2D_AT(m, 1, 0), .z = MAT2D_AT(m, 2, 0), .w = 1};
    return res;
}

Tri_mesh ae_tri_mesh_get_from_obj_file(char *file_path)
{
    char current_line[MAX_LEN_LINE], current_word[MAX_LEN_LINE], current_num_str[MAX_LEN_LINE];
    char file_name[MAX_LEN_LINE], file_extention[MAX_LEN_LINE], mesh_name[MAX_LEN_LINE];
    int texture_warning_was_printed = 0;

    strncpy(file_name, file_path, MAX_LEN_LINE);
    strncpy(file_extention, file_name, MAX_LEN_LINE);

    /* check if file is an obj file*/
    asm_get_word_and_cut(file_name, file_extention, '.');
    asm_get_word_and_cut(file_name, file_extention, '.');
    if (strncmp(file_extention, ".obj", MAX_LEN_LINE)) {
        fprintf(stderr, "%s:%d: [Error] unsupported file format: '%s'\n", __FILE__, __LINE__, file_name);
        exit(1);
    }

    strncpy(mesh_name, file_name, MAX_LEN_LINE);
    while(asm_length(mesh_name)) {
        asm_get_word_and_cut(current_word, mesh_name, '/');
    }

    strncpy(mesh_name, current_word, MAX_LEN_LINE);

    strncpy(current_word, ".", MAX_LEN_LINE);
    strncat(file_name, ".obj", MAX_LEN_LINE/2);
    strncat(current_word, file_name, MAX_LEN_LINE/2);

    FILE *fp_input = fopen(current_word, "rt");
    if (fp_input == NULL) {
        fprintf(stderr, "%s:%d: [Error] failed to open input file: '%s', %s\n", __FILE__, __LINE__, current_word, strerror(errno));
        exit(1);
    }

    // strncpy(output_file_name, "./build/", MAX_LEN_LINE);
    // strncat(output_file_name, mesh_name, MAX_LEN_LINE/2);
    // strncat(output_file_name, ".c", MAX_LEN_LINE/2);
    // FILE *fp_output = fopen(output_file_name, "wt");
    // if (fp_input == NULL) {
    //     fprintf(stderr, "%s:%d: [Error] failed to open output file: '%s'. %s\n", __FILE__, __LINE__, output_file_name, strerror(errno));
    //     exit(1);
    // }

    /* parsing data from file */
    Curve points = {0};
    ada_init_array(Point, points);
    Tri_mesh mesh;
    ada_init_array(Tri, mesh);

    int line_len;

    while ((line_len = asm_get_line(fp_input, current_line)) != -1) {
        asm_get_next_word_from_line(current_word, current_line, ' ');
        if (!strncmp(current_word, "v", 1)) {
            Point p;
            asm_get_word_and_cut(current_word, current_line, ' ');
            asm_get_word_and_cut(current_word, current_line, ' ');
            p.x = atof(current_word);
            asm_get_word_and_cut(current_word, current_line, ' ');
            p.y = atof(current_word);
            asm_get_word_and_cut(current_word, current_line, ' ');
            p.z = atof(current_word);
            // printf("current word: %s\n", current_word);
            ada_appand(Point, points, p);
            // break;
        }
        if (!strncmp(current_word, "f", 1)) {
            Tri tri1 = {0}, tri2 = {0};

            // printf("line: %s\nword: %s, %d\n", current_line, current_word, atoi(current_word));
            asm_get_word_and_cut(current_word, current_line, ' ');
            // printf("line: %s\nword: %s, %d\n", current_line, current_word, atoi(current_word));

            int number_of_spaces = asm_str_in_str(current_line, " ");
            // printf("%d\n", number_of_spaces);
            // exit(1);
            if (!(number_of_spaces == 3 || number_of_spaces == 4 || number_of_spaces == 5)) {
                fprintf(stderr, "%s:%d: [Error] there is unsupported number of vertices for a face: %d\n", __FILE__, __LINE__, number_of_spaces);
                exit(1);
            }
            if (number_of_spaces == 3) {
                /* there are 3 vertices for the face. */
                asm_get_word_and_cut(current_word, current_line, ' ');
                // printf("line: %s\nword: %s, %d\n", current_line, current_word, atoi(current_word));
                int number_of_backslash = asm_str_in_str(current_word, "/");
                if (number_of_backslash == 0) {
                    tri1.points[0] = points.elements[atoi(current_word)-1];
                    asm_get_word_and_cut(current_word, current_line, ' ');
                    tri1.points[1] = points.elements[atoi(current_word)-1];
                    asm_get_word_and_cut(current_word, current_line, ' ');
                    tri1.points[2] = points.elements[atoi(current_word)-1];
                }
                if (number_of_backslash == 2) {
                    if (!texture_warning_was_printed) {
                        fprintf(stderr, "%s:%d [Warning] texture and normals data ignored of file at - '%s'\n", __FILE__, __LINE__, file_path);
                        texture_warning_was_printed = 1;
                    }

                    asm_get_word_and_cut(current_num_str, current_word, '/');
                    // printf("line: %s\nword: %s\nnum str: %s, %d\n", current_line, current_word, current_num_str, atoi(current_num_str));
                    tri1.points[0] = points.elements[atoi(current_num_str)-1];

                    asm_get_word_and_cut(current_word, current_line, ' ');
                    asm_get_word_and_cut(current_num_str, current_word, '/');
                    // printf("line: %s\nword: %s\nnum str: %s, %d\n", current_line, current_word, current_num_str, atoi(current_num_str));
                    tri1.points[1] = points.elements[atoi(current_num_str)-1];

                    asm_get_word_and_cut(current_word, current_line, ' ');
                    asm_get_word_and_cut(current_num_str, current_word, '/');
                    // printf("line: %s\nword: %s\nnum str: %s, %d\n", current_line, current_word, current_num_str, atoi(current_num_str));
                    tri1.points[2] = points.elements[atoi(current_num_str)-1];
                }

                tri1.to_draw = true;
                tri1.light_intensity[0] = 1;
                tri1.light_intensity[1] = 1;
                tri1.light_intensity[2] = 1;
                tri1.center.x = (tri1.points[0].x + tri1.points[1].x + tri1.points[2].x) / 3;
                tri1.center.y = (tri1.points[0].y + tri1.points[1].y + tri1.points[2].y) / 3;
                tri1.center.z = (tri1.points[0].z + tri1.points[1].z + tri1.points[2].z) / 3;
                tri1.z_min = fmin(tri1.points[0].z, fmin(tri1.points[1].z, tri1.points[2].z));
                tri1.z_max = fmax(tri1.points[0].z, fmax(tri1.points[1].z, tri1.points[2].z));
                tri1.colors[0] = 0xFFFFFFFF;
                tri1.colors[1] = 0xFFFFFFFF;
                tri1.colors[2] = 0xFFFFFFFF;

                ada_appand(Tri, mesh, tri1);
                // AE_PRINT_TRI(tri1);
            } else if (number_of_spaces == 5 || number_of_spaces == 4) {
                /* there are 4 vertices for the face. */
                /* sometimes there is a space in the end */
                asm_get_word_and_cut(current_word, current_line, ' ');
                // printf("line: %s\nword: %s, %d\n", current_line, current_word, atoi(current_word));
                int number_of_backslash = asm_str_in_str(current_word, "/");
                if (number_of_backslash == 0) {
                    tri1.points[0] = points.elements[atoi(current_word)-1];
                    asm_get_word_and_cut(current_word, current_line, ' ');
                    tri1.points[1] = points.elements[atoi(current_word)-1];
                    asm_get_word_and_cut(current_word, current_line, ' ');
                    tri1.points[2] = points.elements[atoi(current_word)-1];
                }
                if (number_of_backslash == 2 || number_of_backslash == 1) {
                    if (!texture_warning_was_printed) {
                        fprintf(stderr, "%s:%d [Warning] texture and normals data ignored of file at - '%s'\n", __FILE__, __LINE__, file_path);
                        texture_warning_was_printed = 1;
                    }

                    asm_get_word_and_cut(current_num_str, current_word, '/');
                    // printf("line: %s\nword: %s\nnum str: %s, %d\n", current_line, current_word, current_num_str, atoi(current_num_str));
                    tri1.points[0] = points.elements[atoi(current_num_str)-1];
                    tri2.points[2] = points.elements[atoi(current_num_str)-1];

                    asm_get_word_and_cut(current_word, current_line, ' ');
                    asm_get_word_and_cut(current_num_str, current_word, '/');
                    // printf("line: %s\nword: %s\nnum str: %s, %d\n", current_line, current_word, current_num_str, atoi(current_num_str));
                    tri1.points[1] = points.elements[atoi(current_num_str)-1];

                    asm_get_word_and_cut(current_word, current_line, ' ');
                    asm_get_word_and_cut(current_num_str, current_word, '/');
                    // printf("line: %s\nword: %s\nnum str: %s, %d\n", current_line, current_word, current_num_str, atoi(current_num_str));
                    tri1.points[2] = points.elements[atoi(current_num_str)-1];
                    tri2.points[0] = points.elements[atoi(current_num_str)-1];

                    asm_get_word_and_cut(current_word, current_line, ' ');
                    asm_get_word_and_cut(current_num_str, current_word, '/');
                    // printf("line: %s\nword: %s\nnum str: %s, %d\n", current_line, current_word, current_num_str, atoi(current_num_str));
                    tri2.points[1] = points.elements[atoi(current_num_str)-1];
                }

                tri1.to_draw = true;
                tri1.light_intensity[0] = 1;
                tri1.light_intensity[1] = 1;
                tri1.light_intensity[2] = 1;
                tri1.center.x = (tri1.points[0].x + tri1.points[1].x + tri1.points[2].x) / 3;
                tri1.center.y = (tri1.points[0].y + tri1.points[1].y + tri1.points[2].y) / 3;
                tri1.center.z = (tri1.points[0].z + tri1.points[1].z + tri1.points[2].z) / 3;
                tri1.z_min = fmin(tri1.points[0].z, fmin(tri1.points[1].z, tri1.points[2].z));
                tri1.z_max = fmax(tri1.points[0].z, fmax(tri1.points[1].z, tri1.points[2].z));
                tri1.colors[0] = 0xFFFFFFFF;
                tri1.colors[1] = 0xFFFFFFFF;
                tri1.colors[2] = 0xFFFFFFFF;

                tri2.to_draw = true;
                tri2.light_intensity[0] = 1;
                tri2.light_intensity[1] = 1;
                tri2.light_intensity[2] = 1;
                tri2.center.x = (tri2.points[0].x + tri2.points[1].x + tri2.points[2].x) / 3;
                tri2.center.y = (tri2.points[0].y + tri2.points[1].y + tri2.points[2].y) / 3;
                tri2.center.z = (tri2.points[0].z + tri2.points[1].z + tri2.points[2].z) / 3;
                tri2.z_min = fmin(tri2.points[0].z, fmin(tri2.points[1].z, tri2.points[2].z));
                tri2.z_max = fmax(tri2.points[0].z, fmax(tri2.points[1].z, tri2.points[2].z));
                tri2.colors[0] = 0xFFFFFFFF;
                tri2.colors[1] = 0xFFFFFFFF;
                tri2.colors[2] = 0xFFFFFFFF;

                ada_appand(Tri, mesh, tri1);
                ada_appand(Tri, mesh, tri2);
                // AE_PRINT_TRI(tri1);
                // AE_PRINT_TRI(tri2);
            }
            // exit(2);
        }
    }
    
    return mesh;
}

Tri_mesh ae_tri_mesh_get_from_stl_file(char *file_path)
{
    FILE *file;
    file = fopen(file_path, "rb");
    if (file == NULL) {
        fprintf(stderr, "%s:%d: [Error] failed to open input file: '%s', %s\n", __FILE__, __LINE__, file_path, strerror(errno));
        exit(1);
    }

    char header[STL_HEADER_SIZE];
    fread(header, STL_HEADER_SIZE, 1, file);
    // dprintSTRING(header);

    uint32_t num_of_tri;
    fread(&num_of_tri, STL_NUM_SIZE, 1, file);
    // dprintINT(num_of_tri);

    Tri_mesh mesh;
    ada_init_array(Tri, mesh);
    for (size_t i = 0; i < num_of_tri; i++) {
        Tri temp_tri = {0};

        fread(&(temp_tri.normals[0].x), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.normals[0].y), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.normals[0].z), STL_NUM_SIZE, 1, file);

        temp_tri.normals[0].x = - temp_tri.normals[0].x;
        temp_tri.normals[0].y = - temp_tri.normals[0].y;
        temp_tri.normals[0].z = - temp_tri.normals[0].z;

        temp_tri.normals[1] = temp_tri.normals[0];
        temp_tri.normals[2] = temp_tri.normals[0];

        fread(&(temp_tri.points[0].x), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[0].y), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[0].z), STL_NUM_SIZE, 1, file);

        fread(&(temp_tri.points[1].x), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[1].y), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[1].z), STL_NUM_SIZE, 1, file);
        
        fread(&(temp_tri.points[2].x), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[2].y), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[2].z), STL_NUM_SIZE, 1, file);

        fseek(file, STL_ATTRIBUTE_BITS_SIZE, SEEK_CUR);

        temp_tri.to_draw = true;
        temp_tri.light_intensity[0] = 1;
        temp_tri.light_intensity[1] = 1;
        temp_tri.light_intensity[2] = 1;
        temp_tri.center.x = (temp_tri.points[0].x + temp_tri.points[1].x + temp_tri.points[2].x) / 3;
        temp_tri.center.y = (temp_tri.points[0].y + temp_tri.points[1].y + temp_tri.points[2].y) / 3;
        temp_tri.center.z = (temp_tri.points[0].z + temp_tri.points[1].z + temp_tri.points[2].z) / 3;
        temp_tri.z_min = fmin(temp_tri.points[0].z, fmin(temp_tri.points[1].z, temp_tri.points[2].z));
        temp_tri.z_max = fmax(temp_tri.points[0].z, fmax(temp_tri.points[1].z, temp_tri.points[2].z));
        temp_tri.colors[0] = 0xFFFFFFFF;
        temp_tri.colors[1] = 0xFFFFFFFF;
        temp_tri.colors[2] = 0xFFFFFFFF;

        // ae_tri_set_normals(&temp_tri);

        ada_appand(Tri, mesh, temp_tri);
    }

    return mesh;
}

Tri_mesh ae_tri_mesh_get_from_file(char *file_path)
{
    char file_extention[MAX_LEN_LINE], temp_word[MAX_LEN_LINE];

    strncpy(file_extention, file_path, MAX_LEN_LINE);

    int num_of_dots;
    while ((num_of_dots = asm_str_in_str(file_extention, ".")) >= 1) {
        asm_get_word_and_cut(temp_word, file_extention, '.');
    }

    if (!(!strncmp(file_extention, "obj", 3) || !strncmp(file_extention, "STL", 3) || !strncmp(file_extention, "stl", 3))) {
        fprintf(stderr, "%s:%d: [Error] unsupported file format: '%s'\n", __FILE__, __LINE__, file_path);
        exit(1);
    }

    if (!strncmp(file_extention, "STL", 3) || !strncmp(file_extention, "stl", 3)) {
        return ae_tri_mesh_get_from_stl_file(file_path);
    } else if (!strncmp(file_extention, "obj", 3)) {
        return ae_tri_mesh_get_from_obj_file(file_path);
    }
    
    Tri_mesh null_mesh = {0};
    return null_mesh;
}

void ae_tri_mesh_appand_copy(Tri_mesh_array *mesh_array, Tri_mesh mesh)
{
    Tri_mesh_array temp_mesh_array = *mesh_array;
    Tri_mesh temp_mesh;
    ada_init_array(Tri, temp_mesh);
    for (size_t i = 0; i < mesh.length; i++) {
        ada_appand(Tri, temp_mesh, mesh.elements[i]);
    }
    ada_appand(Tri_mesh, temp_mesh_array, temp_mesh);

    *mesh_array = temp_mesh_array;
}

Tri_mesh ae_tri_mesh_get_from_quad_mesh(Quad_mesh q_mesh)
{
    Tri_mesh t_mesh;
    ada_init_array(Tri, t_mesh);

    for (size_t q_index = 0; q_index < q_mesh.length; q_index++) {
        Quad current_q = q_mesh.elements[q_index];
        Tri temp_t = {.to_draw = current_q.to_draw};

        temp_t.points[0] = current_q.points[0];
        temp_t.colors[0] = current_q.colors[0];
        temp_t.normals[0] = current_q.normals[0];
        temp_t.light_intensity[0] = current_q.light_intensity[0];
        temp_t.points[1] = current_q.points[1];
        temp_t.colors[1] = current_q.colors[1];
        temp_t.normals[1] = current_q.normals[1];
        temp_t.light_intensity[1] = current_q.light_intensity[1];
        temp_t.points[2] = current_q.points[2];
        temp_t.colors[2] = current_q.colors[2];
        temp_t.normals[2] = current_q.normals[2];
        temp_t.light_intensity[2] = current_q.light_intensity[2];

        ada_appand(Tri, t_mesh, temp_t);

        temp_t.points[0] = current_q.points[2];
        temp_t.colors[0] = current_q.colors[2];
        temp_t.normals[0] = current_q.normals[2];
        temp_t.light_intensity[0] = current_q.light_intensity[2];
        temp_t.points[1] = current_q.points[3];
        temp_t.colors[1] = current_q.colors[3];
        temp_t.normals[1] = current_q.normals[3];
        temp_t.light_intensity[1] = current_q.light_intensity[3];
        temp_t.points[2] = current_q.points[0];
        temp_t.colors[2] = current_q.colors[0];
        temp_t.normals[2] = current_q.normals[0];
        temp_t.light_intensity[2] = current_q.light_intensity[0];

        ada_appand(Tri, t_mesh, temp_t);
    }

    return t_mesh;
}

void ae_print_points(Curve p)
{
    for (size_t i = 0; i < p.length; i++) {
        printf("point %3zu: (%5f, %5f, %5f)\n", i, p.elements[i].x, p.elements[i].y, p.elements[i].z);
    }
}

void ae_print_tri(Tri tri, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);
    printf("%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n", (int) padding, "", tri.points[0].x, tri.points[0].y, tri.points[0].z, (int) padding, "", tri.points[1].x, tri.points[1].y, tri.points[1].z, (int) padding, "", tri.points[2].x, tri.points[2].y, tri.points[2].z);
    printf("%*s    draw? %d\n", (int)padding, "", tri.to_draw);
}

void ae_print_tri_mesh(Tri_mesh mesh, char *name, size_t padding)
{
    char tri_name[256];
    printf("%*s%s:\n", (int) padding, "", name);
    for (size_t i = 0; i < mesh.length; i++) {
        snprintf(tri_name, 256, "tri %zu", i);
        ae_print_tri(mesh.elements[i], tri_name, 4);
    }
}

Point ae_point_normalize_xyz(Point p)
{
    Point res = {0};

    float norma = ae_point_calc_norma(p);

    res.x = p.x / norma;
    res.y = p.y / norma;
    res.z = p.z / norma;
    res.w = p.w;

    return res;
}

void ae_tri_set_normals(Tri *tri)
{
    ae_assert_tri_is_valid(*tri);

    Mat2D point  = mat2D_alloc(3, 1);
    Mat2D to_p   = mat2D_alloc(3, 1);
    Mat2D from_p = mat2D_alloc(3, 1);
    Mat2D normal = mat2D_alloc(3, 1);

    for (int i = 0; i < 3; i++) {
        int current_index  = i;
        int next_index     = (i + 1) % 3;
        int previous_index = (i - 1 + 3) % 3;
        ae_point_to_mat2D(tri->points[current_index], point);
        ae_point_to_mat2D(tri->points[next_index], from_p);
        ae_point_to_mat2D(tri->points[previous_index], to_p);

        mat2D_sub(from_p, point);
        mat2D_sub(point, to_p);

        mat2D_copy(to_p, point);

        mat2D_cross(normal, to_p, from_p);
        // mat2D_cross(normal, from_p, to_p);
        mat2D_normalize(normal);

        tri->normals[current_index] = ae_mat2D_to_point(normal);
    }

    mat2D_free(point);
    mat2D_free(to_p);
    mat2D_free(from_p);
    mat2D_free(normal);
}

Point ae_tri_get_average_normal(Tri tri)
{
    Point normal0 = tri.normals[0];
    Point normal1 = tri.normals[1];
    Point normal2 = tri.normals[2];

    Point res;
    res.x = (normal0.x + normal1.x + normal2.x) / 3;
    res.y = (normal0.y + normal1.y + normal2.y) / 3;
    res.z = (normal0.z + normal1.z + normal2.z) / 3;
    res.w = (normal0.w + normal1.w + normal2.w) / 3;

    return ae_point_normalize_xyz(res);
}

Point ae_tri_get_average_point(Tri tri)
{
    Point point0 = tri.points[0];
    Point point1 = tri.points[1];
    Point point2 = tri.points[2];

    Point res;
    res.x = (point0.x + point1.x + point2.x) / 3;
    res.y = (point0.y + point1.y + point2.y) / 3;
    res.z = (point0.z + point1.z + point2.z) / 3;
    res.w = (point0.w + point1.w + point2.w) / 3;

    return res;
}

void ae_tri_calc_normal(Mat2D normal, Tri tri)
{
    AE_ASSERT(3 == normal.rows && 1 == normal.cols);
    ae_assert_tri_is_valid(tri);

    Mat2D a = mat2D_alloc(3, 1);
    Mat2D b = mat2D_alloc(3, 1);
    Mat2D c = mat2D_alloc(3, 1);

    ae_point_to_mat2D(tri.points[0], a);
    ae_point_to_mat2D(tri.points[1], b);
    ae_point_to_mat2D(tri.points[2], c);

    mat2D_sub(b, a);
    mat2D_sub(c, a);

    mat2D_cross(normal, b, c);

    mat2D_mult(normal, 1/mat2D_calc_norma(normal));

    mat2D_free(a);
    mat2D_free(b);
    mat2D_free(c);
}

void ae_tri_mesh_translate(Tri_mesh mesh, float x, float y, float z)
{
    for (size_t i = 0; i < mesh.length; i++) {
        for (int j = 0; j < 3; j++) {
            mesh.elements[i].points[j].x += x;
            mesh.elements[i].points[j].y += y;
            mesh.elements[i].points[j].z += z;
        }
    }
}

/* phi around x, theta around y, psi around z.
DCM = Cz*Cy*Cx */
void ae_tri_mesh_rotate_Euler_xyz(Tri_mesh mesh, float phi_deg, float theta_deg, float psi_deg)
{
    Mat2D RotZ = mat2D_alloc(3,3);
    mat2D_set_rot_mat_z(RotZ, psi_deg);
    Mat2D RotY = mat2D_alloc(3,3);
    mat2D_set_rot_mat_y(RotY, theta_deg);
    Mat2D RotX = mat2D_alloc(3,3);
    mat2D_set_rot_mat_x(RotX, phi_deg);
    Mat2D DCM = mat2D_alloc(3,3);
    // mat2D_fill(DCM,0);
    Mat2D temp = mat2D_alloc(3,3);
    // mat2D_fill(temp,0);
    mat2D_dot(temp, RotY, RotZ);
    mat2D_dot(DCM, RotX, temp); /* I have a DCM */

    Mat2D src_point_mat = mat2D_alloc(3,1);
    Mat2D des_point_mat = mat2D_alloc(3,1);

    for (size_t i = 0; i < mesh.length; i++) {
        for (int j = 0; j < 3; j++) {
            // mat2D_fill(src_point_mat, 0);
            // mat2D_fill(des_point_mat, 0);
            Point des;
            Point src = mesh.elements[i].points[j];

            MAT2D_AT(src_point_mat, 0, 0) = src.x;
            MAT2D_AT(src_point_mat, 1, 0) = src.y;
            MAT2D_AT(src_point_mat, 2, 0) = src.z;

            mat2D_dot(des_point_mat, DCM, src_point_mat);

            des.x = MAT2D_AT(des_point_mat, 0, 0);
            des.y = MAT2D_AT(des_point_mat, 1, 0);
            des.z = MAT2D_AT(des_point_mat, 2, 0);

            mesh.elements[i].points[j] = des;
        }
    }

    mat2D_free(RotZ);
    mat2D_free(RotY);
    mat2D_free(RotX);
    mat2D_free(DCM);
    mat2D_free(src_point_mat);
    mat2D_free(des_point_mat);
}

void ae_tri_mesh_set_bounding_box(Tri_mesh mesh, float *x_min, float *x_max, float *y_min, float *y_max, float *z_min, float *z_max)
{
    float xmin = FLT_MAX, xmax = FLT_MIN;
    float ymin = FLT_MAX, ymax = FLT_MIN;
    float zmin = FLT_MAX, zmax = FLT_MIN;

    float x, y, z;

    for (size_t t = 0; t < mesh.length; t++) {
        for (size_t p = 0; p < 3; p++) {
            x = mesh.elements[t].points[p].x;
            y = mesh.elements[t].points[p].y;
            z = mesh.elements[t].points[p].z;
            if (x > xmax) xmax = x;
            if (x < xmin) xmin = x;
            if (y > ymax) ymax = y;
            if (y < ymin) ymin = y;
            if (z > zmax) zmax = z;
            if (z < zmin) zmin = z;
        }
    }
    *x_min = xmin;
    *x_max = xmax; 
    *y_min = ymin;
    *y_max = ymax;
    *z_min = zmin;
    *z_max = zmax;
}

void ae_tri_set_center_zmin_zmax(Tri *tri)
{
    ae_assert_tri_is_valid(*tri);
    tri->center.x = (tri->points[0].x + tri->points[1].x + tri->points[2].x) / 3;
    tri->center.y = (tri->points[0].y + tri->points[1].y + tri->points[2].y) / 3;
    tri->center.z = (tri->points[0].z + tri->points[1].z + tri->points[2].z) / 3;
    tri->z_min = fmin(tri->points[0].z, fmin(tri->points[1].z, tri->points[2].z));
    tri->z_max = fmax(tri->points[0].z, fmax(tri->points[1].z, tri->points[2].z));
}

/* normalize all the points in between -1 and 1. the origin is in the center of the body. */
void ae_tri_mesh_normalize(Tri_mesh mesh)
{
    float xmax, xmin, ymax, ymin, zmax, zmin;
    ae_tri_mesh_set_bounding_box(mesh, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax);

    for (size_t t = 0; t < mesh.length; t++) {
        for (size_t p = 0; p < 3; p++) {
            float x, y, z;
            x = mesh.elements[t].points[p].x;
            y = mesh.elements[t].points[p].y;
            z = mesh.elements[t].points[p].z;

            float xdiff = xmax-xmin;
            float ydiff = ymax-ymin;
            float zdiff = zmax-zmin;
            float max_diff = fmax(xdiff, fmax(ydiff, zdiff));
            float xfactor = xdiff/max_diff;
            float yfactor = ydiff/max_diff;
            float zfactor = zdiff/max_diff;

            x = (((x - xmin) / (xdiff)) * 2 - 1) * xfactor;
            y = (((y - ymin) / (ydiff)) * 2 - 1) * yfactor;
            z = (((z - zmin) / (zdiff)) * 2 - 1) * zfactor;

            // ae_set_tri_center_zmin_zmax(&(mesh.elements[t]));

            mesh.elements[t].points[p].x = x;
            mesh.elements[t].points[p].y = y;
            mesh.elements[t].points[p].z = z;
        }
    }
}

void ae_tri_mesh_flip_normals(Tri_mesh mesh)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Tri res_tri, tri = mesh.elements[i];

        res_tri.center  = tri.center;
        res_tri.to_draw = tri.to_draw;
        res_tri.z_max   = tri.z_max;
        res_tri.z_min   = tri.z_min;

        res_tri.colors[0]          = tri.colors[2];
        res_tri.light_intensity[0] = tri.light_intensity[2];
        res_tri.normals[0]         = tri.normals[2];
        res_tri.points[0]          = tri.points[2];
        res_tri.tex_points[0]      = tri.tex_points[2];

        res_tri.colors[1]          = tri.colors[1];
        res_tri.light_intensity[1] = tri.light_intensity[1];
        res_tri.normals[1]         = tri.normals[1];
        res_tri.points[1]          = tri.points[1];
        res_tri.tex_points[1]      = tri.tex_points[1];

        res_tri.colors[2]          = tri.colors[0];
        res_tri.light_intensity[2] = tri.light_intensity[0];
        res_tri.normals[2]         = tri.normals[0];
        res_tri.points[2]          = tri.points[0];
        res_tri.tex_points[2]      = tri.tex_points[0];

        ae_tri_set_normals(&res_tri);

        mesh.elements[i] = res_tri;
    }
}

void ae_quad_set_normals(Quad *quad)
{
    ae_assert_quad_is_valid(*quad);

    Mat2D point  = mat2D_alloc(3, 1);
    Mat2D to_p   = mat2D_alloc(3, 1);
    Mat2D from_p = mat2D_alloc(3, 1);
    Mat2D normal = mat2D_alloc(3, 1);

    for (int i = 0; i < 4; i++) {
        int current_index  = i;
        int next_index     = (i + 1) % 4;
        int previous_index = (i - 1 + 4) % 4;
        ae_point_to_mat2D(quad->points[current_index], point);
        ae_point_to_mat2D(quad->points[next_index], from_p);
        ae_point_to_mat2D(quad->points[previous_index], to_p);

        mat2D_sub(from_p, point);
        mat2D_sub(point, to_p);

        mat2D_copy(to_p, point);

        mat2D_cross(normal, to_p, from_p);
        mat2D_normalize(normal);

        quad->normals[current_index] = ae_mat2D_to_point(normal);
    }

    mat2D_free(point);
    mat2D_free(to_p);
    mat2D_free(from_p);
    mat2D_free(normal);

}

Point ae_quad_get_average_normal(Quad quad)
{
    Point normal0 = quad.normals[0];
    Point normal1 = quad.normals[1];
    Point normal2 = quad.normals[2];
    Point normal3 = quad.normals[3];

    Point res;
    res.x = (normal0.x + normal1.x + normal2.x + normal3.x) / 4;
    res.y = (normal0.y + normal1.y + normal2.y + normal3.y) / 4;
    res.z = (normal0.z + normal1.z + normal2.z + normal3.z) / 4;
    res.w = (normal0.w + normal1.w + normal2.w + normal3.w) / 4;

    res = ae_point_normalize_xyz(res);

    return res;
}

Point ae_quad_get_average_point(Quad quad)
{
    Point point0 = quad.points[0];
    Point point1 = quad.points[1];
    Point point2 = quad.points[2];
    Point point3 = quad.points[3];

    Point res;
    res.x = (point0.x + point1.x + point2.x + point3.x) / 4;
    res.y = (point0.y + point1.y + point2.y + point3.y) / 4;
    res.z = (point0.z + point1.z + point2.z + point3.z) / 4;
    res.w = (point0.w + point1.w + point2.w + point3.w) / 4;

    return res;
}

void ae_quad_calc_normal(Mat2D normal, Quad quad)
{
    AE_ASSERT(3 == normal.rows && 1 == normal.cols);
    ae_assert_quad_is_valid(quad);

    Mat2D a = mat2D_alloc(3, 1);
    Mat2D b = mat2D_alloc(3, 1);
    Mat2D c = mat2D_alloc(3, 1);

    ae_point_to_mat2D(quad.points[0], a);
    ae_point_to_mat2D(quad.points[1], b);
    ae_point_to_mat2D(quad.points[2], c);

    mat2D_sub(b, a);
    mat2D_sub(c, a);

    mat2D_cross(normal, b, c);

    mat2D_mult(normal, 1/mat2D_calc_norma(normal));

    mat2D_free(a);
    mat2D_free(b);
    mat2D_free(c);
}

void ae_curve_copy(Curve *des, Curve src)
{
    Curve temp_des = *des;
    temp_des.length = 0;

    for (size_t i = 0; i < src.length; i++) {
        ada_appand(Point, temp_des, src.elements[i]);
    }

    *des = temp_des;
}

void ae_tri_calc_light_intensity(Tri *tri, Scene *scene, Lighting_mode lighting_mode)
{
    /* based on the lighting model described in: 'Alexandru C. Telea-Data Visualization_ Principles and Practice-A K Peters_CRC Press (2014)' Pg.29 */
    Point L = {0};
    Point r = {0};
    Point v = {0};
    Point mL = {0};
    Point pml = {0};
    Point mLn2n = {0};
    Point ave_norm = ae_tri_get_average_normal(*tri);
    Point camera_pos = ae_mat2D_to_point(scene->camera.current_position);

    float c_ambi = scene->material0.c_ambi;
    float c_diff = scene->material0.c_diff;
    float c_spec = scene->material0.c_spec;
    float alpha  = scene->material0.specular_power_alpha;

    switch (lighting_mode) {
    case AE_LIGHTING_FLAT:
        for (int i = 0; i < 3; i++) {
            if (scene->light_source0.light_direction_or_pos.w == 0) {
                L = scene->light_source0.light_direction_or_pos;
                L = ae_point_normalize_xyz(L);
                mL = L;
                ae_point_mult(mL, -1);
            } else {
                Point l = scene->light_source0.light_direction_or_pos;
                Point p = tri->points[i];
                ae_point_sub_point(pml, p, l);
                pml = ae_point_normalize_xyz(pml);
                L = pml;
                L.w = 0;
                mL = L;
                ae_point_mult(mL, -1);
            }
            
            ae_point_sub_point(v, camera_pos, ae_tri_get_average_point(*tri));
            float mL_dot_norm = ae_point_dot_point(mL, ave_norm);
            mLn2n = ave_norm;
            ae_point_mult(mLn2n, 2 * mL_dot_norm);
            ae_point_add_point(r, L, mLn2n);
            
            tri->light_intensity[i] = c_ambi + scene->light_source0.light_intensity * (c_diff * fmaxf(mL_dot_norm, 0) + c_spec * powf(fmaxf(ae_point_dot_point(r, v), 0), alpha));
        }
        break;
    case AE_LIGHTING_SMOOTH:
        for (int i = 0; i < 3; i++) {
            if (scene->light_source0.light_direction_or_pos.w == 0) {
                L = scene->light_source0.light_direction_or_pos;
                L = ae_point_normalize_xyz(L);
                mL = L;
                ae_point_mult(mL, -1);
            } else {
                Point l = scene->light_source0.light_direction_or_pos;
                Point p = tri->points[i];
                ae_point_sub_point(pml, p, l);
                pml = ae_point_normalize_xyz(pml);
                L = pml;
                L.w = 0;
                mL = L;
                ae_point_mult(mL, -1);
            }
            ae_point_sub_point(v, camera_pos, tri->points[i]);
            float mL_dot_norm = ae_point_dot_point(mL, tri->normals[i]);
            mLn2n = tri->normals[i];
            ae_point_mult(mLn2n, 2 * mL_dot_norm);
            ae_point_add_point(r, L, mLn2n);
            
            tri->light_intensity[i] = c_ambi + scene->light_source0.light_intensity * (c_diff * fmaxf(mL_dot_norm, 0) + c_spec * powf(fmaxf(ae_point_dot_point(r, v), 0), alpha));
        }
        break;
    default:
        for (int i = 0; i < 3; i++) {
            tri->light_intensity[i] = 1;
        }
        break;
    }

    for (int i = 0; i < 3; i++) {
        tri->light_intensity[i] = fminf(1, fmaxf(0, tri->light_intensity[i]));
    }
}

void ae_quad_calc_light_intensity(Quad *quad, Scene *scene, Lighting_mode lighting_mode)
{
    /* based on the lighting model described in: 'Alexandru C. Telea-Data Visualization_ Principles and Practice-A K Peters_CRC Press (2014)' Pg.29 */
    Point L = {0};
    Point r = {0};
    Point v = {0};
    Point mL = {0};
    Point pml = {0};
    Point mLn2n = {0};
    Point ave_norm = ae_quad_get_average_normal(*quad);
    Point camera_pos = ae_mat2D_to_point(scene->camera.current_position);

    float c_ambi = scene->material0.c_ambi;
    float c_diff = scene->material0.c_diff;
    float c_spec = scene->material0.c_spec;
    float alpha  = scene->material0.specular_power_alpha;

    switch (lighting_mode) {
    case AE_LIGHTING_FLAT:
        for (int i = 0; i < 4; i++) {
            if (scene->light_source0.light_direction_or_pos.w == 0) {
                L = scene->light_source0.light_direction_or_pos;
                L = ae_point_normalize_xyz(L);
                mL = L;
                ae_point_mult(mL, -1);
            } else {
                Point l = scene->light_source0.light_direction_or_pos;
                Point p = quad->points[i];
                ae_point_sub_point(pml, p, l);
                pml = ae_point_normalize_xyz(pml);
                L = pml;
                L.w = 0;
                mL = L;
                ae_point_mult(mL, -1);
            }
            
            ae_point_sub_point(v, camera_pos, ae_quad_get_average_point(*quad));
            float mL_dot_norm = ae_point_dot_point(mL, ave_norm);
            mLn2n = ave_norm;
            ae_point_mult(mLn2n, 2 * mL_dot_norm);
            ae_point_add_point(r, L, mLn2n);
            
            quad->light_intensity[i] = c_ambi + scene->light_source0.light_intensity * (c_diff * fmaxf(mL_dot_norm, 0) + c_spec * powf(fmaxf(ae_point_dot_point(r, v), 0), alpha));
        }
        break;
    case AE_LIGHTING_SMOOTH:
        for (int i = 0; i < 4; i++) {
            if (scene->light_source0.light_direction_or_pos.w == 0) {
                L = scene->light_source0.light_direction_or_pos;
                L = ae_point_normalize_xyz(L);
                mL = L;
                ae_point_mult(mL, -1);
            } else {
                Point l = scene->light_source0.light_direction_or_pos;
                Point p = quad->points[i];
                ae_point_sub_point(pml, p, l);
                pml = ae_point_normalize_xyz(pml);
                L = pml;
                L.w = 0;
                mL = L;
                ae_point_mult(mL, -1);
            }
            ae_point_sub_point(v, camera_pos, quad->points[i]);
            float mL_dot_norm = ae_point_dot_point(mL, quad->normals[i]);
            mLn2n = quad->normals[i];
            ae_point_mult(mLn2n, 2 * mL_dot_norm);
            ae_point_add_point(r, L, mLn2n);
            
            quad->light_intensity[i] = c_ambi + scene->light_source0.light_intensity * (c_diff * fmaxf(mL_dot_norm, 0) + c_spec * powf(fmaxf(ae_point_dot_point(r, v), 0), alpha));
        }
        break;
    default:
        for (int i = 0; i < 4; i++) {
            quad->light_intensity[i] = 1;
        }
        break;
    }

    for (int i = 0; i < 4; i++) {
        quad->light_intensity[i] = fminf(1, fmaxf(0, quad->light_intensity[i]));
    }
}

Point ae_line_itersect_plane(Mat2D plane_p, Mat2D plane_n, Mat2D line_start, Mat2D line_end, float *t)
{
    mat2D_normalize(plane_n);
    float plane_d = - mat2D_dot_product(plane_n, plane_p);
    float ad = mat2D_dot_product(line_start, plane_n);
    float bd = mat2D_dot_product(line_end, plane_n);
    *t  = (- plane_d - ad) / (bd - ad);
    mat2D_sub(line_end, line_start);
    Mat2D line_start_to_end = line_end;
    mat2D_mult(line_start_to_end, *t);
    Mat2D line_to_intersection = line_start_to_end;
    
    Mat2D intersection_p = mat2D_alloc(3, 1);
    mat2D_fill(intersection_p, 0);
    mat2D_add(intersection_p, line_start);
    mat2D_add(intersection_p, line_to_intersection);

    Point ans_p = ae_mat2D_to_point(intersection_p);

    mat2D_free(intersection_p);

    return ans_p;
}

/* signed distance from point to plane */
float ae_signed_dist_point_and_plane(Point p, Mat2D plane_p, Mat2D plane_n)
{
    ae_assert_point_is_valid(p);

    // mat2D_normalize(plane_n);
    // Mat2D p_mat2D = mat2D_alloc(3, 1);
    // ae_point_to_mat2D(p, p_mat2D);

    // float res = mat2D_dot_product(plane_n, p_mat2D) - mat2D_dot_product(plane_n, plane_p); 

    float res = MAT2D_AT(plane_n, 0, 0) * p.x + MAT2D_AT(plane_n, 1, 0) * p.y + MAT2D_AT(plane_n, 2, 0) * p.z - (MAT2D_AT(plane_n, 0, 0) * MAT2D_AT(plane_p, 0, 0) + MAT2D_AT(plane_n, 1, 0) * MAT2D_AT(plane_p, 1, 0) + MAT2D_AT(plane_n, 2, 0) * MAT2D_AT(plane_p, 2, 0)); 

    // mat2D_free(p_mat2D);

    return res;
}

int ae_line_clip_with_plane(Point start_in, Point end_in, Mat2D plane_p, Mat2D plane_n, Point *start_out, Point *end_out)
{
    ae_assert_point_is_valid(start_in);
    ae_assert_point_is_valid(end_in);

    mat2D_normalize(plane_n);

    /* if the signed distance is positive, the point lies on the "inside" of the plane */
    Point inside_points[2];
    Point outside_points[2];
    int inside_points_count = 0;
    int outside_points_count = 0;
    
    /* calc signed distance of each point of tri_in */
    float d0 = ae_signed_dist_point_and_plane(start_in, plane_p, plane_n);
    float d1 = ae_signed_dist_point_and_plane(end_in, plane_p, plane_n);
    float t;

    // float epsilon = 1e-3;
    float epsilon = 0;
    if (d0 >= epsilon) {
        inside_points[inside_points_count++] = start_in;
    } else {
        outside_points[outside_points_count++] = start_in;
    }
    if (d1 >= epsilon) {
        inside_points[inside_points_count++] = end_in;
    } else {
        outside_points[outside_points_count++] = end_in;
    }

    /* classifying the triangle points */
    if (outside_points_count == 2) {
        return 0;
    } else if (inside_points_count == 2) {
        *start_out = start_in;
        *end_out = end_in;
        return 1;
    } else if (d0 >= epsilon && d1 < epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);

        *start_out = inside_points[0];

        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        *end_out = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);

        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_point_is_valid(*start_out);
        ae_assert_point_is_valid(*end_out);

        return 1;
    } else if (d1 >= epsilon && d0 < epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);

        *end_out = inside_points[0];

        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        *start_out = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);

        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_point_is_valid(*start_out);
        ae_assert_point_is_valid(*end_out);

        return 1;
    }
    return -1;
}

/* returns number of inside triangles
return -1 on error */
int ae_tri_clip_with_plane(Tri tri_in, Mat2D plane_p, Mat2D plane_n, Tri *tri_out1, Tri *tri_out2)
{
    ae_assert_tri_is_valid(tri_in);

    mat2D_normalize(plane_n);

    /* if the signed distance is positive, the point lies on the "inside" of the plane */
    Point inside_points[3];
    Point outside_points[3];
    int inside_points_count = 0;
    int outside_points_count = 0;
    Point tex_inside_points[3];
    Point tex_outside_points[3];
    int tex_inside_points_count = 0;
    int tex_outside_points_count = 0;
    
    /* calc signed distance of each point of tri_in */
    float d0 = ae_signed_dist_point_and_plane(tri_in.points[0], plane_p, plane_n);
    float d1 = ae_signed_dist_point_and_plane(tri_in.points[1], plane_p, plane_n);
    float d2 = ae_signed_dist_point_and_plane(tri_in.points[2], plane_p, plane_n);
    float t;

    // float epsilon = 1e-3;
    float epsilon = 0;
    if (d0 >= epsilon) {
        inside_points[inside_points_count++] = tri_in.points[0];
        tex_inside_points[tex_inside_points_count++] = tri_in.tex_points[0];
    } else {
        outside_points[outside_points_count++] = tri_in.points[0];
        tex_outside_points[tex_outside_points_count++] = tri_in.tex_points[0];
    }
    if (d1 >= epsilon) {
        inside_points[inside_points_count++] = tri_in.points[1];
        tex_inside_points[tex_inside_points_count++] = tri_in.tex_points[1];
    } else {
        outside_points[outside_points_count++] = tri_in.points[1];
        tex_outside_points[tex_outside_points_count++] = tri_in.tex_points[1];
    }
    if (d2 >= epsilon) {
        inside_points[inside_points_count++] = tri_in.points[2];
        tex_inside_points[tex_inside_points_count++] = tri_in.tex_points[2];
    } else {
        outside_points[outside_points_count++] = tri_in.points[2];
        tex_outside_points[tex_outside_points_count++] = tri_in.tex_points[2];
    }

    /* classifying the triangle points */
    if (inside_points_count == 0) {
        return 0;
    } else if (inside_points_count == 3) {
        *tri_out1 = tri_in;
        return 1;
    } else if (inside_points_count == 1 && outside_points_count == 2 && d2 >= epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);

        *tri_out1 = tri_in;
        // tri_out1->colors[0] = 0xFF0000;
        // tri_out1->colors[1] = 0xFF0000;
        // tri_out1->colors[2] = 0xFF0000;
        
        (*tri_out1).points[0] = inside_points[0];
        (*tri_out1).tex_points[0] = tex_inside_points[0];

        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        (*tri_out1).points[1] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*tri_out1).points[1].w = t * (outside_points[0].w - inside_points[0].w) + inside_points[0].w;
        (*tri_out1).tex_points[1].x = t * (tex_outside_points[0].x - tex_inside_points[0].x) + tex_inside_points[0].x;
        (*tri_out1).tex_points[1].y = t * (tex_outside_points[0].y - tex_inside_points[0].y) + tex_inside_points[0].y;

        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[1], line_end);
        (*tri_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*tri_out1).points[2].w = t * (outside_points[1].w - inside_points[0].w) + inside_points[0].w;
        (*tri_out1).tex_points[2].x = t * (tex_outside_points[1].x - tex_inside_points[0].x) + tex_inside_points[0].x;
        (*tri_out1).tex_points[2].y = t * (tex_outside_points[1].y - tex_inside_points[0].y) + tex_inside_points[0].y;

        mat2D_free(line_start);
        mat2D_free(line_end);

        /* fixing color ordering */
        uint32_t temp_color = tri_out1->colors[2]; 
        tri_out1->colors[2] = tri_out1->colors[1];
        tri_out1->colors[1] = tri_out1->colors[0];
        tri_out1->colors[0] = temp_color;

        ae_assert_tri_is_valid(*tri_out1);

        return 1;
    } else if (inside_points_count == 1 && outside_points_count == 2 && d1 >= epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);

        *tri_out1 = tri_in;
        // tri_out1->colors[0] = 0xFF0000;
        // tri_out1->colors[1] = 0xFF0000;
        // tri_out1->colors[2] = 0xFF0000;
        
        (*tri_out1).points[0] = inside_points[0];
        (*tri_out1).tex_points[0] = tex_inside_points[0];

        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        (*tri_out1).points[1] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*tri_out1).points[1].w = t * (outside_points[0].w - inside_points[0].w) + inside_points[0].w;
        (*tri_out1).tex_points[1].x = t * (tex_outside_points[0].x - tex_inside_points[0].x) + tex_inside_points[0].x;
        (*tri_out1).tex_points[1].y = t * (tex_outside_points[0].y - tex_inside_points[0].y) + tex_inside_points[0].y;

        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[1], line_end);
        (*tri_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*tri_out1).points[2].w = t * (outside_points[1].w - inside_points[0].w) + inside_points[0].w;
        (*tri_out1).tex_points[2].x = t * (tex_outside_points[1].x - tex_inside_points[0].x) + tex_inside_points[0].x;
        (*tri_out1).tex_points[2].y = t * (tex_outside_points[1].y - tex_inside_points[0].y) + tex_inside_points[0].y;

        mat2D_free(line_start);
        mat2D_free(line_end);

        /* fixing color ordering */
        uint32_t temp_color = tri_out1->colors[2]; 
        tri_out1->colors[2] = tri_out1->colors[1];
        tri_out1->colors[1] = tri_out1->colors[0];
        tri_out1->colors[0] = temp_color;

        temp_color = tri_out1->colors[2]; 
        tri_out1->colors[2] = tri_out1->colors[0];
        tri_out1->colors[0] = temp_color;

        ae_assert_tri_is_valid(*tri_out1);

        return 1;
    } else if (inside_points_count == 1 && outside_points_count == 2 && d0 >= epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);

        *tri_out1 = tri_in;
        // tri_out1->colors[0] = 0xFF0000;
        // tri_out1->colors[1] = 0xFF0000;
        // tri_out1->colors[2] = 0xFF0000;
        
        (*tri_out1).points[0] = inside_points[0];
        (*tri_out1).tex_points[0] = tex_inside_points[0];

        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        (*tri_out1).points[1] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*tri_out1).points[1].w = t * (outside_points[0].w - inside_points[0].w) + inside_points[0].w;
        (*tri_out1).tex_points[1].x = t * (tex_outside_points[0].x - tex_inside_points[0].x) + tex_inside_points[0].x;
        (*tri_out1).tex_points[1].y = t * (tex_outside_points[0].y - tex_inside_points[0].y) + tex_inside_points[0].y;

        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[1], line_end);
        (*tri_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*tri_out1).points[2].w = t * (outside_points[1].w - inside_points[0].w) + inside_points[0].w;
        (*tri_out1).tex_points[2].x = t * (tex_outside_points[1].x - tex_inside_points[0].x) + tex_inside_points[0].x;
        (*tri_out1).tex_points[2].y = t * (tex_outside_points[1].y - tex_inside_points[0].y) + tex_inside_points[0].y;

        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_tri_is_valid(*tri_out1);

        return 1;
    } else if (inside_points_count == 2 && outside_points_count == 1 && d2 < epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);
        
        *tri_out1 = tri_in;
        // tri_out1->colors[0] = 0x00FF00;
        // tri_out1->colors[1] = 0x00FF00;
        // tri_out1->colors[2] = 0x00FF00;

        *tri_out2 = tri_in;
        // tri_out2->colors[0] = 0x0000FF;
        // tri_out2->colors[1] = 0x0000FF;
        // tri_out2->colors[2] = 0x0000FF;

        (*tri_out1).points[0] = inside_points[0];
        (*tri_out1).tex_points[0] = tex_inside_points[0];
        (*tri_out1).points[1] = inside_points[1];
        (*tri_out1).tex_points[1] = tex_inside_points[1];
        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        (*tri_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*tri_out1).points[2].w = t * (outside_points[0].w - inside_points[0].w) + inside_points[0].w;
        (*tri_out1).tex_points[2].x = t * (tex_outside_points[0].x - tex_inside_points[0].x) + tex_inside_points[0].x;
        (*tri_out1).tex_points[2].y = t * (tex_outside_points[0].y - tex_inside_points[0].y) + tex_inside_points[0].y;

        (*tri_out2).points[0] = inside_points[1];
        (*tri_out2).tex_points[0] = tex_inside_points[1];
        ae_point_to_mat2D(inside_points[1], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        (*tri_out2).points[1] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*tri_out2).points[1].w = t * (outside_points[0].w - inside_points[1].w) + inside_points[1].w;
        (*tri_out2).tex_points[1].x = t * (tex_outside_points[0].x - tex_inside_points[1].x) + tex_inside_points[1].x;
        (*tri_out2).tex_points[1].y = t * (tex_outside_points[0].y - tex_inside_points[1].y) + tex_inside_points[1].y;
        (*tri_out2).points[2] = (*tri_out1).points[2];
        (*tri_out2).tex_points[2] = (*tri_out1).tex_points[2];

        mat2D_free(line_start);
        mat2D_free(line_end);

        /* fixing color ordering */
        uint32_t temp_color = tri_out2->colors[2]; 
        tri_out2->colors[2] = tri_out2->colors[0];
        tri_out2->colors[0] = tri_out2->colors[1];
        tri_out2->colors[1] = temp_color;

        ae_assert_tri_is_valid(*tri_out1);
        ae_assert_tri_is_valid(*tri_out2);

        return 2;
    } else if (inside_points_count == 2 && outside_points_count == 1 && d1 < epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);
        
        *tri_out1 = tri_in;
        // tri_out1->colors[0] = 0x00FF00;
        // tri_out1->colors[1] = 0x00FF00;
        // tri_out1->colors[2] = 0x00FF00;

        *tri_out2 = tri_in;
        // tri_out2->colors[0] = 0x0000FF;
        // tri_out2->colors[1] = 0x0000FF;
        // tri_out2->colors[2] = 0x0000FF;

        (*tri_out1).points[0] = inside_points[0];
        (*tri_out1).tex_points[0] = tex_inside_points[0];
        (*tri_out1).points[1] = inside_points[1];
        (*tri_out1).tex_points[1] = tex_inside_points[1];
        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        (*tri_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*tri_out1).points[2].w = t * (outside_points[0].w - inside_points[0].w) + inside_points[0].w;
        (*tri_out1).tex_points[2].x = t * (tex_outside_points[0].x - tex_inside_points[0].x) + tex_inside_points[0].x;
        (*tri_out1).tex_points[2].y = t * (tex_outside_points[0].y - tex_inside_points[0].y) + tex_inside_points[0].y;

        (*tri_out2).points[0] = inside_points[1];
        (*tri_out2).tex_points[0] = tex_inside_points[1];
        ae_point_to_mat2D(inside_points[1], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        (*tri_out2).points[1] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*tri_out2).points[1].w = t * (outside_points[0].w - inside_points[1].w) + inside_points[1].w;
        (*tri_out2).tex_points[1].x = t * (tex_outside_points[0].x - tex_inside_points[1].x) + tex_inside_points[1].x;
        (*tri_out2).tex_points[1].y = t * (tex_outside_points[0].y - tex_inside_points[1].y) + tex_inside_points[1].y;
        (*tri_out2).points[2] = (*tri_out1).points[2];
        (*tri_out2).tex_points[2] = (*tri_out1).tex_points[2];

        mat2D_free(line_start);
        mat2D_free(line_end);
        
        /* fixing color ordering */
        uint32_t temp_color = tri_out1->colors[2]; 
        tri_out1->colors[2] = tri_out1->colors[1];
        tri_out1->colors[1] = temp_color;
        
        temp_color = tri_out2->colors[2]; 
        tri_out2->colors[2] = tri_out2->colors[0];
        tri_out2->colors[0] = tri_out2->colors[1];
        tri_out2->colors[1] = temp_color;
        temp_color = tri_out2->colors[1]; 
        tri_out2->colors[1] = tri_out2->colors[0];
        tri_out2->colors[0] = temp_color;

        ae_assert_tri_is_valid(*tri_out1);
        ae_assert_tri_is_valid(*tri_out2);

        return 2;
    } else if (inside_points_count == 2 && outside_points_count == 1 && d0 < epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);
        
        *tri_out1 = tri_in;
        // tri_out1->colors[0] = 0x00FF00;
        // tri_out1->colors[1] = 0x00FF00;
        // tri_out1->colors[2] = 0x00FF00;

        *tri_out2 = tri_in;
        // tri_out2->colors[0] = 0x0000FF;
        // tri_out2->colors[1] = 0x0000FF;
        // tri_out2->colors[2] = 0x0000FF;

        (*tri_out1).points[0] = inside_points[0];
        (*tri_out1).tex_points[0] = tex_inside_points[0];
        (*tri_out1).points[1] = inside_points[1];
        (*tri_out1).tex_points[1] = tex_inside_points[1];
        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        (*tri_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*tri_out1).points[2].w = t * (outside_points[0].w - inside_points[0].w) + inside_points[0].w;
        (*tri_out1).tex_points[2].x = t * (tex_outside_points[0].x - tex_inside_points[0].x) + tex_inside_points[0].x;
        (*tri_out1).tex_points[2].y = t * (tex_outside_points[0].y - tex_inside_points[0].y) + tex_inside_points[0].y;

        (*tri_out2).points[0] = inside_points[1];
        (*tri_out2).tex_points[0] = tex_inside_points[1];
        ae_point_to_mat2D(inside_points[1], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        (*tri_out2).points[1] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*tri_out2).points[1].w = t * (outside_points[0].w - inside_points[1].w) + inside_points[1].w;
        (*tri_out2).tex_points[1].x = t * (tex_outside_points[0].x - tex_inside_points[1].x) + tex_inside_points[1].x;
        (*tri_out2).tex_points[1].y = t * (tex_outside_points[0].y - tex_inside_points[1].y) + tex_inside_points[1].y;
        (*tri_out2).points[2] = (*tri_out1).points[2];
        (*tri_out2).tex_points[2] = (*tri_out1).tex_points[2];

        mat2D_free(line_start);
        mat2D_free(line_end);

        /* fixing color ordering */
        uint32_t temp_color = tri_out1->colors[2]; 
        tri_out1->colors[2] = tri_out1->colors[0];
        tri_out1->colors[0] = tri_out1->colors[1];
        tri_out1->colors[1] = temp_color;

        temp_color = tri_out2->colors[2]; 
        tri_out2->colors[2] = tri_out2->colors[1];
        tri_out2->colors[1] = tri_out2->colors[0];
        tri_out2->colors[0] = temp_color;

        ae_assert_tri_is_valid(*tri_out1);
        ae_assert_tri_is_valid(*tri_out2);

        return 2;
    }
    return -1;
}

/* returns number of inside quads
return -1 on error */
int ae_quad_clip_with_plane(Quad quad_in, Mat2D plane_p, Mat2D plane_n, Quad *quad_out1, Quad *quad_out2)
{
    ae_assert_quad_is_valid(quad_in);

    mat2D_normalize(plane_n);

    /* if the signed distance is positive, the point lies on the "inside" of the plane */
    Point inside_points[4];
    Point outside_points[4];
    int inside_points_count = 0;
    int outside_points_count = 0;
    
    /* calc signed distance of each point of tri_in */
    float d0 = ae_signed_dist_point_and_plane(quad_in.points[0], plane_p, plane_n);
    float d1 = ae_signed_dist_point_and_plane(quad_in.points[1], plane_p, plane_n);
    float d2 = ae_signed_dist_point_and_plane(quad_in.points[2], plane_p, plane_n);
    float d3 = ae_signed_dist_point_and_plane(quad_in.points[3], plane_p, plane_n);
    float t;

    // float epsilon = 1e-3;
    float epsilon = 0;
    if (d0 >= epsilon) {
        inside_points[inside_points_count++] = quad_in.points[0];
    } else {
        outside_points[outside_points_count++] = quad_in.points[0];
    }
    if (d1 >= epsilon) {
        inside_points[inside_points_count++] = quad_in.points[1];
    } else {
        outside_points[outside_points_count++] = quad_in.points[1];
    }
    if (d2 >= epsilon) {
        inside_points[inside_points_count++] = quad_in.points[2];
    } else {
        outside_points[outside_points_count++] = quad_in.points[2];
    }
    if (d3 >= epsilon) {
        inside_points[inside_points_count++] = quad_in.points[3];
    } else {
        outside_points[outside_points_count++] = quad_in.points[3];
    }

    /* classifying the triangle points */
    if (inside_points_count == 0) {
        return 0;
    } else if (inside_points_count == 4) {
        *quad_out1 = quad_in;
        return 1;
    } else if (inside_points_count == 1 && outside_points_count == 3 && d1 >= epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);

        *quad_out1 = quad_in;        
        *quad_out2 = quad_in;        

        (*quad_out1).points[1] = quad_in.points[1];

        ae_point_to_mat2D(quad_in.points[1], line_start);
        ae_point_to_mat2D(quad_in.points[2], line_end);
        (*quad_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[2].w = t * (quad_in.points[2].w - quad_in.points[1].w) + quad_in.points[1].w;
        (*quad_out1).colors[2] = quad_in.colors[2];

        ae_point_to_mat2D(quad_in.points[1], line_start);
        ae_point_to_mat2D(quad_in.points[0], line_end);
        (*quad_out1).points[0] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[0].w = t * (quad_in.points[0].w - quad_in.points[1].w) + quad_in.points[1].w;
        (*quad_out1).colors[0] = quad_in.colors[0];

        (*quad_out1).points[3].x = ((*quad_out1).points[0].x + (*quad_out1).points[2].x) / 2;
        (*quad_out1).points[3].y = ((*quad_out1).points[0].y + (*quad_out1).points[2].y) / 2;
        (*quad_out1).points[3].z = ((*quad_out1).points[0].z + (*quad_out1).points[2].z) / 2;
        (*quad_out1).points[3].w = ((*quad_out1).points[0].w + (*quad_out1).points[2].w) / 2;
        (*quad_out1).colors[3] = quad_in.colors[3];

        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_quad_is_valid(*quad_out1);

        return 1;
    } else if (inside_points_count == 1 && outside_points_count == 3 && d2 >= epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);

        *quad_out1 = quad_in;        
        *quad_out2 = quad_in;        

        (*quad_out1).points[2] = quad_in.points[2];

        ae_point_to_mat2D(quad_in.points[2], line_start);
        ae_point_to_mat2D(quad_in.points[3], line_end);
        (*quad_out1).points[3] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[3].w = t * (quad_in.points[3].w - quad_in.points[2].w) + quad_in.points[2].w;
        (*quad_out1).colors[3] = quad_in.colors[3];

        ae_point_to_mat2D(quad_in.points[2], line_start);
        ae_point_to_mat2D(quad_in.points[1], line_end);
        (*quad_out1).points[1] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[1].w = t * (quad_in.points[1].w - quad_in.points[2].w) + quad_in.points[2].w;
        (*quad_out1).colors[1] = quad_in.colors[1];

        (*quad_out1).points[0].x = ((*quad_out1).points[3].x + (*quad_out1).points[1].x) / 2;
        (*quad_out1).points[0].y = ((*quad_out1).points[3].y + (*quad_out1).points[1].y) / 2;
        (*quad_out1).points[0].z = ((*quad_out1).points[3].z + (*quad_out1).points[1].z) / 2;
        (*quad_out1).points[0].w = ((*quad_out1).points[3].w + (*quad_out1).points[1].w) / 2;
        (*quad_out1).colors[0] = quad_in.colors[0];

        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_quad_is_valid(*quad_out1);

        return 1;
    } else if (inside_points_count == 1 && outside_points_count == 3 && d3 >= epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);

        *quad_out1 = quad_in;        
        *quad_out2 = quad_in;        

        (*quad_out1).points[3] = quad_in.points[3];

        ae_point_to_mat2D(quad_in.points[3], line_start);
        ae_point_to_mat2D(quad_in.points[0], line_end);
        (*quad_out1).points[0] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[0].w = t * (quad_in.points[0].w - quad_in.points[3].w) + quad_in.points[3].w;
        (*quad_out1).colors[0] = quad_in.colors[0];

        ae_point_to_mat2D(quad_in.points[3], line_start);
        ae_point_to_mat2D(quad_in.points[2], line_end);
        (*quad_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[2].w = t * (quad_in.points[2].w - quad_in.points[3].w) + quad_in.points[3].w;
        (*quad_out1).colors[2] = quad_in.colors[2];

        (*quad_out1).points[1].x = ((*quad_out1).points[2].x + (*quad_out1).points[0].x) / 2;
        (*quad_out1).points[1].y = ((*quad_out1).points[2].y + (*quad_out1).points[0].y) / 2;
        (*quad_out1).points[1].z = ((*quad_out1).points[2].z + (*quad_out1).points[0].z) / 2;
        (*quad_out1).points[1].w = ((*quad_out1).points[2].w + (*quad_out1).points[0].w) / 2;
        (*quad_out1).colors[1] = quad_in.colors[1];

        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_quad_is_valid(*quad_out1);

        return 1;
    } else if (inside_points_count == 1 && outside_points_count == 3) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);

        *quad_out1 = quad_in;        
        *quad_out2 = quad_in;        

        (*quad_out1).points[0] = inside_points[0];

        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        (*quad_out1).points[1] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[1].w = t * (outside_points[0].w - inside_points[0].w) + inside_points[0].w;

        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[1], line_end);
        (*quad_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[2].w = t * (outside_points[1].w - inside_points[0].w) + inside_points[0].w;

        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[2], line_end);
        (*quad_out1).points[3] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[3].w = t * (outside_points[2].w - inside_points[0].w) + inside_points[0].w;

        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_quad_is_valid(*quad_out1);

        return 1;
    } else if (inside_points_count == 2 && outside_points_count == 2 && d2 < epsilon && d1 < epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);
        
        *quad_out1 = quad_in;

        (*quad_out1).points[0] = quad_in.points[3];
        (*quad_out1).colors[0] = quad_in.colors[3];
        (*quad_out1).points[1] = quad_in.points[0];
        (*quad_out1).colors[1] = quad_in.colors[0];

        ae_point_to_mat2D(quad_in.points[0], line_start);
        ae_point_to_mat2D(quad_in.points[1], line_end);
        (*quad_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[2].w = t * (quad_in.points[1].w - quad_in.points[0].w) + quad_in.points[0].w;
        (*quad_out1).colors[2] = quad_in.colors[1];

        ae_point_to_mat2D(quad_in.points[3], line_start);
        ae_point_to_mat2D(quad_in.points[2], line_end);
        (*quad_out1).points[3] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[3].w = t * (quad_in.points[2].w - quad_in.points[3].w) + quad_in.points[3].w;
        (*quad_out1).colors[3] = quad_in.colors[2];

        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_quad_is_valid(*quad_out1);

        return 2;
    } else if (inside_points_count == 2 && outside_points_count == 2 && d0 < epsilon && d1 < epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);
        
        *quad_out1 = quad_in;

        (*quad_out1).points[0] = quad_in.points[2];
        (*quad_out1).colors[0] = quad_in.colors[2];
        (*quad_out1).points[1] = quad_in.points[3];
        (*quad_out1).colors[1] = quad_in.colors[3];

        ae_point_to_mat2D(quad_in.points[2], line_start);
        ae_point_to_mat2D(quad_in.points[1], line_end);
        (*quad_out1).points[3] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[3].w = t * (quad_in.points[1].w - quad_in.points[2].w) + quad_in.points[2].w;
        (*quad_out1).colors[3] = quad_in.colors[1];

        ae_point_to_mat2D(quad_in.points[3], line_start);
        ae_point_to_mat2D(quad_in.points[0], line_end);
        (*quad_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[2].w = t * (quad_in.points[0].w - quad_in.points[3].w) + quad_in.points[3].w;
        (*quad_out1).colors[2] = quad_in.colors[0];

        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_quad_is_valid(*quad_out1);

        return 2;
    } else if (inside_points_count == 2 && outside_points_count == 2 && d0 < epsilon && d3 < epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);
        
        *quad_out1 = quad_in;

        (*quad_out1).points[0] = quad_in.points[1];
        (*quad_out1).colors[0] = quad_in.colors[1];
        (*quad_out1).points[1] = quad_in.points[2];
        (*quad_out1).colors[1] = quad_in.colors[2];

        ae_point_to_mat2D(quad_in.points[2], line_start);
        ae_point_to_mat2D(quad_in.points[3], line_end);
        (*quad_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[2].w = t * (quad_in.points[3].w - quad_in.points[2].w) + quad_in.points[2].w;
        (*quad_out1).colors[2] = quad_in.colors[3];

        ae_point_to_mat2D(quad_in.points[1], line_start);
        ae_point_to_mat2D(quad_in.points[0], line_end);
        (*quad_out1).points[3] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[3].w = t * (quad_in.points[1].w - quad_in.points[3].w) + quad_in.points[3].w;
        (*quad_out1).colors[3] = quad_in.colors[0];

        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_quad_is_valid(*quad_out1);

        return 1;
    } else if (inside_points_count == 2 && outside_points_count == 2) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);
        
        *quad_out1 = quad_in;

        (*quad_out1).points[0] = inside_points[0];
        (*quad_out1).points[1] = inside_points[1];
        ae_point_to_mat2D(inside_points[1], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        (*quad_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[2].w = t * (outside_points[0].w - inside_points[1].w) + inside_points[1].w;

        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[1], line_end);
        (*quad_out1).points[3] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[3].w = t * (outside_points[1].w - inside_points[0].w) + inside_points[0].w;

        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_quad_is_valid(*quad_out1);

        return 1;
    } else if (inside_points_count == 3 && outside_points_count == 1 && d0 < epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);
        
        *quad_out1 = quad_in;
        *quad_out2 = quad_in;

        ae_point_to_mat2D(quad_in.points[3], line_start);
        ae_point_to_mat2D(quad_in.points[0], line_end);
        (*quad_out1).points[0] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[0].w = t * (quad_in.points[0].w - quad_in.points[3].w) + quad_in.points[3].w;
        // adl_interpolate_ARGBcolor_on_RGB((quad_in).colors[3], (quad_in).colors[0], t, &((*quad_out1).colors[0]));

        (*quad_out2).points[1] = quad_in.points[1];
        // (*quad_out2).colors[1] = quad_in.colors[1];

        ae_point_to_mat2D(quad_in.points[1], line_start);
        ae_point_to_mat2D(quad_in.points[0], line_end);
        (*quad_out2).points[0] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out2).points[0].w = t * (quad_in.points[0].w - quad_in.points[1].w) + quad_in.points[1].w;
        // adl_interpolate_ARGBcolor_on_RGB((quad_in).colors[1], (quad_in).colors[0], t, &((*quad_out2).colors[0]));

        (*quad_out2).points[2] = (*quad_out1).points[0];
        // (*quad_out2).colors[2] = (*quad_out1).colors[0];

        (*quad_out2).points[3].x = ((*quad_out2).points[2].x + (*quad_out2).points[0].x) / 2;
        (*quad_out2).points[3].y = ((*quad_out2).points[2].y + (*quad_out2).points[0].y) / 2;
        (*quad_out2).points[3].z = ((*quad_out2).points[2].z + (*quad_out2).points[0].z) / 2;
        (*quad_out2).points[3].w = ((*quad_out2).points[2].w + (*quad_out2).points[0].w) / 2;
        // adl_interpolate_ARGBcolor_on_RGB((*quad_out2).colors[2], (*quad_out2).colors[0], 0.5f, &((*quad_out2).colors[3]));


        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_quad_is_valid(*quad_out1);

        return 2;
    } else if (inside_points_count == 3 && outside_points_count == 1 && d1 < epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);
        
        *quad_out1 = quad_in;
        *quad_out2 = quad_in;

        (*quad_out1).points[0] = quad_in.points[0];
        (*quad_out1).colors[0] = quad_in.colors[0];
        (*quad_out1).points[2] = quad_in.points[2];
        (*quad_out1).colors[2] = quad_in.colors[2];
        (*quad_out1).points[3] = quad_in.points[3];
        (*quad_out1).colors[3] = quad_in.colors[3];

        ae_point_to_mat2D(quad_in.points[2], line_start);
        ae_point_to_mat2D(quad_in.points[1], line_end);
        (*quad_out1).points[1] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[1].w = t * (quad_in.points[1].w - quad_in.points[2].w) + quad_in.points[2].w;
        (*quad_out1).colors[1] = (*quad_out1).colors[1];
        // adl_interpolate_ARGBcolor_on_RGB((quad_in).colors[2], (quad_in).colors[1], t, &((*quad_out1).colors[1]));

        (*quad_out2).points[0] = quad_in.points[0];
        (*quad_out2).colors[0] = quad_in.colors[0];
        (*quad_out2).points[3] = (*quad_out1).points[1];
        (*quad_out2).colors[3] = (*quad_out1).colors[3];

        ae_point_to_mat2D(quad_in.points[0], line_start);
        ae_point_to_mat2D(quad_in.points[1], line_end);
        (*quad_out2).points[1] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out2).points[1].w = t * (quad_in.points[1].w - quad_in.points[0].w) + quad_in.points[0].w;
        (*quad_out2).colors[1] = quad_in.colors[1];
        // adl_interpolate_ARGBcolor_on_RGB((quad_in).colors[0], (quad_in).colors[1], t, &((*quad_out2).colors[1]));

        (*quad_out2).points[2].x = ((*quad_out2).points[1].x + (*quad_out2).points[3].x) / 2;
        (*quad_out2).points[2].y = ((*quad_out2).points[1].y + (*quad_out2).points[3].y) / 2;
        (*quad_out2).points[2].z = ((*quad_out2).points[1].z + (*quad_out2).points[3].z) / 2;
        (*quad_out2).points[2].w = ((*quad_out2).points[1].w + (*quad_out2).points[3].w) / 2;
        // adl_interpolate_ARGBcolor_on_RGB((*quad_out2).colors[1], (*quad_out2).colors[3], 0.5f, &((*quad_out2).colors[2]));


        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_quad_is_valid(*quad_out1);

        return 2;
    } else if (inside_points_count == 3 && outside_points_count == 1 && d2 < epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);
        
        *quad_out1 = quad_in;
        *quad_out2 = quad_in;

        (*quad_out1).points[0] = quad_in.points[0];
        (*quad_out1).colors[0] = quad_in.colors[0];
        (*quad_out1).points[1] = quad_in.points[1];
        (*quad_out1).colors[1] = quad_in.colors[1];
        (*quad_out1).points[3] = quad_in.points[3];
        (*quad_out1).colors[3] = quad_in.colors[3];

        ae_point_to_mat2D(quad_in.points[1], line_start);
        ae_point_to_mat2D(quad_in.points[2], line_end);
        (*quad_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[2].w = t * (quad_in.points[2].w - quad_in.points[1].w) + quad_in.points[1].w;
        (*quad_out1).colors[2] = (*quad_out1).colors[2];
        // adl_interpolate_ARGBcolor_on_RGB((quad_in).colors[2], (quad_in).colors[1], t, &((*quad_out1).colors[1]));

        (*quad_out2).points[3] = quad_in.points[3];
        (*quad_out2).colors[3] = quad_in.colors[3];
        (*quad_out2).points[0] = (*quad_out1).points[2];
        (*quad_out2).colors[0] = (*quad_out1).colors[0];

        ae_point_to_mat2D(quad_in.points[3], line_start);
        ae_point_to_mat2D(quad_in.points[2], line_end);
        (*quad_out2).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out2).points[2].w = t * (quad_in.points[2].w - quad_in.points[3].w) + quad_in.points[3].w;
        (*quad_out2).colors[2] = quad_in.colors[2];
        // adl_interpolate_ARGBcolor_on_RGB((quad_in).colors[0], (quad_in).colors[1], t, &((*quad_out2).colors[1]));

        (*quad_out2).points[1].x = ((*quad_out2).points[2].x + (*quad_out2).points[0].x) / 2;
        (*quad_out2).points[1].y = ((*quad_out2).points[2].y + (*quad_out2).points[0].y) / 2;
        (*quad_out2).points[1].z = ((*quad_out2).points[2].z + (*quad_out2).points[0].z) / 2;
        (*quad_out2).points[1].w = ((*quad_out2).points[2].w + (*quad_out2).points[0].w) / 2;
        // adl_interpolate_ARGBcolor_on_RGB((*quad_out2).colors[1], (*quad_out2).colors[3], 0.5f, &((*quad_out2).colors[2]));


        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_quad_is_valid(*quad_out1);

        return 2;
    } else if (inside_points_count == 3 && outside_points_count == 1 && d3 < epsilon) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);
        
        *quad_out1 = quad_in;
        *quad_out2 = quad_in;

        (*quad_out1).points[0] = quad_in.points[0];
        (*quad_out1).colors[0] = quad_in.colors[0];
        (*quad_out1).points[1] = quad_in.points[1];
        (*quad_out1).colors[1] = quad_in.colors[1];
        (*quad_out1).points[2] = quad_in.points[2];
        (*quad_out1).colors[2] = quad_in.colors[2];

        ae_point_to_mat2D(quad_in.points[0], line_start);
        ae_point_to_mat2D(quad_in.points[3], line_end);
        (*quad_out1).points[3] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[3].w = t * (quad_in.points[3].w - quad_in.points[0].w) + quad_in.points[0].w;
        (*quad_out1).colors[3] = (*quad_out1).colors[3];
        // adl_interpolate_ARGBcolor_on_RGB((quad_in).colors[2], (quad_in).colors[1], t, &((*quad_out1).colors[1]));

        (*quad_out2).points[2] = quad_in.points[2];
        (*quad_out2).colors[2] = quad_in.colors[2];
        (*quad_out2).points[1] = (*quad_out1).points[3];
        (*quad_out2).colors[1] = (*quad_out1).colors[1];

        ae_point_to_mat2D(quad_in.points[2], line_start);
        ae_point_to_mat2D(quad_in.points[3], line_end);
        (*quad_out2).points[3] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out2).points[3].w = t * (quad_in.points[3].w - quad_in.points[2].w) + quad_in.points[2].w;
        (*quad_out2).colors[3] = quad_in.colors[3];
        // adl_interpolate_ARGBcolor_on_RGB((quad_in).colors[0], (quad_in).colors[1], t, &((*quad_out2).colors[1]));

        (*quad_out2).points[0].x = ((*quad_out2).points[3].x + (*quad_out2).points[1].x) / 2;
        (*quad_out2).points[0].y = ((*quad_out2).points[3].y + (*quad_out2).points[1].y) / 2;
        (*quad_out2).points[0].z = ((*quad_out2).points[3].z + (*quad_out2).points[1].z) / 2;
        (*quad_out2).points[0].w = ((*quad_out2).points[3].w + (*quad_out2).points[1].w) / 2;
        // adl_interpolate_ARGBcolor_on_RGB((*quad_out2).colors[1], (*quad_out2).colors[3], 0.5f, &((*quad_out2).colors[2]));


        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_quad_is_valid(*quad_out1);

        return 2;
    } else if (inside_points_count == 3 && outside_points_count == 1) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);
        
        *quad_out1 = quad_in;

        (*quad_out1).points[0] = inside_points[0];
        (*quad_out1).points[1] = inside_points[1];
        (*quad_out1).points[2] = inside_points[2];
        ae_point_to_mat2D(inside_points[2], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        (*quad_out1).points[3] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end, &t);
        (*quad_out1).points[3].w = t * (outside_points[0].w - inside_points[2].w) + inside_points[2].w;

        mat2D_free(line_start);
        mat2D_free(line_end);

        ae_assert_quad_is_valid(*quad_out1);

        return 1;
    }
    return -1;
}

void ae_projection_mat_set(Mat2D proj_mat,float aspect_ratio, float FOV_deg, float z_near, float z_far)
{
    AE_ASSERT(4 == proj_mat.cols); 
    AE_ASSERT(4 == proj_mat.rows); 
    AE_ASSERT(FOV_deg && "FOV needs to bee bigger then zero");

    mat2D_fill(proj_mat, 0);

    float field_of_view = 1.0f / tanf(0.5f * FOV_deg * PI / 180);
    float z_normalization = z_far / (z_far - z_near);

    MAT2D_AT(proj_mat, 0, 0) = aspect_ratio * field_of_view;
    MAT2D_AT(proj_mat, 1, 1) = field_of_view;
    MAT2D_AT(proj_mat, 2, 2) = z_normalization;
    MAT2D_AT(proj_mat, 2, 3) = 1;
    MAT2D_AT(proj_mat, 3, 2) = - z_normalization * z_near;
}

void ae_view_mat_set(Mat2D view_mat, Camera camera, Mat2D up)
{
    Mat2D DCM = mat2D_alloc(3,3);
    Mat2D DCM_trans = mat2D_alloc(3,3);
    mat2D_set_DCM_zyx(DCM, camera.yaw_offset_deg, camera.pitch_offset_deg, camera.roll_offset_deg);
    mat2D_transpose(DCM_trans, DCM);

    Mat2D temp_vec = mat2D_alloc(3, 1);
    Mat2D camera_direction = mat2D_alloc(3, 1);

    /* rotating camera_direction */
    mat2D_dot(camera_direction, DCM_trans, camera.direction);

    /* calc new forward direction */
    Mat2D new_forward = mat2D_alloc(3, 1);
    mat2D_copy(new_forward, camera_direction);
    mat2D_mult(new_forward, 1.0 / mat2D_calc_norma(new_forward));

    /* calc new up direction */
    mat2D_copy(temp_vec, new_forward);
    mat2D_mult(temp_vec, mat2D_dot_product(up, new_forward));
    Mat2D new_up = mat2D_alloc(3, 1);
    mat2D_copy(new_up, up);
    mat2D_sub(new_up, temp_vec);
    mat2D_mult(new_up, 1.0 / mat2D_calc_norma(new_up));

    /* calc new right direction */
    Mat2D new_right = mat2D_alloc(3, 1);
    mat2D_cross(new_right, new_up, new_forward);
    mat2D_mult(new_right, 1.0 / mat2D_calc_norma(new_right));

    mat2D_copy(camera.camera_x, new_right);
    mat2D_copy(camera.camera_y, new_up);
    mat2D_copy(camera.camera_z, new_forward);

    /* adding offset to init_position */
    // mat2D_add(camera_pos, camera.offset_position);

    mat2D_copy(temp_vec, camera.camera_x);
    mat2D_mult(temp_vec, MAT2D_AT(camera.offset_position, 0, 0));
    mat2D_add(camera.current_position, temp_vec);
    mat2D_copy(temp_vec, camera.camera_y);
    mat2D_mult(temp_vec, MAT2D_AT(camera.offset_position, 1, 0));
    mat2D_add(camera.current_position, temp_vec);
    mat2D_copy(temp_vec, camera.camera_z);
    mat2D_mult(temp_vec, MAT2D_AT(camera.offset_position, 2, 0));
    mat2D_add(camera.current_position, temp_vec);

    mat2D_fill(camera.offset_position, 0);

    MAT2D_AT(view_mat, 0, 0) = MAT2D_AT(new_right, 0, 0);
    MAT2D_AT(view_mat, 0, 1) = MAT2D_AT(new_up, 0, 0);
    MAT2D_AT(view_mat, 0, 2) = MAT2D_AT(new_forward, 0, 0);
    MAT2D_AT(view_mat, 0, 3) = 0;
    MAT2D_AT(view_mat, 1, 0) = MAT2D_AT(new_right, 1, 0);
    MAT2D_AT(view_mat, 1, 1) = MAT2D_AT(new_up, 1, 0);
    MAT2D_AT(view_mat, 1, 2) = MAT2D_AT(new_forward, 1, 0);
    MAT2D_AT(view_mat, 1, 3) = 0;
    MAT2D_AT(view_mat, 2, 0) = MAT2D_AT(new_right, 2, 0);
    MAT2D_AT(view_mat, 2, 1) = MAT2D_AT(new_up, 2, 0);
    MAT2D_AT(view_mat, 2, 2) = MAT2D_AT(new_forward, 2, 0);
    MAT2D_AT(view_mat, 2, 3) = 0;
    MAT2D_AT(view_mat, 3, 0) = - mat2D_dot_product(camera.current_position, new_right);
    MAT2D_AT(view_mat, 3, 1) = - mat2D_dot_product(camera.current_position, new_up);
    MAT2D_AT(view_mat, 3, 2) = - mat2D_dot_product(camera.current_position, new_forward);
    MAT2D_AT(view_mat, 3, 3) = 1;


    mat2D_free(temp_vec);
    mat2D_free(new_forward);
    mat2D_free(new_up);
    mat2D_free(new_right);
    mat2D_free(DCM);
    mat2D_free(DCM_trans);
}

Point ae_point_project_world2screen(Mat2D view_mat, Mat2D proj_mat, Point src, int window_w, int window_h)
{
    Point view_point = ae_point_project_world2view(view_mat, src);
    Point screen_point = ae_point_project_view2screen(proj_mat, view_point, window_w, window_h);

    return screen_point;
}

Point ae_point_project_world2view(Mat2D view_mat, Point src)
{
    ae_assert_point_is_valid(src);

    Mat2D src_point_mat = mat2D_alloc(1,4);
    Mat2D des_point_mat = mat2D_alloc(1,4);

    Point des_point = {0};

    MAT2D_AT(src_point_mat, 0, 0) = src.x;
    MAT2D_AT(src_point_mat, 0, 1) = src.y;
    MAT2D_AT(src_point_mat, 0, 2) = src.z;
    MAT2D_AT(src_point_mat, 0, 3) = 1;

    mat2D_dot(des_point_mat, src_point_mat, view_mat);

    double w = MAT2D_AT(des_point_mat, 0, 3);
    AE_ASSERT(w == 1);
    des_point.x = MAT2D_AT(des_point_mat, 0, 0) / w;
    des_point.y = MAT2D_AT(des_point_mat, 0, 1) / w;
    des_point.z = MAT2D_AT(des_point_mat, 0, 2) / w;
    des_point.w = w;

    mat2D_free(src_point_mat);
    mat2D_free(des_point_mat);

    return des_point;

}

Point ae_point_project_view2screen(Mat2D proj_mat, Point src, int window_w, int window_h)
{
    ae_assert_point_is_valid(src);

    Mat2D src_point_mat = mat2D_alloc(1,4);
    Mat2D des_point_mat = mat2D_alloc(1,4);
    Point des;

    MAT2D_AT(src_point_mat, 0, 0) = src.x;
    MAT2D_AT(src_point_mat, 0, 1) = src.y;
    MAT2D_AT(src_point_mat, 0, 2) = src.z;
    MAT2D_AT(src_point_mat, 0, 3) = 1;

    mat2D_dot(des_point_mat, src_point_mat, proj_mat);

    double w = MAT2D_AT(des_point_mat, 0, 3);
    if (fabs(w) > 1e-3) {
        des.x = MAT2D_AT(des_point_mat, 0, 0) / w;
        des.y = MAT2D_AT(des_point_mat, 0, 1) / w;
        des.z = MAT2D_AT(des_point_mat, 0, 2) / w;
        des.w = w;
    } else {
        // des.x = MAT2D_AT(des_point_mat, 0, 0);
        // des.y = MAT2D_AT(des_point_mat, 0, 1);
        // des.z = MAT2D_AT(des_point_mat, 0, 2);
        // des.w = 1;

        des.x = 0;
        des.y = 0;
        des.z = 0;
        des.w = 1;
    }

    mat2D_free(src_point_mat);
    mat2D_free(des_point_mat);


    /* scale into view */
    des.x += 1;
    des.y += 1;

    des.x *= 0.5f * window_w;
    des.y *= 0.5f * window_h;

    return des;
}

void ae_line_project_world2screen(Mat2D view_mat, Mat2D proj_mat, Point start_src, Point end_src, int window_w, int window_h, Point *start_des, Point *end_des, Scene *scene)
{
    Point start_view_point = ae_point_project_world2view(view_mat, start_src);
    Point end_view_point = ae_point_project_world2view(view_mat, end_src);

    Mat2D z_p = mat2D_alloc(3, 1);
    Mat2D z_n = mat2D_alloc(3, 1);
    mat2D_fill(z_p, 0);
    mat2D_fill(z_n, 0);
    MAT2D_AT(z_p, 2, 0) = scene->camera.z_near+0.01;
    MAT2D_AT(z_n, 2, 0) = 1;

    Point clipped_start_view_point = {0}, clipped_end_view_point = {0};
    int rc = ae_line_clip_with_plane(start_view_point, end_view_point, z_p, z_n, &clipped_start_view_point, &clipped_end_view_point);           

    if (rc == -1) {
        fprintf(stderr, "%s:%d: [error] problem with clipping lines\n", __FILE__, __LINE__);
        exit(1);
    } else if (rc == 0) {
        clipped_start_view_point = (Point){-1,-1,1,1};
        clipped_end_view_point = (Point){-1,-1,1,1};
        start_view_point = clipped_start_view_point;
        end_view_point = clipped_end_view_point;
        
        *start_des = start_view_point;
        *end_des = end_view_point;
        return;

    } else if (rc == 1) {
        start_view_point = clipped_start_view_point;
        end_view_point = clipped_end_view_point;
    }


    Point start_screen_point = ae_point_project_view2screen(proj_mat, start_view_point, window_w, window_h);
    Point end_screen_point = ae_point_project_view2screen(proj_mat, end_view_point, window_w, window_h);

    mat2D_free(z_p);
    mat2D_free(z_n);

    *start_des = start_screen_point;
    *end_des = end_screen_point;

}

Tri ae_tri_transform_to_view(Mat2D view_mat, Tri tri)
{
    ae_assert_tri_is_valid(tri);

    Mat2D src_point_mat = mat2D_alloc(1,4);
    Mat2D des_point_mat = mat2D_alloc(1,4);

    Tri des_tri = tri;

    for (int i = 0; i < 3; i++) {
        MAT2D_AT(src_point_mat, 0, 0) = tri.points[i].x;
        MAT2D_AT(src_point_mat, 0, 1) = tri.points[i].y;
        MAT2D_AT(src_point_mat, 0, 2) = tri.points[i].z;
        MAT2D_AT(src_point_mat, 0, 3) = 1;

        mat2D_dot(des_point_mat, src_point_mat, view_mat);

        double w = MAT2D_AT(des_point_mat, 0, 3);
        AE_ASSERT(w == 1);
        des_tri.points[i].x = MAT2D_AT(des_point_mat, 0, 0) / w;
        des_tri.points[i].y = MAT2D_AT(des_point_mat, 0, 1) / w;
        des_tri.points[i].z = MAT2D_AT(des_point_mat, 0, 2) / w;
        des_tri.points[i].w = w;
    }

    mat2D_free(src_point_mat);
    mat2D_free(des_point_mat);

    ae_assert_tri_is_valid(des_tri);

    return des_tri;
}

Tri_mesh ae_tri_project_world2screen(Mat2D proj_mat, Mat2D view_mat, Tri tri, int window_w, int window_h, Scene *scene, Lighting_mode lighting_mode)
{
    ae_assert_tri_is_valid(tri);

    Mat2D tri_normal = mat2D_alloc(3, 1);
    Mat2D temp_camera2tri = mat2D_alloc(3, 1);
    Mat2D camera2tri = mat2D_alloc(1, 3);
    Mat2D dot_product = mat2D_alloc(1, 1);
    Tri des_tri = tri;

    ae_point_to_mat2D(tri.points[0], temp_camera2tri);
    mat2D_sub(temp_camera2tri, scene->camera.current_position);
    mat2D_transpose(camera2tri, temp_camera2tri);

    /* calc lighting intensity of tri */
    #if 1
        ae_tri_calc_light_intensity(&des_tri, scene, lighting_mode);
    #else
    for (int i = 0; i < 3; i++) {
        ae_point_to_mat2D(tri.normals[i], tri_normal);
        MAT2D_AT(dot_product, 0, 0) = MAT2D_AT(light_direction, 0, 0) * MAT2D_AT(tri_normal, 0, 0) + MAT2D_AT(light_direction, 1, 0) * MAT2D_AT(tri_normal, 1, 0) + MAT2D_AT(light_direction, 2, 0) * MAT2D_AT(tri_normal, 2, 0);
        des_tri.light_intensity[i] = fmaxf(0.2, fminf(1, MAT2D_AT(dot_product, 0, 0)));
    }
    #endif

    /* calc if tri is visible to the camera */
    ae_tri_calc_normal(tri_normal, tri);
    // ae_point_to_mat2D(tri.normals[0], tri_normal);
    MAT2D_AT(dot_product, 0, 0) = MAT2D_AT(camera2tri, 0, 0) * MAT2D_AT(tri_normal, 0, 0) + MAT2D_AT(camera2tri, 0, 1) * MAT2D_AT(tri_normal, 1, 0) + MAT2D_AT(camera2tri, 0, 2) * MAT2D_AT(tri_normal, 2, 0);
    if (MAT2D_AT(dot_product, 0, 0) < 0) {
        des_tri.to_draw = true;
    } else {
        des_tri.to_draw = false;
    }

    /* transform tri to camera view */
    tri = ae_tri_transform_to_view(view_mat, tri);

    // Tri_mesh temp_tri_array; 
    // ada_init_array(Tri, temp_tri_array);
    // ada_appand(Tri, temp_tri_array, tri);
    /* clip tir */
    Tri clipped_tri1 = {0};
    Tri clipped_tri2 = {0};
    Mat2D z_plane_p = mat2D_alloc(3, 1);
    Mat2D z_plane_n = mat2D_alloc(3, 1);
    mat2D_fill(z_plane_p, 0);
    mat2D_fill(z_plane_n, 0);
    MAT2D_AT(z_plane_p, 2, 0) = scene->camera.z_near+0.01;
    MAT2D_AT(z_plane_n, 2, 0) = 1;

    int num_clipped_tri = ae_tri_clip_with_plane(tri, z_plane_p, z_plane_n, &clipped_tri1, &clipped_tri2);
    Tri_mesh temp_tri_array; 
    ada_init_array(Tri, temp_tri_array);
    if (num_clipped_tri == -1) {
        fprintf(stderr, "%s:%d: [error] problem with clipping triangles\n", __FILE__, __LINE__);
        exit(1);
    } else if (num_clipped_tri == 0) {
        ;
    } else if (num_clipped_tri == 1) {
        ae_assert_tri_is_valid(clipped_tri1);
        ada_appand(Tri, temp_tri_array, clipped_tri1);
    } else if (num_clipped_tri == 2) {
        ae_assert_tri_is_valid(clipped_tri1);
        ae_assert_tri_is_valid(clipped_tri2);
        ada_appand(Tri, temp_tri_array, clipped_tri1);
        ada_appand(Tri, temp_tri_array, clipped_tri2);
    }
    mat2D_free(z_plane_p);
    mat2D_free(z_plane_n);

    for (size_t temp_tri_index = 0; temp_tri_index < temp_tri_array.length; temp_tri_index++) {
        /* project tri to screen */
        for (int i = 0; i < 3; i++) {
            des_tri.points[i] = ae_point_project_view2screen(proj_mat, temp_tri_array.elements[temp_tri_index].points[i], window_w, window_h);

            if (des_tri.points[i].w) {
                des_tri.tex_points[i].x /= des_tri.points[i].w;
                des_tri.tex_points[i].y /= des_tri.points[i].w;
                des_tri.tex_points[i].z /= des_tri.points[i].w;
                des_tri.tex_points[i].w  = des_tri.points[i].w;
            }

        }
        ae_assert_tri_is_valid(des_tri);
        ae_tri_set_center_zmin_zmax(&des_tri);
        temp_tri_array.elements[temp_tri_index] = des_tri;
    }


    mat2D_free(tri_normal);
    mat2D_free(temp_camera2tri);
    mat2D_free(camera2tri);
    mat2D_free(dot_product);

    return temp_tri_array;
}

void ae_tri_mesh_project_world2screen(Mat2D proj_mat, Mat2D view_mat, Tri_mesh *des, Tri_mesh src, int window_w, int window_h, Scene *scene, Lighting_mode lighting_mode)
{
    Tri_mesh temp_des = *des;

    size_t i;
    for (i = 0; i < src.length; i++) {
        Tri_mesh temp_tri_array = ae_tri_project_world2screen(proj_mat, view_mat, src.elements[i], window_w, window_h, scene, lighting_mode);

        for (size_t tri_index = 0; tri_index < temp_tri_array.length; tri_index++) {
            Tri temp_tri = temp_tri_array.elements[tri_index];
            ada_appand(Tri, temp_des, temp_tri);
        }

        free(temp_tri_array.elements);
    }

    /* clip tir */
    int offset = 0;
    Mat2D top_p = mat2D_alloc(3, 1);
    Mat2D top_n = mat2D_alloc(3, 1);
    mat2D_fill(top_p, 0);
    mat2D_fill(top_n, 0);
    MAT2D_AT(top_p, 1, 0) = 0 + offset;
    MAT2D_AT(top_n, 1, 0) = 1;

    Mat2D bottom_p = mat2D_alloc(3, 1);
    Mat2D bottom_n = mat2D_alloc(3, 1);
    mat2D_fill(bottom_p, 0);
    mat2D_fill(bottom_n, 0);
    MAT2D_AT(bottom_p, 1, 0) = window_h - offset;
    MAT2D_AT(bottom_n, 1, 0) = -1;

    Mat2D left_p = mat2D_alloc(3, 1);
    Mat2D left_n = mat2D_alloc(3, 1);
    mat2D_fill(left_p, 0);
    mat2D_fill(left_n, 0);
    MAT2D_AT(left_p, 0, 0) = 0 + offset;
    MAT2D_AT(left_n, 0, 0) = 1;

    Mat2D right_p = mat2D_alloc(3, 1);
    Mat2D right_n = mat2D_alloc(3, 1);
    mat2D_fill(right_p, 0);
    mat2D_fill(right_n, 0);
    MAT2D_AT(right_p, 0, 0) = window_w - offset;
    MAT2D_AT(right_n, 0, 0) = -1;

    for (int plane_number = 0; plane_number < 4; plane_number++) {
        for (int tri_index = 0; tri_index < (int)(temp_des.length); tri_index++) {
            if (temp_des.length == 0) {
                break;
            }
            // if (temp_des.elements[tri_index].to_draw == false) {
            //     ada_remove_unordered(Tri, temp_des, tri_index);
            //     tri_index--;
            //     tri_index = (int)fmaxf((float)tri_index, 0.0f);
            //     continue;
            // }
            Tri clipped_tri1 = {0};
            Tri clipped_tri2 = {0};
            int num_clipped_tri;
            switch (plane_number) {
                case 0:
                    num_clipped_tri = ae_tri_clip_with_plane(temp_des.elements[tri_index], top_p, top_n, &clipped_tri1, &clipped_tri2);
                break;
                case 1:
                    num_clipped_tri = ae_tri_clip_with_plane(temp_des.elements[tri_index], right_p, right_n, &clipped_tri1, &clipped_tri2);
                break;
                case 2:
                    num_clipped_tri = ae_tri_clip_with_plane(temp_des.elements[tri_index], bottom_p, bottom_n, &clipped_tri1, &clipped_tri2);
                break;
                case 3:
                    num_clipped_tri = ae_tri_clip_with_plane(temp_des.elements[tri_index], left_p, left_n, &clipped_tri1, &clipped_tri2);
                break;
            }
            if (num_clipped_tri == -1) {
                fprintf(stderr, "%s:%d: [error] problem with clipping triangles\n", __FILE__, __LINE__);
                exit(1);
            } else if (num_clipped_tri == 0) {
                ada_remove_unordered(Tri, temp_des, tri_index);
                tri_index--;
                tri_index = (int)fmaxf((float)tri_index, 0.0f);
            } else if (num_clipped_tri == 1) {
                ae_assert_tri_is_valid(clipped_tri1);
                temp_des.elements[tri_index] = clipped_tri1;
            } else if (num_clipped_tri == 2) {
                ae_assert_tri_is_valid(clipped_tri1);
                ae_assert_tri_is_valid(clipped_tri2);
                temp_des.elements[tri_index] = clipped_tri1;
                ada_insert_unordered(Tri, temp_des, clipped_tri2, tri_index+1);
            }
        }
    }

    // if (temp_des.length > 2) {
    //     ae_qsort_tri(temp_des.elements, 0, temp_des.length-1);
    // }

    mat2D_free(top_p);
    mat2D_free(top_n);
    mat2D_free(bottom_p);
    mat2D_free(bottom_n);
    mat2D_free(left_p);
    mat2D_free(left_n);
    mat2D_free(right_p);
    mat2D_free(right_n);

    *des = temp_des;
}

Quad ae_quad_transform_to_view(Mat2D view_mat, Quad quad)
{
    ae_assert_quad_is_valid(quad);

    Mat2D src_point_mat = mat2D_alloc(1,4);
    Mat2D des_point_mat = mat2D_alloc(1,4);

    Quad des_quad = quad;

    for (int i = 0; i < 4; i++) {
        MAT2D_AT(src_point_mat, 0, 0) = quad.points[i].x;
        MAT2D_AT(src_point_mat, 0, 1) = quad.points[i].y;
        MAT2D_AT(src_point_mat, 0, 2) = quad.points[i].z;
        MAT2D_AT(src_point_mat, 0, 3) = 1;

        mat2D_dot(des_point_mat, src_point_mat, view_mat);

        double w = MAT2D_AT(des_point_mat, 0, 3);
        AE_ASSERT(w == 1);
        des_quad.points[i].x = MAT2D_AT(des_point_mat, 0, 0) / w;
        des_quad.points[i].y = MAT2D_AT(des_point_mat, 0, 1) / w;
        des_quad.points[i].z = MAT2D_AT(des_point_mat, 0, 2) / w;
        des_quad.points[i].w = w;
    }

    mat2D_free(src_point_mat);
    mat2D_free(des_point_mat);

    ae_assert_quad_is_valid(des_quad);

    return des_quad;
}

Quad_mesh ae_quad_project_world2screen(Mat2D proj_mat, Mat2D view_mat, Quad quad, int window_w, int window_h, Scene *scene, Lighting_mode lighting_mode)
{
    ae_assert_quad_is_valid(quad);

    Mat2D quad_normal = mat2D_alloc(3, 1);
    Mat2D camera2quad = mat2D_alloc(3, 1);
    Mat2D dot_product = mat2D_alloc(1, 1);
    Quad des_quad = quad;

    /* calc lighting intensity of tri */
    #if 1
        ae_quad_calc_light_intensity(&des_quad, scene, lighting_mode);
    #else
    for (int i = 0; i < 4; i++) {
        ae_point_to_mat2D(quad.normals[i], quad_normal);
        MAT2D_AT(dot_product, 0, 0) = scene->light_source0.light_direction_or_pos.x * MAT2D_AT(quad_normal, 0, 0) + scene->light_source0.light_direction_or_pos.y * MAT2D_AT(quad_normal, 1, 0) + scene->light_source0.light_direction_or_pos.z * MAT2D_AT(quad_normal, 2, 0);
        des_quad.light_intensity[i] = fmaxf(0.2, fminf(1, MAT2D_AT(dot_product, 0, 0)));
    }
    #endif

    /* calc if quad is visible to the camera */
    bool visible = 0;
    #if 1
    for (int i = 0; i < 4; i++) {
        ae_point_to_mat2D(quad.points[i], camera2quad);
        mat2D_sub(camera2quad, scene->camera.current_position);

        ae_point_to_mat2D(quad.normals[i], quad_normal);
        MAT2D_AT(dot_product, 0, 0) = MAT2D_AT(camera2quad, 0, 0) * MAT2D_AT(quad_normal, 0, 0) + MAT2D_AT(camera2quad, 1, 0) * MAT2D_AT(quad_normal, 1, 0) + MAT2D_AT(camera2quad, 2, 0) * MAT2D_AT(quad_normal, 2, 0);
        visible = visible || (MAT2D_AT(dot_product, 0, 0) < 0);
    }
    #else
    ae_point_to_mat2D(quad.points[0], camera2quad);
    mat2D_sub(camera2quad, scene->camera.current_position);

    Point ave_norm = ae_quad_get_average_normal(quad);
    ae_point_to_mat2D(ave_norm, quad_normal);
    MAT2D_AT(dot_product, 0, 0) = MAT2D_AT(camera2quad, 0, 0) * MAT2D_AT(quad_normal, 0, 0) + MAT2D_AT(camera2quad, 1, 0) * MAT2D_AT(quad_normal, 1, 0) + MAT2D_AT(camera2quad, 2, 0) * MAT2D_AT(quad_normal, 2, 0);
    visible = MAT2D_AT(dot_product, 0, 0) < 0;
    #endif

    if (visible) {
        des_quad.to_draw = true;
    } else {
        des_quad.to_draw = false;
    }

    /* transform quad to camera view */
    quad = ae_quad_transform_to_view(view_mat, quad);

    /* clip quad */
    Quad  clipped_quad1 = {0}, clipped_quad2 = {0};
    Mat2D z_plane_p = mat2D_alloc(3, 1);
    Mat2D z_plane_n = mat2D_alloc(3, 1);
    mat2D_fill(z_plane_p, 0);
    mat2D_fill(z_plane_n, 0);
    MAT2D_AT(z_plane_p, 2, 0) = scene->camera.z_near+0.01;
    MAT2D_AT(z_plane_n, 2, 0) = 1;

    int num_clipped_quad = ae_quad_clip_with_plane(quad, z_plane_p, z_plane_n, &clipped_quad1, &clipped_quad2);
    Quad_mesh temp_quad_array; 
    ada_init_array(Quad, temp_quad_array);
    if (num_clipped_quad == -1) {
        fprintf(stderr, "%s:%d: [error] problem with clipping quad\n", __FILE__, __LINE__);
        exit(1);
    } else if (num_clipped_quad == 0) {
        ;
    } else if (num_clipped_quad == 1) {
        ae_assert_quad_is_valid(clipped_quad1);
        ada_appand(Quad, temp_quad_array, clipped_quad1);
    } else if (num_clipped_quad == 2) {
        ae_assert_quad_is_valid(clipped_quad1);
        ae_assert_quad_is_valid(clipped_quad2);
        ada_appand(Quad, temp_quad_array, clipped_quad1);
        ada_appand(Quad, temp_quad_array, clipped_quad2);
    }
    mat2D_free(z_plane_p);
    mat2D_free(z_plane_n);

    for (size_t temp_quad_index = 0; temp_quad_index < temp_quad_array.length; temp_quad_index++) {
        /* project quad to screen */
        for (int i = 0; i < 4; i++) {
            des_quad.points[i] = ae_point_project_view2screen(proj_mat, temp_quad_array.elements[temp_quad_index].points[i], window_w, window_h);

        }
        ae_assert_quad_is_valid(des_quad);
        temp_quad_array.elements[temp_quad_index] = des_quad;
    }


    mat2D_free(quad_normal);
    mat2D_free(camera2quad);
    mat2D_free(dot_product);

    return temp_quad_array;
}

void ae_quad_mesh_project_world2screen(Mat2D proj_mat, Mat2D view_mat, Quad_mesh *des, Quad_mesh src, int window_w, int window_h, Scene *scene, Lighting_mode lighting_mode)
{
    Quad_mesh temp_des = *des;

    size_t i;
    for (i = 0; i < src.length; i++) {
        Quad_mesh temp_quad_array = ae_quad_project_world2screen(proj_mat, view_mat, src.elements[i], window_w, window_h, scene, lighting_mode);

        for (size_t quad_index = 0; quad_index < temp_quad_array.length; quad_index++) {
            Quad temp_quad = temp_quad_array.elements[quad_index];
            ada_appand(Quad, temp_des, temp_quad);
        }

        free(temp_quad_array.elements);
    }


    /* clip quad */
    int offset = 0;
    Mat2D top_p = mat2D_alloc(3, 1);
    Mat2D top_n = mat2D_alloc(3, 1);
    mat2D_fill(top_p, 0);
    mat2D_fill(top_n, 0);
    MAT2D_AT(top_p, 1, 0) = 0 + offset;
    MAT2D_AT(top_n, 1, 0) = 1;

    Mat2D bottom_p = mat2D_alloc(3, 1);
    Mat2D bottom_n = mat2D_alloc(3, 1);
    mat2D_fill(bottom_p, 0);
    mat2D_fill(bottom_n, 0);
    MAT2D_AT(bottom_p, 1, 0) = window_h - offset;
    MAT2D_AT(bottom_n, 1, 0) = -1;

    Mat2D left_p = mat2D_alloc(3, 1);
    Mat2D left_n = mat2D_alloc(3, 1);
    mat2D_fill(left_p, 0);
    mat2D_fill(left_n, 0);
    MAT2D_AT(left_p, 0, 0) = 0 + offset;
    MAT2D_AT(left_n, 0, 0) = 1;

    Mat2D right_p = mat2D_alloc(3, 1);
    Mat2D right_n = mat2D_alloc(3, 1);
    mat2D_fill(right_p, 0);
    mat2D_fill(right_n, 0);
    MAT2D_AT(right_p, 0, 0) = window_w - offset;
    MAT2D_AT(right_n, 0, 0) = -1;

    for (int plane_number = 0; plane_number < 4; plane_number++) {
        for (int quad_index = 0; quad_index < (int)(temp_des.length); quad_index++) {
            if (temp_des.length == 0) {
                break;
            }
            // if (temp_des.elements[quad_index].to_draw == false) {
            //     ada_remove_unordered(Quad, temp_des, quad_index);
            //     quad_index--;
            //     quad_index = (int)fmaxf((float)quad_index, 0.0f);
            //     continue;
            // }
            Quad clipped_quad1 = {0}, clipped_quad2 = {0};
            int num_clipped_quad;
            switch (plane_number) {
                case 0:
                    num_clipped_quad = ae_quad_clip_with_plane(temp_des.elements[quad_index], top_p, top_n, &clipped_quad1, &clipped_quad2);
                break;
                case 1:
                    num_clipped_quad = ae_quad_clip_with_plane(temp_des.elements[quad_index], right_p, right_n, &clipped_quad1, &clipped_quad2);
                break;
                case 2:
                    num_clipped_quad = ae_quad_clip_with_plane(temp_des.elements[quad_index], bottom_p, bottom_n, &clipped_quad1, &clipped_quad2);
                break;
                case 3:
                    num_clipped_quad = ae_quad_clip_with_plane(temp_des.elements[quad_index], left_p, left_n, &clipped_quad1, &clipped_quad2);
                break;
            }
            if (num_clipped_quad == -1) {
                fprintf(stderr, "%s:%d: [error] problem with clipping quads\n", __FILE__, __LINE__);
                exit(1);
            } else if (num_clipped_quad == 0) {
                ada_remove_unordered(Quad, temp_des, quad_index);
                quad_index--;
                quad_index = (int)fmaxf((float)quad_index, 0.0f);
            } else if (num_clipped_quad == 1) {
                ae_assert_quad_is_valid(clipped_quad1);
                temp_des.elements[quad_index] = clipped_quad1;
            } else if (num_clipped_quad == 2) {
                ae_assert_quad_is_valid(clipped_quad1);
                ae_assert_quad_is_valid(clipped_quad2);
                temp_des.elements[quad_index] = clipped_quad1;
                ada_insert_unordered(Quad, temp_des, clipped_quad2, quad_index+1);
                quad_index++;
            }
        }
    }

    mat2D_free(top_p);
    mat2D_free(top_n);
    mat2D_free(bottom_p);
    mat2D_free(bottom_n);
    mat2D_free(left_p);
    mat2D_free(left_n);
    mat2D_free(right_p);
    mat2D_free(right_n);

    *des = temp_des;
}

void ae_curve_project_world2screen(Mat2D proj_mat, Mat2D view_mat, Curve *des, Curve src, int window_w, int window_h, Scene *scene)
{
    ae_curve_copy(des, src);
    Curve temp_des = *des;
    /* set planes */
    int offset = 50;
    Mat2D top_p = mat2D_alloc(3, 1);
    Mat2D top_n = mat2D_alloc(3, 1);
    mat2D_fill(top_p, 0);
    mat2D_fill(top_n, 0);
    MAT2D_AT(top_p, 1, 0) = 0 + offset;
    MAT2D_AT(top_n, 1, 0) = 1;
    Mat2D bottom_p = mat2D_alloc(3, 1);
    Mat2D bottom_n = mat2D_alloc(3, 1);
    mat2D_fill(bottom_p, 0);
    mat2D_fill(bottom_n, 0);
    MAT2D_AT(bottom_p, 1, 0) = window_h - offset;
    MAT2D_AT(bottom_n, 1, 0) = -1;
    Mat2D left_p = mat2D_alloc(3, 1);
    Mat2D left_n = mat2D_alloc(3, 1);
    mat2D_fill(left_p, 0);
    mat2D_fill(left_n, 0);
    MAT2D_AT(left_p, 0, 0) = 0 + offset;
    MAT2D_AT(left_n, 0, 0) = 1;
    Mat2D right_p = mat2D_alloc(3, 1);
    Mat2D right_n = mat2D_alloc(3, 1);
    mat2D_fill(right_p, 0);
    mat2D_fill(right_n, 0);
    MAT2D_AT(right_p, 0, 0) = window_w - offset;
    MAT2D_AT(right_n, 0, 0) = -1;

    for (size_t point_index = 0; point_index < temp_des.length-1; point_index++) {
        Point start_src_point = src.elements[point_index];
        Point end_src_point = src.elements[point_index+1];

        Point start_des_point = {0}, end_des_point = {0};

        ae_line_project_world2screen(view_mat, proj_mat, start_src_point, end_src_point, window_w, window_h, &start_des_point, &end_des_point, scene);


        Point clipped_start_des_point = {0}, clipped_end_des_point = {0};

        for (int plane_number = 0; plane_number < 4; plane_number++) {
            int rc;
            switch (plane_number) {
                case 0:
                    rc = ae_line_clip_with_plane(start_des_point, end_des_point, top_p, top_n, &clipped_start_des_point, &clipped_end_des_point);           
                break;
                case 1:
                    rc = ae_line_clip_with_plane(start_des_point, end_des_point, right_p, right_n, &clipped_start_des_point, &clipped_end_des_point);           
                break;
                case 2:
                    rc = ae_line_clip_with_plane(start_des_point, end_des_point, bottom_p, bottom_n, &clipped_start_des_point, &clipped_end_des_point);           
                break;
                case 3:
                    rc = ae_line_clip_with_plane(start_des_point, end_des_point, left_p, left_n, &clipped_start_des_point, &clipped_end_des_point);           
                break;
            }
            if (rc == -1) {
                fprintf(stderr, "%s:%d: [error] problem with clipping lines\n", __FILE__, __LINE__);
                exit(1);
            } else if (rc == 0) {
                clipped_start_des_point = (Point){-1,-1,1,1};
                clipped_end_des_point = (Point){-1,-1,1,1};
                start_des_point = clipped_start_des_point;
                end_des_point = clipped_end_des_point;
                temp_des.elements[point_index] = start_des_point;
                temp_des.elements[point_index+1] = end_des_point;
            } else if (rc == 1) {
                start_des_point = clipped_start_des_point;
                end_des_point = clipped_end_des_point;
                temp_des.elements[point_index] = start_des_point;
                temp_des.elements[point_index+1] = end_des_point;
            }
        }

    }

    Point default_point = (Point){-1,-1,1,1};
    for (int i = 0; i < (int)temp_des.length; i++) {
        if (ae_points_equal(temp_des.elements[i], default_point)) {
            ada_remove(Point, temp_des, i);
            i--;
        }
    }

    *des = temp_des;

    mat2D_free(top_p);
    mat2D_free(top_n);
    mat2D_free(bottom_p);
    mat2D_free(bottom_n);
    mat2D_free(left_p);
    mat2D_free(left_n);
    mat2D_free(right_p);
    mat2D_free(right_n);
}

void ae_curve_ada_project_world2screen(Mat2D proj_mat, Mat2D view_mat, Curve_ada *des, Curve_ada src, int window_w, int window_h, Scene *scene)
{
    for (size_t curve_index = 0; curve_index < src.length; curve_index++) {
        ae_curve_project_world2screen(proj_mat, view_mat, &(des->elements[curve_index]), src.elements[curve_index], window_w, window_h, scene);
    }
}

void ae_grid_project_world2screen(Mat2D proj_mat, Mat2D view_mat, Grid des, Grid src, int window_w, int window_h, Scene *scene)
{
    for (size_t curve_index = 0; curve_index < src.curves.length; curve_index++) {
        ae_curve_project_world2screen(proj_mat, view_mat, &(des.curves.elements[curve_index]), src.curves.elements[curve_index], window_w, window_h, scene);
    }
}

/* swap: interchange v[i] and v[j] */
void ae_tri_swap(Tri *v, int i, int j)
{
    Tri temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

bool ae_tri_compare(Tri t1, Tri t2)
{
    // int z_min_1 = fmin(t1.points[0].z, fmin(t1.points[1].z, t1.points[2].z));
    // int z_min_2 = fmin(t2.points[0].z, fmin(t2.points[1].z, t2.points[2].z));

    // return z_min_1 > z_min_2;

    // int z_ave_1 = t1.points[0].z + t1.points[1].z + t1.points[2].z;
    // int z_ave_2 = t2.points[0].z + t2.points[1].z + t2.points[2].z;

    // return z_ave_1 > z_ave_2;

    // return t1.center.z > t2.center.z;

    // return t1.z_min > t2.z_min;

    return t1.z_max > t2.z_max;
}

/* qsort: sort v[left]...v[right] int increasing order */
void ae_tri_qsort(Tri *v, int left, int right)
{
    int i, last;

    if (left >= right)                  /* do nothing if array contains */
        return;                         /* fewer than two elements */
    ae_tri_swap(v, left, (left + right) / 2);  /* move partition elem */
    last = left;                        /* to v[0] */
    for (i = left + 1; i <= right; i++) /* partition */
        if (ae_tri_compare(v[i], v[left]))
            ae_tri_swap(v, ++last, i);
    ae_tri_swap(v, left, last); /* restore partition elem */
    ae_tri_qsort(v, left, last - 1);
    ae_tri_qsort(v, last + 1, right);
}

double ae_linear_map(double s, double min_in, double max_in, double min_out, double max_out)
{
    return (min_out + ((s-min_in)*(max_out-min_out))/(max_in-min_in));
}

void ae_z_buffer_copy_to_screen(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer)
{
    double max_inv_z = 0;
    double min_inv_z = DBL_MAX;
    for (size_t i = 0; i < inv_z_buffer.rows; i++) {
        for (size_t j = 0; j < inv_z_buffer.cols; j++) {
            if (MAT2D_AT(inv_z_buffer, i, j) > max_inv_z) {
                max_inv_z = MAT2D_AT(inv_z_buffer, i, j);
            }
            if (MAT2D_AT(inv_z_buffer, i, j) < min_inv_z && MAT2D_AT(inv_z_buffer, i, j) > 0) {
                min_inv_z = MAT2D_AT(inv_z_buffer, i, j);
            }
        }
    }
    for (size_t i = 0; i < inv_z_buffer.rows; i++) {
        for (size_t j = 0; j < inv_z_buffer.cols; j++) {
            double z_fraq = MAT2D_AT(inv_z_buffer, i, j);
            z_fraq = fmax(z_fraq, min_inv_z);
            z_fraq = ae_linear_map(z_fraq, min_inv_z, max_inv_z, 0.1, 1);
            uint32_t color = RGB_hexRGB(0xFF*z_fraq, 0xFF*z_fraq, 0xFF*z_fraq); 
            MAT2D_AT_UINT32(screen_mat, i, j) = color;
        }
    }
}

#endif /* ALMOG_ENGINE_IMPLEMENTATION */ //