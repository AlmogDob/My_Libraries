#ifndef ALMOG_TEXT_RENDERING_H_
#define ALMOG_TEXT_RENDERING_H_

#if defined(_WIN32) || defined(_WIN64) 
    #pragma warning(disable : 4709)
#endif

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#if defined(ATR_SINGLE_PRECISION)
    typedef float atr_real_type;
    #define ATR_EPS   1e-5f
    #define atr_fabs  fabsf
    #define atr_floor floorf
    #define atr_ceil  ceilf
    #define atr_sqrt  sqrtf
    #define atr_cbrt  cbrtf
    #define atr_cos   cosf
    #define atr_sin   sinf
    #define atr_atan2 atan2f
    #define atr_fmod  fmodf
#else 
    typedef double atr_real_type;
    #define ATR_EPS   1e-10
    #define atr_fabs  fabs
    #define atr_floor floor
    #define atr_ceil  ceil
    #define atr_sqrt  sqrt
    #define atr_cbrt  cbrt
    #define atr_cos   cos
    #define atr_sin   sin
    #define atr_atan2 atan2
    #define atr_fmod  fmod
#endif
#define atr_real atr_real_type

#ifndef ATR_PI
    #define ATR_PI (atr_real)3.14159265358979323846
#endif
#ifndef ATR_ASSERT
    #include <assert.h>
    #define ATR_ASSERT assert
#endif

struct Atr_Pixel_Buffer {
    size_t rows;
    size_t cols;
    size_t stride_r;
    uint32_t *elements;
};

#define atr_dprintSTRING(expr) printf("[Info] %s:%d:\n%*s" #expr " = %s\n", __FILE__, __LINE__, 7, "", expr)
#define atr_dprintCHAR(expr) printf("[Info] %s:%d:\n%*s" #expr " = %c\n", __FILE__, __LINE__, 7, "", expr)
#define atr_dprintINT(expr) printf("[Info] %s:%d:\n%*s" #expr " = %d\n", __FILE__, __LINE__, 7, "", expr)
#define atr_dprintFLOAT(expr) printf("[Info] %s:%d:\n%*s" #expr " = %#f\n", __FILE__, __LINE__, 7, "", expr)
#define atr_dprintDOUBLE(expr) printf("[Info] %s:%d:\n%*s" #expr " = %#g\n", __FILE__, __LINE__, 7, "", expr)
#define atr_dprintSIZE_T(expr) printf("[Info] %s:%d:\n%*s" #expr " = %zu\n", __FILE__, __LINE__, 7, "", expr)
#define atr_dprintINFO(fmt, ...) \
    fprintf(stderr, "[Info] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 7, "", __func__, 7, "", __VA_ARGS__)
#define atr_dprintWARNING(fmt, ...) \
    fprintf(stderr, "[Warning] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 10, "", __func__, 10, "", __VA_ARGS__)
#define atr_dprintERROR(fmt, ...) \
    fprintf(stderr, "[Error] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 8, "", __func__, 8, "", __VA_ARGS__)

#define atr_min(a, b) ((a) < (b) ? (a) : (b))
#define atr_max(a, b) ((a) > (b) ? (a) : (b))
#define ATR_IS_ZERO(x) (atr_fabs(x) < ATR_EPS)
#define ATR_BUFFER_AT(m, i, j) (m).elements[(ATR_ASSERT((i) < (m).rows && (j) < (m).cols), (i) * (m).stride_r + (j))]

#ifndef ATR_DEF
    #ifdef ATR_DEF_STATIC
        #define ATR_DEF static
    #else
        #define ATR_DEF extern
    #endif
#endif


#endif /*ALMOG_TEXT_RENDERING_H_*/

#ifdef ALMOG_TEXT_RENDERING_IMPLEMENTATION
#undef ALMOG_TEXT_RENDERING_IMPLEMENTATION



#endif /*ALMOG_TEXT_RENDERING_IMPLEMENTATION*/
