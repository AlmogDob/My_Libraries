/**
 * @file
 * @brief Immediate-mode 2D/3D raster helpers for drawing onto to the screen.
 * 
 */

#ifndef ALMOG_DRAW_LIBRARY_H_
#define ALMOG_DRAW_LIBRARY_H_

#if defined(_WIN32) || defined(_WIN64) 
    #pragma warning(disable : 4709)
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

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
    struct Adl_Points_Dynamic_Array points;
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

struct Adl_Tris_Dynamic_Array {
    size_t length;
    size_t capacity;
    struct Adl_Tri *elements;
};

struct Adl_Quad {
    struct Adl_Point points[4];
    struct Adl_Point normals[4];
    uint32_t colors[4];
    bool to_draw;
    float light_intensity[4];
};

struct Adl_Quads_Dynamic_Array {
    size_t length;
    size_t capacity;
    struct Adl_Quad *elements;
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

#define ADL_HexARGB_EXPEND_TO_RGBA(x) ((x)>>(8*2)&0xFF), ((x)>>(8*1)&0xFF), ((x)>>(8*0)&0xFF), ((x)>>(8*3)&0xFF)
#define ADL_HexARGB_EXPEND_TO_RGB(x) ((x)>>(8*2)&0xFF), ((x)>>(8*1)&0xFF), ((x)>>(8*0)&0xFF)

#define ADL_COLOR_BLACK_hexARGB  0xFF000000
#define ADL_COLOR_BLUE_hexARGB   0xFF0000FF
#define ADL_COLOR_CYAN_hexARGB   0xFF00FFFF
#define ADL_COLOR_GREEN_hexARGB  0xFF00FF00
#define ADL_COLOR_PURPLE_hexARGB 0xFFFF00FF
#define ADL_COLOR_RED_hexARGB    0xFFFF0000
#define ADL_COLOR_YELLOW_hexARGB 0xFFFFFF00
#define ADL_COLOR_WHITE_hexARGB  0xFFFFFFFF

#define adl_assert_point_is_valid(p) ADL_ASSERT(isfinite(p.x) && isfinite(p.y) && isfinite(p.z) && isfinite(p.w))
#define adl_assert_tri_is_valid(tri) adl_assert_point_is_valid(tri.points[0]); \
        adl_assert_point_is_valid(tri.points[1]);                              \
        adl_assert_point_is_valid(tri.points[2])
#define adl_assert_quad_is_valid(quad) adl_assert_point_is_valid(quad.points[0]);   \
        adl_assert_point_is_valid(quad.points[1]);                                  \
        adl_assert_point_is_valid(quad.points[2]);                                  \
        adl_assert_point_is_valid(quad.points[3])

#define ADL_DEFAULT_OFFSET_ZOOM (struct Adl_Offset_Zoom){.zoom_multiplier = 1, .offset_x = 0, .offset_y = 0}

#ifndef ADL_DEF
    #ifdef ADL_DEF_STATIC
        #define ADL_DEF static
    #else
        #define ADL_DEF extern
    #endif
#endif

ADL_DEF uint32_t    adl_alpha_blend(uint32_t dst, uint32_t src);

ADL_DEF void        adl_circle_draw(struct Adl_Pixel_Buffer screen, adl_real center_x, adl_real center_y, adl_real r, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void        adl_circle_fill(struct Adl_Pixel_Buffer screen, adl_real center_x, adl_real center_y, adl_real r, uint32_t color, struct Adl_Offset_Zoom offzoom);

ADL_DEF void        adl_hexargb_to_rgba(uint32_t color, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);

ADL_DEF void        adl_point_draw(struct Adl_Pixel_Buffer screen, adl_real x, adl_real y, uint32_t color, struct Adl_Offset_Zoom offzoom);

ADL_DEF uint32_t    adl_rgba_to_hexargb(int r, int g, int b, int a);

ADL_DEF uint8_t     adl_u8_clamp_int(int x);

#endif /*ALMOG_RENDER_SHAPES_H_*/

#ifdef ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#undef ALMOG_DRAW_LIBRARY_IMPLEMENTATION

ADL_DEF uint32_t adl_alpha_blend(uint32_t dst, uint32_t src)
{
    uint8_t sr, sg, sb, sa;
    uint8_t dr, dg, db;

    adl_hexargb_to_rgba(src, &sr, &sg, &sb, &sa);
    adl_hexargb_to_rgba(dst, &dr, &dg, &db, NULL);

    adl_real a = (adl_real)sa / 255.0f;

    int r = (int)((adl_real)dr * (1.0f - a) + (adl_real)sr * a);
    int g = (int)((adl_real)dg * (1.0f - a) + (adl_real)sg * a);
    int b = (int)((adl_real)db * (1.0f - a) + (adl_real)sb * a);

    return adl_rgba_to_hexargb(r, g, b, 255);
}

ADL_DEF void adl_circle_draw(struct Adl_Pixel_Buffer screen, adl_real center_x, adl_real center_y, adl_real r, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    for (adl_real dy = -r; dy <= r; dy+=1) {
        for (adl_real dx = -r; dx <= r; dx+=1) {
            adl_real diff = dx * dx + dy * dy - r * r;
            if (diff < 0 && diff > -r*2) {
                adl_point_draw(screen, center_x + dx, center_y + dy, color, offzoom);
            }
        }
    }
}

ADL_DEF void adl_circle_fill(struct Adl_Pixel_Buffer screen, adl_real center_x, adl_real center_y, adl_real r, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    for (adl_real dy = -r; dy <= r; dy+=1) {
        for (adl_real dx = -r; dx <= r; dx+=1) {
            adl_real diff = dx * dx + dy * dy - r * r;
            if (diff < 0) {
                adl_point_draw(screen, center_x + dx, center_y + dy, color, offzoom);
            }
        }
    }
}

ADL_DEF void adl_hexargb_to_rgba(uint32_t color, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a)
{
    if (a) *a = (uint8_t)((color >> 24) & 0xFF);
    if (r) *r = (uint8_t)((color >> 16) & 0xFF);
    if (g) *g = (uint8_t)((color >> 8) & 0xFF);
    if (b) *b = (uint8_t)((color >> 0) & 0xFF);
}

ADL_DEF void adl_point_draw(struct Adl_Pixel_Buffer screen, adl_real x, adl_real y, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    adl_real window_w = (adl_real)screen.cols;
    adl_real window_h = (adl_real)screen.rows;

    adl_real zoom = offzoom.zoom_multiplier;
    adl_real start_x = (x - window_w/2.0f + offzoom.offset_x) * zoom + window_w/2.0f;
    adl_real start_y = (y - window_h/2.0f + offzoom.offset_y) * zoom + window_h/2.0f;
    int ix0 = (int)start_x;
    int iy0 = (int)start_y;

    if (offzoom.zoom_multiplier <= 0) return;
    int block = (int)(zoom + (adl_real)0.5);
    if (block < 1) block = 1;

    for (int dx = 0; dx <= block; dx++) {
        for (int dy = 0; dy <= block; dy++) {
            int ix = ix0 + dx;
            int iy = iy0 + dy;

            if ((ix >= 0 && iy >= 0) && ((size_t)ix < screen.cols && (size_t)iy < screen.rows)) { /* point is in screen */
                ADL_BUFFER_AT(screen, iy, ix) = adl_alpha_blend(ADL_BUFFER_AT(screen, iy, ix), color);
            }
        }
    }
}

ADL_DEF uint32_t adl_rgba_to_hexargb(int r, int g, int b, int a)
{
    uint32_t ru = adl_u8_clamp_int(r);
    uint32_t gu = adl_u8_clamp_int(g);
    uint32_t bu = adl_u8_clamp_int(b);
    uint32_t au = adl_u8_clamp_int(a);

    return (au << 24) | (ru << 16) | (gu << 8) | bu;
}

ADL_DEF uint8_t adl_u8_clamp_int(int x)
{
    if (x < 0) {
        return 0;
    }
    if (x > 255) {
        return 255;
    }
    return (uint8_t)x;
}

#endif /*ALMOG_DRAW_LIBRARY_IMPLEMENTATION*/
