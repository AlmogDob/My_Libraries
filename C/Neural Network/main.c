#include <stdio.h>
#include <time.h>
#define NN_IMPLEMENTATION
#include "NN.h"


#define dprintF(expr) printf(#expr " = %g\n", expr)
#define train_count (sizeof(train)/sizeof(train[0]))

float train[][2] = {
    {0,0},
    {1,2},
    {2,4},
    {3,6},
    {4,8},
};

float cost(float w, float b) {
    float result = 0.0f;
    for (size_t i = 0; i < train_count; ++i) {
        float x = train[i][0];
        float y = x*w + b;
        float d = y - train[i][1];
        result += d*d;
    }
    result /= train_count;
    return result;
}

int main()
{
    srand(time(0));
    float w = rand_float()*10;
    float b = rand_float()*5;
    
    float eps = 1e-3;
    float rate = 1e-3;
    dprintF(cost(w, b));

    float dw, db, c;
    
    for (int i = 0; i < 500; i++) {
        c = cost(w, b);
        dw = (cost(w+eps, b) - c)/eps;
        db = (cost(w, b+eps) - c)/eps;
        w -= rate*dw;
        b -= rate*db;
        printf("cost = %f, w = %f, b = %f\n", cost(w, b), w, b);

    }

    printf("------------\n");
    dprintF(cost(w, b));
    printf("w = %f, b = %f\n", w, b);


    return 0;
}
