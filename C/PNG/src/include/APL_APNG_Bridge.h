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

uint32_t    apl_apng_alpha_blend(uint32_t dst, uint32_t src);
void        _apl_apng_apng_pixel_buffer_copy_to_apl_pixel_buffer(struct Apl_Pixel_Buffer des, struct Apng_Pixel_Buffer src, struct Apl_Apng_Copy_Pixel_Offset_Zoom offzoom);
#define     apl_apng_apng_pixel_buffer_copy_to_apl_pixel_buffer(des, src, ...) _apl_apng_apng_pixel_buffer_copy_to_apl_pixel_buffer((des), (src), (struct Apl_Apng_Copy_Pixel_Offset_Zoom){__VA_ARGS__})
void        apl_apng_pixel_draw(struct Apl_Pixel_Buffer screen, apl_real x, apl_real y, uint32_t color, apl_real offset_x, apl_real offset_y, apl_real zoom);


#endif /*APL_APNG_BRIDGE_H_*/
#ifdef APL_APNG_BRIDGE_IMPLEMENTATION
#undef APL_APNG_BRIDGE_IMPLEMENTATION

uint32_t apl_apng_alpha_blend(uint32_t dst, uint32_t src)
{
    uint8_t sr, sg, sb, sa;
    uint8_t dr, dg, db;

    apng_hexargb_to_rgba(src, &sr, &sg, &sb, &sa);
    apng_hexargb_to_rgba(dst, &dr, &dg, &db, NULL);

    apl_real a = (apl_real)sa / 255.0f;

    int r = (int)((apl_real)dr * (1.0f - a) + (apl_real)sr * a);
    int g = (int)((apl_real)dg * (1.0f - a) + (apl_real)sg * a);
    int b = (int)((apl_real)db * (1.0f - a) + (apl_real)sb * a);

    return apng_rgba_to_hexargb(r, g, b, 255);
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

    apl_real window_w = (apl_real)screen.cols;
    apl_real window_h = (apl_real)screen.rows;
    
    if (APL_IS_ZERO(zoom - (apl_real)1)) {
        int ix = (int)(x + offset_x);
        int iy = (int)(y + offset_y);
        if ((ix >= 0 && iy >= 0) && ((size_t)ix < screen.cols && (size_t)iy < screen.rows)) { /* vec2 is in screen */
            APL_BUFFER_AT(screen, iy, ix) = apl_apng_alpha_blend(APL_BUFFER_AT(screen, iy, ix), color);
        }
        return;
    }

    apl_real start_x0 = (x - window_w/2.0f + offset_x) * zoom + window_w/2.0f;
    apl_real start_y0 = (y - window_h/2.0f + offset_y) * zoom + window_h/2.0f;
    apl_real start_x1 = (x + 1 - window_w/2.0f + offset_x) * zoom + window_w/2.0f;
    apl_real start_y1 = (y + 1 - window_h/2.0f + offset_y) * zoom + window_h/2.0f;

    int ix0 = (int)apl_floor(apl_min(start_x0, start_x1));
    int iy0 = (int)apl_floor(apl_min(start_y0, start_y1));
    int ix1 = (int)apl_ceil(apl_max(start_x0, start_x1));
    int iy1 = (int)apl_ceil(apl_max(start_y0, start_y1));

    for (int ix = ix0; ix < ix1; ix++) {
        for (int iy = iy0; iy < iy1; iy++) {
            if ((ix >= 0 && iy >= 0) && ((size_t)ix < screen.cols && (size_t)iy < screen.rows)) { /* vec2 is in screen */
                APL_BUFFER_AT(screen, iy, ix) = apl_apng_alpha_blend(APL_BUFFER_AT(screen, iy, ix), color);
            }
        }
    }

}


#endif /*APL_APNG_BRIDGE_IMPLEMENTATION*/
