#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include "Matrix2D.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH (16 * 50)
#endif

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT (9 * 50)
#endif

#ifndef FPS
#define FPS 100
#endif

#ifndef FRAME_TARGET_TIME
#define FRAME_TARGET_TIME (1000 / FPS)
#endif

#ifndef TH_COUNT
#define TH_COUNT 4
#endif

#define dprintSTRING(expr) printf(#expr " = %s\n", expr)
#define dprintCHAR(expr) printf(#expr " = %c\n", expr)
#define dprintINT(expr) printf(#expr " = %d\n", expr)
#define dprintD(expr) printf(#expr " = %g\n", expr)
#define dprintSIZE_T(expr) printf(#expr " = %zu\n", expr)

#define HexARGB_RGBA(x) (x>>(8*2)&0xFF), (x>>(8*1)&0xFF), (x>>(8*0)&0xFF), (x>>(8*3)&0xFF)
#define ARGB_hexARGB(a, r, g, b) 0x01000000*(a) + 0x00010000*(r) + 0x00000100*(g) + 0x00000001*(b)
#define RGB_hexRGB(r, g, b) (int)(0x010000*(r) + 0x000100*(g) + 0x000001*(b))

#define PI M_PI

typedef struct {
    int game_is_running;
    float delta_time;
    float elapsed_time;
    float const_fps;
    float fps;
    float frame_target_time;
    int space_bar_was_pressed;
    int to_render;
    int to_update;
    size_t previous_frame_time;
    int left_button_pressed;
    int to_limit_fps;
    int to_clear_renderer;

    SDL_Window *window;
    int window_w;
    int window_h;
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;

    SDL_Surface *window_surface;
    SDL_Texture *window_texture;

    SDL_Rect fps_place;
    SDL_Color white_color;
    SDL_Color fps_color;

    Mat2D window_pixels_mat;
} game_state_t;

typedef struct {
    int id;
    int th_count;
    Mat2D mat;
    uint32_t *pixels;
} thread_arg_t;

int initialize_window(game_state_t *game_state);
void setup_window(game_state_t *game_state);
void process_input_window(game_state_t *game_state);
void update_window(game_state_t *game_state);
void render_window(game_state_t *game_state);
void destroy_window(game_state_t *game_state);
void fix_framerate(game_state_t *game_state);
void setup(game_state_t *game_state);
void update(game_state_t *game_state);
void render(game_state_t *game_state);

void check_window_mat_size(game_state_t *game_state);
void *routine(void *arg);
void copy_mat_to_surface_RGB(game_state_t *game_state);

int main()
{
    game_state_t game_state;

    game_state.game_is_running = 0;
    game_state.delta_time = 0;
    game_state.elapsed_time = 0;
    game_state.const_fps = FPS;
    game_state.fps = 0;
    game_state.frame_target_time = FRAME_TARGET_TIME;
    game_state.space_bar_was_pressed = 0;
    game_state.to_render = 1;
    game_state.to_update = 1;
    game_state.previous_frame_time = 0;
    game_state.left_button_pressed = 0;
    game_state.to_limit_fps = 1;
    game_state.to_clear_renderer = 1;
    game_state.window = NULL;
    game_state.window_w = WINDOW_WIDTH;
    game_state.window_h = WINDOW_HEIGHT;
    game_state.renderer = NULL;
    game_state.font = NULL;
    game_state.text_surface = NULL;
    game_state.text_texture = NULL;

    game_state.game_is_running = !initialize_window(&game_state);

    setup_window(&game_state);

    while (game_state.game_is_running) {
        process_input_window(&game_state);
        if (game_state.to_update) {
            update_window(&game_state);
        }
        if (game_state.to_render) {
            render_window(&game_state);
        }
        
    }
    destroy_window(&game_state);

    return 0;
}

int initialize_window(game_state_t *game_state)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "%s:%d: [Error] initializing SDL.\n", __FILE__, __LINE__);
        return -1;
    }

    game_state->window = SDL_CreateWindow(NULL,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              game_state->window_w,
                              game_state->window_h,
                              SDL_WINDOW_RESIZABLE
                              );
    if (!game_state->window) {
        fprintf(stderr, "%s:%d: [Error] creating SDL window.\n", __FILE__, __LINE__);
        return -1;
    }

    game_state->renderer = SDL_CreateRenderer(game_state->window, -1, 0);
    if (!game_state->renderer) {
        fprintf(stderr, "%s:%d: [Error] creating SDL renderer.\n", __FILE__, __LINE__);
        return -1;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "%s:%d: [Error] initializing SDL_ttf.\n", __FILE__, __LINE__);
        return -1;
    }

    game_state->font = TTF_OpenFont("./font/Gabriely Black.ttf",32);
    if (!game_state->font) {
        fprintf(stderr, "%s:%d: [Error] loading font.\n", __FILE__, __LINE__);
        return -1;
    }

    (void)game_state;
    
    return 0;
}

void setup_window(game_state_t *game_state)
{
    game_state->white_color.a = 255;
    game_state->white_color.b = 255;
    game_state->white_color.g = 255;
    game_state->white_color.r = 255;

    game_state->fps_color = game_state->white_color;

    game_state->fps_place.x = 10;
    game_state->fps_place.y = 10;
    game_state->fps_place.w = 135;
    game_state->fps_place.h = 25;

    /*-----------------------------------*/

    setup(game_state);

}

void process_input_window(game_state_t *game_state)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game_state->game_is_running = 0;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    game_state->game_is_running = 0;
                }
                if (event.key.keysym.sym == SDLK_q) {
                    game_state->game_is_running = 0;
                }
                if (event.key.keysym.sym == SDLK_SPACE) {
                    if (!game_state->space_bar_was_pressed) {
                        game_state->to_render = 0;
                        game_state->to_update = 0;
                        game_state->space_bar_was_pressed = 1;
                        break;
                    }
                    if (game_state->space_bar_was_pressed) {
                        game_state->to_render = 1;
                        game_state->to_update = 1;
                        game_state->previous_frame_time = SDL_GetTicks();
                        game_state->space_bar_was_pressed = 0;
                        break;
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    game_state->left_button_pressed = 1;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    game_state->left_button_pressed = 0;
                }
                break;
        }
    }
}

void update_window(game_state_t *game_state)
{
    SDL_GetWindowSize(game_state->window, &(game_state->window_w), &(game_state->window_h));

    fix_framerate(game_state);
    game_state->elapsed_time += game_state->delta_time;
    game_state->fps = 1.0f / game_state->delta_time;
    game_state->frame_target_time = 1000/game_state->const_fps;

    char fps_count[100];
    if (!game_state->to_limit_fps) {
        sprintf(fps_count, "dt = %5.02f [ms]", game_state->delta_time*1000);
    } else {
        sprintf(fps_count, "FPS = %5.2f", game_state->fps);
    }

    if (game_state->elapsed_time*10-(int)(game_state->elapsed_time*10) < 0.1) {
        SDL_SetWindowTitle(game_state->window, fps_count);
    }

    /*----------------------------------------------------------------------------*/

    update(game_state);

}

void render_window(game_state_t *game_state)
{
    if (game_state->to_clear_renderer) {
        SDL_SetRenderDrawColor(game_state->renderer, HexARGB_RGBA(0xFF181818));
        SDL_RenderClear(game_state->renderer);
    }
    /*------------------------------------------------------------------------*/

    render(game_state);

    /*------------------------------------------------------------------------*/
    // if (game_state->to_clear_renderer) {
    //     SDL_RenderCopy(renderer, text_texture, NULL, &(game_state->fps_place));
    //     SDL_RenderPresent(renderer);
    // }
}

void destroy_window(game_state_t *game_state)
{
    mat2D_free(game_state->window_pixels_mat);

    if (!game_state->window_surface) SDL_FreeSurface(game_state->window_surface);
    if (!game_state->window_texture) SDL_DestroyTexture(game_state->window_texture);

    if (!game_state->text_texture) SDL_DestroyTexture(game_state->text_texture);
    if (!game_state->text_surface) SDL_FreeSurface(game_state->text_surface);
    
    SDL_DestroyRenderer(game_state->renderer);
    SDL_DestroyWindow(game_state->window);

    SDL_Quit();

    (void)game_state;
}

void fix_framerate(game_state_t *game_state)
{
    int time_ellapsed = SDL_GetTicks() - game_state->previous_frame_time;
    int time_to_wait = game_state->frame_target_time - time_ellapsed;
    if (time_to_wait > 0 && time_to_wait < game_state->frame_target_time) {
        if (game_state->to_limit_fps) {
            SDL_Delay(time_to_wait);
        }
    }
    game_state->delta_time = (SDL_GetTicks() - game_state->previous_frame_time) / 1000.0f;
    game_state->previous_frame_time = SDL_GetTicks();
}

#ifndef SETUP
#define SETUP
void setup(game_state_t *game_state) { (void)game_state; }
#endif

#ifndef UPDATE
#define UPDATE
void update(game_state_t *game_state) { (void)game_state; }
#endif

#ifndef RENDER
#define RENDER
void render(game_state_t *game_state) { (void)game_state; }
#endif

void check_window_mat_size(game_state_t *game_state)
{
    if (game_state->window_h != (int)game_state->window_pixels_mat.rows || game_state->window_w != (int)game_state->window_pixels_mat.cols) {
        mat2D_free(game_state->window_pixels_mat);
        SDL_FreeSurface(game_state->window_surface);
        game_state->window_pixels_mat = mat2D_alloc(game_state->window_h, game_state->window_w);
        // printf("hello\nmat rows: %5zu, mat cols: %5zu\n", game_state->window_pixels_mat.rows, game_state->window_pixels_mat.cols);
        game_state->window_surface = SDL_GetWindowSurface(game_state->window);
    }
}

void *routine(void *arg)
{
    thread_arg_t th_arg = *(thread_arg_t *)arg;

    for (size_t r = 0; r < th_arg.mat.rows; r++) {
        for (size_t c = th_arg.id*th_arg.mat.cols/th_arg.th_count; c < th_arg.mat.cols/th_arg.th_count * (th_arg.id+1); c++) {
            th_arg.pixels[r*th_arg.mat.cols + c] = MAT2D_AT(th_arg.mat, r, c);
        }
    }
    // printf("from: %d, to: %d\n", th_arg.id*(int)th_arg.mat.cols/TH_COUNT, (int)th_arg.mat.cols/TH_COUNT * (th_arg.id+1));

    return 0; 
}

void copy_mat_to_surface_RGB(game_state_t *game_state)
{
    pthread_t th[TH_COUNT];
    thread_arg_t th_arg_array[TH_COUNT];
    int pitch;
    uint32_t *pixels;
    int th_count = TH_COUNT;
    if (!((game_state->window_pixels_mat.cols % TH_COUNT == 0) && (game_state->window_pixels_mat.rows % TH_COUNT == 0))) {
        th_count = 1;
    }

    SDL_LockSurface(game_state->window_surface);
    check_window_mat_size(game_state);

    pitch = game_state->window_surface->pitch;
    pixels = game_state->window_surface->pixels;

    pitch = pitch/4;
    assert((int)game_state->window_pixels_mat.cols <= pitch);

    for (int i = 0; i < th_count; i++) {
        th_arg_array[i].id = i;
        th_arg_array[i].th_count = th_count;
        th_arg_array[i].mat = game_state->window_pixels_mat;
        th_arg_array[i].pixels = pixels;

        if (pthread_create(th+i, NULL, &routine, (void *)&th_arg_array[i]) != 0) {
            fprintf(stderr, "%s:%d: [Error] failed to create thread: %s", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
    }
    for (int i = 0; i < th_count; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            fprintf(stderr, "%s:%d: [Error] failed to join thread: %s", __FILE__, __LINE__, strerror(errno));
            exit(2);
        }
    }
    SDL_UnlockSurface(game_state->window_surface);
}

