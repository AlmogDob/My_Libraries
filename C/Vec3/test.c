#define VEC3_IMPLEMENTATION
#include "Vec3.h"

int main(void)
{
    Vec3 v = Vec3_new(1, 1, 0);
    v = Vec3_rotate_around_z(v, Vec3_new(0,0,0), (3.141592*4)/2);

    VEC3_PRINT(v);
    return 0;
}
