#define VEC3_IMPLEMENTATION
#include "Vec3.h"

int main(void)
{
    Vec3 v1 = vec3_new(1, 1, 0);
    Vec3 v2 = vec3_new(2, 2, 0);
    
    float res = vec3_dot(&v1, &v2);

    printf("res: %g\n", res);
    return 0;
}
