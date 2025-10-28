/**
 * @file
 * @brief Immediate-mode 2D/3D raster helpers for drawing onto
 *        Mat2D_uint32 pixel buffers.
 *
 * Conventions
 * - Pixel buffer: Mat2D_uint32 with elements encoded as ARGB 0xAARRGGBB.
 * - Coordinates: x grows to the right, y grows downward; origin is the
 *   top-left corner of the destination buffer.
 * - Depth: Functions that accept inv_z_buffer perform a depth test using
 *   inverse-Z (larger values are closer). The buffer stores doubles.
 * - Transform: Most drawing functions accept an Offset_zoom_param
 *   describing a pan/zoom transform that is applied about the screen
 *   center. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 * - Colors: Unless noted otherwise, colors are ARGB in 0xAARRGGBB format.
 * - Alpha: adl_point_draw alpha-blends source over destination and writes
 *   an opaque result (A = 255) to the pixel buffer.
 *
 * This header contains function declarations and optional implementations
 * (guarded by ALMOG_DRAW_LIBRARY_IMPLEMENTATION).
 */

#ifndef ALMOG_DRAW_LIBRARY_H_
#define ALMOG_DRAW_LIBRARY_H_

#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <float.h>

#include "./Matrix2D.h"
#include "./Almog_Dynamic_Array.h"

#ifndef ADL_ASSERT
#include <assert.h>
#define ADL_ASSERT assert
#endif

typedef struct {
    float zoom_multiplier;
    float offset_x;
    float offset_y;
    int mouse_x;
    int mouse_y;
} Offset_zoom_param;

#ifndef POINT
#define POINT
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Point ;
#endif

#ifndef CURVE
#define CURVE
typedef struct {
    uint32_t color;
    size_t length;
    size_t capacity;
    Point *elements;
} Curve;
#endif

#ifndef CURVE_ADA
#define CURVE_ADA
typedef struct {
    size_t length;
    size_t capacity;
    Curve *elements;
} Curve_ada;
#endif

#ifndef TRI
#define TRI
typedef struct {
    Point points[3];
    Point tex_points[3];
    Point normals[3];
    uint32_t colors[3];
    bool to_draw;
    float light_intensity[3];
} Tri;
#endif

#ifndef QUAD
#define QUAD
typedef struct {
    Point points[4];
    Point normals[4];
    uint32_t colors[4];
    bool to_draw;
    float light_intensity[4];
} Quad;
#endif

#ifndef TRI_MESH
#define TRI_MESH
typedef struct {
    size_t length;
    size_t capacity;
    Tri *elements;
} Tri_mesh; /* Tri ada array */
#endif

#ifndef QUAD_MESH
#define QUAD_MESH
typedef struct {
    size_t length;
    size_t capacity;
    Quad *elements;
} Quad_mesh; /* Quad ada array */
#endif

typedef struct {
    int min_x_pixel;
    int max_x_pixel;
    int min_y_pixel;
    int max_y_pixel;
    float min_x;
    float max_x;
    float min_y;
    float max_y;
    int x_axis_head_size;
    int y_axis_head_size;
    Offset_zoom_param offset_zoom_param;
    Curve_ada src_curve_array;
    Point top_left_position;
    Mat2D_uint32 pixels_mat;
    Mat2D inv_z_buffer_mat;
    uint32_t background_color;
    bool to_draw_axis;
    bool to_draw_max_min_values;
} Figure;

typedef struct {
    Curve_ada curves;
    float min_e1;
    float max_e1;
    float min_e2;
    float max_e2;
    int num_samples_e1;
    int num_samples_e2;
    float de1;
    float de2;
    char plane[3];
} Grid; /* direction: e1, e2 */


#ifndef HexARGB_RGBA
#define HexARGB_RGBA(x) ((x)>>(8*2)&0xFF), ((x)>>(8*1)&0xFF), ((x)>>(8*0)&0xFF), ((x)>>(8*3)&0xFF)
#endif
#ifndef HexARGB_RGB_VAR
#define HexARGB_RGB_VAR(x, r, g, b) r = ((x)>>(8*2)&0xFF); g = ((x)>>(8*1)&0xFF); b = ((x)>>(8*0)&0xFF);
#endif
#ifndef HexARGB_RGBA_VAR
#define HexARGB_RGBA_VAR(x, r, g, b, a) r = ((x)>>(8*2)&0xFF); g = ((x)>>(8*1)&0xFF); b = ((x)>>(8*0)&0xFF); a = ((x)>>(8*3)&0xFF)
#endif
#ifndef RGB_hexRGB
#define RGB_hexRGB(r, g, b) (int)(0x010000*(int)(r) + 0x000100*(int)(g) + 0x000001*(int)(b))
#endif
#ifndef RGBA_hexARGB
#define RGBA_hexARGB(r, g, b, a) (int)(0x01000000l*(int)(fminf(a, 255)) + 0x010000*(int)(r) + 0x000100*(int)(g) + 0x000001*(int)(b))
#endif


void    adl_point_draw(Mat2D_uint32 screen_mat, int x, int y, uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_line_draw(Mat2D_uint32 screen_mat, const float x1_input, const float y1_input, const float x2_input, const float y2_input, uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_lines_draw(const Mat2D_uint32 screen_mat, const Point *points, const size_t len, const uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_lines_loop_draw(const Mat2D_uint32 screen_mat, const Point *points, const size_t len, const uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_arrow_draw(Mat2D_uint32 screen_mat, int xs, int ys, int xe, int ye, float head_size, float angle_deg, uint32_t color, Offset_zoom_param offset_zoom_param);

void    adl_character_draw(Mat2D_uint32 screen_mat, char c, int width_pixel, int hight_pixel, int x_top_left, int y_top_left, uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_sentence_draw(Mat2D_uint32 screen_mat, const char sentence[], size_t len, const int x_top_left, const int y_top_left, const int hight_pixel, const uint32_t color, Offset_zoom_param offset_zoom_param);

void    adl_rectangle_draw_min_max(Mat2D_uint32 screen_mat, int min_x, int max_x, int min_y, int max_y, uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_rectangle_fill_min_max(Mat2D_uint32 screen_mat, int min_x, int max_x, int min_y, int max_y, uint32_t color, Offset_zoom_param offset_zoom_param);

void    adl_quad_draw(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Quad quad, uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_quad_fill(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Quad quad, uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_quad_fill_interpolate_normal_mean_value(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Quad quad, uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_quad_fill_interpolate_color_mean_value(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Quad quad, Offset_zoom_param offset_zoom_param);

void    adl_quad_mesh_draw(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Quad_mesh mesh, uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_quad_mesh_fill(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Quad_mesh mesh, uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_quad_mesh_fill_interpolate_normal(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Quad_mesh mesh, uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_quad_mesh_fill_interpolate_color(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Quad_mesh mesh, Offset_zoom_param offset_zoom_param);

void    adl_circle_draw(Mat2D_uint32 screen_mat, float center_x, float center_y, float r, uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_circle_fill(Mat2D_uint32 screen_mat, float center_x, float center_y, float r, uint32_t color, Offset_zoom_param offset_zoom_param);

void    adl_tri_draw(Mat2D_uint32 screen_mat, Tri tri, uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_tri_fill_Pinedas_rasterizer(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Tri tri, uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_tri_fill_Pinedas_rasterizer_interpolate_color(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Tri tri, Offset_zoom_param offset_zoom_param);
void    adl_tri_fill_Pinedas_rasterizer_interpolate_normal(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Tri tri, uint32_t color, Offset_zoom_param offset_zoom_param);

void    adl_tri_mesh_draw(Mat2D_uint32 screen_mat, Tri_mesh mesh, uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_tri_mesh_fill_Pinedas_rasterizer(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Tri_mesh mesh, uint32_t color, Offset_zoom_param offset_zoom_param);
void    adl_tri_mesh_fill_Pinedas_rasterizer_interpolate_color(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Tri_mesh mesh, Offset_zoom_param offset_zoom_param);
void    adl_tri_mesh_fill_Pinedas_rasterizer_interpolate_normal(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Tri_mesh mesh, uint32_t color, Offset_zoom_param offset_zoom_param);

float   adl_tan_half_angle(Point vi, Point vj, Point p, float li, float lj);
float   adl_linear_map(float s, float min_in, float max_in, float min_out, float max_out);
void    adl_quad2tris(Quad quad, Tri *tri1, Tri *tri2, char split_line[]);
void    adl_linear_sRGB_to_okLab(uint32_t hex_ARGB, float *L, float *a, float *b);
void    adl_okLab_to_linear_sRGB(float L, float a, float b, uint32_t *hex_ARGB);
void    adl_linear_sRGB_to_okLch(uint32_t hex_ARGB, float *L, float *c, float *h_deg);
void    adl_okLch_to_linear_sRGB(float L, float c, float h_deg, uint32_t *hex_ARGB);
void    adl_interpolate_ARGBcolor_on_okLch(uint32_t color1, uint32_t color2, float t, float num_of_rotations, uint32_t *color_out);

Figure  adl_figure_alloc(size_t rows, size_t cols, Point top_left_position);
void    adl_figure_copy_to_screen(Mat2D_uint32 screen_mat, Figure figure);
void    adl_axis_draw_on_figure(Figure *figure);
void    adl_max_min_values_draw_on_figure(Figure figure);
void    adl_curve_add_to_figure(Figure *figure, Point *src_points, size_t src_len, uint32_t color);
void    adl_curves_plot_on_figure(Figure figure);
void    adl_2Dscalar_interp_on_figure(Figure figure, double *x_2Dmat, double *y_2Dmat, double *scalar_2Dmat, int ni, int nj, char color_scale[], float num_of_rotations);

Grid    adl_cartesian_grid_create(float min_e1, float max_e1, float min_e2, float max_e2, int num_samples_e1, int num_samples_e2, char plane[], float third_direction_position);
void    adl_grid_draw(Mat2D_uint32 screen_mat, Grid grid, uint32_t color, Offset_zoom_param offset_zoom_param);

#endif /*ALMOG_RENDER_SHAPES_H_*/

#ifdef ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#undef ALMOG_DRAW_LIBRARY_IMPLEMENTATION

#define RED_hexARGB    0xFFFF0000
#define GREEN_hexARGB  0xFF00FF00
#define BLUE_hexARGB   0xFF0000FF
#define PURPLE_hexARGB 0xFFFF00FF
#define CYAN_hexARGB   0xFF00FFFF
#define YELLOW_hexARGB 0xFFFFFF00

#define edge_cross_point(a1, b, a2, p) (b.x-a1.x)*(p.y-a2.y)-(b.y-a1.y)*(p.x-a2.x)
#define is_top_edge(x, y) (y == 0 && x > 0)
#define is_left_edge(x, y) (y < 0)
#define is_top_left(ps, pe) (is_top_edge(pe.x-ps.x, pe.y-ps.y) || is_left_edge(pe.x-ps.x, pe.y-ps.y))

#define ADL_MAX_POINT_VAL 1e5
#define adl_assert_point_is_valid(p) ADL_ASSERT(isfinite(p.x) && isfinite(p.y) && isfinite(p.z) && isfinite(p.w))
#define adl_assert_tri_is_valid(tri) adl_assert_point_is_valid(tri.points[0]); \
        adl_assert_point_is_valid(tri.points[1]);                              \
        adl_assert_point_is_valid(tri.points[2])
#define adl_assert_quad_is_valid(quad) adl_assert_point_is_valid(quad.points[0]);   \
        adl_assert_point_is_valid(quad.points[1]);                                  \
        adl_assert_point_is_valid(quad.points[2]);                                  \
        adl_assert_point_is_valid(quad.points[3])

#define ADL_FIGURE_PADDING_PRECENTAGE 20
#define ADL_MAX_FIGURE_PADDING 70
#define ADL_MIN_FIGURE_PADDING 20
#define ADL_MAX_HEAD_SIZE 15
#define ADL_FIGURE_HEAD_ANGLE_DEG 30
#define ADL_FIGURE_AXIS_COLOR 0xff000000

#define ADL_MAX_CHARACTER_OFFSET 10
#define ADL_MIN_CHARACTER_OFFSET 5
#define ADL_MAX_SENTENCE_LEN 256
#define ADL_MAX_ZOOM 1e3

#define ADL_DEFAULT_OFFSET_ZOOM (Offset_zoom_param){1,0,0,0,0}
#define adl_offset_zoom_point(p, window_w, window_h, offset_zoom_param)                                             \
    (p).x = ((p).x - (window_w)/2 + offset_zoom_param.offset_x) * offset_zoom_param.zoom_multiplier + (window_w)/2; \
    (p).y = ((p).y - (window_h)/2 + offset_zoom_param.offset_y) * offset_zoom_param.zoom_multiplier + (window_h)/2

/**
 * @brief Draw a single pixel with alpha blending.
 *
 * Applies the pan/zoom transform and writes the pixel if it falls inside
 * the destination bounds. The source color is blended over the existing
 * pixel using the source alpha; the stored alpha is set to 255.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param x X coordinate in pixels (before pan/zoom).
 * @param y Y coordinate in pixels (before pan/zoom).
 * @param color Source color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM
 *        for identity.
 */
void adl_point_draw(Mat2D_uint32 screen_mat, int x, int y, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    float window_w = (float)screen_mat.cols;
    float window_h = (float)screen_mat.rows;

    x = (x - window_w/2 + offset_zoom_param.offset_x) * offset_zoom_param.zoom_multiplier + window_w/2;
    y = (y - window_h/2 + offset_zoom_param.offset_y) * offset_zoom_param.zoom_multiplier + window_h/2;

    if ((x < (int)screen_mat.cols && y < (int)screen_mat.rows) && (x >= 0 && y >= 0)) { /* point is in screen */
        uint8_t r_new, g_new, b_new, a_new;
        uint8_t r_current, g_current, b_current, a_current;
        HexARGB_RGBA_VAR(MAT2D_AT_UINT32(screen_mat, y, x), r_current, g_current, b_current, a_current);
        HexARGB_RGBA_VAR(color, r_new, g_new, b_new, a_new);
        MAT2D_AT_UINT32(screen_mat, y, x) = RGBA_hexARGB(r_current*(1-a_new/255.0f) + r_new*a_new/255.0f, g_current*(1-a_new/255.0f) + g_new*a_new/255.0f, b_current*(1-a_new/255.0f) + b_new*a_new/255.0f, 255);
        (void)a_current;
    }
}

/**
 * @brief Draw an anti-aliased-like line by vertical spans (integer grid).
 *
 * The line is rasterized with a simple integer-span approach. Pan/zoom is
 * applied about the screen center prior to rasterization.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param x1_input Line start X (before pan/zoom).
 * @param y1_input Line start Y (before pan/zoom).
 * @param x2_input Line end X (before pan/zoom).
 * @param y2_input Line end Y (before pan/zoom).
 * @param color Line color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM
 *        for identity.
 */
void adl_line_draw(Mat2D_uint32 screen_mat, const float x1_input, const float y1_input, const float x2_input, const float y2_input, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    /* This function is inspired by the Olive.c function developed by 'Tsoding' on his YouTube channel. You can fined the video in this link: https://youtu.be/LmQKZmQh1ZQ?list=PLpM-Dvs8t0Va-Gb0Dp4d9t8yvNFHaKH6N&t=4683. */

    float window_w = (float)screen_mat.cols;
    float window_h = (float)screen_mat.rows;

    int x1 = (x1_input - window_w/2 + offset_zoom_param.offset_x) * offset_zoom_param.zoom_multiplier + window_w/2;
    int x2 = (x2_input - window_w/2 + offset_zoom_param.offset_x) * offset_zoom_param.zoom_multiplier + window_w/2;
    int y1 = (y1_input - window_h/2 + offset_zoom_param.offset_y) * offset_zoom_param.zoom_multiplier + window_h/2;
    int y2 = (y2_input - window_h/2 + offset_zoom_param.offset_y) * offset_zoom_param.zoom_multiplier + window_h/2;

    ADL_ASSERT((int)fabsf(fabsf((float)x2) - fabsf((float)x1)) < ADL_MAX_POINT_VAL);
    ADL_ASSERT((int)fabsf(fabsf((float)y2) - fabsf((float)y1)) < ADL_MAX_POINT_VAL);

    int x = x1;
    int y = y1;
    int dx, dy;

    adl_point_draw(screen_mat, x, y, color, (Offset_zoom_param){1,0,0,0,0});

    dx = x2 - x1;
    dy = y2 - y1;

    ADL_ASSERT(dy > INT_MIN && dy < INT_MAX);
    ADL_ASSERT(dx > INT_MIN && dx < INT_MAX);

    if (0 == dx && 0 == dy) return;
    if (0 == dx) {
        while (x != x2 || y != y2) {
            if (dy > 0) {
                y++;
            }
            if (dy < 0) {
                y--;
            }
            adl_point_draw(screen_mat, x, y, color, (Offset_zoom_param){1,0,0,0,0});
        }
        return;
    }
    if (0 == dy) {
        while (x != x2 || y != y2) {
            if (dx > 0) {
                x++;
            }
            if (dx < 0) {
                x--;
            }
            adl_point_draw(screen_mat, x, y, color, (Offset_zoom_param){1,0,0,0,0});
        }
        return;
    }

    /* float m = (float)dy / dx */
    int b = y1 - dy * x1 / dx;

    if (x1 > x2) {
        int temp_x = x1;
        x1 = x2;
        x2 = temp_x;
    }
    for (x = x1; x < x2; x++) {
        int sy1 = dy * x / dx + b;
        int sy2 = dy * (x + 1) / dx + b;
        if (sy1 > sy2) {
            int temp_y = sy1;
            sy1 = sy2;
            sy2 = temp_y;
        }
        for (y = sy1; y <= sy2; y++) {
            adl_point_draw(screen_mat, x, y, color, (Offset_zoom_param){1,0,0,0,0});
        }
    }

}

/**
 * @brief Draw a polyline connecting an array of points.
 *
 * Draws segments between consecutive points: p[0]-p[1]-...-p[len-1].
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param points Array of points in pixel space (before pan/zoom).
 * @param len Number of points in the array (>= 1).
 * @param color Line color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_lines_draw(const Mat2D_uint32 screen_mat, const Point *points, const size_t len, const uint32_t color, Offset_zoom_param offset_zoom_param)
{
    if (len == 0) return;
    for (size_t i = 0; i < len-1; i++) {
        adl_line_draw(screen_mat, points[i].x, points[i].y, points[i+1].x, points[i+1].y, color, offset_zoom_param);
    }
}

/**
 * @brief Draw a closed polyline (loop).
 *
 * Same as adl_lines_draw, plus an extra segment from the last point back
 * to the first point.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param points Array of points in pixel space (before pan/zoom).
 * @param len Number of points in the array (>= 1).
 * @param color Line color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_lines_loop_draw(const Mat2D_uint32 screen_mat, const Point *points, const size_t len, const uint32_t color, Offset_zoom_param offset_zoom_param)
{
    if (len == 0) return;
    for (size_t i = 0; i < len-1; i++) {
        adl_line_draw(screen_mat, points[i].x, points[i].y, points[i+1].x, points[i+1].y, color, offset_zoom_param);
    }
    adl_line_draw(screen_mat, points[len-1].x, points[len-1].y, points[0].x, points[0].y, color, offset_zoom_param);
}


/**
 * @brief Draw an arrow from start to end with a triangular head.
 *
 * The head is constructed by rotating around the arrow tip by
 * +/- angle_deg and using head_size as a fraction of the shaft length.

 * @note: This function is a bit complicated and expansive but this is what I could come up with
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param xs Start X (before pan/zoom).
 * @param ys Start Y (before pan/zoom).
 * @param xe End X (before pan/zoom), i.e., the arrow tip.
 * @param ye End Y (before pan/zoom), i.e., the arrow tip.
 * @param head_size Head size as a fraction of total length in [0,1].
 * @param angle_deg Head wing rotation angle in degrees.
 * @param color Arrow color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_arrow_draw(Mat2D_uint32 screen_mat, int xs, int ys, int xe, int ye, float head_size, float angle_deg, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    Mat2D pe = mat2D_alloc(3, 1);
    mat2D_fill(pe, 0);
    MAT2D_AT(pe, 0, 0) = xe;
    MAT2D_AT(pe, 1, 0) = ye;
    Mat2D v1 = mat2D_alloc(3, 1);
    mat2D_fill(v1, 0);
    Mat2D v2 = mat2D_alloc(3, 1);
    mat2D_fill(v2, 0);
    Mat2D temp_v = mat2D_alloc(3, 1);
    mat2D_fill(temp_v, 0);
    Mat2D DCM_p = mat2D_alloc(3, 3);
    mat2D_fill(DCM_p, 0);
    mat2D_set_rot_mat_z(DCM_p, angle_deg);
    Mat2D DCM_m = mat2D_alloc(3, 3);
    mat2D_fill(DCM_m, 0);
    mat2D_set_rot_mat_z(DCM_m, -angle_deg);

    int x_center = xs*head_size + xe*(1-head_size);
    int y_center = ys*head_size + ye*(1-head_size);

    MAT2D_AT(v1, 0, 0) = x_center;
    MAT2D_AT(v1, 1, 0) = y_center;
    mat2D_copy(v2, v1);

    /* v1 */
    mat2D_copy(temp_v, v1);
    mat2D_sub(temp_v, pe);
    mat2D_fill(v1, 0);
    mat2D_dot(v1, DCM_p, temp_v);
    mat2D_add(v1, pe);

    /* v2 */
    mat2D_copy(temp_v, v2);
    mat2D_sub(temp_v, pe);
    mat2D_fill(v2, 0);
    mat2D_dot(v2, DCM_m, temp_v);
    mat2D_add(v2, pe);

    adl_line_draw(screen_mat, MAT2D_AT(v1, 0, 0), MAT2D_AT(v1, 1, 0), xe, ye, color, offset_zoom_param);
    adl_line_draw(screen_mat, MAT2D_AT(v2, 0, 0), MAT2D_AT(v2, 1, 0), xe, ye, color, offset_zoom_param);
    adl_line_draw(screen_mat, xs, ys, xe, ye, color, offset_zoom_param);

    mat2D_free(pe);
    mat2D_free(v1);
    mat2D_free(v2);
    mat2D_free(temp_v);
    mat2D_free(DCM_p);
    mat2D_free(DCM_m);
}

/**
 * @brief Draw a vector glyph for a single ASCII character.
 *
 * Only a limited set of characters is supported (A–Z, a–z, 0–9, space,
 * '.', ':', '-', '+'). Unsupported characters are rendered as a framed
 * box with an 'X'. Coordinates are for the character's top-left corner.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param c The character to draw.
 * @param width_pixel Character box width in pixels.
 * @param hight_pixel Character box height in pixels (spelled as in API).
 * @param x_top_left X of top-left corner (before pan/zoom).
 * @param y_top_left Y of top-left corner (before pan/zoom).
 * @param color Stroke color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_character_draw(Mat2D_uint32 screen_mat, char c, int width_pixel, int hight_pixel, int x_top_left, int y_top_left, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    switch (c)
    {
    case 'a':
    case 'A':
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel/2, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/6, y_top_left+2*hight_pixel/3, x_top_left+5*width_pixel/6, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        break;
    case 'b':
    case 'B':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'c':
    case 'C':
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'd':
    case 'D':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left, y_top_left, color, offset_zoom_param);
        break;
    case 'e':
    case 'E':
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/2, color, offset_zoom_param);
        break;
    case 'f':
    case 'F':
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/2, color, offset_zoom_param);
        break;
    case 'g':
    case 'G':
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/2, x_top_left+width_pixel/2, y_top_left+hight_pixel/2, color, offset_zoom_param);
        break;
    case 'h':
    case 'H':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/2, color, offset_zoom_param);
        break;
    case 'i':
    case 'I':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'j':
    case 'J':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+2*width_pixel/3, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/2, y_top_left+hight_pixel, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel/6, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        break;
    case 'k':
    case 'K':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        break;
    case 'l':
    case 'L':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'm':
    case 'M':
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/2, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'n':
    case 'N':
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        break;
    case 'o':
    case 'O':
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        break;
    case 'p':
    case 'P':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left, y_top_left+hight_pixel/2, color, offset_zoom_param);
        break;
    case 'q':
    case 'Q':
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'r':
    case 'R':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 's':
    case 'S':
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/3, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        break;
    case 't':
    case 'T':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'u':
    case 'U':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        break;
    case 'v':
    case 'V':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/2, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        break;
    case 'w':
    case 'W':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel/2, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        break;
    case 'x':
    case 'X':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        break;
    case 'y':
    case 'Y':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/2, y_top_left+hight_pixel/2, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'z':
    case 'Z':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case '.':
        adl_rectangle_fill_min_max(screen_mat, x_top_left+width_pixel/6, x_top_left+width_pixel/3, y_top_left+5*hight_pixel/6, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case ':':
        adl_rectangle_fill_min_max(screen_mat, x_top_left+width_pixel/6, x_top_left+width_pixel/3, y_top_left+5*hight_pixel/6, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_rectangle_fill_min_max(screen_mat, x_top_left+width_pixel/6, x_top_left+width_pixel/3, y_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);
        break;
    case '0':
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        break;
    case '1':
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left+width_pixel/2, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case '2':
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case '3':
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        break;
    case '4':
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+2*hight_pixel/3, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        break;
    case '5':
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        break;
    case '6':
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, x_top_left, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        break;
    case '7':
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case '8':
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/3, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);

        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, x_top_left, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+2*hight_pixel/3, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        break;
    case '9':
        adl_line_draw(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/3, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/3, color, offset_zoom_param);
        break;
    case '-':
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/2, color, offset_zoom_param);
        break;
    case '+':
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case ' ':
        break;
    default:
        adl_rectangle_draw_min_max(screen_mat, x_top_left, x_top_left+width_pixel, y_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_line_draw(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        break;
    }
}

/**
 * @brief Draw a horizontal sentence using vector glyphs.
 *
 * Characters are laid out left-to-right with a spacing derived from the
 * character height. All characters share the same height.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param sentence ASCII string buffer.
 * @param len Number of characters to draw from sentence.
 * @param x_top_left X of top-left of the first character (before transform).
 * @param y_top_left Y of top-left of the first character (before transform).
 * @param hight_pixel Character height in pixels (spelled as in API).
 * @param color Stroke color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_sentence_draw(Mat2D_uint32 screen_mat, const char sentence[], size_t len, const int x_top_left, const int y_top_left, const int hight_pixel, const uint32_t color, Offset_zoom_param offset_zoom_param)
{
    int character_width_pixel = hight_pixel/2;
    int current_x_top_left = x_top_left;
    int character_x_offset = (int)fmaxf(fminf(ADL_MAX_CHARACTER_OFFSET, character_width_pixel / 5), ADL_MIN_CHARACTER_OFFSET);

    for (size_t char_index = 0; char_index < len; char_index++) {
        adl_character_draw(screen_mat, sentence[char_index], character_width_pixel, hight_pixel, current_x_top_left, y_top_left, color, offset_zoom_param);
        current_x_top_left += character_width_pixel + character_x_offset;
    }

}

/**
 * @brief Draw a rectangle outline defined by min/max corners (inclusive).
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param min_x Minimum X (before pan/zoom).
 * @param max_x Maximum X (before pan/zoom).
 * @param min_y Minimum Y (before pan/zoom).
 * @param max_y Maximum Y (before pan/zoom).
 * @param color Stroke color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_rectangle_draw_min_max(Mat2D_uint32 screen_mat, int min_x, int max_x, int min_y, int max_y, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    adl_line_draw(screen_mat, min_x, min_y, max_x, min_y, color, offset_zoom_param);
    adl_line_draw(screen_mat, min_x, max_y, max_x, max_y, color, offset_zoom_param);
    adl_line_draw(screen_mat, min_x, min_y, min_x, max_y, color, offset_zoom_param);
    adl_line_draw(screen_mat, max_x, min_y, max_x, max_y, color, offset_zoom_param);
}

/**
 * @brief Fill a rectangle defined by min/max corners (inclusive).
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param min_x Minimum X (before pan/zoom).
 * @param max_x Maximum X (before pan/zoom).
 * @param min_y Minimum Y (before pan/zoom).
 * @param max_y Maximum Y (before pan/zoom).
 * @param color Fill color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_rectangle_fill_min_max(Mat2D_uint32 screen_mat, int min_x, int max_x, int min_y, int max_y, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (int y = min_y; y <= max_y; y++) {
        adl_line_draw(screen_mat, min_x, y, max_x, y, color, offset_zoom_param);
    }
}

/**
 * @brief Draw the outline of a quad (four points, looped).
 *
 * Depth buffer is not used in this outline variant.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param inv_z_buffer Unused for outline; safe to pass a dummy Mat2D.
 * @param quad Quad to draw in pixel space (before transform).
 * @param color Stroke color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_quad_draw(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Quad quad, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    (void)inv_z_buffer;
    adl_lines_loop_draw(screen_mat, quad.points, 4, color, offset_zoom_param);
}

/**
 * @brief Fill a quad using mean-value (Barycentric) coordinates and flat base color.
 *
 * Performs a depth test against inv_z_buffer and modulates the base color
 * with the average light_intensity of the quad's vertices.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param inv_z_buffer Inverse-Z buffer (larger is closer).
 * @param quad Quad in pixel space; points carry z and w for depth.
 * @param color Base color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_quad_fill(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Quad quad, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    Point p0 = quad.points[0];
    Point p1 = quad.points[1];
    Point p2 = quad.points[2];
    Point p3 = quad.points[3];

    int x_min = fminf(p0.x, fminf(p1.x, fminf(p2.x, p3.x)));
    int x_max = fmaxf(p0.x, fmaxf(p1.x, fmaxf(p2.x, p3.x)));
    int y_min = fminf(p0.y, fminf(p1.y, fminf(p2.y, p3.y)));
    int y_max = fmaxf(p0.y, fmaxf(p1.y, fmaxf(p2.y, p3.y)));

    if (x_min < 0) x_min = 0;
    if (y_min < 0) y_min = 0;
    if (x_max >= (int)screen_mat.cols) x_max = (int)screen_mat.cols - 1;
    if (y_max >= (int)screen_mat.rows) y_max = (int)screen_mat.rows - 1;

    float w = edge_cross_point(p0, p1, p1, p2) + edge_cross_point(p2, p3, p3, p0);
    if (fabs(w) < 1e-6) {
        // adl_quad_draw(screen_mat, inv_z_buffer, quad, quad.colors[0], offset_zoom_param);
        return;
    }

    float size_p3_to_p0 = sqrt((p0.x - p3.x)*(p0.x - p3.x) + (p0.y - p3.y)*(p0.y - p3.y));
    float size_p0_to_p1 = sqrt((p1.x - p0.x)*(p1.x - p0.x) + (p1.y - p0.y)*(p1.y - p0.y));
    float size_p1_to_p2 = sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
    float size_p2_to_p3 = sqrt((p3.x - p2.x)*(p3.x - p2.x) + (p3.y - p2.y)*(p3.y - p2.y));

    int r, g, b, a;
    HexARGB_RGBA_VAR(color, r, g, b, a);
    float light_intensity = (quad.light_intensity[0] + quad.light_intensity[1] + quad.light_intensity[2] + quad.light_intensity[3]) / 4;
    uint8_t base_r = (uint8_t)fmaxf(0, fminf(255, r * light_intensity));
    uint8_t base_g = (uint8_t)fmaxf(0, fminf(255, g * light_intensity));
    uint8_t base_b = (uint8_t)fmaxf(0, fminf(255, b * light_intensity));

    for (int y = y_min; y <= y_max; y++) {
        for (int x = x_min; x <= x_max; x++) {
            Point p = {.x = x, .y = y, .z = 0};
            bool in_01, in_12, in_23, in_30;

            in_01 = (edge_cross_point(p0, p1, p0, p) >= 0) != (w < 0);
            in_12 = (edge_cross_point(p1, p2, p1, p) >= 0) != (w < 0);
            in_23 = (edge_cross_point(p2, p3, p2, p) >= 0) != (w < 0);
            in_30 = (edge_cross_point(p3, p0, p3, p) >= 0) != (w < 0);

            /* https://www.mn.uio.no/math/english/people/aca/michaelf/papers/mv3d.pdf. */
            float size_p_to_p0 = sqrt((p0.x - p.x)*(p0.x - p.x) + (p0.y - p.y)*(p0.y - p.y));
            float size_p_to_p1 = sqrt((p1.x - p.x)*(p1.x - p.x) + (p1.y - p.y)*(p1.y - p.y));
            float size_p_to_p2 = sqrt((p2.x - p.x)*(p2.x - p.x) + (p2.y - p.y)*(p2.y - p.y));
            float size_p_to_p3 = sqrt((p3.x - p.x)*(p3.x - p.x) + (p3.y - p.y)*(p3.y - p.y));

            /* tangent of half the angle directly using vector math */
            float tan_theta_3_over_2 = size_p3_to_p0 / (size_p_to_p3 + size_p_to_p0);
            float tan_theta_0_over_2 = size_p0_to_p1 / (size_p_to_p0 + size_p_to_p1);
            float tan_theta_1_over_2 = size_p1_to_p2 / (size_p_to_p1 + size_p_to_p2);
            float tan_theta_2_over_2 = size_p2_to_p3 / (size_p_to_p2 + size_p_to_p3);
            float w0 = (tan_theta_3_over_2 + tan_theta_0_over_2) / size_p_to_p0;
            float w1 = (tan_theta_0_over_2 + tan_theta_1_over_2) / size_p_to_p1;
            float w2 = (tan_theta_1_over_2 + tan_theta_2_over_2) / size_p_to_p2;
            float w3 = (tan_theta_2_over_2 + tan_theta_3_over_2) / size_p_to_p3;

            float inv_w_tot = 1.0f / (w0 + w1 + w2 + w3);
            float alpha = w0 * inv_w_tot;
            float beta  = w1 * inv_w_tot;
            float gamma = w2 * inv_w_tot;
            float delta = w3 * inv_w_tot;

            if (in_01 && in_12 && in_23 && in_30) {

                double inv_w = alpha * (1.0f / p0.w) + beta  * (1.0f / p1.w) + gamma * (1.0f / p2.w) + delta * (1.0f / p3.w);
                double z_over_w = alpha * (p0.z / p0.w) + beta  * (p1.z / p1.w) + gamma * (p2.z / p2.w) + delta * (p3.z / p3.w);
                double inv_z = inv_w / z_over_w;

                if (inv_z >= MAT2D_AT(inv_z_buffer, y, x)) {
                    adl_point_draw(screen_mat, x, y, RGBA_hexARGB(base_r, base_g, base_b, a), offset_zoom_param);
                    MAT2D_AT(inv_z_buffer, y, x) = inv_z;
                }
            }
        }
    }
}

/**
 * @brief Fill a quad with per-pixel light interpolation (mean value coords).
 *
 * Interpolates light_intensity across the quad using mean-value
 * coordinates and modulates a uniform base color. Depth-tested.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param inv_z_buffer Inverse-Z buffer (larger is closer).
 * @param quad Quad in pixel space; points carry z and w for depth.
 * @param color Base color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_quad_fill_interpolate_normal_mean_value(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Quad quad, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    Point p0 = quad.points[0];
    Point p1 = quad.points[1];
    Point p2 = quad.points[2];
    Point p3 = quad.points[3];

    int x_min = fminf(p0.x, fminf(p1.x, fminf(p2.x, p3.x)));
    int x_max = fmaxf(p0.x, fmaxf(p1.x, fmaxf(p2.x, p3.x)));
    int y_min = fminf(p0.y, fminf(p1.y, fminf(p2.y, p3.y)));
    int y_max = fmaxf(p0.y, fmaxf(p1.y, fmaxf(p2.y, p3.y)));

    if (x_min < 0) x_min = 0;
    if (y_min < 0) y_min = 0;
    if (x_max >= (int)screen_mat.cols) x_max = (int)screen_mat.cols - 1;
    if (y_max >= (int)screen_mat.rows) y_max = (int)screen_mat.rows - 1;

    float w = edge_cross_point(p0, p1, p1, p2) + edge_cross_point(p2, p3, p3, p0);
    if (fabs(w) < 1e-6) {
        // adl_quad_draw(screen_mat, inv_z_buffer, quad, quad.colors[0], offset_zoom_param);
        return;
    }

    int r, g, b, a;
    HexARGB_RGBA_VAR(color, r, g, b, a);

    for (int y = y_min; y <= y_max; y++) {
        for (int x = x_min; x <= x_max; x++) {
            Point p = {.x = x, .y = y, .z = 0};
            bool in_01, in_12, in_23, in_30;

            in_01 = (edge_cross_point(p0, p1, p0, p) >= 0) != (w < 0);
            in_12 = (edge_cross_point(p1, p2, p1, p) >= 0) != (w < 0);
            in_23 = (edge_cross_point(p2, p3, p2, p) >= 0) != (w < 0);
            in_30 = (edge_cross_point(p3, p0, p3, p) >= 0) != (w < 0);

            /* using 'mean value coordinates'
             * https://www.mn.uio.no/math/english/people/aca/michaelf/papers/mv3d.pdf. */
            float size_p_to_p0 = sqrt((p0.x - p.x)*(p0.x - p.x) + (p0.y - p.y)*(p0.y - p.y));
            float size_p_to_p1 = sqrt((p1.x - p.x)*(p1.x - p.x) + (p1.y - p.y)*(p1.y - p.y));
            float size_p_to_p2 = sqrt((p2.x - p.x)*(p2.x - p.x) + (p2.y - p.y)*(p2.y - p.y));
            float size_p_to_p3 = sqrt((p3.x - p.x)*(p3.x - p.x) + (p3.y - p.y)*(p3.y - p.y));

            /* calculating the tangent of half the angle directly using vector math */
            float t0 = adl_tan_half_angle(p0, p1, p, size_p_to_p0, size_p_to_p1);
            float t1 = adl_tan_half_angle(p1, p2, p, size_p_to_p1, size_p_to_p2);
            float t2 = adl_tan_half_angle(p2, p3, p, size_p_to_p2, size_p_to_p3);
            float t3 = adl_tan_half_angle(p3, p0, p, size_p_to_p3, size_p_to_p0);

            float w0 = (t3 + t0) / size_p_to_p0;
            float w1 = (t0 + t1) / size_p_to_p1;
            float w2 = (t1 + t2) / size_p_to_p2;
            float w3 = (t2 + t3) / size_p_to_p3;

            float inv_w_tot = 1.0f / (w0 + w1 + w2 + w3);
            float alpha = w0 * inv_w_tot;
            float beta  = w1 * inv_w_tot;
            float gamma = w2 * inv_w_tot;
            float delta = w3 * inv_w_tot;

            if (in_01 && in_12 && in_23 && in_30) {
                float light_intensity = quad.light_intensity[0]*alpha + quad.light_intensity[1]*beta + quad.light_intensity[2]*gamma + quad.light_intensity[3]*delta;

                float rf = r * light_intensity;
                float gf = g * light_intensity;
                float bf = b * light_intensity;
                uint8_t r8 = (uint8_t)fmaxf(0, fminf(255, rf));
                uint8_t g8 = (uint8_t)fmaxf(0, fminf(255, gf));
                uint8_t b8 = (uint8_t)fmaxf(0, fminf(255, bf));

                double inv_w = alpha * (1.0f / p0.w) + beta  * (1.0f / p1.w) + gamma * (1.0f / p2.w) + delta * (1.0f / p3.w);
                double z_over_w = alpha * (p0.z / p0.w) + beta  * (p1.z / p1.w) + gamma * (p2.z / p2.w) + delta * (p3.z / p3.w);
                double inv_z = inv_w / z_over_w;

                if (inv_z >= MAT2D_AT(inv_z_buffer, y, x)) {
                    adl_point_draw(screen_mat, x, y, RGBA_hexARGB(r8, g8, b8, a), offset_zoom_param);
                    MAT2D_AT(inv_z_buffer, y, x) = inv_z;
                }
            }
        }
    }
}

/**
 * @brief Fill a quad with per-vertex colors (mean value coords).
 *
 * Interpolates ARGB vertex colors using mean-value coordinates, optionally
 * modulated by the average light_intensity. Depth-tested.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param inv_z_buffer Inverse-Z buffer (larger is closer).
 * @param quad Quad in pixel space with quad.colors[] set.
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_quad_fill_interpolate_color_mean_value(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Quad quad, Offset_zoom_param offset_zoom_param)
{
    Point p0 = quad.points[0];
    Point p1 = quad.points[1];
    Point p2 = quad.points[2];
    Point p3 = quad.points[3];

    int x_min = fminf(p0.x, fminf(p1.x, fminf(p2.x, p3.x)));
    int x_max = fmaxf(p0.x, fmaxf(p1.x, fmaxf(p2.x, p3.x)));
    int y_min = fminf(p0.y, fminf(p1.y, fminf(p2.y, p3.y)));
    int y_max = fmaxf(p0.y, fmaxf(p1.y, fmaxf(p2.y, p3.y)));

    if (x_min < 0) x_min = 0;
    if (y_min < 0) y_min = 0;
    if (x_max >= (int)screen_mat.cols) x_max = (int)screen_mat.cols - 1;
    if (y_max >= (int)screen_mat.rows) y_max = (int)screen_mat.rows - 1;

    float w = edge_cross_point(p0, p1, p1, p2) + edge_cross_point(p2, p3, p3, p0);
    if (fabs(w) < 1e-6) {
        // adl_quad_draw(screen_mat, inv_z_buffer, quad, quad.colors[0], offset_zoom_param);
        return;
    }

    for (int y = y_min; y <= y_max; y++) {
        for (int x = x_min; x <= x_max; x++) {
            Point p = {.x = x, .y = y, .z = 0};
            bool in_01, in_12, in_23, in_30;

            in_01 = (edge_cross_point(p0, p1, p0, p) >= 0) != (w < 0);
            in_12 = (edge_cross_point(p1, p2, p1, p) >= 0) != (w < 0);
            in_23 = (edge_cross_point(p2, p3, p2, p) >= 0) != (w < 0);
            in_30 = (edge_cross_point(p3, p0, p3, p) >= 0) != (w < 0);

            /* using 'mean value coordinates'
             * https://www.mn.uio.no/math/english/people/aca/michaelf/papers/mv3d.pdf. */
            float size_p_to_p0 = sqrt((p0.x - p.x)*(p0.x - p.x) + (p0.y - p.y)*(p0.y - p.y));
            float size_p_to_p1 = sqrt((p1.x - p.x)*(p1.x - p.x) + (p1.y - p.y)*(p1.y - p.y));
            float size_p_to_p2 = sqrt((p2.x - p.x)*(p2.x - p.x) + (p2.y - p.y)*(p2.y - p.y));
            float size_p_to_p3 = sqrt((p3.x - p.x)*(p3.x - p.x) + (p3.y - p.y)*(p3.y - p.y));

            /* calculating the tangent of half the angle directly using vector math */
            float t0 = adl_tan_half_angle(p0, p1, p, size_p_to_p0, size_p_to_p1);
            float t1 = adl_tan_half_angle(p1, p2, p, size_p_to_p1, size_p_to_p2);
            float t2 = adl_tan_half_angle(p2, p3, p, size_p_to_p2, size_p_to_p3);
            float t3 = adl_tan_half_angle(p3, p0, p, size_p_to_p3, size_p_to_p0);

            float w0 = (t3 + t0) / size_p_to_p0;
            float w1 = (t0 + t1) / size_p_to_p1;
            float w2 = (t1 + t2) / size_p_to_p2;
            float w3 = (t2 + t3) / size_p_to_p3;

            float inv_w_tot = 1.0f / (w0 + w1 + w2 + w3);
            float alpha = w0 * inv_w_tot;
            float beta  = w1 * inv_w_tot;
            float gamma = w2 * inv_w_tot;
            float delta = w3 * inv_w_tot;

            if (in_01 && in_12 && in_23 && in_30) {
                int r0, g0, b0, a0;
                int r1, g1, b1, a1;
                int r2, g2, b2, a2;
                int r3, g3, b3, a3;
                HexARGB_RGBA_VAR(quad.colors[0], r0, g0, b0, a0);
                HexARGB_RGBA_VAR(quad.colors[1], r1, g1, b1, a1);
                HexARGB_RGBA_VAR(quad.colors[2], r2, g2, b2, a2);
                HexARGB_RGBA_VAR(quad.colors[3], r3, g3, b3, a3);
                
                uint8_t current_r = r0*alpha + r1*beta + r2*gamma + r3*delta;
                uint8_t current_g = g0*alpha + g1*beta + g2*gamma + g3*delta;
                uint8_t current_b = b0*alpha + b1*beta + b2*gamma + b3*delta;
                uint8_t current_a = a0*alpha + a1*beta + a2*gamma + a3*delta;

                float light_intensity = (quad.light_intensity[0] + quad.light_intensity[1] + quad.light_intensity[2] + quad.light_intensity[3]) / 4;
                float rf = current_r * light_intensity;
                float gf = current_g * light_intensity;
                float bf = current_b * light_intensity;
                uint8_t r8 = (uint8_t)fmaxf(0, fminf(255, rf));
                uint8_t g8 = (uint8_t)fmaxf(0, fminf(255, gf));
                uint8_t b8 = (uint8_t)fmaxf(0, fminf(255, bf));

                double inv_w = alpha * (1.0f / p0.w) + beta  * (1.0f / p1.w) + gamma * (1.0f / p2.w) + delta * (1.0f / p3.w);
                double z_over_w = alpha * (p0.z / p0.w) + beta  * (p1.z / p1.w) + gamma * (p2.z / p2.w) + delta * (p3.z / p3.w);
                double inv_z = inv_w / z_over_w;

                if (inv_z >= MAT2D_AT(inv_z_buffer, y, x)) {
                    adl_point_draw(screen_mat, x, y, RGBA_hexARGB(r8, g8, b8, current_a), offset_zoom_param);
                    MAT2D_AT(inv_z_buffer, y, x) = inv_z;
                }
            }
        }
    }
}

/**
 * @brief Draw outlines for all quads in a mesh.
 *
 * Skips elements with to_draw == false. Depth buffer is not used.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param inv_z_buffer_mat Unused for outline; safe to pass a dummy Mat2D.
 * @param mesh Quad mesh (array + length).
 * @param color Stroke color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_quad_mesh_draw(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Quad_mesh mesh, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Quad quad = mesh.elements[i];
        /* Reject invalid quad */
        adl_assert_quad_is_valid(quad);

        if (!quad.to_draw) continue;

        adl_quad_draw(screen_mat, inv_z_buffer_mat, quad, color, offset_zoom_param);
    }
}

/**
 * @brief Fill all quads in a mesh with a uniform base color.
 *
 * Applies per-quad average light_intensity. Depth-tested.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param inv_z_buffer_mat Inverse-Z buffer (larger is closer).
 * @param mesh Quad mesh (array + length).
 * @param color Base color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_quad_mesh_fill(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Quad_mesh mesh, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Quad quad = mesh.elements[i];
        /* Reject invalid quad */
        adl_assert_quad_is_valid(quad);

        if (!quad.to_draw) continue;

        // color = rand_double() * 0xFFFFFFFF;

        adl_quad_fill(screen_mat, inv_z_buffer_mat, quad, color, offset_zoom_param);
    }
}

/**
 * @brief Fill all quads in a mesh using interpolated lighting.
 *
 * Interpolates light_intensity across quads and modulates a uniform base
 * color. Depth-tested.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param inv_z_buffer_mat Inverse-Z buffer (larger is closer).
 * @param mesh Quad mesh (array + length).
 * @param color Base color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_quad_mesh_fill_interpolate_normal(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Quad_mesh mesh, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Quad quad = mesh.elements[i];
        /* Reject invalid quad */
        adl_assert_quad_is_valid(quad);

        uint8_t a, r, g, b;
        HexARGB_RGBA_VAR(color, a, r, g, b);
        (void)r;
        (void)g;
        (void)b;

        if (!quad.to_draw && a == 255) continue;

        adl_quad_fill_interpolate_normal_mean_value(screen_mat, inv_z_buffer_mat, quad, color, offset_zoom_param);
    }
}

/**
 * @brief Fill all quads in a mesh using per-vertex colors.
 *
 * Interpolates quad.colors[] across each quad with mean-value coordinates.
 * Depth-tested.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param inv_z_buffer_mat Inverse-Z buffer (larger is closer).
 * @param mesh Quad mesh (array + length).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_quad_mesh_fill_interpolate_color(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Quad_mesh mesh, Offset_zoom_param offset_zoom_param)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Quad quad = mesh.elements[i];
        /* Reject invalid quad */
        adl_assert_quad_is_valid(quad);

        if (!quad.to_draw) continue;

        adl_quad_fill_interpolate_color_mean_value(screen_mat, inv_z_buffer_mat, quad, offset_zoom_param);
    }
}

/**
 * @brief Draw an approximate circle outline (1px thickness).
 *
 * The outline is approximated on the integer grid by sampling a band
 * around radius r.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param center_x Circle center X (before pan/zoom).
 * @param center_y Circle center Y (before pan/zoom).
 * @param r Circle radius in pixels.
 * @param color Stroke color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_circle_draw(Mat2D_uint32 screen_mat, float center_x, float center_y, float r, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx ++) {
            float diff = dx * dx + dy * dy - r * r;
            if (diff < 0 && diff > -r*2) {
                adl_point_draw(screen_mat, center_x + dx, center_y + dy, color, offset_zoom_param);
            }
        }
    }
}

/**
 * @brief Fill a circle.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param center_x Circle center X (before pan/zoom).
 * @param center_y Circle center Y (before pan/zoom).
 * @param r Circle radius in pixels.
 * @param color Fill color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_circle_fill(Mat2D_uint32 screen_mat, float center_x, float center_y, float r, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx ++) {
            float diff = dx * dx + dy * dy - r * r;
            if (diff < 0) {
                adl_point_draw(screen_mat, center_x + dx, center_y + dy, color, offset_zoom_param);
            }
        }
    }
}

/**
 * @brief Draw the outline of a triangle.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param tri Triangle in pixel space (before transform).
 * @param color Stroke color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_tri_draw(Mat2D_uint32 screen_mat, Tri tri, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    adl_line_draw(screen_mat, tri.points[0].x, tri.points[0].y, tri.points[1].x, tri.points[1].y, color, offset_zoom_param);
    adl_line_draw(screen_mat, tri.points[1].x, tri.points[1].y, tri.points[2].x, tri.points[2].y, color, offset_zoom_param);
    adl_line_draw(screen_mat, tri.points[2].x, tri.points[2].y, tri.points[0].x, tri.points[0].y, color, offset_zoom_param);

    // adl_draw_arrow(screen_mat, tri.points[0].x, tri.points[0].y, tri.points[1].x, tri.points[1].y, 0.3, 22, color);
    // adl_draw_arrow(screen_mat, tri.points[1].x, tri.points[1].y, tri.points[2].x, tri.points[2].y, 0.3, 22, color);
    // adl_draw_arrow(screen_mat, tri.points[2].x, tri.points[2].y, tri.points[0].x, tri.points[0].y, 0.3, 22, color);
}

/**
 * @brief Fill a triangle using Pineda's rasterizer with flat base color.
 *
 * Uses the top-left fill convention and performs a depth test using
 * inverse-Z computed from per-vertex z and w.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param inv_z_buffer Inverse-Z buffer (larger is closer).
 * @param tri Triangle in pixel space; points carry z and w for depth.
 * @param color Base color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_tri_fill_Pinedas_rasterizer(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Tri tri, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    /* This function follows the rasterizer of 'Pikuma' shown in his YouTube video. You can fine the video in this link: https://youtu.be/k5wtuKWmV48. */

    Point p0, p1, p2;
    p0 = tri.points[0];
    p1 = tri.points[1];
    p2 = tri.points[2];

    /* finding bounding box */
    int x_min = fmin(p0.x, fmin(p1.x, p2.x));
    int x_max = fmax(p0.x, fmax(p1.x, p2.x));
    int y_min = fmin(p0.y, fmin(p1.y, p2.y));
    int y_max = fmax(p0.y, fmax(p1.y, p2.y));

    /* Clamp to screen bounds */
    if (x_min < 0) x_min = 0;
    if (y_min < 0) y_min = 0;
    if (x_max >= (int)screen_mat.cols) x_max = screen_mat.cols - 1;
    if (y_max >= (int)screen_mat.rows) y_max = screen_mat.rows - 1;

    /* draw only outline of the tri if there is no area */
    float w = edge_cross_point(p0, p1, p1, p2);
    if (fabsf(w) < 1e-6) {
        // adl_tri_draw(screen_mat, tri, tri.colors[0], offset_zoom_param);
        return;
    }
    MATRIX2D_ASSERT(fabsf(w) > 1e-6 && "triangle must have area");

    /* fill conventions */
    int bias0 = is_top_left(p0, p1) ? 0 : -1;
    int bias1 = is_top_left(p1, p2) ? 0 : -1;
    int bias2 = is_top_left(p2, p0) ? 0 : -1;

    for (int y = y_min; y <= y_max; y++) {
        for (int x = x_min; x <= x_max; x++) {
            Point p = {.x = x, .y = y, .z = 0};

            float w0 = edge_cross_point(p0, p1, p0, p) + bias0;
            float w1 = edge_cross_point(p1, p2, p1, p) + bias1;
            float w2 = edge_cross_point(p2, p0, p2, p) + bias2;

            float alpha = fabs(w1 / w);
            float beta  = fabs(w2 / w);
            float gamma = fabs(w0 / w);

            if (w0 * w >= 0 && w1 * w >= 0 &&  w2 * w >= 0) {
                int r, b, g, a;
                HexARGB_RGBA_VAR(color, r, g, b, a);
                float light_intensity = (tri.light_intensity[0] + tri.light_intensity[1] + tri.light_intensity[2]) / 3;
                float rf = r * light_intensity;
                float gf = g * light_intensity;
                float bf = b * light_intensity;
                uint8_t r8 = (uint8_t)fmaxf(0, fminf(255, rf));
                uint8_t g8 = (uint8_t)fmaxf(0, fminf(255, gf));
                uint8_t b8 = (uint8_t)fmaxf(0, fminf(255, bf));

                double inv_w = alpha * (1.0 / p0.w) + beta  * (1.0 / p1.w) + gamma * (1.0 / p2.w);
                double z_over_w = alpha * (p0.z / p0.w) + beta  * (p1.z / p1.w) + gamma * (p2.z / p2.w);
                double inv_z = inv_w / z_over_w;

                if (inv_z >= MAT2D_AT(inv_z_buffer, y, x)) {
                    adl_point_draw(screen_mat, x, y, RGBA_hexARGB(r8, g8, b8, a), offset_zoom_param);
                    MAT2D_AT(inv_z_buffer, y, x) = inv_z;
                }
            }
        }
    }
}

/**
 * @brief Fill a triangle using Pineda's rasterizer with per-vertex colors.
 *
 * Interpolates tri.colors[] and optionally modulates by average
 * light_intensity. Depth-tested.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param inv_z_buffer Inverse-Z buffer (larger is closer).
 * @param tri Triangle in pixel space with colors set.
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_tri_fill_Pinedas_rasterizer_interpolate_color(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Tri tri, Offset_zoom_param offset_zoom_param)
{
    /* This function follows the rasterizer of 'Pikuma' shown in his YouTube video. You can fine the video in this link: https://youtu.be/k5wtuKWmV48. */
    Point p0, p1, p2;
    p0 = tri.points[0];
    p1 = tri.points[1];
    p2 = tri.points[2];

    float w = edge_cross_point(p0, p1, p1, p2);
    if (fabsf(w) < 1e-6) {
        // adl_tri_draw(screen_mat, tri, tri.colors[0], offset_zoom_param);
        return;
    }
    MATRIX2D_ASSERT(w != 0 && "triangle has area");

    /* fill conventions */
    int bias0 = is_top_left(p0, p1) ? 0 : -1;
    int bias1 = is_top_left(p1, p2) ? 0 : -1;
    int bias2 = is_top_left(p2, p0) ? 0 : -1;

    /* finding bounding box */
    int x_min = fmin(p0.x, fmin(p1.x, p2.x));
    int x_max = fmax(p0.x, fmax(p1.x, p2.x));
    int y_min = fmin(p0.y, fmin(p1.y, p2.y));
    int y_max = fmax(p0.y, fmax(p1.y, p2.y));
    // printf("xmin: %d, xmax: %d || ymin: %d, ymax: %d\n", x_min, x_max, y_min, y_max);

    /* Clamp to screen bounds */
    if (x_min < 0) x_min = 0;
    if (y_min < 0) y_min = 0;
    if (x_max >= (int)screen_mat.cols) x_max = screen_mat.cols - 1;
    if (y_max >= (int)screen_mat.rows) y_max = screen_mat.rows - 1;

    for (int y = y_min; y <= y_max; y++) {
        for (int x = x_min; x <= x_max; x++) {
            Point p = {.x = x, .y = y, .z = 0};

            float w0 = edge_cross_point(p0, p1, p0, p) + bias0;
            float w1 = edge_cross_point(p1, p2, p1, p) + bias1;
            float w2 = edge_cross_point(p2, p0, p2, p) + bias2;

            float alpha = fabs(w1 / w);
            float beta  = fabs(w2 / w);
            float gamma = fabs(w0 / w);

            if (w0 * w >= 0 && w1 * w >= 0 &&  w2 * w >= 0) {
                int r0, b0, g0, a0;
                int r1, b1, g1, a1;
                int r2, b2, g2, a2;
                HexARGB_RGBA_VAR(tri.colors[0], r0, g0, b0, a0);
                HexARGB_RGBA_VAR(tri.colors[1], r1, g1, b1, a1);
                HexARGB_RGBA_VAR(tri.colors[2], r2, g2, b2, a2);
                
                uint8_t current_r = r0*alpha + r1*beta + r2*gamma;
                uint8_t current_g = g0*alpha + g1*beta + g2*gamma;
                uint8_t current_b = b0*alpha + b1*beta + b2*gamma;
                uint8_t current_a = a0*alpha + a1*beta + a2*gamma;

                float light_intensity = (tri.light_intensity[0] + tri.light_intensity[1] + tri.light_intensity[2]) / 3;
                float rf = current_r * light_intensity;
                float gf = current_g * light_intensity;
                float bf = current_b * light_intensity;
                uint8_t r8 = (uint8_t)fmaxf(0, fminf(255, rf));
                uint8_t g8 = (uint8_t)fmaxf(0, fminf(255, gf));
                uint8_t b8 = (uint8_t)fmaxf(0, fminf(255, bf));

                double inv_w = alpha * (1.0 / p0.w) + beta  * (1.0 / p1.w) + gamma * (1.0 / p2.w);
                double z_over_w = alpha * (p0.z / p0.w) + beta  * (p1.z / p1.w) + gamma * (p2.z / p2.w);
                double inv_z = inv_w / z_over_w;

                if (inv_z >= MAT2D_AT(inv_z_buffer, y, x)) {
                    adl_point_draw(screen_mat, x, y, RGBA_hexARGB(r8, g8, b8, current_a), offset_zoom_param);
                    MAT2D_AT(inv_z_buffer, y, x) = inv_z;
                }
            }
        }
    }
}

/**
 * @brief Fill a triangle with interpolated lighting over a uniform color.
 *
 * Interpolates light_intensity across the triangle and modulates a
 * uniform base color. Depth-tested.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param inv_z_buffer Inverse-Z buffer (larger is closer).
 * @param tri Triangle in pixel space; points carry z and w for depth.
 * @param color Base color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_tri_fill_Pinedas_rasterizer_interpolate_normal(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Tri tri, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    /* This function follows the rasterizer of 'Pikuma' shown in his YouTube video. You can fine the video in this link: https://youtu.be/k5wtuKWmV48. */
    Point p0, p1, p2;
    p0 = tri.points[0];
    p1 = tri.points[1];
    p2 = tri.points[2];

    float w = edge_cross_point(p0, p1, p1, p2);
    if (fabsf(w) < 1e-6) {
        // adl_tri_draw(screen_mat, tri, tri.colors[0], offset_zoom_param);
        return;
    }
    MATRIX2D_ASSERT(w != 0 && "triangle has area");

    /* fill conventions */
    int bias0 = is_top_left(p0, p1) ? 0 : -1;
    int bias1 = is_top_left(p1, p2) ? 0 : -1;
    int bias2 = is_top_left(p2, p0) ? 0 : -1;

    /* finding bounding box */
    int x_min = fmin(p0.x, fmin(p1.x, p2.x));
    int x_max = fmax(p0.x, fmax(p1.x, p2.x));
    int y_min = fmin(p0.y, fmin(p1.y, p2.y));
    int y_max = fmax(p0.y, fmax(p1.y, p2.y));
    // printf("xmin: %d, xmax: %d || ymin: %d, ymax: %d\n", x_min, x_max, y_min, y_max);

    /* Clamp to screen bounds */
    if (x_min < 0) x_min = 0;
    if (y_min < 0) y_min = 0;
    if (x_max >= (int)screen_mat.cols) x_max = screen_mat.cols - 1;
    if (y_max >= (int)screen_mat.rows) y_max = screen_mat.rows - 1;

    int r, b, g, a;
    HexARGB_RGBA_VAR(color, r, g, b, a);

    for (int y = y_min; y <= y_max; y++) {
        for (int x = x_min; x <= x_max; x++) {
            Point p = {.x = x, .y = y, .z = 0};

            float w0 = edge_cross_point(p0, p1, p0, p) + bias0;
            float w1 = edge_cross_point(p1, p2, p1, p) + bias1;
            float w2 = edge_cross_point(p2, p0, p2, p) + bias2;

            float alpha = fabs(w1 / w);
            float beta  = fabs(w2 / w);
            float gamma = fabs(w0 / w);

            if (w0 * w >= 0 && w1 * w >= 0 &&  w2 * w >= 0) {
                
                float light_intensity = tri.light_intensity[0]*alpha + tri.light_intensity[1]*beta + tri.light_intensity[2]*gamma;

                float rf = r * light_intensity;
                float gf = g * light_intensity;
                float bf = b * light_intensity;
                uint8_t r8 = (uint8_t)fmaxf(0, fminf(255, rf));
                uint8_t g8 = (uint8_t)fmaxf(0, fminf(255, gf));
                uint8_t b8 = (uint8_t)fmaxf(0, fminf(255, bf));

                double inv_w = alpha * (1.0 / p0.w) + beta  * (1.0 / p1.w) + gamma * (1.0 / p2.w);
                double z_over_w = alpha * (p0.z / p0.w) + beta  * (p1.z / p1.w) + gamma * (p2.z / p2.w);
                double inv_z = inv_w / z_over_w;

                if (inv_z >= MAT2D_AT(inv_z_buffer, y, x)) {
                    adl_point_draw(screen_mat, x, y, RGBA_hexARGB(r8, g8, b8, a), offset_zoom_param);
                    MAT2D_AT(inv_z_buffer, y, x) = inv_z;
                }
            }
        }
    }
}

/**
 * @brief Draw outlines for all triangles in a mesh.
 *
 * Skips elements with to_draw == false.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param mesh Triangle mesh (array + length).
 * @param color Stroke color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_tri_mesh_draw(Mat2D_uint32 screen_mat, Tri_mesh mesh, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Tri tri = mesh.elements[i];
        if (tri.to_draw) {
            // color = rand_double() * 0xFFFFFFFF;
            adl_tri_draw(screen_mat, tri, color, offset_zoom_param);
        }
    }
}

/**
 * @brief Fill all triangles in a mesh with a uniform base color.
 *
 * Applies average light_intensity per triangle. Depth-tested.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param inv_z_buffer_mat Inverse-Z buffer (larger is closer).
 * @param mesh Triangle mesh (array + length).
 * @param color Base color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_tri_mesh_fill_Pinedas_rasterizer(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Tri_mesh mesh, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Tri tri = mesh.elements[i];
        /* Reject invalid triangles */
        adl_assert_tri_is_valid(tri);

        if (!tri.to_draw) continue;

        adl_tri_fill_Pinedas_rasterizer(screen_mat, inv_z_buffer_mat, tri, color, offset_zoom_param);
    }
}

/**
 * @brief Fill all triangles in a mesh with a uniform base color.
 *
 * Applies average light_intensity per triangle. Depth-tested.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param inv_z_buffer_mat Inverse-Z buffer (larger is closer).
 * @param mesh Triangle mesh (array + length).
 * @param color Base color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_tri_mesh_fill_Pinedas_rasterizer_interpolate_color(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Tri_mesh mesh, Offset_zoom_param offset_zoom_param)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Tri tri = mesh.elements[i];
        /* Reject invalid triangles */
        adl_assert_tri_is_valid(tri);

        if (!tri.to_draw) continue;

        adl_tri_fill_Pinedas_rasterizer_interpolate_color(screen_mat, inv_z_buffer_mat, tri, offset_zoom_param);
    }
}

/**
 * @brief Fill all triangles in a mesh with interpolated lighting.
 *
 * Interpolates light_intensity across each triangle and modulates a
 * uniform base color. Depth-tested.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param inv_z_buffer_mat Inverse-Z buffer (larger is closer).
 * @param mesh Triangle mesh (array + length).
 * @param color Base color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_tri_mesh_fill_Pinedas_rasterizer_interpolate_normal(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Tri_mesh mesh, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Tri tri = mesh.elements[i];
        /* Reject invalid triangles */
        adl_assert_tri_is_valid(tri);

        if (!tri.to_draw) continue;

        adl_tri_fill_Pinedas_rasterizer_interpolate_normal(screen_mat, inv_z_buffer_mat, tri, color, offset_zoom_param);
    }
}

/**
 * @brief Compute tan(alpha/2) for the angle at point p between segments p->vi
 *        and p->vj.
 *
 * Uses the identity tan(alpha/2) = |a×b| / (|a||b| + a·b), where a = vi - p
 * and b = vj - p. The lengths li = |a| and lj = |b| are passed in to
 * avoid recomputation.
 *
 * @param vi Vertex i.
 * @param vj Vertex j.
 * @param p Pivot point.
 * @param li Precomputed |vi - p|.
 * @param lj Precomputed |vj - p|.
 * @return tan(alpha/2) (non-negative).
 */
float adl_tan_half_angle(Point vi, Point vj, Point p, float li, float lj)
{
    float ax = vi.x - p.x, ay = vi.y - p.y;
    float bx = vj.x - p.x, by = vj.y - p.y;
    float dot = ax * bx + ay * by;
    float cross = ax * by - ay * bx;              // signed 2D cross (scalar)
    float denom = dot + li * lj;                   // = |a||b|(1 + cos(alpha))
    return fabsf(cross) / fmaxf(1e-20f, denom);    // tan(alpha/2)
}

/**
 * @brief Affine map from one scalar range to another (no clamping).
 *
 * @param s Input value.
 * @param min_in Input range minimum.
 * @param max_in Input range maximum.
 * @param min_out Output range minimum.
 * @param max_out Output range maximum.
 * @return Mapped value in the output range (may exceed if s is out-of-range).
 */
float adl_linear_map(float s, float min_in, float max_in, float min_out, float max_out)
{
    return (min_out + ((s-min_in)*(max_out-min_out))/(max_in-min_in));
}

/**
 * @brief Split a quad into two triangles along a chosen diagonal.
 *
 * The split is controlled by split_line:
 * - "02" splits along diagonal from vertex 0 to vertex 2.
 * - "13" splits along diagonal from vertex 1 to vertex 3.
 *
 * The function copies positions, per-vertex colors, light_intensity, and
 * the to_draw flag into the output triangles.
 *
 * @param quad Input quad.
 * @param tri1 [out] First output triangle.
 * @param tri2 [out] Second output triangle.
 * @param split_line Null-terminated code: "02" or "13".
 */
void adl_quad2tris(Quad quad, Tri *tri1, Tri *tri2, char split_line[])
{
    if (!strncmp(split_line, "02", 2)) {
        tri1->points[0] = quad.points[0];
        tri1->points[1] = quad.points[1];
        tri1->points[2] = quad.points[2];
        tri1->to_draw = quad.to_draw;
        tri1->light_intensity[0] = quad.light_intensity[0];
        tri1->light_intensity[1] = quad.light_intensity[1];
        tri1->light_intensity[2] = quad.light_intensity[2];
        tri1->colors[0] = quad.colors[0];
        tri1->colors[1] = quad.colors[1];
        tri1->colors[2] = quad.colors[2];

        tri2->points[0] = quad.points[2];
        tri2->points[1] = quad.points[3];
        tri2->points[2] = quad.points[0];
        tri2->to_draw = quad.to_draw;
        tri1->light_intensity[0] = quad.light_intensity[2];
        tri1->light_intensity[1] = quad.light_intensity[3];
        tri1->light_intensity[2] = quad.light_intensity[0];
        tri2->colors[0] = quad.colors[2];
        tri2->colors[1] = quad.colors[3];
        tri2->colors[2] = quad.colors[0];
    } else if (!strncmp(split_line, "13", 2)) {
        tri1->points[0] = quad.points[1];
        tri1->points[1] = quad.points[2];
        tri1->points[2] = quad.points[3];
        tri1->to_draw = quad.to_draw;
        tri1->light_intensity[0] = quad.light_intensity[1];
        tri1->light_intensity[1] = quad.light_intensity[2];
        tri1->light_intensity[2] = quad.light_intensity[3];
        tri1->colors[0] = quad.colors[1];
        tri1->colors[1] = quad.colors[2];
        tri1->colors[2] = quad.colors[3];

        tri2->points[0] = quad.points[3];
        tri2->points[1] = quad.points[0];
        tri2->points[2] = quad.points[1];
        tri2->to_draw = quad.to_draw;
        tri1->light_intensity[0] = quad.light_intensity[3];
        tri1->light_intensity[1] = quad.light_intensity[0];
        tri1->light_intensity[2] = quad.light_intensity[1];
        tri2->colors[0] = quad.colors[3];
        tri2->colors[1] = quad.colors[0];
        tri2->colors[2] = quad.colors[1];
    }
}

/**
 * @brief Convert a linear sRGB color (ARGB) to Oklab components.
 *
 * Oklab components are returned in ranges: L in [0,1], a in [-0.5,0.5],
 * b in [-0.5,0.5] (typical). Input is assumed to be linear sRGB.
 *
 * @param hex_ARGB Input color (0xAARRGGBB). Alpha is ignored.
 * @param L [out] Perceptual lightness.
 * @param a [out] First opponent axis.
 * @param b [out] Second opponent axis.
 */
void adl_linear_sRGB_to_okLab(uint32_t hex_ARGB, float *L, float *a, float *b)
{
    /* https://bottosson.github.io/posts/oklab/
       https://en.wikipedia.org/wiki/Oklab_color_space */
    int R_255, G_255, B_255;
    HexARGB_RGB_VAR(hex_ARGB, R_255, G_255, B_255);

    float R = R_255;
    float G = G_255;
    float B = B_255;

    float l = 0.4122214705f * R + 0.5363325363f * G + 0.0514459929f * B;
    float m = 0.2119034982f * R + 0.6806995451f * G + 0.1073969566f * B;
    float s = 0.0883024619f * R + 0.2817188376f * G + 0.6299787005f * B;

    float l_ = cbrtf(l);
    float m_ = cbrtf(m);
    float s_ = cbrtf(s);

    *L = 0.2104542553f * l_ + 0.7936177850f * m_ - 0.0040720468f * s_;
    *a = 1.9779984951f * l_ - 2.4285922050f * m_ + 0.4505937099f * s_;
    *b = 0.0259040371f * l_ + 0.7827717662f * m_ - 0.8086757660f * s_;

}

/**
 * @brief Convert Oklab components to a linear sRGB ARGB color.
 *
 * Output RGB components are clamped to [0,255], alpha is set to 255.
 *
 * @param L Oklab lightness.
 * @param a Oklab a component.
 * @param b Oklab b component.
 * @param hex_ARGB [out] Output color (0xAARRGGBB, A=255).
 */
void adl_okLab_to_linear_sRGB(float L, float a, float b, uint32_t *hex_ARGB)
{
    /* https://bottosson.github.io/posts/oklab/
       https://en.wikipedia.org/wiki/Oklab_color_space */

    float l_ = L + 0.3963377774f * a + 0.2158037573f * b;
    float m_ = L - 0.1055613458f * a - 0.0638541728f * b;
    float s_ = L - 0.0894841775f * a - 1.2914855480f * b;

    float l = l_ * l_ * l_;
    float m = m_ * m_ * m_;
    float s = s_ * s_ * s_;

    float R = + 4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s;
    float G = - 1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s;
    float B = - 0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s;

    R = fmaxf(fminf(R, 255), 0);
    G = fmaxf(fminf(G, 255), 0);
    B = fmaxf(fminf(B, 255), 0);

    *hex_ARGB = RGBA_hexARGB(R, G, B, 0xFF);
}

/**
 * @brief Convert a linear sRGB color (ARGB) to OkLch components.
 *
 * @param hex_ARGB Input color (0xAARRGGBB). Alpha is ignored.
 * @param L [out] Lightness in [0,1].
 * @param c [out] Chroma (non-negative).
 * @param h_deg [out] Hue angle in degrees [-180,180] from atan2.
 */
void adl_linear_sRGB_to_okLch(uint32_t hex_ARGB, float *L, float *c, float *h_deg)
{
    float a, b;
    adl_linear_sRGB_to_okLab(hex_ARGB, L, &a, &b);

    *c = sqrtf(a * a + b * b);
    *h_deg = atan2f(b, a) * 180 / PI;
}

/**
 * @brief Convert OkLch components to a linear sRGB ARGB color.
 *
 * Hue is wrapped to [0,360). Output RGB is clamped to [0,255], alpha=255.
 *
 * @param L Lightness.
 * @param c Chroma.
 * @param h_deg Hue angle in degrees.
 * @param hex_ARGB [out] Output color (0xAARRGGBB, A=255).
 */
void adl_okLch_to_linear_sRGB(float L, float c, float h_deg, uint32_t *hex_ARGB)
{
    h_deg = fmodf((h_deg + 360), 360);
    float a = c * cosf(h_deg * PI / 180);
    float b = c * sinf(h_deg * PI / 180);
    adl_okLab_to_linear_sRGB(L, a, b, hex_ARGB);
}

/**
 * @brief Interpolate between two ARGB colors in OkLch space.
 *
 * Lightness and chroma are interpolated linearly. Hue is interpolated in
 * degrees after adding 360*num_of_rotations to the second hue, allowing
 * control over the winding direction.
 *
 * @param color1 Start color (0xAARRGGBB).
 * @param color2 End color (0xAARRGGBB).
 * @param t Interpolation factor in [0,1].
 * @param num_of_rotations Number of hue turns to add to color2 (can be
 *        fractional/negative).
 * @param color_out [out] Interpolated ARGB color (A=255).
 */
void adl_interpolate_ARGBcolor_on_okLch(uint32_t color1, uint32_t color2, float t, float num_of_rotations, uint32_t *color_out)
{
    float L_1, c_1, h_1;
    float L_2, c_2, h_2;
    adl_linear_sRGB_to_okLch(color1, &L_1, &c_1, &h_1);
    adl_linear_sRGB_to_okLch(color2, &L_2, &c_2, &h_2);
    h_2 = h_2 + 360 * num_of_rotations;

    float L, c, h;
    L = L_1 * (1 - t) + L_2 * (t);
    c = c_1 * (1 - t) + c_2 * (t);
    h = h_1 * (1 - t) + h_2 * (t);
    adl_okLch_to_linear_sRGB(L, c, h, color_out);
}

/**
 * @brief Allocate and initialize a Figure with an internal pixel buffer.
 *
 * Initializes the pixel buffer (rows x cols), an inverse-Z buffer (zeroed),
 * an empty source curve array, and default padding/axes bounds. The
 * background_color, to_draw_axis, and to_draw_max_min_values should be
 * set by the caller before rendering.
 *
 * @param rows Height of the figure in pixels.
 * @param cols Width of the figure in pixels.
 * @param top_left_position Target position when copying to a screen.
 * @return A new Figure with allocated buffers.
 */
Figure adl_figure_alloc(size_t rows, size_t cols, Point top_left_position)
{
    ADL_ASSERT(rows && cols);
    adl_assert_point_is_valid(top_left_position);

    Figure figure = {0};
    figure.pixels_mat = mat2D_alloc_uint32(rows, cols);
    figure.inv_z_buffer_mat = mat2D_alloc(rows, cols);
    memset(figure.inv_z_buffer_mat.elements, 0x0, sizeof(double) * figure.inv_z_buffer_mat.rows * figure.inv_z_buffer_mat.cols);
    ada_init_array(Curve, figure.src_curve_array);

    figure.top_left_position = top_left_position;

    int max_i    = (int)(figure.pixels_mat.rows);
    int max_j    = (int)(figure.pixels_mat.cols);
    int offset_i = (int)fminf(figure.pixels_mat.rows * ADL_FIGURE_PADDING_PRECENTAGE / 100.0f, ADL_MAX_FIGURE_PADDING);
    int offset_j = (int)fminf(figure.pixels_mat.cols * ADL_FIGURE_PADDING_PRECENTAGE / 100.0f, ADL_MAX_FIGURE_PADDING);

    figure.min_x_pixel = offset_j;
    figure.max_x_pixel = max_j - offset_j;
    figure.min_y_pixel = offset_i;
    figure.max_y_pixel = max_i - offset_i;

    figure.min_x = + FLT_MAX;
    figure.max_x = - FLT_MAX;
    figure.min_y = + FLT_MAX;
    figure.max_y = - FLT_MAX;

    figure.offset_zoom_param = ADL_DEFAULT_OFFSET_ZOOM;

    return figure;
}

/**
 * @brief Blit a Figure's pixels onto a destination screen buffer.
 *
 * Performs per-pixel blending using adl_point_draw and the identity
 * transform. The figure's top_left_position is used as the destination
 * offset.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param figure Source figure to copy from.
 */
void adl_figure_copy_to_screen(Mat2D_uint32 screen_mat, Figure figure)
{
    for (size_t i = 0; i < figure.pixels_mat.rows; i++) {
        for (size_t j = 0; j < figure.pixels_mat.cols; j++) {
            int offset_i = figure.top_left_position.y;
            int offset_j = figure.top_left_position.x;
            
            adl_point_draw(screen_mat, offset_j+j, offset_i+i, MAT2D_AT_UINT32(figure.pixels_mat, i, j), (Offset_zoom_param){1,0,0,0,0});
        }
    }
}

/**
 * @brief Draw X/Y axes with arrowheads into a Figure.
 *
 * Uses the current figure's pixel extents and padding to place axes, and
 * stores the computed head sizes for later label layout.
 *
 * @param figure [in,out] Figure to draw onto.
 */
void adl_axis_draw_on_figure(Figure *figure)
{
    int max_i    = (int)(figure->pixels_mat.rows);
    int max_j    = (int)(figure->pixels_mat.cols);
    int offset_i = (int)fmaxf(fminf(figure->pixels_mat.rows * ADL_FIGURE_PADDING_PRECENTAGE / 100.0f, ADL_MAX_FIGURE_PADDING), ADL_MIN_FIGURE_PADDING);
    int offset_j = (int)fmaxf(fminf(figure->pixels_mat.cols * ADL_FIGURE_PADDING_PRECENTAGE / 100.0f, ADL_MAX_FIGURE_PADDING), ADL_MIN_FIGURE_PADDING);

    int arrow_head_size_x = (int)fminf(ADL_MAX_HEAD_SIZE, ADL_FIGURE_PADDING_PRECENTAGE / 100.0f * (max_j - 2 * offset_j));
    int arrow_head_size_y = (int)fminf(ADL_MAX_HEAD_SIZE, ADL_FIGURE_PADDING_PRECENTAGE / 100.0f * (max_i - 2 * offset_i));

    adl_arrow_draw(figure->pixels_mat, figure->min_x_pixel, figure->max_y_pixel, figure->max_x_pixel, figure->max_y_pixel, (float)arrow_head_size_x / (max_j-2*offset_j), ADL_FIGURE_HEAD_ANGLE_DEG, ADL_FIGURE_AXIS_COLOR, figure->offset_zoom_param);
    adl_arrow_draw(figure->pixels_mat, figure->min_x_pixel, figure->max_y_pixel, figure->min_x_pixel, figure->min_y_pixel, (float)arrow_head_size_y / (max_i-2*offset_i), ADL_FIGURE_HEAD_ANGLE_DEG, ADL_FIGURE_AXIS_COLOR, figure->offset_zoom_param);
    // adl_draw_rectangle_min_max(figure->pixels_mat, figure->min_x_pixel, figure->max_x_pixel, figure->min_y_pixel, figure->max_y_pixel, 0);

    figure->x_axis_head_size = arrow_head_size_x;
    figure->y_axis_head_size = arrow_head_size_y;
}

/**
 * @brief Draw min/max numeric labels for the current data range.
 *
 * Renders textual min/max values for both axes inside the figure area.
 * Assumes figure.min_x/max_x/min_y/max_y have been populated.
 *
 * @param figure Figure whose labels are drawn into its own pixel buffer.
 */
void adl_max_min_values_draw_on_figure(Figure figure)
{
    char x_min_sentence[256];
    char x_max_sentence[256];
    snprintf(x_min_sentence, 256, "%g", figure.min_x);
    snprintf(x_max_sentence, 256, "%g", figure.max_x);

    int x_sentence_hight_pixel = (figure.pixels_mat.rows - figure.max_y_pixel - ADL_MIN_CHARACTER_OFFSET * 3);
    int x_min_char_width_pixel = x_sentence_hight_pixel / 2;
    int x_max_char_width_pixel = x_sentence_hight_pixel / 2;

    int x_min_sentence_width_pixel = (int)fminf((figure.max_x_pixel - figure.min_x_pixel)/2, (x_min_char_width_pixel + ADL_MAX_CHARACTER_OFFSET)*strlen(x_min_sentence));
    x_min_char_width_pixel = x_min_sentence_width_pixel / strlen(x_min_sentence) - ADL_MIN_CHARACTER_OFFSET;

    int x_max_sentence_width_pixel = (int)fminf((figure.max_x_pixel - figure.min_x_pixel)/2, (x_max_char_width_pixel + ADL_MAX_CHARACTER_OFFSET)*strlen(x_max_sentence)) - figure.x_axis_head_size;
    x_max_char_width_pixel = (x_max_sentence_width_pixel + figure.x_axis_head_size) / strlen(x_max_sentence) - ADL_MIN_CHARACTER_OFFSET;

    int x_min_sentence_hight_pixel = (int)fminf(x_min_char_width_pixel * 2, x_sentence_hight_pixel);
    int x_max_sentence_hight_pixel = (int)fminf(x_max_char_width_pixel * 2, x_sentence_hight_pixel);

    x_min_sentence_hight_pixel = (int)fminf(x_min_sentence_hight_pixel, x_max_sentence_hight_pixel);
    x_max_sentence_hight_pixel = x_min_sentence_hight_pixel;

    int x_max_x_top_left = figure.max_x_pixel - strlen(x_max_sentence) * (x_max_sentence_hight_pixel / 2 + ADL_MIN_CHARACTER_OFFSET) - figure.x_axis_head_size;

    adl_sentence_draw(figure.pixels_mat, x_min_sentence, strlen(x_min_sentence), figure.min_x_pixel, figure.max_y_pixel+ADL_MIN_CHARACTER_OFFSET*2, x_min_sentence_hight_pixel, ADL_FIGURE_AXIS_COLOR, figure.offset_zoom_param);
    adl_sentence_draw(figure.pixels_mat, x_max_sentence, strlen(x_max_sentence), x_max_x_top_left, figure.max_y_pixel+ADL_MIN_CHARACTER_OFFSET*2, x_max_sentence_hight_pixel, ADL_FIGURE_AXIS_COLOR, figure.offset_zoom_param);
    
    char y_min_sentence[256];
    char y_max_sentence[256];
    snprintf(y_min_sentence, 256, "%g", figure.min_y);
    snprintf(y_max_sentence, 256, "%g", figure.max_y);

    int y_sentence_width_pixel = figure.min_x_pixel - ADL_MAX_CHARACTER_OFFSET - figure.y_axis_head_size;
    int y_max_char_width_pixel = y_sentence_width_pixel;
    y_max_char_width_pixel /= strlen(y_max_sentence);
    int y_max_sentence_hight_pixel = y_max_char_width_pixel * 2;

    int y_min_char_width_pixel = y_sentence_width_pixel;
    y_min_char_width_pixel /= strlen(y_min_sentence);
    int y_min_sentence_hight_pixel = y_min_char_width_pixel * 2;

    y_min_sentence_hight_pixel = (int)fmaxf(fminf(y_min_sentence_hight_pixel, y_max_sentence_hight_pixel), 1);
    y_max_sentence_hight_pixel = y_min_sentence_hight_pixel;

    adl_sentence_draw(figure.pixels_mat, y_max_sentence, strlen(y_max_sentence), ADL_MAX_CHARACTER_OFFSET/2, figure.min_y_pixel, y_max_sentence_hight_pixel, ADL_FIGURE_AXIS_COLOR, figure.offset_zoom_param);
    adl_sentence_draw(figure.pixels_mat, y_min_sentence, strlen(y_min_sentence), ADL_MAX_CHARACTER_OFFSET/2, figure.max_y_pixel-y_min_sentence_hight_pixel, y_min_sentence_hight_pixel, ADL_FIGURE_AXIS_COLOR, figure.offset_zoom_param);
}

/**
 * @brief Add a curve (polyline) to a Figure and update its data bounds.
 *
 * The input points are copied into the figure's source curve array with
 * the given color. Figure min/max bounds are updated to include them.
 *
 * @param figure [in,out] Target figure.
 * @param src_points Array of source points (in data space).
 * @param src_len Number of points.
 * @param color Curve color (0xAARRGGBB).
 */
void adl_curve_add_to_figure(Figure *figure, Point *src_points, size_t src_len, uint32_t color)
{
    Curve src_points_ada;
    ada_init_array(Point, src_points_ada);
    src_points_ada.color = color;

    for (size_t i = 0; i < src_len; i++) {
        Point current_point = src_points[i];
        if (current_point.x > figure->max_x) {
            figure->max_x = current_point.x;
        }
        if (current_point.y > figure->max_y) {
            figure->max_y = current_point.y;
        }
        if (current_point.x < figure->min_x) {
            figure->min_x = current_point.x;
        }
        if (current_point.y < figure->min_y) {
            figure->min_y = current_point.y;
        }
        ada_appand(Point, src_points_ada, current_point);
    }
    
    ada_appand(Curve, figure->src_curve_array, src_points_ada);
}

/**
 * @brief Render all added curves into a Figure's pixel buffer.
 *
 * Clears the pixel buffer to background_color, draws axes if enabled, maps
 * data-space points to pixel-space using current min/max bounds, draws the
 * polylines, and optionally draws min/max labels.
 *
 * @param figure Figure to render into (uses its own pixel buffer).
 */
void adl_curves_plot_on_figure(Figure figure)
{
    mat2D_fill_uint32(figure.pixels_mat, figure.background_color);
    memset(figure.inv_z_buffer_mat.elements, 0x0, sizeof(double) * figure.inv_z_buffer_mat.rows * figure.inv_z_buffer_mat.cols);
    if (figure.to_draw_axis) adl_axis_draw_on_figure(&figure);

    for (size_t curve_index = 0; curve_index < figure.src_curve_array.length; curve_index++) {
        size_t src_len = figure.src_curve_array.elements[curve_index].length;
        Point *src_points = figure.src_curve_array.elements[curve_index].elements;
        for (size_t i = 0; i < src_len-1; i++) {
            Point src_start = src_points[i];
            Point src_end   = src_points[i+1];
            Point des_start = {0};
            Point des_end = {0};

            des_start.x = adl_linear_map(src_start.x, figure.min_x, figure.max_x, figure.min_x_pixel, figure.max_x_pixel);
            des_start.y = ((figure.max_y_pixel + figure.min_y_pixel) - adl_linear_map(src_start.y, figure.min_y, figure.max_y, figure.min_y_pixel, figure.max_y_pixel));

            des_end.x = adl_linear_map(src_end.x, figure.min_x, figure.max_x, figure.min_x_pixel, figure.max_x_pixel);
            des_end.y = ((figure.max_y_pixel + figure.min_y_pixel) - adl_linear_map(src_end.y, figure.min_y, figure.max_y, figure.min_y_pixel, figure.max_y_pixel));

            adl_line_draw(figure.pixels_mat, des_start.x, des_start.y, des_end.x, des_end.y, figure.src_curve_array.elements[curve_index].color, figure.offset_zoom_param);
        }
    }

    if (figure.to_draw_max_min_values) adl_max_min_values_draw_on_figure(figure);
}

/* check offset2D. might convert it to a Mat2D */
#define adl_offset2d(i, j, ni) (j) * (ni) + (i)
/**
 * @brief Visualize a scalar field on a Figure by colored quads.
 *
 * Treats x_2Dmat and y_2Dmat as a structured 2D grid of positions
 * (column-major with stride ni) and colors each cell using scalar_2Dmat
 * mapped through a two-color OkLch gradient. Also updates figure bounds
 * from the provided data. Depth-tested inside the figure's buffers.
 *
 * @param figure Figure to render into (uses its own pixel buffers).
 * @param x_2Dmat Grid X coordinates, size ni*nj.
 * @param y_2Dmat Grid Y coordinates, size ni*nj.
 * @param scalar_2Dmat Scalar values per grid node, size ni*nj.
 * @param ni Number of samples along the first index (rows).
 * @param nj Number of samples along the second index (cols).
 * @param color_scale Two-letter code of endpoints ("b-c","b-g","b-r",
 *        "b-y","g-y","g-p","g-r","r-y").
 * @param num_of_rotations Hue turns for the OkLch interpolation (can be
 *        fractional/negative).
 */
void adl_2Dscalar_interp_on_figure(Figure figure, double *x_2Dmat, double *y_2Dmat, double *scalar_2Dmat, int ni, int nj, char color_scale[], float num_of_rotations)
{
    mat2D_fill_uint32(figure.pixels_mat, figure.background_color);
    memset(figure.inv_z_buffer_mat.elements, 0x0, sizeof(double) * figure.inv_z_buffer_mat.rows * figure.inv_z_buffer_mat.cols);
    if (figure.to_draw_axis) adl_axis_draw_on_figure(&figure);

    float min_scalar = FLT_MAX; 
    float max_scalar = FLT_MIN; 
    for (int i = 0; i < ni; i++) {
        for (int j = 0; j < nj; j++) {
            float val = scalar_2Dmat[adl_offset2d(i, j, ni)];
            if (val > max_scalar) max_scalar = val;
            if (val < min_scalar) min_scalar = val;
            float current_x = x_2Dmat[adl_offset2d(i, j, ni)];
            float current_y = y_2Dmat[adl_offset2d(i, j, ni)];
            if (current_x > figure.max_x) {
                figure.max_x = current_x;
            }
            if (current_y > figure.max_y) {
                figure.max_y = current_y;
            }
            if (current_x < figure.min_x) {
                figure.min_x = current_x;
            }
            if (current_y < figure.min_y) {
                figure.min_y = current_y;
            }
        }
    }

    float window_w = (float)figure.pixels_mat.cols;
    float window_h = (float)figure.pixels_mat.rows;

    for (int i = 0; i < ni-1; i++) {
        for (int j = 0; j < nj-1; j++) {
            Quad quad = {0};
            quad.light_intensity[0] = 1;
            quad.light_intensity[1] = 1;
            quad.light_intensity[2] = 1;
            quad.light_intensity[3] = 1;
            quad.to_draw = 1;

            quad.points[3].x = x_2Dmat[adl_offset2d(i  , j  , ni)];
            quad.points[3].y = y_2Dmat[adl_offset2d(i  , j  , ni)];
            quad.points[2].x = x_2Dmat[adl_offset2d(i+1, j  , ni)];
            quad.points[2].y = y_2Dmat[adl_offset2d(i+1, j  , ni)];
            quad.points[1].x = x_2Dmat[adl_offset2d(i+1, j+1, ni)];
            quad.points[1].y = y_2Dmat[adl_offset2d(i+1, j+1, ni)];
            quad.points[0].x = x_2Dmat[adl_offset2d(i  , j+1, ni)];
            quad.points[0].y = y_2Dmat[adl_offset2d(i  , j+1, ni)];

            for (int p_index = 0; p_index < 4; p_index++) {
                quad.points[p_index].z = 1;
                quad.points[p_index].w = 1;
                quad.points[p_index].x = adl_linear_map(quad.points[p_index].x, figure.min_x, figure.max_x, figure.min_x_pixel, figure.max_x_pixel);
                quad.points[p_index].y = ((figure.max_y_pixel + figure.min_y_pixel) - adl_linear_map(quad.points[p_index].y, figure.min_y, figure.max_y, figure.min_y_pixel, figure.max_y_pixel));

                adl_offset_zoom_point(quad.points[p_index], window_w, window_h, figure.offset_zoom_param);
            }

            float t3 = adl_linear_map(scalar_2Dmat[adl_offset2d(i  , j  , ni)], min_scalar, max_scalar, 0, 1);
            float t2 = adl_linear_map(scalar_2Dmat[adl_offset2d(i+1, j  , ni)], min_scalar, max_scalar, 0, 1);
            float t1 = adl_linear_map(scalar_2Dmat[adl_offset2d(i+1, j+1, ni)], min_scalar, max_scalar, 0, 1);
            float t0 = adl_linear_map(scalar_2Dmat[adl_offset2d(  i, j+1, ni)], min_scalar, max_scalar, 0, 1);

            /* https://en.wikipedia.org/wiki/Oklab_color_space */
            if (!strcmp(color_scale, "b-c")) {
                uint32_t color = 0, color1 = BLUE_hexARGB, color2 = CYAN_hexARGB;
                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t0, num_of_rotations, &color);
                quad.colors[0] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t1, num_of_rotations, &color);
                quad.colors[1] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t2, num_of_rotations, &color);
                quad.colors[2] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t3, num_of_rotations, &color);
                quad.colors[3] = color;
            } else if (!strcmp(color_scale, "b-g")) {
                uint32_t color = 0, color1 = BLUE_hexARGB, color2 = GREEN_hexARGB;
                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t0, num_of_rotations, &color);
                quad.colors[0] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t1, num_of_rotations, &color);
                quad.colors[1] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t2, num_of_rotations, &color);
                quad.colors[2] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t3, num_of_rotations, &color);
                quad.colors[3] = color;
            } else if (!strcmp(color_scale, "b-r")) {
                uint32_t color = 0, color1 = BLUE_hexARGB, color2 = RED_hexARGB;
                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t0, num_of_rotations, &color);
                quad.colors[0] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t1, num_of_rotations, &color);
                quad.colors[1] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t2, num_of_rotations, &color);
                quad.colors[2] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t3, num_of_rotations, &color);
                quad.colors[3] = color;
            } else if (!strcmp(color_scale, "b-y")) {
                uint32_t color = 0, color1 = BLUE_hexARGB, color2 = YELLOW_hexARGB;
                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t0, num_of_rotations, &color);
                quad.colors[0] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t1, num_of_rotations, &color);
                quad.colors[1] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t2, num_of_rotations, &color);
                quad.colors[2] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t3, num_of_rotations, &color);
                quad.colors[3] = color;
            } else if (!strcmp(color_scale, "g-y")) {
                uint32_t color = 0, color1 = GREEN_hexARGB, color2 = YELLOW_hexARGB;
                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t0, num_of_rotations, &color);
                quad.colors[0] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t1, num_of_rotations, &color);
                quad.colors[1] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t2, num_of_rotations, &color);
                quad.colors[2] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t3, num_of_rotations, &color);
                quad.colors[3] = color;
            } else if (!strcmp(color_scale, "g-p")) {
                uint32_t color = 0, color1 = GREEN_hexARGB, color2 = PURPLE_hexARGB;
                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t0, num_of_rotations, &color);
                quad.colors[0] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t1, num_of_rotations, &color);
                quad.colors[1] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t2, num_of_rotations, &color);
                quad.colors[2] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t3, num_of_rotations, &color);
                quad.colors[3] = color;
            } else if (!strcmp(color_scale, "g-r")) {
                uint32_t color = 0, color1 = GREEN_hexARGB, color2 = RED_hexARGB;
                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t0, num_of_rotations, &color);
                quad.colors[0] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t1, num_of_rotations, &color);
                quad.colors[1] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t2, num_of_rotations, &color);
                quad.colors[2] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t3, num_of_rotations, &color);
                quad.colors[3] = color;
            } else if (!strcmp(color_scale, "r-y")) {
                uint32_t color = 0, color1 = RED_hexARGB, color2 = YELLOW_hexARGB;
                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t0, num_of_rotations, &color);
                quad.colors[0] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t1, num_of_rotations, &color);
                quad.colors[1] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t2, num_of_rotations, &color);
                quad.colors[2] = color;

                adl_interpolate_ARGBcolor_on_okLch(color1, color2, t3, num_of_rotations, &color);
                quad.colors[3] = color;
            }

            adl_quad_fill_interpolate_color_mean_value(figure.pixels_mat, figure.inv_z_buffer_mat, quad, ADL_DEFAULT_OFFSET_ZOOM); 
        }
    }

    if (figure.to_draw_max_min_values) {
        adl_max_min_values_draw_on_figure(figure);
    }

}

/**
 * @brief Create a Cartesian grid (as curves) on one of the principal planes.
 *
 * Supported planes (case-insensitive): "XY","xy","XZ","xz","YX","yx","YZ","yz","ZX","zx","ZY","zy".
 * The third_direction_position places the grid along the axis normal to
 * the plane (e.g., Z for "XY").
 *
 * @param min_e1 Minimum coordinate along the first axis of the plane.
 * @param max_e1 Maximum coordinate along the first axis of the plane.
 * @param min_e2 Minimum coordinate along the second axis of the plane.
 * @param max_e2 Maximum coordinate along the second axis of the plane.
 * @param num_samples_e1 Number of segments along first axis.
 * @param num_samples_e2 Number of segments along second axis.
 * @param plane Plane code string ("XY","xy","XZ","xz","YX","yx","YZ","yz","ZX","zx","ZY","zy").
 * @param third_direction_position Position along the axis normal to plane.
 * @return Grid structure containing the generated curves and spacing.
 */
Grid adl_cartesian_grid_create(float min_e1, float max_e1, float min_e2, float max_e2, int num_samples_e1, int num_samples_e2, char plane[], float third_direction_position)
{
    Grid grid;
    ada_init_array(Curve, grid.curves);

    grid.min_e1 = min_e1;
    grid.max_e1 = max_e1;
    grid.min_e2 = min_e2;
    grid.max_e2 = max_e2;
    grid.num_samples_e1 = num_samples_e1;
    grid.num_samples_e2 = num_samples_e2;
    strncpy(grid.plane, plane, 2);

    float del_e1 = (max_e1 - min_e1) / num_samples_e1;
    float del_e2 = (max_e2 - min_e2) / num_samples_e2;

    grid.de1 = del_e1;
    grid.de2 = del_e2;

    if (!strncmp(plane, "XY", 3) || !strncmp(plane, "xy", 3)) {
        for (int e1_index = 0; e1_index <= num_samples_e1; e1_index++) {
            Curve curve;
            ada_init_array(Point, curve);
            Point point_max = {0}, point_min = {0};

            point_min.x = min_e1 + e1_index * del_e1;
            point_min.y = min_e2;
            point_min.z = third_direction_position;
            point_min.w = 1;

            point_max.x = min_e1 + e1_index * del_e1;
            point_max.y = max_e2;
            point_max.z = third_direction_position;
            point_max.w = 1;

            ada_appand(Point, curve, point_min);
            ada_appand(Point, curve, point_max);

            ada_appand(Curve, grid.curves, curve);
        }
        for (int e2_index = 0; e2_index <= num_samples_e2; e2_index++) {
            Curve curve;
            ada_init_array(Point, curve);
            Point point_max = {0}, point_min = {0};

            point_min.x = min_e1;
            point_min.y = min_e2 + e2_index * del_e2;
            point_min.z = third_direction_position;
            point_min.w = 1;

            point_max.x = max_e1;
            point_max.y = min_e2 + e2_index * del_e2;
            point_max.z = third_direction_position;
            point_max.w = 1;

            ada_appand(Point, curve, point_min);
            ada_appand(Point, curve, point_max);

            ada_appand(Curve, grid.curves, curve);
        }
    } else if (!strncmp(plane, "XZ", 3) || !strncmp(plane, "xz", 3)) {
        for (int e1_index = 0; e1_index <= num_samples_e1; e1_index++) {
            Curve curve;
            ada_init_array(Point, curve);
            Point point_max = {0}, point_min = {0};

            point_min.x = min_e1 + e1_index * del_e1;
            point_min.y = third_direction_position;
            point_min.z = min_e2;
            point_min.w = 1;

            point_max.x = min_e1 + e1_index * del_e1;
            point_max.y = third_direction_position;
            point_max.z = max_e2;
            point_max.w = 1;

            ada_appand(Point, curve, point_min);
            ada_appand(Point, curve, point_max);

            ada_appand(Curve, grid.curves, curve);
        }
        for (int e2_index = 0; e2_index <= num_samples_e2; e2_index++) {
            Curve curve;
            ada_init_array(Point, curve);
            Point point_max = {0}, point_min = {0};

            point_min.x = min_e1;
            point_min.y = third_direction_position;
            point_min.z = min_e2 + e2_index * del_e2;
            point_min.w = 1;

            point_max.x = max_e1;
            point_max.y = third_direction_position;
            point_max.z = min_e2 + e2_index * del_e2;
            point_max.w = 1;

            ada_appand(Point, curve, point_min);
            ada_appand(Point, curve, point_max);

            ada_appand(Curve, grid.curves, curve);
        }
    } else if (!strncmp(plane, "YX", 3) || !strncmp(plane, "yx", 3)) {
        for (int e1_index = 0; e1_index <= num_samples_e1; e1_index++) {
            Curve curve;
            ada_init_array(Point, curve);
            Point point_max = {0}, point_min = {0};

            point_min.x = min_e2;
            point_min.y = min_e1 + e1_index * del_e1;
            point_min.z = third_direction_position;
            point_min.w = 1;

            point_max.x = max_e2;
            point_max.y = min_e1 + e1_index * del_e1;
            point_max.z = third_direction_position;
            point_max.w = 1;

            ada_appand(Point, curve, point_min);
            ada_appand(Point, curve, point_max);

            ada_appand(Curve, grid.curves, curve);
        }
        for (int e2_index = 0; e2_index <= num_samples_e2; e2_index++) {
            Curve curve;
            ada_init_array(Point, curve);
            Point point_max = {0}, point_min = {0};

            point_min.x = min_e2 + e2_index * del_e2;
            point_min.y = min_e1;
            point_min.z = third_direction_position;
            point_min.w = 1;

            point_max.x = min_e2 + e2_index * del_e2;
            point_max.y = max_e1;
            point_max.z = third_direction_position;
            point_max.w = 1;

            ada_appand(Point, curve, point_min);
            ada_appand(Point, curve, point_max);

            ada_appand(Curve, grid.curves, curve);
        }
    } else if (!strncmp(plane, "YZ", 3) || !strncmp(plane, "yz", 3)) {
        for (int e1_index = 0; e1_index <= num_samples_e1; e1_index++) {
            Curve curve;
            ada_init_array(Point, curve);
            Point point_max = {0}, point_min = {0};

            point_min.x = third_direction_position;
            point_min.y = min_e1 + e1_index * del_e1;
            point_min.z = min_e2;
            point_min.w = 1;

            point_max.x = third_direction_position;
            point_max.y = min_e1 + e1_index * del_e1;
            point_max.z = max_e2;
            point_max.w = 1;

            ada_appand(Point, curve, point_min);
            ada_appand(Point, curve, point_max);

            ada_appand(Curve, grid.curves, curve);
        }
        for (int e2_index = 0; e2_index <= num_samples_e2; e2_index++) {
            Curve curve;
            ada_init_array(Point, curve);
            Point point_max = {0}, point_min = {0};

            point_min.x = third_direction_position;
            point_min.y = min_e1;
            point_min.z = min_e2 + e2_index * del_e2;
            point_min.w = 1;

            point_max.x = third_direction_position;
            point_max.y = max_e1;
            point_max.z = min_e2 + e2_index * del_e2;
            point_max.w = 1;

            ada_appand(Point, curve, point_min);
            ada_appand(Point, curve, point_max);

            ada_appand(Curve, grid.curves, curve);
        }
    } else if (!strncmp(plane, "ZX", 3) || !strncmp(plane, "zx", 3)) {
        for (int e1_index = 0; e1_index <= num_samples_e1; e1_index++) {
            Curve curve;
            ada_init_array(Point, curve);
            Point point_max = {0}, point_min = {0};

            point_min.x = min_e2;
            point_min.y = third_direction_position;
            point_min.z = min_e1 + e1_index * del_e1;
            point_min.w = 1;

            point_max.x = max_e2;
            point_max.y = third_direction_position;
            point_max.z = min_e1 + e1_index * del_e1;
            point_max.w = 1;

            ada_appand(Point, curve, point_min);
            ada_appand(Point, curve, point_max);

            ada_appand(Curve, grid.curves, curve);
        }
        for (int e2_index = 0; e2_index <= num_samples_e2; e2_index++) {
            Curve curve;
            ada_init_array(Point, curve);
            Point point_max = {0}, point_min = {0};

            point_min.x = min_e2 + e2_index * del_e2;
            point_min.y = third_direction_position;
            point_min.z = min_e1;
            point_min.w = 1;

            point_max.x = min_e2 + e2_index * del_e2;
            point_max.y = third_direction_position;
            point_max.z = max_e1;
            point_max.w = 1;

            ada_appand(Point, curve, point_min);
            ada_appand(Point, curve, point_max);

            ada_appand(Curve, grid.curves, curve);
        }
    } else if (!strncmp(plane, "ZY", 3) || !strncmp(plane, "zy", 3)) {
        for (int e1_index = 0; e1_index <= num_samples_e1; e1_index++) {
            Curve curve;
            ada_init_array(Point, curve);
            Point point_max = {0}, point_min = {0};

            point_min.x = third_direction_position;
            point_min.y = min_e2;
            point_min.z = min_e1 + e1_index * del_e1;
            point_min.w = 1;

            point_max.x = third_direction_position;
            point_max.y = max_e2;
            point_max.z = min_e1 + e1_index * del_e1;
            point_max.w = 1;

            ada_appand(Point, curve, point_min);
            ada_appand(Point, curve, point_max);

            ada_appand(Curve, grid.curves, curve);
        }
        for (int e2_index = 0; e2_index <= num_samples_e2; e2_index++) {
            Curve curve;
            ada_init_array(Point, curve);
            Point point_max = {0}, point_min = {0};

            point_min.x = third_direction_position;
            point_min.y = min_e2 + e2_index * del_e2;
            point_min.z = min_e1;
            point_min.w = 1;

            point_max.x = third_direction_position;
            point_max.y = min_e2 + e2_index * del_e2;
            point_max.z = max_e1;
            point_max.w = 1;

            ada_appand(Point, curve, point_min);
            ada_appand(Point, curve, point_max);

            ada_appand(Curve, grid.curves, curve);
        }
    }

    return grid;
}

/**
 * @brief Draw a previously created Grid as line segments.
 *
 * @param screen_mat Destination ARGB pixel buffer.
 * @param grid Grid to draw (curves are 2-point polylines).
 * @param color Line color (0xAARRGGBB).
 * @param offset_zoom_param Pan/zoom transform. Use ADL_DEFAULT_OFFSET_ZOOM for identity.
 */
void adl_grid_draw(Mat2D_uint32 screen_mat, Grid grid, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (size_t curve_index = 0; curve_index < grid.curves.length; curve_index++) {
        adl_lines_draw(screen_mat, grid.curves.elements[curve_index].elements, grid.curves.elements[curve_index].length, color, offset_zoom_param);
    }
}

#endif /*ALMOG_DRAW_LIBRARY_IMPLEMENTATION*/
