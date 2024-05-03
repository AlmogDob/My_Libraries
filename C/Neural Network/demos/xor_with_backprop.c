#define NN_IMPLEMENTATION
#include "nn.h"
#include "time.h"

#define dprintF(expr) printf(#expr " = %f\n", expr)
#define dprintT(expr) printf(#expr " = %zu\n", expr)

float td_xor[] = {
    0, 0, 0,
    0, 1, 1,
    1, 0, 1,
    1, 1, 0,
};

float td_or[] = {
    0, 0, 0,
    0, 1, 1,
    1, 0, 1,
    1, 1, 1,
};

int main(void)
{
    srand(time(0));
    
    float *td = td_xor;
    size_t stride = 3;
    size_t n = 4;
    Mat ti = {
        .rows = n,
        .cols = 2,
        .stride = stride,
        .elements = td
    };
    Mat to = {
        .rows = n, 
        .cols = 1,
        .stride = stride,
        .elements = td + 2,
    };

    size_t arch[] = {2, 2, 1};
    NN nn = nn_alloc(arch, ARRAY_LEN(arch));
    NN g = nn_alloc(arch, ARRAY_LEN(arch));
    float eps = 1e-1, rate = 1e-1;

    nn_rand(nn, 0, 1);

    printf("cost = %f\n", nn_cost(nn, ti, to));
    for (size_t i = 0; i < 5e5; i++) {
#if 0
        nn_finite_diff(nn, g, eps, ti, to);
#else
        nn_backprop(nn, g, ti, to);
#endif
        nn_learn(nn, g, rate);
    }
    printf("cost = %f\n", nn_cost(nn, ti, to));

    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            MAT_AT(NN_INPUT(nn), 0, 0) = i;
            MAT_AT(NN_INPUT(nn), 0, 1) = j;
            nn_forward(nn);

            printf("%zu ^ %zu = %f\n", i, j, MAT_AT(NN_OUTPUT(nn), 0, 0));
        }
    }

    return 0;
}
