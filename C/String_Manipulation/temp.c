#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"

int main(void)
{
    char str1[] = "1012";
    char str[ASM_MAX_LEN];

    asm_dprintSTRING(str);
    asm_dprintSTRING(str1);
    asm_strncat(str, str1, ASM_MAX_LEN);
    asm_dprintSTRING(str);



    return 0;
}
