#ifndef ALMOG_HUFFMAN_H_
#define ALMOG_HUFFMAN_H_

#include <stdio.h>
#include <stdint.h>
#include "Almog_Dynamic_Array.h"

#define AH_DEF static inline
#define AH_UNUSED(x) ((void)x)
#define ah_dprintSTRING(expr) printf("[Info] %s:%d:\n" #expr " = %s\n", __FILE__, __LINE__, expr)
#define ah_dprintCHAR(expr) printf("[Info] %s:%d:\n" #expr " = %c\n", __FILE__, __LINE__, expr)
#define ah_dprintINT(expr) printf("[Info] %s:%d:\n" #expr " = %d\n", __FILE__, __LINE__, expr)
#define ah_dprintFLOAT(expr) printf("[Info] %s:%d:\n" #expr " = %#g\n", __FILE__, __LINE__, expr)
#define ah_dprintDOUBLE(expr) printf("[Info] %s:%d:\n" #expr " = %#g\n", __FILE__, __LINE__, expr)
#define ah_dprintSIZE_T(expr) printf("[Info] %s:%d:\n" #expr " = %zu\n", __FILE__, __LINE__, expr)
#define ah_dprintINFO(fmt, ...) \
    fprintf(stderr, "[Info] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 7, "", __func__, 7, "", __VA_ARGS__)
#define ah_dprintWARNING(fmt, ...) \
    fprintf(stderr, "[Warning] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 10, "", __func__, 10, "", __VA_ARGS__)
#define ah_dprintERROR(fmt, ...) \
    fprintf(stderr, "[Error] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 8, "", __func__, 8, "", __VA_ARGS__)

#endif /*ALMOG_HUFFMAN_H_*/

#ifdef ALMOG_HUFFMAN_IMPLEMENTATION
#undef ALMOG_HUFFMAN_IMPLEMENTATION



#endif /*ALMOG_HUFFMAN_IMPLEMENTATION*/
