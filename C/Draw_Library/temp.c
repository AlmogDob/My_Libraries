#define SETUP
#define UPDATE
#define RENDER
#include "./display.c"
#define MATRIX2D_IMPLEMENTATION
#include "./Matrix2D.h"
#define ALMOG_DRAW_LIBRARY_IMPLEMENTATION
#include "./Almog_Draw_Library.h"

void adl_draw_character(Mat2D_uint32 screen_mat, char c, int width_pixel, int hight_pixel, int x_top_left, int y_top_left, uint32_t color)
{
    switch (c)
    {
    case 'a':
    case 'A':
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel/2, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/6, y_top_left+2*hight_pixel/3, x_top_left+5*width_pixel/6, y_top_left+2*hight_pixel/3, color);
        break;
    case 'b':
    case 'B':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/3, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left, y_top_left+hight_pixel/2, color);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left, y_top_left+hight_pixel, color);
        break;
    case 'c':
    case 'C':
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color);
        break;
    case 'd':
    case 'D':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left, y_top_left, color);
        break;
    case 'e':
    case 'E':
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color);

        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/2, color);
        break;
    case 'f':
    case 'F':
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color);

        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/2, color);
        break;
    case 'g':
    case 'G':
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/2, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/2, x_top_left+width_pixel/2, y_top_left+hight_pixel/2, color);
        break;
    case 'h':
    case 'H':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/2, color);
        break;
    case 'i':
    case 'I':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color);
        break;
    case 'j':
    case 'J':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+2*width_pixel/3, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/2, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left+hight_pixel, x_top_left+width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel/6, y_top_left+5*hight_pixel/6, color);
        break;
    case 'k':
    case 'K':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left, color);
        break;
    case 'l':
    case 'L':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color);
        break;
    case 'm':
    case 'M':
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color);
        break;
    case 'n':
    case 'N':
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color);
        break;
    case 'o':
    case 'O':
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color);
        break;
    case 'p':
    case 'P':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/3, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left, y_top_left+hight_pixel/2, color);
        break;
    case 'q':
    case 'Q':
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel, color);
        break;
    case 'r':
    case 'R':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/3, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left, y_top_left+hight_pixel/2, color);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel, color);
        break;
    case 's':
    case 'S':
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color);

        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+hight_pixel/3, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/3, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color);

        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color);
        break;
    case 't':
    case 'T':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color);
        break;
    case 'u':
    case 'U':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left, color);
        break;
    case 'v':
    case 'V':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color);
        break;
    case 'w':
    case 'W':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel/2, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color);
        break;
    case 'x':
    case 'X':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left, color);
        break;
    case 'y':
    case 'Y':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel/2, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel/2, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left+hight_pixel/2, x_top_left+width_pixel/2, y_top_left+hight_pixel, color);
        break;
    case 'z':
    case 'Z':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left, y_top_left+hight_pixel, color);
        break;
    case '.':
        adl_fill_rectangle_min_max(screen_mat, x_top_left+width_pixel/6, x_top_left+width_pixel/3, y_top_left+5*hight_pixel/6, y_top_left+hight_pixel, color);
        break;
    case '0':
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color);

        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color);
        break;
    case '1':
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left+width_pixel/2, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/2, y_top_left, x_top_left+width_pixel/2, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color);
        break;
    case '2':
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left+width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/3, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+hight_pixel, color);
        break;
    case '3':
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left+width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left+2*width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/3, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, color);

        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left, y_top_left+5*hight_pixel/6, color);
        break;
    case '4':
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left, y_top_left+2*hight_pixel/3, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+2*hight_pixel/3, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color);
        break;
    case '5':
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left, y_top_left+hight_pixel/2, color);

        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/2, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color);

        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color);
        break;
    case '6':
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color);

        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, x_top_left, y_top_left+2*hight_pixel/3, color);
        break;
    case '7':
        adl_draw_line(screen_mat, x_top_left, y_top_left, x_top_left+width_pixel, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left, x_top_left+width_pixel/3, y_top_left+hight_pixel, color);
        break;
    case '8':
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/3, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/3, x_top_left+width_pixel, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color);

        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+hight_pixel/3, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/3, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color);

        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, x_top_left, y_top_left+2*hight_pixel/3, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+2*hight_pixel/3, x_top_left, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+2*hight_pixel/3, color);
        break;
    case '9':
        adl_draw_line(screen_mat, x_top_left, y_top_left+5*hight_pixel/6, x_top_left+width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+5*hight_pixel/6, x_top_left+width_pixel, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel, y_top_left+hight_pixel/6, x_top_left+2*width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left, x_top_left+width_pixel/3, y_top_left, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left, x_top_left, y_top_left+hight_pixel/6, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/6, x_top_left, y_top_left+hight_pixel/3, color);
        adl_draw_line(screen_mat, x_top_left, y_top_left+hight_pixel/3, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, color);
        adl_draw_line(screen_mat, x_top_left+width_pixel/3, y_top_left+hight_pixel/2, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, color);
        adl_draw_line(screen_mat, x_top_left+2*width_pixel/3, y_top_left+hight_pixel/2, x_top_left+width_pixel, y_top_left+hight_pixel/3, color);
        break;
    }
}

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


    adl_draw_character(game_state->window_pixels_mat, 'A', 50, 100, 700 , 200, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'B', 50, 100, 755 , 200, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'C', 50, 100, 810 , 200, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'D', 50, 100, 865 , 200, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'E', 50, 100, 920 , 200, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'F', 50, 100, 975 , 200, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'G', 50, 100, 1030, 200, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'H', 50, 100, 1085, 200, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'I', 50, 100, 1140, 200, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'J', 50, 100, 1195, 200, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'K', 50, 100, 700 , 305, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'L', 50, 100, 755 , 305, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'M', 50, 100, 810 , 305, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'N', 50, 100, 865 , 305, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'O', 50, 100, 920 , 305, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'P', 50, 100, 975 , 305, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'Q', 50, 100, 1030, 305, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'R', 50, 100, 1085, 305, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'S', 50, 100, 1140, 305, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'T', 50, 100, 1195, 305, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'U', 50, 100, 700 , 410, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'V', 50, 100, 755 , 410, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'W', 50, 100, 810 , 410, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'X', 50, 100, 865 , 410, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'Y', 50, 100, 920 , 410, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, 'Z', 50, 100, 975 , 410, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, '.', 50, 100, 1030, 410, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, '0', 50, 100, 700 , 515, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, '1', 50, 100, 755 , 515, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, '2', 50, 100, 810 , 515, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, '3', 50, 100, 865 , 515, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, '4', 50, 100, 920 , 515, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, '5', 50, 100, 975 , 515, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, '6', 50, 100, 1030, 515, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, '7', 50, 100, 1085, 515, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, '8', 50, 100, 1140, 515, 0xFFFFFF);
    adl_draw_character(game_state->window_pixels_mat, '9', 50, 100, 1195, 515, 0xFFFFFF);

}

