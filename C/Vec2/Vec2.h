/* This one-file library is heavily inspired by one of the additioal files of 
Gustavo Pezzi to the triangle-rasterizer-int 'Vec2.c'. I add a few functions
you can find the source code in:
https://github.com/gustavopezzi/triangle-rasterizer-int .
featured in this video of his:
https://youtu.be/k5wtuKWmV48 .*/


#ifndef VEC2_H_
#define VEC2_H_

#include <math.h>
#include <stdio.h>

#define VEC2_PRINT(v) Vec2_print(v, #v)

typedef struct {
    float x;
    float y;
} Vec2;

Vec2 Vec2_new(float x, float y);
Vec2 Vec2_add(Vec2 *a,Vec2 *b);
Vec2 Vec2_sub(Vec2 *a,Vec2 *b);
Vec2 Vec2_mul(Vec2 *v, float factor);
Vec2 Vec2_div(Vec2 *v, float factor);
float Vec2_length(Vec2 *v);
float Vec2_dot(Vec2 *a, Vec2 *b);
float Vec2_cross(Vec2 *a, Vec2 *b);
void Vec2_normalize(Vec2 *v);
Vec2 Vec2_rotate(Vec2 v, Vec2 center, float angle);
void Vec2_print(Vec2 v, char *name);

#endif //VEC2_H_

#ifdef VEC2_IMPLEMENTATION

Vec2 Vec2_new(float x, float y)
{
    Vec2 result = {x, y};
    return result;
}

Vec2 Vec2_add(Vec2 *a,Vec2 *b)
{
    Vec2 result = {
        .x = a->x + b->x,
        .y = a->y + b->y
    };
    return result;
}

Vec2 Vec2_sub(Vec2 *a,Vec2 *b)
{
    Vec2 result = {
        .x = a->x - b->x,
        .y = a->y - b->y
    };
    return result;
}

Vec2 Vec2_mul(Vec2 *v, float factor)
{
    Vec2 result = {
        .x = v->x * factor,
        .y = v->y * factor
    };
    return result;
}

Vec2 Vec2_div(Vec2 *v, float factor)
{
    Vec2 result = {
        .x = v->x / factor,
        .y = v->y / factor
    };
    return result;
}

float Vec2_length(Vec2 *v)
{
    return sqrtf((v->x * v->x) + (v->y * v->y));
}

float Vec2_dot(Vec2 *a, Vec2 *b)
{
    return (a->x * b->x + a->y * b->y);
}

float Vec2_cross(Vec2 *a, Vec2 *b)
{
    return (a->x * b->y - a->y * b->x);
}

void Vec2_normalize(Vec2 *v)
{
    float length = Vec2_length(v);
    v->x /= length;
    v->y /= length;
}

Vec2 Vec2_rotate(Vec2 v, Vec2 center, float angle)
{
    Vec2 rot;
    v.x -= center.x;
    v.y -= center.y;
    rot.x = v.x * cos(angle) - v.y * sin(angle);
    rot.y = v.x * sin(angle) + v.y * cos(angle);
    rot.x += center.x;
    rot.y += center.y;
    return rot;
}

void Vec2_print(Vec2 v, char *name)
{
    printf("%s: (%g, %g)\n", name, v.x, v.y);
}


#endif //VEC2_IMPLEMENTATION
