
#ifndef ALMOG_SHAPES_H_
#define ALMOG_SHAPES_H_

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include "Matrix2D.h"
#include "Almog_Dynamic_Array.h"

#ifndef AS_ASSERT
#include <assert.h>
#define AS_ASSERT assert
#endif

#ifndef PI
#define PI M_PI
#endif

#ifndef POINT
#define POINT
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Point;
#endif

#ifndef CURVE
#define CURVE
typedef struct {
    uint32_t color;
    size_t length;
    size_t capacity;
    Point *elements;
} Curve;
#endif

#ifndef CURVE_ADA
#define CURVE_ADA
typedef struct {
    size_t length;
    size_t capacity;
    Curve *elements;
} Curve_ada;
#endif

#ifndef TRI
#define TRI
typedef struct {
    Point points[3];
    Point tex_points[3];
    Point center;
    Point normals[3];
    uint32_t colors[3];
    float z_min;
    float z_max;
    bool to_draw;
    float light_intensity[3];
} Tri;
#endif

#ifndef QUAD
#define QUAD
typedef struct {
    Point points[4];
    Point normals[4];
    uint32_t colors[4];
    bool to_draw;
    float light_intensity[4];
} Quad;
#endif

#ifndef TRI_MESH
#define TRI_MESH
typedef struct {
    size_t length;
    size_t capacity;
    Tri *elements;
} Tri_mesh; /* Tri ada array */
#endif

#define AS_TRI_MESH_PRINT(tm) as_tri_mesh_print(tm, #tm, 0)
#define AS_CURVE_PRINT(c) as_curve_print(c, #c, 0)

void        as_curve_print(Curve c, char *name, size_t padding);
void        as_tri_print(Tri tri, char *name, size_t padding);
void        as_tri_mesh_print(Tri_mesh mesh, char *name, size_t padding);

void        as_point_to_mat2D(Point p, Mat2D m);
Point       as_mat2D_to_point(Mat2D m);
void        as_tri_set_normals(Tri *tri);
void        as_curve_ada_free(Curve_ada curves);

Tri_mesh    as_cube_create_tri_mesh(const size_t len, const uint32_t color);
Curve       as_circle_curve_create(const Point center, const float r, const size_t num_of_points, const uint32_t color, const char plane[]);
Curve_ada   as_sphere_curve_ada_create(const Point center, const float r, const size_t num_of_points_horizontal, const size_t num_of_points_vertical, const uint32_t color);
Tri_mesh    as_sphere_tri_mesh_create(const Point center, const float r, const size_t num_of_points_horizontal, const size_t num_of_points_vertical, const uint32_t color, float light_intensity);

#endif /*ALMOG_SHAPES_H_*/

#ifdef ALMOG_SHAPES_IMPLEMENTATION
#undef ALMOG_SHAPES_IMPLEMENTATION

void as_curve_print(Curve c, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);
    for (size_t i = 0; i < c.length; i++) {
        printf("%*s    point%zu: (%f, %f, %f)\n", (int) padding, "", i, c.elements[i].x, c.elements[i].y, c.elements[i].z);
    }
    printf("%*s    color: %X\n", (int)padding, "", c.color);
}

void as_tri_print(Tri tri, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);
    printf("%*s    points:\n", (int)padding, "");
    printf("%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n", (int) padding, "", tri.points[0].x, tri.points[0].y, tri.points[0].z, (int) padding, "", tri.points[1].x, tri.points[1].y, tri.points[1].z, (int) padding, "", tri.points[2].x, tri.points[2].y, tri.points[2].z);
    printf("%*s    normals:\n", (int)padding, "");
    printf("%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n", (int) padding, "", tri.normals[0].x, tri.normals[0].y, tri.normals[0].z, (int) padding, "", tri.normals[1].x, tri.normals[1].y, tri.normals[1].z, (int) padding, "", tri.normals[2].x, tri.normals[2].y, tri.normals[2].z);
    printf("%*s    colors:\n", (int)padding, "");
    printf("%*s    (%X, %X, %X)\n", (int)padding, "", tri.colors[0], tri.colors[1], tri.colors[2]);
    printf("%*s    light intensity:\n", (int)padding, "");
    printf("%*s    (%f, %f, %f)\n", (int)padding, "", tri.light_intensity[0], tri.light_intensity[1], tri.light_intensity[2]);
    printf("%*s    draw? %d\n", (int)padding, "", tri.to_draw);
}

void as_tri_mesh_print(Tri_mesh mesh, char *name, size_t padding)
{
    char tri_name[256];
    printf("%*s%s:\n", (int) padding, "", name);
    for (size_t i = 0; i < mesh.length; i++) {
        snprintf(tri_name, 256, "tri %zu", i);
        as_tri_print(mesh.elements[i], tri_name, 4);
    }
}

void as_point_to_mat2D(Point p, Mat2D m)
{
    MATRIX2D_ASSERT((3 == m.rows && 1 == m.cols) || (1 == m.rows && 3 == m.cols));
    
    if (3 == m.rows) {
        MAT2D_AT(m, 0, 0) = p.x;
        MAT2D_AT(m, 1, 0) = p.y;
        MAT2D_AT(m, 2, 0) = p.z;
    }
    if (3 == m.cols) {
        MAT2D_AT(m, 0, 0) = p.x;
        MAT2D_AT(m, 0, 1) = p.y;
        MAT2D_AT(m, 0, 2) = p.z;
    }
}

Point as_mat2D_to_point(Mat2D m)
{
    Point res = {.x = MAT2D_AT(m, 0, 0), .y = MAT2D_AT(m, 1, 0), .z = MAT2D_AT(m, 2, 0), .w = 1};
    return res;
}

void as_tri_set_normals(Tri *tri)
{
    Mat2D point  = mat2D_alloc(3, 1);
    Mat2D to_p   = mat2D_alloc(3, 1);
    Mat2D from_p = mat2D_alloc(3, 1);
    Mat2D normal = mat2D_alloc(3, 1);

    for (int i = 0; i < 3; i++) {
        int current_index  = i;
        int next_index     = (i + 1) % 3;
        int previous_index = (i - 1 + 3) % 3;
        as_point_to_mat2D(tri->points[current_index], point);
        as_point_to_mat2D(tri->points[next_index], from_p);
        as_point_to_mat2D(tri->points[previous_index], to_p);

        mat2D_sub(from_p, point);
        mat2D_sub(point, to_p);

        mat2D_copy(to_p, point);

        mat2D_cross(normal, to_p, from_p);
        mat2D_normalize(normal);

        tri->normals[current_index] = as_mat2D_to_point(normal);
    }

    mat2D_free(point);
    mat2D_free(to_p);
    mat2D_free(from_p);
    mat2D_free(normal);
}

void as_curve_ada_free(Curve_ada curves)
{
    for (size_t i = 0; i < curves.length; i++) {
        free(curves.elements[i].elements);
    }
    free(curves.elements);
}

Tri_mesh as_cube_create_tri_mesh(const size_t len, const uint32_t color)
{
    Tri_mesh cube;

    ada_init_array(Tri, cube);

    Tri tri1 = { /* south */
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = 0,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = 0,
        .to_draw = true,
        .colors = {color, color, color},
    };
    as_tri_set_normals(&tri1);
    ada_appand(Tri, cube, tri1);
    Tri tri2 = {
        .points[0].x = len,
        .points[0].y = len,
        .points[0].z = 0,

        .points[1].x = len,
        .points[1].y = 0,
        .points[1].z = 0,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = 0,
        .to_draw = true,
        .colors = {color, color, color},
    };
    as_tri_set_normals(&tri2);
    ada_appand(Tri, cube, tri2);
    Tri tri3 = { /* north */
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = len,
        .points[2].z = len,
        .to_draw = true,
        .colors = {color, color, color},
    };
    as_tri_set_normals(&tri3);
    ada_appand(Tri, cube, tri3);
    Tri tri4 = {
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = len,
        .to_draw = true,
        .colors = {color, color, color},
    };
    as_tri_set_normals(&tri4);
    ada_appand(Tri, cube, tri4);
    Tri tri5 = { /* east */
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = 0,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = len,
        .to_draw = true,
        .colors = {color, color, color},
    };
    as_tri_set_normals(&tri5);
    ada_appand(Tri, cube, tri5);
    Tri tri6 = {
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = 0,
        .points[2].z = len,
        .to_draw = true,
        .colors = {color, color, color},
    };
    as_tri_set_normals(&tri6);
    ada_appand(Tri, cube, tri6);
    Tri tri7 = { /* west */
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = len,
        .points[2].z = 0,
        .to_draw = true,
        .colors = {color, color, color},
    };
    as_tri_set_normals(&tri7);
    ada_appand(Tri, cube, tri7);
    Tri tri8 = {
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = 0,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = 0,
        .to_draw = true,
        .colors = {color, color, color},
    };
    as_tri_set_normals(&tri8);
    ada_appand(Tri, cube, tri8);
    Tri tri9 = { /* top */
        .points[0].x = 0,
        .points[0].y = len,
        .points[0].z = 0,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = len,
        .to_draw = true,
        .colors = {color, color, color},
    };
    as_tri_set_normals(&tri9);
    ada_appand(Tri, cube, tri9);
    Tri tri10 = {
        .points[0].x = 0,
        .points[0].y = len,
        .points[0].z = 0,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = 0,
        .to_draw = true,
        .colors = {color, color, color},
    };
    as_tri_set_normals(&tri10);
    ada_appand(Tri, cube, tri10);
    Tri tri11 = { /* bottom */
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = 0,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = 0,
        .to_draw = true,
        .colors = {color, color, color},
    };
    as_tri_set_normals(&tri11);
    ada_appand(Tri, cube, tri11);
    Tri tri12 = {
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = 0,
        .points[1].z = 0,

        .points[2].x = len,
        .points[2].y = 0,
        .points[2].z = 0,
        .to_draw = true,
        .colors = {color, color, color},
    };
    as_tri_set_normals(&tri12);
    ada_appand(Tri, cube, tri12);
    
    return cube;
}

Curve as_circle_curve_create(const Point center, const float r, const size_t num_of_points, const uint32_t color, const char plane[])
{
    AS_ASSERT(r > 0);
    AS_ASSERT(num_of_points > 0);
    AS_ASSERT((!strncmp(plane, "XY", 3) || !strncmp(plane, "xy", 3)) && "other planes are no implemented.");

    float delta_theta = 2 * PI / (float)num_of_points; 

    Curve c = {0};
    ada_init_array(Point, c);
    c.color = color;

    for (int i = 0; i < (int)num_of_points; i++) {
        Point p = center;

        p.x += r * cosf(delta_theta * i - PI / 2.0f);
        p.y += r * sinf(delta_theta * i - PI / 2.0f);

        ada_appand(Point, c, p);
    }

    return c;
}

Curve_ada as_sphere_curve_ada_create(const Point center, const float r, const size_t num_of_points_horizontal, const size_t num_of_points_vertical, const uint32_t color)
{
    AS_ASSERT(r > 0);
    AS_ASSERT(num_of_points_horizontal > 0);
    AS_ASSERT(num_of_points_vertical > 0);
    AS_ASSERT(!(num_of_points_horizontal % 2) && "needs to be even");
    AS_ASSERT(!(num_of_points_vertical % 2) && "needs to be even");

    Curve_ada sphere = {0};
    ada_init_array(Curve, sphere);

    float delta_theta_hor = 2.0f * PI / (float)num_of_points_horizontal;
    float delta_theta_ver =     PI / ((float)num_of_points_vertical - 1);

    for (size_t ver = 0; ver < num_of_points_vertical; ver++) {
        Curve c = {0};
        ada_init_array(Point, c);
        c.color = color;
        for (size_t hor = 0; hor < num_of_points_horizontal; hor++) {
            Point p = center;

            p.x += r * sin(delta_theta_ver * ver) * cosf(delta_theta_hor * hor);
            p.z += r * sin(delta_theta_ver * ver) * sinf(delta_theta_hor * hor);
            p.y += r * cos(delta_theta_ver * ver);

            ada_appand(Point, c, p);
        }
        ada_appand(Curve, sphere, c);
    }

    for (size_t hor = 0; hor < num_of_points_horizontal / 2.0f; hor++) {
        Curve c = {0};
        ada_init_array(Point, c);
        c.color = color;
        for (size_t ver = 0; ver < 2.0f*(num_of_points_vertical)-1; ver++) {
            Point p = center;

            p.x += r * sin(delta_theta_ver * ver) * cosf(delta_theta_hor * hor);
            p.z += r * sin(delta_theta_ver * ver) * sinf(delta_theta_hor * hor);
            p.y += r * cos(delta_theta_ver * ver);

            ada_appand(Point, c, p);
        }
        ada_appand(Curve, sphere, c);
    }

    return sphere;
}

Tri_mesh as_sphere_tri_mesh_create(const Point center, const float r, const size_t num_of_points_horizontal, const size_t num_of_points_vertical, const uint32_t color, float light_intensity)
{
    AS_ASSERT(r > 0);
    AS_ASSERT(num_of_points_horizontal > 0);
    AS_ASSERT(num_of_points_vertical > 0);
    AS_ASSERT(!(num_of_points_horizontal % 2) && "needs to be even");
    AS_ASSERT(!(num_of_points_vertical % 2) && "needs to be even");

    Curve_ada sphere = {0};
    ada_init_array(Curve, sphere);

    float delta_theta_hor = 2.0f * PI / (float)num_of_points_horizontal;
    float delta_theta_ver =     PI / ((float)num_of_points_vertical - 1);

    for (size_t ver = 0; ver < num_of_points_vertical; ver++) {
        Curve c = {0};
        ada_init_array(Point, c);
        c.color = color;
        for (size_t hor = 0; hor < num_of_points_horizontal; hor++) {
            Point p = center;

            p.x += r * sin(delta_theta_ver * ver) * cosf(delta_theta_hor * hor);
            p.z += r * sin(delta_theta_ver * ver) * sinf(delta_theta_hor * hor);
            p.y += r * cos(delta_theta_ver * ver);

            ada_appand(Point, c, p);
        }
        ada_appand(Curve, sphere, c);
    }

    Tri_mesh mesh = {0};
    ada_init_array(Tri, mesh);

    for (size_t c_index = 1; c_index < sphere.length; c_index++) {
        Curve current_curve  = sphere.elements[c_index];
        Curve previous_curve = sphere.elements[c_index-1];
        for (size_t p_index = 0; p_index < current_curve.length; p_index++) {
            size_t p_index_p1 = (p_index + 1) % current_curve.length;
            Tri tri1 = {0};
            for (int i = 0; i < 3; i++) {
                tri1.colors[i] = color;
                tri1.light_intensity[i] = light_intensity;
            }
            tri1.to_draw = 1;
            
            tri1.points[0] = current_curve.elements[p_index];
            tri1.points[1] = previous_curve.elements[p_index];
            tri1.points[2] = current_curve.elements[p_index_p1];
            as_tri_set_normals(&tri1);
            ada_appand(Tri, mesh, tri1);

            if (c_index != 1) {
                Tri tri2 = tri1;
                tri2.points[0] = current_curve.elements[p_index_p1];
                tri2.points[1] = previous_curve.elements[p_index];
                tri2.points[2] = previous_curve.elements[p_index_p1];
                as_tri_set_normals(&tri2);
                ada_appand(Tri, mesh, tri2);
            }
        }
    }

    as_curve_ada_free(sphere);

    return mesh;
}

#endif /*ALMOG_SHAPES_IMPLEMENTATION*/
