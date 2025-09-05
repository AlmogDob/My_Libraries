#define SETUP
#define UPDATE
#define RENDER
#include "./display.c"
#define MATRIX2D_IMPLEMENTATION
#include "./Matrix2D.h"
#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "./Almog_Draw_Library.h"


Figure figure1;
Figure figure2;
Point points[6];
void setup(game_state_t *game_state)
{
    game_state->const_fps = 30;
    // game_state->to_limit_fps = 0;

    figure1 = adl_alloc_figure(100, 100, (Point){100, 100, 0, 0});
    figure2 = adl_alloc_figure(600, 500, (Point){300, 100, 0, 0});

    points[0] = (Point){1,1,0,0};
    points[1] = (Point){2,2,0,0};
    points[2] = (Point){3,1,0,0};
    points[3] = (Point){4,10,0,0};
    points[4] = (Point){5,-10,0,0};
    points[5] = (Point){3,-20,0,0};

    mat2D_fill_uint32(figure1.pixels_mat, 0xFFFFFF);
    mat2D_fill_uint32(figure2.pixels_mat, 0xFFFFFF);

    adl_draw_axis_on_figure(figure1);
    adl_draw_axis_on_figure(figure2);

    adl_plot_points(&figure1, points, 6, 0xFF0000);
    adl_plot_points(&figure2, points, 3, 0xFF0000);
    adl_plot_points(&figure2, points, 6, 0x00FF00);

}

void update(game_state_t *game_state)
{
}

void render(game_state_t *game_state)
{
    adl_copy_figure_to_screen(game_state->window_pixels_mat, figure1);
    adl_copy_figure_to_screen(game_state->window_pixels_mat, figure2);
}

