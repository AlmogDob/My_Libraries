#include <stdio.h>
#include <Windows.h>
#include <setjmp.h>

static jmp_buf restart_point;
static volatile LONG g_ctrl = 0;

static BOOL WINAPI console_handler(DWORD type)
{
    switch (type) {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
            InterlockedExchange(&g_ctrl, 1);
            return TRUE; /* we handled it; don't terminate */
        default:
            return FALSE; /* let default handling occur */
    }
}
int main(void)
{
    SetConsoleCtrlHandler(console_handler, TRUE);

    if (setjmp(restart_point) != 0) {
        /* Keep it simple: print a message and continue */
        puts("\nCtrl-C pressed. Kill terminal to kill program.\n");
    }

    puts("Running... press Ctrl-C to test. (Program will not exit.)");

    for (;;) {
        if (InterlockedExchange(&g_ctrl, 0)) {
            longjmp(restart_point, 1);
        }
        Sleep(10);
    }

    return 0;
}
