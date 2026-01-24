#define ALMOG_LEXER_IMPLEMENTATION
#include "Almog_Lexer.h"

int main(void)
{
    char str[] = "#define Hello World!\n";

    struct Lexer l = al_lexer_alloc(str, asm_length(str));
    al_lexer_next_token(&l);

    return 0;
}
