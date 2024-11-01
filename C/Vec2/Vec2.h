/* This one-file library is heavily inspired by one of the additioal files of 
Gustavo Pezzi to the triangle-rasterizer-int 'Vec2.c'. I add a few functions
you can find the source code in:
https://github.com/gustavopezzi/triangle-rasterizer-int .
featured in this video of his:
https://youtu.be/k5wtuKWmV48 .*/


#ifndef VEC2_H_
#define VEC2_H_

#ifndef MATH
#include <math.h>
#endif //MATH

#ifndef PI
#define PI M_PI
#endif //PI

#ifndef STDIO
#include <stdio.h>
#endif //STDIO

// #ifndef VEC2_MALLOC
// #include <stdlib.h>
// #define VEC2_MALLOC malloc
// #endif //VEC2_MALLOC

// #ifndef VEC2_ASSERT
// #include <assert.h>
// #define VEC2_ASSERT assert
// #endif //VEC2_ASSERT

#define VEC2_PRINT(v) vec2_print(&v, #v)

typedef struct {
    float x;
    float y;
} Vec2;

Vec2 vec2_new(float x, float y);
void vec2_add(Vec2 *target, Vec2 *a,Vec2 *b);
void vec2_sub(Vec2 *target, Vec2 *a,Vec2 *b);
void vec2_mul(Vec2 *target, Vec2 *v, float factor);
void vec2_div(Vec2 *target, Vec2 *v, float factor);
float vec2_length(Vec2 *v);
float vec2_dot(Vec2 *a, Vec2 *b);
float vec2_cross(Vec2 *a, Vec2 *b);
void vec2_normalize(Vec2 *v);
void vec2_rotate(Vec2 *target, Vec2 *v, Vec2 *center, float angle);
void vec2_print(Vec2 *v, char *name);

#endif //VEC2_H_

#ifdef VEC2_IMPLEMENTATION
#undef VEC2_IMPLEMENTATION

Vec2 vec2_new(float x, float y)
{
    Vec2 result = {x, y};
    return result;
}

void vec2_add(Vec2 *target, Vec2 *a,Vec2 *b)
{
    target->x = a->x + b->x;
    target->y = a->y + b->y;

    // Vec2 result = {
    //     .x = a->x + b->x,
    //     .y = a->y + b->y
    // };
    // return result;
}

void vec2_sub(Vec2 *target, Vec2 *a,Vec2 *b)
{
    target->x = a->x - b->x;
    target->y = a->y - b->y;
    
    // Vec2 result = {
    //     .x = a->x - b->x,
    //     .y = a->y - b->y
    // };
    // return result;
}

void vec2_mul(Vec2 *target, Vec2 *v, float factor)
{
    target->x = v->x * factor;
    target->y = v->y * factor;
    
    // Vec2 result = {
    //     .x = v->x * factor,
    //     .y = v->y * factor
    // };
    // return result;
}

void vec2_div(Vec2 *target, Vec2 *v, float factor)
{
    target->x = v->x / factor;
    target->y = v->y / factor;

    // Vec2 result = {
    //     .x = v->x / factor,
    //     .y = v->y / factor
    // };
    // return result;
}

float vec2_length(Vec2 *v)
{
    return sqrtf((v->x * v->x) + (v->y * v->y));
}

float vec2_dot(Vec2 *a, Vec2 *b)
{
    return (a->x * b->x + a->y * b->y);
}

float vec2_cross(Vec2 *a, Vec2 *b)
{
    return (a->x * b->y - a->y * b->x);
}

void vec2_normalize(Vec2 *v)
{
    float length = vec2_length(v);
    v->x /= length;
    v->y /= length;
}

void vec2_rotate(Vec2 *target, Vec2 *v, Vec2 *center, float angle)
{
    v->x -= center->x;
    v->y -= center->y;
    target->x = v->x * cos(angle) - v->y * sin(angle);
    target->y = v->x * sin(angle) + v->y * cos(angle);
    target->x += center->x;
    target->y += center->y;
    
    // Vec2 rot;
    // v.x -= center.x;
    // v.y -= center.y;
    // rot.x = v.x * cos(angle) - v.y * sin(angle);
    // rot.y = v.x * sin(angle) + v.y * cos(angle);
    // rot.x += center.x;
    // rot.y += center.y;
    // return rot;
}

void vec2_print(Vec2 *v, char *name)
{
    printf("%s: (%g, %g)\n", name, v->x, v->y);
}


#endif //VEC2_IMPLEMENTATION
