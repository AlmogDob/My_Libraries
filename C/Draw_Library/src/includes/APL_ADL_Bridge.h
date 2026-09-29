#ifndef APL_ADL_BRIDGE_H_
#define APL_ADL_BRIDGE_H_

#ifndef APL_ADL_MALLOC
    #include <stdlib.h>
    #define APL_ADL_MALLOC malloc
#endif
#ifndef APL_ADL_FREE
    #include <stdlib.h>
    #define APL_ADL_FREE free
#endif
#ifndef APL_ADL_REALLOC
    #include <stdlib.h>
    #define APL_ADL_REALLOC realloc
#endif

#if defined(APL_ADL_SINGLE_PRECISION)
    #define APL_SINGLE_PRECISION
    #define ADL_SINGLE_PRECISION
#endif

#ifdef APL_ADL_ASSERT
    #ifndef APL_ASSERT
        #define APL_ASSERT APL_ADL_ASSERT 
    #endif
#endif
#ifndef APL_REALLOC
    #define APL_REALLOC APL_ADL_REALLOC
#endif
#include "./Almog_Platform_Library.h"
#ifndef APL_ADL_ASSERT
    #define APL_ADL_ASSERT APL_ASSERT
#endif
#ifndef APL_ADL_PI
    #define APL_ADL_PI APL_PI
#endif

#define ADL_ASSERT APL_ADL_ASSERT
#define ADL_MALLOC APL_ADL_MALLOC
#define ADL_REALLOC APL_ADL_REALLOC
#define ADL_FREE APL_ADL_FREE
#define ADL_PI APL_ADL_PI
#include "./Almog_Draw_Library.h"


struct Adl_Pixel_Buffer apl_pixel_buffer_as_adl_pixel_buffer(struct Apl_Pixel_Buffer apl_b);


#endif /*APL_ADL_BRIDGE_H_*/
#ifdef APL_ADL_BRIDGE_IMPLEMENTATION
#undef APL_ADL_BRIDGE_IMPLEMENTATION

struct Adl_Pixel_Buffer apl_pixel_buffer_as_adl_pixel_buffer(struct Apl_Pixel_Buffer apl_b) 
{
    struct Adl_Pixel_Buffer adl_b = {
        .cols = apl_b.cols,
        .rows = apl_b.rows,
        .stride_r = apl_b.stride_r,
        .elements = apl_b.elements,
    };

    return adl_b;
}


#endif /*APL_ADL_BRIDGE_IMPLEMENTATION*/
