# Almog PNG

A small C project centered around a single-header PNG decoder, along with
lightweight platform and drawing utilities used to display decoded images.

This repository includes:

- `Almog_PNG.h` — single-header PNG decoder
- `Almog_Platform_Library.h` — small platform/window layer
- `Almog_Draw_Library.h` — drawing helpers
- a demo app that loads PNG test images and displays them in a window
- `make.bat` — Windows build/run helper for MSVC

Note: despite the internal `APNG_*` prefix, the current library decodes regular
PNG images, not animated PNGs.

## Features

### PNG decoder

The decoder:

- reads a PNG file into memory
- validates the PNG signature
- validates chunk CRCs
- concatenates `IDAT` chunks
- parses the zlib stream
- inflates DEFLATE blocks
- reverses PNG scanline filters
- writes decoded output into a 32-bit pixel buffer

### Supported PNG features

Supported color types:

- `0` — grayscale
- `2` — RGB
- `4` — grayscale + alpha
- `6` — RGBA

Supported bit depths:

- `1`
- `2`
- `4`
- `8`

Other supported constraints:

- zlib/DEFLATE compression
- filter method `0`
- interlace method `0` only
- multiple `IDAT` chunks

### Currently unsupported

- APNG / animated PNG
- palette PNGs, color type `3`
- 16-bit channels
- interlaced PNGs (Adam7)
- broader support for all PNG critical/ancillary chunk behaviors

## Design

This project uses a single-header style:

- declarations live in the header
- implementation is enabled in exactly one translation unit with a macro

Example:

```c
#define ALMOG_PNG_IMPLEMENTATION
#include "include/Almog_PNG.h"
```

The same pattern is used by the platform and drawing libraries.

## Quick example

```c
#include <stdbool.h>

#define ALMOG_PNG_IMPLEMENTATION
#include "include/Almog_PNG.h"

int main(void)
{
    struct Apng_PNG_Image image = {0};

    if (apng_png_load("image.png", &image, true) != APNG_SUCCESS) {
        return 1;
    }

    /* image.pixels.elements contains 32-bit pixels in 0xAARRGGBB format */
    /* image.pixels.rows / cols contain image dimensions */

    apng_png_free(&image);
    return 0;
}
```

## Pixel format

Decoded pixels are stored in:

```c
struct Apng_Pixel_Buffer {
    size_t rows;
    size_t cols;
    size_t stride_r;
    uint32_t *elements;
};
```

Packed pixel format:

```text
0xAARRGGBB
```

Useful helper macros:

- `APNG_RGBA_TO_hexARGB(r, g, b, a)`
- `APNG_HexARGB_TO_RGBA_VAR(x, r, g, b, a)`

## Demo application

The demo uses the platform and drawing libraries to open a window and display
decoded PNG images from PngSuite.

### Demo controls

- Left mouse button — zoom in
- Right mouse button — zoom out
- `R` — reset zoom
- Up arrow — next image
- Down arrow — previous image

### Tested images

The demo is set up to load PNG files such as:

```text
../src/test_images/PngSuite/Basic-formats/basn0g01.png
../src/test_images/PngSuite/Basic-formats/basn0g02.png
../src/test_images/PngSuite/Basic-formats/basn0g04.png
../src/test_images/PngSuite/Basic-formats/basn0g08.png
../src/test_images/PngSuite/Basic-formats/basn2c08.png
../src/test_images/PngSuite/Basic-formats/basn4a08.png
../src/test_images/PngSuite/Basic-formats/basn6a08.png
../src/test_images/PngSuite/Image-filtering/f00n0g08.png
...
```

## Building

### Windows support

The current platform layer is implemented for Windows and uses:

- `user32`
- `gdi32`
- `winmm`
- `dbghelp`

The platform header already includes:

```c
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Gdi32.lib")
```

So MSVC links the required libraries automatically.

### Linux support

Linux support in `Almog_Platform_Library.h` is currently only a stub and is not
yet a complete windowing backend.

## Using `make.bat`

This repository includes a Windows batch build script for MSVC:

```text
make.bat
```

It:

- finds a Visual Studio installation using `vswhere`
- initializes the MSVC environment with `VsDevCmd.bat`
- builds one or more `.c` files
- optionally runs them
- optionally cleans generated artifacts
- supports debug and release modes

### Requirements

- Windows
- Visual Studio with C/C++ tools installed
- `vswhere.exe` available at:

```text
%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe
```

### Basic usage

```bash
make.bat path\to\demo.c
```

Build only:

```bash
make.bat path\to\demo.c --build-only
```

Release build:

```bash
make.bat path\to\demo.c --release
```

Keep build artifacts after running:

```bash
make.bat path\to\demo.c --no-clean
```

Clean only:

```bash
make.bat path\to\demo.c --clean-only
```

Build multiple C files:

```bash
make.bat file1.c file2.c
```

Show help:

```bash
make.bat --help
```

### Command-line options

From `make.bat`:

```text
Usage: make.bat <file1.c> [file2.c ...] [options]

Options:
  --help, -h           Show this help message
  --input, -i "args"   Pass a single argument string to the program
  --no-clean, -nc      Do not remove build artifacts after running
  --build-only, -b     Build only, do not run
  --clean-only, -c     Remove build artifacts only
  --release, -r        Build in release mode
```

### Examples

Build and run the demo:

```bash
make.bat src\demo.c
```

Build in release mode and keep artifacts:

```bash
make.bat src\demo.c --release --no-clean
```

Build only:

```bash
make.bat src\demo.c -b
```

Clean build outputs for a file:

```bash
make.bat src\demo.c -c
```

Pass an input string to the built executable:

```bash
make.bat src\demo.c --input "hello"
```

### Build output

The script writes artifacts into:

```text
build\
```

Typical outputs include:

- `.exe`
- `.obj`
- `.pdb`

In some configurations it may also copy:

- `clang_rt.asan_dynamic-x86_64.dll`

### Default build modes

Debug mode defaults to flags similar to:

- `/Zi`
- `/Od`
- `/MDd`
- `/DDEBUG`
- `/RTC1`

Release mode defaults to flags similar to:

- `/O2`
- `/GL`
- `/Gy`
- `/MT`
- `/DNDEBUG`
- `/Zi`
- linker `/LTCG /OPT:REF /OPT:ICF /DEBUG`

Warnings are configured roughly like:

- `/W4`
- `/wd4996`

C standard:

- `/std:c11`

## Customization

### PNG library hooks

You can override the PNG library allocation and assertion macros:

- `APNG_MALLOC`
- `APNG_FREE`
- `APNG_ASSERT`

Example:

```c
#define APNG_MALLOC my_alloc
#define APNG_FREE my_free
#define APNG_ASSERT my_assert
#include "include/Almog_PNG.h"
```

### Internal dynamic array hooks

The internal dynamic array utility supports:

- `ADA_MALLOC`
- `ADA_REALLOC`
- `ADA_ASSERT`
- `ADA_INIT_CAPACITY`

## API overview

Main PNG API:

```c
enum Apng_Return_Types apng_png_load(
    char *file_name,
    struct Apng_PNG_Image *image,
    bool print_info
);

enum Apng_Return_Types apng_png_decode(
    struct Apng_Byte_String file,
    struct Apng_PNG_Image *image,
    bool print_info
);

void apng_png_free(struct Apng_PNG_Image *image);
```

Useful structures:

- `Apng_PNG_Image`
- `Apng_Pixel_Buffer`
- `Apng_Byte_String`

## Limitations and notes

- The decoder is intentionally small and educational in style.
- It validates PNG chunk CRCs and zlib Adler-32.
- Unsupported critical PNG features are rejected rather than partially ignored.
- The implementation is inspired in part by Handmade Hero–style parsing and
  custom infrastructure.

## References

This project references:

- Handmade Hero by Casey Muratori
- PNG specification:
  https://www.w3.org/TR/2003/REC-PNG-20031110/
- zlib specification:
  https://www.ietf.org/rfc/rfc1950.txt
- DEFLATE specification:
  https://www.ietf.org/rfc/rfc1951.txt

## Roadmap ideas

Possible future improvements:

- palette PNG support
- 16-bit PNG support
- Adam7 interlacing
- complete Linux window backend
- faster Huffman decoding tables
- better error reporting
- actual APNG support if desired

## License

This project is licensed under the MIT License. See the `LICENSE` file for
details.