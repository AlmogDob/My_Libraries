#ifndef APL_APNG_BRIDGE_H_
#define APL_APNG_BRIDGE_H_

#ifndef APL_APNG_MALLOC
    #include <stdlib.h>
    #define APL_APNG_MALLOC malloc
#endif
#ifndef APL_APNG_FREE
    #include <stdlib.h>
    #define APL_APNG_FREE free
#endif
#ifndef APL_APNG_REALLOC
    #include <stdlib.h>
    #define APL_APNG_REALLOC realloc
#endif

#if defined(APL_APNG_SINGLE_PRECISION)
    #define APL_SINGLE_PRECISION
#endif

#ifdef APL_APNG_ASSERT
    #ifndef APL_ASSERT
        #define APL_ASSERT APL_APNG_ASSERT 
    #endif
#endif
#ifndef APL_REALLOC
    #define APL_REALLOC APL_APNG_REALLOC
#endif
#include "./Almog_Platform_Library.h"

#ifndef APL_APNG_ASSERT
    #define APL_APNG_ASSERT APL_ASSERT
#endif
#ifndef APL_APNG_PI
    #define APL_APNG_PI APL_PI
#endif

#define APNG_ASSERT APL_APNG_ASSERT
#define APNG_MALLOC APL_APNG_MALLOC
#define APNG_REALLOC APL_APNG_REALLOC
#define APNG_FREE APL_APNG_FREE
#include "./Almog_PNG.h"
/* macro idea from: https://x.com/vkrajacic/status/1749816169736073295 */
struct Apl_Apng_Copy_Pixel_Offset_Zoom {
    apl_real offset_x;
    apl_real offset_y;
    apl_real zoom;
};

#define APL_IS_ZERO(x) (apl_fabs(x) < APL_EPS)

uint32_t                    apl_apng_alpha_blend(uint32_t dst, uint32_t src);
void                       _apl_apng_apng_pixel_buffer_copy_to_apl_pixel_buffer(struct Apl_Pixel_Buffer des, struct Apng_Pixel_Buffer src, struct Apl_Apng_Copy_Pixel_Offset_Zoom offzoom);
#define                     apl_apng_apng_pixel_buffer_copy_to_apl_pixel_buffer(des, src, ...) _apl_apng_apng_pixel_buffer_copy_to_apl_pixel_buffer((des), (src), (struct Apl_Apng_Copy_Pixel_Offset_Zoom){__VA_ARGS__})
void                        apl_apng_pixel_draw(struct Apl_Pixel_Buffer screen, apl_real x, apl_real y, uint32_t color, apl_real offset_x, apl_real offset_y, apl_real zoom);
struct Apng_Pixel_Buffer    apl_apng_apl_pixel_buffer_as_apng_pixel_buffer(struct Apl_Pixel_Buffer apl_b);

#endif /*APL_APNG_BRIDGE_H_*/
#ifdef APL_APNG_BRIDGE_IMPLEMENTATION
#undef APL_APNG_BRIDGE_IMPLEMENTATION

uint32_t apl_apng_alpha_blend(uint32_t dst, uint32_t src)
{
    uint8_t sr, sg, sb, sa;
    uint8_t dr, dg, db;

    apng_packed_argb_to_rgba(src, &sr, &sg, &sb, &sa);
    apng_packed_argb_to_rgba(dst, &dr, &dg, &db, NULL);

    apl_real a = (apl_real)sa / 255.0f;

    int r = (int)((apl_real)dr * (1.0f - a) + (apl_real)sr * a);
    int g = (int)((apl_real)dg * (1.0f - a) + (apl_real)sg * a);
    int b = (int)((apl_real)db * (1.0f - a) + (apl_real)sb * a);

    return apng_rgba_to_packed_argb(r, g, b, 255);
}

void _apl_apng_apng_pixel_buffer_copy_to_apl_pixel_buffer(struct Apl_Pixel_Buffer des, struct Apng_Pixel_Buffer src, struct Apl_Apng_Copy_Pixel_Offset_Zoom offzoom)
{
    if (offzoom.zoom == 0) {
        offzoom.zoom = 1;
    }
    for (size_t r = 0; r < src.rows; r++) {
        for (size_t c = 0; c < src.cols; c++) {
            apl_real y = (apl_real)r;
            apl_real x = (apl_real)c;
            apl_apng_pixel_draw(des, x, y, APNG_PIXEL_BUFFER_AT(src, r, c), offzoom.offset_x, offzoom.offset_y, offzoom.zoom);
        }
    }
}

void apl_apng_pixel_draw(struct Apl_Pixel_Buffer screen, apl_real x, apl_real y, uint32_t color, apl_real offset_x, apl_real offset_y, apl_real zoom)
{
    APL_APNG_ASSERT(zoom > 0);
    if (!(zoom > 0) || screen.cols == 0 || screen.rows == 0) {
        return;
    }
    if ((color >> 24) == 0) {
        return;
    }

    apl_real half_w = (apl_real)screen.cols / 2;
    apl_real half_h = (apl_real)screen.rows / 2;

    apl_real left = (x - half_w + offset_x) * zoom + half_w;
    apl_real top = (y - half_h + offset_y) * zoom + half_h;
    apl_real right = (x + 1 - half_w + offset_x) * zoom + half_w;
    apl_real bottom = (y + 1 - half_h + offset_y) * zoom + half_h;

    /*
     * Cover pixel centers in [left, right) and [top, bottom).
     * Matching rounding at both edges prevents overlap.
     */
    apl_real x0 = apl_ceil(left - (apl_real)0.5);
    apl_real y0 = apl_ceil(top - (apl_real)0.5);
    apl_real x1 = apl_ceil(right - (apl_real)0.5);
    apl_real y1 = apl_ceil(bottom - (apl_real)0.5);

    /* Clip before converting to integer coordinates. */
    x0 = apl_max(x0, (apl_real)0);
    y0 = apl_max(y0, (apl_real)0);
    x1 = apl_min(x1, (apl_real)screen.cols);
    y1 = apl_min(y1, (apl_real)screen.rows);

    if (!(x0 < x1 && y0 < y1)) {
        return;
    }

    size_t ix0 = (size_t)x0;
    size_t iy0 = (size_t)y0;
    size_t ix1 = (size_t)x1;
    size_t iy1 = (size_t)y1;

    bool opaque = (color >> 24) == 255;
    for (size_t iy = iy0; iy < iy1; ++iy) {
        for (size_t ix = ix0; ix < ix1; ++ix) {
            uint32_t *dst = &APL_BUFFER_AT(screen, iy, ix);
            if (opaque) {
                *dst = color;
            } else {
                *dst = apl_apng_alpha_blend(*dst, color);
            }
        }
    }

}

struct Apng_Pixel_Buffer apl_apng_apl_pixel_buffer_as_apng_pixel_buffer(struct Apl_Pixel_Buffer apl_b)
{
    struct Apng_Pixel_Buffer apng_b = {
        .cols = apl_b.cols,
        .rows = apl_b.rows,
        .stride_r = apl_b.stride_r,
        .elements = apl_b.elements,
    };

    return apng_b;
}



#endif /*APL_APNG_BRIDGE_IMPLEMENTATION*/
