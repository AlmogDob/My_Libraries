# Almog Huffman

A small single-header Huffman encoder/decoder library written in C.

This project provides a simple implementation of Huffman compression for files.
It can:

- encode a file into a `.huff` file
- decode a `.huff` file back into its original contents
- build a Huffman tree from byte frequencies
- serialize the tree into the encoded output

## Features

- Single-header library style
- Works with binary and text files
- Built-in bit writer and bit reader
- File-based encode/decode API
- Optional CLI frontend
- Simple dynamic-array-based internal structures

## Repository Overview

This repository contains:

- `Almog_Huffman.h` — the Huffman implementation
- `Almog_Dynamic_Array.h` — dynamic array utilities used internally
- one or more `main.c` examples:
  - a CLI version
  - a hardcoded test version

## File Format

Encoded `.huff` files are written in this order:

1. original byte count (`uint32_t`)
2. number of tree nodes (`uint32_t`)
3. serialized Huffman tree nodes
4. compressed bitstream

During decoding, the program reads the header and tree first, then uses the
bitstream to reconstruct the original bytes.

## Build

This project uses the single-header pattern.

Define `ALMOG_HUFFMAN_IMPLEMENTATION` in exactly one `.c` file before including
the header:

```c
#define ALMOG_HUFFMAN_IMPLEMENTATION
#include "./include/Almog_Huffman.h"
```

Then compile normally.

Example:

```powershell
.\make.bat .\src\huff.c -i -e "..\input.txt"
```

## Usage

### Library API

The main public functions are:

```c
void ah_encode_file(const char *file_path);
void ah_decode_file(const char *file_path);
```

Example:

```c
#define ALMOG_HUFFMAN_IMPLEMENTATION
#include "./include/Almog_Huffman.h"

int main(void)
{
    ah_encode_file("input.txt");
    ah_decode_file("input.txt.huff");
    return 0;
}
```

### CLI Example

If using the command-line `huff.c`, the program supports:

```text
Usage:
  program -e <input_file>
  program --encode <input_file>
  program -d <input_file.huff>
  program --decode <input_file.huff>
```

Examples:

```bash
./huff.exe -e input.txt
./huff.exe -d input.txt.huff
```

### Hardcoded Example

If using the simpler test `test.c`, you can hardcode a path:

```c
#define ALMOG_HUFFMAN_IMPLEMENTATION
#include "./include/Almog_Huffman.h"

int main(void)
{
    const char file_path[] = "../input.txt";

    ah_encode_file(file_path);
    ah_decode_file("../input.txt.huff");

    return 0;
}
```

## Output Files

### Encoding

Encoding:

```text
input.txt -> input.txt.huff
```

### Decoding

Decoding produces:

```text
input.txt.huff -> input.txt.decoded
```

If the input does not end with `.huff`, decoding writes:

```text
<file>.decoded
```

## How It Works

### Encoding Flow

1. Read the input file byte by byte
2. Count frequency of each distinct byte
3. Build a Huffman tree from the frequency table
4. Generate binary codes for each byte
5. Write metadata and the serialized tree to the output file
6. Write the compressed bitstream

### Decoding Flow

1. Open encoded file
2. Read original byte count
3. Read serialized Huffman tree
4. Reconstruct the tree in memory
5. Read bits from the compressed stream
6. Walk the tree until reaching leaves
7. Write decoded bytes to output

## Notes

### Empty Input Files

If the input file is empty, the current implementation prints a warning and
exits without producing output.

### Debug Output

The encoder currently prints the generated code table during encoding.

If you want quieter output, you can remove or comment out the line:

```c
ah_code_table_print(code_table);
```

### Tree Serialization Portability

The current implementation writes raw `struct Ah_Tree_Node` data using
`fwrite`. This is simple, but it means the encoded format may depend on:

- compiler
- platform
- struct layout
- endianness
- integer sizes

So `.huff` files may not be portable across different systems or builds unless
the serialization format is redesigned.

## Limitations

- Not designed as a portable archival format
- Some internal lookups use linear search
- Priority queue behavior is implemented with dynamic-array insertion, not a
  heap
- Error handling uses `exit(1)` in many places instead of returning error codes
- No automated tests included by default

## Example Project Structure

```text
src/
├── include/
│   ├── Almog_Huffman.h
│   └── Almog_Dynamic_Array.h
└── main.c
```
