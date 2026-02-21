#include <stdio.h>
#include <stdlib.h>

#define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#include "Almog_Platform_Library.h"


LRESULT CALLBACK main_window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;
    
    switch (message)
    { 
        case WM_SIZE:
        {
            // apl_dprintERROR("%s", "WM_SIZE");
        } break;
        case WM_DESTROY:
        {
            apl_dprintINFO("%s", "WM_DESTROY");
            exit(1);
        } break;
        case WM_ACTIVATEAPP:
        {
            // apl_dprintERROR("%s", "WM_ACTIVATEAPP");
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

int main(void) 
{
    printf("hello from %s\n", apl_platform_name());

    WNDCLASS window_class = {
        .style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = main_window_callback,
        .hInstance = GetModuleHandleA(NULL),
        .hCursor   = LoadCursorA(NULL, IDC_ARROW),
        .lpszClassName = "apl_window_class", 
    };
    if (!RegisterClassA(&window_class)) {
        apl_dprintERROR("%s", "register window class filed\n");
    }

    HWND window_handle = CreateWindowExA(0,
                                        window_class.lpszClassName,
                                        "apl window",
                                        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                        CW_USEDEFAULT,
                                        CW_USEDEFAULT,
                                        CW_USEDEFAULT,
                                        CW_USEDEFAULT,
                                        0,
                                        0,
                                        GetModuleHandleA(NULL),
                                        0);
    if (!window_handle) {
        apl_dprintERROR("%s", "failed to create window\n");
    }

    MSG message;
    for (;;) {
        BOOL message_result = GetMessageA(&message, 0, 0, 0);
        if (message_result <= 0) {
            break;
        }
        TranslateMessageA(&message);
        DispatchMessageA(&message);
    }

    return 0;
}


