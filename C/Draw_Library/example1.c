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

    figure1 = adl_alloc_figure(100, 70, (Point){100, 100, 0, 0});
    figure2 = adl_alloc_figure(600, 500, (Point){190, 100, 0, 0});

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

    temp_point = (Point){3.5,-10,0,0};
    ada_appand(Point, points1, temp_point);

    figure1.background_color = 0xFFFFFF;
    figure1.to_draw_axis = true;
    figure1.to_draw_max_min_values = true;

    figure2.background_color = 0xFFFFFF;
    figure2.to_draw_axis = true;
    figure2.to_draw_max_min_values = true;

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


    adl_draw_character(game_state->window_pixels_mat, 'A', 50, 100, 700 , 200, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'B', 50, 100, 755 , 200, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'C', 50, 100, 810 , 200, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'D', 50, 100, 865 , 200, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'E', 50, 100, 920 , 200, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'F', 50, 100, 975 , 200, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'G', 50, 100, 1030, 200, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'H', 50, 100, 1085, 200, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'I', 50, 100, 1140, 200, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'J', 50, 100, 1195, 200, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'K', 50, 100, 700 , 305, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'L', 50, 100, 755 , 305, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'M', 50, 100, 810 , 305, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'N', 50, 100, 865 , 305, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'O', 50, 100, 920 , 305, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'P', 50, 100, 975 , 305, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'Q', 50, 100, 1030, 305, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'R', 50, 100, 1085, 305, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'S', 50, 100, 1140, 305, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'T', 50, 100, 1195, 305, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'U', 50, 100, 700 , 410, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'V', 50, 100, 755 , 410, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'W', 50, 100, 810 , 410, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'X', 50, 100, 865 , 410, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'Y', 50, 100, 920 , 410, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, 'Z', 50, 100, 975 , 410, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, '.', 50, 100, 1030, 410, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, '0', 50, 100, 700 , 515, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, '1', 50, 100, 755 , 515, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, '2', 50, 100, 810 , 515, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, '3', 50, 100, 865 , 515, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, '4', 50, 100, 920 , 515, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, '5', 50, 100, 975 , 515, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, '6', 50, 100, 1030, 515, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, '7', 50, 100, 1085, 515, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, '8', 50, 100, 1140, 515, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);
    adl_draw_character(game_state->window_pixels_mat, '9', 50, 100, 1195, 515, 0xFFFFFF, ADL_DEFAULT_OFFSET_ZOOM);

}

