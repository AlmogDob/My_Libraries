
#ifndef ALMOG_SHAPES_H_
#define ALMOG_SHAPES_H_

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

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

typedef struct {
    size_t p1_index;
    size_t p2_index;
    bool is_segment;
} Edge_implicit;

typedef struct {
    size_t length;
    size_t capacity;
    Edge_implicit *elements;
} Edge_implicit_ada; /* Edge implicit ada array */

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

typedef struct {
    size_t edges_index[3];
} Tri_edge_implicit;

typedef struct {
    size_t length;
    size_t capacity;
    Tri_edge_implicit *elements;
} Tri_edge_implicit_ada; /* Tri edge implicit ada array */

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

typedef struct {
    Curve points;
    Edge_implicit_ada edges;
    Tri_edge_implicit_ada triangles;
} Tri_edge_implicit_mesh;

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

#define                 as_point_expand_to_xyz(p) (p).x, (p).y, (p).z
#define                 as_point_add_point(p, p1, p2) (p).x = (p1).x + (p2).x;  \
                            (p).y = (p1).y + (p2).y;                            \
                            (p).z = (p1).z + (p2).z;                            \
                            (p).w = (p1).w + (p2).w
#define                 as_point_sub_point(p, p1, p2) (p).x = (p1).x - (p2).x;  \
                            (p).y = (p1).y - (p2).y;                            \
                            (p).z = (p1).z - (p2).z;                            \
                            (p).w = (p1).w - (p2).w
#define                 as_point_dot_point(p1, p2) (((p1).x * (p2).x) + ((p1).y * (p2).y) + ((p1).z * (p2).z))
#define                 as_point_mult(p, const) (p).x *= const; \
                            (p).y *= const;                     \
                            (p).z *= const
#define                 as_points_interpolate(p, p1, p2, t)             \
                            (p).x = (p1).x * (t) + (p2).x * (1 - (t));  \
                            (p).y = (p1).y * (t) + (p2).y * (1 - (t));  \
                            (p).z = (p1).z * (t) + (p2).z * (1 - (t));  \
                            (p).w = (p1).w * (t) + (p2).w * (1 - (t))
#define                 as_points_equal(p1, p2) ((p1).x == (p2).x && (p1).y == (p2).y && (p1).z == (p2).z)
#define                 as_tri_area_xy(p1, p2, p3) 0.5 * ((p2).x-(p1).x)*((p3).y-(p1).y)- 0.5 * ((p3).x-(p1).x)*((p2).y-(p1).y)
#define                 as_tri_equal_z(tri) ((tri.points[0].z == tri.points[1].z) && (tri.points[1].z == tri.points[2].z) && (tri.points[2].z == tri.points[0].z))
#define                 as_tri_implicit_mesh_expand_tri_to_points(tim, t_index) (tim).points.elements[(tim).triangles.elements[t_index].points_index[0]], (tim).points.elements[(tim).triangles.elements[t_index].points_index[1]], (tim).points.elements[(tim).triangles.elements[t_index].points_index[2]]
#define                 as_tri_implicit_mesh_get_point_of_tri_implicit(tim, t_index, p_index) (tim).points.elements[(tim).triangles.elements[t_index].points_index[p_index]]
#define                 as_tri_implicit_area_xy(tim, t_index) as_tri_area_xy(as_tri_implicit_mesh_get_point_of_tri_implicit(tim, t_index, 0), as_tri_implicit_mesh_get_point_of_tri_implicit(tim, t_index, 1), as_tri_implicit_mesh_get_point_of_tri_implicit(tim, t_index, 2))
#define                 as_tri_implicit_mesh_get_tri_implicit(tim, t_index) (tim).triangles.elements[t_index]
#define                 as_tri_edge_implicit_mesh_get_point_of_tri(teim, t_index, p_index) teim.points.elements[teim.edges.elements[teim.triangles.elements[t_index].edges_index[p_index]].p1_index]
#define                 as_tri_edge_implicit_mesh_expand_tri_to_points(teim, t_index) teim.points.elements[teim.edges.elements[teim.triangles.elements[t_index].edges_index[0]].p1_index], teim.points.elements[teim.edges.elements[teim.triangles.elements[t_index].edges_index[1]].p1_index], teim.points.elements[teim.edges.elements[teim.triangles.elements[t_index].edges_index[2]].p1_index]
#define                 as_tri_edge_implicit_mesh_expand_point_of_tri_to_xyz(teim, t_index, p_index) teim.points.elements[teim.edges.elements[teim.triangles.elements[t_index].edges_index[p_index]].p1_index].x, teim.points.elements[teim.edges.elements[teim.triangles.elements[t_index].edges_index[p_index]].p1_index].y, teim.points.elements[teim.edges.elements[teim.triangles.elements[t_index].edges_index[p_index]].p1_index].z

/* init functions */

void                    as_curve_ada_free(Curve_ada curves);
Tri_implicit_mesh       as_tri_implicit_mesh_init(void);
void                    as_tri_implicit_mesh_free(Tri_implicit_mesh mesh);
Tri_edge_implicit_mesh  as_tri_edge_implicit_mesh_init(void);
void                    as_tri_edge_implicit_mesh_free(Tri_edge_implicit_mesh mesh);

/* printing functions */

void                    as_point_print(Point p, char *name, size_t padding);
void                    as_curve_print(Curve c, char *name, size_t padding);
void                    as_tri_print(Tri tri, char *name, size_t padding);
void                    as_tri_mesh_print(Tri_mesh mesh, char *name, size_t padding);
void                    as_tri_implicit_print(Tri_implicit tri_imp, Point *points, char *name, size_t padding);
void                    as_tri_implicit_mesh_print(Tri_implicit_mesh mesh, char *name, size_t padding);
void                    as_tri_edge_implicit_mesh_print(Tri_edge_implicit_mesh mesh, char *name, size_t padding);

/* utils functions */

uint32_t                as_color_interpolate(uint32_t c1, uint32_t c2, float t);
Curve                   as_curve_create_random_points(size_t num_of_points, float min_x, float max_x, float min_y, float max_y, float min_z, float max_z, unsigned int seed);
int                     as_edge_implicit_ada_get_edge_index(Edge_implicit_ada eia, Point *points, Point p1, Point p2);
Point                   as_mat2D_to_point(Mat2D m);
void                    as_point_to_mat2D(Point p, Mat2D m);
size_t                  as_point_in_curve_occurrences(Point p, Curve c);
int                     as_point_in_curve_index(Point p, Curve c);
float                   as_points_distance(Point p1, Point p2);
float                   as_rand_float(void);
void                    as_tri_set_normals(Tri *tri);
Tri_implicit_mesh       as_tri_mesh_to_tri_implicit_mesh(Tri_mesh mesh);
void                    as_tri_mesh_copy(Tri_mesh *des, Tri_mesh src);
void                    as_tri_mesh_subdivide_simple(Tri_mesh *mesh);
float                   as_tri_mesh_const_x(Tri_mesh mesh);
float                   as_tri_mesh_const_y(Tri_mesh mesh);
float                   as_tri_mesh_const_z(Tri_mesh mesh);
Tri                     as_tri_implicit_to_tri(Tri_implicit tri_implicit, Point *points);
float                   as_tri_implicit_mesh_const_x(Tri_implicit_mesh mesh);
float                   as_tri_implicit_mesh_const_y(Tri_implicit_mesh mesh);
float                   as_tri_implicit_mesh_const_z(Tri_implicit_mesh mesh);
Tri_mesh                as_tri_implicit_mesh_to_tri_mesh(Tri_implicit_mesh implicit_mesh, float light_intensity, uint32_t color);
Tri_edge_implicit_mesh  as_tri_implicit_mesh_to_tri_edge_implicit_mesh(Tri_implicit_mesh implicit_mesh);
int                     as_tri_edge_implicit_mesh_get_triangles_with_edge(Tri_edge_implicit_mesh mesh, Point p1, Point p2, Tri_edge_implicit *tri_out1, Tri_edge_implicit *tri_out2);
int                     as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge(Tri_edge_implicit_mesh mesh, Point p1, Point p2, size_t *tri_out1_index, size_t *tri_out2_index);
void                    as_tri_edge_implicit_mesh_remove_point(Tri_edge_implicit_mesh *mesh, Point p);
void                    as_tri_edge_implicit_mesh_remove_edge(Tri_edge_implicit_mesh *mesh, Point p1, Point p2);
void                    as_tri_edge_implicit_mesh_remove_triangle(Tri_edge_implicit_mesh *mesh, size_t tri_index);
Tri_mesh                as_tri_edge_implicit_mesh_to_tri_mesh(Tri_edge_implicit_mesh tei_mesh, float light_intensity, uint32_t color);
Tri_implicit_mesh       as_tri_edge_implicit_mesh_to_tri_implicit_mesh(Tri_edge_implicit_mesh tei_mesh);

/* Delaunay Mesh Generation utils functions */

void                    as_points_array_convex_hull_Jarvis_march(Curve *conv, Point *points, const size_t len);
void                    as_points_array_swap_points(Point *c, const size_t index1, const size_t index2);
void                    as_points_array_order_lexicographically(Point *c, const size_t len);
Tri_implicit_mesh       as_points_array_get_lexicographic_triangulation(Point *points, const size_t len);
void                    as_tri_get_circumcircle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r);
void                    as_tri_get_in_circle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r);
void                    as_tri_get_min_containment_circle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r);
bool                    as_tri_implicit_mesh_check_Delaunay(Tri_implicit_mesh mesh);
int                     as_tri_implicit_mesh_check_edge_is_locally_Delaunay(Tri_implicit_mesh mesh, Point p1, Point p2);
void                    as_tri_implicit_mesh_flip_edge(Tri_implicit_mesh mesh, Point p1, Point p2);
int                     as_tri_implicit_mesh_get_triangles_with_edge(Tri_implicit_mesh mesh, Point p1, Point p2, Tri_implicit *tri_out1, Tri_implicit *tri_out2);
int                     as_tri_implicit_mesh_get_triangles_indexs_with_edge(Tri_implicit_mesh mesh, Point p1, Point p2, size_t*tri_out1_index, size_t*tri_out2_index);
void                    as_tri_implicit_mesh_set_Delaunay_triangulation_flip_algorithm(Tri_implicit_mesh mesh);

/* circle operations functions */

Curve                   as_circle_curve_create(const Point center, const float r, const size_t num_of_points, const uint32_t color, const char plane[]);
Tri_mesh                as_circle_tri_mesh_create_simple(const Point center, const float r, const size_t num_of_points, const uint32_t color, float light_intensity, const char plane[]);
Tri_mesh                as_circle_tri_mesh_create_lexicographically(const Point center, const float r, const size_t num_of_points, const uint32_t color, float light_intensity, const char plane[]);

/* cube operations functions */

Tri_mesh                as_cube_create_tri_mesh_simple(const size_t len, const uint32_t color);

/* sphere operations functions */

Curve_ada               as_sphere_curve_ada_create(const Point center, const float r, const size_t num_of_points_horizontal, const size_t num_of_points_vertical, const uint32_t color);
Tri_mesh                as_sphere_tri_mesh_create_simple(const Point center, const float r, const size_t num_of_points_horizontal, const size_t num_of_points_vertical, const uint32_t color, float light_intensity);

#endif /*ALMOG_SHAPES_H_*/

#ifdef ALMOG_SHAPES_IMPLEMENTATION
#undef ALMOG_SHAPES_IMPLEMENTATION

#define                 AS_POINT_PRINT(c) as_point_print(c, #c, 0)
#define                 AS_CURVE_PRINT(c) as_curve_print(c, #c, 0)
#define                 AS_TRI_MESH_PRINT(tm) as_tri_mesh_print(tm, #tm, 0)
#define                 AS_TRI_IMPLICIT_PRINT(t, points) as_tri_implicit_print(t, points, #t, 0)
#define                 AS_TRI_IMPLICIT_MESH_PRINT(tm) as_tri_implicit_mesh_print(tm, #tm, 0)
#define                 AS_TRI_EDGE_IMPLICIT_MESH_PRINT(tm) as_tri_edge_implicit_mesh_print(tm, #tm, 0)

/** Free a Curve_ada by freeing each Curve’s points and the curves array. */
void as_curve_ada_free(Curve_ada curves)
{
    for (size_t i = 0; i < curves.length; i++) {
        free(curves.elements[i].elements);
    }
    free(curves.elements);
}

/**
 * Initialize and return an empty Tri_implicit_mesh.
 * Allocates dynamic arrays for points and triangles (length=0).
 * Must be freed with as_tri_implicit_mesh_free.
 */
Tri_implicit_mesh as_tri_implicit_mesh_init(void)
{
    Tri_implicit_mesh mesh = {0};

    ada_init_array(Point, mesh.points);
    ada_init_array(Tri_implicit, mesh.triangles);

    return mesh;
}

/**
 * Free internal allocations of a Tri_implicit_mesh (points, triangles).
 * The mesh itself is passed by value; call after done using the mesh.
 */
void as_tri_implicit_mesh_free(Tri_implicit_mesh mesh)
{
    free(mesh.points.elements);
    free(mesh.triangles.elements);
}

/**
 * Initialize and return an empty Tri_edge_implicit_mesh.
 * Allocates dynamic arrays for points, edges and triangles (length=0).
 * Must be freed with as_tri_edge_implicit_mesh_free.
 */
Tri_edge_implicit_mesh as_tri_edge_implicit_mesh_init(void)
{
    Tri_edge_implicit_mesh mesh = {0};

    ada_init_array(Point, mesh.points);
    ada_init_array(Edge_implicit, mesh.edges);
    ada_init_array(Tri_edge_implicit, mesh.triangles);

    return mesh;
}

/**
 * Free internal allocations of a Tri_edge_implicit_mesh (points, edges, triangles).
 * The mesh itself is passed by value; call after done using the mesh.
 */
void as_tri_edge_implicit_mesh_free(Tri_edge_implicit_mesh mesh)
{
    free(mesh.points.elements);
    free(mesh.edges.elements);
    free(mesh.triangles.elements);
}

/** Print a Point with a label and optional left padding. */
void as_point_print(Point p, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);
    printf("%*s    (%f, %f, %f)\n", (int) padding, "", p.x, p.y, p.z);
}

/** Print a Curve (list of points and color) with a label and padding. */
void as_curve_print(Curve c, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);
    for (size_t i = 0; i < c.length; i++) {
        printf("%*s    point%zu: (%f, %f, %f)\n", (int) padding, "", i, c.elements[i].x, c.elements[i].y, c.elements[i].z);
    }
    printf("%*s    color: %X\n", (int)padding, "", c.color);
}

/** Print a Tri (points, normals, colors, light, draw flag) with a label and padding. */
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

/** Print a Tri_mesh by iterating and printing each Tri. */
void as_tri_mesh_print(Tri_mesh mesh, char *name, size_t padding)
{
    char tri_name[256];
    printf("%*s%s:\n", (int) padding, "", name);
    for (size_t i = 0; i < mesh.length; i++) {
        snprintf(tri_name, 256, "tri %zu", i);
        as_tri_print(mesh.elements[i], tri_name, 4);
    }
}

/** Print a Tri_implicit (resolved through points array) with a label and padding. */
void as_tri_implicit_print(Tri_implicit tri_imp, Point *points, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);
    printf("%*s    points:\n", (int)padding, "");
    printf("%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n", (int) padding, "", points[tri_imp.points_index[0]].x, points[tri_imp.points_index[0]].y, points[tri_imp.points_index[0]].z, (int) padding, "", points[tri_imp.points_index[1]].x, points[tri_imp.points_index[1]].y, points[tri_imp.points_index[1]].z, (int) padding, "", points[tri_imp.points_index[2]].x, points[tri_imp.points_index[2]].y, points[tri_imp.points_index[2]].z);
}

/** Print an entire Tri_implicit_mesh (points and triangles). */
void as_tri_implicit_mesh_print(Tri_implicit_mesh mesh, char *name, size_t padding)
{
    char tri_name[256];
    printf("%*s%s:\n", (int) padding, "", name);
    as_curve_print(mesh.points, "points", 4);

    printf("%*s------------------------------\n", (int) padding+4, "");

    for (size_t i = 0; i < mesh.triangles.length; i++) {
        Tri_implicit tri_imp = mesh.triangles.elements[i];

        snprintf(tri_name, 256, "tri %zu", i);
        printf("%*s%s:\n", (int) padding+4, "", tri_name);
        printf("%*s    points:\n", (int)padding+4, "");
        printf("%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n", (int) padding+4, "", mesh.points.elements[tri_imp.points_index[0]].x, mesh.points.elements[tri_imp.points_index[0]].y, mesh.points.elements[tri_imp.points_index[0]].z, (int) padding+4, "", mesh.points.elements[tri_imp.points_index[1]].x, mesh.points.elements[tri_imp.points_index[1]].y, mesh.points.elements[tri_imp.points_index[1]].z, (int) padding+4, "", mesh.points.elements[tri_imp.points_index[2]].x, mesh.points.elements[tri_imp.points_index[2]].y, mesh.points.elements[tri_imp.points_index[2]].z);
    }
}

/** Print an entire Tri_edge_implicit_mesh (points, edges, and triangles). */
void as_tri_edge_implicit_mesh_print(Tri_edge_implicit_mesh mesh, char *name, size_t padding)
{
    char temp_str[256];
    printf("%*s%s:\n", (int) padding, "", name);
    as_curve_print(mesh.points, "points", 4);

    printf("%*s------------------------------\n", (int) padding+4, "");

    for (size_t i = 0; i < mesh.edges.length; i++) {
        snprintf(temp_str, 256, "edge %zu", i);
        printf("%*s%s:\n", (int) padding+4, "", temp_str);
        printf("%*s    (%f, %f, %f)\n", (int) padding+4, "", as_point_expand_to_xyz(mesh.points.elements[mesh.edges.elements[i].p1_index]));
        printf("%*s    (%f, %f, %f)\n", (int) padding+4, "", as_point_expand_to_xyz(mesh.points.elements[mesh.edges.elements[i].p2_index]));
    }

    printf("%*s------------------------------\n", (int) padding+4, "");

    for (size_t i = 0; i < mesh.triangles.length; i++) {
        snprintf(temp_str, 256, "tri %zu", i);
        printf("%*s%s:\n", (int) padding+4, "", temp_str);
        printf("%*s    points:\n", (int)padding+4, "");
        for (size_t j = 0; j < 3; j++) {
            printf("%*s    (%f, %f, %f)\n", (int) padding+4, "", as_tri_edge_implicit_mesh_expand_point_of_tri_to_xyz(mesh, i, j));
        }
    }
}

/**
 * Linearly interpolate between two ARGB colors with factor t in [0,1].
 * t=1 yields c1, t=0 yields c2. Returns ARGB packed uint32_t.
 */
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

/**
 * Create a Curve of uniformly random points in the given AABB [min,max] for x,y,z.
 * Uses the provided seed to seed rand(). Returns Curve with points and default color=0.
 */
Curve as_curve_create_random_points(size_t num_of_points, float min_x, float max_x, float min_y, float max_y, float min_z, float max_z, unsigned int seed)
{
    Curve c = {0};
    ada_init_array(Point, c);

    srand(seed);

    for (size_t i = 0; i < num_of_points; i++) {
        Point p = {0};
        p.x = as_rand_float()*(max_x - min_x) + min_x;
        p.y = as_rand_float()*(max_y - min_y) + min_y;
        p.z = as_rand_float()*(max_z - min_z) + min_z;

        ada_appand(Point, c, p);
    }

    return c;
}

/**
 * Return index of edge (p1->p2) in an Edge_implicit_ada or -1 if not present.
 * Points are matched by exact equality on coordinates.
 */int as_edge_implicit_ada_get_edge_index(Edge_implicit_ada eia, Point *points, Point p1, Point p2)
{
    int res = -1;

    for (size_t edge_index = 0; edge_index < eia.length; edge_index++) {
        bool first_index_match = as_points_equal(points[eia.elements[edge_index].p1_index], p1);
        bool second_index_match = as_points_equal(points[eia.elements[edge_index].p2_index], p2);
        if (first_index_match && second_index_match) {
            res = edge_index;
            return res;
        }
    }

    return res;
}

/** Convert a 3x1 or 1x3 Mat2D into a Point (x,y,z,w=1). */
Point as_mat2D_to_point(Mat2D m)
{
    Point res = {.x = MAT2D_AT(m, 0, 0), .y = MAT2D_AT(m, 1, 0), .z = MAT2D_AT(m, 2, 0), .w = 1};
    return res;
}

/** Store Point (x,y,z) into a 3x1 or 1x3 Mat2D; asserts valid shape. */
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

/** Count number of occurrences of a Point in a Curve (exact comparisons). */
size_t as_point_in_curve_occurrences(Point p, Curve c)
{
    size_t num_of_occurrences = 0;

    for (size_t i = 0; i < c.length; i++) {
        if (as_points_equal(c.elements[i], p)) num_of_occurrences++;
    }

    return num_of_occurrences;
}

/** Return first index of Point in Curve or -1 if not found (exact comparisons). */
int as_point_in_curve_index(Point p, Curve c)
{
    int index = -1;

    for (size_t i = 0; i < c.length; i++) {
        if (as_points_equal(c.elements[i], p)) index = i;
    }

    return index;
}

/** Compute Euclidean distance between two Points (uses x,y,z). */
float as_points_distance(Point p1, Point p2)
{
    return sqrtf((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
}
// #define as_points_distance(p1, p2) sqrtf(((p1).x - (p2).x) * ((p1).x - (p2).x) + ((p1).y - (p2).y) * ((p1).y - (p2).y) + ((p1).z - (p2).z) * ((p1).z - (p2).z));

/** Return a random float in [0,1]. */
float as_rand_float(void)
{
    return (float) rand() / (float) RAND_MAX;
}

/**
 * Compute per-vertex normals for a Tri from its geometry.
 * Uses cross products of adjacent edges and normalizes results.
 */
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

/**
 * Convert an explicit Tri_mesh into a Tri_implicit_mesh.
 * Deduplicates identical points and stores triangle point indices.
 */
Tri_implicit_mesh as_tri_mesh_to_tri_implicit_mesh(Tri_mesh mesh)
{
    Tri_implicit_mesh implicit_mesh = as_tri_implicit_mesh_init();

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

/** Copy all triangles from src into des (appending, resetting des.length first). */
void as_tri_mesh_copy(Tri_mesh *des, Tri_mesh src)
{
    Tri_mesh temp_des = *des;
    temp_des.length = 0;
    for (size_t i = 0; i < src.length; i++) {
        ada_appand(Tri, temp_des, src.elements[i]);
    }
    *des = temp_des;
}

/**
 * Subdivide each triangle into 4 by splitting edges at midpoints.
 * Interpolates colors and light intensities; recomputes normals.
 */
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
        as_points_interpolate(p01, p0, p1, 0.5);
        as_points_interpolate(p12, p1, p2, 0.5);
        as_points_interpolate(p20, p2, p0, 0.5);
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

/** If all points in mesh have same x, return that constant; otherwise NAN. */
float as_tri_mesh_const_x(Tri_mesh mesh)
{
    float x_value;

    for (size_t tri_index = 0; tri_index < mesh.length; tri_index++) {
        Tri current_tri = mesh.elements[tri_index];
        if (tri_index == 0) x_value = current_tri.points[0].x;

        for (size_t i = 0; i < 3; i++) {
            if (x_value != current_tri.points[i].x) return NAN;
        }
    }

    return x_value;
}

/** If all points in mesh have same y, return that constant; otherwise NAN. */
float as_tri_mesh_const_y(Tri_mesh mesh)
{
    float y_value;

    for (size_t tri_index = 0; tri_index < mesh.length; tri_index++) {
        Tri current_tri = mesh.elements[tri_index];
        if (tri_index == 0) y_value = current_tri.points[0].y;

        for (size_t i = 0; i < 3; i++) {
            if (y_value != current_tri.points[i].y) return NAN;
        }
    }

    return y_value;
}

/** If all points in mesh have same z, return that constant; otherwise NAN. */
float as_tri_mesh_const_z(Tri_mesh mesh)
{
    float z_value;

    for (size_t tri_index = 0; tri_index < mesh.length; tri_index++) {
        Tri current_tri = mesh.elements[tri_index];
        if (tri_index == 0) z_value = current_tri.points[0].z;

        for (size_t i = 0; i < 3; i++) {
            if (z_value != current_tri.points[i].z) return NAN;
        }
    }

    return z_value;
}

/** Convert a Tri_implicit and its points array into an explicit Tri (and set normals). */
Tri as_tri_implicit_to_tri(Tri_implicit tri_implicit, Point *points)
{
    Tri tri = {0};

    tri.points[0] = points[tri_implicit.points_index[0]];
    tri.points[1] = points[tri_implicit.points_index[1]];
    tri.points[2] = points[tri_implicit.points_index[2]];

    as_tri_set_normals(&tri);

    return tri;
}

/** If all points in Tri_implicit_mesh have same x, return that constant; else NAN. */
float as_tri_implicit_mesh_const_x(Tri_implicit_mesh mesh)
{
    float value;

    for (size_t p_index = 0; p_index < mesh.points.length; p_index++) {
        if (p_index == 0) value = mesh.points.elements[p_index].x;

        if (value != mesh.points.elements[p_index].x) return NAN;
    }

    return value;
}

/** If all points in Tri_implicit_mesh have same y, return that constant; else NAN. */
float as_tri_implicit_mesh_const_y(Tri_implicit_mesh mesh)
{
    float value;

    for (size_t p_index = 0; p_index < mesh.points.length; p_index++) {
        if (p_index == 0) value = mesh.points.elements[p_index].y;

        if (value != mesh.points.elements[p_index].y) return NAN;
    }

    return value;
}
/** If all points in Tri_implicit_mesh have same z, return that constant; else NAN. */
float as_tri_implicit_mesh_const_z(Tri_implicit_mesh mesh)
{
    float value;

    for (size_t p_index = 0; p_index < mesh.points.length; p_index++) {
        if (p_index == 0) value = mesh.points.elements[p_index].z;

        if (value != mesh.points.elements[p_index].z) return NAN;
    }

    return value;
}

/**
 * Convert Tri_implicit_mesh to explicit Tri_mesh with uniform light/color.
 * Copies geometry, assigns color/light per vertex, and sets normals.
 */
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

/**
 * Convert Tri_implicit_mesh into Tri_edge_implicit_mesh by creating edge list
 * and referencing triangles by edge indices.
 */
Tri_edge_implicit_mesh as_tri_implicit_mesh_to_tri_edge_implicit_mesh(Tri_implicit_mesh ti_mesh)
{
    Tri_edge_implicit_mesh tei_mesh = as_tri_edge_implicit_mesh_init();

    /* collecting all the points */
    for (size_t i = 0; i < ti_mesh.points.length; i++) {
        ada_appand(Point, tei_mesh.points, ti_mesh.points.elements[i]);
    }

    /* collecting triangles and edges */
    for (size_t tri_index = 0; tri_index < ti_mesh.triangles.length; tri_index++) {
        Tri_edge_implicit tei = {0};
        /* collecting edges of current tri implicit */
        for (size_t current_p_index = 0; current_p_index < 3; current_p_index++) {
            size_t next_p_index = (current_p_index + 1) % 3;
            Point current_p = as_tri_implicit_mesh_get_point_of_tri_implicit(ti_mesh, tri_index, current_p_index);
            Point next_p = as_tri_implicit_mesh_get_point_of_tri_implicit(ti_mesh, tri_index, next_p_index);
            int ei_index = as_edge_implicit_ada_get_edge_index(tei_mesh.edges, tei_mesh.points.elements, current_p, next_p);
            if (ei_index != -1) {
                tei.edges_index[current_p_index] = ei_index;
            } else {
                Edge_implicit ei = {0};
                ei.p1_index = as_point_in_curve_index(current_p, tei_mesh.points);
                ei.p2_index = as_point_in_curve_index(next_p, tei_mesh.points);
                ei.is_segment = false;
                ada_appand(Edge_implicit, tei_mesh.edges, ei);
                tei.edges_index[current_p_index] = tei_mesh.edges.length-1;
                // tei.edge_index[current_p_index] = eas_edge_implicit_ada_get_edge_index(tei_mesh.edges, tei_mesh.points.elements, current_p, next_p);
            }
        }
        ada_appand(Tri_edge_implicit, tei_mesh.triangles, tei);
    }

    return tei_mesh;
}

/**
 * Get up to two Tri_edge_implicit that share the directed/undirected edge (p1,p2).
 * Returns number of triangles found (0..2). Outputs via tri_out1/tri_out2.
 */
int as_tri_edge_implicit_mesh_get_triangles_with_edge(Tri_edge_implicit_mesh mesh, Point p1, Point p2, Tri_edge_implicit *tri_out1, Tri_edge_implicit *tri_out2)
{
    int p1_index = as_point_in_curve_index(p1, mesh.points);
    int p2_index = as_point_in_curve_index(p2, mesh.points);

    if (p1_index == -1 || p2_index == -1) {
        return 0;
    }

    int num_of_tri_out = 0;

    for (size_t tri_index = 0; tri_index < mesh.triangles.length; tri_index++) {
        Tri_edge_implicit current_tri = mesh.triangles.elements[tri_index];
        bool p1_is_part_of_tri = false;
        bool p2_is_part_of_tri = false;
        /* check if p1 is part of tri */
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p1, as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri_index, i))) p1_is_part_of_tri = true;
        }
        /* check if p2 is part of tri */
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p2, as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri_index, i))) p2_is_part_of_tri = true;
        }

        if (p1_is_part_of_tri && p2_is_part_of_tri) {
            /* tri has both points*/
            if (num_of_tri_out == 0) {
                *tri_out1 = current_tri;
                num_of_tri_out++;
            } else if (num_of_tri_out == 1) {
                *tri_out2 = current_tri;
                num_of_tri_out++;
            } else if (num_of_tri_out > 2) {
                fprintf(stderr, "%s:%d: [Warning] edge implicit mesh has an edge with more then two triangles\n", __FILE__, __LINE__);
                exit(1);
            }
        }
    }

    return num_of_tri_out;
}

/**
 * Get indices of up to two triangles in Tri_edge_implicit_mesh that share edge (p1,p2).
 * Returns number of triangles found (0..2). Outputs indices via tri_out1_index/tri_out2_index.
 */
int as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge(Tri_edge_implicit_mesh mesh, Point p1, Point p2, size_t *tri_out1_index, size_t *tri_out2_index)
{
    int p1_index = as_point_in_curve_index(p1, mesh.points);
    int p2_index = as_point_in_curve_index(p2, mesh.points);

    if (p1_index == -1 || p2_index == -1) {
        return 0;
    }

    int num_of_tri_out = 0;

    for (size_t tri_index = 0; tri_index < mesh.triangles.length; tri_index++) {
        bool p1_is_part_of_tri = false;
        bool p2_is_part_of_tri = false;
        /* check if p1 is part of tri */
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p1, as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri_index, i))) p1_is_part_of_tri = true;
        }
        /* check if p2 is part of tri */
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p2, as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri_index, i))) p2_is_part_of_tri = true;
        }

        if (p1_is_part_of_tri && p2_is_part_of_tri) {
            /* tri has both points*/
            if (num_of_tri_out == 0) {
                *tri_out1_index = tri_index;
                num_of_tri_out++;
            } else if (num_of_tri_out == 1) {
                *tri_out2_index = tri_index;
                num_of_tri_out++;
            } else if (num_of_tri_out > 2) {
                fprintf(stderr, "%s:%d: [Warning] edge implicit mesh has an edge with more then two triangles\n", __FILE__, __LINE__);
                exit(1);
            }
        }
    }

    return num_of_tri_out;
}

/** Remove all triangles that include point p from a Tri_edge_implicit_mesh. */
void as_tri_edge_implicit_mesh_remove_point(Tri_edge_implicit_mesh *mesh, Point p)
{
    Tri_edge_implicit_mesh temp_mesh = *mesh;

    for (int tri_index = 0; tri_index < (int)temp_mesh.triangles.length; tri_index++) {
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p, as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri_index, i))) {
                ada_remove(Tri_edge_implicit, temp_mesh.triangles, tri_index);
                tri_index--;
                break;
            }
        }
    }

    *mesh = temp_mesh;
}

/** Remove all triangles that include edge (p1,p2) from a Tri_edge_implicit_mesh. */
void as_tri_edge_implicit_mesh_remove_edge(Tri_edge_implicit_mesh *mesh, Point p1, Point p2)
{
    Tri_edge_implicit_mesh temp_mesh = *mesh;

    int edge_index = as_edge_implicit_ada_get_edge_index(mesh->edges, mesh->points.elements, p1, p2);
    int inv_edge_index = as_edge_implicit_ada_get_edge_index(mesh->edges, mesh->points.elements, p2, p1);

    size_t tri_out1_index = {0};
    size_t tri_out2_index = {0};
    int num_of_tri = as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge(temp_mesh, p1, p2, &tri_out1_index, &tri_out2_index);

    if (num_of_tri == 1) {
        as_tri_edge_implicit_mesh_remove_triangle(&temp_mesh, tri_out1_index);
    } else if (num_of_tri == 2) {
        as_tri_edge_implicit_mesh_remove_triangle(&temp_mesh, tri_out1_index);
        num_of_tri = as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge(temp_mesh, p1, p2, &tri_out1_index, &tri_out2_index);
        if (num_of_tri != 1) {
            fprintf(stderr, "%s:%d: [Warning] edge implicit mesh has an edge with more then two triangles\n", __FILE__, __LINE__);
            exit(1);
        }
        as_tri_edge_implicit_mesh_remove_triangle(&temp_mesh, tri_out1_index);
    }

    // if (edge_index != -1) ada_remove(Edge_implicit, temp_mesh.edges, edge_index);
    // if (inv_edge_index != -1) ada_remove(Edge_implicit, temp_mesh.edges, inv_edge_index);

    (void)edge_index;
    (void)inv_edge_index;
    (void)tri_out2_index;

    *mesh = temp_mesh;
}

/** Remove triangle at index tri_index from a Tri_edge_implicit_mesh. */
void as_tri_edge_implicit_mesh_remove_triangle(Tri_edge_implicit_mesh *mesh, size_t tri_index)
{
    Tri_edge_implicit_mesh temp_mesh = *mesh;

    ada_remove(Tri_edge_implicit, temp_mesh.triangles, tri_index);

    *mesh = temp_mesh;
}

/** Convert a Tri_edge_implicit_mesh into a Tri_mesh. */
Tri_mesh as_tri_edge_implicit_mesh_to_tri_mesh(Tri_edge_implicit_mesh tei_mesh, float light_intensity, uint32_t color)
{
    Tri_mesh mesh = {0};
    ada_init_array(Tri, mesh);

    for (size_t tri_index = 0; tri_index < tei_mesh.triangles.length; tri_index++) {
        Tri tri = {0};
        tri.to_draw = 1;
        for (size_t i = 0; i < 3; i++) {
            tri.colors[i] = color;
            tri.light_intensity[i] = light_intensity;
            Point p = (Point){as_tri_edge_implicit_mesh_expand_point_of_tri_to_xyz(tei_mesh, tri_index, i), 0};
            tri.points[i] = p;
        }
        as_tri_set_normals(&tri);
        ada_appand(Tri, mesh, tri);
    }

    return mesh;
}

/** Convert a Tri_edge_implicit_mesh into a Tri_implicit_mesh. */
Tri_implicit_mesh as_tri_edge_implicit_mesh_to_tri_implicit_mesh(Tri_edge_implicit_mesh tei_mesh)
{
    Tri_implicit_mesh ti_mesh = as_tri_implicit_mesh_init();

    /* collecting all the points */
    for (size_t i = 0; i < tei_mesh.points.length; i++) {
        ada_appand(Point, ti_mesh.points, tei_mesh.points.elements[i]);
    }

    /* collecting triangles */
    for (size_t tri_index = 0; tri_index < tei_mesh.triangles.length; tri_index++) {
        Tri_implicit ti = {0};
        for (size_t i = 0; i < 3; i++) {
            Point p = (Point){as_tri_edge_implicit_mesh_expand_point_of_tri_to_xyz(tei_mesh, tri_index, i), 0};
            ti.points_index[i] = as_point_in_curve_index(p, ti_mesh.points);
        }
        ada_appand(Tri_implicit, ti_mesh.triangles, ti);
    }

    return ti_mesh;
}

/**
 * Compute the convex hull of a set of coplanar points (XY plane) using Jarvis march.
 * Includes collinear boundary points. Writes result curve into conv.
 * Expected lexicographically order.
 */
void as_points_array_convex_hull_Jarvis_march(Curve *conv, Point *points, const size_t len)
{
    /* https://youtu.be/nBvCZi34F_o. */
    /* I used AI to add collinear points on the convex hull. */
    /* make sure points have the same z value */
    float z_value = points[0].z;
    for (size_t i = 1; i < len; i++) {
        AS_ASSERT(points[i].z == z_value);
    }

    // for (size_t i = 0; i < len; i++) {
    //     AS_POINT_PRINT(points[i]);
    // }
    
    Curve temp_c = *conv;
    temp_c.length = 0;

    /* first point on the convex hull is the left most point and going acw */
    ada_appand(Point, temp_c, points[0]);

    if (len == 1) {
        *conv = temp_c;
        return;
    }

    float eps = 1e-7;

    /* guess next point on convex hull*/
    for (size_t i = 0; i < len; i++) {
        size_t current_guess = 0;
        for (size_t next_guess = 0; next_guess < len; next_guess++) {
            if (next_guess == current_guess) continue;
            float cross = as_tri_area_xy(temp_c.elements[temp_c.length-1], points[next_guess], points[current_guess]);
            float d_next = as_points_distance(temp_c.elements[temp_c.length-1], points[next_guess]);
            float d_curr = as_points_distance(temp_c.elements[temp_c.length-1], points[current_guess]);
            if (cross > eps) {
                current_guess = next_guess;
            } else if (fabsf(cross) <= eps) {
                /* Colinear: prefer farther point as the pivot */
                if (d_next > d_curr) current_guess = next_guess;
            }
        }

        /* Insert all intermediate colinear boundary points between last and pivot */
        Point last = temp_c.elements[temp_c.length - 1];
        Point pivot = points[current_guess];
        float seg_dx = pivot.x - last.x;
        float seg_dy = pivot.y - last.y;
        float seg_len2 = seg_dx*seg_dx + seg_dy*seg_dy;

        /* Simple insertion sort by distance (few points; avoid temp allocations) */
        for (size_t k = 0; k < len; k++) {
            if (k == current_guess) continue;
            Point cand = points[k];
            /* Colinear with segment last->pivot? */
            float cross_lp = as_tri_area_xy(last, cand, pivot);
            if (fabsf(cross_lp) > eps) continue;
            /* Between last and pivot (projection in [0,1]) */
            float t = (seg_len2 > 0) ? ((cand.x - last.x)*seg_dx + (cand.y - last.y)*seg_dy) / seg_len2 : 0.0f;
            if (t < eps || t > 1.0f - eps) continue;
            if (as_point_in_curve_occurrences(cand, temp_c)) continue;
            /* Insert maintaining increasing distance from last */
            float d_cand = as_points_distance(last, cand);
            size_t insert_at = temp_c.length;
            while (insert_at > 0) {
                Point prev = temp_c.elements[insert_at - 1];
                if (!as_points_equal(prev, last) &&
                    as_tri_area_xy(last, prev, pivot) == 0.0f &&
                    as_points_distance(last, prev) > d_cand) {
                    insert_at--;
                } else {
                    break;
                }
            }
            /* Append and then swap backwards to position insert_at */
            ada_appand(Point, temp_c, cand);
            for (size_t s = temp_c.length - 1; s > insert_at; s--) {
                Point tmp = temp_c.elements[s];
                temp_c.elements[s] = temp_c.elements[s - 1];
                temp_c.elements[s - 1] = tmp;
            }
        }
        /* If we wrapped back, stop */
        if (as_points_equal(temp_c.elements[0], points[current_guess])) break;

        /* Finally add the pivot if not already present */
        if (!as_point_in_curve_occurrences(pivot, temp_c)) {
            ada_appand(Point, temp_c, pivot);
        }
    }

    *conv = temp_c;
}

/** Swap two points in an array by indices. */
void as_points_array_swap_points(Point *c, const size_t index1, const size_t index2)
{
    Point temp = c[index1];
    c[index1] = c[index2];
    c[index2] = temp;
}

/**
 * Sort an array of points lexicographically by x, then y, then z (ascending).
 * Uses a simple bubble sort; stable within epsilon for comparisons.
 */
void as_points_array_order_lexicographically(Point *c, const size_t len)
{
    float epsilon = 1e-5;
    for (size_t i = 0; i < len-1; i++) {
        for (size_t j = i+1; j < len; j++) {
            if (c[i].x > c[j].x) {
                as_points_array_swap_points(c, i, j);
            } else if (fabsf(c[i].x - c[j].x) < epsilon) {
                if (c[i].y > c[j].y) {
                    as_points_array_swap_points(c, i, j);
                } else if (fabsf(c[i].y - c[j].y) < epsilon) {
                    if (c[i].z > c[j].z) {
                        as_points_array_swap_points(c, i, j);
                    }
                }
            }
        }
    }
}

/**
 * Create a lexicographic triangulation (incremental) on XY-coplanar points.
 * Returns a Tri_implicit_mesh with deduplicated points and implicit triangles.
 */
Tri_implicit_mesh as_points_array_get_lexicographic_triangulation(Point *points, const size_t len)
{
    as_points_array_order_lexicographically(points, len);

    Tri_implicit_mesh implicit_mesh = as_tri_implicit_mesh_init();
    Curve convex_hull = {0};
    ada_init_array(Point, convex_hull);

    /* make sure points have the same z value */
    float z_value = points[0].z;
    for (size_t i = 1; i < len; i++) {
        AS_ASSERT(points[i].z == z_value);
    }

    ada_appand(Point, implicit_mesh.points, points[0]);
    ada_appand(Point, implicit_mesh.points, points[1]);

    /* find first non-degenerate triangle */
    Tri_implicit first_implicit_tri = {0};
    size_t point_index;
    for (point_index = 2; point_index < len; point_index++) {
        Point p1 = points[point_index-2];
        Point p2 = points[point_index-1];
        Point p3 = points[point_index];
        ada_appand(Point, implicit_mesh.points, points[point_index]);
        if (as_tri_area_xy(p1, p2, p3)) {
            if (as_tri_area_xy(p1, p2, p3) < 0) {
                for (int i = point_index; i >= 2; i--) {
                    first_implicit_tri.points_index[0] = i-2;
                    first_implicit_tri.points_index[1] = i-1;
                    first_implicit_tri.points_index[2] = point_index;
                    ada_appand(Tri_implicit, implicit_mesh.triangles, first_implicit_tri);
                }
            } else {
                for (int i = point_index; i >= 2; i--) {
                    first_implicit_tri.points_index[2] = i-2;
                    first_implicit_tri.points_index[1] = i-1;
                    first_implicit_tri.points_index[0] = point_index;
                    ada_appand(Tri_implicit, implicit_mesh.triangles, first_implicit_tri);
                }
            }
            break;
        }
    }
    /* WARNING: there might be a need to create triangles for all points that where on the same line at the start. Like in 'Delaunay Mesh Generation' Pg.32 */

    /* adding more points */
    for (point_index = point_index+1; point_index < len; point_index++) {
        as_points_array_convex_hull_Jarvis_march(&convex_hull, implicit_mesh.points.elements, implicit_mesh.points.length);

        Point current_point = points[point_index];
        ada_appand(Point, implicit_mesh.points, current_point);

        for (size_t conv_index = 0; conv_index < convex_hull.length; conv_index++) {
            size_t conv_index_p1 = (conv_index + 1) % convex_hull.length;
            float cross = as_tri_area_xy(convex_hull.elements[conv_index], current_point, convex_hull.elements[conv_index_p1]);
            if (cross > 0) {
                Tri_implicit temp_implicit_tri = {0};
                temp_implicit_tri.points_index[2] = as_point_in_curve_index(convex_hull.elements[conv_index], implicit_mesh.points);
                temp_implicit_tri.points_index[1] = as_point_in_curve_index(current_point, implicit_mesh.points);
                temp_implicit_tri.points_index[0] = as_point_in_curve_index(convex_hull.elements[conv_index_p1], implicit_mesh.points);
                
                ada_appand(Tri_implicit, implicit_mesh.triangles, temp_implicit_tri);
            }
        }
    }

    return implicit_mesh;
}

/**
 * Compute circumcircle of triangle (p1,p2,p3) in XY plane.
 * Outputs center and radius r. Asserts coplanarity and plane="XY".
 */
void as_tri_get_circumcircle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r)
{
    AS_ASSERT((!strncmp(plane, "XY", 3) || !strncmp(plane, "xy", 3)) && "other planes are no implemented.");
    Tri temp_tri = {.points = {p1, p2, p3}};
    AS_ASSERT(as_tri_equal_z(temp_tri));

    /* wiki: https://en.wikipedia.org/wiki/Circumcircle */
    /* https://youtu.be/uIBGSztyB04 */
    float line1_a = (p2.y - p1.y);
    float line1_b = (p1.x - p2.x);
    // float line1_c = line1_a * p1.x + line1_b * p1.y;
    Point line1_mid = {0};
    as_points_interpolate(line1_mid, p1, p2, 0.5);

    float line2_a = (p3.y - p2.y);
    float line2_b = (p2.x - p3.x);
    // float line2_c = line2_a * p2.x + line2_b * p2.y;
    Point line2_mid = {0};
    as_points_interpolate(line2_mid, p2, p3, 0.5);

    float line1_per_a = -line1_b;
    float line1_per_b = line1_a;
    float line1_per_c = line1_per_a * line1_mid.x + line1_per_b * line1_mid.y;

    float line2_per_a = -line2_b;
    float line2_per_b = line2_a;
    float line2_per_c = line2_per_a * line2_mid.x + line2_per_b * line2_mid.y;

    float det  = line1_per_a * line2_per_b - line2_per_a * line1_per_b;
    float detx = line1_per_c * line2_per_b - line2_per_c * line1_per_b;
    float dety = line1_per_a * line2_per_c - line2_per_a * line1_per_c;

    float x = detx / det;
    float y = dety / det;

    *center = (Point){0,0,0,0};
    (*center).x = x;
    (*center).y = y;
    (*center).z = p1.z;

    *r = as_points_distance(p1, *center);
}

/**
 * Compute incircle of triangle (p1,p2,p3) in XY plane.
 * Outputs center and radius r. Asserts coplanarity and plane="XY".
 */
void as_tri_get_in_circle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r)
{
    AS_ASSERT((!strncmp(plane, "XY", 3) || !strncmp(plane, "xy", 3)) && "other planes are no implemented.");
    Tri temp_tri = {.points = {p1, p2, p3}};
    AS_ASSERT(as_tri_equal_z(temp_tri));

    /* wiki: https://en.wikipedia.org/wiki/Incircle_and_excircles */
    float a = as_points_distance(p2, p3);
    float b = as_points_distance(p3, p1);
    float c = as_points_distance(p1, p2);

    (*center)   = (Point){0,0,0,0};
    (*center).x = (a * p1.x + b * p2.x + c * p3.x) / (a + b + c);
    (*center).y = (a * p1.y + b * p2.y + c * p3.y) / (a + b + c);
    (*center).z = p1.z;
    float s = 0.5 * (a + b + c);
    *r = sqrt((s - a) * (s - b) * (s - c) / s);
}

/**
 * Compute minimal containment circle for triangle (p1,p2,p3) in XY plane.
 * Outputs center and radius r (either circumcircle or diameter of longest side).
 */
void as_tri_get_min_containment_circle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r)
{
    AS_ASSERT((!strncmp(plane, "XY", 3) || !strncmp(plane, "xy", 3)) && "other planes are no implemented.");
    Tri temp_tri = {.points = {p1, p2, p3}};
    AS_ASSERT(as_tri_equal_z(temp_tri));

    float d1 = as_points_distance(p1, p2);
    float d2 = as_points_distance(p2, p3);
    float d3 = as_points_distance(p3, p1);
    Point line12 = {0};
    Point line13 = {0};
    Point line21 = {0};
    Point line23 = {0};
    Point line31 = {0};
    Point line32 = {0};
    as_point_sub_point(line12, p2, p1);
    as_point_sub_point(line13, p3, p1);
    as_point_sub_point(line21, p1, p2);
    as_point_sub_point(line23, p3, p2);
    as_point_sub_point(line31, p1, p3);
    as_point_sub_point(line32, p2, p3);

    if (d1 >= fmaxf(d2, d3)) {
        if (as_point_dot_point(line31, line32) >= 0) {
            as_tri_get_circumcircle(p1, p2, p3, plane, center, r);
        } else {
            Point mid = {0};
            as_points_interpolate(mid, p1, p2, 0.5);
            *center = mid;
            *r = d1 / 2;
        }
    } else if (d2 >= fmaxf(d1, d3)) {
        if (as_point_dot_point(line13, line12) >= 0) {
            as_tri_get_circumcircle(p1, p2, p3, plane, center, r);
        } else {
            Point mid = {0};
            as_points_interpolate(mid, p2, p3, 0.5);
            *center = mid;
            *r = d2 / 2;
        }
    } else if (d3 >= fmaxf(d1, d2)) {
        if (as_point_dot_point(line23, line21) >= 0) {
            as_tri_get_circumcircle(p1, p2, p3, plane, center, r);
        } else {
            Point mid = {0};
            as_points_interpolate(mid, p3, p1, 0.5);
            *center = mid;
            *r = d3 / 2;
        }
    }
}

/**
 * Check if all interior edges in a Tri_implicit_mesh satisfy the Delaunay condition.
 * Returns true if Delaunay; false otherwise.
 */
bool as_tri_implicit_mesh_check_Delaunay(Tri_implicit_mesh mesh)
{
    for (size_t i = 0; i < mesh.points.length-1; i++) {
        for (size_t j = i+1; j < mesh.points.length; j++) {
            int is_locally_Delaunay = as_tri_implicit_mesh_check_edge_is_locally_Delaunay(mesh, mesh.points.elements[i], mesh.points.elements[j]);
            if (is_locally_Delaunay == -1) continue;
            if (is_locally_Delaunay == 1) continue;
            if (is_locally_Delaunay == 0) return false;
        }
    }

    return true;
}

/**
 * Check if edge (p1,p2) is locally Delaunay.
 * Returns: -1 not an edge, 0 not locally Delaunay, 1 locally Delaunay.
 */
int as_tri_implicit_mesh_check_edge_is_locally_Delaunay(Tri_implicit_mesh mesh, Point p1, Point p2)
{
    int p1_index = as_point_in_curve_index(p1, mesh.points);
    int p2_index = as_point_in_curve_index(p2, mesh.points);

    AS_ASSERT(p1_index != -1 && p2_index != -1);
    
    size_t tri1_index = 0;
    size_t tri2_index = 0;

    int num_of_triangles = as_tri_implicit_mesh_get_triangles_indexs_with_edge(mesh, p1, p2, &tri1_index, &tri2_index);
    
    if (num_of_triangles == 0) return -1;
    if (num_of_triangles == 1) return 1;

    ADA_ASSERT(num_of_triangles == 2);

    Point circumcenter_1 = {0};
    float r1 = 0;

    as_tri_get_circumcircle(as_tri_implicit_mesh_expand_tri_to_points(mesh, tri1_index), "xy", &circumcenter_1, &r1);
    // AS_POINT_PRINT(circumcenter_1);
    // dprintD(r1);

    /* fined the point on tri2 that is not on the edge */
    Point tri2_outside_p = {0};
    for (size_t i = 0; i < 3; i++) {
        Point current_point = as_tri_implicit_mesh_get_point_of_tri_implicit(mesh, tri2_index, i);
        if (!as_points_equal(current_point, p1) && !as_points_equal(current_point, p2)) tri2_outside_p = current_point;
    }

    float tri2_out_p_and_center_dis = as_points_distance(tri2_outside_p, circumcenter_1);

    if (tri2_out_p_and_center_dis >= r1) return 1;

    return 0;
}

/**
 * Flip the shared edge (p1,p2) between two adjacent triangles to the alternative diagonal.
 * No-op with warnings if edge is not shared by exactly two triangles.
 */
void as_tri_implicit_mesh_flip_edge(Tri_implicit_mesh mesh, Point p1, Point p2)
{
    int p1_index = as_point_in_curve_index(p1, mesh.points);
    int p2_index = as_point_in_curve_index(p2, mesh.points);

    AS_ASSERT(p1_index != -1 || p2_index != -1);
    
    size_t tri1_index = 0;
    size_t tri2_index = 0;

    int num_of_triangles = as_tri_implicit_mesh_get_triangles_indexs_with_edge(mesh, p1, p2, &tri1_index, &tri2_index);
    
    if (num_of_triangles == 0) {
        fprintf(stderr, "%s:%d: [Warning] one of the points is not in the tri implicit mesh.\n", __FILE__, __LINE__);
        return;
    }
    if (num_of_triangles == 1) {
        fprintf(stderr, "%s:%d: [Warning] this is a locally Delaunay edge.\n", __FILE__, __LINE__);
        return;
    }

    int p1_tri1_index = 0;
    int p2_tri1_index = 0;
    int p3_tri1_index = 0;
    for (size_t i = 0; i < 3; i++) {
        Point current_point = as_tri_implicit_mesh_get_point_of_tri_implicit(mesh, tri1_index, i);
        if (as_points_equal(current_point, p1)) p1_tri1_index = mesh.triangles.elements[tri1_index].points_index[i];
        if (as_points_equal(current_point, p2)) p2_tri1_index = mesh.triangles.elements[tri1_index].points_index[i];
        if (!as_points_equal(current_point, p1) && !as_points_equal(current_point, p2)) p3_tri1_index = mesh.triangles.elements[tri1_index].points_index[i];
    }

    int p1_tri2_index = 0;
    int p2_tri2_index = 0;
    int p3_tri2_index = 0;
    for (size_t i = 0; i < 3; i++) {
        Point current_point = as_tri_implicit_mesh_get_point_of_tri_implicit(mesh, tri2_index, i);
        if (as_points_equal(current_point, p1)) p1_tri2_index = mesh.triangles.elements[tri2_index].points_index[i];
        if (as_points_equal(current_point, p2)) p2_tri2_index = mesh.triangles.elements[tri2_index].points_index[i];
        if (!as_points_equal(current_point, p1) && !as_points_equal(current_point, p2)) p3_tri2_index = mesh.triangles.elements[tri2_index].points_index[i];
    }

    mesh.triangles.elements[tri1_index].points_index[0] = p3_tri2_index;
    mesh.triangles.elements[tri1_index].points_index[1] = p3_tri1_index;
    mesh.triangles.elements[tri1_index].points_index[2] = p1_tri1_index;

    mesh.triangles.elements[tri2_index].points_index[0] = p3_tri1_index;
    mesh.triangles.elements[tri2_index].points_index[1] = p3_tri2_index;
    mesh.triangles.elements[tri2_index].points_index[2] = p2_tri2_index;

    /* fix orientation */
    float cross = as_tri_implicit_area_xy(mesh, tri1_index);
    if (cross > 0) {
        int temp = mesh.triangles.elements[tri1_index].points_index[0];
        mesh.triangles.elements[tri1_index].points_index[0] = mesh.triangles.elements[tri1_index].points_index[2];
        mesh.triangles.elements[tri1_index].points_index[2] = temp;
    }
    cross = as_tri_implicit_area_xy(mesh, tri2_index);
    if (cross > 0) {
        int temp = mesh.triangles.elements[tri2_index].points_index[0];
        mesh.triangles.elements[tri2_index].points_index[0] = mesh.triangles.elements[tri2_index].points_index[2];
        mesh.triangles.elements[tri2_index].points_index[2] = temp;
    }

    // AS_TRI_IMPLICIT_PRINT(as_tri_implicit_mesh_get_tri_implicit(mesh, tri1_index), mesh.points.elements);
    // AS_TRI_IMPLICIT_PRINT(as_tri_implicit_mesh_get_tri_implicit(mesh, tri2_index), mesh.points.elements);

    (void)p2_tri1_index;
    (void)p1_tri2_index;
}

/**
 * Get up to two Tri_implicit that share the edge (p1,p2).
 * Returns number of triangles found (0..2). Triangles returned via tri_out1/tri_out2.
 */
int as_tri_implicit_mesh_get_triangles_with_edge(Tri_implicit_mesh mesh, Point p1, Point p2, Tri_implicit *tri_out1, Tri_implicit *tri_out2)
{
    int p1_index = as_point_in_curve_index(p1, mesh.points);
    int p2_index = as_point_in_curve_index(p2, mesh.points);

    if (p1_index == -1 || p2_index == -1) {
        return 0;
    }

    int num_of_tri_out = 0;

    for (size_t tri_index = 0; tri_index < mesh.triangles.length; tri_index++) {
        Tri_implicit current_tri = mesh.triangles.elements[tri_index];
        bool p1_is_part_of_tri = false;
        bool p2_is_part_of_tri = false;
        /* check if p1 is part of tri */
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p1, mesh.points.elements[current_tri.points_index[i]])) p1_is_part_of_tri = true;
        }
        /* check if p2 is part of tri */
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p2, mesh.points.elements[current_tri.points_index[i]])) p2_is_part_of_tri = true;
        }

        if (p1_is_part_of_tri && p2_is_part_of_tri) {
            /* tri has both points*/
            if (num_of_tri_out == 0) {
                *tri_out1 = current_tri;
                num_of_tri_out++;
            } else if (num_of_tri_out == 1) {
                *tri_out2 = current_tri;
                num_of_tri_out++;
            } else if (num_of_tri_out > 2) {
                fprintf(stderr, "%s:%d: [Warning] implicit mesh has an edge with more then two triangles\n", __FILE__, __LINE__);
                exit(1);
            }
        }
    }

    return num_of_tri_out;
}

/**
 * Get indices of up to two Tri_implicit that share edge (p1,p2).
 * Returns number of triangles found (0..2). Outputs via tri_out1_index/tri_out2_index.
 */
int as_tri_implicit_mesh_get_triangles_indexs_with_edge(Tri_implicit_mesh mesh, Point p1, Point p2, size_t*tri_out1_index, size_t*tri_out2_index)
{
    int p1_index = as_point_in_curve_index(p1, mesh.points);
    int p2_index = as_point_in_curve_index(p2, mesh.points);

    if (p1_index == -1 || p2_index == -1) {
        return 0;
    }

    int num_of_tri_out = 0;

    for (size_t tri_index = 0; tri_index < mesh.triangles.length; tri_index++) {
        Tri_implicit current_tri = mesh.triangles.elements[tri_index];
        bool p1_is_part_of_tri = false;
        bool p2_is_part_of_tri = false;
        /* check if p1 is part of tri */
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p1, mesh.points.elements[current_tri.points_index[i]])) p1_is_part_of_tri = true;
        }
        /* check if p2 is part of tri */
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p2, mesh.points.elements[current_tri.points_index[i]])) p2_is_part_of_tri = true;
        }

        if (p1_is_part_of_tri && p2_is_part_of_tri) {
            /* tri has both points*/
            if (num_of_tri_out == 0) {
                *tri_out1_index = tri_index;
                num_of_tri_out++;
            } else if (num_of_tri_out == 1) {
                *tri_out2_index = tri_index;
                num_of_tri_out++;
            } else if (num_of_tri_out > 2) {
                fprintf(stderr, "%s:%d: [Warning] implicit mesh has an edge with more then two triangles\n", __FILE__, __LINE__);
                exit(1);
            }
        }
    }

    return num_of_tri_out;
}

/**
 * Enforce Delaunay triangulation by iterative edge flipping until stable or limit reached.
 * Potentially high time complexity; uses repeated passes and stops when Delaunay.
 * Might want to implement the way described in the 'Delaunay Mesh Generation' book: Pg.39 */
void as_tri_implicit_mesh_set_Delaunay_triangulation_flip_algorithm(Tri_implicit_mesh mesh)
{
    int hard_limit = 10;
    for (int times = 0; times < hard_limit; times++) {
        for (size_t i = 0; i < mesh.points.length-1; i++) {
            for (size_t j = i+1; j < mesh.points.length; j++) {
                // int tri1_out_index = 0, tri2_out_index;
                // int num_of_tri_on_edge = as_tri_implicit_mesh_get_triangles_indexs_with_edge(mesh, mesh.points.elements[i], mesh.points.elements[j], &tri1_out_index, &tri2_out_index);
                int num_of_tri_on_edge = as_tri_implicit_mesh_check_edge_is_locally_Delaunay(mesh, mesh.points.elements[i], mesh.points.elements[j]);
                if (num_of_tri_on_edge == -1) continue;
                if (num_of_tri_on_edge == 1) continue;
                as_tri_implicit_mesh_flip_edge(mesh, mesh.points.elements[i], mesh.points.elements[j]);
            }
        }
        if (as_tri_implicit_mesh_check_Delaunay(mesh)) break;
    }
}

/**
 * Create a polyline approximation of a circle in the XY plane.
 * Returns Curve with num_of_points points around center with radius r and color.
 */
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

/**
 * Create a triangle fan mesh approximating a filled circle in the XY plane.
 * All vertices get the same color and light_intensity.
 */
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

    for (size_t p_index = 1; p_index < c.length-1; p_index++) {
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

/**
 * Create a triangulated circle using lexicographic triangulation of sampled boundary points.
 * Useful as an alternative to the simple fan; assigns uniform color/light.
 */
Tri_mesh as_circle_tri_mesh_create_lexicographically(const Point center, const float r, const size_t num_of_points, const uint32_t color, float light_intensity, const char plane[])
{
    Curve temp_circle = as_circle_curve_create(center, r, num_of_points, color, plane);
    Tri_implicit_mesh temp_implicit_mesh = as_points_array_get_lexicographic_triangulation(temp_circle.elements, temp_circle.length);

    free(temp_circle.elements);

    Tri_mesh mesh =  as_tri_implicit_mesh_to_tri_mesh(temp_implicit_mesh, light_intensity, color);

    as_tri_implicit_mesh_free(temp_implicit_mesh);

    return mesh;
}

/**
 * Create a unit-aligned cube triangulated into 12 triangles with the given edge length.
 * The cube spans [0,len] on x,y,z. Assigns color to all triangle vertices.
 */
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

/**
 * Create a set of latitude and partial longitude curves approximating a sphere.
 * Returns Curve_ada where each element is a Curve of points with the given color.
 */
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

/**
 * Create a simple triangle mesh approximating a sphere using latitudinal rings.
 * Assigns uniform color and light_intensity, and computes normals per triangle.
 */
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
