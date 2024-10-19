#ifndef RENDER_SHAPES_SDL_H_
#define RENDER_SHAPES_SDL_H_

#include <math.h>
#include <SDL2/SDL.h>
#include "Vec2.h"

void draw_circle(SDL_Renderer *renderer, Vec2 center, float r);
void fill_circle(SDL_Renderer *renderer, Vec2 center, float r);

#ifdef RENDER_SHAPES_SDL_IMPLEMENTATION

void draw_circle(SDL_Renderer *renderer, Vec2 center, float r)
{
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx ++) {
            float diff = dx * dx + dy * dy - r * r;
            if (diff < 0 && diff > -r*2) {
                SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
            }
        }
    }
}

void fill_circle(SDL_Renderer *renderer, Vec2 center, float r)
{
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx ++) {
            float diff = dx * dx + dy * dy - r * r;
            if (diff < 0) {
                SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
            }
        }
    }
}

#endif /*RENDER_SHAPES_SDL_IMPLEMENTATION*/

#endif /*RENDER_SHAPES_SDL_H_*/
