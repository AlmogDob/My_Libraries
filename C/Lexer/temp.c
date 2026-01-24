#define ALMOG_LEXER_IMPLEMENTATION
#include "Almog_Lexer.h"

int main(void)
{
    char str[] = 
        "#include <stdio.h>\n"
        "\n"
        "int main(void) {\n"
        "    char str[] = \"almog\"; \n"
        "    if (x > 0 && | y >= -5)\n"
        " // c++ style comments\n"
        "    /* greatest comment*/\nreturn 0;\n"
        "}\n";

    struct Lexer l = al_lexer_alloc(str, asm_length(str));

    struct Token t = al_lexer_next_token(&l);
    while (t.kind != TOKEN_END) {
        al_token_print(t);
        t = al_lexer_next_token(&l);
    }
    al_token_print(al_lexer_next_token(&l));
    asm_dprintCHAR(l.content[l.cursor]);

    return 0;
}
