/* This one-file library is heavily inspired by one of the additioal files of 
Gustavo Pezzi to the triangle-rasterizer-int 'Vec2.c'. I add a few functions
you can find the source code in:
https://github.com/gustavopezzi/triangle-rasterizer-int .
featured in this video of his:
https://youtu.be/k5wtuKWmV48 .*/


#ifndef VEC2_H_
#define VEC2_H_

#include <math.h>

#define VEC2_PRINT(v) vec2_print(v, #v)

typedef struct {
    float x;
    float y;
} vec2;

vec2 vec2_new(float x, float y);
vec2 vec2_add(vec2 *a,vec2 *b);
vec2 vec2_sub(vec2 *a,vec2 *b);
vec2 vec2_mul(vec2 *v, float factor);
vec2 vec2_div(vec2 *v, float factor);
float vec2_length(vec2 *v);
float vec2_dot(vec2 *a, vec2 *b);
float vec2_cross(vec2 *a, vec2 *b);
void vec2_normalize(vec2 *v);
vec2 vec2_rotate(vec2 v, vec2 center, float angle);
void vec2_print(vec2 v, char *name);

#endif //VEC2_H_

#ifdef VEC2_IMPLEMENTATION

vec2 vec2_new(float x, float y)
{
    vec2 result = {x, y};
    return result;
}

vec2 vec2_add(vec2 *a,vec2 *b)
{
    vec2 result = {
        .x = a->x + b->x,
        .y = a->y + b->y
    };
    return result;
}

vec2 vec2_sub(vec2 *a,vec2 *b)
{
    vec2 result = {
        .x = a->x - b->x,
        .y = a->y - b->y
    };
    return result;
}

vec2 vec2_mul(vec2 *v, float factor)
{
    vec2 result = {
        .x = v->x * factor,
        .y = v->y * factor
    };
    return result;
}

vec2 vec2_div(vec2 *v, float factor)
{
    vec2 result = {
        .x = v->x / factor,
        .y = v->y / factor
    };
    return result;
}

float vec2_length(vec2 *v)
{
    return sqrt((v->x * v->x) + (v->y * v->y));
}

float vec2_dot(vec2 *a, vec2 *b)
{
    return (a->x * b->x + a->y * b->y);
}

float vec2_cross(vec2 *a, vec2 *b)
{
    return (a->x * b->y - a->y * b->x);
}

void vec2_normalize(vec2 *v)
{
    float length = vec2_length(v);
    v->x /= length;
    v->y /= length;
}

vec2 vec2_rotate(vec2 v, vec2 center, float angle)
{
    vec2 rot;
    v.x -= center.x;
    v.y -= center.y;
    rot.x = v.x * cos(angle) - v.y * sin(angle);
    rot.y = v.x * sin(angle) + v.y * cos(angle);
    rot.x += center.x;
    rot.y += center.y;
    return rot;
}

void vec2_print(vec2 v, char *name)
{
    printf("%s: (%g, %g)\n", name, v.x, v.y);
}


#endif //VEC2_IMPLEMENTATION
