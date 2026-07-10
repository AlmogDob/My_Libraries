/**
 * @file
 * @brief Cross-platform platform-layer library for creating a window, processing
 * input, managing a software pixel buffer, and presenting frames to the screen.
 *
 * This file is a single-header platform layer. It contains both:
 * - the public interface (header section), and
 * - the optional implementation (enabled by defining
 *   `ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION` before including the file).
 *
 * The library is designed to provide a minimal application framework for
 * software-rendered programs. It handles:
 * - creation of the main window,
 * - platform-specific event processing,
 * - keyboard and mouse input state collection,
 * - frame timing and frame-rate limiting,
 * - management of a pixel buffer and depth buffer,
 * - copying the software-rendered image to the screen.
 *
 * The current implementation is primarily Windows-based. A Linux section exists
 * as a placeholder for future implementation.
 *
 * Typical usage pattern:
 * @code
 * #define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
 * #include "almog_platform_library.h"
 *
 * enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
 * {
 *     // initialize user resources
 *     return APL_SUCCESS;
 * }
 *
 * enum Apl_Return_Types apl_update(struct Apl_Window_State *ws)
 * {
 *     // update simulation/game state
 *     return APL_SUCCESS;
 * }
 *
 * enum Apl_Return_Types apl_render(struct Apl_Window_State *ws)
 * {
 *     // write pixels into ws->window_pixels_mat
 *     return APL_SUCCESS;
 * }
 * @endcode
 *
 * User-provided callbacks:
 * - `apl_setup()`   : called once after the window and buffers are initialized.
 * - `apl_input()`   : called once per frame after OS messages are processed.
 * - `apl_update()`  : called once per frame for simulation/state updates.
 * - `apl_render()`  : called once per frame to draw into the software buffer.
 * - `apl_destroy()` : called on shutdown for cleanup.
 *
 * The platform layer owns the main loop on Windows and calls these user
 * callbacks at the appropriate times.
 *
 * @note The pixel buffer is stored in CPU memory and copied to the window using
 * platform APIs. This is a software-rendering oriented design.
 *
 * @warning Resizing the window reallocates the pixel and depth buffers. Any
 * pointers into those buffers become invalid after resizing.
 */


/** Structure of the file:
 * 
 * ################################################################################
 * ################################################################################
 * HEADER
 * ################################################################################
 * ################################################################################
 * 
 * #ifndef ALMOG_PLATFORM_LIBRARY_H_
 * #define ALMOG_PLATFORM_LIBRARY_H_
 * 
 * shared header (except for the Platform_State definition)
 * 
 * --------------------------------------------------------------------------------
 * #if defined(_WIN32) || defined(_WIN64) //platform.h
 * --------------------------------------------------------------------------------
 * window header
 * 
 * --------------------------------------------------------------------------------
 * #elif defined(__linux__) //platform.h
 * --------------------------------------------------------------------------------
 * Linux header
 * 
 * --------------------------------------------------------------------------------
 * #endif //platform.h
 * --------------------------------------------------------------------------------
 * #endif //ALMOG_PLATFORM_LIBRARY_H_
 * 
 * 
 * ################################################################################
 * ################################################################################
 * IMPLEMENTATION
 * ################################################################################
 * ################################################################################
 * 
 * #ifdef ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
 * #undef ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
 * 
 * shared implementation
 * 
 * --------------------------------------------------------------------------------
 * #if defined(_WIN32) || defined(_WIN64) //platform.c
 * --------------------------------------------------------------------------------
 * window implementation
 * 
 * --------------------------------------------------------------------------------
 * #elif defined(__linux__) //platform.c
 * --------------------------------------------------------------------------------
 * Linux implementation
 * 
 * --------------------------------------------------------------------------------
 * #endif //platform.c
 * --------------------------------------------------------------------------------
 * #endif //ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
 */



/**
 * ################################################################################
 * ################################################################################
 * HEADER
 * ################################################################################
 * ################################################################################
 */
#ifndef ALMOG_PLATFORM_LIBRARY_H_
#define ALMOG_PLATFORM_LIBRARY_H_

/* -------------------------------------------------------------------------------- */
#if defined(_WIN32) || defined(_WIN64) /* PLATFORM_STATE_H_ */
/* -------------------------------------------------------------------------------- */
/**
 * The Windows version is based on Handmade Hero, Casey Muratori's famous video series. 
 * A link to the playlist on YouTube: https://youtube.com/playlist?list=PLnuhp3Xd9PYTt6svyQPyRO_AAuMWGxPzU&si=4QyqgS-84Dr8d8Cu
 * A link to the website: https://mollyrocket.com/#handmade
 */

#define APL_PLATFORM_NAME "Windows"

#include <windows.h>
#include <mmsystem.h> /* for 1ms sleep */
#include <dbghelp.h> /* for SIGSEGV help */
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Gdi32.lib")

struct Platform_State {
    WNDCLASS window_class;
    HWND window_handle;
    HDC device_context;
    WNDPROC window_call_back;
    BITMAPINFO bit_map_info;
    size_t previous_frame_ticks;
};


/* -------------------------------------------------------------------------------- */
#elif defined(__linux__) /* PLATFORM_STATE_H_ */
/* -------------------------------------------------------------------------------- */

#define APL_PLATFORM_NAME "Linux"

struct Platform_State {
    int x;
};

/* -------------------------------------------------------------------------------- */
#endif /* PLATFORM_STATE_H_*/
/* -------------------------------------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#ifndef APL_REALLOC
#include <stdlib.h>
#define APL_REALLOC realloc
#endif

enum Apl_Return_Types {
    APL_SUCCESS,
    APL_FAIL,
};

#ifndef apl_real
    #if defined(APL_SINGLE_PRECISION)
        typedef float apl_real_type;
    #else 
        typedef double apl_real_type;
    #endif
    #define apl_real apl_real_type
#endif

/**
 * @brief 2D pixel buffer used as the software-render target.
 *
 * The buffer is stored as a contiguous array of 32-bit pixels. The intended
 * format in the current Windows implementation is ARGB packed into a
 * `uint32_t`.
 *
 * The dimensions are described by:
 * - `rows`     : number of rows in the buffer,
 * - `cols`     : number of columns in the buffer,
 * - `stride_r` : number of elements between the start of one row and the next.
 *
 * In the current implementation, `stride_r` is equal to `cols`.
 */
struct Apl_Pixel_Buffer {
    size_t rows;
    size_t cols;
    size_t stride_r;
    uint32_t *elements;
};

/**
 * @brief 2D inverse-depth buffer used alongside the pixel buffer.
 *
 * This buffer stores per-pixel depth-related values, typically inverse-Z values,
 * for software rasterization or visibility tests.
 *
 * The dimensions mirror those of the window pixel buffer:
 * - `rows`     : number of rows,
 * - `cols`     : number of columns,
 * - `stride_r` : row stride in elements,
 * - `elements` : contiguous storage for depth values.
 *
 * The buffer is usually cleared each frame when `to_clear_renderer` is enabled.
 */
struct Apl_Depth_Buffer {
    size_t rows;
    size_t cols;
    size_t stride_r;
    apl_real *elements;
};

#define APL_WINDOW_NAME_LEN 256
/**
 * @brief Main application/window state shared between the platform layer and
 * user code.
 *
 * This structure contains:
 * - platform-specific window data,
 * - runtime flags controlling update/render behavior,
 * - frame timing information,
 * - keyboard and mouse state,
 * - current window dimensions,
 * - the software-render pixel buffer,
 * - the inverse-depth buffer,
 * - an opaque pointer for user-owned data.
 *
 * User callbacks receive a pointer to this structure and are expected to read
 * input state, timing data, and draw into `window_pixels_mat`.
 *
 * The platform layer initializes and updates most fields automatically.
 */
struct Apl_Window_State {
    struct Platform_State platform;

    char window_name[APL_WINDOW_NAME_LEN];

    bool running;
    bool to_render;
    bool to_update;
    bool to_limit_fps;
    bool to_clear_renderer;
    bool to_flip_y;

    float delta_time_sec;
    size_t delta_time_micro_sec;
    size_t elapsed_time_micro_sec;
    size_t previous_frame_time_micro_sec;
    float fps;
    float wanted_fps;

    struct {
        bool space_bar_is_pressed;
        bool q_is_pressed;
        bool w_is_pressed;
        bool e_is_pressed;
        bool r_is_pressed;
        bool a_is_pressed;
        bool s_is_pressed;
        bool d_is_pressed;
        bool up_is_pressed;
        bool down_is_pressed;
        bool left_is_pressed;
        bool right_is_pressed;
    } buttons;

    struct {
        bool left_button_is_pressed;
        bool right_button_is_pressed;
        int mouse_x;
        int mouse_y;
    } mouse;

    size_t window_w;
    size_t window_h;

    struct Apl_Pixel_Buffer window_pixels_mat;
    struct Apl_Depth_Buffer inv_z_buffer_mat;

    void * user_data;
};

#define APL_OK APL_SUCCESS
#define APL_UNUSED(x) (void)x
#ifndef APL_DEF
    #ifdef APL_STATIC
        #define APL_DEF static
    #else
        #define APL_DEF extern
    #endif
#endif
#ifndef APL_ASSERT
#define APL_ASSERT(expr) apl_my_assert(expr, #expr, __FILE__, __LINE__, __func__)
#endif

#define apl_dprintSTRING(expr) printf("[Info] %s:%d:\n" #expr " = %s\n", __FILE__, __LINE__, expr)
#define apl_dprintCHAR(expr) printf("[Info] %s:%d:\n" #expr " = %c\n", __FILE__, __LINE__, expr)
#define apl_dprintINT(expr) printf("[Info] %s:%d:\n" #expr " = %d\n", __FILE__, __LINE__, expr)
#define apl_dprintFLOAT(expr) printf("[Info] %s:%d:\n" #expr " = %#g\n", __FILE__, __LINE__, expr)
#define apl_dprintDOUBLE(expr) printf("[Info] %s:%d:\n" #expr " = %#g\n", __FILE__, __LINE__, expr)
#define apl_dprintSIZE_T(expr) printf("[Info] %s:%d:\n" #expr " = %zu\n", __FILE__, __LINE__, expr)
#define apl_dprintINFO(fmt, ...) \
    fprintf(stderr, "[Info] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 7, "", __func__, 7, "", __VA_ARGS__)
#define apl_dprintWARNING(fmt, ...) \
    fprintf(stderr, "[Warning] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 10, "", __func__, 10, "", __VA_ARGS__)
#define apl_dprintERROR(fmt, ...) \
    fprintf(stderr, "[Error] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 8, "", __func__, 8, "", __VA_ARGS__)

#define apl_min(a, b) ((a) < (b) ? (a) : (b))
#define apl_max(a, b) ((a) > (b) ? (a) : (b))

#define APL_BUFFER_AT(m, i, j) (m).elements[(APL_ASSERT((i) < (m).rows && (j) < (m).cols), (i) * (m).stride_r + (j))]

#define APL_INIT_WINDOW_WIDTH 800
#define APL_INIT_WINDOW_HEIGHT 600
#define APL_WANTED_FPS 100
#define APL_DELTA_TIME_MAX_MICRO_SEC (10 * 60 * 1000 * 1000)

#define APL_COLOR_GRAY_hexARGB      0xFF181818
#define APL_COLOR_RED_hexARGB       0xFFFF0000
#define APL_COLOR_GREEN_hexARGB     0xFF00FF00
#define APL_COLOR_BLUE_hexARGB      0xFF0000FF
#define APL_COLOR_PURPLE_hexARGB    0xFFFF00FF
#define APL_COLOR_CYAN_hexARGB      0xFF00FFFF
#define APL_COLOR_YELLOW_hexARGB    0xFFFFFF00
#define APL_COLOR_WHITE_hexARGB     0xFFFFFFFF

#define APL_BACKGROUND_COLOR_hexARGB APL_COLOR_GRAY_hexARGB

/* shared implementation */
APL_DEF const char *            apl_platform_name(void);
APL_DEF struct Apl_Depth_Buffer apl_realloc_depth_buffer(struct Apl_Depth_Buffer m, size_t rows, size_t cols);
APL_DEF struct Apl_Pixel_Buffer apl_realloc_pixel_buffer(struct Apl_Pixel_Buffer m, size_t rows, size_t cols);
APL_DEF enum Apl_Return_Types   apl_window_destroy(struct Apl_Window_State *ws);
APL_DEF enum Apl_Return_Types   apl_window_process_input(struct Apl_Window_State *ws);
APL_DEF enum Apl_Return_Types   apl_window_render(struct Apl_Window_State *ws);

/* shared_platform_implementation */
APL_DEF void                    apl_fix_framerate(struct Apl_Window_State *ws);
APL_DEF enum Apl_Return_Types   apl_initialize_main_window(struct Apl_Window_State *ws);
APL_DEF void                    apl_my_assert(bool expr_bool, const char *expr, const char *file, int line, const char *func);
APL_DEF void                    apl_pixel_mat_copy_to_screen(struct Apl_Window_State *ws);
APL_DEF enum Apl_Return_Types   apl_resize_window_pixel_mat(struct Apl_Window_State *ws, size_t new_w, size_t new_h);
APL_DEF void                    apl_sleep(size_t wait_time_us);
APL_DEF enum Apl_Return_Types   apl_window_setup(struct Apl_Window_State *ws);
APL_DEF enum Apl_Return_Types   apl_window_update(struct Apl_Window_State *ws);

/* user supposed to define: */
        enum Apl_Return_Types   apl_destroy(struct Apl_Window_State *ws);
        enum Apl_Return_Types   apl_input(struct Apl_Window_State *ws);
        enum Apl_Return_Types   apl_render(struct Apl_Window_State *ws);
        enum Apl_Return_Types   apl_setup(struct Apl_Window_State *ws);
        enum Apl_Return_Types   apl_update(struct Apl_Window_State *ws);

/* -------------------------------------------------------------------------------- */
#if defined(_WIN32) || defined(_WIN64) /* PLATFORM_H_ */
/* -------------------------------------------------------------------------------- */

/* specific_platform_implementation */
APL_DEF const char *        apl_get_exception_name(DWORD code);
        LRESULT CALLBACK    apl_main_window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
APL_DEF void                apl_print_module_plus_offset(DWORD64 instruction_pointer);
APL_DEF void                apl_print_stack_trace(void);
        LONG WINAPI         apl_unhandled_exception_filter(EXCEPTION_POINTERS *ep);


/* -------------------------------------------------------------------------------- */
#elif defined(__linux__) /* PLATFORM_H_ */
/* -------------------------------------------------------------------------------- */

/* specific_platform_implementation */

/* -------------------------------------------------------------------------------- */
#endif /* PLATFORM_H_ */
/* -------------------------------------------------------------------------------- */
#endif /* ALMOG_PLATFORM_LIBRARY_H_*/

/**
 * ################################################################################
 * ################################################################################
 * IMPLEMENTATION
 * ################################################################################
 * ################################################################################
 */

#ifdef ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#undef ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION

/**
 * shared implementation
 */ 

/**
 * @brief Returns the name of the active platform implementation.
 *
 * This function exposes a short platform identifier string such as
 * `"Windows"` or `"Linux"`, depending on which platform branch was compiled.
 *
 * It is mainly useful for diagnostics, logging, or startup messages.
 *
 * @return Pointer to a static string describing the current platform.
 *
 * @note The returned string must not be modified or freed by the caller.
 */
APL_DEF const char * apl_platform_name(void)
{
    return APL_PLATFORM_NAME;
}

/**
 * @brief Resizes or allocates a depth buffer.
 *
 * This function updates the dimensions of a depth buffer and reallocates its
 * storage so that it can hold `rows * cols` depth values.
 *
 * It is used together with the pixel buffer during window creation and resize.
 * The returned structure contains updated dimensions and the new storage
 * pointer.
 *
 * @param m Existing depth buffer structure. Its `elements` pointer may be
 * `NULL` for first-time allocation.
 * @param rows Number of rows to allocate.
 * @param cols Number of columns to allocate.
 *
 * @return A new `Apl_Depth_Buffer` value with updated size fields and storage.
 *
 * @warning Reallocation may move the underlying memory. Any old pointers into
 * the previous buffer become invalid.
 *
 * @warning The function asserts if allocation fails.
 */
APL_DEF struct Apl_Depth_Buffer apl_realloc_depth_buffer(struct Apl_Depth_Buffer m, size_t rows, size_t cols)
{
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (apl_real*)APL_REALLOC(m.elements, sizeof(apl_real)*rows*cols);
    APL_ASSERT(m.elements != NULL);
    
    return m;
}

/**
 * @brief Resizes or allocates a pixel buffer.
 *
 * This function updates the dimensions of a pixel buffer and reallocates its
 * storage so that it can hold `rows * cols` pixels.
 *
 * It is typically used when the window is created or resized. The returned
 * buffer structure contains the new metadata and the possibly relocated memory
 * pointer.
 *
 * @param m Existing pixel buffer structure. Its `elements` pointer may be
 * `NULL` for first-time allocation.
 * @param rows Number of rows to allocate.
 * @param cols Number of columns to allocate.
 *
 * @return A new `Apl_Pixel_Buffer` value with updated size fields and storage.
 *
 * @warning Reallocation may move the underlying memory. Any old pointers into
 * `m.elements` become invalid after this call.
 *
 * @warning The function asserts if allocation fails.
 */
APL_DEF struct Apl_Pixel_Buffer apl_realloc_pixel_buffer(struct Apl_Pixel_Buffer m, size_t rows, size_t cols)
{
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (uint32_t*)APL_REALLOC(m.elements, sizeof(uint32_t)*rows*cols);
    APL_ASSERT(m.elements != NULL);
    
    return m;
}

/**
 * @brief Performs application shutdown cleanup through the user callback.
 *
 * This function calls the user-defined `apl_destroy()` function. It exists as
 * the platform layer's shutdown entry point so the main loop can terminate in a
 * uniform way.
 *
 * Typical use:
 * - called once when the main loop finishes,
 * - gives user code a chance to free resources,
 * - returns failure if user cleanup reports an error.
 *
 * @param ws Pointer to the current window/application state.
 *
 * @return `APL_SUCCESS` if cleanup succeeded, otherwise `APL_FAIL`.
 */
APL_DEF enum Apl_Return_Types apl_window_destroy(struct Apl_Window_State *ws)
{
    /*------------------------------------------------------------*/
    if (apl_destroy(ws) != APL_SUCCESS) {
        apl_dprintERROR("%s", "apl_destroy failed");
        return APL_FAIL;
    }
    return APL_SUCCESS;
}

/**
 * @brief Runs the user input-processing callback.
 *
 * This function calls `apl_input()` after platform events have already updated
 * the keyboard and mouse fields inside `Apl_Window_State`.
 *
 * It is intended to be used once per frame from the main loop. The platform
 * layer handles low-level event collection, while this function gives user code
 * a place to interpret that state and react to it.
 *
 * @param ws Pointer to the current window/application state.
 *
 * @return `APL_SUCCESS` if the user callback succeeded, otherwise `APL_FAIL`.
 */
APL_DEF enum Apl_Return_Types apl_window_process_input(struct Apl_Window_State *ws)
{
    if (apl_input(ws) != APL_SUCCESS) {
        apl_dprintERROR("%s", "apl_input failed");
        return APL_FAIL;
    }
    return APL_SUCCESS;
}

/**
 * @brief Clears the render buffers if requested, calls user rendering, and
 * presents the frame to the screen.
 *
 * This function is the main rendering entry point used by the platform loop.
 * Its behavior is:
 * - optionally clear the pixel buffer and depth buffer,
 * - call the user-defined `apl_render()` function,
 * - copy the resulting pixel buffer to the window.
 *
 * It is normally called once per frame when `ws->to_render` is enabled.
 *
 * @param ws Pointer to the current window/application state.
 *
 * @return `APL_SUCCESS` if rendering and presentation succeeded, otherwise
 * `APL_FAIL`.
 *
 * @note The actual drawing is expected to be performed by user code inside
 * `apl_render()`, usually by writing directly into `ws->window_pixels_mat`.
 */
APL_DEF enum Apl_Return_Types apl_window_render(struct Apl_Window_State *ws)
{
    if (ws->to_clear_renderer) {
        memset(ws->window_pixels_mat.elements, 0x18, sizeof(ws->window_pixels_mat.elements[0]) * ws->window_pixels_mat.rows * ws->window_pixels_mat.cols);
        memset(ws->inv_z_buffer_mat.elements, 0x0, sizeof(ws->inv_z_buffer_mat.elements[0]) * ws->inv_z_buffer_mat.rows * ws->inv_z_buffer_mat.cols);
    }
    /*------------------------------------------------------------*/

    if (apl_render(ws) != APL_SUCCESS) {
        apl_dprintERROR("%s", "apl_render failed");
        return APL_FAIL;
    }
            
    /*------------------------------------------------------------*/
    apl_pixel_mat_copy_to_screen(ws);

    return APL_OK;
}

#ifndef APL_DESTROY
#define APL_DESTROY
/**
 * @brief User-defined cleanup callback.
 *
 * The platform layer calls this function during shutdown so the user can free
 * memory, close files, destroy custom resources, or save application state.
 *
 * If the user does not provide an implementation, the library supplies a
 * default stub that does nothing and returns success.
 *
 * @param ws Pointer to the current window/application state.
 *
 * @return `APL_SUCCESS` on success, or `APL_FAIL` if cleanup fails.
 */
enum Apl_Return_Types apl_destroy(struct Apl_Window_State *ws) { APL_UNUSED(ws); return APL_OK; }
#endif

#ifndef APL_INPUT
#define APL_INPUT
/**
 * @brief User-defined per-frame input callback.
 *
 * The platform layer first collects OS input events and updates the keyboard
 * and mouse state fields inside `Apl_Window_State`. It then calls this function
 * so the user can interpret those states and apply game/application logic.
 *
 * Typical uses include:
 * - reacting to pressed keys,
 * - handling mouse interaction,
 * - converting raw button states into higher-level commands.
 *
 * If the user does not provide an implementation, a default stub is used.
 *
 * @param ws Pointer to the current window/application state.
 *
 * @return `APL_SUCCESS` on success, or `APL_FAIL` if input handling fails.
 */
enum Apl_Return_Types apl_input(struct Apl_Window_State *ws) { APL_UNUSED(ws); return APL_OK; }
#endif

#ifndef APL_SETUP
#define APL_SETUP
/**
 * @brief User-defined initialization callback.
 *
 * The platform layer calls this function once after the native window and
 * render buffers have been initialized. It is intended for user startup logic.
 *
 * Typical uses include:
 * - allocating user-owned memory,
 * - initializing application state,
 * - loading assets,
 * - assigning `ws->user_data`.
 *
 * If the user does not provide an implementation, a default stub is used.
 *
 * @param ws Pointer to the current window/application state.
 *
 * @return `APL_SUCCESS` on success, or `APL_FAIL` if setup fails.
 */
enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws) { APL_UNUSED(ws); return APL_OK; }
#endif

#ifndef APL_UPDATE
#define APL_UPDATE
/**
 * @brief User-defined per-frame update callback.
 *
 * This function is called once per frame when updates are enabled. It is
 * intended for non-rendering logic such as:
 * - simulation,
 * - animation,
 * - physics,
 * - game/application state changes.
 *
 * Timing data such as `delta_time_sec` and `elapsed_time_micro_sec` are
 * available through `ws`.
 *
 * If the user does not provide an implementation, a default stub is used.
 *
 * @param ws Pointer to the current window/application state.
 *
 * @return `APL_SUCCESS` on success, or `APL_FAIL` if the update step fails.
 */
enum Apl_Return_Types apl_update(struct Apl_Window_State *ws) { APL_UNUSED(ws); return APL_OK; }
#endif

#ifndef APL_RENDER
#define APL_RENDER
/**
 * @brief User-defined rendering callback.
 *
 * This is where the user draws the current frame, usually by writing directly
 * into `ws->window_pixels_mat` and optionally using `ws->inv_z_buffer_mat`.
 *
 * The platform layer may clear the buffers before calling this function,
 * depending on `ws->to_clear_renderer`. After this function returns, the
 * platform layer presents the pixel buffer to the screen.
 *
 * If the user does not provide an implementation, a default stub is used.
 *
 * @param ws Pointer to the current window/application state.
 *
 * @return `APL_SUCCESS` on success, or `APL_FAIL` if rendering fails.
 */
enum Apl_Return_Types apl_render(struct Apl_Window_State *ws) { APL_UNUSED(ws); return APL_OK; }
#endif

/* -------------------------------------------------------------------------------- */
#if defined(_WIN32) || defined(_WIN64) //PLATFORM_C_
/* -------------------------------------------------------------------------------- */

/**
 * start shared_Windows_implementation
 */ 

/**
 * @brief Updates frame timing information and optionally limits the frame rate.
 *
 * This function measures the time elapsed since the previous frame using a
 * high-resolution timer. If frame limiting is enabled, it waits until the
 * target frame duration has elapsed.
 *
 * It also updates timing fields in `Apl_Window_State`, including:
 * - `delta_time_micro_sec`,
 * - `delta_time_sec`,
 * - `elapsed_time_micro_sec`,
 * - `previous_frame_time_micro_sec`,
 * - `fps`.
 *
 * It is meant to be called once at the end of every frame from the main loop.
 *
 * @param ws Pointer to the current window/application state.
 *
 * @note This function uses `ws->wanted_fps` and `ws->to_limit_fps` to decide
 * whether sleeping/spinning is needed.
 */
APL_DEF void apl_fix_framerate(struct Apl_Window_State *ws)
{
    static LARGE_INTEGER qpc_freq = {0};
    if (qpc_freq.QuadPart == 0) {
        QueryPerformanceFrequency(&qpc_freq);
    }

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    ULONGLONG elapsed_ticks =
        (ULONGLONG)(now.QuadPart - (LONGLONG)ws->platform.previous_frame_ticks);

    ULONGLONG target_ticks = 0;
    if (ws->wanted_fps > 0.0f) {
        target_ticks =
            (ULONGLONG)((double)qpc_freq.QuadPart / (double)ws->wanted_fps);
    }

    if (ws->to_limit_fps && target_ticks > 0 && elapsed_ticks < target_ticks) {
        ULONGLONG remaining_ticks = target_ticks - elapsed_ticks;
        size_t remaining_us =
            (size_t)((remaining_ticks * 1000000ULL) /
                     (ULONGLONG)qpc_freq.QuadPart);

        apl_sleep(remaining_us);

        do {
            QueryPerformanceCounter(&now);
            elapsed_ticks =
                (ULONGLONG)(now.QuadPart -
                            (LONGLONG)ws->platform.previous_frame_ticks);
        } while (elapsed_ticks < target_ticks);
    } else {
        QueryPerformanceCounter(&now);
        elapsed_ticks =
            (ULONGLONG)(now.QuadPart -
                        (LONGLONG)ws->platform.previous_frame_ticks);
    }

    ws->platform.previous_frame_ticks = (size_t)now.QuadPart;

    ws->delta_time_micro_sec =
        (size_t)((elapsed_ticks * 1000000ULL) /
                 (ULONGLONG)qpc_freq.QuadPart);

    ws->delta_time_sec = (float)ws->delta_time_micro_sec / 1000000.0f;

    ws->previous_frame_time_micro_sec =
        (size_t)(((ULONGLONG)now.QuadPart * 1000000ULL) /
                 (ULONGLONG)qpc_freq.QuadPart);

    if (ws->delta_time_micro_sec <= APL_DELTA_TIME_MAX_MICRO_SEC) {
        ws->elapsed_time_micro_sec += ws->delta_time_micro_sec;
    }

    ws->fps = (ws->delta_time_sec > 0.0f) ? (1.0f / ws->delta_time_sec) : 0.0f;
}

/**
 * @brief Creates and initializes the main application window.
 *
 * This function performs the platform-specific setup required to open the main
 * window. In the Windows implementation it:
 * - registers a window class,
 * - creates the native window,
 * - stores the platform handles in `ws->platform`,
 * - acquires the device context used for presenting pixels.
 *
 * It is typically called once during startup before buffer allocation and
 * before the user setup callback.
 *
 * @param ws Pointer to the window/application state to initialize.
 *
 * @return `APL_SUCCESS` if the window was successfully created, otherwise
 * `APL_FAIL`.
 */
APL_DEF enum Apl_Return_Types apl_initialize_main_window(struct Apl_Window_State *ws)
{
    WNDCLASS window_class = {
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = apl_main_window_callback,
        .hInstance = GetModuleHandleA(NULL),
        .hCursor   = LoadCursorA(NULL, IDC_ARROW),
        .lpszClassName = "apl_window_class", 
    };
    if (!RegisterClassA(&window_class)) {
        apl_dprintERROR("%s", "register window class filed\n");
        return APL_FAIL;
    }
    ws->platform.window_class = window_class;
    APL_UNUSED(window_class);
    ws->platform.window_call_back = apl_main_window_callback;

    HWND window_handle = CreateWindowExA(0,
                                         ws->platform.window_class.lpszClassName,
                                         ws->window_name,
                                         WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                         CW_USEDEFAULT,
                                         CW_USEDEFAULT,
                                         APL_INIT_WINDOW_WIDTH,
                                         APL_INIT_WINDOW_HEIGHT,
                                         0,
                                         0,
                                         ws->platform.window_class.hInstance,
                                         ws);
    if (!window_handle) {
        apl_dprintERROR("%s", "failed to create window\n");
        return APL_FAIL;
    }
    ws->platform.window_handle = window_handle;
    APL_UNUSED(window_handle);

    ws->platform.device_context = GetDC(ws->platform.window_handle);

    return APL_SUCCESS;
}

/**
 * @brief Custom assertion failure handler.
 *
 * This function is called by the `APL_ASSERT` macro when an assertion fails.
 * It prints a diagnostic message containing:
 * - the failed expression,
 * - source file,
 * - source line,
 * - function name,
 * and then prints a stack trace before aborting the process.
 *
 * It is intended as a debugging aid during development.
 *
 * @param expr String form of the failed expression.
 * @param file Source file where the assertion failed.
 * @param line Source line where the assertion failed.
 * @param func Function name where the assertion failed.
 *
 * @warning This function does not return. It terminates the program.
 */
APL_DEF void apl_my_assert(bool expr_bool, const char *expr, const char *file, int line, const char *func)
{
    if (!expr_bool) {
        apl_dprintERROR("Assertion failed: %s At %s:%d In function '%s'.\n"
                        "        Call stack:", expr, file, line, func);
        apl_print_stack_trace();
        abort();
    }
}

/**
 * @brief Copies the software-rendered pixel buffer to the native window.
 *
 * This function presents the current contents of `ws->window_pixels_mat` on the
 * screen using the platform-specific drawing API.
 *
 * It is typically called automatically by `apl_window_render()`, and also in
 * paint events when the OS requests the window contents to be redrawn.
 *
 * @param ws Pointer to the current window/application state.
 *
 * @note This function does not generate pixels by itself. It only transfers the
 * already-rendered buffer to the display.
 */
APL_DEF void apl_pixel_mat_copy_to_screen(struct Apl_Window_State *ws)
{
    StretchDIBits(ws->platform.device_context,
                  0, 0, (int)ws->window_w              , (int)ws->window_h,
                  0, 0, (int)ws->window_pixels_mat.cols, (int)ws->window_pixels_mat.rows,
                  ws->window_pixels_mat.elements,
                  &ws->platform.bit_map_info,
                  DIB_RGB_COLORS, SRCCOPY);
}

/**
 * @brief Resizes the software-render buffers to match the current window size.
 *
 * This function updates the stored window dimensions, reconfigures the bitmap
 * metadata used for presentation, and reallocates both:
 * - the color pixel buffer, and
 * - the inverse-depth buffer.
 *
 * It is typically called:
 * - once during initialization,
 * - whenever the window receives a resize event.
 *
 * After resizing the buffers, the function renders the window once so the
 * displayed contents match the new size.
 *
 * @param ws Pointer to the current window/application state.
 * @param new_w New window width in pixels.
 * @param new_h New window height in pixels.
 *
 * @return `APL_SUCCESS` if the resize and reallocation succeeded, otherwise
 * `APL_FAIL`.
 *
 * @warning Buffer reallocation may invalidate any saved pointers into
 * `window_pixels_mat.elements` or `inv_z_buffer_mat.elements`.
 */
APL_DEF enum Apl_Return_Types apl_resize_window_pixel_mat(struct Apl_Window_State *ws, size_t new_w, size_t new_h)
{
    ws->window_w = apl_max(new_w, 1); /* 1 so the pixel mat want be null */
    ws->window_h = apl_max(new_h, 1); /* 1 so the pixel mat want be null */
    APL_UNUSED(new_w);
    APL_UNUSED(new_h);
    
    ws->platform.bit_map_info.bmiHeader.biSize = (LONG)sizeof(ws->platform.bit_map_info.bmiHeader);
    ws->platform.bit_map_info.bmiHeader.biWidth = (LONG)ws->window_w;
    ws->platform.bit_map_info.bmiHeader.biHeight = (-1 * !ws->to_flip_y + 1 * ws->to_flip_y) * (LONG)ws->window_h; /* minus for the origin to be in the top left corner */
    ws->platform.bit_map_info.bmiHeader.biPlanes = 1;
    ws->platform.bit_map_info.bmiHeader.biBitCount = 32;
    ws->platform.bit_map_info.bmiHeader.biCompression = BI_RGB;

    ws->window_pixels_mat = apl_realloc_pixel_buffer(ws->window_pixels_mat, ws->window_h, ws->window_w);
    if (!ws->window_pixels_mat.elements) {
        apl_dprintERROR("%s", "realloc pixel mat failed");
        return APL_FAIL;
    }
    ws->inv_z_buffer_mat = apl_realloc_depth_buffer(ws->inv_z_buffer_mat, ws->window_h, ws->window_w);
    if (!ws->inv_z_buffer_mat.elements) {
        apl_dprintERROR("%s", "realloc inverse z buffer mat failed");
        return APL_FAIL;
    }

    return apl_window_render(ws);
}

/**
 * @brief Sleeps or spins for approximately the requested duration.
 *
 * This helper function waits for a time interval expressed in microseconds.
 * The implementation uses a combination of coarse sleeping and short busy
 * spinning to achieve better timing precision than `Sleep()` alone.
 *
 * It is mainly used by `apl_fix_framerate()` to cap the frame rate.
 *
 * @param wait_time_us Requested wait duration in microseconds.
 *
 * @note The actual wait time may be slightly longer or shorter depending on OS
 * scheduling and timer precision.
 */
APL_DEF void apl_sleep(size_t wait_time_us)
{
    if (wait_time_us == 0) {
        return;
    }

    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    uint64_t start_ticks = counter.QuadPart;
    uint64_t target_ticks = start_ticks + ((wait_time_us * (uint64_t)freq.QuadPart) / 1000000ULL);

    for (;;) {
        QueryPerformanceCounter(&counter);
        uint64_t now_ticks = counter.QuadPart;
        if (now_ticks >= target_ticks) {
            break;
        }

        uint64_t remaining_ticks = target_ticks - now_ticks;
        uint64_t remaining_us = (remaining_ticks * 1000000ULL) / (uint64_t)freq.QuadPart;

        /*
         * Sleep only while we still have a comfortable margin left.
         * Leave the last ~2 ms for spinning to reduce overshoot.
         */
        if (remaining_us > 3000ULL) {
            DWORD sleep_ms = (DWORD)((remaining_us - 2000ULL) / 1000ULL);
            if (sleep_ms > 0) {
                Sleep(sleep_ms);
            } else {
                SwitchToThread();
            }
        } else {
            /*
             * Final precise wait: spin.
             * You can use YieldProcessor() on x86/x64 to be nicer to the CPU.
             */
            #if defined(_M_IX86) || defined(_M_X64)
                YieldProcessor();
            #endif
        }
    }
}

/**
 * @brief Performs initial window-dependent setup and then calls the user setup
 * callback.
 *
 * This function:
 * - queries the current client-area size,
 * - allocates the pixel and depth buffers to match that size,
 * - calls the user-defined `apl_setup()` function.
 *
 * It is intended to be called once after the native window is successfully
 * created.
 *
 * @param ws Pointer to the current window/application state.
 *
 * @return `APL_SUCCESS` if setup succeeded, otherwise `APL_FAIL`.
 */
APL_DEF enum Apl_Return_Types apl_window_setup(struct Apl_Window_State *ws)
{
    RECT client_rect = {0};
    GetClientRect(ws->platform.window_handle, &client_rect); /* client rect are the pixel we can draw at */
    /* allocate a pixel mat */
    if (apl_resize_window_pixel_mat(ws, client_rect.right - client_rect.left, client_rect.bottom - client_rect.top) != APL_SUCCESS) {
        apl_dprintERROR("%s", "resize window pixel mat failed");
        return APL_FAIL;
    }
    
    /*------------------------------------------------------------*/

    if (apl_setup(ws) != APL_SUCCESS) {
        apl_dprintERROR("%s", "apl_setup failed");
        return APL_FAIL;
    }
    return APL_OK;
}

/**
 * @brief Updates per-frame window state and runs the user update callback.
 *
 * This function is called once per frame when updates are enabled. It currently
 * updates the window title with timing information and then calls the
 * user-defined `apl_update()` function.
 *
 * The title shows either:
 * - the current FPS, when frame limiting is enabled, or
 * - the measured frame time in milliseconds.
 *
 * @param ws Pointer to the current window/application state.
 *
 * @return `APL_SUCCESS` if the update step succeeded, otherwise `APL_FAIL`.
 */
APL_DEF enum Apl_Return_Types apl_window_update(struct Apl_Window_State *ws)
{
    char temp_buf[APL_WINDOW_NAME_LEN];
    char fps_count[APL_WINDOW_NAME_LEN];
    if (!ws->to_limit_fps) {
        sprintf(fps_count, "dt = %5.02f [ms]", ws->delta_time_micro_sec / 1000.0f);
    } else {
        sprintf(fps_count, "FPS = %5.02f", ws->fps);
    }

    snprintf(temp_buf, APL_WINDOW_NAME_LEN, "%s | %s", ws->window_name, fps_count);
    // if (!(ws->elapsed_time_micro_sec % 20)) {
        if (ws->to_render) SetWindowTextA(ws->platform.window_handle, temp_buf);
    // }

    /*------------------------------------------------------------*/

    if (apl_update(ws) != APL_SUCCESS) {
        apl_dprintERROR("%s", "apl_update failed");
        return APL_FAIL;
    }

    return APL_OK;
}

/**
 * end shared_Windows_implementation
 */ 

/**
 * start specific_Windows_implementation
 */ 

/**
 * @brief Converts a Windows exception code into a human-readable name.
 *
 * This helper is used by the unhandled exception filter to print clearer crash
 * diagnostics for common fatal exceptions such as access violations and stack
 * overflows.
 *
 * @param code Windows structured exception code.
 *
 * @return Pointer to a static string describing the exception code.
 */
APL_DEF const char * apl_get_exception_name(DWORD code)
{
    switch (code) {
        case EXCEPTION_ACCESS_VIOLATION:
            return "EXCEPTION_ACCESS_VIOLATION";
        case EXCEPTION_IN_PAGE_ERROR:
            return "EXCEPTION_IN_PAGE_ERROR";
        case EXCEPTION_STACK_OVERFLOW:
            return "EXCEPTION_STACK_OVERFLOW";
        default:
            return "UNKNOWN_EXCEPTION";
    }
}

/**
 * @brief Main Win32 window procedure for the application window.
 *
 * This callback receives and handles native Windows messages for the main
 * window. It is responsible for:
 * - storing the `Apl_Window_State` pointer in window user data,
 * - handling resize events,
 * - updating keyboard and mouse state,
 * - reacting to close and destroy messages,
 * - repainting the window when required.
 *
 * The function is registered as the window procedure during window creation.
 *
 * @param window Handle to the native window.
 * @param message Win32 message identifier.
 * @param wparam Additional message-specific information.
 * @param lparam Additional message-specific information.
 *
 * @return A Win32 `LRESULT` according to the processed message.
 *
 * @note Messages not explicitly handled are forwarded to `DefWindowProcA()`.
 */
LRESULT CALLBACK apl_main_window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    struct Apl_Window_State *ws = (struct Apl_Window_State *)GetWindowLongPtrA(window, GWLP_USERDATA);
    LRESULT result = 0;
    
    switch (message)
    { 
        case WM_NCCREATE: {
            CREATESTRUCTA *cs = (CREATESTRUCTA *)lparam;
            SetWindowLongPtrA(window, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
            result = 1;
        } break;
        case WM_SIZE:
        {
            if (ws != NULL) {
                RECT client_rect = {0};
                GetClientRect(ws->platform.window_handle, &client_rect); /* client rect are the pixel we can draw at */
                if (apl_resize_window_pixel_mat(ws, client_rect.right - client_rect.left, client_rect.bottom - client_rect.top) != APL_SUCCESS) {
                    apl_dprintERROR("%s", "resize window pixel mat failed");
                    PostQuitMessage(0);
                    ws->running = false;
                }
            } else {
                result = DefWindowProcA(window, message, wparam, lparam);
            }
        } break;
        case WM_CLOSE:
        {
            ws->running = false;
        } break;
        case WM_DESTROY:
        {
            ws->running = false;
        } break;
        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            BeginPaint(window, &paint);
            apl_pixel_mat_copy_to_screen(ws);
            EndPaint(window, &paint);
        } break;
        case WM_SYSKEYUP:
        /* fall through */
        case WM_KEYUP:
        /* fall through */
        case WM_SYSKEYDOWN:
        /* fall through */
        case WM_KEYDOWN:
        {
            WPARAM VK_code = wparam;
            bool was_down = ((lparam & (1 << 30)) != 0);
            bool is_down = ((lparam & (1 << 31)) == 0);
            if (is_down == was_down) {
                break;
            }
            switch (VK_code) {
                case 'Q':
                {
                    if (is_down)  ws->buttons.q_is_pressed = true;
                    if (was_down) ws->buttons.q_is_pressed = false;
                } break;
                case 'W':
                {
                    if (is_down)  ws->buttons.w_is_pressed = true;
                    if (was_down) ws->buttons.w_is_pressed = false;
                } break;
                case 'E':
                {
                    if (is_down)  ws->buttons.e_is_pressed = true;
                    if (was_down) ws->buttons.e_is_pressed = false;
                } break;
                case 'R':
                {
                    if (is_down)  ws->buttons.r_is_pressed = true;
                    if (was_down) ws->buttons.r_is_pressed = false;
                } break;
                case 'A':
                {
                    if (is_down)  ws->buttons.a_is_pressed = true;
                    if (was_down) ws->buttons.a_is_pressed = false;
                } break;
                case 'S':
                {
                    if (is_down)  ws->buttons.s_is_pressed = true;
                    if (was_down) ws->buttons.s_is_pressed = false;
                } break;
                case 'D':
                {
                    if (is_down)  ws->buttons.d_is_pressed = true;
                    if (was_down) ws->buttons.d_is_pressed = false;
                } break;
                case VK_UP:
                {
                    if (is_down)  ws->buttons.up_is_pressed = true;
                    if (was_down) ws->buttons.up_is_pressed = false;
                } break;
                case VK_DOWN:
                {
                    if (is_down)  ws->buttons.down_is_pressed = true;
                    if (was_down) ws->buttons.down_is_pressed = false;
                } break;
                case VK_LEFT:
                {
                    if (is_down)  ws->buttons.left_is_pressed = true;
                    if (was_down) ws->buttons.left_is_pressed = false;
                } break;
                case VK_RIGHT:
                {
                    if (is_down)  ws->buttons.right_is_pressed = true;
                    if (was_down) ws->buttons.right_is_pressed = false;
                } break;
                case VK_SPACE:
                {
                    if (is_down)  ws->buttons.space_bar_is_pressed = true;
                    if (was_down) ws->buttons.space_bar_is_pressed = false;
                } break;
                case VK_ESCAPE:
                {
                    PostQuitMessage(0);
                    ws->running = false;
                } break;
                default:
                {
                    result = DefWindowProcA(window, message, wparam, lparam);
                } break;
            }
        } break;
        case WM_LBUTTONDOWN:
        {
            ws->mouse.left_button_is_pressed = true;
        } break;
        case WM_LBUTTONUP:
        {
            ws->mouse.left_button_is_pressed = false;
        } break;
        case WM_RBUTTONDOWN:
        {
            ws->mouse.right_button_is_pressed = true;
        } break;
        case WM_RBUTTONUP:
        {
            ws->mouse.right_button_is_pressed = false;
        } break;
        case WM_MOUSEMOVE:
        {
            ws->mouse.mouse_x = (int)(short)LOWORD(lparam);
            ws->mouse.mouse_y = (int)(short)HIWORD(lparam);
        } break;
        default:
        {
            result = DefWindowProcA(window, message, wparam, lparam);
        } break;
    }
    return result;
}

/**
 * @brief Prints the module name and relative address for an instruction pointer.
 *
 * This helper is used during crash reporting to show at least the executable or
 * DLL module and the offset of the faulting instruction, even if symbol lookup
 * is incomplete.
 *
 * @param instruction_pointer Instruction address to analyze.
 */
APL_DEF void apl_print_module_plus_offset(DWORD64 instruction_pointer)
{
    HMODULE mod = NULL;

    if (!GetModuleHandleExA( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                            (LPCSTR)(uintptr_t)instruction_pointer, &mod) && mod) {
        fprintf(stderr, "Module: (unknown) @ %#llx\n",
                (unsigned long long)instruction_pointer);
    }
    char path[MAX_PATH] = {0};
    DWORD n = GetModuleFileNameA(mod, path, (DWORD)sizeof(path));
    if (n == 0) {
        strncpy(path, "<unknown module>", sizeof(path) - 1);
    }

    DWORD64 base = (DWORD64)(uintptr_t)mod;
    DWORD64 offset = instruction_pointer - base;

    // Print basename if possible (optional; keep full path if you prefer).
    const char *base_name = path;
    for (const char *p = path; *p; ++p) {
        if (*p == '\\' || *p == '/') base_name = p + 1;
    }

    fprintf(stderr, "Module: %s + %#llx (base=%#llx)\n",
            base_name,
            (unsigned long long)offset,
            (unsigned long long)base);
} 

/**
 * @brief Captures and prints a stack trace for the current thread.
 *
 * This function uses the Windows debugging APIs to walk the current call stack
 * and print symbol names and source-line information when available.
 *
 * It is mainly used by the custom assertion handler to aid debugging.
 *
 * @note Symbol resolution depends on debug information being available, such as
 * PDB files.
 */
APL_DEF void apl_print_stack_trace(void)
{
    void *stack[64];
    USHORT frames = CaptureStackBackTrace(0, 64, stack, NULL);
    HANDLE process = GetCurrentProcess();

    static int sym_initialized = 0;
    if (!sym_initialized) {
        SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
        if (!SymInitialize(process, NULL, TRUE)) {
            fprintf(stderr, "SymInitialize failed\n");
            return;
        }
        sym_initialized = 1;
    }

    SYMBOL_INFO *symbol =
        (SYMBOL_INFO *)calloc(1, sizeof(SYMBOL_INFO) + 256);
    if (!symbol) {
        fprintf(stderr, "Out of memory\n");
        return;
    }

    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = 255;

    for (USHORT i = 0; i < frames; i++) {
        DWORD64 address = (DWORD64)(stack[i]);
        DWORD64 displacement = 0;
        DWORD line_displacement = 0;

        IMAGEHLP_LINE64 line;
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

        if (SymFromAddr(process, address, &displacement, symbol)) {
            fprintf(stderr, "#%u %s + 0x%llx",
                    i,
                    symbol->Name,
                    (unsigned long long)displacement);
        } else {
            fprintf(stderr, "#%u 0x%llx",
                    i,
                    (unsigned long long)address);
        }

        if (SymGetLineFromAddr64(process,
                                 address,
                                 &line_displacement,
                                 &line)) {
            fprintf(stderr, " (%s:%lu)",
                    line.FileName,
                    (unsigned long)line.LineNumber);
        }

        fprintf(stderr, "\n");
    }

    free(symbol);
}

/**
 * @brief Process-wide unhandled exception filter for fatal crash reporting.
 *
 * This function is installed at startup using `SetUnhandledExceptionFilter()`.
 * When a fatal structured exception occurs, it:
 * - identifies the exception type,
 * - extracts the faulting instruction pointer,
 * - tries to resolve function and source-line information,
 * - prints diagnostic details,
 * - terminates the process.
 *
 * It is intended to make hard crashes easier to debug during development.
 *
 * @param ep Pointer to Windows exception information.
 *
 * @return `EXCEPTION_CONTINUE_SEARCH` for unsupported exceptions, otherwise the
 * process terminates after reporting the error.
 *
 * @warning This function may exit the process and therefore should not be
 * treated as a normal recoverable error path.
 */
LONG WINAPI apl_unhandled_exception_filter(EXCEPTION_POINTERS *ep)
{
    DWORD code = ep->ExceptionRecord->ExceptionCode;

    if (code != EXCEPTION_ACCESS_VIOLATION &&
        code != EXCEPTION_IN_PAGE_ERROR &&
        code != EXCEPTION_STACK_OVERFLOW) {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    #if defined(_M_X64) || defined(__x86_64__)
        DWORD64 instruction_pointer = (DWORD64)ep->ContextRecord->Rip;
    #elif defined(_M_IX86) || defined(__i386__)
        DWORD64 instruction_pointer = (DWORD64)ep->ContextRecord->Eip;
    #else
        DWORD64 instruction_pointer = 0;
    #endif

    fprintf(stderr, "\nFatal exception: %s (%#08lx)\n",
            apl_get_exception_name(code), code);
    fprintf(stderr, "Faulting Instruction Pointer: %#llx\n",
            (unsigned long long)instruction_pointer);

    HANDLE process = GetCurrentProcess();
    BOOL sym_ok = FALSE;

    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);

    if (!SymInitialize(process, NULL, TRUE)) {
        fprintf(stderr, "SymInitialize failed; gle=%lu\n", GetLastError());
        goto exit;
    }
    sym_ok = TRUE;

    char buf[sizeof(SYMBOL_INFO) + MAX_SYM_NAME] = {0};
    SYMBOL_INFO *sym = (SYMBOL_INFO *)buf;
    sym->SizeOfStruct = sizeof(SYMBOL_INFO);
    sym->MaxNameLen = MAX_SYM_NAME;

    DWORD64 displacement64 = 0;

    IMAGEHLP_LINE64 line = {0};
    line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    DWORD displacement32 = 0;

    BOOL have_symbol = SymFromAddr(process, instruction_pointer,
                                   &displacement64, sym);
    if (have_symbol) {
        fprintf(stderr, "Function: %s + %#llx\n",
                sym->Name,
                (unsigned long long)displacement64);
    } else {
        // print module+offset as a baseline.
        apl_print_module_plus_offset(instruction_pointer);
        fprintf(stderr, "Function: (no symbols)\n");
    }

    BOOL have_line = SymGetLineFromAddr64(process, instruction_pointer,
                                          &displacement32, &line);
    if (have_line) {
        fprintf(stderr, "Location: %s:%lu (+%lu/col %lu)\n",
                line.FileName,
                line.LineNumber,
                (unsigned long)displacement32,
                (unsigned long)displacement32);
    } else {
        fprintf(stderr, "Location: (no line info; ensure PDBs are available)\n");
        // Fallback already printed module+offset above.
    }

exit:
    if (sym_ok) {
        SymCleanup(process);
    }

    ExitProcess(1);
}

/**
 * end specific_Windows_implementation
 */ 

/**
 * ============================================================
 * start Windows_main
 * ============================================================
 */

/**
 * @brief Program entry point for the Windows build.
 *
 * This function initializes the platform layer and runs the main application
 * loop. Its responsibilities include:
 * - installing the unhandled exception filter,
 * - increasing timer precision,
 * - initializing the main window state,
 * - creating the native window,
 * - running setup,
 * - processing the Win32 message queue,
 * - calling input, update, render, and frame-timing functions each frame,
 * - calling shutdown cleanup on exit.
 *
 * The loop continues while `window_state.running` remains true.
 *
 * @return Process exit code based on the last `Apl_Return_Types` result.
 */
int main(void) 
{
    SetUnhandledExceptionFilter(apl_unhandled_exception_filter);
    MMRESULT timer_res = timeBeginPeriod(1);

    printf("hello from %s\n", apl_platform_name());

    struct Apl_Window_State window_state = {0};
    enum Apl_Return_Types rt;

    /**
     * initializing the window state
     */
    window_state.to_render = true;
    window_state.to_update = true;
    window_state.to_limit_fps = true;
    window_state.to_clear_renderer = true;
    window_state.wanted_fps = APL_WANTED_FPS;
    window_state.window_w = APL_INIT_WINDOW_WIDTH;
    window_state.window_h = APL_INIT_WINDOW_HEIGHT;

    strncpy(window_state.window_name, "apl window", APL_WINDOW_NAME_LEN);
    rt = apl_initialize_main_window(&window_state);
    if (rt == APL_FAIL) {
        apl_dprintERROR("%s", "failed to initialize main window");
        window_state.running = false;
    } else {
        window_state.running = true;
    }

    rt = apl_window_setup(&window_state);
    if (rt == APL_FAIL) {
        apl_dprintERROR("%s", "failed to preform window setup");
        window_state.running = false;
    } else {
        window_state.running = true;
    }

    MSG message;
    for ( ; window_state.running ; ) {
        /* flash all the messages in the que */
        for ( ; PeekMessageA(&message, 0, 0, 0, PM_REMOVE) ; ) {
            if (message.message == WM_QUIT) {
                window_state.running = false;
                rt = APL_FAIL;
                break; 
            }
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        if (window_state.running == false) {
            break;
        }

        /* the loop */
        rt = apl_window_process_input(&window_state);
        if (rt == APL_FAIL) {
            apl_dprintERROR("%s", "failed to precess input");
            break;
        }
        if (window_state.to_update) {
            rt = apl_window_update(&window_state);
            if (rt == APL_FAIL) {
                apl_dprintERROR("%s", "failed to window update");
                break;
            }
        }
        if (window_state.to_render) {
            rt = apl_window_render(&window_state);
            if (rt == APL_FAIL) {
                apl_dprintERROR("%s", "failed to window render");
                break;
            }
        }
        apl_fix_framerate(&window_state);
        /*
        if (game_state->elapsed_time*10-(int)(game_state->elapsed_time*10) < 0.1) {
            SDL_SetWindowTitle(game_state->window, fps_count);
        }
        */
    }

    rt = apl_window_destroy(&window_state);
    if (rt == APL_FAIL) {
        apl_dprintERROR("%s", "failed to window destroy");
    }

    if (timer_res == TIMERR_NOERROR) {
        timeEndPeriod(1);
    }

    return rt;
}
/**
 * ============================================================
 * end Windows_main
 * ============================================================
 */


/* -------------------------------------------------------------------------------- */
#elif defined(__linux__) //PLATFORM_C_
/* -------------------------------------------------------------------------------- */
/**
 * start shared_Linux_implementation
 */ 
/**
 * end shared_Linux_implementation
 */ 
/**
 * start specific_Linux_implementation
 */ 
/**
 * end specific_Linux_implementation
 */ 

/* -------------------------------------------------------------------------------- */
#endif //PLATFORM_C_
/* -------------------------------------------------------------------------------- */
#endif /* ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION*/

