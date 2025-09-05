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
Curve points;
Curve points1;
void setup(game_state_t *game_state)
{
    game_state->const_fps = 30;
    // game_state->to_limit_fps = 0;

    figure1 = adl_alloc_figure(100, 100, (Point){100, 100, 0, 0});
    figure2 = adl_alloc_figure(600, 500, (Point){300, 100, 0, 0});

    ada_init_array(Point, points);
    ada_init_array(Point, points1);
    Point temp_point = (Point){1,1,0,0};
    ada_appand(Point, points, temp_point);
    ada_appand(Point, points1, temp_point);
    temp_point = (Point){2,2,0,0};
    ada_appand(Point, points, temp_point);
    ada_appand(Point, points1, temp_point);
    temp_point = (Point){3,1,0,0};
    ada_appand(Point, points, temp_point);
    ada_appand(Point, points1, temp_point);
    temp_point = (Point){4,10,0,0};
    ada_appand(Point, points, temp_point);
    temp_point = (Point){5,-10,0,0};
    ada_appand(Point, points, temp_point);
    temp_point = (Point){3,-20,0,0};
    ada_appand(Point, points, temp_point);

    temp_point = (Point){5,-10,0,0};
    ada_appand(Point, points1, temp_point);

    mat2D_fill_uint32(figure1.pixels_mat, 0xFFFFFF);
    mat2D_fill_uint32(figure2.pixels_mat, 0xFFFFFF);

    adl_draw_axis_on_figure(figure1);
    adl_draw_axis_on_figure(figure2);

    adl_add_curve_to_figure(&figure1, points.elements, points.length, 0xFF0000);
    adl_add_curve_to_figure(&figure2, points.elements, points.length, 0xFF0000);

    adl_add_curve_to_figure(&figure1, points1.elements, points1.length, 0x0000FF);
    adl_add_curve_to_figure(&figure2, points1.elements, points1.length, 0x0000FF);

}

void update(game_state_t *game_state)
{
}

void render(game_state_t *game_state)
{
    adl_plot_curves_on_figure(figure1);
    adl_plot_curves_on_figure(figure2);

    adl_copy_figure_to_screen(game_state->window_pixels_mat, figure1);
    adl_copy_figure_to_screen(game_state->window_pixels_mat, figure2);
}

