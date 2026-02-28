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
    #undef APL_DEFINE_ALL_IMPLEMENTATIONS
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
    bool to_flip_y;

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

#define APL_OK APL_SUCCESS
#define APL_UNUSED(x) (void)x

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


enum Apl_Return_Types   apl_initialize_main_window(struct Apl_Window_State *ws, char *name);
LRESULT CALLBACK        apl_main_window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
void                    apl_pixel_mat_copy_to_screen(struct Apl_Window_State *ws, HDC window_paint_device_context);
char *                  apl_platform_name(void);
void                    apl_resize_window_pixel_mat(struct Apl_Window_State *ws, size_t new_w, size_t new_h);
enum Apl_Return_Types   apl_window_destroy(struct Apl_Window_State *ws);
enum Apl_Return_Types   apl_window_process_input(struct Apl_Window_State *ws);
enum Apl_Return_Types   apl_window_render(struct Apl_Window_State *ws);
enum Apl_Return_Types   apl_window_setup(struct Apl_Window_State *ws);
enum Apl_Return_Types   apl_window_update(struct Apl_Window_State *ws);

/* user supposed to define: */
enum Apl_Return_Types   apl_destroy(struct Apl_Window_State *ws);
enum Apl_Return_Types   apl_render(struct Apl_Window_State *ws);
enum Apl_Return_Types   apl_setup(struct Apl_Window_State *ws);
enum Apl_Return_Types   apl_update(struct Apl_Window_State *ws);

#endif /* ALMOG_PLATFORM_LIBRARY_H_*/

#ifdef ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#undef ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION

enum Apl_Return_Types apl_initialize_main_window(struct Apl_Window_State *ws, char *name)
{
    WNDCLASS window_class = {
        .style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
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
                                         name,
                                         WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                         CW_USEDEFAULT,
                                         CW_USEDEFAULT,
                                         APL_INIT_WINDOW_WIDTH,
                                         APL_INIT_WINDOW_HEIGHT,
                                         0,
                                         0,
                                         GetModuleHandleA(NULL),
                                         ws);
    if (!window_handle) {
        apl_dprintERROR("%s", "failed to create window\n");
        return APL_FAIL;
    }
    ws->platform.window_handle = window_handle;
    APL_UNUSED(window_handle);


    return APL_SUCCESS;
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
            result = DefWindowProcA(window, message, wparam, lparam);
        } break;
        case WM_SIZE:
        {
            if (ws == NULL) {
                result = DefWindowProcA(window, message, wparam, lparam);
            } else {
                RECT client_rect = {0};
                GetClientRect(ws->platform.window_handle, &client_rect); /* client rect are the pixel we can draw at */
                apl_resize_window_pixel_mat(ws, client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);
            }
        } break;
        case WM_CLOSE:
        {
            ws->running = false;
        } break;
        case WM_DESTROY:
        {
            ws->running = false;
            PostQuitMessage(0);
        } break;
        case WM_ACTIVATEAPP:
        {
            // apl_dprintINFO("%s", "WM_ACTIVATEAPP");
        } break;
        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC device_context = BeginPaint(window, &paint);
            apl_pixel_mat_copy_to_screen(ws, device_context);
            EndPaint(window, &paint);
            
        } break;
        default:
        {
            result = DefWindowProcA(window, message, wparam, lparam);
        } break;
    }

    return result;
}

void apl_pixel_mat_copy_to_screen(struct Apl_Window_State *ws, HDC window_paint_device_context)
{
    StretchDIBits(window_paint_device_context,
                  0, 0, (int)ws->window_w              , (int)ws->window_h,
                  0, 0, (int)ws->window_pixels_mat.cols, (int)ws->window_pixels_mat.rows,
                  ws->window_pixels_mat.elements,
                  &ws->platform.bit_map_info,
                  DIB_RGB_COLORS, SRCCOPY);
}

char * apl_platform_name(void)
{
    return APL_PLATFORM_NAME;
}

void apl_resize_window_pixel_mat(struct Apl_Window_State *ws, size_t new_w, size_t new_h)
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
    apl_window_render(ws);
}

enum Apl_Return_Types apl_window_destroy(struct Apl_Window_State *ws)
{
    /*------------------------------------------------------------*/

    apl_destroy(ws);

    return APL_OK;
}

enum Apl_Return_Types apl_window_process_input(struct Apl_Window_State *ws)
{
    APL_UNUSED(ws);

    return APL_OK;
}

enum Apl_Return_Types apl_window_render(struct Apl_Window_State *ws)
{
    if (ws->to_clear_renderer) {
        mat2D_fill_uint32(ws->window_pixels_mat, APL_BACKGROUND_COLOR_hexARGB);
    }
    /*------------------------------------------------------------*/

    apl_render(ws);
            
    /*------------------------------------------------------------*/
    HDC device_context = GetDC(ws->platform.window_handle);
    apl_pixel_mat_copy_to_screen(ws, device_context);
    ReleaseDC(ws->platform.window_handle, device_context);

    return APL_OK;
}

enum Apl_Return_Types apl_window_setup(struct Apl_Window_State *ws)
{
    RECT client_rect = {0};
    GetClientRect(ws->platform.window_handle, &client_rect); /* client rect are the pixel we can draw at */
    apl_resize_window_pixel_mat(ws, client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);
    
    /*------------------------------------------------------------*/

    apl_setup(ws);

    return APL_OK;
}

enum Apl_Return_Types apl_window_update(struct Apl_Window_State *ws)
{
    /*------------------------------------------------------------*/

    apl_update(ws);

    return APL_OK;
}

/**
 * ============================================================
 * main
 * ============================================================
 */
int main(void) 
{
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
    // window_state.previous_frame_time = 0;
    window_state.to_limit_fps = true;
    window_state.to_clear_renderer = true;
    // window_state.to_flip_y;
    // window_state.delta_time = 0;
    // window_state.elapsed_time = 0;
    // window_state.fps = 0;
    window_state.const_fps = APL_TARGET_FPS;
    window_state.frame_target_time = APL_FRAME_TARGET_TIME;
    // window_state.space_bar_was_pressed = 0;
    // window_state.w_was_pressed = 0;
    // window_state.s_was_pressed = 0;
    // window_state.a_was_pressed = 0;
    // window_state.d_was_pressed = 0;
    // window_state.e_was_pressed = 0;
    // window_state.q_was_pressed = 0;
    // window_state.left_button_pressed = 0;
    window_state.window_w = APL_INIT_WINDOW_WIDTH;
    window_state.window_h = APL_INIT_WINDOW_HEIGHT;
    // Mat2D_uint32 window_pixels_mat;

    rt = apl_initialize_main_window(&window_state, "apl window");
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

    /**
     * The game loop
     * while (window_state.running) {
     *     process_input_window(&window_state);
     *     if (window_state.to_update) {
     *         update_window(&window_state);
     *     }
     *     if (window_state.to_render) {
     *         render_window(&window_state);
     *     }
     *     
     * }
     * destroy_window(&window_state);
     */

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
    }

    rt = apl_window_destroy(&window_state);
    if (rt == APL_FAIL) {
        apl_dprintERROR("%s", "failed to window destroy");
    }

    return rt;
}
/**
 * ============================================================
 * end of main
 * ============================================================
 */

#ifndef APL_DESTROY
#define APL_DESTROY
enum Apl_Return_Types apl_destroy(struct Apl_Window_State *ws) { APL_UNUSED(ws); return APL_OK; }
#endif

#ifndef APL_SETUP
#define APL_SETUP
enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws) { APL_UNUSED(ws); return APL_OK }
#endif

#ifndef APL_UPDATE
#define APL_UPDATE
enum Apl_Return_Types apl_update(struct Apl_Window_State *ws) { APL_UNUSED(ws); return APL_OK; }
#endif

#ifndef APL_RENDER
#define APL_RENDER
enum Apl_Return_Types apl_render(struct Apl_Window_State *ws) { APL_UNUSED(ws); return APL_OK; }
#endif


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