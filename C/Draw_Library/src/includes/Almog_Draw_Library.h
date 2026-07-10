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
#include <limits.h>
#include <math.h>

#include "./Almog_Dynamic_Array.h"

#if defined(ADL_SINGLE_PRECISION)
    typedef float adl_real_type;
    #define ADL_EPS   1e-5f
    #define adl_fabs  fabsf
    #define adl_floor floorf
    #define adl_ceil  ceilf
#else 
    typedef double adl_real_type;
    #define ADL_EPS   1e-10
    #define adl_fabs  fabs
    #define adl_floor floor
    #define adl_ceil  ceil
#endif
#define adl_real adl_real_type

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

/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define adl_dprintSTRING(expr) printf("[Info] %s:%d:\n%*s" #expr " = %s\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define adl_dprintCHAR(expr) printf("[Info] %s:%d:\n%*s" #expr " = %c\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define adl_dprintINT(expr) printf("[Info] %s:%d:\n%*s" #expr " = %d\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define adl_dprintFLOAT(expr) printf("[Info] %s:%d:\n%*s" #expr " = %#f\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define adl_dprintDOUBLE(expr) printf("[Info] %s:%d:\n%*s" #expr " = %#g\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define adl_dprintSIZE_T(expr) printf("[Info] %s:%d:\n%*s" #expr " = %zu\n", __FILE__, __LINE__, 7, "", expr)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define adl_dprintINFO(fmt, ...) \
    fprintf(stderr, "[Info] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 7, "", __func__, 7, "", __VA_ARGS__)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define adl_dprintWARNING(fmt, ...) \
    fprintf(stderr, "[Warning] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 10, "", __func__, 10, "", __VA_ARGS__)
/**
 * @name Debug-print helpers
 * @brief Convenience macros for diagnostic output.
 *
 * The typed variants print to stdout. The INFO/WARNING/ERROR variants print to
 * stderr and include file, line, and function information.
 */
#define adl_dprintERROR(fmt, ...) \
    fprintf(stderr, "[Error] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 8, "", __func__, 8, "", __VA_ARGS__)

#define adl_min(a, b) ((a) < (b) ? (a) : (b))
#define adl_max(a, b) ((a) > (b) ? (a) : (b))
#define ADL_IS_ZERO(x) (adl_fabs(x) < ADL_EPS)

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

ADL_DEF void        adl_line_draw(struct Adl_Pixel_Buffer screen, adl_real x1_input, adl_real y1_input, adl_real x2_input, adl_real y2_input, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void        adl_line_draw_no_antialiasing(struct Adl_Pixel_Buffer screen, adl_real x1_input, adl_real y1_input, adl_real x2_input, adl_real y2_input, uint32_t color, struct Adl_Offset_Zoom offzoom);

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

ADL_DEF void adl_line_draw(struct Adl_Pixel_Buffer screen, adl_real x1_input, adl_real y1_input, adl_real x2_input, adl_real y2_input, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    uint8_t r, g, b, a;
    adl_hexargb_to_rgba(color, &r, &g, &b, &a);

    if (adl_fabs(y2_input - y1_input) < adl_fabs(x2_input - x1_input)) {
        if (x2_input < x1_input) {
            adl_real temp = x2_input;
            x2_input = x1_input;
            x1_input = temp;

            temp = y2_input;
            y2_input = y1_input;
            y1_input = temp;
        }

        adl_real dx = x2_input - x1_input;
        adl_real dy = y2_input - y1_input;
        adl_real m = dy / dx;

        adl_real overlap = 1 - ((x1_input + 0.5) - (int)(x1_input + 0.5));
        adl_real dis_start = y1_input - (int)y1_input;
        adl_point_draw(screen, (adl_real)((int)(x1_input + 0.5)), (adl_real)((int)(y1_input)), adl_rgba_to_hexargb(r, g, b, (int)(a * ((adl_real)1 - dis_start) * overlap)), offzoom);
        adl_point_draw(screen, (adl_real)((int)(x1_input + 0.5)), (adl_real)((int)(y1_input) + (adl_real)1), adl_rgba_to_hexargb(r, g, b, (int)(a * (dis_start) * overlap)), offzoom);
        overlap = ((x2_input + 0.5) - (int)(x2_input + 0.5));
        adl_real dis_end = y2_input - (int)y2_input;
        adl_point_draw(screen, (adl_real)((int)(x2_input + 0.5)), (adl_real)((int)(y2_input)), adl_rgba_to_hexargb(r, g, b, (int)(a * ((adl_real)1 - dis_end) * overlap)), offzoom);
        adl_point_draw(screen, (adl_real)((int)(x2_input + 0.5)), (adl_real)((int)(y2_input) + (adl_real)1), adl_rgba_to_hexargb(r, g, b, (int)(a * (dis_end) * overlap)), offzoom);

        for (size_t i = 1; i < dx; i++) {
            adl_real x = x1_input + (adl_real)i;
            adl_real y = y1_input + (adl_real)i * m;
            int ix = (int)x;
            int iy = (int)y;
            adl_real down_dis = y - iy;
            adl_real up_dis   = 1 - down_dis;
            adl_point_draw(screen, (adl_real)ix, (adl_real)iy, adl_rgba_to_hexargb(r, g, b, (int)(a * up_dis)), offzoom);
            adl_point_draw(screen, (adl_real)ix, (adl_real)iy + (adl_real)1, adl_rgba_to_hexargb(r, g, b, (int)(a * down_dis)), offzoom);
        }
    } else {
        if (y2_input < y1_input) {
            adl_real temp = x2_input;
            x2_input = x1_input;
            x1_input = temp;

            temp = y2_input;
            y2_input = y1_input;
            y1_input = temp;
        }

        adl_real dx = x2_input - x1_input;
        adl_real dy = y2_input - y1_input;
        adl_real m = dx / dy;

        adl_real overlap = 1 - ((y1_input + 0.5) - (int)(y1_input + 0.5));
        adl_real dis_start = y1_input - (int)y1_input;
        adl_point_draw(screen, (adl_real)((int)(x1_input)), (adl_real)((int)(y1_input + 0.5)), adl_rgba_to_hexargb(r, g, b, (int)(a * ((adl_real)1 - dis_start) * overlap)), offzoom);
        adl_point_draw(screen, (adl_real)((int)(x1_input) + (adl_real)1), (adl_real)((int)(y1_input + 0.5)), adl_rgba_to_hexargb(r, g, b, (int)(a * (dis_start) * overlap)), offzoom);
        overlap = ((y2_input + 0.5) - (int)(y2_input + 0.5));
        adl_real dis_end = y2_input - (int)y2_input;
        adl_point_draw(screen, (adl_real)((int)(x2_input)), (adl_real)((int)(y2_input + 0.5)), adl_rgba_to_hexargb(r, g, b, (int)(a * ((adl_real)1 - dis_end) * overlap)), offzoom);
        adl_point_draw(screen, (adl_real)((int)(x2_input) + (adl_real)1), (adl_real)((int)(y2_input + 0.5)), adl_rgba_to_hexargb(r, g, b, (int)(a * (dis_end) * overlap)), offzoom);

        for (size_t i = 1; i < dy; i++) {
            adl_real y = y1_input + (adl_real)i;
            adl_real x = x1_input + (adl_real)i * m;
            int ix = (int)x;
            int iy = (int)y;
            adl_real down_dis = x - ix;
            adl_real up_dis   = 1 - down_dis;
            adl_point_draw(screen, (adl_real)ix, (adl_real)iy, adl_rgba_to_hexargb(r, g, b, (int)(a * up_dis)), offzoom);
            adl_point_draw(screen, (adl_real)ix + (adl_real)1, (adl_real)iy, adl_rgba_to_hexargb(r, g, b, (int)(a * down_dis)), offzoom);
        }
    }
}

ADL_DEF void adl_line_draw_no_antialiasing(struct Adl_Pixel_Buffer screen, adl_real x1_input, adl_real y1_input, adl_real x2_input, adl_real y2_input, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    /** 
     * This function is inspired by the Olive.c function developed by 'Tsoding' on his YouTube channel.
     * You can fined the video in this link: https://youtu.be/LmQKZmQh1ZQ?list=PLpM-Dvs8t0Va-Gb0Dp4d9t8yvNFHaKH6N&t=4683.
     */

    int x1 = (int)x1_input;
    int x2 = (int)x2_input;
    int y1 = (int)y1_input;
    int y2 = (int)y2_input;

    int x = x1;
    int y = y1;
    int dx, dy;

    adl_point_draw(screen, (adl_real)x, (adl_real)y, color, offzoom);

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
            adl_point_draw(screen, (adl_real)x, (adl_real)y, color, offzoom);
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
            adl_point_draw(screen, (adl_real)x, (adl_real)y, color, offzoom);
        }
        return;
    }

    /* adl_real m = (adl_real)dy / dx */
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
            adl_point_draw(screen, (adl_real)x, (adl_real)y, color, offzoom);
        }
    }
}

ADL_DEF void adl_point_draw(struct Adl_Pixel_Buffer screen, adl_real x, adl_real y, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    adl_real window_w = (adl_real)screen.cols;
    adl_real window_h = (adl_real)screen.rows;
    adl_real zoom = offzoom.zoom_multiplier;
    
    if (ADL_IS_ZERO(zoom - (adl_real)1)) {
        int ix = (int)(x + offzoom.offset_x);
        int iy = (int)(y + offzoom.offset_y);
        if ((ix >= 0 && iy >= 0) && ((size_t)ix < screen.cols && (size_t)iy < screen.rows)) { /* point is in screen */
            ADL_BUFFER_AT(screen, iy, ix) = adl_alpha_blend(ADL_BUFFER_AT(screen, iy, ix), color);
        }
        return;
    }

    adl_real start_x0 = (x - window_w/2.0f + offzoom.offset_x) * zoom + window_w/2.0f;
    adl_real start_y0 = (y - window_h/2.0f + offzoom.offset_y) * zoom + window_h/2.0f;
    adl_real start_x1 = (x + 1 - window_w/2.0f + offzoom.offset_x) * zoom + window_w/2.0f;
    adl_real start_y1 = (y + 1 - window_h/2.0f + offzoom.offset_y) * zoom + window_h/2.0f;

    int ix0 = (int)adl_floor(adl_min(start_x0, start_x1));
    int iy0 = (int)adl_floor(adl_min(start_y0, start_y1));
    int ix1 = (int)adl_ceil(adl_max(start_x0, start_x1));
    int iy1 = (int)adl_ceil(adl_max(start_y0, start_y1));

    if (offzoom.zoom_multiplier <= 0) return;
    int block = (int)(zoom + (adl_real)0.5);
    if (block < 1) block = 1;

    for (int ix = ix0; ix < ix1; ix++) {
        for (int iy = iy0; iy < iy1; iy++) {
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
