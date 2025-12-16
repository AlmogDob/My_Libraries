#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"

#include <stdlib.h>

int main(void)
{
    char str[] = "123456789123456789";

    asm_dprintINT(asm_str2int(str, 10));
    asm_dprintSIZE_T(asm_str2size_t(str, 10));

    return 0;
}
