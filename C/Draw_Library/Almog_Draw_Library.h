#ifndef ALMOG_DRAW_LIBRARY_H_
#define ALMOG_DRAW_LIBRARY_H_

#include <math.h>
#include <stdint.h>
#include <limits.h>
#include "Matrix2D.h"
#include <string.h>
#include <float.h>

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
} Point;
#endif

typedef struct {
    uint32_t color;
    size_t length;
    size_t capacity;
    Point *elements;
} Curve;

typedef struct {
    size_t length;
    size_t capacity;
    Curve *elements;
} Curve_ada;

#ifndef TRI
#define TRI
typedef struct {
    Point points[3];
    Point tex_points[3];
    Point center;
    Point normal;
    float z_min;
    float z_max;
    bool to_draw;
    float light_intensity;
    uint32_t color;
} Tri;
#endif

#ifndef MESH
#define MESH
typedef struct {
    size_t length;
    size_t capacity;
    Tri *elements;
} Mesh; /* Tri ada array */
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
    uint32_t background_color;
    bool to_draw_axis;
    bool to_draw_max_min_values;
} Figure;


#ifndef HexARGB_RGB
#define HexARGB_RGBA(x) ((x)>>(8*2)&0xFF), ((x)>>(8*1)&0xFF), ((x)>>(8*0)&0xFF), ((x)>>(8*3)&0xFF)
#endif
#ifndef HexARGB_RGBA_VAR
#define HexARGB_RGBA_VAR(x) uint8_t r = ((x)>>(8*2)&0xFF); uint8_t g = ((x)>>(8*1)&0xFF); uint8_t b = ((x)>>(8*0)&0xFF); uint8_t a = ((x)>>(8*3)&0xFF)
#endif
#ifndef RGB_hexRGB
#define RGB_hexRGB(r, g, b) (int)(0x010000*(int)(r) + 0x000100*(int)(g) + 0x000001*(int)(b))
#endif

#define edge_cross_point(a1, b, a2, p) (b.x-a1.x)*(p.y-a2.y)-(b.y-a1.y)*(p.x-a2.x)
#define is_top_edge(x, y) (y == 0 && x > 0)
#define is_left_edge(x, y) (y < 0)
#define is_top_left(ps, pe) (is_top_edge(pe.x-ps.x, pe.y-ps.y) || is_left_edge(pe.x-ps.x, pe.y-ps.y))

#define ADL_MAX_POINT_VAL 1e5
#define adl_assert_point_is_valid(p) ADL_ASSERT(isfinite(p.x) && isfinite(p.y) && isfinite(p.z) && isfinite(p.w))
#define adl_assert_tri_is_valid(tri) adl_assert_point_is_valid(tri.points[0]); \
        adl_assert_point_is_valid(tri.points[1]);                              \
        adl_assert_point_is_valid(tri.points[2])

#define ADL_FIGURE_PADDING_PRECENTAGE 20
#define ADL_MAX_FIGURE_PADDING 50
#define ADL_MIN_FIGURE_PADDING 20
#define ADL_MAX_HEAD_SIZE 15
#define ADL_FIGURE_HEAD_ANGLE_DEG 30
#define ADL_FIGURE_AXIS_COLOR 0x0

#define ADL_MAX_CHARACTER_OFFSET 10
#define ADL_MIN_CHARACTER_OFFSET 5
#define ADL_MAX_SENTENCE_LEN 256
#define ADL_MAX_ZOOM 1e3

#define ADL_DEFAULT_OFFSET_ZOOM (Offset_zoom_param){1,0,0,0,0}

void adl_draw_point(Mat2D_uint32 screen_mat, int x, int y, uint32_t color, Offset_zoom_param offset_zoom_param);
void adl_draw_line(Mat2D_uint32 screen_mat, const float x1_input, const float y1_input, const float x2_input, const float y2_input, uint32_t color, Offset_zoom_param offset_zoom_param);
void adl_draw_lines(const Mat2D_uint32 screen_mat, const Point *points, const size_t len, const uint32_t color, Offset_zoom_param offset_zoom_param);
void adl_draw_lines_loop(const Mat2D_uint32 screen_mat, const Point *points, const size_t len, const uint32_t color, Offset_zoom_param offset_zoom_param);
void adl_draw_arrow(Mat2D_uint32 screen_mat, int xs, int ys, int xe, int ye, float head_size, float angle_deg, uint32_t color, Offset_zoom_param offset_zoom_param);

void adl_draw_character(Mat2D_uint32 screen_mat, char c, int width_pixel, int hight_pixel, int x_top_left, int y_top_left, uint32_t color, Offset_zoom_param offset_zoom_param);
void adl_draw_sentence(Mat2D_uint32 screen_mat, const char sentence[], size_t len, const int x_top_left, const int y_top_left, const int hight_pixel, const uint32_t color, Offset_zoom_param offset_zoom_param);

void adl_draw_rectangle_min_max(Mat2D_uint32 screen_mat, int min_x, int max_x, int min_y, int max_y, uint32_t color, Offset_zoom_param offset_zoom_param);
void adl_fill_rectangle_min_max(Mat2D_uint32 screen_mat, int min_x, int max_x, int min_y, int max_y, uint32_t color, Offset_zoom_param offset_zoom_param);

void adl_draw_circle(Mat2D_uint32 screen_mat, float center_x, float center_y, float r, uint32_t color, Offset_zoom_param offset_zoom_param);
void adl_fill_circle(Mat2D_uint32 screen_mat, float center_x, float center_y, float r, uint32_t color, Offset_zoom_param offset_zoom_param);

void adl_draw_tri(Mat2D_uint32 screen_mat, Tri tri, uint32_t color, Offset_zoom_param offset_zoom_param);
void adl_fill_tri_scanline_rasterizer(Mat2D_uint32 screen_mat, Tri tri, Offset_zoom_param offset_zoom_param);
void adl_fill_tri_Pinedas_rasterizer(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Tri tri, float light_intensity, Offset_zoom_param offset_zoom_param);
void adl_fill_tri_Pinedas_rasterizer_interpolate_color(Mat2D_uint32 screen_mat, Tri tri, float light_intensity, Offset_zoom_param offset_zoom_param);

void adl_draw_mesh(Mat2D_uint32 screen_mat, Mesh mesh, uint32_t color, Offset_zoom_param offset_zoom_param);
void adl_fill_mesh_scanline_rasterizer(Mat2D_uint32 screen_mat, Mesh mesh, Offset_zoom_param offset_zoom_param);
void adl_fill_mesh_Pinedas_rasterizer(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Mesh mesh, Offset_zoom_param offset_zoom_param);

float adl_linear_map(float s, float min_in, float max_in, float min_out, float max_out);

Figure adl_alloc_figure(size_t rows, size_t cols, Point top_left_position);
void adl_copy_figure_to_screen(Mat2D_uint32 screen_mat, Figure figure);
void adl_draw_axis_on_figure(Figure *figure);
void adl_add_curve_to_figure(Figure *figure, Point *src_points, size_t src_len, uint32_t color);
void adl_plot_curves_on_figure(Figure figure);

#endif /*ALMOG_RENDER_SHAPES_H_*/

#ifdef ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#undef ALMOG_DRAW_LIBRARY_IMPLEMENTATION

/* default values should be: 
zoom_multiplier = 1;
offset_x = 0;
offset_y = 0; */
void adl_draw_point(Mat2D_uint32 screen_mat, int x, int y, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    float window_w = (float)screen_mat.cols;
    float window_h = (float)screen_mat.rows;

    x = (x - window_w/2 + offset_zoom_param.offset_x) * offset_zoom_param.zoom_multiplier + window_w/2;
    y = (y - window_h/2 + offset_zoom_param.offset_y) * offset_zoom_param.zoom_multiplier + window_h/2;

    if ((x < (int)screen_mat.cols && y < (int)screen_mat.rows) && (x >= 0 && y >= 0)) { /* point is in screen */
        MAT2D_AT_UINT32(screen_mat, y, x) = color;
    }
}

void adl_draw_line(Mat2D_uint32 screen_mat, const float x1_input, const float y1_input, const float x2_input, const float y2_input, uint32_t color, Offset_zoom_param offset_zoom_param)
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

    adl_draw_point(screen_mat, x, y, color, (Offset_zoom_param){1,0,0,0,0});

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
            adl_draw_point(screen_mat, x, y, color, (Offset_zoom_param){1,0,0,0,0});
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
            adl_draw_point(screen_mat, x, y, color, (Offset_zoom_param){1,0,0,0,0});
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
            adl_draw_point(screen_mat, x, y, color, (Offset_zoom_param){1,0,0,0,0});
        }
    }

}

void adl_draw_lines(const Mat2D_uint32 screen_mat, const Point *points, const size_t len, const uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (size_t i = 0; i < len-1; i++) {
        adl_draw_line(screen_mat, points[i].x, points[i].y, points[i+1].x, points[i+1].y, color, offset_zoom_param);
    }
}

void adl_draw_lines_loop(const Mat2D_uint32 screen_mat, const Point *points, const size_t len, const uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (size_t i = 0; i < len-1; i++) {
        adl_draw_line(screen_mat, points[i].x, points[i].y, points[i+1].x, points[i+1].y, color, offset_zoom_param);
    }
    adl_draw_line(screen_mat, points[len-1].x, points[len-1].y, points[0].x, points[0].y, color, offset_zoom_param);
}

/* This function is a bit complicated and expansive but this is what I could come up with */
void adl_draw_arrow(Mat2D_uint32 screen_mat, int xs, int ys, int xe, int ye, float head_size, float angle_deg, uint32_t color, Offset_zoom_param offset_zoom_param)
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

    adl_draw_line(screen_mat, MAT2D_AT(v1, 0, 0), MAT2D_AT(v1, 1, 0), xe, ye, color, offset_zoom_param);
    adl_draw_line(screen_mat, MAT2D_AT(v2, 0, 0), MAT2D_AT(v2, 1, 0), xe, ye, color, offset_zoom_param);
    adl_draw_line(screen_mat, xs, ys, xe, ye, color, offset_zoom_param);

    mat2D_free(pe);
    mat2D_free(v1);
    mat2D_free(v2);
    mat2D_free(temp_v);
    mat2D_free(DCM_p);
    mat2D_free(DCM_m);
}

void adl_draw_character(Mat2D_uint32 screen_mat, char c, int width_pixel, int hight_pixel, int x_top_left, int y_top_left, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    switch (c)
    {
    case 'a':
    case 'A':
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel/2, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/6, y_top_left+2*hight_pixel/3, x_top_left+5*width_pixel/6, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        break;
    case 'b':
    case 'B':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'c':
    case 'C':
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'd':
    case 'D':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left, y_top_left, color, offset_zoom_param);
        break;
    case 'e':
    case 'E':
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/2, color, offset_zoom_param);
        break;
    case 'f':
    case 'F':
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/2, color, offset_zoom_param);
        break;
    case 'g':
    case 'G':
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/2, x_top_left+width_pixel/2, y_top_left+hight_pixel/2, color, offset_zoom_param);
        break;
    case 'h':
    case 'H':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/2, color, offset_zoom_param);
        break;
    case 'i':
    case 'I':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'j':
    case 'J':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+2*width_pixel/3, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left+hight_pixel, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel/6, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        break;
    case 'k':
    case 'K':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        break;
    case 'l':
    case 'L':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'm':
    case 'M':
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'n':
    case 'N':
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        break;
    case 'o':
    case 'O':
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        break;
    case 'p':
    case 'P':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left, y_top_left+hight_pixel/2, color, offset_zoom_param);
        break;
    case 'q':
    case 'Q':
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'r':
    case 'R':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 's':
    case 'S':
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/3, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        break;
    case 't':
    case 'T':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'u':
    case 'U':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        break;
    case 'v':
    case 'V':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        break;
    case 'w':
    case 'W':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel/2, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        break;
    case 'x':
    case 'X':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        break;
    case 'y':
    case 'Y':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left+hight_pixel/2, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case 'z':
    case 'Z':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case '.':
        adl_fill_rectangle_min_max(screen_mat, x_top_left+width_pixel/6, x_top_left+width_pixel/3, y_top_left+5*hight_pixel/6, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case '0':
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        break;
    case '1':
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left+width_pixel/2, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case '2':
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case '3':
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        break;
    case '4':
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+2*hight_pixel/3, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        break;
    case '5':
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel/2, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        break;
    case '6':
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, x_top_left, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        break;
    case '7':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    case '8':
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/3, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);

        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, x_top_left, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+2*hight_pixel/3, x_top_left, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color, offset_zoom_param);
        break;
    case '9':
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+hight_pixel/3, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/3, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/3, color, offset_zoom_param);
        break;
    case '-':
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/2, color, offset_zoom_param);
        break;
    case '+':
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/2, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color, offset_zoom_param);
        break;
    default:
        adl_draw_rectangle_min_max(screen_mat, x_top_left, x_top_left+width_pixel, y_top_left, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color, offset_zoom_param);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color, offset_zoom_param);
        break;
    }
}

void adl_draw_sentence(Mat2D_uint32 screen_mat, const char sentence[], size_t len, const int x_top_left, const int y_top_left, const int hight_pixel, const uint32_t color, Offset_zoom_param offset_zoom_param)
{
    int character_width_pixel = hight_pixel/2;
    int current_x_top_left = x_top_left;
    int character_x_offset = (int)fmaxf(fminf(ADL_MAX_CHARACTER_OFFSET, character_width_pixel / 5), ADL_MIN_CHARACTER_OFFSET);

    for (size_t char_index = 0; char_index < len; char_index++) {
        adl_draw_character(screen_mat, sentence[char_index], character_width_pixel, hight_pixel, current_x_top_left, y_top_left, color, offset_zoom_param);
        current_x_top_left += character_width_pixel + character_x_offset;
    }

}

void adl_draw_rectangle_min_max(Mat2D_uint32 screen_mat, int min_x, int max_x, int min_y, int max_y, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    adl_draw_line(screen_mat, min_x, min_y, max_x, min_y, color, offset_zoom_param);
    adl_draw_line(screen_mat, min_x, max_y, max_x, max_y, color, offset_zoom_param);
    adl_draw_line(screen_mat, min_x, min_y, min_x, max_y, color, offset_zoom_param);
    adl_draw_line(screen_mat, max_x, min_y, max_x, max_y, color, offset_zoom_param);
}

void adl_fill_rectangle_min_max(Mat2D_uint32 screen_mat, int min_x, int max_x, int min_y, int max_y, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (int y = min_y; y <= max_y; y++) {
        adl_draw_line(screen_mat, min_x, y, max_x, y, color, offset_zoom_param);
    }
}

void adl_draw_circle(Mat2D_uint32 screen_mat, float center_x, float center_y, float r, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx ++) {
            float diff = dx * dx + dy * dy - r * r;
            if (diff < 0 && diff > -r*2) {
                adl_draw_point(screen_mat, center_x + dx, center_y + dy, color, offset_zoom_param);
            }
        }
    }
}

void adl_fill_circle(Mat2D_uint32 screen_mat, float center_x, float center_y, float r, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx ++) {
            float diff = dx * dx + dy * dy - r * r;
            if (diff < 0) {
                adl_draw_point(screen_mat, center_x + dx, center_y + dy, color, offset_zoom_param);
            }
        }
    }
}

void adl_draw_tri(Mat2D_uint32 screen_mat, Tri tri, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    adl_draw_line(screen_mat, tri.points[0].x, tri.points[0].y, tri.points[1].x, tri.points[1].y, color, offset_zoom_param);
    adl_draw_line(screen_mat, tri.points[1].x, tri.points[1].y, tri.points[2].x, tri.points[2].y, color, offset_zoom_param);
    adl_draw_line(screen_mat, tri.points[2].x, tri.points[2].y, tri.points[0].x, tri.points[0].y, color, offset_zoom_param);

    // adl_draw_arrow(screen_mat, tri.points[0].x, tri.points[0].y, tri.points[1].x, tri.points[1].y, 0.3, 22, color);
    // adl_draw_arrow(screen_mat, tri.points[1].x, tri.points[1].y, tri.points[2].x, tri.points[2].y, 0.3, 22, color);
    // adl_draw_arrow(screen_mat, tri.points[2].x, tri.points[2].y, tri.points[0].x, tri.points[0].y, 0.3, 22, color);
}

/* This works but there are some artifacts */
void adl_fill_tri_scanline_rasterizer(Mat2D_uint32 screen_mat, Tri tri, Offset_zoom_param offset_zoom_param)
{
    /* This function follows the rasterizer of 'Pikuma' shown in his YouTube video. You can fine the video in this link: https://youtu.be/k5wtuKWmV48. */
    /* arranging the points according to y value */
    Point p0 = tri.points[0];
    Point p1 = tri.points[1];
    Point p2 = tri.points[2];
    if (p1.y > p0.y) {
        Point temp = p1;
        p1 = p0;
        p0 = temp;
    }
    if (p2.y > p1.y) {
        Point temp = p2;
        p2 = p1;
        p1 = temp;
        if (p1.y > p0.y) {
            Point temp = p1;
            p1 = p0;
            p0 = temp;
        }
    }
    if (p2.y > p0.y) {
        Point temp = p2;
        p2 = p0;
        p0 = temp;
    }

    /* finding max and min x */
    int x_max = fmax(p0.x, fmax(p1.x, p2.x));
    int x_min = fmin(p0.x, fmin(p1.x, p2.x));

    if (p0.x == p1.x && p1.x == p2.x) {
        adl_draw_tri(screen_mat, tri, tri.color, offset_zoom_param);
        return;
    }

    /* The rasterization */
    float m01 = (p0.y - p1.y) / (p0.x - p1.x);
    float b01 = p0.y - m01 * p0.x;
    float m02 = (p0.y - p2.y) / (p0.x - p2.x);
    float b02 = p0.y - m02 * p0.x;
    float m12 = (p1.y - p2.y) / (p1.x - p2.x);
    float b12 = p1.y - m12 * p1.x;

    float epsilon = 1e-3;
    int gap = 15;
    // printf("m01: %f, m02: %f, m12: %f\n", m01, m02, m12);
    if (fabs(m02) < epsilon || fabs(m12) < epsilon || fabs(m01) < epsilon) return;
    for (int y = (int)p2.y; y < (int)p1.y; y++)
    {
        float x02 = (y - b02) / m02;
        float x12 = (y - b12) / m12;
        if (x02 <= x_min-gap || x02 >= x_max+gap) continue;
        if (x12 <= x_min-gap || x12 >= x_max+gap) continue;
        if (fabs(p0.x - p2.x) - fabs(p0.x - x02) < 0) continue;
        if (fabs(p1.x - p2.x) - fabs(p1.x - x12) < 0) continue;
        adl_draw_line(screen_mat, x02, y, x12, y, tri.color, offset_zoom_param);
        // printf("x02: %d, x12: %d, y: %d\n", (int)x02, (int)x12, (int)y);
    }
    for (int y = (int)p1.y; y <= (int)p0.y; y++) {
        float x01 = (y - b01) / m01;
        float x02 = (y - b02) / m02;
        if (x01 <= x_min-gap || x01 >= x_max+gap) continue;
        if (x02 <= x_min-gap || x02 >= x_max+gap) continue;
        if (fabs(p1.x - p0.x) - fabs(p1.x - x01) < 0) continue;
        if (fabs(p0.x - p2.x) - fabs(p0.x - x02) < 0) continue;
        adl_draw_line(screen_mat, x02, y, x01, y, tri.color, offset_zoom_param);
    }
}


/* This function is the function for rasterization */
void adl_fill_tri_Pinedas_rasterizer(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer, Tri tri, float light_intensity, Offset_zoom_param offset_zoom_param)
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
        adl_draw_tri(screen_mat, tri, tri.color, offset_zoom_param);
        return;
    }
    MATRIX2D_ASSERT(fabsf(w) > 1e-6 && "triangle has area");

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

            float alpha = fabs(w0 / w);
            float beta  = fabs(w1 / w);
            float gamma = fabs(w2 / w);

            if (w0 * w >= 0 && w1 * w >= 0 &&  w2 * w >= 0) {
                HexARGB_RGBA_VAR(tri.color);
                float rf = r * light_intensity;
                float gf = g * light_intensity;
                float bf = b * light_intensity;
                uint8_t r8 = (uint8_t)fmaxf(0, fminf(255, rf));
                uint8_t g8 = (uint8_t)fmaxf(0, fminf(255, gf));
                uint8_t b8 = (uint8_t)fmaxf(0, fminf(255, bf));
                (void)a;

                double inv_w = beta * (1.0 / p0.w) + gamma  * (1.0 / p1.w) + alpha * (1.0 / p2.w);
                double z_over_w = beta * (p0.z / p0.w) + gamma  * (p1.z / p1.w) + alpha * (p2.z / p2.w);
                double inv_z = inv_w / z_over_w;

                if (inv_z >= MAT2D_AT(inv_z_buffer, y, x)) {
                    adl_draw_point(screen_mat, x, y, RGB_hexRGB(r8, g8, b8), offset_zoom_param);
                    MAT2D_AT(inv_z_buffer, y, x) = inv_z;
                }
            }
        }
    }
}

void adl_fill_tri_Pinedas_rasterizer_interpolate_color(Mat2D_uint32 screen_mat, Tri tri, float light_intensity, Offset_zoom_param offset_zoom_param)
{
    /* This function follows the rasterizer of 'Pikuma' shown in his YouTube video. You can fine the video in this link: https://youtu.be/k5wtuKWmV48. */

    Point p0, p1, p2;
    p0 = tri.points[0];
    p1 = tri.points[1];
    p2 = tri.points[2];

    float w = edge_cross_point(p0, p1, p1, p2);
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
            // adl_draw_point(screen_mat, x, y, color);
            Point p = {.x = x, .y = y, .z = 0};

            float w0 = edge_cross_point(p0, p1, p0, p) + bias0;
            float w1 = edge_cross_point(p1, p2, p1, p) + bias1;
            float w2 = edge_cross_point(p2, p0, p2, p) + bias2;

            float alpha = fabs(w0 / w);
            float beta  = fabs(w1 / w);
            float gamma = fabs(w2 / w);
            // printf("alpha: %5f, beta: %5f, gamma: %5f\n", alpha, beta, gamma);

            if (w0 * w >= 0 && w1 * w >= 0 &&  w2 * w >= 0) {
            // if (w0 < 0 && w1 < 0 &&  w2 < 0) {
                HexARGB_RGBA_VAR(tri.color);
                
                uint8_t current_r = r*alpha + 0xaa*beta + 0x22*gamma;
                uint8_t current_g = g*alpha + 0xaa*beta + 0x22*gamma;
                uint8_t current_b = b*alpha + 0xaa*beta + 0x22*gamma;

                float rf = current_r * light_intensity;
                float gf = current_g * light_intensity;
                float bf = current_b * light_intensity;
                uint8_t r8 = (uint8_t)fmaxf(0, fminf(255, rf));
                uint8_t g8 = (uint8_t)fmaxf(0, fminf(255, gf));
                uint8_t b8 = (uint8_t)fmaxf(0, fminf(255, bf));
                (void)a;

                adl_draw_point(screen_mat, x, y, RGB_hexRGB(r8, g8, b8), offset_zoom_param);
            }
        }
    }
}

void adl_draw_mesh(Mat2D_uint32 screen_mat, Mesh mesh, uint32_t color, Offset_zoom_param offset_zoom_param)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Tri tri = mesh.elements[i];
        if (tri.to_draw) {
            adl_draw_tri(screen_mat, tri, color, offset_zoom_param);
        }
    }
}

void adl_fill_mesh_scanline_rasterizer(Mat2D_uint32 screen_mat, Mesh mesh, Offset_zoom_param offset_zoom_param)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Tri tri = mesh.elements[i];
        if (tri.to_draw) {
            adl_fill_tri_scanline_rasterizer(screen_mat, tri, offset_zoom_param);
        }
    }
}

void adl_fill_mesh_Pinedas_rasterizer(Mat2D_uint32 screen_mat, Mat2D inv_z_buffer_mat, Mesh mesh, Offset_zoom_param offset_zoom_param)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Tri tri = mesh.elements[i];
        /* Reject invalid triangles */
        adl_assert_tri_is_valid(tri);

        if (!tri.to_draw) continue;

        adl_fill_tri_Pinedas_rasterizer(screen_mat, inv_z_buffer_mat, tri, tri.light_intensity, offset_zoom_param);
    }
}

float adl_linear_map(float s, float min_in, float max_in, float min_out, float max_out)
{
    return (min_out + ((s-min_in)*(max_out-min_out))/(max_in-min_in));
}

Figure adl_alloc_figure(size_t rows, size_t cols, Point top_left_position)
{
    ADL_ASSERT(rows && cols);
    adl_assert_point_is_valid(top_left_position);

    Figure figure = {0};
    figure.pixels_mat = mat2D_alloc_uint32(rows, cols);
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

void adl_copy_figure_to_screen(Mat2D_uint32 screen_mat, Figure figure)
{
    for (size_t i = 0; i < figure.pixels_mat.rows; i++) {
        for (size_t j = 0; j < figure.pixels_mat.cols; j++) {
            int offset_i = figure.top_left_position.y;
            int offset_j = figure.top_left_position.x;
            
            adl_draw_point(screen_mat, offset_j+j, offset_i+i, MAT2D_AT_UINT32(figure.pixels_mat, i, j), (Offset_zoom_param){1,0,0,0,0});
        }
    }
}

void adl_draw_axis_on_figure(Figure *figure)
{
    int max_i    = (int)(figure->pixels_mat.rows);
    int max_j    = (int)(figure->pixels_mat.cols);
    int offset_i = (int)fmaxf(fminf(figure->pixels_mat.rows * ADL_FIGURE_PADDING_PRECENTAGE / 100.0f, ADL_MAX_FIGURE_PADDING), ADL_MIN_FIGURE_PADDING);
    int offset_j = (int)fmaxf(fminf(figure->pixels_mat.cols * ADL_FIGURE_PADDING_PRECENTAGE / 100.0f, ADL_MAX_FIGURE_PADDING), ADL_MIN_FIGURE_PADDING);

    int arrow_head_size_x = (int)fminf(ADL_MAX_HEAD_SIZE, ADL_FIGURE_PADDING_PRECENTAGE / 100.0f * (max_j - 2 * offset_j));
    int arrow_head_size_y = (int)fminf(ADL_MAX_HEAD_SIZE, ADL_FIGURE_PADDING_PRECENTAGE / 100.0f * (max_i - 2 * offset_i));

    adl_draw_arrow(figure->pixels_mat, figure->min_x_pixel, figure->max_y_pixel, figure->max_x_pixel, figure->max_y_pixel, (float)arrow_head_size_x / (max_j-2*offset_j), ADL_FIGURE_HEAD_ANGLE_DEG, 0, figure->offset_zoom_param);
    adl_draw_arrow(figure->pixels_mat, figure->min_x_pixel, figure->max_y_pixel, figure->min_x_pixel, figure->min_y_pixel, (float)arrow_head_size_y / (max_i-2*offset_i), ADL_FIGURE_HEAD_ANGLE_DEG, 0, figure->offset_zoom_param);
    // adl_draw_rectangle_min_max(figure->pixels_mat, figure->min_x_pixel, figure->max_x_pixel, figure->min_y_pixel, figure->max_y_pixel, 0);

    figure->x_axis_head_size = arrow_head_size_x;
    figure->y_axis_head_size = arrow_head_size_y;
}

void adl_add_curve_to_figure(Figure *figure, Point *src_points, size_t src_len, uint32_t color)
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

void adl_plot_curves_on_figure(Figure figure)
{
    mat2D_fill_uint32(figure.pixels_mat, figure.background_color);
    if (figure.to_draw_axis) adl_draw_axis_on_figure(&figure);

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

            adl_draw_line(figure.pixels_mat, des_start.x, des_start.y, des_end.x, des_end.y, figure.src_curve_array.elements[curve_index].color, figure.offset_zoom_param);
        }
    }

    if (figure.to_draw_max_min_values) {
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

        adl_draw_sentence(figure.pixels_mat, x_min_sentence, strlen(x_min_sentence), figure.min_x_pixel, figure.max_y_pixel+ADL_MIN_CHARACTER_OFFSET*2, x_min_sentence_hight_pixel, 0, figure.offset_zoom_param);
        adl_draw_sentence(figure.pixels_mat, x_max_sentence, strlen(x_max_sentence), x_max_x_top_left, figure.max_y_pixel+ADL_MIN_CHARACTER_OFFSET*2, x_max_sentence_hight_pixel, 0, figure.offset_zoom_param);
        
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

        y_min_sentence_hight_pixel = (int)fminf(y_min_sentence_hight_pixel, y_max_sentence_hight_pixel);
        y_max_sentence_hight_pixel = y_min_sentence_hight_pixel;

        adl_draw_sentence(figure.pixels_mat, y_max_sentence, strlen(y_max_sentence), ADL_MAX_CHARACTER_OFFSET/2, figure.min_y_pixel, y_max_sentence_hight_pixel, 0, figure.offset_zoom_param);
        adl_draw_sentence(figure.pixels_mat, y_min_sentence, strlen(y_min_sentence), ADL_MAX_CHARACTER_OFFSET/2, figure.max_y_pixel-y_min_sentence_hight_pixel, y_min_sentence_hight_pixel, 0, figure.offset_zoom_param);
    }
}

#endif /*ALMOG_DRAW_LIBRARY_IMPLEMENTATION*/
