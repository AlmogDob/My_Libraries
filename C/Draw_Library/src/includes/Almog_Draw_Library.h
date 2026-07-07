/**
 * @file
 * @brief Immediate-mode 2D/3D raster helpers for drawing onto to the screen.
 * 
 */

#ifndef ALMOG_DRAW_LIBRARY_H_
#define ALMOG_DRAW_LIBRARY_H_

#include <stdint.h>

#include "./Almog_Dynamic_Array.h"

#ifndef adl_real
    #if defined(ADL_SINGLE_PRECISION)
        typedef float adl_real_type;
        #define ADL_EPS 1e-5f
    #else 
        typedef double adl_real_type;
        #define ADL_EPS 1e-10
    #endif
    #define adl_real adl_real_type
#endif
#ifndef ADL_PI
    #define ADL_PI (adl_real)3.14159265358979323846
#endif
#ifndef ADL_ASSERT
    #include <assert.h>
    #define ADL_ASSERT assert
#endif

struct Adl_Offset_Zoom {
    float zoom_multiplier;
    float offset_x;
    float offset_y;
    int mouse_x;
    int mouse_y;
};

struct Adl_Point {
    float x;
    float y;
    float z;
    float w;
};

struct Adl_Points_Dynamic_Array {
    size_t length;
    size_t capacity;
    struct Adl_Point *elements;
};

struct Adl_Curve {
    uint32_t color;
    struct Adl_Point_Dynamic_Array *points;
};

struct Adl_Curves_Dynamic_Array {
    size_t length;
    size_t capacity;
    struct Adl_Curve *elements;
};

struct Adl_Tri {
    struct Adl_Point points[3];
    struct Adl_Point tex_points[3];
    struct Adl_Point normals[3];
    uint32_t colors[3];
    bool to_draw;
    float light_intensity[3];
};

struct Adl_Quad {
    struct Adl_Point points[4];
    struct Adl_Point normals[4];
    uint32_t colors[4];
    bool to_draw;
    float light_intensity[4];
};

struct Adl_Tri_Dynamic_Array {
    size_t length;
    size_t capacity;
    struct Tri *elements;
};

struct Adl_Quad_Dynamic_Array {
    size_t length;
    size_t capacity;
    struct Quad *elements;
};

struct Adl_Pixel_Buffer {
    size_t rows;
    size_t cols;
    size_t stride_r;
    uint32_t *elements;
};

struct Adl_Depth_Buffer {
    size_t rows;
    size_t cols;
    size_t stride_r;
    adl_real *elements;
};

#define adl_min(a, b) ((a) < (b) ? (a) : (b))
#define adl_max(a, b) ((a) > (b) ? (a) : (b))

#define ADL_BUFFER_AT(m, i, j) (m).elements[(ADL_ASSERT((i) < (m).rows && (j) < (m).cols), (i) * (m).stride_r + (j))]

#define ADL_HexARGB_TO_RGBA(x) ((x)>>(8*2)&0xFF), ((x)>>(8*1)&0xFF), ((x)>>(8*0)&0xFF), ((x)>>(8*3)&0xFF)
#define ADL_HexARGB_TO_RGB_VAR(x, r, g, b) (r) = ((x)>>(8*2)&0xFF); (g) = ((x)>>(8*1)&0xFF); (b) = ((x)>>(8*0)&0xFF);
#define ADL_HexARGB_TO_RGBA_VAR(x, r, g, b, a) (r) = ((x)>>(8*2)&0xFF); (g) = ((x)>>(8*1)&0xFF); (b) = ((x)>>(8*0)&0xFF); (a) = ((x)>>(8*3)&0xFF);
#define ADL_RGB_TO_hexRGB(r, g, b) (int)(0x010000*(int)(r) + 0x000100*(int)(g) + 0x000001*(int)(b))
#define ADL_RGBA_TO_hexARGB(r, g, b, a) (int)(0x01000000l*(unsigned int)(adl_max(0, adl_min(a, 255))) + 0x010000*(int)(adl_max(0, adl_min(r, 255))) + 0x000100*(int)(adl_max(0, adl_min(g, 255))) + 0x000001*(int)(adl_max(0, adl_min(b, 255))))

#define ADL_COLOR_RED_hexARGB    0xFFFF0000
#define ADL_COLOR_GREEN_hexARGB  0xFF00FF00
#define ADL_COLOR_BLUE_hexARGB   0xFF0000FF
#define ADL_COLOR_PURPLE_hexARGB 0xFFFF00FF
#define ADL_COLOR_CYAN_hexARGB   0xFF00FFFF
#define ADL_COLOR_YELLOW_hexARGB 0xFFFFFF00

#define adl_assert_point_is_valid(p) ADL_ASSERT(isfinite(p.x) && isfinite(p.y) && isfinite(p.z) && isfinite(p.w))
#define adl_assert_tri_is_valid(tri) adl_assert_point_is_valid(tri.points[0]); \
        adl_assert_point_is_valid(tri.points[1]);                              \
        adl_assert_point_is_valid(tri.points[2])
#define adl_assert_quad_is_valid(quad) adl_assert_point_is_valid(quad.points[0]);   \
        adl_assert_point_is_valid(quad.points[1]);                                  \
        adl_assert_point_is_valid(quad.points[2]);                                  \
        adl_assert_point_is_valid(quad.points[3])



#endif /*ALMOG_RENDER_SHAPES_H_*/

#ifdef ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#undef ALMOG_DRAW_LIBRARY_IMPLEMENTATION


#endif /*ALMOG_DRAW_LIBRARY_IMPLEMENTATION*/
