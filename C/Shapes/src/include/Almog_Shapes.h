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
    Point p1;
    Point p2;
    bool is_segment;
} Edge;

typedef struct {
    size_t length;
    size_t capacity;
    Edge *elements;
} Edge_ada; /* Edge ada array */

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
    int neighbor_tri_index[3]; /* int so -1 means no neighbor */
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


/**
 * should be less then 1e-6. 
 * If it is too big, then the triangulation breaks down.
 * 
 */
#ifndef AS_EPSILON
#define AS_EPSILON 1e-6 
#endif


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

#define                 as_edge_cross_edge(p11, p12, p21, p22) (((p12).x-(p11).x)*((p22).y-(p21).y) - ((p22).x-(p21).x)*((p12).y-(p11).y))
#define                 as_edge_cross_point(a1, b, a2, p) (b.x-a1.x)*(p.y-a2.y)-(b.y-a1.y)*(p.x-a2.x)
#define                 as_point_assert_finite(p) AS_ASSERT(as_point_is_finite(p))
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
#define                 as_points_equal(p1, p2) ((p1).x == (p2).x && (p1).y == (p2).y && (p1).z == (p2).z)
#define                 as_points_equal_approx(p1, p2) (fabs((p1).x - (p2).x) < AS_EPSILON && fabs((p1).y - (p2).y) < AS_EPSILON && fabs((p1).z - (p2).z) < AS_EPSILON)
#define                 as_tri_area_xy(p1, p2, p3) (0.5 * ((p2).x-(p1).x)*((p3).y-(p1).y)- 0.5 * ((p3).x-(p1).x)*((p2).y-(p1).y))
#define                 as_tri_equal_z(tri) ((tri.points[0].z == tri.points[1].z) && (tri.points[1].z == tri.points[2].z) && (tri.points[2].z == tri.points[0].z))
#define                 as_tri_implicit_mesh_expand_tri_to_points(tim, t_index) (tim).points.elements[(tim).triangles.elements[t_index].points_index[0]], (tim).points.elements[(tim).triangles.elements[t_index].points_index[1]], (tim).points.elements[(tim).triangles.elements[t_index].points_index[2]]
#define                 as_tri_implicit_mesh_get_point_of_tri_implicit(tim, t_index, p_index) (tim).points.elements[(tim).triangles.elements[t_index].points_index[p_index]]
#define                 as_tri_implicit_area_xy(tim, t_index) as_tri_area_xy(as_tri_implicit_mesh_get_point_of_tri_implicit(tim, t_index, 0), as_tri_implicit_mesh_get_point_of_tri_implicit(tim, t_index, 1), as_tri_implicit_mesh_get_point_of_tri_implicit(tim, t_index, 2))
#define                 as_tri_implicit_mesh_get_tri_implicit(tim, t_index) (tim).triangles.elements[t_index]
#define                 as_tri_edge_implicit_mesh_get_point_of_tri(teim, t_index, p_index) (teim).points.elements[(teim).edges.elements[(teim).triangles.elements[t_index].edges_index[p_index]].p1_index]
#define                 as_tri_edge_implicit_mesh_expand_edge_to_points(teim, edge_index) (teim).points.elements[(teim).edges.elements[edge_index].p1_index], (teim).points.elements[(teim).edges.elements[edge_index].p2_index]
#define                 as_tri_edge_implicit_mesh_expand_tri_to_points(teim, t_index) (teim).points.elements[(teim).edges.elements[(teim).triangles.elements[t_index].edges_index[0]].p1_index], (teim).points.elements[(teim).edges.elements[(teim).triangles.elements[t_index].edges_index[1]].p1_index], (teim).points.elements[(teim).edges.elements[(teim).triangles.elements[t_index].edges_index[2]].p1_index]
#define                 as_tri_edge_implicit_mesh_expand_point_of_tri_to_xyz(teim, t_index, p_index) (teim).points.elements[(teim).edges.elements[(teim).triangles.elements[t_index].edges_index[p_index]].p1_index].x, (teim).points.elements[(teim).edges.elements[(teim).triangles.elements[t_index].edges_index[p_index]].p1_index].y, (teim).points.elements[(teim).edges.elements[(teim).triangles.elements[t_index].edges_index[p_index]].p1_index].z
#define                 as_tri_edge_implicit_mesh_points_equal_by_indexs(mesh, p1_index, p2_index) as_points_equal((mesh).points.elements[p1_index], (mesh).points.elements[p2_index])

/* Doxygen-style function descriptions: https://www.doxygen.nl/manual/docblocks.html */
/* init functions */

void                    as_curve_ada_free(Curve_ada curves);
Tri_implicit_mesh       as_tri_implicit_mesh_init(void);
void                    as_tri_implicit_mesh_free(Tri_implicit_mesh mesh);
Tri_edge_implicit_mesh  as_tri_edge_implicit_mesh_init(void);
void                    as_tri_edge_implicit_mesh_free(Tri_edge_implicit_mesh mesh);

/* printing functions */

void                    as_point_print(Point p, char *name, size_t padding);
void                    as_curve_print(Curve c, char *name, size_t padding);
void                    as_edge_print(Edge e, char *name, size_t padding);
void                    as_edge_array_print(Edge *edge_array, size_t edge_array_len, char *name, size_t padding);
void                    as_edge_implicit_print(Edge_implicit ei, Point *points, char *name, size_t padding);
void                    as_edge_implicit_ada_print(Edge_implicit_ada ei_ada, Point *points, char *name, size_t padding);
void                    as_tri_print(Tri tri, char *name, size_t padding);
void                    as_tri_mesh_print(Tri_mesh mesh, char *name, size_t padding);
void                    as_tri_implicit_print(Tri_implicit tri_imp, Point *points, char *name, size_t padding);
void                    as_tri_implicit_mesh_print(Tri_implicit_mesh mesh, char *name, size_t padding);
void                    as_tri_edge_implicit_print(Tri_edge_implicit_mesh mesh, size_t tri_index, char *name, size_t padding);
void                    as_tri_edge_implicit_mesh_print(Tri_edge_implicit_mesh mesh, char *name, size_t padding);
void                    as_tri_edge_implicit_mesh_print_segments(Tri_edge_implicit_mesh mesh, char *name, size_t padding);

/* utils functions */

bool                    as_bbox_overlap(Point a, Point b, Point c, Point d);
size_t                  as_choose(int n, int k);
uint32_t                as_color_interpolate(uint32_t c1, uint32_t c2, float t);
Curve                   as_curve_create_random_points(size_t num_of_points, float min_x, float max_x, float min_y, float max_y, float min_z, float max_z, unsigned int seed);
bool                    as_edge_intersect_edge(Point p11, Point p12, Point p21, Point p22);
bool                    as_edge_intersects_any_point_in_array(Point a, Point b, Point *points_array, size_t points_array_len, float eps, Point *hit_point);
bool                    as_edge_array_intersects_points_array(Edge *edges_array, size_t edges_array_len, Point *points_array, size_t points_array_len, float eps);
int                     as_edge_implicit_ada_get_edge_index(Edge_implicit_ada eia, Point *points, Point p1, Point p2);
size_t                  as_factorial(size_t n);
Point                   as_mat2D_to_point(Mat2D m);
float                   as_orient2d(Point a, Point b, Point c);
float                   as_point_get_min_distance_from_point_array(Point *point_array, size_t point_array_len, Point p);
void                    as_point_to_mat2D(Point p, Mat2D m);
size_t                  as_point_in_curve_occurrences(Point p, Curve c);
int                     as_point_in_curve_index(Point p, Curve c);
Point                   as_points_interpolate(Point p1, Point p2, float t);
bool                    as_point_is_finite(Point p);
bool                    as_point_on_edge_xy(Point a, Point b, Point p, float eps);
float                   as_point_on_edge_xy_t(Point a, Point b, Point p);
float                   as_points_distance(Point p1, Point p2);
void                    as_points_array_convex_hull_Jarvis_march_2D(Curve *conv, Point *points, const size_t len);
void                    as_points_array_swap_points(Point *c, const size_t index1, const size_t index2);
void                    as_points_array_order_lexicographically(Point *c, const size_t len);
Tri_implicit_mesh       as_points_array_get_lexicographic_triangulation(Point *points, const size_t len);
bool                    as_quad_is_convex(Point p1, Point p2, Point p3, Point p4);
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
int                     as_tri_implicit_mesh_get_triangles_with_edge(Tri_implicit_mesh mesh, Point p1, Point p2, Tri_implicit *tri_out1, Tri_implicit *tri_out2);
int                     as_tri_implicit_mesh_get_triangles_indexs_with_edge(Tri_implicit_mesh mesh, Point p1, Point p2, size_t*tri_out1_index, size_t*tri_out2_index);
Tri_mesh                as_tri_implicit_mesh_to_tri_mesh(Tri_implicit_mesh implicit_mesh, float light_intensity, uint32_t color);
Tri_edge_implicit_mesh  as_tri_implicit_mesh_to_tri_edge_implicit_mesh(Tri_implicit_mesh implicit_mesh);
int                     as_tri_edge_implicit_mesh_check_neighbors(Tri_edge_implicit_mesh mesh, size_t tri_index);
bool                    as_tri_edge_implicit_mesh_check_point_intersect_any_edge(Tri_edge_implicit_mesh mesh, Point point, float eps, size_t *intersecting_edge_index);
void                    as_tri_edge_implicit_mesh_copy(Tri_edge_implicit_mesh *des, Tri_edge_implicit_mesh src);
int                     as_tri_edge_implicit_mesh_edge_index_belongs_to_tri(Tri_edge_implicit_mesh mesh, size_t edge_index);
float                   as_tri_edge_implicit_mesh_get_min_edge_length(Tri_edge_implicit_mesh mesh);
int                     as_tri_edge_implicit_mesh_get_containing_tri_index_of_point(Tri_edge_implicit_mesh mesh, Point point);
int                     as_tri_edge_implicit_mesh_get_triangles_with_edge(Tri_edge_implicit_mesh mesh, Point p1, Point p2, Tri_edge_implicit *tri_out1, Tri_edge_implicit *tri_out2);
int                     as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge(Tri_edge_implicit_mesh mesh, Point p1, Point p2, size_t *tri_out1_index, size_t *tri_out2_index);
int                     as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge_index(Tri_edge_implicit_mesh mesh, size_t edge_index, size_t inv_edge_index, size_t *tri_out1_index, size_t *tri_out2_index);
int                     as_tri_edge_implicit_mesh_get_third_points_from_edge(Tri_edge_implicit_mesh mesh, Point p1, Point p2, Point *tri1_third_point, Point *tri2_third_point);
void                    as_tri_edge_implicit_mesh_delete_point(Tri_edge_implicit_mesh *mesh, Point p);
void                    as_tri_edge_implicit_mesh_remove_point(Tri_edge_implicit_mesh *mesh, Point p);
void                    as_tri_edge_implicit_mesh_delete_edge(Tri_edge_implicit_mesh *mesh, Point p1, Point p2);
void                    as_tri_edge_implicit_mesh_remove_edge(Tri_edge_implicit_mesh *mesh, Point p1, Point p2, int *edge_index_out, int *inv_edge_index_out);
void                    as_tri_edge_implicit_mesh_remove_triangle(Tri_edge_implicit_mesh *mesh, size_t tri_index);
int                     as_tri_edge_implicit_mesh_set_neighbor_of_tri(Tri_edge_implicit_mesh mesh, size_t tri_index);
void                    as_tri_edge_implicit_mesh_set_all_tri_neighbor(Tri_edge_implicit_mesh mesh);
Tri_mesh                as_tri_edge_implicit_mesh_to_tri_mesh(Tri_edge_implicit_mesh tei_mesh, float light_intensity, uint32_t color);
Tri_implicit_mesh       as_tri_edge_implicit_mesh_to_tri_implicit_mesh(Tri_edge_implicit_mesh tei_mesh);

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
#define                 AS_EDGE_PRINT(e) as_edge_print(e, #e, 0)
#define                 AS_EDGE_ARRAY_PRINT(edge_array, edge_array_len) as_edge_array_print(edge_array, edge_array_len, #edge_array, 0)
#define                 AS_EDGE_IMPLICIT_PRINT(ei, points) as_edge_implicit_print(ei, points, #ei, 0)
#define                 AS_EDGE_IMPLICIT_ADA_PRINT(ei_ada, points) as_edge_implicit_ada_print(ei_ada, points, #ei_ada, 0)
#define                 AS_TRI_MESH_PRINT(tm) as_tri_mesh_print(tm, #tm, 0)
#define                 AS_TRI_IMPLICIT_PRINT(t, points) as_tri_implicit_print(t, points, #t, 0)
#define                 AS_TRI_IMPLICIT_MESH_PRINT(tm) as_tri_implicit_mesh_print(tm, #tm, 0)
#define                 AS_TRI_EDGE_IMPLICIT_PRINT(tm, tri_index) as_tri_edge_implicit_print(tm, tri_index, #tm, 0)
#define                 AS_TRI_EDGE_IMPLICIT_MESH_PRINT(tm) as_tri_edge_implicit_mesh_print(tm, #tm, 0)
#define                 AS_TRI_EDGE_IMPLICIT_MESH_PRINT_SEGMENTS(tm) as_tri_edge_implicit_mesh_print_segments(tm, #tm, 0)

void as_curve_ada_free(Curve_ada curves)
{
    for (size_t i = 0; i < curves.length; i++) {
        free(curves.elements[i].elements);
    }
    free(curves.elements);
}

Tri_implicit_mesh as_tri_implicit_mesh_init(void)
{
    Tri_implicit_mesh mesh = {0};

    ada_init_array(Point, mesh.points);
    ada_init_array(Tri_implicit, mesh.triangles);

    return mesh;
}

void as_tri_implicit_mesh_free(Tri_implicit_mesh mesh)
{
    free(mesh.points.elements);
    free(mesh.triangles.elements);
}

Tri_edge_implicit_mesh as_tri_edge_implicit_mesh_init(void)
{
    Tri_edge_implicit_mesh mesh = {0};

    ada_init_array(Point, mesh.points);
    ada_init_array(Edge_implicit, mesh.edges);
    ada_init_array(Tri_edge_implicit, mesh.triangles);

    return mesh;
}

void as_tri_edge_implicit_mesh_free(Tri_edge_implicit_mesh mesh)
{
    free(mesh.points.elements);
    free(mesh.edges.elements);
    free(mesh.triangles.elements);
}

void as_point_print(Point p, char *name, size_t padding)
{
    as_point_assert_finite(p);

    printf("%*s%s:\n", (int) padding, "", name);
    printf("%*s    (%f, %f, %f)\n", (int) padding, "", p.x, p.y, p.z);
}

void as_curve_print(Curve c, char *name, size_t padding)
{
    AS_ASSERT(c.elements != NULL);
    AS_ASSERT(name != NULL);

    printf("%*s%s:\n", (int) padding, "", name);
    for (size_t i = 0; i < c.length; i++) {
        printf("%*s    point%zu: (%f, %f, %f)\n", (int) padding, "", i, c.elements[i].x, c.elements[i].y, c.elements[i].z);
    }
    printf("%*s    color: %X\n", (int)padding, "", c.color);
}

void as_edge_print(Edge e, char *name, size_t padding)
{
    as_point_assert_finite(e.p1);
    as_point_assert_finite(e.p2);

    printf("%*s%s:\n", (int) padding, "", name);
    printf("%*s    (%f, %f, %f)\n", (int) padding+4, "", as_point_expand_to_xyz(e.p1));
    printf("%*s    (%f, %f, %f)\n", (int) padding+4, "", as_point_expand_to_xyz(e.p2));
    printf("%*s    segment = %d\n", (int) padding+4, "", e.is_segment);

}

void as_edge_array_print(Edge *edge_array, size_t edge_array_len, char *name, size_t padding)
{
    char temp_str[256];
    printf("%*s%s:\n", (int) padding, "", name);

    for (size_t i = 0; i < edge_array_len; i++) {
        snprintf(temp_str, 256, "edge %zu", i);
        as_edge_print(edge_array[i], temp_str, 4);
    }
}

void as_edge_implicit_print(Edge_implicit ei, Point *points, char *name, size_t padding)
{
    AS_ASSERT(points != NULL);
    AS_ASSERT(name != NULL);
    AS_ASSERT(ei.p1_index < (size_t)-1 && ei.p2_index < (size_t)-1 && "indices valid");

    printf("%*s%s:\n", (int) padding, "", name);
    printf("%*s    (%f, %f, %f)\n", (int) padding+4, "", as_point_expand_to_xyz(points[ei.p1_index]));
    printf("%*s    (%f, %f, %f)\n", (int) padding+4, "", as_point_expand_to_xyz(points[ei.p2_index]));
    printf("%*s    (%zu -> %zu)\n", (int) padding+4, "", ei.p1_index, ei.p2_index);
    printf("%*s    segment = %d\n", (int) padding+4, "", ei.is_segment);
}

void as_edge_implicit_ada_print(Edge_implicit_ada ei_ada, Point *points, char *name, size_t padding)
{
    char temp_str[256];
    printf("%*s%s:\n", (int) padding, "", name);

    for (size_t i = 0; i < ei_ada.length; i++) {
        snprintf(temp_str, 256, "edge %zu", i);
        as_edge_implicit_print(ei_ada.elements[i], points, temp_str, padding+4);
    }
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

void as_tri_implicit_print(Tri_implicit tri_imp, Point *points, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);
    printf("%*s    points:\n", (int)padding, "");
    printf("%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n", (int) padding, "", points[tri_imp.points_index[0]].x, points[tri_imp.points_index[0]].y, points[tri_imp.points_index[0]].z, (int) padding, "", points[tri_imp.points_index[1]].x, points[tri_imp.points_index[1]].y, points[tri_imp.points_index[1]].z, (int) padding, "", points[tri_imp.points_index[2]].x, points[tri_imp.points_index[2]].y, points[tri_imp.points_index[2]].z);
}

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

void as_tri_edge_implicit_print(Tri_edge_implicit_mesh mesh, size_t tri_index, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);

    printf("%*s    points:\n", (int)padding+4, "");
    for (size_t j = 0; j < 3; j++) {
        printf("%*s    (%f, %f, %f)\n", (int) padding+4, "", as_tri_edge_implicit_mesh_expand_point_of_tri_to_xyz(mesh, tri_index, j));
    }
}

void as_tri_edge_implicit_mesh_print(Tri_edge_implicit_mesh mesh, char *name, size_t padding)
{
    char temp_str[256];
    printf("%*s%s:\n", (int) padding, "", name);
    as_curve_print(mesh.points, "points", 4);

    printf("%*s------------------------------\n", (int) padding+4, "");

    as_edge_implicit_ada_print(mesh.edges, mesh.points.elements, "edges", 4);

    printf("%*s------------------------------\n", (int) padding+4, "");

    for (size_t i = 0; i < mesh.triangles.length; i++) {
        snprintf(temp_str, 256, "tri %zu", i);
        printf("%*s%s:\n", (int) padding+4, "", temp_str);
        printf("%*s    points: (%zu, %zu, %zu)\n", (int)padding+4, "", mesh.edges.elements[mesh.triangles.elements[i].edges_index[0]].p1_index, mesh.edges.elements[mesh.triangles.elements[i].edges_index[1]].p1_index, mesh.edges.elements[mesh.triangles.elements[i].edges_index[2]].p1_index);
        for (size_t j = 0; j < 3; j++) {
            printf("%*s    (%f, %f, %f)\n", (int) padding+4, "", as_tri_edge_implicit_mesh_expand_point_of_tri_to_xyz(mesh, i, j));
        }
        printf("%*s    neighbors: (%d, %d, %d)\n", (int)padding+4, "", mesh.triangles.elements[i].neighbor_tri_index[0], mesh.triangles.elements[i].neighbor_tri_index[1], mesh.triangles.elements[i].neighbor_tri_index[2]);
    }
}

void as_tri_edge_implicit_mesh_print_segments(Tri_edge_implicit_mesh mesh, char *name, size_t padding)
{
    char temp_str[256];
    printf("%*s%s:\n", (int) padding, "", name);

    for (size_t i = 0; i < mesh.edges.length; i++) {
        if (mesh.edges.elements[i].is_segment) {
            snprintf(temp_str, 256, "edge %zu", i);
            as_edge_implicit_print(mesh.edges.elements[i], mesh.points.elements, temp_str, 4);
        }
    }
}

bool as_bbox_overlap(Point a, Point b, Point c, Point d)
{
    as_point_assert_finite(a);
    as_point_assert_finite(b);
    as_point_assert_finite(c);
    as_point_assert_finite(d);

    float min_ax = fminf(a.x, b.x), max_ax = fmaxf(a.x, b.x);
    float min_ay = fminf(a.y, b.y), max_ay = fmaxf(a.y, b.y);
    float min_cx = fminf(c.x, d.x), max_cx = fmaxf(c.x, d.x);
    float min_cy = fminf(c.y, d.y), max_cy = fmaxf(c.y, d.y);
    return !(max_ax < min_cx || max_cx < min_ax ||
             max_ay < min_cy || max_cy < min_ay);
}

size_t as_choose(int n, int k)
{
    if (n < k || n < 0 || k < 0) {
        return 0;
    }

    size_t ans = 1;

    for (size_t i = (size_t)n; i >= (size_t)n-(size_t)k+1; i--) {
        ans *= i;
    }
    for (size_t i = (size_t)k; i >= 1; i--) {
        ans /= i;
    }

    return ans;
}

uint32_t as_color_interpolate(uint32_t c1, uint32_t c2, float t)
{
    uint32_t r1, g1, b1, a1;
    uint32_t r2, g2, b2, a2;
    HexARGB_RGBA_VAR(c1, r1, g1, b1, a1);
    HexARGB_RGBA_VAR(c2, r2, g2, b2, a2);

    uint32_t r = r1 * (1 - t) + r2 * (t);
    uint32_t g = g1 * (1 - t) + g2 * (t);
    uint32_t b = b1 * (1 - t) + b2 * (t);
    uint32_t a = a1 * (1 - t) + a2 * (t);

    return RGBA_hexARGB(r, g, b, a);
}

Curve as_curve_create_random_points(size_t num_of_points, float min_x, float max_x, float min_y, float max_y, float min_z, float max_z, unsigned int seed)
{
    AS_ASSERT(num_of_points >= 1 && "must request >= 1 point");
    AS_ASSERT(max_x >= min_x && max_y >= min_y && "min/max valid");

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

bool as_edge_intersect_edge(Point p11, Point p12, Point p21, Point p22)
{
    as_point_assert_finite(p11);
    as_point_assert_finite(p12);
    as_point_assert_finite(p21);
    as_point_assert_finite(p22);
    AS_ASSERT(!as_points_equal(p11, p12) && "degenerate segment (p11,p12)");
    AS_ASSERT(!as_points_equal(p21, p22) && "degenerate segment (p21,p22)");

    /* Exclude shared endpoints (do not treat as intersecting) */
    if (as_points_equal(p11, p21) || as_points_equal(p11, p22) ||
        as_points_equal(p12, p21) || as_points_equal(p12, p22)) {
        return false;
    }
    if (!as_bbox_overlap(p11, p12, p21, p22)) return false;
    const float eps = AS_EPSILON;
    float o1 = as_orient2d(p11, p12, p21);
    float o2 = as_orient2d(p11, p12, p22);
    float o3 = as_orient2d(p21, p22, p11);
    float o4 = as_orient2d(p21, p22, p12);
    /* Proper intersection: strict opposite orientations on both segments */
    return (o1 * o2 < -eps) && (o3 * o4 < -eps);
}

bool as_edge_intersects_any_point_in_array(Point a, Point b, Point *points_array, size_t points_array_len, float eps, Point *hit_point)
{
    AS_ASSERT((points_array_len == 0 || points_array != NULL) && "points array");
    AS_ASSERT(eps >= 0.0f && "eps must be non-negative");

    for (size_t i = 0; i < points_array_len; i++) {
        Point q = points_array[i];

        /* Skip endpoints (treat equal within eps) */
        if (as_points_equal(q, a) || as_points_equal(q, b)) {
            continue;
        }

        if (as_point_on_edge_xy(a, b, q, eps)) {
            if (hit_point) *hit_point = points_array[i];
            return true;
        }
    }
    return false;
}

bool as_edge_array_intersects_points_array(Edge *edges_array, size_t edges_array_len, Point *points_array, size_t points_array_len, float eps)
{
    for (size_t edge_index = 0; edge_index < edges_array_len; edge_index++) {
        Edge current_edge = edges_array[edge_index];
        if(as_edge_intersects_any_point_in_array(current_edge.p1, current_edge.p2, points_array, points_array_len, eps, NULL)) {
            return true;
        }
    }

    return false;
}

int as_edge_implicit_ada_get_edge_index(Edge_implicit_ada eia, Point *points, Point p1, Point p2)
{
    for (size_t edge_index = 0; edge_index < eia.length; edge_index++) {
        #if 0
        bool first_index_match = as_points_equal(points[eia.elements[edge_index].p1_index], p1);
        bool second_index_match = as_points_equal(points[eia.elements[edge_index].p2_index], p2);
        if (first_index_match && second_index_match) {
            res = edge_index;
            return res;
        }
        #else
        if (as_points_equal(points[eia.elements[edge_index].p1_index], p1) && as_points_equal(points[eia.elements[edge_index].p2_index], p2)) {
            return edge_index;
        }
        #endif
    }

    return -1;
}

size_t as_factorial(size_t n)
{
    size_t fact = 1, i;

    for (i = 1; i <= n; i++) {
        fact *= i;
    }

    return fact;
}

Point as_mat2D_to_point(Mat2D m)
{
    Point res = {.x = MAT2D_AT(m, 0, 0), .y = MAT2D_AT(m, 1, 0), .z = MAT2D_AT(m, 2, 0), .w = 1};
    return res;
}

float as_orient2d(Point a, Point b, Point c)
{
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

float as_point_get_min_distance_from_point_array(Point *point_array, size_t point_array_len, Point p)
{
    float min_dis = FLT_MAX;

    for (size_t i = 0; i < point_array_len; i++) {
        float current_dis = as_points_distance(point_array[i], p);
        if (current_dis < min_dis) {
            min_dis = current_dis;
        }
    }

    return min_dis;
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

size_t as_point_in_curve_occurrences(Point p, Curve c)
{
    size_t num_of_occurrences = 0;

    for (size_t i = 0; i < c.length; i++) {
        if (as_points_equal(c.elements[i], p)) num_of_occurrences++;
    }

    return num_of_occurrences;
}

int as_point_in_curve_index(Point p, Curve c)
{
    for (size_t i = 0; i < c.length; i++) {
        if (as_points_equal(c.elements[i], p)) return (int)i;
    }

    return -1;
}

Point as_points_interpolate(Point p1, Point p2, float t)
{
    Point p = {0};

    p.x = p1.x * t + p2.x * (1 - t);
    p.y = p1.y * t + p2.y * (1 - t);
    p.z = p1.z * t + p2.z * (1 - t);
    p.w = p1.w * t + p2.w * (1 - t);

    return p;
}

bool as_point_is_finite(Point p)
{
    return isfinite(p.x) && isfinite(p.y) && isfinite(p.z) && isfinite(p.w);
}

bool as_point_on_edge_xy(Point a, Point b, Point p, float eps)
{
    as_point_assert_finite(a);
    as_point_assert_finite(b);
    as_point_assert_finite(p);
    if (eps < AS_EPSILON) eps = AS_EPSILON;

    /* Quick bbox reject */
    if (!as_bbox_overlap(a, b, p, p)) return false;

    /* Vector ab */
    float abx = b.x - a.x;
    float aby = b.y - a.y;

    /* If edge is degenerate, nothing to do */
    float ab_len2 = abx * abx + aby * aby;
    if (ab_len2 <= 0.0f) return false;

    /* Signed area (twice triangle area) -> collinearity check */
    float area = as_orient2d(a, b, p); /* = cross(ab, ap) */
    if (fabsf(area) > eps * sqrtf(ab_len2)) {
        return false;
    }

    /* Parametric projection t of p on edge ab: a + t*(ab) */
    float t = as_point_on_edge_xy_t(a, b, p);

    /* Exclude endpoints: require t strictly inside (0,1) with margin */
    if (t <= eps || t >= 1.0f - eps) return false;

    /* Also check perpendicular distance explicitly vs eps (optional, but
       helps when ab is long and area test alone is too permissive) */
    float dist_perp = fabsf(area) / sqrtf(ab_len2);
    if (dist_perp > eps) return false;

    return true;
}

float as_point_on_edge_xy_t(Point a, Point b, Point p)
{
    as_point_assert_finite(a);
    as_point_assert_finite(b);
    as_point_assert_finite(p);
    /* Vector ab */
    float abx = b.x - a.x;
    float aby = b.y - a.y;
    float ab_len2 = abx * abx + aby * aby;

    /* Parametric projection t of p on edge ab: a + t*(ab) */
    float apx = p.x - a.x;
    float apy = p.y - a.y;
    float t = (apx * abx + apy * aby) / ab_len2;

    return t;
}

float as_points_distance(Point p1, Point p2)
{
    as_point_assert_finite(p1);
    as_point_assert_finite(p2);
    return sqrtf((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
}
// #define as_points_distance(p1, p2) sqrtf(((p1).x - (p2).x) * ((p1).x - (p2).x) + ((p1).y - (p2).y) * ((p1).y - (p2).y) + ((p1).z - (p2).z) * ((p1).z - (p2).z));

void as_points_array_convex_hull_Jarvis_march_2D(Curve *conv, Point *points, const size_t len)
{
    AS_ASSERT(conv != NULL);
    AS_ASSERT(points != NULL);
    AS_ASSERT(len >= 1 && "expected at least one point");

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

    float eps = AS_EPSILON;

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

void as_points_array_swap_points(Point *c, const size_t index1, const size_t index2)
{
    Point temp = c[index1];
    c[index1] = c[index2];
    c[index2] = temp;
}

void as_points_array_order_lexicographically(Point *c, const size_t len)
{
    float epsilon = AS_EPSILON;
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

Tri_implicit_mesh as_points_array_get_lexicographic_triangulation(Point *points, const size_t len)
{
    AS_ASSERT(points != NULL);
    AS_ASSERT(len >= 1 && "triangulation expects >= 1 point");

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
    printf("[INFO] lexicographic triangulation:\n");
    for (point_index = point_index+1; point_index < len; point_index++) {
        printf("\r       points: %zu | done: %f%%", point_index+1, 100.0f * (float)(point_index+1) / (float)(len));
        fflush(stdout);

        as_points_array_convex_hull_Jarvis_march_2D(&convex_hull, implicit_mesh.points.elements, implicit_mesh.points.length);

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
    printf("\n");

    free(convex_hull.elements);

    return implicit_mesh;
}

bool as_quad_is_convex(Point p1, Point p2, Point p3, Point p4)
{
    bool is_convex = true;

    if (as_edge_cross_edge(p1, p2, p2, p3) * as_edge_cross_edge(p2, p3, p3, p4) < 0) {
        is_convex = false;
    } else if (as_edge_cross_edge(p2, p3, p3, p4) * as_edge_cross_edge(p3, p4, p4, p1) < 0) {
        is_convex = false;
    } else if (as_edge_cross_edge(p3, p4, p4, p1) * as_edge_cross_edge(p4, p1, p1, p2) < 0) {
        is_convex = false;
    } else if (as_edge_cross_edge(p4, p1, p1, p2) * as_edge_cross_edge(p1, p2, p2, p3) < 0) {
        is_convex = false;
    }

    return is_convex;
}

float as_rand_float(void)
{
    return (float) rand() / (float) RAND_MAX;
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
        Point p01 = as_points_interpolate(p0, p1, 0.5);
        Point p12 = as_points_interpolate(p1, p2, 0.5);
        Point p20 = as_points_interpolate(p2, p0, 0.5);
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

float as_tri_mesh_const_x(Tri_mesh mesh)
{
    float x_value = 0;

    for (size_t tri_index = 0; tri_index < mesh.length; tri_index++) {
        Tri current_tri = mesh.elements[tri_index];
        if (tri_index == 0) x_value = current_tri.points[0].x;

        for (size_t i = 0; i < 3; i++) {
            if (x_value != current_tri.points[i].x) return NAN;
        }
    }

    return x_value;
}

float as_tri_mesh_const_y(Tri_mesh mesh)
{
    float y_value = 0;

    for (size_t tri_index = 0; tri_index < mesh.length; tri_index++) {
        Tri current_tri = mesh.elements[tri_index];
        if (tri_index == 0) y_value = current_tri.points[0].y;

        for (size_t i = 0; i < 3; i++) {
            if (y_value != current_tri.points[i].y) return NAN;
        }
    }

    return y_value;
}

float as_tri_mesh_const_z(Tri_mesh mesh)
{
    float z_value = 0;

    for (size_t tri_index = 0; tri_index < mesh.length; tri_index++) {
        Tri current_tri = mesh.elements[tri_index];
        if (tri_index == 0) z_value = current_tri.points[0].z;

        for (size_t i = 0; i < 3; i++) {
            if (z_value != current_tri.points[i].z) return NAN;
        }
    }

    return z_value;
}

Tri as_tri_implicit_to_tri(Tri_implicit tri_implicit, Point *points)
{
    Tri tri = {0};

    tri.points[0] = points[tri_implicit.points_index[0]];
    tri.points[1] = points[tri_implicit.points_index[1]];
    tri.points[2] = points[tri_implicit.points_index[2]];

    as_tri_set_normals(&tri);

    return tri;
}

float as_tri_implicit_mesh_const_x(Tri_implicit_mesh mesh)
{
    float value = 0;

    for (size_t p_index = 0; p_index < mesh.points.length; p_index++) {
        if (p_index == 0) value = mesh.points.elements[p_index].x;

        if (value != mesh.points.elements[p_index].x) return NAN;
    }

    return value;
}

float as_tri_implicit_mesh_const_y(Tri_implicit_mesh mesh)
{
    float value = 0;

    for (size_t p_index = 0; p_index < mesh.points.length; p_index++) {
        if (p_index == 0) value = mesh.points.elements[p_index].y;

        if (value != mesh.points.elements[p_index].y) return NAN;
    }

    return value;
}

float as_tri_implicit_mesh_const_z(Tri_implicit_mesh mesh)
{
    float value = 0;

    for (size_t p_index = 0; p_index < mesh.points.length; p_index++) {
        if (p_index == 0) value = mesh.points.elements[p_index].z;

        if (value != mesh.points.elements[p_index].z) return NAN;
    }

    return value;
}

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
                fprintf(stderr, "%s:%d:\n[Warning] implicit mesh has an edge with more then two triangles\n\n", __FILE__, __LINE__);
                exit(1);
            }
        }
    }

    return num_of_tri_out;
}

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
                fprintf(stderr, "%s:%d:\n[Warning] implicit mesh has an edge with more then two triangles\n\n", __FILE__, __LINE__);
                exit(1);
            }
        }
    }

    return num_of_tri_out;
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

int as_tri_edge_implicit_mesh_check_neighbors(Tri_edge_implicit_mesh mesh, size_t tri_index)
{
    int num_of_neighbors = 0;
    for (size_t i = 0; i < 3; i++) {
        int current_neighbor_index = mesh.triangles.elements[tri_index].neighbor_tri_index[i];
        if (current_neighbor_index == -1) continue;
        Tri_edge_implicit current_tri = mesh.triangles.elements[current_neighbor_index];
        bool tri_is_neighbor_of_neighbor = false;
        for (size_t j = 0; j < 3; j++) {
            if ((int)tri_index == current_tri.neighbor_tri_index[j]) {
                tri_is_neighbor_of_neighbor = true;
            }
        }
        if (!tri_is_neighbor_of_neighbor) return -1;
        num_of_neighbors++;
    }

    return num_of_neighbors;
}

bool as_tri_edge_implicit_mesh_check_point_intersect_any_edge(Tri_edge_implicit_mesh mesh, Point point, float eps, size_t *intersecting_edge_index)
{
    for (size_t edge_index = 0; edge_index < mesh.edges.length; edge_index++) {
        Edge_implicit current_edge = mesh.edges.elements[edge_index];
        if (as_point_on_edge_xy(mesh.points.elements[current_edge.p1_index], mesh.points.elements[current_edge.p2_index], point, eps)) {
            if (intersecting_edge_index) *intersecting_edge_index = edge_index;
            return true;
        }
    }

    return false;
}

void as_tri_edge_implicit_mesh_copy(Tri_edge_implicit_mesh *des, Tri_edge_implicit_mesh src)
{
    Tri_edge_implicit_mesh temp_des = *des;

    temp_des.points.length = 0;
    temp_des.edges.length = 0;
    temp_des.triangles.length = 0;

    for (size_t i = 0; i < src.points.length; i++) {
        ada_appand(Point, temp_des.points, src.points.elements[i]);
    }
    for (size_t i = 0; i < src.edges.length; i++) {
        ada_appand(Edge_implicit, temp_des.edges, src.edges.elements[i]);
    }
    for (size_t i = 0; i < src.triangles.length; i++) {
        ada_appand(Tri_edge_implicit, temp_des.triangles, src.triangles.elements[i]);
    }

    *des = temp_des;
}

int as_tri_edge_implicit_mesh_edge_index_belongs_to_tri(Tri_edge_implicit_mesh mesh, size_t edge_index)
{
    for (size_t tri_index = 0; tri_index < mesh.triangles.length; tri_index++) {
        Tri_edge_implicit current_tri = mesh.triangles.elements[tri_index];
        for (size_t i = 0; i < 3; i++) {
            if (current_tri.edges_index[i] == edge_index) {
                return (int)tri_index;
            }
        }
    } 

    return -1;
}

float as_tri_edge_implicit_mesh_get_min_edge_length(Tri_edge_implicit_mesh mesh)
{
    float min_dis = FLT_MAX;

    for (size_t i = 0; i < mesh.edges.length; i++) {
        Edge_implicit current_ei = mesh.edges.elements[i];
        float current_dis = as_points_distance(mesh.points.elements[current_ei.p1_index], mesh.points.elements[current_ei.p2_index]);
        if (min_dis > current_dis) {
            min_dis  =current_dis;
        }
    }

    return min_dis;
}

int as_tri_edge_implicit_mesh_get_containing_tri_index_of_point(Tri_edge_implicit_mesh mesh, Point point)
{
    for (size_t tri_index = 0; tri_index < mesh.triangles.length; tri_index++) {
        Point p0, p1, p2;
        p0 = as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri_index, 0);
        p1 = as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri_index, 1);
        p2 = as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri_index, 2);

        float w = as_edge_cross_point(p0, p1, p1, p2);

        float w0 = as_edge_cross_point(p0, p1, p0, point);
        float w1 = as_edge_cross_point(p1, p2, p1, point);
        float w2 = as_edge_cross_point(p2, p0, p2, point);

        if (w0 * w >= 0 && w1 * w >= 0 &&  w2 * w >= 0) {
            return (int)tri_index;
        }
    }

    return -1;
}

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
                /* if this line will be uncommented, then the check will never be reached */
                return num_of_tri_out;
            } else if (num_of_tri_out > 2) {
                fprintf(stderr, "%s:%d:\n[Warning] edge implicit mesh has an edge with more then two triangles\n\n", __FILE__, __LINE__);
                exit(1);
            }
        }
    }

    return num_of_tri_out;
}

int as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge(Tri_edge_implicit_mesh mesh, Point p1, Point p2, size_t *tri_out1_index, size_t *tri_out2_index)
{
    // int p1_index = as_point_in_curve_index(p1, mesh.points);
    // int p2_index = as_point_in_curve_index(p2, mesh.points);
    // if (p1_index == -1 || p2_index == -1) return 0;

    int num_of_tri_out = 0;

    for (size_t tri_index = 0; tri_index < mesh.triangles.length; tri_index++) {
        bool p1_is_part_of_tri = false;
        bool p2_is_part_of_tri = false;
        // /* check if p1 is part of tri */
        // for (size_t i = 0; i < 3; i++) {
        //     if (as_points_equal(p1, as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri_index, i))) p1_is_part_of_tri = true;
        // }
        // /* check if p2 is part of tri */
        // for (size_t i = 0; i < 3; i++) {
        //     if (as_points_equal(p2, as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri_index, i))) p2_is_part_of_tri = true;
        // }
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p1, as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri_index, i))) p1_is_part_of_tri = true;
            if (as_points_equal(p2, as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri_index, i))) p2_is_part_of_tri = true;
        }

        if (p1_is_part_of_tri && p2_is_part_of_tri) {
            /* tri has both points*/
            if (num_of_tri_out == 0) {
                if (tri_out1_index) *tri_out1_index = tri_index;
                num_of_tri_out++;
            } else if (num_of_tri_out == 1) {
                if (tri_out2_index) *tri_out2_index = tri_index;
                num_of_tri_out++;
                /* if this line will be uncommented, then the check will never be reached */
                return num_of_tri_out;
            } else if (num_of_tri_out > 2) {
                fprintf(stderr, "%s:%d:\n[Warning] edge implicit mesh has an edge with more then two triangles\n\n", __FILE__, __LINE__);
                exit(1);
            }
        }
    }

    return num_of_tri_out;
}

int as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge_index(Tri_edge_implicit_mesh mesh, size_t edge_index, size_t inv_edge_index, size_t *tri_out1_index, size_t *tri_out2_index)
{
    int num_of_tri = 0;
    for (size_t ti = 0; ti < mesh.triangles.length; ti++) {
        Tri_edge_implicit t = mesh.triangles.elements[ti];
        if ((t.edges_index[0] == edge_index || t.edges_index[0] == inv_edge_index) || (t.edges_index[1] == edge_index || t.edges_index[1] == inv_edge_index) || (t.edges_index[2] == edge_index || t.edges_index[2] == inv_edge_index)) {
            if (num_of_tri == 0) {
                *tri_out1_index = ti;
                num_of_tri = 1;
            } else {
                *tri_out2_index = ti;
                num_of_tri = 2;
                break;
            }
        }
    }
    
    return num_of_tri;
}

int as_tri_edge_implicit_mesh_get_third_points_from_edge(Tri_edge_implicit_mesh mesh, Point p1, Point p2, Point *tri1_third_point, Point *tri2_third_point)
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
        int p3_local_index = -1;
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p1, as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri_index, i))) {
                p1_is_part_of_tri = true;
            } else if (as_points_equal(p2, as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri_index, i))) {
                p2_is_part_of_tri = true;
            } else {
                p3_local_index = i;
            }
        }
       
        if (p1_is_part_of_tri && p2_is_part_of_tri) {
            /* tri has both points*/
            if (num_of_tri_out == 0) {
                *tri1_third_point = as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri_index, p3_local_index);
                num_of_tri_out++;
            } else if (num_of_tri_out == 1) {
                *tri2_third_point = as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri_index, p3_local_index);
                num_of_tri_out++;
                /* if this line will be uncommented, then the check will never be reached */
                return num_of_tri_out;
            } else if (num_of_tri_out > 2) {
                fprintf(stderr, "%s:%d:\n[Warning] edge implicit mesh has an edge with more then two triangles\n\n", __FILE__, __LINE__);
                exit(1);
            }
        }
    }

    return num_of_tri_out;
}

void as_tri_edge_implicit_mesh_delete_point(Tri_edge_implicit_mesh *mesh, Point p)
{
    Tri_edge_implicit_mesh temp_mesh = *mesh;

    as_tri_edge_implicit_mesh_remove_point(&temp_mesh, p);

    for (size_t edge_index = 0; edge_index < temp_mesh.edges.length; edge_index++) {
        bool point_in_edge = false;
        Point other_point = {0};
        if (as_points_equal(p, temp_mesh.points.elements[temp_mesh.edges.elements[edge_index].p1_index])) {
            point_in_edge = true;
            other_point = temp_mesh.points.elements[temp_mesh.edges.elements[edge_index].p2_index];
        }
        if (as_points_equal(p, temp_mesh.points.elements[temp_mesh.edges.elements[edge_index].p2_index])) {
            point_in_edge = true;
            other_point = temp_mesh.points.elements[temp_mesh.edges.elements[edge_index].p1_index];
        }
        if (point_in_edge) {
            as_tri_edge_implicit_mesh_delete_edge(&temp_mesh, p, other_point);
        }
    }

    int point_index = as_point_in_curve_index(p, temp_mesh.points);

    if (point_index > -1) {
        ada_remove(Point, temp_mesh.points, point_index);
        for (size_t edge_index = 0; edge_index < temp_mesh.edges.length; edge_index++) {
            if (temp_mesh.edges.elements[edge_index].p1_index > (size_t)point_index) {
                temp_mesh.edges.elements[edge_index].p1_index -= 1;
            }
            if (temp_mesh.edges.elements[edge_index].p2_index > (size_t)point_index) {
                temp_mesh.edges.elements[edge_index].p2_index -= 1;
            }
        }
    }

    *mesh = temp_mesh;
}

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

void as_tri_edge_implicit_mesh_delete_edge(Tri_edge_implicit_mesh *mesh, Point p1, Point p2)
{
    Tri_edge_implicit_mesh temp_mesh = *mesh;
    int edge_index = 0, inv_edge_index = 0;
    as_tri_edge_implicit_mesh_remove_edge(&temp_mesh, p1, p2, &edge_index, &inv_edge_index);

    // int edge_index = as_edge_implicit_ada_get_edge_index(temp_mesh.edges, temp_mesh.points.elements, p1, p2);
    if (edge_index > -1) {
        ada_remove(Edge_implicit, temp_mesh.edges, edge_index);
        for (size_t tri_index = 0; tri_index < temp_mesh.triangles.length; tri_index++) {
            for (size_t i = 0; i < 3; i++) {
                if (temp_mesh.triangles.elements[tri_index].edges_index[i] > (size_t)edge_index) {
                    temp_mesh.triangles.elements[tri_index].edges_index[i] -= 1;
                }
            }
        }
        if (inv_edge_index > edge_index) {
            inv_edge_index-= 1;
        }
    }

    // int inv_edge_index = as_edge_implicit_ada_get_edge_index(temp_mesh.edges, temp_mesh.points.elements, p2, p1);
    if (inv_edge_index > -1) {
        ada_remove(Edge_implicit, temp_mesh.edges, inv_edge_index);
        for (size_t tri_index = 0; tri_index < temp_mesh.triangles.length; tri_index++) {
            for (size_t i = 0; i < 3; i++) {
                if (temp_mesh.triangles.elements[tri_index].edges_index[i] > (size_t)inv_edge_index) {
                    temp_mesh.triangles.elements[tri_index].edges_index[i] -= 1;
                }
            }
        }
    }

    *mesh = temp_mesh;

}

void as_tri_edge_implicit_mesh_remove_edge(Tri_edge_implicit_mesh *mesh, Point p1, Point p2, int *edge_index_out, int *inv_edge_index_out)
{
    Tri_edge_implicit_mesh temp_mesh = *mesh;

    int edge_index = as_edge_implicit_ada_get_edge_index(mesh->edges, mesh->points.elements, p1, p2);
    int inv_edge_index = as_edge_implicit_ada_get_edge_index(mesh->edges, mesh->points.elements, p2, p1);

    size_t tri_out1_index = {0};
    size_t tri_out2_index = {0};
    int num_of_tri = as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge_index(temp_mesh, edge_index, inv_edge_index, &tri_out1_index, &tri_out2_index);

    if (num_of_tri == 1) {
        as_tri_edge_implicit_mesh_remove_triangle(&temp_mesh, tri_out1_index);
    } else if (num_of_tri == 2) {
        as_tri_edge_implicit_mesh_remove_triangle(&temp_mesh, tri_out1_index);
        num_of_tri = as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge_index(temp_mesh, edge_index, inv_edge_index, &tri_out1_index, &tri_out2_index);
        // if (num_of_tri > 2) {
        //     fprintf(stderr, "%s:%d:\n[Warning] edge implicit mesh has an edge with more then two triangles\n\n", __FILE__, __LINE__);
        //     exit(1);
        // }
        as_tri_edge_implicit_mesh_remove_triangle(&temp_mesh, tri_out1_index);
    }

    // if (edge_index != -1) ada_remove(Edge_implicit, temp_mesh.edges, edge_index);
    // if (inv_edge_index != -1) ada_remove(Edge_implicit, temp_mesh.edges, inv_edge_index);

    if (edge_index_out) *edge_index_out = edge_index;
    if (inv_edge_index_out) *inv_edge_index_out = inv_edge_index;

    (void)tri_out2_index;

    *mesh = temp_mesh;
}

void as_tri_edge_implicit_mesh_remove_triangle(Tri_edge_implicit_mesh *mesh, size_t tri_index_to_remove)
{
    AS_ASSERT(tri_index_to_remove < mesh->triangles.length && "tri OOB");
    Tri_edge_implicit_mesh temp_mesh = *mesh;

    ada_remove(Tri_edge_implicit, temp_mesh.triangles, tri_index_to_remove);
    for (size_t tri_index = 0; tri_index < temp_mesh.triangles.length; tri_index++) {
        for (size_t i = 0; i < 3; i++) {
            if ((temp_mesh.triangles.elements[tri_index].neighbor_tri_index[i] > (int)tri_index_to_remove) && (temp_mesh.triangles.elements[tri_index].neighbor_tri_index[i] > -1)) {
                temp_mesh.triangles.elements[tri_index].neighbor_tri_index[i] -= 1;
            } else if (temp_mesh.triangles.elements[tri_index].neighbor_tri_index[i] == (int)tri_index_to_remove) {
                temp_mesh.triangles.elements[tri_index].neighbor_tri_index[i] = -1;
            }
        }
    }

    *mesh = temp_mesh;
}

int as_tri_edge_implicit_mesh_set_neighbor_of_tri(Tri_edge_implicit_mesh mesh, size_t tri_index)
{
    Tri_edge_implicit current_tri = mesh.triangles.elements[tri_index];
    int num_of_neighbors = 0;
    for (size_t i = 0; i < 3; i++) {
        size_t tri_index1 = 0, tri_index2 = 0;
        size_t e_index = current_tri.edges_index[i];
        Edge_implicit ei = mesh.edges.elements[e_index];
        /* find inverse edge index once */
        int inv_ei_index = -1;
        /* check if next edge is the inv edge */
        if (e_index < mesh.edges.length-1 && e_index >= 1) {
            Edge_implicit next_ei = mesh.edges.elements[e_index+1];
            Edge_implicit prev_ei = mesh.edges.elements[e_index-1];
            if ((ei.p1_index == next_ei.p2_index) && (ei.p2_index == next_ei.p1_index)) {
                inv_ei_index = e_index + 1;
            } else if ((ei.p1_index == prev_ei.p2_index) && (ei.p2_index == prev_ei.p1_index)) {
                inv_ei_index = e_index - 1;
            }
        }
        if (inv_ei_index == -1) inv_ei_index = as_edge_implicit_ada_get_edge_index( mesh.edges, mesh.points.elements, mesh.points.elements[ei.p2_index], mesh.points.elements[ei.p1_index]);

        int num_of_tri = as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge_index(mesh, e_index, inv_ei_index, &tri_index1, &tri_index2);

        AS_ASSERT(num_of_tri > 0 && "Triangle has an incorrect edge index.");

        if (num_of_tri == 1) { /* tri does not have a neighbor through this edge. */
            mesh.triangles.elements[tri_index].neighbor_tri_index[i] = -1;
        }
        if (num_of_tri == 2) {
            if (tri_index == tri_index1) {
                mesh.triangles.elements[tri_index].neighbor_tri_index[i] = (int)tri_index2;
            } else if (tri_index == tri_index2) {
                mesh.triangles.elements[tri_index].neighbor_tri_index[i] = (int)tri_index1;
            } else {
                AS_ASSERT(1 && "unreachable. Triangle has an edge how doesn't belong to him.");
            }
            num_of_neighbors++;
        }
    }

    return num_of_neighbors;
}

void as_tri_edge_implicit_mesh_set_all_tri_neighbor(Tri_edge_implicit_mesh mesh)
{
    for (size_t tri_index = 0; tri_index < mesh.triangles.length; tri_index++) {
        as_tri_edge_implicit_mesh_set_neighbor_of_tri(mesh, tri_index);
    }
}

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

Tri_mesh as_circle_tri_mesh_create_lexicographically(const Point center, const float r, const size_t num_of_points, const uint32_t color, float light_intensity, const char plane[])
{
    Curve temp_circle = as_circle_curve_create(center, r, num_of_points, color, plane);
    Tri_implicit_mesh temp_implicit_mesh = as_points_array_get_lexicographic_triangulation(temp_circle.elements, temp_circle.length);

    free(temp_circle.elements);

    Tri_mesh mesh =  as_tri_implicit_mesh_to_tri_mesh(temp_implicit_mesh, light_intensity, color);

    as_tri_implicit_mesh_free(temp_implicit_mesh);

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
