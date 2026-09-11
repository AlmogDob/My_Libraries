#ifndef APL_ATR_BRIDGE_H_
#define APL_ATR_BRIDGE_H_

#ifndef APL_ATR_MALLOC
    #include <stdlib.h>
    #define APL_ATR_MALLOC malloc
#endif
#ifndef APL_ATR_FREE
    #include <stdlib.h>
    #define APL_ATR_FREE free
#endif
#ifndef APL_ATR_REALLOC
    #include <stdlib.h>
    #define APL_ATR_REALLOC realloc
#endif

#if defined(APL_ATR_SINGLE_PRECISION)
    #define APL_SINGLE_PRECISION
    #define ATR_SINGLE_PRECISION
#endif

#ifdef APL_ATR_ASSERT
    #ifndef APL_ASSERT
        #define APL_ASSERT APL_ATR_ASSERT 
    #endif
#endif
#ifndef APL_REALLOC
    #define APL_REALLOC APL_ATR_REALLOC
#endif
#include "./Almog_Platform_Library.h"
#ifndef APL_ATR_ASSERT
    #define APL_ATR_ASSERT APL_ASSERT
#endif
#ifndef APL_ATR_PI
    #define APL_ATR_PI APL_PI
#endif

#define ATR_ASSERT APL_ATR_ASSERT
#define ATR_MALLOC APL_ATR_MALLOC
#define ATR_REALLOC APL_ATR_REALLOC
#define ATR_FREE APL_ATR_FREE
#define ATR_PI APL_ATR_PI
#include "./Almog_Text_Rendering.h"


struct Atr_Pixel_Buffer apl_pixel_buffer_as_atr_pixel_buffer(struct Apl_Pixel_Buffer apl_b);


#endif /*APL_ATR_BRIDGE_H_*/
#ifdef APL_ATR_BRIDGE_IMPLEMENTATION
#undef APL_ATR_BRIDGE_IMPLEMENTATION

struct Atr_Pixel_Buffer apl_pixel_buffer_as_atr_pixel_buffer(struct Apl_Pixel_Buffer apl_b) 
{
    struct Atr_Pixel_Buffer atr_b = {
        .cols = apl_b.cols,
        .rows = apl_b.rows,
        .stride_r = apl_b.stride_r,
        .elements = apl_b.elements,
    };

    return atr_b;
}


#endif /*APL_ATR_BRIDGE_IMPLEMENTATION*/
