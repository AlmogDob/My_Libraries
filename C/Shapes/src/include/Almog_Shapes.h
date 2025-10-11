
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
    Point normals[3];
    uint32_t colors[3];
    bool to_draw;
    float light_intensity[3];
} Tri; /* explicit tri */
#endif

typedef struct {
    size_t points_index[3];
    size_t neighbor_tri_index[3];
} Tri_implicit;

typedef struct {
    size_t length;
    size_t capacity;
    Tri_implicit *elements;
} Tri_implicit_ada; /* Tri implicit ada array */

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

typedef struct {
    Curve points;
    Tri_implicit_ada triangles;
} Tri_implicit_mesh;

#ifndef HexARGB_RGBA
#define HexARGB_RGBA(x) ((x)>>(8*2)&0xFF), ((x)>>(8*1)&0xFF), ((x)>>(8*0)&0xFF), ((x)>>(8*3)&0xFF)
#endif
#ifndef HexARGB_RGB_VAR
#define HexARGB_RGB_VAR(x, r, g, b) r = ((x)>>(8*2)&0xFF); g = ((x)>>(8*1)&0xFF); b = ((x)>>(8*0)&0xFF);
#endif
#ifndef HexARGB_RGBA_VAR
#define HexARGB_RGBA_VAR(x, r, g, b, a) r = ((x)>>(8*2)&0xFF); g = ((x)>>(8*1)&0xFF); b = ((x)>>(8*0)&0xFF); a = ((x)>>(8*3)&0xFF)
#endif
#ifndef RGB_hexRGB
#define RGB_hexRGB(r, g, b) (int)(0x010000*(int)(r) + 0x000100*(int)(g) + 0x000001*(int)(b))
#endif
#ifndef RGBA_hexARGB
#define RGBA_hexARGB(r, g, b, a) (int)(0x01000000l*(int)(fminf(a, 255)) + 0x010000*(int)(r) + 0x000100*(int)(g) + 0x000001*(int)(b))
#endif

#define             as_point_interpolate(p, p1, p2, t)              \
                        (p).x = (p1).x * (t) + (p2).x * (1 - (t));  \
                        (p).y = (p1).y * (t) + (p2).y * (1 - (t));  \
                        (p).z = (p1).z * (t) + (p2).z * (1 - (t));  \
                        (p).w = (p1).w * (t) + (p2).w * (1 - (t))
#define             as_points_equal(p1, p2) (p1).x == (p2).x && (p1).y == (p2).y && (p1).z == (p2).z

#define             AS_CURVE_PRINT(c) as_curve_print(c, #c, 0)
#define             AS_TRI_IMPLICIT_MESH_PRINT(tm) as_tri_implicit_mesh_print(tm, #tm, 0)
#define             AS_TRI_MESH_PRINT(tm) as_tri_mesh_print(tm, #tm, 0)

/* init functions */
Tri_implicit_mesh   as_Tri_implicit_mesh_init(void);

/* printing functions */
void                as_curve_print(Curve c, char *name, size_t padding);
void                as_tri_print(Tri tri, char *name, size_t padding);
void                as_tri_implicit_mesh_print(Tri_implicit_mesh mesh, char *name, size_t padding);
void                as_tri_mesh_print(Tri_mesh mesh, char *name, size_t padding);

/* utils functions */
uint32_t            as_color_interpolate(uint32_t c1, uint32_t c2, float t);
void                as_curve_ada_free(Curve_ada curves);
Point               as_mat2D_to_point(Mat2D m);
void                as_point_to_mat2D(Point p, Mat2D m);
size_t              as_point_in_curve_occurrences(Point p, Curve c);
int                 as_point_in_curve_index(Point p, Curve c);
void                as_tri_set_normals(Tri *tri);
Tri_mesh            as_tri_implicit_mesh_to_tri_mesh(Tri_implicit_mesh implicit_mesh, float light_intensity, uint32_t color);
Tri_implicit_mesh   as_tri_mesh_to_tri_implicit_mesh(Tri_mesh mesh);
void                as_tri_mesh_copy(Tri_mesh *des, Tri_mesh src);
void                as_tri_mesh_subdivide_simple(Tri_mesh *mesh);

/* circle operations functions */
Curve               as_circle_curve_create(const Point center, const float r, const size_t num_of_points, const uint32_t color, const char plane[]);
Tri_mesh            as_circle_tri_mesh_create_simple(const Point center, const float r, const size_t num_of_points, const uint32_t color, float light_intensity, const char plane[]);

/* cube operations functions */
Tri_mesh            as_cube_create_tri_mesh_simple(const size_t len, const uint32_t color);

/* sphere operations functions */
Curve_ada           as_sphere_curve_ada_create(const Point center, const float r, const size_t num_of_points_horizontal, const size_t num_of_points_vertical, const uint32_t color);
Tri_mesh            as_sphere_tri_mesh_create_simple(const Point center, const float r, const size_t num_of_points_horizontal, const size_t num_of_points_vertical, const uint32_t color, float light_intensity);

#endif /*ALMOG_SHAPES_H_*/

#ifdef ALMOG_SHAPES_IMPLEMENTATION
#undef ALMOG_SHAPES_IMPLEMENTATION

Tri_implicit_mesh as_Tri_implicit_mesh_init(void)
{
    Tri_implicit_mesh mesh = {0};

    ada_init_array(Point, mesh.points);
    ada_init_array(Tri_implicit, mesh.triangles);

    return mesh;
}

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

void as_tri_implicit_mesh_print(Tri_implicit_mesh mesh, char *name, size_t padding)
{
    char tri_name[256];
    printf("%*s%s:\n", (int) padding, "", name);
    for (size_t i = 0; i < mesh.triangles.length; i++) {
        Tri_implicit tri_imp = mesh.triangles.elements[i];

        snprintf(tri_name, 256, "tri %zu", i);
        printf("%*s%s:\n", (int) padding+4, "", tri_name);
        printf("%*s    points:\n", (int)padding+4, "");
        printf("%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n", (int) padding+4, "", mesh.points.elements[tri_imp.points_index[0]].x, mesh.points.elements[tri_imp.points_index[0]].y, mesh.points.elements[tri_imp.points_index[0]].z, (int) padding+4, "", mesh.points.elements[tri_imp.points_index[1]].x, mesh.points.elements[tri_imp.points_index[1]].y, mesh.points.elements[tri_imp.points_index[1]].z, (int) padding+4, "", mesh.points.elements[tri_imp.points_index[2]].x, mesh.points.elements[tri_imp.points_index[2]].y, mesh.points.elements[tri_imp.points_index[2]].z);
    }
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

uint32_t as_color_interpolate(uint32_t c1, uint32_t c2, float t)
{
    uint32_t r1, g1, b1, a1;
    uint32_t r2, g2, b2, a2;
    HexARGB_RGBA_VAR(c1, r1, g1, b1, a1);
    HexARGB_RGBA_VAR(c2, r2, g2, b2, a2);

    uint32_t r = r1 * t + r2 * (1 - t);
    uint32_t g = g1 * t + g2 * (1 - t);
    uint32_t b = b1 * t + b2 * (1 - t);
    uint32_t a = a1 * t + a2 * (1 - t);

    return RGBA_hexARGB(r, g, b, a);
}

void as_curve_ada_free(Curve_ada curves)
{
    for (size_t i = 0; i < curves.length; i++) {
        free(curves.elements[i].elements);
    }
    free(curves.elements);
}

Point as_mat2D_to_point(Mat2D m)
{
    Point res = {.x = MAT2D_AT(m, 0, 0), .y = MAT2D_AT(m, 1, 0), .z = MAT2D_AT(m, 2, 0), .w = 1};
    return res;
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

/* returns num of occurrences of p in c */
size_t as_point_in_curve_occurrences(Point p, Curve c)
{
    size_t num_of_occurrences = 0;

    for (size_t i = 0; i < c.length; i++) {
        if (as_points_equal(c.elements[i], p)) num_of_occurrences++;
    }

    return num_of_occurrences;
}

/* return the first index where c[index] = p or -1 if p is not in c */
int as_point_in_curve_index(Point p, Curve c)
{
    int index = -1;

    for (size_t i = 0; i < c.length; i++) {
        if (as_points_equal(c.elements[i], p)) index = i;
    }

    return index;
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

Tri_mesh as_tri_implicit_mesh_to_tri_mesh(Tri_implicit_mesh implicit_mesh, float light_intensity, uint32_t color)
{
    Tri_mesh mesh = {0};
    ada_init_array(Tri, mesh);

    for (size_t tri_index = 0; tri_index < implicit_mesh.triangles.length; tri_index++) {
        Tri_implicit current_imp_tri = implicit_mesh.triangles.elements[tri_index];
        Tri tri = {0};
        tri.to_draw = 1;
        for (size_t i = 0; i < 3; i++) {
            tri.colors[i] = color;
            tri.light_intensity[i] = light_intensity;
            tri.points[i] = implicit_mesh.points.elements[current_imp_tri.points_index[i]];
        }
        as_tri_set_normals(&tri);
        ada_appand(Tri, mesh, tri);
    }

    return mesh;
}

Tri_implicit_mesh as_tri_mesh_to_tri_implicit_mesh(Tri_mesh mesh)
{
    Tri_implicit_mesh implicit_mesh = as_Tri_implicit_mesh_init();

    for (size_t tri_index = 0; tri_index < mesh.length; tri_index++) {
        Tri current_tri = mesh.elements[tri_index];
        Tri_implicit imp_tri = {0};
        for (size_t i = 0; i < 3; i++) {
            if (as_point_in_curve_index(current_tri.points[i], implicit_mesh.points) == -1) {
                ada_appand(Point, implicit_mesh.points, current_tri.points[i]);
            }
            size_t point_index = as_point_in_curve_index(current_tri.points[i], implicit_mesh.points);
            imp_tri.points_index[i] = point_index;
        }
        ada_appand(Tri_implicit, implicit_mesh.triangles, imp_tri);
    }

    return implicit_mesh;
}

void as_tri_mesh_copy(Tri_mesh *des, Tri_mesh src)
{
    Tri_mesh temp_des = *des;
    temp_des.length = 0;
    for (size_t i = 0; i < src.length; i++) {
        ada_appand(Tri, temp_des, src.elements[i]);
    }
    *des = temp_des;
}

void as_tri_mesh_subdivide_simple(Tri_mesh *mesh)
{
    Tri_mesh origin_mesh = {0};
    ada_init_array(Tri, origin_mesh);
    as_tri_mesh_copy(&origin_mesh, *mesh);

    Tri_mesh new_mesh = *mesh;
    new_mesh.length = 0;
    
    for (size_t tri_index = 0; tri_index < origin_mesh.length; tri_index++) {
        Tri current_tri = origin_mesh.elements[tri_index];
        Point p0 = current_tri.points[0];
        Point p1 = current_tri.points[1];
        Point p2 = current_tri.points[2];
        Point p01 = {0}, p12 = {0}, p20 = {0};
        as_point_interpolate(p01, p0, p1, 0.5);
        as_point_interpolate(p12, p1, p2, 0.5);
        as_point_interpolate(p20, p2, p0, 0.5);
        uint32_t c01 = as_color_interpolate(current_tri.colors[0], current_tri.colors[1], 0.5);
        uint32_t c12 = as_color_interpolate(current_tri.colors[1], current_tri.colors[2], 0.5);
        uint32_t c20 = as_color_interpolate(current_tri.colors[2], current_tri.colors[0], 0.5);
        float t = 0.5;
        float light_intensity01 = current_tri.light_intensity[0] * t + current_tri.light_intensity[1] * (1 - t);
        float light_intensity12 = current_tri.light_intensity[1] * t + current_tri.light_intensity[2] * (1 - t);
        float light_intensity20 = current_tri.light_intensity[2] * t + current_tri.light_intensity[0] * (1 - t);

        Tri tri1 = {0};
        tri1.points[0] = current_tri.points[0];
        tri1.points[1] = p01;
        tri1.points[2] = p20;
        tri1.colors[0] = current_tri.colors[0];
        tri1.colors[1] = c01;
        tri1.colors[2] = c20;
        tri1.light_intensity[0] = current_tri.light_intensity[0];
        tri1.light_intensity[1] = light_intensity01;
        tri1.light_intensity[2] = light_intensity20;
        as_tri_set_normals(&tri1);
        tri1.to_draw = 1;
        ada_appand(Tri, new_mesh, tri1);
        
        Tri tri2 = {0};
        tri2.points[0] = current_tri.points[1];
        tri2.points[1] = p12;
        tri2.points[2] = p01;
        tri2.colors[0] = current_tri.colors[1];
        tri2.colors[1] = c12;
        tri2.colors[2] = c01;
        tri2.light_intensity[0] = current_tri.light_intensity[1];
        tri2.light_intensity[1] = light_intensity12;
        tri2.light_intensity[2] = light_intensity01;
        as_tri_set_normals(&tri2);
        tri2.to_draw = 1;
        ada_appand(Tri, new_mesh, tri2);
        
        Tri tri3 = {0};
        tri3.points[0] = current_tri.points[2];
        tri3.points[1] = p20;
        tri3.points[2] = p12;
        tri3.colors[0] = current_tri.colors[2];
        tri3.colors[1] = c20;
        tri3.colors[2] = c12;
        tri3.light_intensity[0] = current_tri.light_intensity[2];
        tri3.light_intensity[1] = light_intensity20;
        tri3.light_intensity[2] = light_intensity12;
        as_tri_set_normals(&tri3);
        tri3.to_draw = 1;
        ada_appand(Tri, new_mesh, tri3);
        
        Tri tri4 = {0};
        tri4.points[0] = p01;
        tri4.points[1] = p12;
        tri4.points[2] = p20;
        tri4.colors[0] = c01;
        tri4.colors[1] = c12;
        tri4.colors[2] = c20;
        tri4.light_intensity[0] = light_intensity01;
        tri4.light_intensity[1] = light_intensity12;
        tri4.light_intensity[2] = light_intensity20;
        as_tri_set_normals(&tri4);
        tri4.to_draw = 1;
        ada_appand(Tri, new_mesh, tri4);
    }

    free(origin_mesh.elements);

    *mesh = new_mesh;
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

Tri_mesh as_circle_tri_mesh_create_simple(const Point center, const float r, const size_t num_of_points, const uint32_t color, float light_intensity, const char plane[])
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

    Tri_mesh mesh = {0};
    ada_init_array(Tri, mesh);

    for (size_t p_index = 0; p_index < c.length; p_index++) {
        size_t p_index_p1 = (p_index + 1) % c.length;
        Tri tri = {0};
        for (int i = 0; i < 3; i++) {
            tri.colors[i] = color;
            tri.light_intensity[i] = light_intensity;
        }
        tri.to_draw = 1;
        
        tri.points[0] = c.elements[0];
        tri.points[1] = c.elements[p_index_p1];
        tri.points[2] = c.elements[p_index];
        as_tri_set_normals(&tri);
        ada_appand(Tri, mesh, tri);

    }

    free(c.elements);

    return mesh;
}

Tri_mesh as_cube_create_tri_mesh_simple(const size_t len, const uint32_t color)
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

Tri_mesh as_sphere_tri_mesh_create_simple(const Point center, const float r, const size_t num_of_points_horizontal, const size_t num_of_points_vertical, const uint32_t color, float light_intensity)
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
