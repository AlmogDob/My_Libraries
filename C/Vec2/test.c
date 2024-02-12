#define VEC2_IMPLEMENTATION
#include "Vec2.h"

int main(void)
{
    Vec2 v = vec2_new(0, 1);
    Vec2 center = vec2_new(1, 0);

    Vec2 res;

    vec2_rotate(&res, &v, &center, PI);

    VEC2_PRINT(&res);

    return 0;
}

