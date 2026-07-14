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
    #define adl_sqrt  sqrtf
    #define adl_cbrt  cbrtf
    #define adl_cos   cosf
    #define adl_sin   sinf
    #define adl_atan2 atan2f
    #define adl_fmod  fmodf
#else 
    typedef double adl_real_type;
    #define ADL_EPS   1e-10
    #define adl_fabs  fabs
    #define adl_floor floor
    #define adl_ceil  ceil
    #define adl_sqrt  sqrt
    #define adl_cbrt  cbrt
    #define adl_cos   cos
    #define adl_sin   sin
    #define adl_atan2 atan2
    #define adl_fmod  fmod
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
    adl_real zoom_multiplier;
    adl_real offset_x;
    adl_real offset_y;
};

struct Adl_Vec2 {
    adl_real x;
    adl_real y;
};

struct Adl_Vec2_Dynamic_Array {
    size_t length;
    size_t capacity;
    struct Adl_Vec2 *elements;
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

#define adl_dprintSTRING(expr) printf("[Info] %s:%d:\n%*s" #expr " = %s\n", __FILE__, __LINE__, 7, "", expr)
#define adl_dprintCHAR(expr) printf("[Info] %s:%d:\n%*s" #expr " = %c\n", __FILE__, __LINE__, 7, "", expr)
#define adl_dprintINT(expr) printf("[Info] %s:%d:\n%*s" #expr " = %d\n", __FILE__, __LINE__, 7, "", expr)
#define adl_dprintFLOAT(expr) printf("[Info] %s:%d:\n%*s" #expr " = %#f\n", __FILE__, __LINE__, 7, "", expr)
#define adl_dprintDOUBLE(expr) printf("[Info] %s:%d:\n%*s" #expr " = %#g\n", __FILE__, __LINE__, 7, "", expr)
#define adl_dprintSIZE_T(expr) printf("[Info] %s:%d:\n%*s" #expr " = %zu\n", __FILE__, __LINE__, 7, "", expr)
#define adl_dprintINFO(fmt, ...) \
    fprintf(stderr, "[Info] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 7, "", __func__, 7, "", __VA_ARGS__)
#define adl_dprintWARNING(fmt, ...) \
    fprintf(stderr, "[Warning] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 10, "", __func__, 10, "", __VA_ARGS__)
#define adl_dprintERROR(fmt, ...) \
    fprintf(stderr, "[Error] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 8, "", __func__, 8, "", __VA_ARGS__)

#define adl_min(a, b) ((a) < (b) ? (a) : (b))
#define adl_max(a, b) ((a) > (b) ? (a) : (b))
#define ADL_IS_ZERO(x) (adl_fabs(x) < ADL_EPS)

#define ADL_BUFFER_AT(m, i, j) (m).elements[(ADL_ASSERT((i) < (m).rows && (j) < (m).cols), (i) * (m).stride_r + (j))]

#define ADL_HexARGB_EXPEND_TO_RGBA(x) ((x)>>(8*2)&0xFF), ((x)>>(8*1)&0xFF), ((x)>>(8*0)&0xFF), ((x)>>(8*3)&0xFF)
#define ADL_HexARGB_EXPEND_TO_RGB(x) ((x)>>(8*2)&0xFF), ((x)>>(8*1)&0xFF), ((x)>>(8*0)&0xFF)
#define ADL_VEC2_EXPEND_TO_XY(p) (p).x, (p).y

#define ADL_COLOR_BLACK_hexARGB  0xFF000000
#define ADL_COLOR_BLUE_hexARGB   0xFF0000FF
#define ADL_COLOR_CYAN_hexARGB   0xFF00FFFF
#define ADL_COLOR_GREEN_hexARGB  0xFF00FF00
#define ADL_COLOR_PURPLE_hexARGB 0xFFFF00FF
#define ADL_COLOR_RED_hexARGB    0xFFFF0000
#define ADL_COLOR_YELLOW_hexARGB 0xFFFFFF00
#define ADL_COLOR_WHITE_hexARGB  0xFFFFFFFF

#define ADL_DEFAULT_OFFSET_ZOOM (struct Adl_Offset_Zoom){.zoom_multiplier = 1, .offset_x = 0, .offset_y = 0}

#ifndef ADL_DEF
    #ifdef ADL_DEF_STATIC
        #define ADL_DEF static
    #else
        #define ADL_DEF extern
    #endif
#endif

ADL_DEF uint32_t            adl_alpha_blend(uint32_t dst, uint32_t src);
ADL_DEF void                adl_arrow_draw(struct Adl_Pixel_Buffer screen, adl_real xs, adl_real ys, adl_real xe, adl_real ye, adl_real head_size, adl_real head_angle_deg, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_arrows_draw(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 *vec2s, size_t count, adl_real head_size, adl_real head_angle_deg, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_arrows_draw_loop(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 *vec2s, size_t count, adl_real head_size, adl_real head_angle_deg, uint32_t color, struct Adl_Offset_Zoom offzoom);

ADL_DEF void                adl_circle_draw(struct Adl_Pixel_Buffer screen, adl_real center_x, adl_real center_y, adl_real r, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_circle_fill(struct Adl_Pixel_Buffer screen, adl_real center_x, adl_real center_y, adl_real r, uint32_t color, struct Adl_Offset_Zoom offzoom);

ADL_DEF adl_real            adl_edge_cross_vec2(struct Adl_Vec2 a1, struct Adl_Vec2 b, struct Adl_Vec2 a2, struct Adl_Vec2 p);

ADL_DEF void                adl_hexargb_to_rgba(uint32_t color, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);

ADL_DEF uint32_t            adl_interpolate_ARGBcolor_on_okLch(uint32_t color1, uint32_t color2, adl_real t, adl_real num_of_rotations);
ADL_DEF bool                adl_is_left_edge(adl_real x, adl_real y);
ADL_DEF bool                adl_is_top_edge(adl_real x, adl_real y);
ADL_DEF bool                adl_is_top_left(struct Adl_Vec2 vec2_s, struct Adl_Vec2 vec2_e);

ADL_DEF void                adl_line_draw(struct Adl_Pixel_Buffer screen, adl_real x1_input, adl_real y1_input, adl_real x2_input, adl_real y2_input, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_line_draw_fix_width(struct Adl_Pixel_Buffer screen, adl_real x1_input, adl_real y1_input, adl_real x2_input, adl_real y2_input, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_line_draw_no_antialiasing(struct Adl_Pixel_Buffer screen, adl_real x1_input, adl_real y1_input, adl_real x2_input, adl_real y2_input, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_lines_draw(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 *vec2s, size_t count, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_lines_draw_loop(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 *vec2s, size_t count, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_linear_sRGB_to_okLab(uint32_t hex_ARGB, adl_real *L, adl_real *a, adl_real *b);
ADL_DEF void                adl_linear_sRGB_to_okLch(uint32_t hex_ARGB, adl_real *L, adl_real *c, adl_real *h_deg);

ADL_DEF uint32_t            adl_okLab_to_linear_sRGB(adl_real L, adl_real a, adl_real b);
ADL_DEF uint32_t            adl_okLch_to_linear_sRGB(adl_real L, adl_real c, adl_real h_deg);

ADL_DEF void                adl_pixel_draw(struct Adl_Pixel_Buffer screen, adl_real x, adl_real y, uint32_t color, struct Adl_Offset_Zoom offzoom);

ADL_DEF void                adl_quad_draw(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, struct Adl_Vec2 vec23, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_quad_draw_fix_width(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, struct Adl_Vec2 vec23, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_quad_draw_no_antialiasing(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, struct Adl_Vec2 vec23, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_quad_fill_flat_Pinedas_rasterizer(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, struct Adl_Vec2 vec23, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_quad_fill_flat_Pinedas_rasterizer_antialiasing(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, struct Adl_Vec2 vec23, uint32_t color, struct Adl_Offset_Zoom offzoom);

ADL_DEF void                adl_rectangle_draw_min_max(struct Adl_Pixel_Buffer screen, adl_real min_x, adl_real max_x, adl_real min_y, adl_real max_y, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_rectangle_fill_min_max(struct Adl_Pixel_Buffer screen, adl_real min_x, adl_real max_x, adl_real min_y, adl_real max_y, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF uint32_t            adl_rgba_to_hexargb(int r, int g, int b, int a);

ADL_DEF void                adl_tri_draw(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_tri_draw_fix_width(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_tri_draw_no_antialiasing(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_tri_fill_flat_Pinedas_rasterizer(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, uint32_t color, struct Adl_Offset_Zoom offzoom);
ADL_DEF void                adl_tri_fill_flat_Pinedas_rasterizer_antialiasing(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, uint32_t color, struct Adl_Offset_Zoom offzoom);

ADL_DEF uint8_t             adl_u8_clamp_int(int x);

ADL_DEF struct Adl_Vec2     adl_vec2_add_vec2(struct Adl_Vec2 vec21, struct Adl_Vec2 vec22);
ADL_DEF struct Adl_Vec2     adl_vec2_get_from_xy(adl_real x, adl_real y);
ADL_DEF adl_real            adl_vec2_magnitude(struct Adl_Vec2 vec2);
ADL_DEF struct Adl_Vec2     adl_vec2_mult(struct Adl_Vec2 vec2, adl_real x);
ADL_DEF struct Adl_Vec2     adl_vec2_normalize(struct Adl_Vec2 vec2);
#define                     adl_vec2_print(vec2) do {adl_dprintINFO("%s", ""); adl_vec2_print_imp(vec2, #vec2, 7);} while (0)
ADL_DEF void                adl_vec2_print_imp(struct Adl_Vec2 vec2, char *name, size_t padding);
ADL_DEF struct Adl_Vec2     adl_vec2_rotate_around_vec2_XY(struct Adl_Vec2 vec2, struct Adl_Vec2 center, adl_real angle_deg);
ADL_DEF struct Adl_Vec2     adl_vec2_sub_vec2(struct Adl_Vec2 vec21, struct Adl_Vec2 vec22);

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

ADL_DEF void adl_arrow_draw(struct Adl_Pixel_Buffer screen, adl_real xs, adl_real ys, adl_real xe, adl_real ye, adl_real head_size, adl_real head_angle_deg, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    adl_line_draw(screen, xs, ys, xe, ye, color, offzoom);

    struct Adl_Vec2 start = adl_vec2_get_from_xy(xs, ys);
    struct Adl_Vec2 end = adl_vec2_get_from_xy(xe, ye);
    struct Adl_Vec2 diff = adl_vec2_sub_vec2(end, start);
    adl_real line_len = adl_vec2_magnitude(diff);
    if (ADL_IS_ZERO(line_len)) return;
    struct Adl_Vec2 rescaled_diff = adl_vec2_normalize(diff);
    rescaled_diff = adl_vec2_mult(rescaled_diff, - head_size * line_len);

    struct Adl_Vec2 head_base = adl_vec2_add_vec2(rescaled_diff, end);
    struct Adl_Vec2 edge1 = adl_vec2_rotate_around_vec2_XY(head_base, end, head_angle_deg / 2);
    struct Adl_Vec2 edge2 = adl_vec2_rotate_around_vec2_XY(head_base, end, - head_angle_deg / 2);

    adl_line_draw(screen, xe, ye, ADL_VEC2_EXPEND_TO_XY(edge1), color, offzoom);
    adl_line_draw(screen, xe, ye, ADL_VEC2_EXPEND_TO_XY(edge2), color, offzoom);
}

ADL_DEF void adl_arrows_draw(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 *vec2s, size_t count, adl_real head_size, adl_real head_angle_deg, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    for (size_t i = 0; i < count - 1; i++) {
        size_t start = i;
        size_t end   = i + 1;
        adl_arrow_draw(screen, ADL_VEC2_EXPEND_TO_XY(vec2s[start]), ADL_VEC2_EXPEND_TO_XY(vec2s[end]), head_size, head_angle_deg, color, offzoom);
    }
}

ADL_DEF void adl_arrows_draw_loop(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 *vec2s, size_t count, adl_real head_size, adl_real head_angle_deg, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    for (size_t i = 0; i < count; i++) {
        size_t start = i % count;
        size_t end   = (i + 1) % count;
        adl_arrow_draw(screen, ADL_VEC2_EXPEND_TO_XY(vec2s[start]), ADL_VEC2_EXPEND_TO_XY(vec2s[end]), head_size, head_angle_deg, color, offzoom);
    }
}

ADL_DEF void adl_circle_draw(struct Adl_Pixel_Buffer screen, adl_real center_x, adl_real center_y, adl_real r, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    for (adl_real dy = -r; dy <= r; dy+=1) {
        for (adl_real dx = -r; dx <= r; dx+=1) {
            adl_real diff = dx * dx + dy * dy - r * r;
            if (diff < 0 && diff > -r*2) {
                adl_pixel_draw(screen, center_x + dx, center_y + dy, color, offzoom);
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
                adl_pixel_draw(screen, center_x + dx, center_y + dy, color, offzoom);
            }
        }
    }
}

ADL_DEF adl_real adl_edge_cross_vec2(struct Adl_Vec2 a1, struct Adl_Vec2 b, struct Adl_Vec2 a2, struct Adl_Vec2 p)
{
    return (b.x-a1.x)*(p.y-a2.y)-(b.y-a1.y)*(p.x-a2.x);
}

ADL_DEF void adl_hexargb_to_rgba(uint32_t color, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a)
{
    if (a) *a = (uint8_t)((color >> 24) & 0xFF);
    if (r) *r = (uint8_t)((color >> 16) & 0xFF);
    if (g) *g = (uint8_t)((color >> 8) & 0xFF);
    if (b) *b = (uint8_t)((color >> 0) & 0xFF);
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
ADL_DEF uint32_t adl_interpolate_ARGBcolor_on_okLch(uint32_t color1, uint32_t color2, adl_real t, adl_real num_of_rotations)
{
    adl_real L_1, c_1, h_1;
    adl_real L_2, c_2, h_2;
    adl_linear_sRGB_to_okLch(color1, &L_1, &c_1, &h_1);
    adl_linear_sRGB_to_okLch(color2, &L_2, &c_2, &h_2);
    h_2 = h_2 + 360 * num_of_rotations;

    adl_real L, c, h;
    L = L_1 * (1 - t) + L_2 * (t);
    c = c_1 * (1 - t) + c_2 * (t);
    h = h_1 * (1 - t) + h_2 * (t);
    return adl_okLch_to_linear_sRGB(L, c, h);
}

ADL_DEF bool adl_is_left_edge(adl_real x, adl_real y)
{
    (void)x;
    return (y < 0);
}

ADL_DEF bool adl_is_top_edge(adl_real x, adl_real y)
{
    return (y == 0 && x > 0);
}

ADL_DEF bool adl_is_top_left(struct Adl_Vec2 vec2_s, struct Adl_Vec2 vec2_e)
{
    return (adl_is_top_edge(vec2_e.x-vec2_s.x, vec2_e.y-vec2_s.y) || adl_is_left_edge(vec2_e.x-vec2_s.x, vec2_e.y-vec2_s.y));
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
        adl_pixel_draw(screen, (adl_real)((int)(x1_input + 0.5)), (adl_real)((int)(y1_input)), adl_rgba_to_hexargb(r, g, b, (int)(a * ((adl_real)1 - dis_start) * overlap)), offzoom);
        adl_pixel_draw(screen, (adl_real)((int)(x1_input + 0.5)), (adl_real)((int)(y1_input) + (adl_real)1), adl_rgba_to_hexargb(r, g, b, (int)(a * (dis_start) * overlap)), offzoom);
        overlap = ((x2_input + 0.5) - (int)(x2_input + 0.5));
        adl_real dis_end = y2_input - (int)y2_input;
        adl_pixel_draw(screen, (adl_real)((int)(x2_input + 0.5)), (adl_real)((int)(y2_input)), adl_rgba_to_hexargb(r, g, b, (int)(a * ((adl_real)1 - dis_end) * overlap)), offzoom);
        adl_pixel_draw(screen, (adl_real)((int)(x2_input + 0.5)), (adl_real)((int)(y2_input) + (adl_real)1), adl_rgba_to_hexargb(r, g, b, (int)(a * (dis_end) * overlap)), offzoom);

        for (size_t i = 1; i < dx; i++) {
            adl_real x = x1_input + (adl_real)i;
            adl_real y = y1_input + (adl_real)i * m;
            int ix = (int)x;
            int iy = (int)y;
            adl_real down_dis = y - iy;
            adl_real up_dis   = 1 - down_dis;
            adl_pixel_draw(screen, (adl_real)ix, (adl_real)iy, adl_rgba_to_hexargb(r, g, b, (int)(a * up_dis)), offzoom);
            adl_pixel_draw(screen, (adl_real)ix, (adl_real)iy + (adl_real)1, adl_rgba_to_hexargb(r, g, b, (int)(a * down_dis)), offzoom);
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
        adl_pixel_draw(screen, (adl_real)((int)(x1_input)), (adl_real)((int)(y1_input + 0.5)), adl_rgba_to_hexargb(r, g, b, (int)(a * ((adl_real)1 - dis_start) * overlap)), offzoom);
        adl_pixel_draw(screen, (adl_real)((int)(x1_input) + (adl_real)1), (adl_real)((int)(y1_input + 0.5)), adl_rgba_to_hexargb(r, g, b, (int)(a * (dis_start) * overlap)), offzoom);
        overlap = ((y2_input + 0.5) - (int)(y2_input + 0.5));
        adl_real dis_end = y2_input - (int)y2_input;
        adl_pixel_draw(screen, (adl_real)((int)(x2_input)), (adl_real)((int)(y2_input + 0.5)), adl_rgba_to_hexargb(r, g, b, (int)(a * ((adl_real)1 - dis_end) * overlap)), offzoom);
        adl_pixel_draw(screen, (adl_real)((int)(x2_input) + (adl_real)1), (adl_real)((int)(y2_input + 0.5)), adl_rgba_to_hexargb(r, g, b, (int)(a * (dis_end) * overlap)), offzoom);

        for (size_t i = 1; i < dy; i++) {
            adl_real y = y1_input + (adl_real)i;
            adl_real x = x1_input + (adl_real)i * m;
            int ix = (int)x;
            int iy = (int)y;
            adl_real down_dis = x - ix;
            adl_real up_dis   = 1 - down_dis;
            adl_pixel_draw(screen, (adl_real)ix, (adl_real)iy, adl_rgba_to_hexargb(r, g, b, (int)(a * up_dis)), offzoom);
            adl_pixel_draw(screen, (adl_real)ix + (adl_real)1, (adl_real)iy, adl_rgba_to_hexargb(r, g, b, (int)(a * down_dis)), offzoom);
        }
    }
}

ADL_DEF void adl_line_draw_fix_width(struct Adl_Pixel_Buffer screen, adl_real x1_input, adl_real y1_input, adl_real x2_input, adl_real y2_input, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    adl_real window_w = (adl_real)screen.cols;
    adl_real window_h = (adl_real)screen.rows;
    adl_real zoom = offzoom.zoom_multiplier;

    adl_real x1 = (x1_input - window_w/2.0f + offzoom.offset_x) * zoom + window_w/2.0f;
    adl_real y1 = (y1_input - window_h/2.0f + offzoom.offset_y) * zoom + window_h/2.0f;

    adl_real x2 = (x2_input - window_w/2.0f + offzoom.offset_x) * zoom + window_w/2.0f;
    adl_real y2 = (y2_input - window_h/2.0f + offzoom.offset_y) * zoom + window_h/2.0f;

    adl_line_draw(screen, x1, y1, x2, y2, color, ADL_DEFAULT_OFFSET_ZOOM);
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

    adl_pixel_draw(screen, (adl_real)x, (adl_real)y, color, offzoom);

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
            adl_pixel_draw(screen, (adl_real)x, (adl_real)y, color, offzoom);
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
            adl_pixel_draw(screen, (adl_real)x, (adl_real)y, color, offzoom);
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
            adl_pixel_draw(screen, (adl_real)x, (adl_real)y, color, offzoom);
        }
    }
}

ADL_DEF void adl_lines_draw(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 *vec2s, size_t count, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    for (size_t i = 0; i < count - 1; i++) {
        size_t start = i;
        size_t end   = i + 1;
        adl_line_draw(screen, ADL_VEC2_EXPEND_TO_XY(vec2s[start]), ADL_VEC2_EXPEND_TO_XY(vec2s[end]), color, offzoom);
    }
}

ADL_DEF void adl_lines_draw_loop(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 *vec2s, size_t count, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    for (size_t i = 0; i < count; i++) {
        size_t start = i % count;
        size_t end   = (i + 1) % count;
        adl_line_draw(screen, ADL_VEC2_EXPEND_TO_XY(vec2s[start]), ADL_VEC2_EXPEND_TO_XY(vec2s[end]), color, offzoom);
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
ADL_DEF void adl_linear_sRGB_to_okLab(uint32_t hex_ARGB, adl_real *L, adl_real *a, adl_real *b)
{
    /* https://bottosson.github.io/posts/oklab/
       https://en.wikipedia.org/wiki/Oklab_color_space */
    uint8_t R_255, G_255, B_255;
    adl_hexargb_to_rgba(hex_ARGB, &R_255, &G_255, &B_255, NULL);

    adl_real R = (float)R_255;
    adl_real G = (float)G_255;
    adl_real B = (float)B_255;

    adl_real l = 0.4122214705f * R + 0.5363325363f * G + 0.0514459929f * B;
    adl_real m = 0.2119034982f * R + 0.6806995451f * G + 0.1073969566f * B;
    adl_real s = 0.0883024619f * R + 0.2817188376f * G + 0.6299787005f * B;

    adl_real l_ = adl_cbrt(l);
    adl_real m_ = adl_cbrt(m);
    adl_real s_ = adl_cbrt(s);

    *L = 0.2104542553f * l_ + 0.7936177850f * m_ - 0.0040720468f * s_;
    *a = 1.9779984951f * l_ - 2.4285922050f * m_ + 0.4505937099f * s_;
    *b = 0.0259040371f * l_ + 0.7827717662f * m_ - 0.8086757660f * s_;

}

/**
 * @brief Convert a linear sRGB color (ARGB) to OkLch components.
 *
 * @param hex_ARGB Input color (0xAARRGGBB). Alpha is ignored.
 * @param L [out] Lightness in [0,1].
 * @param c [out] Chroma (non-negative).
 * @param h_deg [out] Hue angle in degrees [-180,180] from atan2.
 */
ADL_DEF void adl_linear_sRGB_to_okLch(uint32_t hex_ARGB, adl_real *L, adl_real *c, adl_real *h_deg)
{
    adl_real a, b;
    adl_linear_sRGB_to_okLab(hex_ARGB, L, &a, &b);

    *c = adl_sqrt(a * a + b * b);
    *h_deg = adl_atan2(b, a) * 180.0f / ADL_PI;
}

/**
 * @brief Convert Oklab components to a linear sRGB ARGB color.
 *
 * Output RGB components are clamped to [0,255], alpha is set to 255.
 *
 * @param L Oklab lightness.
 * @param a Oklab a component.
 * @param b Oklab b component.
 * @return hex_ARGB [out] Output color (0xAARRGGBB, A=255).
 */
ADL_DEF uint32_t adl_okLab_to_linear_sRGB(adl_real L, adl_real a, adl_real b)
{
    /* https://bottosson.github.io/posts/oklab/
       https://en.wikipedia.org/wiki/Oklab_color_space */

    adl_real l_ = L + 0.3963377774f * a + 0.2158037573f * b;
    adl_real m_ = L - 0.1055613458f * a - 0.0638541728f * b;
    adl_real s_ = L - 0.0894841775f * a - 1.2914855480f * b;

    adl_real l = l_ * l_ * l_;
    adl_real m = m_ * m_ * m_;
    adl_real s = s_ * s_ * s_;

    adl_real R = + 4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s;
    adl_real G = - 1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s;
    adl_real B = - 0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s;

    R = adl_max(adl_min(R, 255), 0);
    G = adl_max(adl_min(G, 255), 0);
    B = adl_max(adl_min(B, 255), 0);

    return adl_rgba_to_hexargb((int)R, (int)G, (int)B, 0xFF);
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
ADL_DEF uint32_t adl_okLch_to_linear_sRGB(adl_real L, adl_real c, adl_real h_deg)
{
    h_deg = adl_fmod((h_deg + 360), 360);
    adl_real a = c * adl_cos(h_deg * ADL_PI / 180.0f);
    adl_real b = c * adl_sin(h_deg * ADL_PI / 180.0f);
    return adl_okLab_to_linear_sRGB(L, a, b);
}

ADL_DEF void adl_pixel_draw(struct Adl_Pixel_Buffer screen, adl_real x, adl_real y, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    adl_real window_w = (adl_real)screen.cols;
    adl_real window_h = (adl_real)screen.rows;
    adl_real zoom = offzoom.zoom_multiplier;
    
    if (ADL_IS_ZERO(zoom - (adl_real)1)) {
        int ix = (int)(x + offzoom.offset_x);
        int iy = (int)(y + offzoom.offset_y);
        if ((ix >= 0 && iy >= 0) && ((size_t)ix < screen.cols && (size_t)iy < screen.rows)) { /* vec2 is in screen */
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
            if ((ix >= 0 && iy >= 0) && ((size_t)ix < screen.cols && (size_t)iy < screen.rows)) { /* vec2 is in screen */
                ADL_BUFFER_AT(screen, iy, ix) = adl_alpha_blend(ADL_BUFFER_AT(screen, iy, ix), color);
            }
        }
    }
}

ADL_DEF void adl_quad_draw(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, struct Adl_Vec2 vec23, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    adl_line_draw(screen, vec20.x, vec20.y, vec21.x, vec21.y, color, offzoom);
    adl_line_draw(screen, vec21.x, vec21.y, vec22.x, vec22.y, color, offzoom);
    adl_line_draw(screen, vec22.x, vec22.y, vec23.x, vec23.y, color, offzoom);
    adl_line_draw(screen, vec23.x, vec23.y, vec20.x, vec20.y, color, offzoom);
}

ADL_DEF void adl_quad_draw_fix_width(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, struct Adl_Vec2 vec23, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    adl_line_draw_fix_width(screen, vec20.x, vec20.y, vec21.x, vec21.y, color, offzoom);
    adl_line_draw_fix_width(screen, vec21.x, vec21.y, vec22.x, vec22.y, color, offzoom);
    adl_line_draw_fix_width(screen, vec22.x, vec22.y, vec23.x, vec23.y, color, offzoom);
    adl_line_draw_fix_width(screen, vec23.x, vec23.y, vec20.x, vec20.y, color, offzoom);
}

ADL_DEF void adl_quad_draw_no_antialiasing(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, struct Adl_Vec2 vec23, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    adl_line_draw_no_antialiasing(screen, vec20.x, vec20.y, vec21.x, vec21.y, color, offzoom);
    adl_line_draw_no_antialiasing(screen, vec21.x, vec21.y, vec22.x, vec22.y, color, offzoom);
    adl_line_draw_no_antialiasing(screen, vec22.x, vec22.y, vec23.x, vec23.y, color, offzoom);
    adl_line_draw_no_antialiasing(screen, vec23.x, vec23.y, vec20.x, vec20.y, color, offzoom);
}

ADL_DEF void adl_quad_fill_flat_Pinedas_rasterizer(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, struct Adl_Vec2 vec23, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    /* finding bounding box */
    int x_min = (int)adl_min(vec20.x, adl_min(vec21.x, adl_min(vec22.x, vec23.x)));
    int x_max = (int)adl_max(vec20.x, adl_max(vec21.x, adl_max(vec22.x, vec23.x)));
    int y_min = (int)adl_min(vec20.y, adl_min(vec21.y, adl_min(vec22.y, vec23.y)));
    int y_max = (int)adl_max(vec20.y, adl_max(vec21.y, adl_max(vec22.y, vec23.y)));

    /* Clamp to screen bounds */
    if (x_min < 0) x_min = 0;
    if (y_min < 0) y_min = 0;
    if (x_max >= (int)screen.cols) x_max = (int)screen.cols - 1;
    if (y_max >= (int)screen.rows) y_max = (int)screen.rows - 1;

    /* draw only outline of the tri if there is no area */
    adl_real w = adl_edge_cross_vec2(vec20, vec21, vec21, vec22) + adl_edge_cross_vec2(vec22, vec23, vec23, vec20);
    if (ADL_IS_ZERO(w)) {
        return;
    }

    // adl_real size_vec23_to_vec20 = adl_sqrt((vec20.x - vec23.x)*(vec20.x - vec23.x) + (vec20.y - vec23.y)*(vec20.y - vec23.y));
    // adl_real size_vec20_to_vec21 = adl_sqrt((vec21.x - vec20.x)*(vec21.x - vec20.x) + (vec21.y - vec20.y)*(vec21.y - vec20.y));
    // adl_real size_vec21_to_vec22 = adl_sqrt((vec22.x - vec21.x)*(vec22.x - vec21.x) + (vec22.y - vec21.y)*(vec22.y - vec21.y));
    // adl_real size_vec22_to_vec23 = adl_sqrt((vec23.x - vec22.x)*(vec23.x - vec22.x) + (vec23.y - vec22.y)*(vec23.y - vec22.y));

    for (int y = y_min; y <= y_max; y++) {
        for (int x = x_min; x <= x_max; x++) {
            struct Adl_Vec2 vec2 = {.x = (adl_real)x, .y = (adl_real)y};
            bool in_01, in_12, in_23, in_30;

            in_01 = (adl_edge_cross_vec2(vec20, vec21, vec20, vec2) >= 0) != (w < 0);
            in_12 = (adl_edge_cross_vec2(vec21, vec22, vec21, vec2) >= 0) != (w < 0);
            in_23 = (adl_edge_cross_vec2(vec22, vec23, vec22, vec2) >= 0) != (w < 0);
            in_30 = (adl_edge_cross_vec2(vec23, vec20, vec23, vec2) >= 0) != (w < 0);

            // /* https://www.mn.uio.no/math/english/people/aca/michaelf/papers/mv3d.pdf. */
            // adl_real size_vec2_to_vec20 = adl_sqrt((vec20.x - vec2.x)*(vec20.x - vec2.x) + (vec20.y - vec2.y)*(vec20.y - vec2.y));
            // adl_real size_vec2_to_vec21 = adl_sqrt((vec21.x - vec2.x)*(vec21.x - vec2.x) + (vec21.y - vec2.y)*(vec21.y - vec2.y));
            // adl_real size_vec2_to_vec22 = adl_sqrt((vec22.x - vec2.x)*(vec22.x - vec2.x) + (vec22.y - vec2.y)*(vec22.y - vec2.y));
            // adl_real size_vec2_to_vec23 = adl_sqrt((vec23.x - vec2.x)*(vec23.x - vec2.x) + (vec23.y - vec2.y)*(vec23.y - vec2.y));

            // /* tangent of half the angle directly using vector math */
            // adl_real tan_theta_3_over_2 = size_vec23_to_vec20 / (size_vec2_to_vec23 + size_vec2_to_vec20);
            // adl_real tan_theta_0_over_2 = size_vec20_to_vec21 / (size_vec2_to_vec20 + size_vec2_to_vec21);
            // adl_real tan_theta_1_over_2 = size_vec21_to_vec22 / (size_vec2_to_vec21 + size_vec2_to_vec22);
            // adl_real tan_theta_2_over_2 = size_vec22_to_vec23 / (size_vec2_to_vec22 + size_vec2_to_vec23);
            // adl_real w0 = (tan_theta_3_over_2 + tan_theta_0_over_2) / size_vec2_to_vec20;
            // adl_real w1 = (tan_theta_0_over_2 + tan_theta_1_over_2) / size_vec2_to_vec21;
            // adl_real w2 = (tan_theta_1_over_2 + tan_theta_2_over_2) / size_vec2_to_vec22;
            // adl_real w3 = (tan_theta_2_over_2 + tan_theta_3_over_2) / size_vec2_to_vec23;

            // adl_real inv_w_tot = 1.0f / (w0 + w1 + w2 + w3);
            // adl_real alpha = w0 * inv_w_tot;
            // adl_real beta  = w1 * inv_w_tot;
            // adl_real gamma = w2 * inv_w_tot;
            // adl_real delta = w3 * inv_w_tot;

            if (in_01 && in_12 && in_23 && in_30) {
                adl_pixel_draw(screen, (adl_real)x, (adl_real)y, color, offzoom);
            }
        }
    }
}

ADL_DEF void adl_quad_fill_flat_Pinedas_rasterizer_antialiasing(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, struct Adl_Vec2 vec23, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    adl_quad_fill_flat_Pinedas_rasterizer(screen, vec20, vec21, vec22, vec23, color, offzoom);
    adl_quad_draw(screen, vec20, vec21, vec22, vec23, color, offzoom);
}

ADL_DEF void adl_rectangle_draw_min_max(struct Adl_Pixel_Buffer screen, adl_real min_x, adl_real max_x, adl_real min_y, adl_real max_y, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    adl_line_draw_no_antialiasing(screen, min_x, min_y, max_x, min_y, color, offzoom);
    adl_line_draw_no_antialiasing(screen, max_x, min_y, max_x, max_y, color, offzoom);
    adl_line_draw_no_antialiasing(screen, max_x, max_y, min_x, max_y, color, offzoom);
    adl_line_draw_no_antialiasing(screen, min_x, max_y, min_x, min_y, color, offzoom);
}

ADL_DEF void adl_rectangle_fill_min_max(struct Adl_Pixel_Buffer screen, adl_real min_x, adl_real max_x, adl_real min_y, adl_real max_y, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    for (adl_real x = min_x; x <= max_x; x++) {
        for (adl_real y = min_y; y <= max_y; y++) {
            adl_pixel_draw(screen, x, y, color, offzoom);
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

ADL_DEF void adl_tri_draw(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    adl_line_draw(screen, vec20.x, vec20.y, vec21.x, vec21.y, color, offzoom);
    adl_line_draw(screen, vec21.x, vec21.y, vec22.x, vec22.y, color, offzoom);
    adl_line_draw(screen, vec22.x, vec22.y, vec20.x, vec20.y, color, offzoom);
}

ADL_DEF void adl_tri_draw_fix_width(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    adl_line_draw_fix_width(screen, vec20.x, vec20.y, vec21.x, vec21.y, color, offzoom);
    adl_line_draw_fix_width(screen, vec21.x, vec21.y, vec22.x, vec22.y, color, offzoom);
    adl_line_draw_fix_width(screen, vec22.x, vec22.y, vec20.x, vec20.y, color, offzoom);
}

ADL_DEF void adl_tri_draw_no_antialiasing(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    adl_line_draw_no_antialiasing(screen, vec20.x, vec20.y, vec21.x, vec21.y, color, offzoom);
    adl_line_draw_no_antialiasing(screen, vec21.x, vec21.y, vec22.x, vec22.y, color, offzoom);
    adl_line_draw_no_antialiasing(screen, vec22.x, vec22.y, vec20.x, vec20.y, color, offzoom);
}

ADL_DEF void adl_tri_fill_flat_Pinedas_rasterizer(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    /* This function follows the rasterizer of 'Pikuma' shown in his YouTube video. You can fine the video in this link: https://youtu.be/k5wtuKWmV48. */

    /* finding bounding box */
    int x_min = (int)adl_min(vec20.x, adl_min(vec21.x, vec22.x));
    int x_max = (int)adl_max(vec20.x, adl_max(vec21.x, vec22.x));
    int y_min = (int)adl_min(vec20.y, adl_min(vec21.y, vec22.y));
    int y_max = (int)adl_max(vec20.y, adl_max(vec21.y, vec22.y));

    /* Clamp to screen bounds */
    if (x_min < 0) x_min = 0;
    if (y_min < 0) y_min = 0;
    if (x_max >= (int)screen.cols) x_max = (int)screen.cols - 1;
    if (y_max >= (int)screen.rows) y_max = (int)screen.rows - 1;

    /* draw only outline of the tri if there is no area */
    adl_real w = adl_edge_cross_vec2(vec20, vec21, vec21, vec22);
    if (ADL_IS_ZERO(w)) {
        return;
    }

    /* fill conventions */
    int bias0 = adl_is_top_left(vec20, vec21) ? 0 : -1;
    int bias1 = adl_is_top_left(vec21, vec22) ? 0 : -1;
    int bias2 = adl_is_top_left(vec22, vec20) ? 0 : -1;

    for (int y = y_min; y <= y_max; y++) {
        for (int x = x_min; x <= x_max; x++) {
            struct Adl_Vec2 vec2 = {.x = (adl_real)x, .y = (adl_real)y};
            adl_real w0 = adl_edge_cross_vec2(vec20, vec21, vec20, vec2) + bias0;
            adl_real w1 = adl_edge_cross_vec2(vec21, vec22, vec21, vec2) + bias1;
            adl_real w2 = adl_edge_cross_vec2(vec22, vec20, vec22, vec2) + bias2;

            if (w0 * w >= 0 && w1 * w >= 0 &&  w2 * w >= 0) {
                adl_pixel_draw(screen, (adl_real)x, (adl_real)y, color, offzoom);
            }
        }
    }
}

ADL_DEF void adl_tri_fill_flat_Pinedas_rasterizer_antialiasing(struct Adl_Pixel_Buffer screen, struct Adl_Vec2 vec20, struct Adl_Vec2 vec21, struct Adl_Vec2 vec22, uint32_t color, struct Adl_Offset_Zoom offzoom)
{
    adl_tri_fill_flat_Pinedas_rasterizer(screen, vec20, vec21, vec22, color, offzoom);
    adl_tri_draw(screen, vec20, vec21, vec22, color, offzoom);
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

ADL_DEF struct Adl_Vec2 adl_vec2_add_vec2(struct Adl_Vec2 vec21, struct Adl_Vec2 vec22)
{
    return (struct Adl_Vec2){
        .x = vec21.x + vec22.x,
        .y = vec21.y + vec22.y,
    };
}

ADL_DEF struct Adl_Vec2 adl_vec2_get_from_xy(adl_real x, adl_real y)
{
    return (struct Adl_Vec2){
        .x = x,
        .y = y
    };
}

ADL_DEF adl_real adl_vec2_magnitude(struct Adl_Vec2 vec2)
{
    return adl_sqrt((vec2.x) * (vec2.x) + (vec2.y) * (vec2.y));
}

ADL_DEF struct Adl_Vec2 adl_vec2_mult(struct Adl_Vec2 vec2, adl_real x)
{
    return (struct Adl_Vec2){
        .x = vec2.x * x,
        .y = vec2.y * x,
    };
}

ADL_DEF struct Adl_Vec2 adl_vec2_normalize(struct Adl_Vec2 vec2)
{
    adl_real mag = adl_vec2_magnitude(vec2);
    return (struct Adl_Vec2){
        .x = vec2.x / mag,
        .y = vec2.y / mag,
    };
}

ADL_DEF void adl_vec2_print_imp(struct Adl_Vec2 vec2, char *name, size_t padding)
{
    printf("\33[A\33[2K\r");
    printf("%*.s%s: {.x = %g, .y = %g}\n", (int)padding, "", name, ADL_VEC2_EXPEND_TO_XY(vec2));
}

ADL_DEF struct Adl_Vec2 adl_vec2_rotate_around_vec2_XY(struct Adl_Vec2 vec2, struct Adl_Vec2 center, adl_real angle_deg)
{
    adl_real angle_rad = ADL_PI * angle_deg / 180;
    struct Adl_Vec2 diff = adl_vec2_sub_vec2(vec2, center);
    struct Adl_Vec2 rot_diff = {
        .x = diff.x * adl_cos(angle_rad) - diff.y * adl_sin(angle_rad), 
        .y = diff.x * adl_sin(angle_rad) + diff.y * adl_cos(angle_rad), 
    };
    return adl_vec2_add_vec2(center, rot_diff);
}

ADL_DEF struct Adl_Vec2 adl_vec2_sub_vec2(struct Adl_Vec2 vec21, struct Adl_Vec2 vec22)
{
    return (struct Adl_Vec2){
        .x = vec21.x - vec22.x,
        .y = vec21.y - vec22.y,
    };
}


#endif /*ALMOG_DRAW_LIBRARY_IMPLEMENTATION*/
