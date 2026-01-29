#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"

int main(void)
{
    char str[] = "-1.1e-1";

    asm_dprintFLOAT(asm_str2float(str, NULL, 10));
    asm_dprintDOUBLE(asm_str2double(str, NULL, 10));
 


    return 0;
}
