#define VEC3_IMPLEMENTATION
#include "Vec3.h"

int main(void)
{
    Vec3 r = vec3_new(-3.5, 7.6, 0);
    vec3_mul(&r, &r, 1000);
    Vec3 v = vec3_new(-1.8, -2.04, 6.2);
    Vec3 h, temp1, e;
    float mu = 3.986e5;

    vec3_cross(&h, &r, &v);
    vec3_cross(&temp1, &v, &h);
    vec3_div(&temp1, &temp1, mu);
    
    VEC3_PRINT(&h);
    VEC3_PRINT(&temp1);
    vec3_normalize(&r);
    VEC3_PRINT(&r);
    vec3_sub(&e, &temp1, &r);
    VEC3_PRINT(&e);
    float len = vec3_length(&e);
    printf("e = %g\n", len);

    return 0;
}
