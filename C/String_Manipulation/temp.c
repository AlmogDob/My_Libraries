#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"

int main(void)
{
    char str1[] = "10110110110110110110.101";
    char *str = str1;
    char *temp;

    asm_dprintINT(asm_str2int(str, &temp, 10));
    asm_dprintSTRING(temp);
    asm_dprintSIZE_T(asm_str2size_t(str, &temp, 10));
    asm_dprintSTRING(temp);
    asm_dprintFLOAT(asm_str2float(str, &temp, 10));
    asm_dprintSTRING(temp);
    asm_dprintDOUBLE(asm_str2double(str, &temp, 10));
    asm_dprintSTRING(temp);

    return 0;
}
