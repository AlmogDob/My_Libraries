#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#ifndef DISPLAY_MALLOC
#include <stdlib.h>
#define DISPLAY_MALLOC malloc
#endif //DISPLAY_MALLOC

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

#define FPS 30
#define MILLISECS_PER_FRAM 1000/FPS

bool creat_window(void);
void destroy_window(void);

void fix_framerate(void);

void clear_framebuffer(uint32_t color);
void render_framebuffer(void);

void draw_pixel(uint8_t x, uint8_t y, uint32_t color);

#endif //DISPLAY_H_

#ifdef DISPLAY_IMPLEMENTATION

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static uint32_t *framebuffer = NULL;
static SDL_Texture *framebuffer_texture = NULL;
static uint16_t window_width = 800;
static uint16_t window_height = 800;


bool creat_window(void)
{
    int c = SDL_Init(SDL_INIT_EVERYTHING);
    if (c != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    window = SDL_CreateWindow("Triangle rasteriazation",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, 
                               window_width,
                               window_height,
                               0);
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    framebuffer = DISPLAY_MALLOC(sizeof(uint32_t) * SCREEN_WIDTH * SCREEN_HEIGHT);

    framebuffer_texture = SDL_CreateTexture(renderer,
                                            SDL_PIXELFORMAT_RGBA32,
                                            SDL_TEXTUREACCESS_STREAMING,
                                            SCREEN_WIDTH,
                                            SCREEN_HEIGHT);

    return true;
}

void destroy_window(void)
{
    free(framebuffer);
    SDL_DestroyTexture(framebuffer_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void fix_framerate(void)
{
    static int32_t previous_frame_time = 0;
    int time_ellapsed = SDL_GetTicks() - previous_frame_time;
    int time_to_wait = MILLISECS_PER_FRAM - time_ellapsed;
    if (time_to_wait > 0 && time_to_wait < MILLISECS_PER_FRAM) {
        SDL_Delay(time_to_wait);
    }
    previous_frame_time = SDL_GetTicks();
}

void clear_framebuffer(uint32_t color)
{
    for (uint16_t i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) {
        framebuffer[i] = color;
    }
}

void render_framebuffer(void)
{
    SDL_UpdateTexture(framebuffer_texture,
                      NULL,
                      framebuffer,
                      (int)(SCREEN_WIDTH*sizeof(uint32_t)));
    SDL_RenderCopy(renderer, framebuffer_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void draw_pixel(uint8_t x, uint8_t y, uint32_t color)
{
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
        return;
    }
    framebuffer[(SCREEN_WIDTH*y) + x] = color;
}


#endif //DISPLAY_IMPLEMENTATION
