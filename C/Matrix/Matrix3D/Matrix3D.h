/* This one-file library is heavily inspired by Tsoding's nn.h implementation of matrix
creation and operation. you can find the source code in:
https://github.com/tsoding/nn.h .
featured in this video of his:
https://youtu.be/L1TbWe8bVOc?list=PLpM-Dvs8t0VZPZKggcql-MmjaBdZKeDMw .*/


#ifndef MATRIX3D_H_
#define MATRIX3D_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef MATRIX3D_MALLOC
#define MATRIX3D_MALLOC malloc
#endif //MATRIX3D_MALLOC

#ifndef MATRIX3D_ASSERT
#include <assert.h>
#define MATRIX3D_ASSERT assert
#endif //MATRIX3D_ASSERT

typedef struct {
    size_t rows;
    size_t cols;
    size_t layers;
    size_t stride;
    float *elements;
    
} Mat3D;

#define MAT3D_AT(m, i, j, k) (m).elements[((i)*(m).cols + (j))*(m).layers + (k)]
#define MAT3D_PRINT(m) mat3D_print(m, #m, 0)

float rand_float(void);
Mat3D mat3D_alloc(size_t rows, size_t cols, size_t layers);
void mat3D_free(Mat3D m);
void mat3D_fill(Mat3D m, float x);
void mat3D_rand(Mat3D m, float low, float high);
void mat3D_sum(Mat3D dst, Mat3D a);
void mat3D_mult(Mat3D m, size_t factor);
void mat3D_print(Mat3D m, const char *name, size_t padding);
void mat3D_identity_mat(Mat3D m);
void mat3D_copy(Mat3D res, Mat3D src);

#endif // MATRIX3D_H_

#ifdef MATRIX3D_IMPLEMENTATION

float rand_float(void)
{
    return (float) rand() / (float) RAND_MAX;
}

Mat3D mat3D_alloc(size_t rows, size_t cols, size_t layers)
{
    Mat3D m;
    m.rows = rows;
    m.cols = cols;
    m.layers = layers; 
    m.stride = cols * layers;
    m.elements = (float*)MATRIX3D_MALLOC(sizeof(*m.elements)*rows*cols*layers);
    MATRIX3D_ASSERT(m.elements != NULL);
    return m;    
}

void mat3D_free(Mat3D m)
{
    free(m.elements);
}

void mat3D_fill(Mat3D m, float x)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            for (size_t k = 0; k < m.cols; ++k) {
                MAT3D_AT(m, i, j, k) = x;
            }
        }
    }
}

void mat3D_rand(Mat3D m, float low, float high)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            for (size_t k = 0; k < m.layers; ++k) {
                MAT3D_AT(m, i, j, k) = rand_float()*(high - low) + low;
            }
        }
    }
}

void mat3D_sum(Mat3D dst, Mat3D a)
{
    MATRIX3D_ASSERT(dst.rows == a.rows);
    MATRIX3D_ASSERT(dst.cols == a.cols);
    MATRIX3D_ASSERT(dst.layers == a.layers);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            for (size_t k = 0; k < dst.layers; ++k) {
                MAT3D_AT(dst, i, j, k) += MAT3D_AT(a, i, j, k);
            }
        }
    }
}

void mat3D_mult(Mat3D m, size_t factor)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            for (size_t k = 0; k < m.layers; ++k) {
                MAT3D_AT(m, i, j, k) *= factor;
            }
        }
    }
}

void mat3D_print(Mat3D m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t k = 0; k < m.layers; ++k) {
        printf("%*s    k=%zu\n", (int) padding, "", k);
        for (size_t i = 0; i < m.rows; ++i) {
            printf("%*s    ", (int) padding, "");
            for (size_t j = 0; j < m.cols; ++j) {
                printf("%f ", MAT3D_AT(m, i, j, k));
            }
            printf("\n");
        }

        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < m.cols; ++j) {
            printf("---------");
        }
        printf("\n");
    }
        
    printf("%*s]\n", (int) padding, "");
}

void mat3D_identity_mat(Mat3D m)
{
    MATRIX3D_ASSERT(m.cols == m.rows);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            for (size_t k = 0; k < m.cols; ++k) {
                if (i == j && j == k) {
                    MAT3D_AT(m, i, j, k) = 1;
                }
                else {
                    MAT3D_AT(m, i, j, k) = 0;
                }
            }
        }
    }
}

void mat3D_copy(Mat3D res, Mat3D src)
{
    MATRIX3D_ASSERT(res.cols == src.cols);
    MATRIX3D_ASSERT(res.rows == src.rows);
    MATRIX3D_ASSERT(res.layers == src.layers);

    for (size_t i = 0; i < res.rows; ++i) {
        for (size_t j = 0; j < res.cols; ++j) {
            for (size_t k = 0; k < res.cols; ++k) {
                MAT3D_AT(res, i, j, k) = MAT3D_AT(src, i, j, k);
            }
        }
    }
}

#endif // MATRIX3D_IMPLEMENTATION
