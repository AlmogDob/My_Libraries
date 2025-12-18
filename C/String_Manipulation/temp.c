#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"

int main(void)
{
    char c = '2';

    asm_dprintINT(asm_check_char_belong_to_base(c, 229));

    return 0;
}
