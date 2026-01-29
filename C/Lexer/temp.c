#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#define ALMOG_LEXER_IMPLEMENTATION
#include "Almog_Lexer.h"
#include "Almog_Dynamic_Array.h"

struct String {
  size_t length;
  size_t capacity;
  char* elements;
};

int main(void)
{
    FILE *fp = fopen("./temp.c", "r");

    #

    struct String string = {0};
    ada_init_array(char, string);
    char temp_str[ASM_MAX_LEN];
    int len = 0;
    while ((len = asm_get_line(fp, temp_str)) != EOF) {
        for (int i = 0; i < len; i++) {
            ada_appand(char, string, temp_str[i]);
        }
        ada_appand(char, string, '\n');
    }

    struct Lexer l = al_lexer_alloc(string.elements, string.length);

    struct Token t = al_lexer_next_token(&l);
    int num_of_tokens = 0;
    while (t.kind != TOKEN_EOF) {
        al_token_print(t);
        t = al_lexer_next_token(&l);
        num_of_tokens++;
    }
    al_token_print(al_lexer_next_token(&l));

    asm_dprintINT(num_of_tokens);

    free(string.elements);

    return 0;
}
