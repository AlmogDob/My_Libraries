#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MATRIX2D_IMPLEMENTATION
#define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#include "Almog_Platform_Library.h"


LRESULT CALLBACK main_window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    struct Apl_Window_State *ws = (struct Apl_Window_State *)GetWindowLongPtrA(window, GWLP_USERDATA);
    LRESULT result = 0;
    
    switch (message)
    { 
        case WM_NCCREATE: {
            CREATESTRUCTA *cs = (CREATESTRUCTA *)lparam;
            SetWindowLongPtrA(window, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
            result = TRUE;
        } break;
        case WM_SIZE:
        {
            // apl_dprintINFO("%s", "WM_SIZE");
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
            int x = paint.rcPaint.left;
            int y = paint.rcPaint.top;
            int width = paint.rcPaint.right - paint.rcPaint.left;
            int height = paint.rcPaint.bottom - paint.rcPaint.top;
            PatBlt(device_context, x, y, width, height, BLACKNESS);
            EndPaint(window, &paint);
        } break;
        default:
        {
            result = DefWindowProcA(window, message, wparam, lparam);
        } break;
    }

    return result;
}

enum Apl_Return_Types apl_initialize_main_window(struct Apl_Window_State *ws)
{
    WNDCLASS window_class = {
        .style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = main_window_callback,
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
    ws->platform.window_call_back = main_window_callback;

    HWND window_handle = CreateWindowExA(0,
                                        ws->platform.window_class.lpszClassName,
                                        "apl window",
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

int main(void) 
{
    printf("hello from %s\n", apl_platform_name());

    struct Apl_Window_State window_state = {0};

    window_state.running = 0;
    window_state.delta_time = 0;
    window_state.elapsed_time = 0;
    window_state.previous_frame_time = 0;
    window_state.const_fps = APL_TARGET_FPS;
    window_state.frame_target_time = APL_FRAME_TARGET_TIME;
    window_state.fps = 0;
    window_state.to_limit_fps = 1;

    window_state.to_render = 1;
    window_state.to_update = 1;
    window_state.to_clear_renderer = 1;

    window_state.space_bar_was_pressed = 0;
    window_state.w_was_pressed = 0;
    window_state.s_was_pressed = 0;
    window_state.a_was_pressed = 0;
    window_state.d_was_pressed = 0;
    window_state.e_was_pressed = 0;
    window_state.q_was_pressed = 0;
    window_state.left_button_pressed = 0;

    window_state.window_w = APL_INIT_WINDOW_WIDTH;
    window_state.window_h = APL_INIT_WINDOW_HEIGHT;
    // window_state.platform.window_class;
    window_state.platform.window_handle = NULL;
    window_state.platform.window_call_back = NULL;

    window_state.running = apl_initialize_main_window(&window_state);



    window_state.window_pixels_mat = mat2D_alloc_uint32(window_state.window_h, window_state.window_w);
    mat2D_fill_uint32(window_state.window_pixels_mat, 0xFFFFFFFF);
    // MAT2D_PRINT_UINT32(window_state.window_pixels_mat);


    MSG message;
    for (window_state.running = true; window_state.running ; ) {
        BOOL message_result = GetMessageA(&message, 0, 0, 0);
        if (message_result <= 0) {
            break;
        }
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return 0;
}


