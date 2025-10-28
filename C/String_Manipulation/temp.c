#include <stdio.h>
#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"

int main(void)
{
    char str[] = "almog dobrescu";
    char word[256];

    asm_get_word_and_cut(word, str, ' ');

    printf("str:  %s\n", str);

    printf("word: %s\n", word);


    return 0;
}
