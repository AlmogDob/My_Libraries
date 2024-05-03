#define NN_IMPLEMENTATION
#include "nn.h"
#include <time.h>

#define BITS 3
#define dprintF(expr) printf(#expr " = %f\n", expr)
#define dprintT(expr) printf(#expr " = %zu\n", expr)

int main(void)
{
    srand(time(0));
    size_t n = (1<<BITS);
    size_t rows = n*n;
    Mat ti = mat_alloc(rows, 2*BITS), to = mat_alloc(rows, BITS + 1);
    size_t x, y, z;
    
    for (size_t i = 0; i < ti.rows; i++) {
        x = i/n;
        y = i%n;
        z = x + y;
        /*test*/
        // printf("i = %zu, x = %zu, y = %zu, z = %zu\n", i, x, y, z);
        /*test*/
        size_t overflow = z >= n;
        for (size_t j = 0; j < BITS; j++) {
            // size_t k = BITS - 1 - j;
            // MAT_AT(ti, i, j) = (x>>j)&1;
            // MAT_AT(ti, i, k + BITS) = (y>>j)&1;

            MAT_AT(ti, i, j) = (x>>j)&1;
            MAT_AT(ti, i, j + BITS) = (y>>j)&1;
            MAT_AT(to, i, j) = (z>>j)&1;
        }
        MAT_AT(to, i, BITS) = (z>>BITS)&1; 
    }

    // MAT_PRINT(ti);
    // MAT_PRINT(to);

    size_t arch[] = {2*BITS, 2*BITS + 1, BITS + 1};
    NN nn = nn_alloc(arch, ARRAY_LEN(arch));    
    NN g = nn_alloc(arch, ARRAY_LEN(arch));    
    float rate = 1e0, overflow;
    size_t bit, number_of_fails = 0;
    nn_rand(nn, 0, 1);
/*-------------------------------------------------- */
    printf("c = %f\n", nn_cost(nn, ti, to));
    for (size_t i = 0; i < 5e4; i++) {
        nn_backprop(nn, g, ti, to);
        nn_learn(nn, g, rate);
    }
    printf("c = %f\n", nn_cost(nn, ti, to));
/*-------------------------------------------------- */

    for (size_t x = 0; x < n; x++) {
        for (size_t y = 0; y < n; y++) {
            printf("%zu + %zu = ", x, y);

            for (size_t k = 0; k < BITS; k++) {
                MAT_AT(NN_INPUT(nn), 0, k) = MAT_AT(ti, x*n+y, k);
                MAT_AT(NN_INPUT(nn), 0, k + BITS) = MAT_AT(ti, x*n+y, k + BITS);
            }
            nn_forward(nn);
            z = 0;
            for (size_t j = 0; j < BITS + 1; j++) {
                bit = MAT_AT(NN_OUTPUT(nn), 0, j) > 0.5f;
                z |= bit<<j;
            }
            overflow = 0;MAT_AT(NN_OUTPUT(nn), 0, BITS) > 0.5f;
            if (!overflow) {
                if ((x+y) != z) {
                    printf("%zu, BAD!!!\n", z);
                    number_of_fails++;
                } else {
                    printf("%zu\n", z);
                }
            } else {
                printf("%s\n", "OVERFLOW");
            }
        }
    }
    dprintT(number_of_fails);

    return 0;
}
