#include <stdio.h>
#include <time.h>
#define NN_IMPLEMENTATION
#include "NN.h"


#define dprintF(expr) printf(#expr " = %g\n", expr)
#define train_count (sizeof(train)/sizeof(train[0]))

/* OR-gate */
float train[][3] = {
    {0, 0, 0},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 1},
};

float cost(float w1, float w2) {
    float result = 0.0f;
    for (size_t i = 0; i < train_count; ++i) {
        float x1 = train[i][0];
        float x2 = train[i][1];
        float y = x1*w1 + x2*w2;
        float d = y - train[i][2];
        result += d*d;
    }
    result /= train_count;
    return result;
}

int main()
{
    srand(69);
    float w1 = rand_float();
    float w2 = rand_float();
    
    float eps = 1e-3;
    float rate = 1e-2;

    float c, dw1, dw2;

    c = cost(w1, w2);
    printf("w1 = %f, w2 = %f, c = %f\n", w1, w2, c);

    for (int i = 0; i < 2000; i++) {
        c = cost(w1, w2);
        printf("w1 = %f, w2 = %f, c = %f\n", w1, w2, c);
        dw1 = (cost(w1 + eps, w2) - c) / eps;
        dw2 = (cost(w1, w2 + eps) - c) / eps;
        w1 -= dw1*rate;
        w2 -= dw2*rate;
    }
    printf("w1 = %f, w2 = %f, c = %f\n", w1, w2, c);

    return 0;
}
