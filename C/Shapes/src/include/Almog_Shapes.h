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
    size_t neighbor_tri_index[3];
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


#define AS_EPSILON 1e-6


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
#define                 as_points_equal_approx(p1, p2) (fabs((p1).x - (p2).x) < AS_EPSILON && fabs((p1).y - (p2).y) < AS_EPSILON && fabs((p1).z - (p2).z) < AS_EPSILON)
#define                 as_tri_area_xy(p1, p2, p3) 0.5 * ((p2).x-(p1).x)*((p3).y-(p1).y)- 0.5 * ((p3).x-(p1).x)*((p2).y-(p1).y)
#define                 as_tri_equal_z(tri) ((tri.points[0].z == tri.points[1].z) && (tri.points[1].z == tri.points[2].z) && (tri.points[2].z == tri.points[0].z))
#define                 as_tri_implicit_mesh_expand_tri_to_points(tim, t_index) (tim).points.elements[(tim).triangles.elements[t_index].points_index[0]], (tim).points.elements[(tim).triangles.elements[t_index].points_index[1]], (tim).points.elements[(tim).triangles.elements[t_index].points_index[2]]
#define                 as_tri_implicit_mesh_get_point_of_tri_implicit(tim, t_index, p_index) (tim).points.elements[(tim).triangles.elements[t_index].points_index[p_index]]
#define                 as_tri_implicit_area_xy(tim, t_index) as_tri_area_xy(as_tri_implicit_mesh_get_point_of_tri_implicit(tim, t_index, 0), as_tri_implicit_mesh_get_point_of_tri_implicit(tim, t_index, 1), as_tri_implicit_mesh_get_point_of_tri_implicit(tim, t_index, 2))
#define                 as_tri_implicit_mesh_get_tri_implicit(tim, t_index) (tim).triangles.elements[t_index]
#define                 as_tri_edge_implicit_mesh_get_point_of_tri(teim, t_index, p_index) (teim).points.elements[(teim).edges.elements[(teim).triangles.elements[t_index].edges_index[p_index]].p1_index]
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
int                     as_edge_implicit_ada_get_edge_index(Edge_implicit_ada eia, Point *points, Point p1, Point p2);
size_t                  as_factorial(size_t n);
Point                   as_mat2D_to_point(Mat2D m);
float                   as_orient2d(Point a, Point b, Point c);
void                    as_point_to_mat2D(Point p, Mat2D m);
size_t                  as_point_in_curve_occurrences(Point p, Curve c);
int                     as_point_in_curve_index(Point p, Curve c);
bool                    as_point_on_segment_xy(Point a, Point b, Point p, float eps);
float                   as_points_distance(Point p1, Point p2);
bool                    as_quad_is_convex(Point p1, Point p2, Point p3, Point p4);
float                   as_rand_float(void);
bool                    as_segment_hits_any_mesh_point(Tri_edge_implicit_mesh mesh, Point a, Point b, float eps, size_t *hit_index);
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
void                    as_tri_edge_implicit_mesh_copy(Tri_edge_implicit_mesh *des, Tri_edge_implicit_mesh src);
int                     as_tri_edge_implicit_mesh_get_triangles_with_edge(Tri_edge_implicit_mesh mesh, Point p1, Point p2, Tri_edge_implicit *tri_out1, Tri_edge_implicit *tri_out2);
int                     as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge(Tri_edge_implicit_mesh mesh, Point p1, Point p2, size_t *tri_out1_index, size_t *tri_out2_index);
int                     as_tri_edge_implicit_mesh_get_third_points_from_edge(Tri_edge_implicit_mesh mesh, Point p1, Point p2, Point *tri1_third_point, Point *tri2_third_point);
void                    as_tri_edge_implicit_mesh_delete_point(Tri_edge_implicit_mesh *mesh, Point p);
void                    as_tri_edge_implicit_mesh_remove_point(Tri_edge_implicit_mesh *mesh, Point p);
void                    as_tri_edge_implicit_mesh_delete_edge(Tri_edge_implicit_mesh *mesh, Point p1, Point p2);
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
void                    as_tri_get_incircle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r);
void                    as_tri_get_min_containment_circle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r);
bool                    as_tri_implicit_mesh_check_Delaunay(Tri_implicit_mesh mesh);
int                     as_tri_implicit_mesh_check_edge_is_locally_Delaunay(Tri_implicit_mesh mesh, Point p1, Point p2);
void                    as_tri_implicit_mesh_flip_edge(Tri_implicit_mesh mesh, Point p1, Point p2);
Tri_implicit_mesh       as_tri_implicit_mesh_make_Delaunay_triangulation_flip_algorithm_fixed_iterations(Point *c, const size_t len);
void                    as_tri_implicit_mesh_set_Delaunay_triangulation_flip_algorithm_fixed_iterations(Tri_implicit_mesh mesh);
bool                    as_tri_edge_implicit_mesh_check_Delaunay(Tri_edge_implicit_mesh mesh);
int                     as_tri_edge_implicit_mesh_check_edge_is_locally_Delaunay(Tri_edge_implicit_mesh mesh, Point p1, Point p2);
Edge_implicit           as_tri_edge_implicit_mesh_flip_edge(Tri_edge_implicit_mesh *mesh, Point p1, Point p2, bool debug_print);
void                    as_tri_edge_implicit_mesh_insert_segment(Tri_edge_implicit_mesh *mesh, Point p1, Point p2);
Tri_edge_implicit_mesh  as_tri_edge_implicit_mesh_make_Delaunay_triangulation_flip_algorithm(Point *c, const size_t len);
Tri_edge_implicit_mesh  as_tri_edge_implicit_mesh_make_Delaunay_triangulation_flip_algorithm_fixed_iterations(Point *c, const size_t len);
void                    as_tri_edge_implicit_mesh_set_Delaunay_triangulation_flip_algorithm(Tri_edge_implicit_mesh *mesh);
void                    as_tri_edge_implicit_mesh_set_Delaunay_triangulation_flip_algorithm_fixed_iterations(Tri_edge_implicit_mesh *mesh);

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
#define                 AS_EDGE_IMPLICIT_PRINT(ei, points) as_edge_implicit_print(ei, points, #ei, 0)
#define                 AS_EDGE_IMPLICIT_ADA_PRINT(ei_ada, points) as_edge_implicit_ada_print(ei_ada, points, #ei_ada, 0)
#define                 AS_TRI_MESH_PRINT(tm) as_tri_mesh_print(tm, #tm, 0)
#define                 AS_TRI_IMPLICIT_PRINT(t, points) as_tri_implicit_print(t, points, #t, 0)
#define                 AS_TRI_IMPLICIT_MESH_PRINT(tm) as_tri_implicit_mesh_print(tm, #tm, 0)
#define                 AS_TRI_EDGE_IMPLICIT_PRINT(tm, tri_index) as_tri_edge_implicit_print(tm, tri_index, #tm, 0)
#define                 AS_TRI_EDGE_IMPLICIT_MESH_PRINT(tm) as_tri_edge_implicit_mesh_print(tm, #tm, 0)
#define                 AS_TRI_EDGE_IMPLICIT_MESH_PRINT_SEGMENTS(tm) as_tri_edge_implicit_mesh_print_segments(tm, #tm, 0)

/** @brief Free a Curve_ada and all contained point arrays.
 *
 * Frees each inner Curve's points buffer and then the curves array.
 * The Curve_ada itself is passed by value.
 *
 * @param curves Curve_ada whose internal allocations will be freed.
 */
void as_curve_ada_free(Curve_ada curves)
{
    for (size_t i = 0; i < curves.length; i++) {
        free(curves.elements[i].elements);
    }
    free(curves.elements);
}

/** @brief Initialize an empty Tri_implicit_mesh.
 *
 * Allocates dynamic arrays for points and triangles with length = 0.
 * Must be freed with as_tri_implicit_mesh_free.
 *
 * @return Newly initialized empty Tri_implicit_mesh.
 */
Tri_implicit_mesh as_tri_implicit_mesh_init(void)
{
    Tri_implicit_mesh mesh = {0};

    ada_init_array(Point, mesh.points);
    ada_init_array(Tri_implicit, mesh.triangles);

    return mesh;
}

/** @brief Free internal allocations of a Tri_implicit_mesh.
 *
 * Frees the points and triangles arrays. The mesh itself is passed by
 * value.
 *
 * @param mesh Tri_implicit_mesh whose internal arrays will be freed.
 */
void as_tri_implicit_mesh_free(Tri_implicit_mesh mesh)
{
    free(mesh.points.elements);
    free(mesh.triangles.elements);
}

/** @brief Initialize an empty Tri_edge_implicit_mesh.
 *
 * Allocates dynamic arrays for points, edges, and triangles with
 * length = 0. Must be freed with as_tri_edge_implicit_mesh_free.
 *
 * @return Newly initialized empty Tri_edge_implicit_mesh.
 */
Tri_edge_implicit_mesh as_tri_edge_implicit_mesh_init(void)
{
    Tri_edge_implicit_mesh mesh = {0};

    ada_init_array(Point, mesh.points);
    ada_init_array(Edge_implicit, mesh.edges);
    ada_init_array(Tri_edge_implicit, mesh.triangles);

    return mesh;
}

/** @brief Free internal allocations of a Tri_edge_implicit_mesh.
 *
 * Frees the points, edges, and triangles arrays. The mesh itself is
 * passed by value.
 *
 * @param mesh Tri_edge_implicit_mesh whose internal arrays will be
 *             freed.
 */
void as_tri_edge_implicit_mesh_free(Tri_edge_implicit_mesh mesh)
{
    free(mesh.points.elements);
    free(mesh.edges.elements);
    free(mesh.triangles.elements);
}

/** @brief Print a Point with a label and optional left padding.
 *
 * @param p       The point to print.
 * @param name    Label to print before the point.
 * @param padding Number of leading spaces to indent.
 */
void as_point_print(Point p, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);
    printf("%*s    (%f, %f, %f)\n", (int) padding, "", p.x, p.y, p.z);
}

/** @brief Print a Curve (list of points and color) with a label.
 *
 * @param c       The curve to print.
 * @param name    Label to print before the curve.
 * @param padding Number of leading spaces to indent.
 */
void as_curve_print(Curve c, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);
    for (size_t i = 0; i < c.length; i++) {
        printf("%*s    point%zu: (%f, %f, %f)\n", (int) padding, "", i, c.elements[i].x, c.elements[i].y, c.elements[i].z);
    }
    printf("%*s    color: %X\n", (int)padding, "", c.color);
}

/** @brief Print a single implicit edge by resolving endpoint indices.
 *
 * @param ei      The edge to print (stores global point indices).
 * @param points  The global points array the indices refer to.
 * @param name    Label to print before the edge.
 * @param padding Number of leading spaces to indent.
 */
void as_edge_implicit_print(Edge_implicit ei, Point *points, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);
    printf("%*s    (%f, %f, %f)\n", (int) padding+4, "", as_point_expand_to_xyz(points[ei.p1_index]));
    printf("%*s    (%f, %f, %f)\n", (int) padding+4, "", as_point_expand_to_xyz(points[ei.p2_index]));
    printf("%*s    (%zu -> %zu)\n", (int) padding+4, "", ei.p1_index, ei.p2_index);
    printf("%*s    segment = %d\n", (int) padding+4, "", ei.is_segment);
}

/** @brief Print all edges in an Edge_implicit_ada with coordinates.
 *
 * @param ei_ada  List of edges (indices refer to points[]).
 * @param points  The global points array the indices refer to.
 * @param name    Label to print before the list.
 * @param padding Number of leading spaces to indent.
 */
void as_edge_implicit_ada_print(Edge_implicit_ada ei_ada, Point *points, char *name, size_t padding)
{
    char temp_str[256];
    printf("%*s%s:\n", (int) padding, "", name);

    for (size_t i = 0; i < ei_ada.length; i++) {
        snprintf(temp_str, 256, "edge %zu", i);
        as_edge_implicit_print(ei_ada.elements[i], points, temp_str, padding+4);
    }
}

/** @brief Print a Tri (points, normals, colors, light) with a label.
 *
 * @param tri     The triangle to print.
 * @param name    Label to print before the triangle.
 * @param padding Number of leading spaces to indent.
 */
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

/** @brief Print all triangles in a Tri_mesh with a label.
 *
 * @param mesh    The triangle mesh to print.
 * @param name    Label to print before the mesh.
 * @param padding Number of leading spaces to indent.
 */
void as_tri_mesh_print(Tri_mesh mesh, char *name, size_t padding)
{
    char tri_name[256];
    printf("%*s%s:\n", (int) padding, "", name);
    for (size_t i = 0; i < mesh.length; i++) {
        snprintf(tri_name, 256, "tri %zu", i);
        as_tri_print(mesh.elements[i], tri_name, 4);
    }
}

/** @brief Print a Tri_implicit by resolving its point indices.
 *
 * @param tri_imp The implicit triangle to print.
 * @param points  The global points array the indices refer to.
 * @param name    Label to print before the triangle.
 * @param padding Number of leading spaces to indent.
 */
void as_tri_implicit_print(Tri_implicit tri_imp, Point *points, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);
    printf("%*s    points:\n", (int)padding, "");
    printf("%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n", (int) padding, "", points[tri_imp.points_index[0]].x, points[tri_imp.points_index[0]].y, points[tri_imp.points_index[0]].z, (int) padding, "", points[tri_imp.points_index[1]].x, points[tri_imp.points_index[1]].y, points[tri_imp.points_index[1]].z, (int) padding, "", points[tri_imp.points_index[2]].x, points[tri_imp.points_index[2]].y, points[tri_imp.points_index[2]].z);
}

/** @brief Print a Tri_implicit_mesh (points and triangles).
 *
 * @param mesh    The implicit mesh to print.
 * @param name    Label to print before the mesh.
 * @param padding Number of leading spaces to indent.
 */
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

/** @brief Print a single triangle from a Tri_edge_implicit_mesh.
 *
 * Tri vertices are resolved via each triangle's three edges which hold
 * global point indices.
 *
 * @param mesh     The edge-implicit mesh.
 * @param tri_index Global index of the triangle to print.
 * @param name     Label to print before the triangle.
 * @param padding  Number of leading spaces to indent.
 */
void as_tri_edge_implicit_print(Tri_edge_implicit_mesh mesh, size_t tri_index, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);

    printf("%*s    points:\n", (int)padding+4, "");
    for (size_t j = 0; j < 3; j++) {
        printf("%*s    (%f, %f, %f)\n", (int) padding+4, "", as_tri_edge_implicit_mesh_expand_point_of_tri_to_xyz(mesh, tri_index, j));
    }
}

/** @brief Print a Tri_edge_implicit_mesh (points, edges, triangles).
 *
 * @param mesh    The edge-implicit mesh to print.
 * @param name    Label to print before the mesh.
 * @param padding Number of leading spaces to indent.
 */
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
        printf("%*s    points:\n", (int)padding+4, "");
        for (size_t j = 0; j < 3; j++) {
            printf("%*s    (%f, %f, %f)\n", (int) padding+4, "", as_tri_edge_implicit_mesh_expand_point_of_tri_to_xyz(mesh, i, j));
        }
    }
}

/** @brief Print only edges marked as segments (constrained edges).
 *
 * @param mesh    The edge-implicit mesh.
 * @param name    Label to print before the list.
 * @param padding Number of leading spaces to indent.
 */
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

/** @brief Test AABB overlap of two segments (a,b) and (c,d) in XY.
 *
 * @param a First segment endpoint.
 * @param b First segment endpoint.
 * @param c Second segment endpoint.
 * @param d Second segment endpoint.
 * @return true if the XY bounding boxes overlap; false otherwise.
 */
bool as_bbox_overlap(Point a, Point b, Point c, Point d)
{
    float min_ax = fminf(a.x, b.x), max_ax = fmaxf(a.x, b.x);
    float min_ay = fminf(a.y, b.y), max_ay = fmaxf(a.y, b.y);
    float min_cx = fminf(c.x, d.x), max_cx = fmaxf(c.x, d.x);
    float min_cy = fminf(c.y, d.y), max_cy = fmaxf(c.y, d.y);
    return !(max_ax < min_cx || max_cx < min_ax ||
             max_ay < min_cy || max_cy < min_ay);
}

/** @brief Compute binomial coefficient "n choose k".
 *
 * Simple integer implementation; returns 0 for invalid inputs (n < k or
 * negative). May overflow for large n, k.
 *
 * @param n Non-negative integer.
 * @param k Non-negative integer (k <= n).
 * @return The binomial coefficient as size_t (undefined on overflow).
 */
size_t as_choose(int n, int k)
{
    if (n < k || n < 0 || k < 0) {
        return 0;
    }

    int ans = 1;

    for (int i = n; i >= n-k+1; i--) {
        ans *= i;
    }
    for (int i = k; i >= 1; i--) {
        ans /= i;
    }

    return (size_t)ans;
}

/** @brief Linearly interpolate between two ARGB colors.
 *
 * Expects t in [0,1]. t=0 yields c1, t=1 yields c2. Colors are packed
 * ARGB in 0xAARRGGBB.
 *
 * @param c1 First color (ARGB).
 * @param c2 Second color (ARGB).
 * @param t  Interpolation factor in [0,1].
 * @return Interpolated color (ARGB).
 */
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

/** @brief Create a Curve of random points inside an AABB.
 *
 * Uses the provided seed to seed rand(). Coordinates are uniform over
 * [min,max] per axis.
 *
 * @param num_of_points Number of points to generate.
 * @param min_x Minimum x.
 * @param max_x Maximum x.
 * @param min_y Minimum y.
 * @param max_y Maximum y.
 * @param min_z Minimum z.
 * @param max_z Maximum z.
 * @param seed  Seed for rand().
 * @return Curve with points; color is left at default (0).
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

// bool as_edge_intersect_edge(Point p11, Point p12, Point p21, Point p22)
// {
//     return (((as_edge_cross_edge(p11, p12, p12, p22) * as_edge_cross_edge(p11, p12, p12, p21)) < 0) && ((as_edge_cross_edge(p21, p22, p22, p12) * as_edge_cross_edge(p21, p22, p21, p11)) < 0));
// }

/** @brief Robust proper intersection test between two 2D segments.
 *
 * Tests segments (p11,p12) and (p21,p22) in the XY plane. Touching at
 * endpoints is not considered an intersection.
 *
 * @param p11 First segment endpoint.
 * @param p12 First segment endpoint.
 * @param p21 Second segment endpoint.
 * @param p22 Second segment endpoint.
 * @return true if segments properly intersect; false otherwise.
 */
bool as_edge_intersect_edge(Point p11, Point p12, Point p21, Point p22)
{
    /* Exclude shared endpoints (do not treat as intersecting) */
    if (as_points_equal_approx(p11, p21) || as_points_equal_approx(p11, p22) ||
        as_points_equal_approx(p12, p21) || as_points_equal_approx(p12, p22)) {
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

/** @brief Return index of the directed edge (p1->p2) or -1 if absent.
 *
 * Matches by equality (as_points_equal) against the
 * points referenced by edge indices.
 *
 * @param eia    Edge array to search.
 * @param points Global points array.
 * @param p1     First endpoint.
 * @param p2     Second endpoint.
 * @return Edge index in eia, or -1 if not found.
 */
int as_edge_implicit_ada_get_edge_index(Edge_implicit_ada eia, Point *points, Point p1, Point p2)
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

/** @brief Compute n! as size_t (no overflow checks).
 *
 * @param n Non-negative integer.
 * @return Factorial of n.
 */
size_t as_factorial(size_t n)
{
    size_t fact = 1, i;

    for (i = 1; i <= n; i++) {
        fact *= i;
    }

    return fact;
}

/** @brief Convert a 3x1 or 1x3 Mat2D to a Point (w=1).
 *
 * @param m Input matrix.
 * @return Point with x,y,z from m and w=1.
 */
Point as_mat2D_to_point(Mat2D m)
{
    Point res = {.x = MAT2D_AT(m, 0, 0), .y = MAT2D_AT(m, 1, 0), .z = MAT2D_AT(m, 2, 0), .w = 1};
    return res;
}

/** @brief Twice the signed area/orientation of triangle (a,b,c) in XY.
 *
 * >0 for left turn, <0 for right turn, 0 for collinear.
 *
 * @param a Point a.
 * @param b Point b.
 * @param c Point c.
 * @return Signed area*2.
 */
float as_orient2d(Point a, Point b, Point c)
{
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

/** @brief Store Point (x,y,z) into a 3x1 or 1x3 Mat2D.
 *
 * Asserts the matrix has compatible shape.
 *
 * @param p Point to store.
 * @param m Destination matrix.
 */
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

/** @brief Count exact occurrences of Point p in Curve c.
 *
 * Uses exact equality (as_points_equal).
 *
 * @param p Point to count.
 * @param c Curve to scan.
 * @return Number of occurrences.
 */
size_t as_point_in_curve_occurrences(Point p, Curve c)
{
    size_t num_of_occurrences = 0;

    for (size_t i = 0; i < c.length; i++) {
        if (as_points_equal(c.elements[i], p)) num_of_occurrences++;
    }

    return num_of_occurrences;
}

/** @brief Return last index of Point p in Curve c, or -1 if absent.
 *
 * Uses exact equality (as_points_equal).
 *
 * @param p Point to find.
 * @param c Curve to scan.
 * @return Last index of p, or -1 if not found.
 */
int as_point_in_curve_index(Point p, Curve c)
{
    int index = -1;

    for (size_t i = 0; i < c.length; i++) {
        if (as_points_equal(c.elements[i], p)) index = i;
    }

    return index;
}

/** @brief Test if p lies on open segment ab in XY within tolerance.
 *
 * Endpoints are excluded. eps is in world units.
 *
 * @param a   Segment endpoint.
 * @param b   Segment endpoint.
 * @param p   Query point.
 * @param eps Tolerance in world units.
 * @return true if p lies strictly inside segment ab; false otherwise.
 */
bool as_point_on_segment_xy(Point a, Point b, Point p, float eps)
{
    /* Quick bbox reject */
    if (!as_bbox_overlap(a, b, p, p)) return false;

    /* Vector ab */
    float abx = b.x - a.x;
    float aby = b.y - a.y;

    /* If segment is degenerate, nothing to do */
    float ab_len2 = abx * abx + aby * aby;
    if (ab_len2 <= 0.0f) return false;

    /* Signed area (twice triangle area) -> collinearity check */
    float area = as_orient2d(a, b, p); /* = cross(ab, ap) */
    if (fabsf(area) > eps * sqrtf(ab_len2)) {
        return false;
    }

    /* Parametric projection t of p on segment ab: a + t*(ab) */
    float apx = p.x - a.x;
    float apy = p.y - a.y;
    float t = (apx * abx + apy * aby) / ab_len2;

    /* Exclude endpoints: require t strictly inside (0,1) with margin */
    if (t <= eps || t >= 1.0f - eps) return false;

    /* Also check perpendicular distance explicitly vs eps (optional, but
       helps when ab is long and area test alone is too permissive) */
    float dist_perp = fabsf(area) / sqrtf(ab_len2);
    if (dist_perp > eps) return false;

    return true;
}

/** @brief Euclidean distance between two Points (x,y,z).
 *
 * @param p1 First point.
 * @param p2 Second point.
 * @return Distance between p1 and p2.
 */
float as_points_distance(Point p1, Point p2)
{
    return sqrtf((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
}
// #define as_points_distance(p1, p2) sqrtf(((p1).x - (p2).x) * ((p1).x - (p2).x) + ((p1).y - (p2).y) * ((p1).y - (p2).y) + ((p1).z - (p2).z) * ((p1).z - (p2).z));

/** @brief Test if quadrilateral (p1,p2,p3,p4) is convex in XY.
 *
 * Points are taken in order around the quad.
 *
 * @param p1 First vertex.
 * @param p2 Second vertex.
 * @param p3 Third vertex.
 * @param p4 Fourth vertex.
 * @return true if convex; false otherwise.
 */
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

/** @brief Random float in [0,1] using rand().
 *
 * @return A float in [0,1].
 */
float as_rand_float(void)
{
    return (float) rand() / (float) RAND_MAX;
}

/** @brief Check if segment (a,b) hits any mesh point (except ends).
 *
 * Tests in XY within eps. Optionally returns the hit index.
 *
 * @param mesh      Edge-implicit mesh containing points.
 * @param a         Segment endpoint.
 * @param b         Segment endpoint.
 * @param eps       Tolerance in world units.
 * @param hit_index Optional out-parameter for the offending point (if not NULL).
 * @return true if any point lies strictly on segment; false otherwise.
 */
bool as_segment_hits_any_mesh_point(Tri_edge_implicit_mesh mesh, Point a, Point b, float eps, size_t *hit_index)
{
    for (size_t i = 0; i < mesh.points.length; i++) {
        Point q = mesh.points.elements[i];

        /* Skip endpoints (treat equal within eps) */
        if (as_points_equal_approx(q, a) || as_points_equal_approx(q, b)) {
            continue;
        }

        if (as_point_on_segment_xy(a, b, q, eps)) {
            if (hit_index) *hit_index = i;
            return true;
        }
    }
    return false;
}

/** @brief Compute per-vertex normals for a Tri.
 *
 * Uses cross products of adjacent edges and normalizes results. Writes
 * normals into tri->normals.
 *
 * @param tri Triangle whose normals will be set in place.
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

/** @brief Convert an explicit Tri_mesh into a Tri_implicit_mesh.
 *
 * Deduplicates identical points and stores triangle point indices.
 *
 * @param mesh Explicit triangle mesh.
 * @return Equivalent implicit mesh (deduplicated points).
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

/** @brief Copy src Tri_mesh into des (resets des length first).
 *
 * Appends all triangles from src to des; grows capacity as needed.
 *
 * @param des Destination mesh (must be initialized).
 * @param src Source mesh to copy from.
 */
void as_tri_mesh_copy(Tri_mesh *des, Tri_mesh src)
{
    Tri_mesh temp_des = *des;
    temp_des.length = 0;
    for (size_t i = 0; i < src.length; i++) {
        ada_appand(Tri, temp_des, src.elements[i]);
    }
    *des = temp_des;
}

/** @brief Subdivide each triangle into 4 using edge midpoints.
 *
 * Interpolates colors and light intensities and recomputes normals.
 * Modifies the mesh in place.
 *
 * @param mesh Tri_mesh to subdivide in place.
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

/** @brief If all points in mesh share the same x, return that x.
 *
 * @param mesh Triangle mesh to test.
 * @return The constant x, or NAN if not constant.
 */
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

/** @brief If all points in mesh share the same y, return that y.
 *
 * @param mesh Triangle mesh to test.
 * @return The constant y, or NAN if not constant.
 */
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

/** @brief If all points in mesh share the same z, return that z.
 *
 * @param mesh Triangle mesh to test.
 * @return The constant z, or NAN if not constant.
 */
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

/** @brief Convert Tri_implicit and points[] to explicit Tri.
 *
 * Also computes per-vertex normals.
 *
 * @param tri_implicit Triangle in index form.
 * @param points       Global points array.
 * @return Explicit Tri with geometry and normals.
 */
Tri as_tri_implicit_to_tri(Tri_implicit tri_implicit, Point *points)
{
    Tri tri = {0};

    tri.points[0] = points[tri_implicit.points_index[0]];
    tri.points[1] = points[tri_implicit.points_index[1]];
    tri.points[2] = points[tri_implicit.points_index[2]];

    as_tri_set_normals(&tri);

    return tri;
}

/** @brief If all points in Tri_implicit_mesh share the same x, return it.
 *
 * @param mesh Implicit mesh to test.
 * @return The constant x, or NAN if not constant.
 */
float as_tri_implicit_mesh_const_x(Tri_implicit_mesh mesh)
{
    float value = 0;

    for (size_t p_index = 0; p_index < mesh.points.length; p_index++) {
        if (p_index == 0) value = mesh.points.elements[p_index].x;

        if (value != mesh.points.elements[p_index].x) return NAN;
    }

    return value;
}

/** @brief If all points in Tri_implicit_mesh share the same y, return it.
 *
 * @param mesh Implicit mesh to test.
 * @return The constant y, or NAN if not constant.
 */
float as_tri_implicit_mesh_const_y(Tri_implicit_mesh mesh)
{
    float value = 0;

    for (size_t p_index = 0; p_index < mesh.points.length; p_index++) {
        if (p_index == 0) value = mesh.points.elements[p_index].y;

        if (value != mesh.points.elements[p_index].y) return NAN;
    }

    return value;
}

/** @brief If all points in Tri_implicit_mesh share the same z, return it.
 *
 * @param mesh Implicit mesh to test.
 * @return The constant z, or NAN if not constant.
 */
float as_tri_implicit_mesh_const_z(Tri_implicit_mesh mesh)
{
    float value = 0;

    for (size_t p_index = 0; p_index < mesh.points.length; p_index++) {
        if (p_index == 0) value = mesh.points.elements[p_index].z;

        if (value != mesh.points.elements[p_index].z) return NAN;
    }

    return value;
}

/** @brief Get up to two Tri_implicit that share edge (p1,p2).
 *
 * Triangles are returned by value via tri_out1/tri_out2.
 *
 * @param mesh        Implicit mesh.
 * @param p1          First endpoint.
 * @param p2          Second endpoint.
 * @param tri_out1    Output triangle 1 (if present).
 * @param tri_out2    Output triangle 2 (if present).
 * @return Number of triangles found (0..2).
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

/** @brief Get indices of up to two Tri_implicit that share edge (p1,p2).
 *
 * Outputs global triangle indices into mesh.triangles[].
 *
 * @param mesh            Implicit mesh.
 * @param p1              First endpoint.
 * @param p2              Second endpoint.
 * @param tri_out1_index  Out: first triangle index.
 * @param tri_out2_index  Out: second triangle index.
 * @return Number of triangles found (0..2).
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

/** @brief Convert Tri_implicit_mesh to explicit Tri_mesh.
 *
 * Assigns uniform color and light intensity; computes normals.
 *
 * @param implicit_mesh  Source implicit mesh.
 * @param light_intensity Per-vertex light intensity to assign.
 * @param color          Packed ARGB color (0xAARRGGBB).
 * @return Explicit Tri_mesh.
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

/** @brief Convert Tri_implicit_mesh to Tri_edge_implicit_mesh.
 *
 * Builds edges and references triangles by edge indices.
 *
 * @param implicit_mesh Source implicit mesh.
 * @return Edge-implicit mesh.
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

/** @brief Deep-copy a Tri_edge_implicit_mesh into an initialized dest.
 *
 * Clears destination arrays and appends all elements from source.
 *
 * @param des Destination mesh (must be initialized).
 * @param src Source mesh to copy from.
 */
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

/** @brief Get up to two Tri_edge_implicit that share edge (p1,p2).
 *
 * Triangles are returned by value via tri_out1/tri_out2.
 *
 * @param mesh     Edge-implicit mesh.
 * @param p1       First endpoint.
 * @param p2       Second endpoint.
 * @param tri_out1 Output triangle 1 (if present).
 * @param tri_out2 Output triangle 2 (if present).
 * @return Number of triangles found (0..2).
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

/** @brief Get indices of up to two triangles that share edge (p1,p2).
 *
 * Outputs global triangle indices into mesh.triangles[].
 *
 * @param mesh            Edge-implicit mesh.
 * @param p1              First endpoint.
 * @param p2              Second endpoint.
 * @param tri_out1_index  Out: first triangle index.
 * @param tri_out2_index  Out: second triangle index.
 * @return Number of triangles found (0..2).
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
                *tri_out1_index = tri_index;
                num_of_tri_out++;
            } else if (num_of_tri_out == 1) {
                *tri_out2_index = tri_index;
                num_of_tri_out++;
                /* if this line will be uncommented, then the check will never be reached */
                // return num_of_tri_out;
            } else if (num_of_tri_out > 2) {
                fprintf(stderr, "%s:%d: [Warning] edge implicit mesh has an edge with more then two triangles\n", __FILE__, __LINE__);
                exit(1);
            }
        }
    }

    return num_of_tri_out;
}

/** @brief Return the third vertex from up to two triangles on edge (p1,p2).
 *
 * For each triangle adjacent to the edge, returns the vertex not on the
 * edge. Writes up to two points.
 *
 * @param mesh             Edge-implicit mesh.
 * @param p1               First endpoint.
 * @param p2               Second endpoint.
 * @param tri1_third_point Out: third point of first triangle.
 * @param tri2_third_point Out: third point of second triangle.
 * @return Number of triangles found (0..2).
 */
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
                // return num_of_tri_out;
            } else if (num_of_tri_out > 2) {
                fprintf(stderr, "%s:%d: [Warning] edge implicit mesh has an edge with more then two triangles\n", __FILE__, __LINE__);
                exit(1);
            }
        }
    }

    return num_of_tri_out;
}

/** @brief Delete a point, its incident triangles, and incident edges.
 *
 * Compacts point and edge indices after removal. Modifies mesh in place.
 *
 * @param mesh Mesh to modify (in place).
 * @param p    Point to delete (exact match).
 */
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

/** @brief Remove all triangles that include point p.
 *
 * Leaves points/edges intact. Modifies mesh in place.
 *
 * @param mesh Mesh to modify (in place).
 * @param p    Point whose incident triangles are removed.
 */
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

/** @brief Delete edge (p1,p2) and its triangles; adjust indices.
 *
 * Removes triangles that use the edge in either direction and deletes
 * the edge and its reverse if present. Modifies mesh in place.
 *
 * @param mesh Mesh to modify (in place).
 * @param p1   First endpoint.
 * @param p2   Second endpoint.
 */
void as_tri_edge_implicit_mesh_delete_edge(Tri_edge_implicit_mesh *mesh, Point p1, Point p2)
{
    Tri_edge_implicit_mesh temp_mesh = *mesh;
    as_tri_edge_implicit_mesh_remove_edge(&temp_mesh, p1, p2);

    int edge_index = as_edge_implicit_ada_get_edge_index(temp_mesh.edges, temp_mesh.points.elements, p1, p2);
    if (edge_index > -1) {
        ada_remove(Edge_implicit, temp_mesh.edges, edge_index);
        for (size_t tri_index = 0; tri_index < temp_mesh.triangles.length; tri_index++) {
            for (size_t i = 0; i < 3; i++) {
                if (temp_mesh.triangles.elements[tri_index].edges_index[i] > (size_t)edge_index) {
                    temp_mesh.triangles.elements[tri_index].edges_index[i] -= 1;
                }
            }
        }
    }

    int inv_edge_index = as_edge_implicit_ada_get_edge_index(temp_mesh.edges, temp_mesh.points.elements, p2, p1);
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

/** @brief Remove all triangles that include edge (p1,p2).
 *
 * Does not remove the edge itself. Modifies mesh in place.
 *
 * @param mesh Mesh to modify (in place).
 * @param p1   First endpoint.
 * @param p2   Second endpoint.
 */
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

/** @brief Remove triangle at the given global index.
 *
 * @param mesh      Mesh to modify (in place).
 * @param tri_index Global triangle index to remove.
 */
void as_tri_edge_implicit_mesh_remove_triangle(Tri_edge_implicit_mesh *mesh, size_t tri_index)
{
    Tri_edge_implicit_mesh temp_mesh = *mesh;

    ada_remove(Tri_edge_implicit, temp_mesh.triangles, tri_index);

    *mesh = temp_mesh;
}

/** @brief Convert a Tri_edge_implicit_mesh into a Tri_mesh.
 *
 * Assigns uniform color and light intensity; computes normals.
 *
 * @param tei_mesh        Edge-implicit mesh.
 * @param light_intensity Per-vertex light intensity to assign.
 * @param color           Packed ARGB color (0xAARRGGBB).
 * @return Explicit Tri_mesh.
 */
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

/** @brief Convert a Tri_edge_implicit_mesh into a Tri_implicit_mesh.
 *
 * Resolves triangle vertices via their local edges' p1 endpoints.
 *
 * @param tei_mesh Source edge-implicit mesh.
 * @return Destination implicit mesh.
 */
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

/** @brief Compute convex hull (Jarvis march) in XY, including collinear.
 *
 * Writes result into conv. All input points must have the same z.
 *
 * @param conv   Out: convex hull as a Curve.
 * @param points Input points array.
 * @param len    Number of input points.
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

/** @brief Swap two points in an array.
 *
 * @param c      Point array.
 * @param index1 First index.
 * @param index2 Second index.
 */
void as_points_array_swap_points(Point *c, const size_t index1, const size_t index2)
{
    Point temp = c[index1];
    c[index1] = c[index2];
    c[index2] = temp;
}

/** @brief Sort points lexicographically by x, then y, then z (asc).
 *
 * @param c   Point array to sort in place.
 * @param len Number of points.
 */
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

/** @brief Create a lexicographic triangulation on XY-coplanar points.
 *
 * Deduplicates points and returns a Tri_implicit_mesh.
 *
 * @param points Input points (will be reordered).
 * @param len    Number of input points.
 * @return Implicit triangulation.
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
    printf("[INFO] lexicographic triangulation:\n");
    for (point_index = point_index+1; point_index < len; point_index++) {
        printf("\r       points: %zu | done: %f%%", point_index+1, 100.0f * (float)(point_index+1) / (float)(len));
        fflush(stdout);

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
    printf("\n");

    free(convex_hull.elements);

    return implicit_mesh;
}

/** @brief Compute circumcircle of triangle (p1,p2,p3) in XY.
 *
 * Asserts coplanarity and plane equals "XY" or "xy".
 *
 * @param p1     First vertex.
 * @param p2     Second vertex.
 * @param p3     Third vertex.
 * @param plane  Plane selector ("XY" only).
 * @param center Out: circle center.
 * @param r      Out: circle radius.
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

/** @brief Compute incircle of triangle (p1,p2,p3) in XY.
 *
 * Asserts coplanarity and plane equals "XY" or "xy".
 *
 * @param p1     First vertex.
 * @param p2     Second vertex.
 * @param p3     Third vertex.
 * @param plane  Plane selector ("XY" only).
 * @param center Out: circle center.
 * @param r      Out: circle radius.
 */
void as_tri_get_incircle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r)
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

/** @brief Minimal containment circle for triangle (p1,p2,p3) in XY.
 *
 * Returns either the circumcircle or the diameter of the longest side.
 * Asserts coplanarity and plane equals "XY" or "xy".
 *
 * @param p1     First vertex.
 * @param p2     Second vertex.
 * @param p3     Third vertex.
 * @param plane  Plane selector ("XY" only).
 * @param center Out: circle center.
 * @param r      Out: circle radius.
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

/** @brief Check if all interior non-segment edges in an implicit mesh are locally Delaunay.
 *
 * @param mesh Implicit mesh to test.
 * @return true if Delaunay; false otherwise.
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

/** @brief Test if edge (p1,p2) is locally Delaunay in implicit mesh.
 *
 * @param mesh Implicit mesh.
 * @param p1   First endpoint.
 * @param p2   Second endpoint.
 * @return -1 if not an edge, 0 if not locally Delaunay, 1 otherwise.
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

/** @brief Flip the shared edge (p1,p2) to the opposite diagonal.
 *
 * No-op with warnings if edge is not shared by exactly two triangles.
 *
 * @param mesh Implicit mesh to modify (in place).
 * @param p1   First endpoint.
 * @param p2   Second endpoint.
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

/** @brief Build Delaunay triangulation via fixed-iteration flips.
 *
 * Convenience wrapper that builds a lexicographic triangulation and
 * performs fixed iterations of edge flips.
 *
 * @param c   Input XY-coplanar points.
 * @param len Number of points.
 * @return Implicit Delaunay triangulation.
 */
Tri_implicit_mesh as_tri_implicit_mesh_make_Delaunay_triangulation_flip_algorithm_fixed_iterations(Point *c, const size_t len)
{
    Tri_implicit_mesh ti_lexi_mesh = as_points_array_get_lexicographic_triangulation(c, len);

    as_tri_implicit_mesh_set_Delaunay_triangulation_flip_algorithm_fixed_iterations(ti_lexi_mesh);

    return ti_lexi_mesh;
}

/** @brief Enforce Delaunay by repeated passes (fixed iteration cap).
 *
 * Modifies the mesh in place.
 *
 * @param mesh Implicit mesh to modify (in place).
 */
void as_tri_implicit_mesh_set_Delaunay_triangulation_flip_algorithm_fixed_iterations(Tri_implicit_mesh mesh)
{
    printf("[INFO] Delaunay triangulation:\n");

    int hard_limit = 10;
    size_t total_num_of_iteration_per_time = as_choose(mesh.points.length, 2) * hard_limit;
    for (int times = 0, counter = 0; times < hard_limit; times++) {
        for (size_t i = 0; i < mesh.points.length-1; i++) {
            for (size_t j = i+1; j < mesh.points.length; j++) {
                counter++;
                printf("\033[A\33[2K\r       edges checked: %d | done: %f%%\n", counter, 100.0f * (float)counter / (float)total_num_of_iteration_per_time);
                fflush(stdout);
    
                int num_of_tri_on_edge = as_tri_implicit_mesh_check_edge_is_locally_Delaunay(mesh, mesh.points.elements[i], mesh.points.elements[j]);
                if (num_of_tri_on_edge == -1) continue;
                if (num_of_tri_on_edge == 1) continue;
                as_tri_implicit_mesh_flip_edge(mesh, mesh.points.elements[i], mesh.points.elements[j]);
            }
        }
        if (as_tri_implicit_mesh_check_Delaunay(mesh)) break;
    }
    printf("\n");
}

/** @brief Check if all interior non-segment edges are locally Delaunay.
 *
 * @param mesh Edge-implicit mesh to test.
 * @return true if Delaunay; false otherwise.
 */
bool as_tri_edge_implicit_mesh_check_Delaunay(Tri_edge_implicit_mesh mesh)
{
    for (size_t i = 0; i < mesh.edges.length; i++) {
        int is_locally_Delaunay = as_tri_edge_implicit_mesh_check_edge_is_locally_Delaunay(mesh, mesh.points.elements[mesh.edges.elements[i].p1_index], mesh.points.elements[mesh.edges.elements[i].p2_index]);
        if (is_locally_Delaunay == -1) continue;
        if (is_locally_Delaunay == 1) continue;
        if (is_locally_Delaunay == 0) return false;
    }

    return true;
}

/** @brief Test if edge (p1,p2) is locally Delaunay in edge-implicit mesh.
 *
 * Constrained edges (is_segment) are treated as already valid.
 *
 * @param mesh Edge-implicit mesh.
 * @param p1   First endpoint.
 * @param p2   Second endpoint.
 * @return -1 if not an edge, 0 if not locally Delaunay, 1 otherwise.
 */
int as_tri_edge_implicit_mesh_check_edge_is_locally_Delaunay(Tri_edge_implicit_mesh mesh, Point p1, Point p2)
{
    // int p1_index = as_point_in_curve_index(p1, mesh.points);
    // int p2_index = as_point_in_curve_index(p2, mesh.points);

    // AS_ASSERT(p1_index != -1 && p2_index != -1);

    int edge_index = as_edge_implicit_ada_get_edge_index(mesh.edges, mesh.points.elements, p1, p2);
    if (edge_index == -1) return -1;
    if (mesh.edges.elements[edge_index].is_segment) return 1;
    
    size_t tri1_index = 0;
    size_t tri2_index = 0;

    int num_of_triangles = as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge(mesh, p1, p2, &tri1_index, &tri2_index);
    
    if (num_of_triangles == 0) return -1;
    if (num_of_triangles == 1) return 1;

    ADA_ASSERT(num_of_triangles == 2);

    Point circumcenter_1 = {0};
    float r1 = 0;

    as_tri_get_circumcircle(as_tri_edge_implicit_mesh_expand_tri_to_points(mesh, tri1_index), "xy", &circumcenter_1, &r1);

    Point tri2_outside_p = {0};
    for (size_t i = 0; i < 3; i++) {
        Point current_point = as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri2_index, i);
        if (!as_points_equal(current_point, p1) && !as_points_equal(current_point, p2)) tri2_outside_p = current_point;
    }

    float tri2_out_p_and_center_dis = as_points_distance(tri2_outside_p, circumcenter_1);

    if (tri2_out_p_and_center_dis >= r1) return 1;

    return 0;
}

/** @brief Flip common edge (p1,p2) to opposite diagonal; return new edge.
 *
 * Returns the newly inserted diagonal (global point indices) or {0} on
 * failure. Constrained edges are preserved.
 * 
 * @note - The orientation of the returned diagonal depends on which
 *         triangle contained the ordered edge (p1->p2).
 * @note - Updates edges[] and triangles[] in place; global indices may
 *         shift due to removals/insertions. debug_print controls warnings.
 *
 * @param mesh        Mesh to modify (in place).
 * @param p1          First endpoint.
 * @param p2          Second endpoint.
 * @param debug_print Emit warnings to stderr if true.
 * @return Newly inserted diagonal as an Edge_implicit, or zeroed struct.
 */
Edge_implicit as_tri_edge_implicit_mesh_flip_edge(Tri_edge_implicit_mesh *mesh, Point p1, Point p2, bool debug_print)
{
    Tri_edge_implicit_mesh temp_mesh = *mesh;

    int p1_index = as_point_in_curve_index(p1, temp_mesh.points);
    int p2_index = as_point_in_curve_index(p2, temp_mesh.points);

    AS_ASSERT(p1_index != -1 && p2_index != -1);
    
    size_t tri1_index = -1;
    size_t tri2_index = -1;

    int num_of_triangles = as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge(temp_mesh, p1, p2, &tri1_index, &tri2_index);
    
    if (num_of_triangles == 0) {
        if (debug_print) fprintf(stderr, "%s:%s:%d: [Warning] one of the points is not in the tri edge implicit mesh or edge does not exists.\n", __FILE__, __func__, __LINE__);
        return (Edge_implicit){0};
    }
    if (num_of_triangles == 1) {
        if (debug_print) fprintf(stderr, "%s:%s:%d: [Warning] this is a locally Delaunay edge.\n", __FILE__, __func__, __LINE__);
        return (Edge_implicit){0};
    }

    /* getting the third point index and checking which triangles has the ordered edge */
    int edge_index_tri1 = -1;
    for (size_t i = 0; i < 3; i++) {
        if (as_points_equal(p1, as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri1_index, i)) && as_points_equal(p2, as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri1_index, (i+1)%3))) {
            edge_index_tri1 = i;
        }
    }
    bool tri1_has_the_ordered_edge;
    if (edge_index_tri1 == -1) {
        tri1_has_the_ordered_edge = false;
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p2, as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri1_index, i)) && as_points_equal(p1, as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri1_index, (i+1)%3))) {
                edge_index_tri1 = i;
            }
        }
    } else {
        tri1_has_the_ordered_edge = true;
    }
    int third_p_index_tri1 = (edge_index_tri1 + 2) % 3;

    int edge_index_tri2 = -1;
    for (size_t i = 0; i < 3; i++) {
        if (as_points_equal(p1, as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri2_index, i)) && as_points_equal(p2, as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri2_index, (i+1)%3))) {
            edge_index_tri2 = i;
        }
    }
    bool tri2_has_the_ordered_edge;
    if (edge_index_tri2 == -1) {
        tri2_has_the_ordered_edge = false;
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p2, as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri2_index, i)) && as_points_equal(p1, as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri2_index, (i+1)%3))) {
                edge_index_tri2 = i;
            }
        }
    } else {
        tri2_has_the_ordered_edge = true;
    }
    int third_p_index_tri2 = (edge_index_tri2 + 2) % 3;

    Edge_implicit third_edge = {0};
    Edge_implicit inv_third_edge = {0};
    if (tri2_has_the_ordered_edge) {
        Tri_edge_implicit temp_tri1 = {0};
        third_edge.is_segment = false;
        third_edge.p1_index = as_point_in_curve_index(as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri1_index, third_p_index_tri1), temp_mesh.points);
        third_edge.p2_index = as_point_in_curve_index(as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri2_index, third_p_index_tri2), temp_mesh.points);
        ada_appand(Edge_implicit, temp_mesh.edges, third_edge);
        temp_tri1.edges_index[0] = temp_mesh.edges.length - 1;
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(temp_mesh.points.elements[third_edge.p2_index], as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri2_index, i))) {
                temp_tri1.edges_index[1] = temp_mesh.triangles.elements[tri2_index].edges_index[i];
            }
        }
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p1, as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri1_index, i))) {
                temp_tri1.edges_index[2] = temp_mesh.triangles.elements[tri1_index].edges_index[i];
            }
        }
        ada_appand(Tri_edge_implicit, temp_mesh.triangles, temp_tri1);

        Tri_edge_implicit temp_tri2 = {0};
        inv_third_edge.is_segment = false;
        inv_third_edge.p2_index = as_point_in_curve_index(as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri1_index, third_p_index_tri1), temp_mesh.points);
        inv_third_edge.p1_index = as_point_in_curve_index(as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri2_index, third_p_index_tri2), temp_mesh.points);
        ada_appand(Edge_implicit, temp_mesh.edges, inv_third_edge);
        temp_tri2.edges_index[0] = temp_mesh.edges.length - 1;
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(temp_mesh.points.elements[third_edge.p1_index], as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri1_index, i))) {
                temp_tri2.edges_index[1] = temp_mesh.triangles.elements[tri1_index].edges_index[i];
            }
        }
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p2, as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri2_index, i))) {
                temp_tri2.edges_index[2] = temp_mesh.triangles.elements[tri2_index].edges_index[i];
            }
        }
        ada_appand(Tri_edge_implicit, temp_mesh.triangles, temp_tri2);
        // as_tri_edge_implicit_mesh_remove_edge(&temp_mesh, p1, p2);
        as_tri_edge_implicit_mesh_delete_edge(&temp_mesh, p1, p2);

    }

    if (tri1_has_the_ordered_edge) {
        Tri_edge_implicit temp_tri1 = {0};
        third_edge.is_segment = false;
        third_edge.p1_index = as_point_in_curve_index(as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri1_index, third_p_index_tri1), temp_mesh.points);
        third_edge.p2_index = as_point_in_curve_index(as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri2_index, third_p_index_tri2), temp_mesh.points);
        ada_appand(Edge_implicit, temp_mesh.edges, third_edge);
        temp_tri1.edges_index[0] = temp_mesh.edges.length - 1;
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(temp_mesh.points.elements[third_edge.p2_index], as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri2_index, i))) {
                temp_tri1.edges_index[1] = temp_mesh.triangles.elements[tri2_index].edges_index[i];
            }
        }
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p2, as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri1_index, i))) {
                temp_tri1.edges_index[2] = temp_mesh.triangles.elements[tri1_index].edges_index[i];
            }
        }
        ada_appand(Tri_edge_implicit, temp_mesh.triangles, temp_tri1);

        Tri_edge_implicit temp_tri2 = {0};
        inv_third_edge.is_segment = false;
        inv_third_edge.p2_index = as_point_in_curve_index(as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri1_index, third_p_index_tri1), temp_mesh.points);
        inv_third_edge.p1_index = as_point_in_curve_index(as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri2_index, third_p_index_tri2), temp_mesh.points);
        ada_appand(Edge_implicit, temp_mesh.edges, inv_third_edge);
        temp_tri2.edges_index[0] = temp_mesh.edges.length - 1;
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(temp_mesh.points.elements[third_edge.p1_index], as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri1_index, i))) {
                temp_tri2.edges_index[1] = temp_mesh.triangles.elements[tri1_index].edges_index[i];
            }
        }
        for (size_t i = 0; i < 3; i++) {
            if (as_points_equal(p1, as_tri_edge_implicit_mesh_get_point_of_tri(temp_mesh, tri2_index, i))) {
                temp_tri2.edges_index[2] = temp_mesh.triangles.elements[tri2_index].edges_index[i];
            }
        }
        ada_appand(Tri_edge_implicit, temp_mesh.triangles, temp_tri2);

        // as_tri_edge_implicit_mesh_remove_edge(&temp_mesh, p1, p2);
        as_tri_edge_implicit_mesh_delete_edge(&temp_mesh, p1, p2);
    }

    *mesh = temp_mesh;

    return third_edge;
}

/** @brief Insert a constrained segment (p1,p2) into triangulation in the XY plane.
 *
 * Endpoints must exist; flips intersecting edges until the segment is
 * present, then marks it as a segment. Performs local Delaunay repairs.
 * Fails if any existing point lies on the segment (within tolerance).
 *
 * @param mesh Mesh to modify (in place).
 * @param p1   First endpoint (must exist).
 * @param p2   Second endpoint (must exist).
 */
void as_tri_edge_implicit_mesh_insert_segment(Tri_edge_implicit_mesh *mesh, Point p1, Point p2)
{
    /* points must be part of the triangulations */
    int p1_index = as_point_in_curve_index(p1, mesh->points);
    int p2_index = as_point_in_curve_index(p2, mesh->points);

    if (p1_index == -1) {
        fprintf(stderr, "%s:%s:%d: [Warning] p1 is not in the mesh.\n", __FILE__, __func__, __LINE__);
        return;
    }
    if (p2_index == -1) {
        fprintf(stderr, "%s:%s:%d: [Warning] p2 is not in the mesh.\n", __FILE__, __func__, __LINE__);
        return;
    }

    Tri_edge_implicit_mesh temp_mesh = *mesh;

    if (as_segment_hits_any_mesh_point(temp_mesh, p1, p2, AS_EPSILON, NULL)) {
        fprintf(stderr, "%s:%s:%d: [ERROR] segment intersects a point of the mesh.\n", __FILE__, __func__, __LINE__);
        return;
    }

    int edge_index = as_edge_implicit_ada_get_edge_index(temp_mesh.edges, temp_mesh.points.elements, p1, p2);
    if (-1 < edge_index) {
        temp_mesh.edges.elements[edge_index].is_segment = true;
        *mesh = temp_mesh;
        return;
    }
    int inv_edge_index = as_edge_implicit_ada_get_edge_index(temp_mesh.edges, temp_mesh.points.elements, p2, p1);
    if (-1 < inv_edge_index) {
        temp_mesh.edges.elements[inv_edge_index].is_segment = true;
        *mesh = temp_mesh;
        return;
    }
     
    Edge_implicit_ada new_edges_list = {0};
    ada_init_array(Edge_implicit, new_edges_list);
    Edge_implicit_ada intersecting_edges_list = {0};
    ada_init_array(Edge_implicit, intersecting_edges_list);

    for (size_t i = 0; i < temp_mesh.edges.length; i++) {
        if (as_edge_intersect_edge(p1, p2, temp_mesh.points.elements[temp_mesh.edges.elements[i].p1_index], temp_mesh.points.elements[temp_mesh.edges.elements[i].p2_index])) {
            ada_appand(Edge_implicit, intersecting_edges_list, temp_mesh.edges.elements[i]);
        }
    }

    // AS_EDGE_ADA_PRINT(intersecting_edges_list, temp_mesh.points.elements);

    size_t safety = 0, safety_limit = temp_mesh.edges.length * 10 + 1000;
    for (;intersecting_edges_list.length > 0;) {
        if (++safety > safety_limit) {
            fprintf(stderr, "%s:%s:%d: [Warning] segment insertion safety limit reached; aborting loop.\n", __FILE__, __func__, __LINE__);
            break;
        }

        size_t index = intersecting_edges_list.length - 1;
        Edge_implicit current_edge = intersecting_edges_list.elements[index];
        /* pop current */
        ada_remove_unordered(Edge_implicit, intersecting_edges_list, index);

        Point p3_tri1 = {0};
        Point p3_tri2 = {0};
        int num_of_third_points = as_tri_edge_implicit_mesh_get_third_points_from_edge( temp_mesh, temp_mesh.points.elements[current_edge.p1_index], temp_mesh.points.elements[current_edge.p2_index], &p3_tri1, &p3_tri2);
        if (num_of_third_points < 2) {
            /* Edge isn't shared by two triangles anymore; skip safely */
            continue;
        }

        if (!as_quad_is_convex(temp_mesh.points.elements[current_edge.p1_index], p3_tri1, temp_mesh.points.elements[current_edge.p2_index], p3_tri2)) {
            ada_insert(Edge_implicit, intersecting_edges_list, current_edge, 0);
        } else if (!as_edge_intersect_edge(p1, p2, p3_tri1, p3_tri2)) {
            Edge_implicit new_edge = as_tri_edge_implicit_mesh_flip_edge(&temp_mesh, temp_mesh.points.elements[current_edge.p1_index], temp_mesh.points.elements[current_edge.p2_index], 1);
            Tri_edge_implicit tri1 = {0}, tri2 = {0};
            as_tri_edge_implicit_mesh_get_triangles_with_edge(temp_mesh, temp_mesh.points.elements[new_edge.p1_index], temp_mesh.points.elements[new_edge.p2_index], &tri1, &tri2); 
            for (int j = 0; j < 3; j++) {
                ada_appand(Edge_implicit, new_edges_list, temp_mesh.edges.elements[tri1.edges_index[j]]);
                ada_appand(Edge_implicit, new_edges_list, temp_mesh.edges.elements[tri2.edges_index[j]]);
            }
        } else if (as_edge_intersect_edge(p1, p2, p3_tri1, p3_tri2)) {
            as_tri_edge_implicit_mesh_flip_edge(&temp_mesh, temp_mesh.points.elements[current_edge.p1_index], temp_mesh.points.elements[current_edge.p2_index], 1);
            Edge_implicit new_edge = {0};
            new_edge.p1_index = as_point_in_curve_index(p3_tri1, temp_mesh.points);
            new_edge.p2_index = as_point_in_curve_index(p3_tri2, temp_mesh.points);
            ada_insert(Edge_implicit, intersecting_edges_list, new_edge, intersecting_edges_list.length/2);
        }

    }
    
    int segment_index = as_edge_implicit_ada_get_edge_index(temp_mesh.edges, temp_mesh.points.elements, p1, p2);
    if (segment_index == -1) {
        fprintf(stderr, "%s:%d: [Warning] failed to insert segment.\n", __FILE__, __LINE__);
    } else {
        temp_mesh.edges.elements[segment_index].is_segment = true;
    }
    int inv_segment_index = as_edge_implicit_ada_get_edge_index(temp_mesh.edges, temp_mesh.points.elements, p2, p1);
    if (inv_segment_index > -1) {
        temp_mesh.edges.elements[inv_segment_index].is_segment = true;
    }

    for (;new_edges_list.length > 0;) {
        Edge_implicit current_edge = new_edges_list.elements[0];
        ada_remove_unordered(Edge_implicit, new_edges_list, 0);
        int is_delaunay = as_tri_edge_implicit_mesh_check_edge_is_locally_Delaunay(temp_mesh, temp_mesh.points.elements[current_edge.p1_index], temp_mesh.points.elements[current_edge.p2_index]);
        if (is_delaunay == 0) {
            Edge_implicit new_edge = as_tri_edge_implicit_mesh_flip_edge(&temp_mesh, temp_mesh.points.elements[current_edge.p1_index], temp_mesh.points.elements[current_edge.p2_index], 1);
            /* adding the edges of the new triangles to the edge list */
            Tri_edge_implicit tri1 = {0}, tri2 = {0};
            as_tri_edge_implicit_mesh_get_triangles_with_edge(temp_mesh, temp_mesh.points.elements[new_edge.p1_index], temp_mesh.points.elements[new_edge.p2_index], &tri1, &tri2); 
            for (int j = 0; j < 3; j++) {
                ada_appand(Edge_implicit, new_edges_list, temp_mesh.edges.elements[tri1.edges_index[j]]);
                ada_appand(Edge_implicit, new_edges_list, temp_mesh.edges.elements[tri2.edges_index[j]]);
            }
        }
    }

    free(new_edges_list.elements);
    free(intersecting_edges_list.elements);

    *mesh = temp_mesh;
}

/** @brief Build a Delaunay triangulation using a work-queue of flips.
 *
 * Steps: lexicographic triangulation -> edge mesh -> queue-based flips.
 *
 * @param c   Input XY-coplanar points.
 * @param len Number of points.
 * @return Edge-implicit Delaunay triangulation.
 */
Tri_edge_implicit_mesh as_tri_edge_implicit_mesh_make_Delaunay_triangulation_flip_algorithm(Point *c, const size_t len)
{
    Tri_implicit_mesh ti_lexi_mesh = as_points_array_get_lexicographic_triangulation(c, len);

    Tri_edge_implicit_mesh temp_tei_mesh = as_tri_implicit_mesh_to_tri_edge_implicit_mesh(ti_lexi_mesh);

    as_tri_edge_implicit_mesh_set_Delaunay_triangulation_flip_algorithm(&temp_tei_mesh);

    as_tri_implicit_mesh_free(ti_lexi_mesh);

    return temp_tei_mesh;
}

/** @brief Build Delaunay triangulation with fixed-iteration passes.
 *
 * Steps: lexicographic triangulation -> edge mesh -> fixed passes.
 *
 * @param c   Input XY-coplanar points.
 * @param len Number of points.
 * @return Edge-implicit triangulation (approximately Delaunay).
 */
Tri_edge_implicit_mesh as_tri_edge_implicit_mesh_make_Delaunay_triangulation_flip_algorithm_fixed_iterations(Point *c, const size_t len)
{
    Tri_implicit_mesh ti_lexi_mesh = as_points_array_get_lexicographic_triangulation(c, len);

    Tri_edge_implicit_mesh temp_tei_mesh = as_tri_implicit_mesh_to_tri_edge_implicit_mesh(ti_lexi_mesh);

    as_tri_edge_implicit_mesh_set_Delaunay_triangulation_flip_algorithm_fixed_iterations(&temp_tei_mesh);

    as_tri_implicit_mesh_free(ti_lexi_mesh);

    return temp_tei_mesh;
}

/** @brief In-place Delaunay enforcement (work-queue scheme).
 *
 * Pops edges, tests local Delaunay, flips if needed, and pushes new
 * neighboring edges. Constrained edges are preserved.
 *
 * @param mesh Mesh to modify (in place).
 */
void as_tri_edge_implicit_mesh_set_Delaunay_triangulation_flip_algorithm(Tri_edge_implicit_mesh *mesh)
{
    Tri_edge_implicit_mesh tei_temp_mesh = *mesh;
    Edge_implicit_ada edge_list = {0};
    ada_init_array(Edge_implicit, edge_list);
    
    for (size_t i = 0; i < tei_temp_mesh.edges.length; i++) {
        ada_appand(Edge_implicit, edge_list, tei_temp_mesh.edges.elements[i]);
    }

    printf("[INFO] Delaunay triangulation:\n");
    for (size_t init_len = edge_list.length; edge_list.length > 0;) {

        Edge_implicit current_edge = edge_list.elements[edge_list.length-1];
        edge_list.length -= 1;

        printf("\033[A\33[2K\r       remaining edges: %zu/%zu | done: %f%%\n", edge_list.length, init_len, 100.0f * (float)fmaxf((int)init_len - (int)edge_list.length, 0) / (float)init_len);
        fflush(stdout);

        int num_of_tri_on_edge = as_tri_edge_implicit_mesh_check_edge_is_locally_Delaunay(tei_temp_mesh, tei_temp_mesh.points.elements[current_edge.p1_index], tei_temp_mesh.points.elements[current_edge.p2_index]);
        if (num_of_tri_on_edge == -1) continue;
        if (num_of_tri_on_edge == 1) continue;
        Edge_implicit new_edge = as_tri_edge_implicit_mesh_flip_edge(&tei_temp_mesh, tei_temp_mesh.points.elements[current_edge.p1_index], tei_temp_mesh.points.elements[current_edge.p2_index], 0);
        
        /* adding the edges of the new triangles to the edge list */
        Tri_edge_implicit tri1 = {0}, tri2 = {0};
        as_tri_edge_implicit_mesh_get_triangles_with_edge(tei_temp_mesh, tei_temp_mesh.points.elements[new_edge.p1_index], tei_temp_mesh.points.elements[new_edge.p2_index], &tri1, &tri2); 
        for (int j = 0; j < 3; j++) {
            ada_appand(Edge_implicit, edge_list, tei_temp_mesh.edges.elements[tri1.edges_index[j]]);
            ada_appand(Edge_implicit, edge_list, tei_temp_mesh.edges.elements[tri2.edges_index[j]]);
        }
    }

    printf("\n");
    
    free(edge_list.elements);

    *mesh = tei_temp_mesh;
}

/** @brief In-place Delaunay enforcement (fixed-iteration passes).
 *
 * Runs repeated passes over all point pairs up to a hard limit. Keeps
 * constrained edges intact.
 *
 * @param mesh Mesh to modify (in place).
 */
void as_tri_edge_implicit_mesh_set_Delaunay_triangulation_flip_algorithm_fixed_iterations(Tri_edge_implicit_mesh *mesh)
{
    Tri_edge_implicit_mesh tei_temp_mesh = *mesh;

    printf("[INFO] Delaunay triangulation:\n");
    int hard_limit = 10;
    size_t total_num_of_iteration_per_time = as_choose(tei_temp_mesh.points.length, 2) * hard_limit;
    for (int times = 0, counter = 0; times < hard_limit; times++) {
        for (size_t i = 0; i < tei_temp_mesh.points.length-1; i++) {
            for (size_t j = i+1; j < tei_temp_mesh.points.length; j++) {
                counter++;
                printf("\033[A\33[2K\r       checked edges: %d | done: %f%%\n", counter, 100.0f * (float)counter / (float)total_num_of_iteration_per_time);
                fflush(stdout);

                int num_of_tri_on_edge = as_tri_edge_implicit_mesh_check_edge_is_locally_Delaunay(tei_temp_mesh, tei_temp_mesh.points.elements[i], tei_temp_mesh.points.elements[j]);
                if (num_of_tri_on_edge == -1) continue;
                if (num_of_tri_on_edge == 1) continue;
                as_tri_edge_implicit_mesh_flip_edge(&tei_temp_mesh, tei_temp_mesh.points.elements[i], tei_temp_mesh.points.elements[j], 0);
            }
        }
        if (as_tri_edge_implicit_mesh_check_Delaunay(tei_temp_mesh)) break;
    }

    printf("\n");

    *mesh = tei_temp_mesh;
}

/** @brief Create a polyline approximation of a circle in the XY plane.
 *
 * @param center       Circle center.
 * @param r            Radius (> 0).
 * @param num_of_points Number of samples (> 0).
 * @param color        Packed ARGB color (0xAARRGGBB).
 * @param plane        Plane selector ("XY" or "xy").
 * @return Curve of points forming the circle polyline.
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

/** @brief Create a triangle fan approximating a filled circle in XY.
 *
 * Assigns uniform color and light intensity.
 *
 * @param center        Circle center.
 * @param r             Radius (> 0).
 * @param num_of_points Number of boundary samples (> 0).
 * @param color         Packed ARGB color (0xAARRGGBB).
 * @param light_intensity Per-vertex light intensity.
 * @param plane         Plane selector ("XY" or "xy").
 * @return Tri_mesh covering the circle.
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

/** @brief Triangulate sampled circle points via lexicographic method.
 *
 * Assigns uniform color and light intensity.
 *
 * @param center        Circle center.
 * @param r             Radius (> 0).
 * @param num_of_points Number of boundary samples (> 0).
 * @param color         Packed ARGB color (0xAARRGGBB).
 * @param light_intensity Per-vertex light intensity.
 * @param plane         Plane selector ("XY" or "xy").
 * @return Tri_mesh approximating a filled circle.
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

/** @brief Create a unit-aligned cube triangulated into 12 triangles.
 *
 * The cube spans [0,len] on x,y,z. Assigns uniform color.
 *
 * @param len   Edge length.
 * @param color Packed ARGB color (0xAARRGGBB).
 * @return Tri_mesh of the cube.
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

/** @brief Create latitude/partial longitude curves approximating a sphere.
 *
 * Returns a Curve_ada where each element is a Curve of points at a
 * latitude or longitude. Both horizontal and vertical counts must be
 * even and > 0.
 *
 * @param center                  Sphere center.
 * @param r                       Radius (> 0).
 * @param num_of_points_horizontal Number of samples around longitude (even).
 * @param num_of_points_vertical   Number of samples along latitude (even).
 * @param color                   Packed ARGB color (0xAARRGGBB).
 * @return Curve_ada of curves forming the sphere wireframe.
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

/** @brief Create a simple triangle mesh approximating a sphere.
 *
 * Builds latitudinal rings and tessellates quads into triangles.
 * Assigns uniform color and light intensity.
 *
 * @param center                  Sphere center.
 * @param r                       Radius (> 0).
 * @param num_of_points_horizontal Number of samples around longitude (even).
 * @param num_of_points_vertical   Number of samples along latitude (even).
 * @param color                   Packed ARGB color (0xAARRGGBB).
 * @param light_intensity         Per-vertex light intensity.
 * @return Tri_mesh approximating the sphere.
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
