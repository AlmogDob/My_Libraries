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

#include "Matrix2D.h"

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
#include <dbghelp.h> /* for SIGSEGV help */
#pragma comment(lib, "Dbghelp.lib")
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

enum Apl_Return_Types {
    APL_SUCCESS,
    APL_FAIL,
};

#define APL_WINDOW_NAME_LEN 256
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
        bool left_button_pressed;
    } buttons;

    size_t window_w;
    size_t window_h;

    Mat2D_uint32 window_pixels_mat;
    Mat2D inv_z_buffer_mat;

    void * user_data;
};

#define APL_OK APL_SUCCESS
#define APL_UNUSED(x) (void)x
#ifndef APL_DEF
    #define APL_DEF static inline
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
APL_DEF char *                  apl_platform_name(void);
APL_DEF enum Apl_Return_Types   apl_window_destroy(struct Apl_Window_State *ws);
APL_DEF enum Apl_Return_Types   apl_window_process_input(struct Apl_Window_State *ws);
APL_DEF enum Apl_Return_Types   apl_window_render(struct Apl_Window_State *ws);

/* shared_platform_implementation */
APL_DEF void                    apl_fix_framerate(struct Apl_Window_State *ws);
APL_DEF enum Apl_Return_Types   apl_initialize_main_window(struct Apl_Window_State *ws);
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

APL_DEF char * apl_platform_name(void)
{
    return APL_PLATFORM_NAME;
}

APL_DEF enum Apl_Return_Types apl_window_destroy(struct Apl_Window_State *ws)
{
    /*------------------------------------------------------------*/
    if (apl_destroy(ws) != APL_SUCCESS) {
        apl_dprintERROR("%s", "apl_destroy failed");
        return APL_FAIL;
    }
    return APL_SUCCESS;
}

APL_DEF enum Apl_Return_Types apl_window_process_input(struct Apl_Window_State *ws)
{
    if (apl_input(ws) != APL_SUCCESS) {
        apl_dprintERROR("%s", "apl_input failed");
        return APL_FAIL;
    }
    return APL_SUCCESS;
}

APL_DEF enum Apl_Return_Types apl_window_render(struct Apl_Window_State *ws)
{
    if (ws->to_clear_renderer) {
        memset(ws->window_pixels_mat.elements, 0x18, sizeof(uint32_t) * ws->window_pixels_mat.rows * ws->window_pixels_mat.cols);
        memset(ws->inv_z_buffer_mat.elements, 0x0, sizeof(mat2D_real) * ws->inv_z_buffer_mat.rows * ws->inv_z_buffer_mat.cols);
        // mat2D_fill_uint32(ws->window_pixels_mat, APL_BACKGROUND_COLOR_hexARGB);
        // mat2D_fill(ws->inv_z_buffer_mat, 0);
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
enum Apl_Return_Types apl_destroy(struct Apl_Window_State *ws) { APL_UNUSED(ws); return APL_OK; }
#endif

#ifndef APL_INPUT
#define APL_INPUT
enum Apl_Return_Types apl_input(struct Apl_Window_State *ws) { APL_UNUSED(ws); return APL_OK; }
#endif

#ifndef APL_SETUP
#define APL_SETUP
enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws) { APL_UNUSED(ws); return APL_OK; }
#endif

#ifndef APL_UPDATE
#define APL_UPDATE
enum Apl_Return_Types apl_update(struct Apl_Window_State *ws) { APL_UNUSED(ws); return APL_OK; }
#endif

#ifndef APL_RENDER
#define APL_RENDER
enum Apl_Return_Types apl_render(struct Apl_Window_State *ws) { APL_UNUSED(ws); return APL_OK; }
#endif

/* -------------------------------------------------------------------------------- */
#if defined(_WIN32) || defined(_WIN64) //PLATFORM_C_
/* -------------------------------------------------------------------------------- */

/**
 * start shared_Windows_implementation
 */ 

APL_DEF void apl_fix_framerate(struct Apl_Window_State *ws)
{
    LARGE_INTEGER count_freq;
    QueryPerformanceFrequency(&count_freq);
    LARGE_INTEGER current_count;
    QueryPerformanceCounter(&current_count);
    size_t delta_ticks = current_count.QuadPart - ws->platform.previous_frame_ticks; /* count_freq is in seconds */
    size_t delta_time_from_previous_frame_micro_sec = (1000 * 1000 * delta_ticks) / (count_freq.QuadPart);
    size_t wanted_delta_time_from_previous_frame_micro_sec = (size_t)((1000 * 1000) / ws->wanted_fps);
    
    if (delta_time_from_previous_frame_micro_sec < wanted_delta_time_from_previous_frame_micro_sec && ws->to_limit_fps) {
        size_t time_to_wait_micro_sec = wanted_delta_time_from_previous_frame_micro_sec - delta_time_from_previous_frame_micro_sec;
        apl_sleep((time_to_wait_micro_sec));
    }

    QueryPerformanceCounter(&current_count);
    delta_ticks = current_count.QuadPart - (ws->previous_frame_time_micro_sec / 1000) * (count_freq.QuadPart / 1000); /* count_freq is in seconds */
    ws->delta_time_micro_sec = (1000 * 1000 * delta_ticks) / (count_freq.QuadPart);
    ws->delta_time_sec = ws->delta_time_micro_sec / 1e6f;
    ws->platform.previous_frame_ticks = current_count.QuadPart;
    ws->previous_frame_time_micro_sec = (1000 * current_count.QuadPart) / (count_freq.QuadPart / 1000);
    ws->elapsed_time_micro_sec += ws->delta_time_micro_sec * (ws->delta_time_micro_sec > APL_DELTA_TIME_MAX_MICRO_SEC ? 0 : 1);
    ws->fps = 1.0f / ws->delta_time_sec;
}

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

APL_DEF void apl_pixel_mat_copy_to_screen(struct Apl_Window_State *ws)
{
    StretchDIBits(ws->platform.device_context,
                  0, 0, (int)ws->window_w              , (int)ws->window_h,
                  0, 0, (int)ws->window_pixels_mat.cols, (int)ws->window_pixels_mat.rows,
                  ws->window_pixels_mat.elements,
                  &ws->platform.bit_map_info,
                  DIB_RGB_COLORS, SRCCOPY);
}

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

    ws->window_pixels_mat = mat2D_realloc_uint32(ws->window_pixels_mat, ws->window_h, ws->window_w);
    if (!ws->window_pixels_mat.elements) {
        apl_dprintERROR("%s", "realloc pixel mat failed");
        return APL_FAIL;
    }
    ws->inv_z_buffer_mat = mat2D_realloc(ws->inv_z_buffer_mat, ws->window_h, ws->window_w);
    if (!ws->inv_z_buffer_mat.elements) {
        apl_dprintERROR("%s", "realloc inverse z buffer mat failed");
        return APL_FAIL;
    }

    return apl_window_render(ws);
}

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
        SetWindowTextA(ws->platform.window_handle, temp_buf);
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
        default:
        {
            result = DefWindowProcA(window, message, wparam, lparam);
        } break;
    }
    return result;
}

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
int main(void) 
{
    SetUnhandledExceptionFilter(apl_unhandled_exception_filter);

    printf("hello from %s\n", apl_platform_name());

    struct Apl_Window_State window_state = {0};
    enum Apl_Return_Types rt;

    /**
     * initializing the window state
     */
    // window_state.platform.window_class;
    // window_state.platform.window_handle = NULL;
    // window_state.platform.window_call_back = NULL;
    // window_state.platform.bit_map_info;
    // window_state.running = 0;
    window_state.to_render = true;
    window_state.to_update = true;
    window_state.to_limit_fps = true;
    window_state.to_clear_renderer = true;
    // window_state.to_flip_y;
    // window_state.delta_time = 0;
    // window_state.elapsed_time_sec = 0;
    // window_state.previous_frame_time = 0;
    // window_state.fps = 0;
    window_state.wanted_fps = APL_WANTED_FPS;
    // window_state.buttons.space_bar_is_pressed = 0;
    // window_state.buttons.w_is_pressed = 0;
    // window_state.buttons.s_is_pressed = 0;
    // window_state.buttons.a_is_pressed = 0;
    // window_state.buttons.d_is_pressed = 0;
    // window_state.buttons.e_is_pressed = 0;
    // window_state.buttons.q_is_pressed = 0;
    // window_state.buttons.left_button_pressed = 0;
    window_state.window_w = APL_INIT_WINDOW_WIDTH;
    window_state.window_h = APL_INIT_WINDOW_HEIGHT;
    // Mat2D_uint32 window_pixels_mat;

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
    for (window_state.running = true; window_state.running ; ) {
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

