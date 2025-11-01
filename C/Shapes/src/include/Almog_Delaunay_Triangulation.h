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
int                     adt_tri_edge_implicit_mesh_check_edge_index_is_locally_Delaunay(Tri_edge_implicit_mesh mesh, size_t edge_index);
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


void adt_tri_get_circumcircle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r)
{
    ADT_ASSERT((!strncmp(plane, "XY", 3) || !strncmp(plane, "xy", 3)) && "other planes are no implemented.");
    ADT_ASSERT(center != NULL);
    ADT_ASSERT(r != NULL);
    as_point_assert_finite(p1);
    as_point_assert_finite(p2);
    as_point_assert_finite(p3);

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

    /* Guard against degenerate (collinear or near-collinear) triangles:
    perpendicular bisectors are parallel => det ~ 0. Provide a
    reasonable fallback (diameter of the longest side). */
    if (fabsf(det) <= ADT_EPSILON) {
        float d12 = as_points_distance(p1, p2);
        float d23 = as_points_distance(p2, p3);
        float d31 = as_points_distance(p3, p1);
        Point a = p1, b = p2;
        float dmax = d12;
        if (d23 > dmax) {
            dmax = d23;
            a = p2;
            b = p3;
        }
        if (d31 > dmax) {
            dmax = d31;
            a = p3;
            b = p1;
        }
        as_points_interpolate((*center), a, b, 0.5f);
        *r = dmax * 0.5f;
        return;
    }

    float x = detx / det;
    float y = dety / det;

    *center = (Point){0,0,0,0};
    (*center).x = x;
    (*center).y = y;
    (*center).z = p1.z;

    *r = as_points_distance(p1, *center);
}

void adt_tri_get_incircle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r)
{
    ADT_ASSERT((!strncmp(plane, "XY", 3) || !strncmp(plane, "xy", 3)) && "other planes are no implemented.");
    ADT_ASSERT(center != NULL);
    ADT_ASSERT(r != NULL);
    as_point_assert_finite(p1);
    as_point_assert_finite(p2);
    as_point_assert_finite(p3);

    Tri temp_tri = {.points = {p1, p2, p3}};
    ADT_ASSERT(as_tri_equal_z(temp_tri));

    /* wiki: https://en.wikipedia.org/wiki/Incircle_and_excircles */
    float a = as_points_distance(p2, p3);
    float b = as_points_distance(p3, p1);
    float c = as_points_distance(p1, p2);
    ADT_ASSERT(a > 0.0f && b > 0.0f && c > 0.0f && "triangle edges must be non-degenerate");
    float area = fabsf((float)as_tri_area_xy(p1, p2, p3));
    ADT_ASSERT(area > ADT_EPSILON && "incircle undefined for degenerate tri");

    (*center)   = (Point){0,0,0,0};
    (*center).x = (a * p1.x + b * p2.x + c * p3.x) / (a + b + c);
    (*center).y = (a * p1.y + b * p2.y + c * p3.y) / (a + b + c);
    (*center).z = p1.z;
    float s = 0.5 * (a + b + c);
    ADT_ASSERT(s > 0.0f && "triangle semi-perimeter must be positive");
    float rad_sqr = (s - a) * (s - b) * (s - c) / s;
    /* numeric noise safety */
    ADT_ASSERT(rad_sqr >= 0.0f && "negative incircle radicand");
    *r = sqrtf(fmaxf(rad_sqr, 0.0f));
}

void adt_tri_get_min_containment_circle(Point p1, Point p2, Point p3, const char plane[], Point *center, float *r)
{
    ADT_ASSERT((!strncmp(plane, "XY", 3) || !strncmp(plane, "xy", 3)) && "other planes are no implemented.");
    ADT_ASSERT(center != NULL);
    ADT_ASSERT(r != NULL);
    as_point_assert_finite(p1);
    as_point_assert_finite(p2);
    as_point_assert_finite(p3);

    Tri temp_tri = {.points = {p1, p2, p3}};
    ADT_ASSERT(as_tri_equal_z(temp_tri));

    float d1 = as_points_distance(p1, p2);
    float d2 = as_points_distance(p2, p3);
    float d3 = as_points_distance(p3, p1);

    ADT_ASSERT(d1 > 0.0f && d2 > 0.0f && d3 > 0.0f && "triangle edges must be non-degenerate");

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

int adt_tri_implicit_mesh_check_edge_is_locally_Delaunay(Tri_implicit_mesh mesh, Point p1, Point p2)
{
    ADT_ASSERT(!as_points_equal(p1, p2) && "edge endpoints must differ");

    int p1_index = as_point_in_curve_index(p1, mesh.points);
    int p2_index = as_point_in_curve_index(p2, mesh.points);

    ADT_ASSERT(p1_index != -1 && p2_index != -1);
    
    size_t tri1_index = 0;
    size_t tri2_index = 0;

    int num_of_triangles = as_tri_implicit_mesh_get_triangles_indexs_with_edge(mesh, p1, p2, &tri1_index, &tri2_index);
    
    if (num_of_triangles == 0) return -1;
    if (num_of_triangles == 1) return 1;

    ADT_ASSERT(num_of_triangles == 2 && "an edge cannot be shared by more then 2 triangles");
    ADT_ASSERT((tri1_index < mesh.triangles.length) && "tri index OOB");
    ADT_ASSERT((tri2_index < mesh.triangles.length) && "tri index OOB");

    Point circumcenter_1 = {0};
    float r1 = 0;

    adt_tri_get_circumcircle(as_tri_implicit_mesh_expand_tri_to_points(mesh, tri1_index), "xy", &circumcenter_1, &r1);

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

void adt_tri_implicit_mesh_flip_edge(Tri_implicit_mesh mesh, Point p1, Point p2)
{
    ADT_ASSERT(!as_points_equal(p1, p2) && "edge endpoints must differ");

    int p1_index = as_point_in_curve_index(p1, mesh.points);
    int p2_index = as_point_in_curve_index(p2, mesh.points);

    ADT_ASSERT(p1_index != -1 && p2_index != -1);
    
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
    ADT_ASSERT(isfinite(cross) && "area must be finite");
    if (cross > 0) {
        int temp = mesh.triangles.elements[tri1_index].points_index[0];
        mesh.triangles.elements[tri1_index].points_index[0] = mesh.triangles.elements[tri1_index].points_index[2];
        mesh.triangles.elements[tri1_index].points_index[2] = temp;
    }
    cross = as_tri_implicit_area_xy(mesh, tri2_index);
    ADT_ASSERT(isfinite(cross) && "area must be finite");
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

Tri_implicit_mesh adt_tri_implicit_mesh_make_Delaunay_triangulation_flip_algorithm_fixed_iterations(Point *c, const size_t len)
{
    ADT_ASSERT(c != NULL);
    ADT_ASSERT(len >= 1 && "need at least one point");

    Tri_implicit_mesh ti_lexi_mesh = as_points_array_get_lexicographic_triangulation(c, len);

    adt_tri_implicit_mesh_set_Delaunay_triangulation_flip_algorithm_fixed_iterations(ti_lexi_mesh);

    return ti_lexi_mesh;
}

void adt_tri_implicit_mesh_set_Delaunay_triangulation_flip_algorithm_fixed_iterations(Tri_implicit_mesh mesh)
{
    ADT_ASSERT(mesh.points.elements != NULL);
    ADT_ASSERT(mesh.triangles.elements != NULL);
    ADT_ASSERT(mesh.points.length > 2 && "need > 2 points");

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

bool adt_tri_edge_implicit_mesh_check_Delaunay(Tri_edge_implicit_mesh mesh)
{
    ADT_ASSERT(mesh.points.elements != NULL);
    ADT_ASSERT((mesh.edges.length == 0 || mesh.edges.elements != NULL) && "edges elements pointer invalid");
    for (size_t i = 0; i < mesh.edges.length; i++) {
        int is_locally_Delaunay = adt_tri_edge_implicit_mesh_check_edge_is_locally_Delaunay(mesh, mesh.points.elements[mesh.edges.elements[i].p1_index], mesh.points.elements[mesh.edges.elements[i].p2_index]);
        if (is_locally_Delaunay == -1) continue;
        if (is_locally_Delaunay == 1) continue;
        if (is_locally_Delaunay == 0) return false;
    }

    return true;
}

int adt_tri_edge_implicit_mesh_check_edge_is_locally_Delaunay(Tri_edge_implicit_mesh mesh, Point p1, Point p2)
{
    ADT_ASSERT(mesh.points.elements != NULL && "points array null");
    ADT_ASSERT((mesh.edges.length == 0 || mesh.edges.elements != NULL) && "edges elements pointer invalid");
    ADT_ASSERT(!as_points_equal(p1, p2) && "edge endpoints must differ");

    int edge_index = as_edge_implicit_ada_get_edge_index(mesh.edges, mesh.points.elements, p1, p2);
    if (edge_index == -1) return -1;
    if (mesh.edges.elements[edge_index].is_segment) return 1;
    
    size_t tri1_index = 0;
    size_t tri2_index = 0;
    Edge_implicit ei = mesh.edges.elements[edge_index];
    /* find inverse edge index once */
    int inv_ei_index = as_edge_implicit_ada_get_edge_index( mesh.edges, mesh.points.elements, mesh.points.elements[ei.p2_index], mesh.points.elements[ei.p1_index]);

    int num_of_triangles = as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge_index(mesh, edge_index, inv_ei_index, &tri1_index, &tri2_index);
    
    if (num_of_triangles == 0) return -1;
    if (num_of_triangles == 1) return 1;

    ADT_ASSERT(num_of_triangles == 2 && "an edge cannot be shared by > 2 triangles");

    ADT_ASSERT(tri1_index < mesh.triangles.length && "tri index OOB");
    ADT_ASSERT(tri2_index < mesh.triangles.length && "tri index OOB");

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

int adt_tri_edge_implicit_mesh_check_edge_index_is_locally_Delaunay(Tri_edge_implicit_mesh mesh, size_t edge_index)
{
    ADT_ASSERT(mesh.points.elements != NULL && "points array null");
    ADT_ASSERT((mesh.edges.length == 0 || mesh.edges.elements != NULL) && "edges elements pointer invalid");

    if (mesh.edges.elements[edge_index].is_segment) return 1;

    Point p1 = mesh.points.elements[mesh.edges.elements[edge_index].p1_index];
    Point p2 = mesh.points.elements[mesh.edges.elements[edge_index].p2_index];
    
    size_t tri1_index = 0;
    size_t tri2_index = 0;
    Edge_implicit ei = mesh.edges.elements[edge_index];
    /* find inverse edge index once */
    int inv_ei_index = -1;
    /* check if next edge is the inv edge */
    if (edge_index < mesh.edges.length-1 && edge_index >= 1) {
        Edge_implicit next_ei = mesh.edges.elements[edge_index+1];
        Edge_implicit prev_ei = mesh.edges.elements[edge_index-1];
        if ((ei.p1_index == next_ei.p2_index) && (ei.p2_index == next_ei.p1_index)) {
            inv_ei_index = edge_index + 1;
        } else if ((ei.p1_index == prev_ei.p2_index) && (ei.p2_index == prev_ei.p1_index)) {
            inv_ei_index = edge_index - 1;
        }
    }
    if (inv_ei_index == -1) inv_ei_index = as_edge_implicit_ada_get_edge_index( mesh.edges, mesh.points.elements, mesh.points.elements[ei.p2_index], mesh.points.elements[ei.p1_index]);

    int num_of_triangles = as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge_index(mesh, edge_index, inv_ei_index, &tri1_index, &tri2_index);
    
    if (num_of_triangles == 0) return -1;
    if (num_of_triangles == 1) return 1;

    ADT_ASSERT(num_of_triangles == 2 && "an edge cannot be shared by > 2 triangles");

    ADT_ASSERT(tri1_index < mesh.triangles.length && "tri index OOB");
    ADT_ASSERT(tri2_index < mesh.triangles.length && "tri index OOB");

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

Edge_implicit adt_tri_edge_implicit_mesh_flip_edge(Tri_edge_implicit_mesh *mesh, Point p1, Point p2, bool debug_print)
{
    ADT_ASSERT(!as_points_equal(p1, p2) && "edge endpoints must differ");
    ADT_ASSERT(mesh != NULL);

    Tri_edge_implicit_mesh temp_mesh = *mesh;
    ADT_ASSERT(temp_mesh.points.elements != NULL && "points array null");
    ADT_ASSERT((temp_mesh.edges.length == 0 || temp_mesh.edges.elements != NULL) && "edges array null");

    // int p1_index = as_point_in_curve_index(p1, temp_mesh.points);
    // int p2_index = as_point_in_curve_index(p2, temp_mesh.points);
    // ADT_ASSERT(p1_index != -1 && p2_index != -1);
    
    size_t tri1_index = -1;
    size_t tri2_index = -1;

    int num_of_triangles = as_tri_edge_implicit_mesh_get_triangles_indexs_with_edge(temp_mesh, p1, p2, &tri1_index, &tri2_index);
    ADT_ASSERT(tri1_index < temp_mesh.triangles.length && "tri index OOB");
    ADT_ASSERT(tri2_index < temp_mesh.triangles.length && "tri index OOB");
    if (num_of_triangles == 0) {
        if (debug_print) fprintf(stderr, "%s:%s:%d:\n[Warning] one of the points is not in the tri edge implicit mesh or edge does not exists.\n\n", __FILE__, __func__, __LINE__);
        return (Edge_implicit){0};
    }
    if (num_of_triangles == 1) {
        if (debug_print) fprintf(stderr, "%s:%s:%d:\n[Warning] this is a locally Delaunay edge.\n\n", __FILE__, __func__, __LINE__);
        return (Edge_implicit){0};
    }

    /* getting neighbor indexes */
    #if 0
    /* making sure the neighbors indexes of tri1 and tri2 is correct */
    as_tri_edge_implicit_mesh_set_neighbor_of_tri(temp_mesh, tri1_index);
    as_tri_edge_implicit_mesh_set_neighbor_of_tri(temp_mesh, tri2_index);
    int neighboring_tri_index[4] = {-1};
    size_t neighbors_array_counter = 0;
    for (size_t i = 0; i < 3; i++) {
        if (mesh->triangles.elements[tri1_index].neighbor_tri_index[i] != (int)tri2_index) {
            neighboring_tri_index[neighbors_array_counter++] = mesh->triangles.elements[tri1_index].neighbor_tri_index[i];
        }
    }
    for (size_t i = 0; i < 3; i++) {
        if (mesh->triangles.elements[tri2_index].neighbor_tri_index[i] != (int)tri1_index) {
            neighboring_tri_index[neighbors_array_counter++] = mesh->triangles.elements[tri2_index].neighbor_tri_index[i];
        }
    }
    #else
    int neighboring_tri_index[6] = {-1};
    size_t neighbors_array_counter = 0;
    for (size_t i = 0; i < 3; i++) {
        neighboring_tri_index[neighbors_array_counter++] = mesh->triangles.elements[tri1_index].neighbor_tri_index[i];
    }
    for (size_t i = 0; i < 3; i++) {
        neighboring_tri_index[neighbors_array_counter++] = mesh->triangles.elements[tri2_index].neighbor_tri_index[i];
    }
    #endif


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


    as_tri_edge_implicit_mesh_set_neighbor_of_tri(temp_mesh, temp_mesh.triangles.length-2);
    as_tri_edge_implicit_mesh_set_neighbor_of_tri(temp_mesh, temp_mesh.triangles.length-1);
    for (size_t i = 0; i < neighbors_array_counter; i++) {
        if (neighboring_tri_index[i] > -1) {
            as_tri_edge_implicit_mesh_set_neighbor_of_tri(temp_mesh, neighboring_tri_index[i]);
        }
    }



    

    *mesh = temp_mesh;

    return third_edge;
}

void adt_tri_edge_implicit_mesh_insert_segment(Tri_edge_implicit_mesh *mesh, Point p1, Point p2, float eps)
{
    ADT_ASSERT(mesh != NULL);
    ADT_ASSERT(mesh->points.elements != NULL && "points array null");
    ADT_ASSERT(eps >= 0.0f && "eps must be non-negative");
    ADT_ASSERT(!as_points_equal(p1, p2) && "segment endpoints must differ");

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
    ADT_ASSERT(mesh != NULL);
    ADT_ASSERT(mesh->points.elements != NULL && "points array null");
    ADT_ASSERT(eps >= 0.0f && "eps must be non-negative");
    ADT_ASSERT((len == 0 || mesh->points.elements != NULL) && "points null");

    for (size_t i = 0; i < len; i++) {
        Edge current_edge = edge_list[i];
        adt_tri_edge_implicit_mesh_insert_segment(mesh, current_edge.p1, current_edge.p2, eps);
    }
}

void adt_tri_edge_implicit_mesh_insert_segment_no_intersection(Tri_edge_implicit_mesh *mesh, Point p1, Point p2, float eps)
{
    ADT_ASSERT(mesh != NULL);
    ADT_ASSERT(mesh->points.elements != NULL && "points array null");
    ADT_ASSERT(eps >= 0.0f && "eps must be non-negative");
    ADT_ASSERT(!as_points_equal(p1, p2) && "segment endpoints must differ");

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

                free(new_edges_list.elements);
                free(intersecting_edges_list.elements);
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
    ADT_ASSERT(mesh != NULL);
    ADT_ASSERT(mesh->points.elements != NULL && "points array null");
    ADT_ASSERT(eps >= 0.0f && "eps must be non-negative");
    ADT_ASSERT((len == 0 || mesh->points.elements != NULL) && "points null");

    for (size_t i = 0; i < len; i++) {
        Edge current_edge = edge_list[i];
        adt_tri_edge_implicit_mesh_insert_segment_no_intersection(mesh, current_edge.p1, current_edge.p2, eps);
    }
}

Tri_edge_implicit_mesh adt_tri_edge_implicit_mesh_make_Delaunay_triangulation_flip_algorithm(Point *c, const size_t len)
{
    ADT_ASSERT(c != NULL);
    ADT_ASSERT(len >= 1 && "need at least one point");

    Tri_implicit_mesh ti_lexi_mesh = as_points_array_get_lexicographic_triangulation(c, len);
    Tri_edge_implicit_mesh tei_mesh = as_tri_implicit_mesh_to_tri_edge_implicit_mesh(ti_lexi_mesh);

    as_tri_edge_implicit_mesh_set_all_tri_neighbor(tei_mesh);

    adt_tri_edge_implicit_mesh_set_Delaunay_triangulation_flip_algorithm(&tei_mesh);

    as_tri_implicit_mesh_free(ti_lexi_mesh);

    float min_edge_len = as_tri_edge_implicit_mesh_get_min_edge_length(tei_mesh);
    if (min_edge_len < ADT_EPSILON) {
        fprintf(stderr, "%s:%s:%d:\n[Warning] shortest edge is smaller then 'ADT_EPSILON' (%g).\n\n", __FILE__, __func__, __LINE__, ADT_EPSILON);
    }

    return tei_mesh;
}

Tri_edge_implicit_mesh adt_tri_edge_implicit_mesh_make_Delaunay_triangulation_flip_algorithm_fixed_iterations(Point *c, const size_t len)
{
    Tri_implicit_mesh ti_lexi_mesh = as_points_array_get_lexicographic_triangulation(c, len);

    Tri_edge_implicit_mesh temp_tei_mesh = as_tri_implicit_mesh_to_tri_edge_implicit_mesh(ti_lexi_mesh);

    adt_tri_edge_implicit_mesh_set_Delaunay_triangulation_flip_algorithm_fixed_iterations(&temp_tei_mesh);

    as_tri_implicit_mesh_free(ti_lexi_mesh);

    return temp_tei_mesh;
}

void adt_tri_edge_implicit_mesh_set_Delaunay_triangulation_flip_algorithm(Tri_edge_implicit_mesh *mesh)
{
    Tri_edge_implicit_mesh tei_temp_mesh = *mesh;
    Edge_implicit_ada edge_list = {0};
    ada_init_array(Edge_implicit, edge_list);
    
    for (size_t i = 0; i < tei_temp_mesh.edges.length; i++) {
        ada_appand(Edge_implicit, edge_list, tei_temp_mesh.edges.elements[i]);
    }

    printf("[INFO] Delaunay triangulation:\n\n");
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
        #if 0
        Tri_edge_implicit tri1 = {0}, tri2 = {0};
        as_tri_edge_implicit_mesh_get_triangles_with_edge(tei_temp_mesh, tei_temp_mesh.points.elements[new_edge.p1_index], tei_temp_mesh.points.elements[new_edge.p2_index], &tri1, &tri2); 
        #else
        Tri_edge_implicit tri1 = tei_temp_mesh.triangles.elements[tei_temp_mesh.triangles.length-1];
        Tri_edge_implicit tri2 = tei_temp_mesh.triangles.elements[tei_temp_mesh.triangles.length-2];
        (void)new_edge;
        #endif
        for (int j = 0; j < 3; j++) {
            ada_appand(Edge_implicit, edge_list, tei_temp_mesh.edges.elements[tri1.edges_index[j]]);
            ada_appand(Edge_implicit, edge_list, tei_temp_mesh.edges.elements[tri2.edges_index[j]]);
        }
    }

    printf("\n");
    
    free(edge_list.elements);

    *mesh = tei_temp_mesh;
}

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


