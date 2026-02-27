/* -------------------------------------------------------------------------------- */
#if defined(_WIN32) || defined(_WIN64)
/* -------------------------------------------------------------------------------- */
#ifndef ALMOG_PLATFORM_LIBRARY_H_
#define ALMOG_PLATFORM_LIBRARY_H_

/**
 * The Windows version is based on Handmade Hero, Casey Muratori's famous video series. 
 * A link to the playlist on YouTube: https://youtube.com/playlist?list=PLnuhp3Xd9PYTt6svyQPyRO_AAuMWGxPzU&si=4QyqgS-84Dr8d8Cu
 * A link to the website: https://mollyrocket.com/#handmade
 */

#define APL_PLATFORM_NAME "Windows"

#include <windows.h>
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Gdi32.lib")

#ifdef APL_DEFINE_ALL_IMPLEMENTATIONS
    #define MATRIX2D_IMPLEMENTATION
    #define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
    #define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#endif
#include "Matrix2D.h"
#include "Almog_Draw_Library.h"

enum Apl_Return_Types {
    APL_SUCCESS,
    APL_FAIL,
};

struct Apl_Window_State {
    struct {
        WNDCLASS window_class;
        HWND window_handle;
        WNDPROC window_call_back;
        BITMAPINFO bit_map_info;
    } platform;

    bool running;
    bool to_render;
    bool to_update;
    size_t previous_frame_time;
    bool to_limit_fps;
    bool to_clear_renderer;

    float delta_time;
    float elapsed_time;
    float fps;
    float const_fps;
    float frame_target_time;

    bool space_bar_was_pressed;
    bool w_was_pressed;
    bool s_was_pressed;
    bool a_was_pressed;
    bool d_was_pressed;
    bool e_was_pressed;
    bool q_was_pressed;
    bool left_button_pressed;

    size_t window_w;
    size_t window_h;

    Mat2D_uint32 window_pixels_mat;
};

#define APL_UNUSED(x) (void)x

#define apl_dprintINFO(fmt, ...) \
    fprintf(stderr, "[Info] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 7, "", __func__, 7, "", __VA_ARGS__)
#define apl_dprintWARNING(fmt, ...) \
    fprintf(stderr, "[Warning] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 10, "", __func__, 10, "", __VA_ARGS__)
#define apl_dprintERROR(fmt, ...) \
    fprintf(stderr, "[Error] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 8, "", __func__, 8, "", __VA_ARGS__)

#define APL_INIT_WINDOW_WIDTH 800
#define APL_INIT_WINDOW_HEIGHT 600
#define APL_TARGET_FPS 100
#define APL_FRAME_TARGET_TIME (1000 / APL_TARGET_FPS)

#define APL_COLOR_GRAY_hexARGB      0xFF181818
#define APL_COLOR_RED_hexARGB       0xFFFF0000
#define APL_COLOR_GREEN_hexARGB     0xFF00FF00
#define APL_COLOR_BLUE_hexARGB      0xFF0000FF
#define APL_COLOR_PURPLE_hexARGB    0xFFFF00FF
#define APL_COLOR_CYAN_hexARGB      0xFF00FFFF
#define APL_COLOR_YELLOW_hexARGB    0xFFFFFF00
#define APL_COLOR_WHITE_hexARGB     0xFFFFFFFF

#define APL_BACKGROUND_COLOR_hexARGB APL_COLOR_GRAY_hexARGB



char * apl_platform_name(void);

#endif /* ALMOG_PLATFORM_LIBRARY_H_*/

#ifdef ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#undef ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION

char * apl_platform_name(void)
{
    return APL_PLATFORM_NAME;
}


#endif /* ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION*/

/* -------------------------------------------------------------------------------- */
#elif defined(__linux__)
/* -------------------------------------------------------------------------------- */

#ifndef ALMOG_PLATFORM_LIBRARY_H_
#define ALMOG_PLATFORM_LIBRARY_H_

#define APL_PLATFORM_NAME "Linux"






#define APL_INIT_WINDOW_WIDTH 800
#define APL_INIT_WINDOW_HEIGHT 600
#define APL_TARGET_FPS 100
#define APL_FRAME_TARGET_TIME (1000 / APL_TARGET_FPS)

#define apl_dprintINFO(fmt, ...) \
    fprintf(stderr, "[Info] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 7, "", __func__, 7, "", __VA_ARGS__)
#define apl_dprintWARNING(fmt, ...) \
    fprintf(stderr, "[Warning] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 10, "", __func__, 10, "", __VA_ARGS__)
#define apl_dprintERROR(fmt, ...) \
    fprintf(stderr, "[Error] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 8, "", __func__, 8, "", __VA_ARGS__)

#define APL_UNUSED(x) (void)x





char * apl_platform_name(void);

#endif /* ALMOG_PLATFORM_LIBRARY_H_*/

#ifdef ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#undef ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION

char * apl_platform_name(void)
{
    return APL_PLATFORM_NAME;
}


#endif /* ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION*/

#endif /* platform */