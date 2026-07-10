# Almog Memory Debug

A simple, lightweight memory debugger for C.  
Inspired by [Eskil Steenberg’s memory debugger](https://github.com/valiet/quel_solaar/blob/main/Forge/f_mem_debug.c)  
(see his talk [here](https://youtu.be/sfrnU3-EpPI?t=1552)).

Track every allocation by **file and line**, detect buffer **overruns**, report leaks, and optionally override `malloc` / `realloc` / `free` with a single define.

This is a **single-header** library (STB-style).

---

## Features

- **Allocation tracking** by source file and line number  
- **Buffer overrun detection** via canary / magic values after each allocation  
- **Leak & usage reports** (allocations, frees, total bytes, remaining bytes)  
- **Optional global overrides** for `malloc`, `realloc`, and `free`  
- **Built-in dynamic array helpers** (ADA macros)  
- **Debug print macros** for common types and formatted info/warning/error output  
- Highly configurable via macros (`AMD_MALLOC`, `AMD_FREE`, init capacity, etc.)  
- Zero dependencies beyond the C standard library  

---

## Quick start

### 1. Drop the header in

Copy `Almog_Memory_Debug.h` into your project.

### 2. Create one translation unit with the implementation

```c
#define ALMOG_MEMORY_DEBUG_IMPLEMENTATION
#include "Almog_Memory_Debug.h"
```

### 3. Enable the memory debugger (recommended)

Define `AMD_MEMORY_DEBUG` **before** including the header (in the same TU as the implementation and/or wherever you want the overrides):

```c
#define AMD_MEMORY_DEBUG
#define ALMOG_MEMORY_DEBUG_IMPLEMENTATION
#include "Almog_Memory_Debug.h"
```

When `AMD_MEMORY_DEBUG` is defined, the header does:

```c
#define malloc(n)       amd_debug_mem_malloc(n, __FILE__, __LINE__)
#define realloc(n, m)   amd_debug_mem_realloc(n, m, __FILE__, __LINE__)
#define free(n)         amd_debug_mem_free(n)
```

So existing code that uses `malloc` / `realloc` / `free` is automatically tracked.

### 4. At the end of your program (or when you care)

```c
amd_debug_mem();              /* check for buffer overruns */
amd_debug_mem_print(0);       /* print report (0 = show even 1-alloc sites) */
size_t still_used = amd_debug_mem_consumption();
amd_debug_mem_reset();        /* free internal tracking structures */
```

---

## Minimal example

```c
#define AMD_MEMORY_DEBUG
#define ALMOG_MEMORY_DEBUG_IMPLEMENTATION
#include "Almog_Memory_Debug.h"

#include <stdio.h>

int main(void)
{
    int *arr = malloc(100 * sizeof(int));
    for (int i = 0; i < 100; i++) {
        arr[i] = i;
    }

    /* intentional leak if you comment out free */
    free(arr);

    printf("Current retained bytes: %zu\n", amd_debug_mem_consumption());
    amd_debug_mem_print(0);
    amd_debug_mem();   /* should succeed if no overrun */
    amd_debug_mem_reset();
    return 0;
}
```

---

## How it works (overview)

| Concept | Details |
|--------|---------|
| **Tracking** | Each allocation is recorded under its `(file, line)` site. Multiple allocs from the same site are grouped. |
| **Over-allocation** | Every block is allocated `size + AMD_MEM_OVER_ALLOC` bytes. The extra bytes are filled with `AMD_MEM_MAGIC_NUMBER` and checked on free / `amd_debug_mem()`. |
| **Initialization pattern** | User payload is filled with `AMD_MEM_MAGIC_NUMBER + 1` on malloc (helps catch use of uninitialized / freed-ish patterns in some cases). |
| **Realloc** | Implemented as allocate → copy → free (with correct bookkeeping). |
| **Max sites** | Up to `AMD_MEM_ALLOC_LINES_MAX_NUM` (default 1024) distinct `(file, line)` allocation sites. |

Reporting groups by **where the allocation happened**, not by pointer address, so you get actionable “this line leaks” information.

---

## API

### Core memory debugger

| Function | Description |
|----------|-------------|
| `void *amd_debug_mem_malloc(size_t size, char *file, size_t line)` | Allocate `size` bytes and track the call site. |
| `void *amd_debug_mem_realloc(void *p, size_t size, char *file, size_t line)` | Reallocate; `NULL` acts like malloc. Unknown pointers abort with diagnostics. |
| `void  amd_debug_mem_free(void *buf)` | Free a tracked pointer. Double free / unknown pointer → error + exit. |
| `bool  amd_debug_mem(void)` | Scan live allocations for canary corruption (buffer overrun). Returns success/fail. |
| `bool  amd_debug_mem_remove(void *buf)` | Remove from tracking (and check canaries). Used internally by free. |
| `void  amd_debug_mem_add(void *p, size_t size, char *file, size_t line)` | Manually register an allocation (advanced / low-level). |
| `size_t amd_debug_mem_consumption(void)` | Total bytes **currently** held (not yet freed). |
| `void  amd_debug_mem_print(size_t min_allocs)` | Print a report for sites that did at least `min_allocs` allocations. |
| `void  amd_debug_mem_reset(void)` | Free all internal bookkeeping and reset state. Call when done or between test phases. |

## Configuration

Override any of these **before** including the header:

| Macro | Default | Meaning |
|-------|---------|---------|
| `AMD_MALLOC` / `AMD_REALLOC` / `AMD_FREE` / `AMD_EXIT` | `malloc` / `realloc` / `free` / `exit` | Underlying allocators / exit used by the debugger |
| `AMD_ADA_MALLOC` / `AMD_ADA_REALLOC` / … | Same as above | Allocators used by ADA macros |
| `AMD_ADA_INIT_CAPACITY` | `10` | Initial dynamic array capacity |
| `AMD_ADA_ASSERT` | `assert` | Assertion macro used by ADA |
| `AMD_MEM_ALLOC_LINES_MAX_NUM` | `1024` | Max distinct allocation sites |
| `AMD_MEM_OVER_ALLOC` | `32` | Extra canary bytes after each block |
| `AMD_MEM_MAGIC_NUMBER` | `132` | Fill value for canary region |
| `AMD_MAX_LEN` | `1000` | Max stored path length for `file` |
| `AMD_DEF_STATIC` | (undefined) | If defined, all functions become `static` |
| `AMD_SUCCESS` / `AMD_FAIL` | `1` / `0` | Boolean return conventions |
| `AMD_MEMORY_DEBUG` | (undefined) | If defined: remap `malloc`/`realloc`/`free` |
| `ALMOG_MEMORY_DEBUG_IMPLEMENTATION` | (undefined) | Define in **exactly one** `.c` to emit the implementation |

Example of custom allocators:

```c
#define AMD_MALLOC  my_arena_alloc
#define AMD_FREE    my_arena_free
#define AMD_REALLOC my_arena_realloc
#define ALMOG_MEMORY_DEBUG_IMPLEMENTATION
#include "Almog_Memory_Debug.h"
```

---

## Report example

Calling `amd_debug_mem_print(0)` produces output similar to:

```text
[Info] main.c:42:
       In function 'main':
       Memory report:
       src/foo.c:128
        - Allocations: 42
        - Frees: 40
        - Bytes allocated over all: 8192
        - Bytes remained not freed: 256
```

Use `min_allocs` to hide one-off noise (e.g. `amd_debug_mem_print(10)`).

---

## Integration tips

1. **One implementation only**  
   Put `#define ALMOG_MEMORY_DEBUG_IMPLEMENTATION` in a single `.c` file.

2. **Define `AMD_MEMORY_DEBUG` consistently**  
   If only some TUs redefine `malloc`, you will get mismatched tracking. Prefer defining it in a common debug config header or build flags:

   ```bash
   gcc -DAMD_MEMORY_DEBUG -DALMOG_MEMORY_DEBUG_IMPLEMENTATION ...
   ```

3. **Call `amd_debug_mem_reset()` before exit in long-running tools or tests**  
   It frees the internal arrays that store `(file, line)` bookkeeping.

4. **Debugging overruns**  
   Prefer calling `amd_debug_mem()` periodically (tests, end of frame, etc.). Overruns are also checked when freeing.

5. **Do not mix with other malloc wrappers** without care  
   AddressSanitizer, Valgrind, mimalloc overrides, etc. can coexist in principle, but double-wrapping can be confusing. Prefer one system at a time during development.

6. **Production builds**  
   Leave `AMD_MEMORY_DEBUG` undefined and do not define `ALMOG_MEMORY_DEBUG_IMPLEMENTATION` so the macros and implementation drop out of the binary path you care about.

---

## Limitations

- Not a substitute for AddressSanitizer / Valgrind for advanced heap bugs (use-after-free heuristics, concurrency, etc.).
- Realloc is not in-place; it always allocates a new block, copies, and frees the old one.
- File path string compare is capped; very long identical prefixes could theoretically collide (paths are null-terminated and limited to `AMD_MAX_LEN`).
- Allocation-site table is fixed-size (`AMD_MEM_ALLOC_LINES_MAX_NUM`). Hitting the limit aborts.
- Global state is not thread-safe out of the box.

## Credits

- Concepts and approach inspired by **Eskil Steenberg**  
  - Source: https://github.com/valiet/quel_solaar/blob/main/Forge/f_mem_debug.c  
  - Talk: https://youtu.be/sfrnU3-EpPI?t=1552  