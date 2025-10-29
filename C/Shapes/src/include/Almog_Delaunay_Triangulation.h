#ifndef ALMOG_DELAUNAY_TRIANGULATION_H_
#define ALMOG_DELAUNAY_TRIANGULATION_H_

#define ALMOG_SHAPES_IMPLEMENTATION
#include "./Almog_Shapes.h"

#ifndef ADT_ASSERT
#include <assert.h>
#define ADT_ASSERT assert
#endif

#define ADT_EPSILON AS_EPSILON


void                    adt_tri_get_circumcircle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r);
void                    adt_tri_get_incircle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r);
void                    adt_tri_get_min_containment_circle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r);
bool                    adt_tri_implicit_mesh_check_Delaunay(Tri_implicit_mesh mesh);
int                     adt_tri_implicit_mesh_check_edge_is_locally_Delaunay(Tri_implicit_mesh mesh, Point p1, Point p2);
void                    adt_tri_implicit_mesh_flip_edge(Tri_implicit_mesh mesh, Point p1, Point p2);
Tri_implicit_mesh       adt_tri_implicit_mesh_make_Delaunay_triangulation_flip_algorithm_fixed_iterations(Point *c, const size_t len);
void                    adt_tri_implicit_mesh_set_Delaunay_triangulation_flip_algorithm_fixed_iterations(Tri_implicit_mesh mesh);
bool                    adt_tri_edge_implicit_mesh_check_Delaunay(Tri_edge_implicit_mesh mesh);
int                     adt_tri_edge_implicit_mesh_check_edge_is_locally_Delaunay(Tri_edge_implicit_mesh mesh, Point p1, Point p2);
Edge_implicit           adt_tri_edge_implicit_mesh_flip_edge(Tri_edge_implicit_mesh *mesh, Point p1, Point p2, bool debug_print);
void                    adt_tri_edge_implicit_mesh_insert_segment(Tri_edge_implicit_mesh *mesh, Point p1, Point p2, float eps);
void                    adt_tri_edge_implicit_mesh_insert_segment_array(Tri_edge_implicit_mesh *mesh, Edge *edge_list, size_t len, float eps);
void                    adt_tri_edge_implicit_mesh_insert_segment_no_intersection(Tri_edge_implicit_mesh *mesh, Point p1, Point p2, float eps);
void                    adt_tri_edge_implicit_mesh_insert_segment_array_no_intersection(Tri_edge_implicit_mesh *mesh, Edge *edge_list, size_t len, float eps);
Tri_edge_implicit_mesh  adt_tri_edge_implicit_mesh_make_Delaunay_triangulation_flip_algorithm(Point *c, const size_t len);
Tri_edge_implicit_mesh  adt_tri_edge_implicit_mesh_make_Delaunay_triangulation_flip_algorithm_fixed_iterations(Point *c, const size_t len);
void                    adt_tri_edge_implicit_mesh_set_Delaunay_triangulation_flip_algorithm(Tri_edge_implicit_mesh *mesh);
void                    adt_tri_edge_implicit_mesh_set_Delaunay_triangulation_flip_algorithm_fixed_iterations(Tri_edge_implicit_mesh *mesh);

#endif

#ifdef ALMOG_DELAUNAY_TRIANGULATION_IMPLEMENTATION
#undef ALMOG_DELAUNAY_TRIANGULATION_IMPLEMENTATION


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
void adt_tri_get_circumcircle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r)
{
    ADT_ASSERT((!strncmp(plane, "XY", 3) || !strncmp(plane, "xy", 3)) && "other planes are no implemented.");
    Tri temp_tri = {.points = {p1, p2, p3}};
    ADT_ASSERT(as_tri_equal_z(temp_tri));

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
void adt_tri_get_incircle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r)
{
    ADT_ASSERT((!strncmp(plane, "XY", 3) || !strncmp(plane, "xy", 3)) && "other planes are no implemented.");
    Tri temp_tri = {.points = {p1, p2, p3}};
    ADT_ASSERT(as_tri_equal_z(temp_tri));

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
void adt_tri_get_min_containment_circle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r)
{
    ADT_ASSERT((!strncmp(plane, "XY", 3) || !strncmp(plane, "xy", 3)) && "other planes are no implemented.");
    Tri temp_tri = {.points = {p1, p2, p3}};
    ADT_ASSERT(as_tri_equal_z(temp_tri));

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
            adt_tri_get_circumcircle(p1, p2, p3, plane, center, r);
        } else {
            Point mid = {0};
            as_points_interpolate(mid, p1, p2, 0.5);
            *center = mid;
            *r = d1 / 2;
        }
    } else if (d2 >= fmaxf(d1, d3)) {
        if (as_point_dot_point(line13, line12) >= 0) {
            adt_tri_get_circumcircle(p1, p2, p3, plane, center, r);
        } else {
            Point mid = {0};
            as_points_interpolate(mid, p2, p3, 0.5);
            *center = mid;
            *r = d2 / 2;
        }
    } else if (d3 >= fmaxf(d1, d2)) {
        if (as_point_dot_point(line23, line21) >= 0) {
            adt_tri_get_circumcircle(p1, p2, p3, plane, center, r);
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
bool adt_tri_implicit_mesh_check_Delaunay(Tri_implicit_mesh mesh)
{
    for (size_t i = 0; i < mesh.points.length-1; i++) {
        for (size_t j = i+1; j < mesh.points.length; j++) {
            int is_locally_Delaunay = adt_tri_implicit_mesh_check_edge_is_locally_Delaunay(mesh, mesh.points.elements[i], mesh.points.elements[j]);
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
int adt_tri_implicit_mesh_check_edge_is_locally_Delaunay(Tri_implicit_mesh mesh, Point p1, Point p2)
{
    int p1_index = as_point_in_curve_index(p1, mesh.points);
    int p2_index = as_point_in_curve_index(p2, mesh.points);

    ADT_ASSERT(p1_index != -1 && p2_index != -1);
    
    size_t tri1_index = 0;
    size_t tri2_index = 0;

    int num_of_triangles = as_tri_implicit_mesh_get_triangles_indexs_with_edge(mesh, p1, p2, &tri1_index, &tri2_index);
    
    if (num_of_triangles == 0) return -1;
    if (num_of_triangles == 1) return 1;

    ADA_ASSERT(num_of_triangles == 2);

    Point circumcenter_1 = {0};
    float r1 = 0;

    adt_tri_get_circumcircle(as_tri_implicit_mesh_expand_tri_to_points(mesh, tri1_index), "xy", &circumcenter_1, &r1);
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
void adt_tri_implicit_mesh_flip_edge(Tri_implicit_mesh mesh, Point p1, Point p2)
{
    int p1_index = as_point_in_curve_index(p1, mesh.points);
    int p2_index = as_point_in_curve_index(p2, mesh.points);

    ADT_ASSERT(p1_index != -1 || p2_index != -1);
    
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
Tri_implicit_mesh adt_tri_implicit_mesh_make_Delaunay_triangulation_flip_algorithm_fixed_iterations(Point *c, const size_t len)
{
    Tri_implicit_mesh ti_lexi_mesh = as_points_array_get_lexicographic_triangulation(c, len);

    adt_tri_implicit_mesh_set_Delaunay_triangulation_flip_algorithm_fixed_iterations(ti_lexi_mesh);

    return ti_lexi_mesh;
}

/** @brief Enforce Delaunay by repeated passes (fixed iteration cap).
 *
 * Modifies the mesh in place.
 *
 * @param mesh Implicit mesh to modify (in place).
 */
void adt_tri_implicit_mesh_set_Delaunay_triangulation_flip_algorithm_fixed_iterations(Tri_implicit_mesh mesh)
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
    
                int num_of_tri_on_edge = adt_tri_implicit_mesh_check_edge_is_locally_Delaunay(mesh, mesh.points.elements[i], mesh.points.elements[j]);
                if (num_of_tri_on_edge == -1) continue;
                if (num_of_tri_on_edge == 1) continue;
                adt_tri_implicit_mesh_flip_edge(mesh, mesh.points.elements[i], mesh.points.elements[j]);
            }
        }
        if (adt_tri_implicit_mesh_check_Delaunay(mesh)) break;
    }
    printf("\n");
}

/** @brief Check if all interior non-segment edges are locally Delaunay.
 *
 * @param mesh Edge-implicit mesh to test.
 * @return true if Delaunay; false otherwise.
 */
bool adt_tri_edge_implicit_mesh_check_Delaunay(Tri_edge_implicit_mesh mesh)
{
    for (size_t i = 0; i < mesh.edges.length; i++) {
        int is_locally_Delaunay = adt_tri_edge_implicit_mesh_check_edge_is_locally_Delaunay(mesh, mesh.points.elements[mesh.edges.elements[i].p1_index], mesh.points.elements[mesh.edges.elements[i].p2_index]);
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
int adt_tri_edge_implicit_mesh_check_edge_is_locally_Delaunay(Tri_edge_implicit_mesh mesh, Point p1, Point p2)
{
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

    adt_tri_get_circumcircle(as_tri_edge_implicit_mesh_expand_tri_to_points(mesh, tri1_index), "xy", &circumcenter_1, &r1);

    Point tri2_outside_p = {0};
    for (size_t i = 0; i < 3; i++) {
        Point current_point = as_tri_edge_implicit_mesh_get_point_of_tri(mesh, tri2_index, i);
        if (!as_points_equal(current_point, p1) && !as_points_equal(current_point, p2)) tri2_outside_p = current_point;
    }

    float tri2_out_p_and_center_dis = as_points_distance(tri2_outside_p, circumcenter_1);

    if (tri2_out_p_and_center_dis > r1) return 1;

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
Edge_implicit adt_tri_edge_implicit_mesh_flip_edge(Tri_edge_implicit_mesh *mesh, Point p1, Point p2, bool debug_print)
{
    Tri_edge_implicit_mesh temp_mesh = *mesh;

    int p1_index = as_point_in_curve_index(p1, temp_mesh.points);
    int p2_index = as_point_in_curve_index(p2, temp_mesh.points);

    ADT_ASSERT(p1_index != -1 && p2_index != -1);
    
    size_t tri1_index = -1;
    size_t tri2_index = -1;

    int num_of_triangles = as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge(temp_mesh, p1, p2, &tri1_index, &tri2_index);
    
    if (num_of_triangles == 0) {
        if (debug_print) fprintf(stderr, "%s:%s:%d:\n[Warning] one of the points is not in the tri edge implicit mesh or edge does not exists.\n\n", __FILE__, __func__, __LINE__);
        return (Edge_implicit){0};
    }
    if (num_of_triangles == 1) {
        if (debug_print) fprintf(stderr, "%s:%s:%d:\n[Warning] this is a locally Delaunay edge.\n\n", __FILE__, __func__, __LINE__);
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

void adt_tri_edge_implicit_mesh_insert_segment(Tri_edge_implicit_mesh *mesh, Point p1, Point p2, float eps)
{
    Edge_ada seg_list = {0};
    ada_init_array(Edge, seg_list);
    Edge temp_edge = {.p1 = p1, .p2 = p2};
    ada_appand(Edge, seg_list, temp_edge);

    for (;as_edge_array_intersects_points_array(seg_list.elements, seg_list.length, mesh->points.elements, mesh->points.length, eps);) {
        Edge current_edge = seg_list.elements[--(seg_list.length)];
        Point intersecting_point = {0};
        if (as_edge_intersects_any_point_in_array(current_edge.p1, current_edge.p2, mesh->points.elements, mesh->points.length, eps, &intersecting_point)) {
            Edge new_edge1 = {.p1 = current_edge.p1, .p2 = intersecting_point, .is_segment = 1};
            Edge new_edge2 = {.p1 = intersecting_point, .p2 = current_edge.p2, .is_segment = 1};
            ada_appand(Edge, seg_list, new_edge1);
            ada_appand(Edge, seg_list, new_edge2);
        }
    }

    adt_tri_edge_implicit_mesh_insert_segment_array_no_intersection(mesh, seg_list.elements, seg_list.length, eps);

    free(seg_list.elements);
}

void adt_tri_edge_implicit_mesh_insert_segment_array(Tri_edge_implicit_mesh *mesh, Edge *edge_list, size_t len, float eps)
{
    for (size_t i = 0; i < len; i++) {
        Edge current_edge = edge_list[i];
        adt_tri_edge_implicit_mesh_insert_segment(mesh, current_edge.p1, current_edge.p2, eps);
    }
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
void adt_tri_edge_implicit_mesh_insert_segment_no_intersection(Tri_edge_implicit_mesh *mesh, Point p1, Point p2, float eps)
{
    /* points must be part of the triangulations */
    int p1_index = as_point_in_curve_index(p1, mesh->points);
    int p2_index = as_point_in_curve_index(p2, mesh->points);

    if (p1_index == -1) {
        fprintf(stderr, "%s:%s:%d:\n[Warning] p1 is not in the mesh.\n\n", __FILE__, __func__, __LINE__);
        return;
    }
    if (p2_index == -1) {
        fprintf(stderr, "%s:%s:%d:\n[Warning] p2 is not in the mesh.\n\n", __FILE__, __func__, __LINE__);
        return;
    }

    Tri_edge_implicit_mesh temp_mesh = *mesh;

    Point inters_p = {0};
    if (as_edge_intersects_any_point_in_array(p1, p2, temp_mesh.points.elements, temp_mesh.points.length, eps, &inters_p)) {
        fprintf(stderr, "%s:%s:%d:\n[ERROR] segment intersects a point of the mesh.\n", __FILE__, __func__, __LINE__);
        fprintf(stderr, "        Tried to input segment:\n");
        fprintf(stderr, "           (%f, %f, %f)\n", as_point_expand_to_xyz(p1));
        fprintf(stderr, "           (%f, %f, %f)\n", as_point_expand_to_xyz(p2));
        fprintf(stderr, "        intersects point:\n");
        fprintf(stderr, "           (%f, %f, %f)\n\n", as_point_expand_to_xyz(inters_p));

        return;
    }

    int edge_index = as_edge_implicit_ada_get_edge_index(temp_mesh.edges, temp_mesh.points.elements, p1, p2);
    if (-1 < edge_index) {
        temp_mesh.edges.elements[edge_index].is_segment = true;
        Edge_implicit inv_segment = {.is_segment = true, .p1_index = as_point_in_curve_index(p2, temp_mesh.points), .p2_index = as_point_in_curve_index(p1, temp_mesh.points)};
        ada_appand(Edge_implicit, temp_mesh.edges, inv_segment);

        *mesh = temp_mesh;
        return;
    }
    int inv_edge_index = as_edge_implicit_ada_get_edge_index(temp_mesh.edges, temp_mesh.points.elements, p2, p1);
    if (-1 < inv_edge_index) {
        temp_mesh.edges.elements[inv_edge_index].is_segment = true;
        Edge_implicit segment = {.is_segment = true, .p1_index = as_point_in_curve_index(p1, temp_mesh.points), .p2_index = as_point_in_curve_index(p2, temp_mesh.points)};
        ada_appand(Edge_implicit, temp_mesh.edges, segment);

        *mesh = temp_mesh;
        return;
    }
     
    Edge_implicit_ada new_edges_list = {0};
    ada_init_array(Edge_implicit, new_edges_list);
    Edge_implicit_ada intersecting_edges_list = {0};
    ada_init_array(Edge_implicit, intersecting_edges_list);

    for (size_t i = 0; i < temp_mesh.edges.length; i++) {
        if (as_edge_intersect_edge(p1, p2, temp_mesh.points.elements[temp_mesh.edges.elements[i].p1_index], temp_mesh.points.elements[temp_mesh.edges.elements[i].p2_index])) {
            if (temp_mesh.edges.elements[i].is_segment) {
                fprintf(stderr, "%s:%s:%d:\n[ERROR] segment intersects a segment of the mesh. failed to insert segment\n\n", __FILE__, __func__, __LINE__);

                *mesh = temp_mesh;
                return;
            }
            ada_appand(Edge_implicit, intersecting_edges_list, temp_mesh.edges.elements[i]);
        }
    }

    // AS_EDGE_ADA_PRINT(intersecting_edges_list, temp_mesh.points.elements);

    size_t safety = 0, safety_limit = temp_mesh.edges.length * 10 + 1000;
    for (;intersecting_edges_list.length > 0;) {
        if (++safety > safety_limit) {
            fprintf(stderr, "%s:%s:%d:\n[Error] segment insertion safety limit reached; aborting loop.\n\n", __FILE__, __func__, __LINE__);
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
            Edge_implicit new_edge = adt_tri_edge_implicit_mesh_flip_edge(&temp_mesh, temp_mesh.points.elements[current_edge.p1_index], temp_mesh.points.elements[current_edge.p2_index], 1);
            Tri_edge_implicit tri1 = {0}, tri2 = {0};
            as_tri_edge_implicit_mesh_get_triangles_with_edge(temp_mesh, temp_mesh.points.elements[new_edge.p1_index], temp_mesh.points.elements[new_edge.p2_index], &tri1, &tri2); 
            for (int j = 0; j < 3; j++) {
                ada_appand(Edge_implicit, new_edges_list, temp_mesh.edges.elements[tri1.edges_index[j]]);
                ada_appand(Edge_implicit, new_edges_list, temp_mesh.edges.elements[tri2.edges_index[j]]);
            }
        } else if (as_edge_intersect_edge(p1, p2, p3_tri1, p3_tri2)) {
            adt_tri_edge_implicit_mesh_flip_edge(&temp_mesh, temp_mesh.points.elements[current_edge.p1_index], temp_mesh.points.elements[current_edge.p2_index], 1);
            Edge_implicit new_edge = {0};
            new_edge.p1_index = as_point_in_curve_index(p3_tri1, temp_mesh.points);
            new_edge.p2_index = as_point_in_curve_index(p3_tri2, temp_mesh.points);
            ada_insert(Edge_implicit, intersecting_edges_list, new_edge, intersecting_edges_list.length/2);
        }

    }
    
    int segment_index = as_edge_implicit_ada_get_edge_index(temp_mesh.edges, temp_mesh.points.elements, p1, p2);
    if (segment_index == -1) {
        fprintf(stderr, "%s:%s:%d:\n[Error] failed to insert segment.\n\n", __FILE__, __func__, __LINE__);
    } else {
        temp_mesh.edges.elements[segment_index].is_segment = true;
    }
    int inv_segment_index = as_edge_implicit_ada_get_edge_index(temp_mesh.edges, temp_mesh.points.elements, p2, p1);
    if (inv_segment_index == -1) {
        Edge_implicit inv_segment = {.is_segment = 1, .p1_index = as_point_in_curve_index(p2, temp_mesh.points), .p2_index = as_point_in_curve_index(p1, temp_mesh.points)};
        ada_appand(Edge_implicit, temp_mesh.edges, inv_segment);
    } else {
        temp_mesh.edges.elements[inv_segment_index].is_segment = true;
    }

    for (;new_edges_list.length > 0;) {
        Edge_implicit current_edge = new_edges_list.elements[0];
        ada_remove_unordered(Edge_implicit, new_edges_list, 0);
        int is_delaunay = adt_tri_edge_implicit_mesh_check_edge_is_locally_Delaunay(temp_mesh, temp_mesh.points.elements[current_edge.p1_index], temp_mesh.points.elements[current_edge.p2_index]);
        if (is_delaunay == 0) {
            Edge_implicit new_edge = adt_tri_edge_implicit_mesh_flip_edge(&temp_mesh, temp_mesh.points.elements[current_edge.p1_index], temp_mesh.points.elements[current_edge.p2_index], 1);
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

void adt_tri_edge_implicit_mesh_insert_segment_array_no_intersection(Tri_edge_implicit_mesh *mesh, Edge *edge_list, size_t len, float eps)
{
    for (size_t i = 0; i < len; i++) {
        Edge current_edge = edge_list[i];
        adt_tri_edge_implicit_mesh_insert_segment_no_intersection(mesh, current_edge.p1, current_edge.p2, eps);
    }
}

/** @brief Build a Delaunay triangulation using a work-queue of flips.
 *
 * Steps: lexicographic triangulation -> edge mesh -> queue-based flips.
 *
 * @param c   Input XY-coplanar points.
 * @param len Number of points.
 * @return Edge-implicit Delaunay triangulation.
 */
Tri_edge_implicit_mesh adt_tri_edge_implicit_mesh_make_Delaunay_triangulation_flip_algorithm(Point *c, const size_t len)
{
    Tri_implicit_mesh ti_lexi_mesh = as_points_array_get_lexicographic_triangulation(c, len);

    Tri_edge_implicit_mesh temp_tei_mesh = as_tri_implicit_mesh_to_tri_edge_implicit_mesh(ti_lexi_mesh);

    adt_tri_edge_implicit_mesh_set_Delaunay_triangulation_flip_algorithm(&temp_tei_mesh);

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
Tri_edge_implicit_mesh adt_tri_edge_implicit_mesh_make_Delaunay_triangulation_flip_algorithm_fixed_iterations(Point *c, const size_t len)
{
    Tri_implicit_mesh ti_lexi_mesh = as_points_array_get_lexicographic_triangulation(c, len);

    Tri_edge_implicit_mesh temp_tei_mesh = as_tri_implicit_mesh_to_tri_edge_implicit_mesh(ti_lexi_mesh);

    adt_tri_edge_implicit_mesh_set_Delaunay_triangulation_flip_algorithm_fixed_iterations(&temp_tei_mesh);

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
void adt_tri_edge_implicit_mesh_set_Delaunay_triangulation_flip_algorithm(Tri_edge_implicit_mesh *mesh)
{
    Tri_edge_implicit_mesh tei_temp_mesh = *mesh;
    Edge_implicit_ada edge_list = {0};
    ada_init_array(Edge_implicit, edge_list);
    
    for (size_t i = 0; i < tei_temp_mesh.edges.length; i++) {
        ada_appand(Edge_implicit, edge_list, tei_temp_mesh.edges.elements[i]);
    }

    printf("[INFO] Delaunay triangulation:\n");
    for (size_t init_len = edge_list.length; edge_list.length > 0;) {

        if (edge_list.length > init_len * 2) {
            fprintf(stderr, "%s:%s:%d:\n[Warning] flip algorithm safety limit reached; aborting loop.\n\n", __FILE__, __func__, __LINE__);
            break;
        }

        Edge_implicit current_edge = edge_list.elements[edge_list.length-1];
        edge_list.length -= 1;

        printf("\033[A\33[2K\r       remaining edges: %zu/%zu | done: %f%%\n", edge_list.length, init_len, 100.0f * (float)fmaxf((int)init_len - (int)edge_list.length, 0) / (float)init_len);
        fflush(stdout);

        int num_of_tri_on_edge = adt_tri_edge_implicit_mesh_check_edge_is_locally_Delaunay(tei_temp_mesh, tei_temp_mesh.points.elements[current_edge.p1_index], tei_temp_mesh.points.elements[current_edge.p2_index]);
        if (num_of_tri_on_edge == -1) continue;
        if (num_of_tri_on_edge == 1) continue;
        Edge_implicit new_edge = adt_tri_edge_implicit_mesh_flip_edge(&tei_temp_mesh, tei_temp_mesh.points.elements[current_edge.p1_index], tei_temp_mesh.points.elements[current_edge.p2_index], 0);
        
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
void adt_tri_edge_implicit_mesh_set_Delaunay_triangulation_flip_algorithm_fixed_iterations(Tri_edge_implicit_mesh *mesh)
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

                int num_of_tri_on_edge = adt_tri_edge_implicit_mesh_check_edge_is_locally_Delaunay(tei_temp_mesh, tei_temp_mesh.points.elements[i], tei_temp_mesh.points.elements[j]);
                if (num_of_tri_on_edge == -1) continue;
                if (num_of_tri_on_edge == 1) continue;
                adt_tri_edge_implicit_mesh_flip_edge(&tei_temp_mesh, tei_temp_mesh.points.elements[i], tei_temp_mesh.points.elements[j], 0);
            }
        }
        if (adt_tri_edge_implicit_mesh_check_Delaunay(tei_temp_mesh)) break;
    }

    printf("\n");

    *mesh = tei_temp_mesh;
}


#endif


