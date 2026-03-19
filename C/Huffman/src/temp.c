#define ALMOG_HUFFMAN_IMPLEMENTATION
#include "./include/Almog_Huffman.h"

int main(void)
{
    const char file_path[] = "../input.txt";
    // const char file_path[] = "../src/temp.c";
    // const char file_path[] = "../src/include/Almog_Huffman.h";

    ah_encode_file(file_path);
    ah_decode_file("../input.txt.huff");

    return 0;
}
