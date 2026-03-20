#define ALMOG_HUFFMAN_IMPLEMENTATION
#include "./include/Almog_Huffman.h"

static void ah_print_usage(const char *program)
{
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s -e <input_file>\n", program);
    fprintf(stderr, "  %s --encode <input_file>\n", program);
    fprintf(stderr, "  %s -d <input_file.huff>\n", program);
    fprintf(stderr, "  %s --decode <input_file.huff>\n", program);
}

int main(int argc, char const *argv[])
{
    if (argc != 3) {
        ah_print_usage(argv[0]);
        return 1;
    }

    const char *command = argv[1];
    const char *file_path = argv[2];

    if (strcmp(command, "-e") == 0 || strcmp(command, "--encode") == 0) {
        ah_encode_file(file_path);
        printf("Encoded: %s -> %s.huff\n", file_path, file_path);
        return 0;
    }

    if (strcmp(command, "-d") == 0 || strcmp(command, "--decode") == 0) {
        ah_decode_file(file_path);
        printf("Decoded: %s\n", file_path);
        return 0;
    }

    fprintf(stderr, "Unknown command: %s\n", command);
    ah_print_usage(argv[0]);
    return 1;
}
