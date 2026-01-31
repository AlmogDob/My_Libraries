/**
 * @file

 * @note This single header library is insapired by
 * Tsoding's JSON parser implementation: https://youtu.be/FBpgdSjJ6nQ 
 */

#ifndef ALMOG_JSON_PARSER_H_
#define ALMOG_JSON_PARSER_H_

#include "Almog_Lexer.h"

#define ajp_dprintERROR(fmt, ...) \
    fprintf(stderr, "\n%s:%d:\n[Error] in function '%s':\n        " \
    fmt "\n\n", __FILE__, __LINE__, __func__, __VA_ARGS__)


#endif /*ALMOG_JSON_PARSER_H_*/

#ifdef ALMOG_JSON_PARSER_IMPLEMENTATION
#undef ALMOG_JSON_PARSER_IMPLEMENTATION




#endif /*ALMOG_JSON_PARSER_IMPLEMENTATION*/

