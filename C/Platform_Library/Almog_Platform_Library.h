/* -------------------------------------------------------------------------------- */
#if defined(_WIN32) || defined(_WIN64)
/* -------------------------------------------------------------------------------- */
#ifndef ALMOG_PLATFORM_LIBRARY_H_
#define ALMOG_PLATFORM_LIBRARY_H_

#define APL_PLATFORM_NAME "Windows"

#include <windows.h>
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Gdi32.lib")

#define apl_dprintINFO(fmt, ...) \
    fprintf(stderr, "[Info] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 7, "", __func__, 7, "", __VA_ARGS__)
#define apl_dprintWARNING(fmt, ...) \
    fprintf(stderr, "[Warning] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 10, "", __func__, 10, "", __VA_ARGS__)
#define apl_dprintERROR(fmt, ...) \
    fprintf(stderr, "[Error] %s:%d:\n%*sIn function '%s':\n%*s" fmt "\n", __FILE__, __LINE__, 8, "", __func__, 8, "", __VA_ARGS__)




char * apl_platform_name(void);

#endif /* ALMOG_PLATFORM_LIBRARY_H_*/

#ifdef ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#undef ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION

char * apl_platform_name(void)
{
    return APL_PLATFORM_NAME;
}


#endif /* ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION*/

/* -------------------------------------------------------------------------------- */
#elif defined(__linux__)
/* -------------------------------------------------------------------------------- */

#ifndef ALMOG_PLATFORM_LIBRARY_H_
#define ALMOG_PLATFORM_LIBRARY_H_

#define APL_PLATFORM_NAME "Linux"

char * apl_platform_name(void);

#endif /* ALMOG_PLATFORM_LIBRARY_H_*/

#ifdef ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#undef ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION

char * apl_platform_name(void)
{
    return APL_PLATFORM_NAME;
}


#endif /* ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION*/

#endif /* platform */