#define VEC3_IMPLEMENTATION
#include "Vec3.h"

int main(void)
{
    Vec3 v = vec3_new(1, 0, 0);
    Vec3 center = vec3_new(0,0,0);
    Vec3 u;
    vec3_rotate_around_y(&u, &v, &center, PI/2);

    Vec3 res;

    vec3_sub(&res, &v, &u);

    VEC3_PRINT(&res);
    return 0;
}
