#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <dbghelp.h>

#pragma comment(lib, "Dbghelp.lib")

static const char *get_exception_name(DWORD code)
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

static void print_module_plus_offset(DWORD64 instruction_pointer)
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

static LONG WINAPI unhandled_exception_filter(EXCEPTION_POINTERS *ep)
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
            get_exception_name(code), code);
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
        print_module_plus_offset(instruction_pointer);
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
    return EXCEPTION_EXECUTE_HANDLER; // unreachable
}

int main(void)
{
    SetUnhandledExceptionFilter(unhandled_exception_filter);

    puts("Running. About to crash...");

    // Test crash:
    int *p = 0;
    int x = p[1];

    return 0;
}

/** AI explanation
 * EXCEPTION_ACCESS_VIOLATION (0xC0000005)
 * _____________________________________________
 * The process tried to access memory in an invalid way.
 * 
 * Common causes:
 * - Dereferencing a null / uninitialized / dangling pointer
 * - Reading/writing past the end of an array or buffer
 * - Use-after-free (object freed, then accessed)
 * - Writing to a read-only page (e.g., string literal memory)
 * 
 * Typical symptoms:
 * - Crashes at a pointer dereference instruction
 * - Often the exception record includes whether it was a read/write/execute and the bad address
 * 
 * 
 * EXCEPTION_IN_PAGE_ERROR (0xC0000006)
 * _____________________________________________
 * A memory page fault happened, but Windows could not bring the page into RAM (“page-in” failed).
 * In other words: the address is valid in the virtual address space, but the OS can’t fetch the backing data.
 * 
 * Common causes:
 * - Disk I/O failure while paging (bad disk, transient I/O error)
 * - Accessing memory-mapped files when the underlying file/device/network share is unavailable
 * - Corrupted paging file or storage issues
 * - Rarely, severe memory/storage subsystem problems
 * 
 * Typical symptoms:
 * - Looks similar to an access violation from the app’s perspective, but the root cause is I/O/page-in failure
 * - Often associated with memory-mapped I/O or paging activity
 * 
 * 
 * EXCEPTION_STACK_OVERFLOW (0xC00000FD)
 * _____________________________________________
 * The thread’s stack ran out of space.
 * 
 * Common causes:
 * - Infinite or very deep recursion
 * - Large local variables/arrays allocated on the stack (int big[10000000];)
 * - Excessive stack usage in a call chain (many large stack frames)
 * 
 * Typical symptoms:
 * - Crash occurs after many nested calls
 * - Sometimes the program can’t reliably run much code in the handler because the stack is exhausted (Windows provides a small “guard” region, but it’s limited)
 * 
 * 
 * What EXCEPTION_CONTINUE_SEARCH means here
 * _____________________________________________
 * In an SEH filter, returning EXCEPTION_CONTINUE_SEARCH tells Windows: “this handler doesn’t apply; keep looking for another handler up the chain.” Your code is effectively saying it only wants to intercept these three exceptions and ignore the rest.
*/