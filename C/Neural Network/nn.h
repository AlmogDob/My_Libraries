/* This one-file library is heavily inspired by Tsoding's nn.h .
you can find the source code in:
https://github.com/tsoding/nn.h .
featured in this video of his:
https://youtu.be/PGSba51aRYU?list=PLpM-Dvs8t0VZPZKggcql-MmjaBdZKeDMw .*/


#ifndef NN_H_
#define NN_H_

#include <stddef.h>
#include <stdio.h>
#include <math.h>

#ifndef NN_MALLOC
#include <stdlib.h>
#define NN_MALLOC malloc
#endif //NN_MALLOC

#ifndef NN_ASSERT
#include <assert.h>
#define NN_ASSERT assert
#endif //NN_ASSERT

typedef struct {
    size_t rows;
    size_t cols;
    size_t stride;
    float *elements;
    
} Mat;

#define MAT_AT(m, i, j) (m).elements[(i)*(m).stride + (j)]
#define MAT_PRINT(m) mat_print((m), #m, 0)

float rand_float(void);
Mat mat_alloc(size_t rows, size_t cols);
void mat_fill(Mat m, float x);
void mat_rand(Mat m, float low, float high);
void mat_dot(Mat dst, Mat a, Mat b);
void mat_sum(Mat dst, Mat a);
void mat_mult(Mat a, size_t factor);
void mat_print(Mat m, const char *name, size_t padding);
void mat_identity_mat(Mat m);
Mat mat_row(Mat m, size_t row);

float sigmoidf(float x);
void mat_sig(Mat m);
void mat_copy(Mat dst, Mat src);

#endif // NN_H_

#ifdef NN_IMPLEMENTATION

float rand_float(void)
{
    return (float) rand() / (float) RAND_MAX;
}

Mat mat_alloc(size_t rows, size_t cols)
{
    Mat m;
    m.rows = rows;
    m.cols = cols;
    m.stride = cols;
    m.elements = (float*)NN_MALLOC(sizeof(*m.elements)*rows*cols);
    NN_ASSERT(m.elements != NULL);
    return m;    
}

void mat_fill(Mat m, float x)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT_AT(m, i, j) = x;
        }
    }
}

void mat_rand(Mat m, float low, float high)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT_AT(m, i, j) = rand_float()*(high - low) + low;
        }
    }
}

void mat_dot(Mat dst, Mat a, Mat b)
{
    NN_ASSERT(a.cols == b.rows);
    size_t n = a.cols;
    NN_ASSERT(a.rows == dst.rows);
    NN_ASSERT(b.cols == dst.cols);
    mat_fill(dst, 0);

    for (size_t i = 0; i < dst.rows; i++) {
        for (size_t j = 0; j < dst.cols; j++) {
            for (size_t k = 0; k < n; k++) {
                MAT_AT(dst, i, j) += MAT_AT(a, i, k)*MAT_AT(b, k, j);
            }
        }
    }

}

void mat_sum(Mat dst, Mat a)
{
    NN_ASSERT(dst.rows == a.rows);
    NN_ASSERT(dst.cols == a.cols);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            MAT_AT(dst, i, j) += MAT_AT(a, i, j);
        }
    }
}

void mat_mult(Mat m, size_t factor)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT_AT(m, i, j) = MAT_AT(m, i, j) * factor;
        }
    }
}

void mat_print(Mat m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows; ++i) {
        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < m.cols; ++j) {
            printf("%f ", MAT_AT(m, i, j));
        }
        printf("\n");
    }
    printf("%*s]\n", (int) padding, "");
}

/* make matrix into the identity matrix*/
void mat_identity_mat(Mat m)
{
    NN_ASSERT(m.cols == m.rows);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            if (i == j) {
                MAT_AT(m, i, j) = 1;
            }
            else {
                MAT_AT(m, i, j) = 0;
            }
        }
    }
}

Mat mat_row(Mat m, size_t row)
{
    return (Mat){
        .rows = 1,
        .cols = m.cols,
        .stride = m.stride,
        .elements = &MAT_AT(m, row, 0)
    };
}

float sigmoidf(float x)
{
    return 1.f / (1.f + expf(-x));
}

void mat_sig(Mat m)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT_AT(m, i, j) = sigmoidf(MAT_AT(m, i, j));
        }
    }
    
}

void mat_copy(Mat dst, Mat src)
{
    NN_ASSERT(dst.cols == src.cols);
    NN_ASSERT(dst.rows == src.rows);

    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            MAT_AT(dst, i, j) = MAT_AT(src, i, j);
        }
    }
    
}


#endif // NN_IMPLEMENTATION