#include <stdio.h>
#include <time.h>
#define NN_IMPLEMENTATION
#include "nn.h"
#include <math.h>

#define dprintF(expr) printf(#expr " = %g\n", expr)

typedef float sample[3];

float cost(float w1, float w2, float b);

/* OR-gate */
sample or_train[] = {
    {0, 0, 0},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 1},
};

sample and_train[] = {
    {0, 0, 0},
    {1, 0, 0},
    {0, 1, 0},
    {1, 1, 1},
};

sample nand_train[] = {
    {0, 0, 1},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 0},
};

sample *train = and_train;
size_t train_count = 4;

float cost(float w1, float w2, float b)
{
    float result = 0.0f;
    for (size_t i = 0; i < train_count; ++i) {
        float x1 = train[i][0];
        float x2 = train[i][1];
        float y = sigmoidf(x1*w1 + x2*w2 + b);
        float d = y - train[i][2];
        result += d*d;
    }
    result /= train_count;
    return result;
}

int main()
{
    srand(time(0));
    float w1 = rand_float();
    float w2 = rand_float();
    float b  = rand_float();
    
    float eps = 1e-1;
    float rate = 1e-1;

    float c, dw1, dw2, db;

    c = cost(w1, w2, b);
    printf("w1 = %f, w2 = %f, b = %f, c = %f\n", w1, w2, b, c);

    for (size_t i = 0; i < 1e6; i++) {
        c = cost(w1, w2, b);
        // printf("w1 = %f, w2 = %f, b = %f, c = %f\n", w1, w2, b, c);
        dw1 = (cost(w1 + eps, w2, b) - c) / eps;
        dw2 = (cost(w1, w2 + eps, b) - c) / eps;
        db  = (cost(w1, w2, b + eps) - c) / eps;
        w1 -= dw1*rate;
        w2 -= dw2*rate;
        b  -= db*rate;
    }
    printf("w1 = %f, w2 = %f, b = %f, c = %f\n", w1, w2, b, c);

    for (size_t i = 0; i < 2; i++) {
        for(size_t j = 0; j < 2; j++) {
            printf("%zu | %zu = %f\n", i, j , sigmoidf(i*w1 + j*w2 + b));
        }
    }

    return 0;
}
