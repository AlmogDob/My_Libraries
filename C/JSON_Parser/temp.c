#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#define ALMOG_LEXER_IMPLEMENTATION
#define ALMOG_JSON_PARSER_IMPLEMENTATION
#include "Almog_JSON_Parser.h"

int main(void)
{
    FILE *fp = fopen("./temp.c", "r");

    struct Tokens tokens = al_lex_entire_file(fp);
    
    for (size_t i = 0; i < tokens.length; i++) {
        al_token_print(tokens.elements[i]);
    }
    asm_dprintSIZE_T(tokens.length);

    al_tokens_free(tokens);


    return 0;
}
