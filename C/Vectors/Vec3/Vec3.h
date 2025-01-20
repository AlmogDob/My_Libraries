#ifndef VEC3_H_
#define VEC3_H_

#ifndef MATH
#include <math.h>
#endif //MATH

#ifndef PI
#define PI M_PI
#endif //PI

#ifndef STDIO
#include <stdio.h>
#endif //STDIO

// #ifndef VEC3_MALLOC
// #include <stdlib.h>
// #define VEC3_MALLOC malloc
// #endif //VEC3_MALLOC

// #ifndef VEC3_ASSERT
// #include <assert.h>
// #define VEC3_ASSERT assert
// #endif //VEC3_ASSERT

#define VEC3_PRINT(v) vec3_print(&v, #v)

typedef struct {
    float x;
    float y;
    float z;
} Vec3;

Vec3 vec3_new(float x, float y, float z);
void vec3_add(Vec3 *target, Vec3 *a, Vec3 *b);
void vec3_sub(Vec3 *target, Vec3 *a,Vec3 *b);
void vec3_mul(Vec3 *target, Vec3 *v, float factor);
void vec3_div(Vec3 *target, Vec3 *v, float factor);
float vec3_length(Vec3 *v);
float vec3_dot(Vec3 *a, Vec3 *b);
void vec3_cross(Vec3 *target, Vec3 *a, Vec3 *b);
void vec3_normalize(Vec3 *v);
void vec3_rotate_around_x(Vec3 *target, Vec3 *v, Vec3 *center, float angle);
void vec3_rotate_around_y(Vec3 *target, Vec3 *v, Vec3 *center, float angle);
void vec3_rotate_around_z(Vec3 *target, Vec3 *v, Vec3 *center, float angle);
void vec3_print(Vec3 *v, char *name);

#endif //VEC3_H_

/* ------------------------------------------------ */

#ifdef VEC3_IMPLEMENTATION
#undef VEC3_IMPLEMENTATION

Vec3 vec3_new(float x, float y, float z)
{
    Vec3 result = {x, y, z};
    return result;
}

void vec3_add(Vec3 *target, Vec3 *a, Vec3 *b)
{
    target->x = a->x + b->x;
    target->y = a->y + b->y;
    target->z = a->z + b->z;

    // Vec3 result = {
    //     .x = a->x + b->x,
    //     .y = a->y + b->y,
    //     .z = a->z + b->z,
    // };
    // return result;
}

void vec3_sub(Vec3 *target, Vec3 *a,Vec3 *b)
{
    target->x = a->x - b->x;
    target->y = a->y - b->y;
    target->z = a->z - b->z;

    // Vec3 result = {
    //     .x = a->x - b->x,
    //     .y = a->y - b->y,
    //     .z = a->z - b->z,
    // };
    // return result;
}

void vec3_mul(Vec3 *target, Vec3 *v, float factor)
{
    target->x = v->x * factor;
    target->y = v->y * factor;
    target->z = v->z * factor;

    // Vec3 result = {
    //     .x = v->x * factor,
    //     .y = v->y * factor,
    //     .z = v->z * factor,
    // };
    // return result;
}

void vec3_div(Vec3 *target, Vec3 *v, float factor)
{
    target->x = v->x / factor;
    target->y = v->y / factor;
    target->z = v->z / factor;

    // Vec3 result = {
    //     .x = v->x / factor,
    //     .y = v->y / factor,
    //     .z = v->z / factor,
    // };
    // return result;
}

float vec3_length(Vec3 *v)
{
    return sqrtf((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

float vec3_dot(Vec3 *a, Vec3 *b)
{
    return ((a->x * b->x) + (a->y * b->y) + (a->z * b->z));
}

void vec3_cross(Vec3 *target, Vec3 *a, Vec3 *b)
{
    target->x = (a->y*b->z) - (a->z*b->y);
    target->y = (a->z*b->x) - (a->x*b->z);
    target->z = (a->x*b->y) - (a->y*b->x);

    // Vec3 result = {
    //     .x = (a->y*b->z) - (a->z*b->y),
    //     .y = (a->z*b->x) - (a->x*b->z),
    //     .z = (a->x*b->y) - (a->y*b->x),
    // };
    // return result;
}

void vec3_normalize(Vec3 *v)
{
    float length = vec3_length(v);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

void vec3_rotate_around_x(Vec3 *target, Vec3 *v, Vec3 *center, float angle)
{
    v->x -= center->x;
    v->y -= center->y;
    v->z -= center->z;
    target->x = v->x;
    target->y = v->y * cosf(angle) - v->z * sinf(angle);
    target->z = v->y * sinf(angle) + v->z * cosf(angle);
    target->x += center->x;
    target->y += center->y;
    target->z += center->z;

//     Vec3 rot;
//     v.x -= center.x;
//     v.y -= center.y;
//     v.z -= center.z;
//     rot.x = v.x;
//     rot.y = v.y * cosf(angle) - v.z * sinf(angle);
//     rot.z = v.y * sinf(angle) + v.z * cosf(angle);
//     rot.x += center.x;
//     rot.y += center.y;
//     rot.z += center.z;
//     return rot;
}

void vec3_rotate_around_y(Vec3 *target, Vec3 *v, Vec3 *center, float angle)
{
    v->x -= center->x;
    v->y -= center->y;
    v->z -= center->z;
    target->x = v->x * cosf(angle) + v->z * sinf(angle);
    target->y = v->y;
    target->z = -v->x * sinf(angle) + v->z * cosf(angle);
    target->x += center->x;
    target->y += center->y;
    target->z += center->z;
    
    // Vec3 rot;
    // v.x -= center.x;
    // v.y -= center.y;
    // v.z -= center.z;
    // rot.x = v.x * cosf(angle) + v.z * sinf(angle);
    // rot.y = v.y;
    // rot.z = -v.x * sinf(angle) + v.z * cosf(angle);
    // rot.x += center.x;
    // rot.y += center.y;
    // rot.z += center.z;
    // return rot;
}

void vec3_rotate_around_z(Vec3 *target, Vec3 *v, Vec3 *center, float angle)
{
    v->x -= center->x;
    v->y -= center->y;
    v->z -= center->z;
    target->x = v->x * cosf(angle) - v->y * sinf(angle);
    target->y = v->x * sinf(angle) + v->y * cosf(angle);
    target->z = v->z;
    target->x += center->x;
    target->y += center->y;
    target->z += center->z;

    // Vec3 rot;
    // v.x -= center.x;
    // v.y -= center.y;
    // v.z -= center.z;
    // rot.x = v.x * cosf(angle) - v.y * sinf(angle);
    // rot.y = v.x * sinf(angle) + v.y * cosf(angle);
    // rot.z = v.z;
    // rot.x += center.x;
    // rot.y += center.y;
    // rot.z += center.z;
    // return rot;
}

void vec3_print(Vec3 *v, char *name)
{
    printf("%s: (%g, %g, %g)\n", name, v->x, v->y, v->z);
}


#endif //VEC3_IMPLEMENTATION
