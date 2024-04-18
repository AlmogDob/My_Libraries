#include <stdio.h>
#include <time.h>
#define NN_IMPLEMENTATION
#include "NN.h"


#define dprintF(expr) printf(#expr " = %g\n", expr)

float train[][2] = {
    {0,0},
    {1,2},
    {2,4},
    {3,6},
    {4,8},
};

int main(int argc, char const *argv[])
{
    srand(time(0));
    float w = rand_float()*10;

    dprintF(w);

    return 0;
}
