#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"

int main(void)
{
    char str[] = "1012";
    char str1[5] = {'2'};
 
    asm_copy_array_by_indexes(str1, 0, asm_length(str), str);

    asm_dprintSTRING(str);
    asm_dprintSTRING(str1);
    
    asm_get_token_and_cut(str1, str, '0', false);

    asm_dprintSTRING(str);
    asm_dprintSTRING(str1);

    asm_left_pad(str1, 2, '*');
    asm_dprintSTRING(str1);


    return 0;
}
