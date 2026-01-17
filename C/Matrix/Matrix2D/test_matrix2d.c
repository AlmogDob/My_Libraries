/**
 * tests/test_matrix2d.c
 * 
 * written by AI
 * 
 */
#define MATRIX2D_IMPLEMENTATION
#include "Matrix2D.h"
#include <unistd.h> /* dup/dup2/close for silencing stdout in print-smoke tests */

/* -------------------------------------------------------------------------- */
/* Deterministic fuzz helpers (no libc rand(); stable across platforms)        */
/* -------------------------------------------------------------------------- */

static uint64_t xorshift64star(uint64_t *state)
{
    /* Marsaglia xorshift* variant */
    uint64_t x = *state;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    *state = x;
    return x * 2685821657736338717ULL;
}

static double rng_unit01(uint64_t *state)
{
    /* Convert top 53 bits to a double in [0, 1). */
    uint64_t x = xorshift64star(state);
    uint64_t top53 = x >> 11;
    return (double)top53 * (1.0 / 9007199254740992.0); /* 2^53 */
}

static double rng_range(uint64_t *state, double low, double high)
{
    return low + (high - low) * rng_unit01(state);
}

static int close_rel_abs(double a, double b, double abs_eps, double rel_eps)
{
    double diff = fabs(a - b);
    if (diff <= abs_eps) return 1;
    return diff <= rel_eps * fmax(fabs(a), fabs(b));
}

static void fill_strictly_diag_dominant(Mat2D a, uint64_t *rng)
{
    /* Strict diagonal dominance => nonsingular (and usually stable to invert). */
    MAT2D_ASSERT(a.rows == a.cols);
    double mag = 2.0;

    for (size_t i = 0; i < a.rows; i++) {
        double row_sum = 0.0;
        for (size_t j = 0; j < a.cols; j++) {
            if (i == j) continue;
            double v = rng_range(rng, -mag, mag);
            MAT2D_AT(a, i, j) = v;
            row_sum += fabs(v);
        }
        MAT2D_AT(a, i, i) = row_sum + 1.0;
    }
}


static double det_by_minors_first_col(Mat2D a);

static int nearly_equal(double a, double b, double eps)
{
    return fabs(a - b) <= eps;
}

static void assert_mat_close(Mat2D a, Mat2D b, double eps)
{
    MAT2D_ASSERT(a.rows == b.rows);
    MAT2D_ASSERT(a.cols == b.cols);

    for (size_t i = 0; i < a.rows; i++) {
        for (size_t j = 0; j < a.cols; j++) {
            double va = MAT2D_AT(a, i, j);
            double vb = MAT2D_AT(b, i, j);
            if (!nearly_equal(va, vb, eps)) {
                fprintf(stderr,
                        "Matrix mismatch at (%zu,%zu): a=%g b=%g\n",
                        i,
                        j,
                        va,
                        vb);
                MAT2D_ASSERT(0);
            }
        }
    }
}

static void assert_identity_close(Mat2D m, double eps)
{
    MAT2D_ASSERT(m.rows == m.cols);
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            double expected = (i == j) ? 1.0 : 0.0;
            MAT2D_ASSERT(nearly_equal(MAT2D_AT(m, i, j), expected, eps));
        }
    }
}

static void fill_mat_from_array(Mat2D m, const double *data)
{
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            MAT2D_AT(m, i, j) = data[i * m.cols + j];
        }
    }
}

static void assert_permutation_matrix(Mat2D p)
{
    MAT2D_ASSERT(p.rows == p.cols);

    for (size_t i = 0; i < p.rows; i++) {
        double row_sum = 0.0;
        for (size_t j = 0; j < p.cols; j++) {
            double v = MAT2D_AT(p, i, j);
            MAT2D_ASSERT(v == 0.0 || v == 1.0);
            row_sum += v;
        }
        MAT2D_ASSERT(nearly_equal(row_sum, 1.0, 0.0));
    }

    for (size_t j = 0; j < p.cols; j++) {
        double col_sum = 0.0;
        for (size_t i = 0; i < p.rows; i++) {
            col_sum += MAT2D_AT(p, i, j);
        }
        MAT2D_ASSERT(nearly_equal(col_sum, 1.0, 0.0));
    }

    /* det(P) must be ±1 for a permutation matrix */
    double dp = mat2D_det(p);
    MAT2D_ASSERT(nearly_equal(fabs(dp), 1.0, MAT2D_EPS));
}

static void assert_inverse_identity_both_sides(Mat2D a, double eps)
{
    MAT2D_ASSERT(a.rows == a.cols);

    Mat2D inv = mat2D_alloc(a.rows, a.cols);
    Mat2D prod1 = mat2D_alloc(a.rows, a.cols);
    Mat2D prod2 = mat2D_alloc(a.rows, a.cols);

    mat2D_invert(inv, a);

    /* A * inv(A) == I */
    mat2D_dot(prod1, a, inv);
    assert_identity_close(prod1, eps);

    /* inv(A) * A == I (catches some subtle multiply/invert issues) */
    mat2D_dot(prod2, inv, a);
    assert_identity_close(prod2, eps);

    mat2D_free(inv);
    mat2D_free(prod1);
    mat2D_free(prod2);
}

static void test_alloc_fill_copy_add_sub(void)
{
    Mat2D a = mat2D_alloc(2, 3);
    Mat2D b = mat2D_alloc(2, 3);
    Mat2D c = mat2D_alloc(2, 3);

    mat2D_fill(a, 1.5);
    mat2D_fill(b, 2.0);

    mat2D_copy(c, a);
    mat2D_add(c, b); // c = 3.5
    for (size_t i = 0; i < c.rows; i++) {
        for (size_t j = 0; j < c.cols; j++) {
            MAT2D_ASSERT(nearly_equal(MAT2D_AT(c, i, j), 3.5, 0.0));
        }
    }

    mat2D_sub(c, b); // c back to 1.5
    assert_mat_close(c, a, 0.0);

    mat2D_free(a);
    mat2D_free(b);
    mat2D_free(c);
}

static void test_transpose(void)
{
    Mat2D a = mat2D_alloc(2, 3);
    Mat2D t = mat2D_alloc(3, 2);

    // a =
    // [1 2 3
    //  4 5 6]
    double v = 1.0;
    for (size_t i = 0; i < a.rows; i++) {
        for (size_t j = 0; j < a.cols; j++) {
            MAT2D_AT(a, i, j) = v++;
        }
    }

    mat2D_transpose(t, a);

    MAT2D_ASSERT(nearly_equal(MAT2D_AT(t, 0, 0), 1.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(t, 1, 0), 2.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(t, 2, 0), 3.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(t, 0, 1), 4.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(t, 1, 1), 5.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(t, 2, 1), 6.0, 0.0));

    mat2D_free(a);
    mat2D_free(t);
}

static void test_dot_product_matrix_multiply(void)
{
    Mat2D a = mat2D_alloc(2, 3);
    Mat2D b = mat2D_alloc(3, 2);
    Mat2D c = mat2D_alloc(2, 2);

    // a =
    // [1 2 3
    //  4 5 6]
    MAT2D_AT(a, 0, 0) = 1;
    MAT2D_AT(a, 0, 1) = 2;
    MAT2D_AT(a, 0, 2) = 3;
    MAT2D_AT(a, 1, 0) = 4;
    MAT2D_AT(a, 1, 1) = 5;
    MAT2D_AT(a, 1, 2) = 6;

    // b =
    // [ 7  8
    //   9 10
    //  11 12]
    MAT2D_AT(b, 0, 0) = 7;
    MAT2D_AT(b, 0, 1) = 8;
    MAT2D_AT(b, 1, 0) = 9;
    MAT2D_AT(b, 1, 1) = 10;
    MAT2D_AT(b, 2, 0) = 11;
    MAT2D_AT(b, 2, 1) = 12;

    mat2D_dot(c, a, b);

    // Expected:
    // [ 58  64
    //  139 154]
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(c, 0, 0), 58.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(c, 0, 1), 64.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(c, 1, 0), 139.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(c, 1, 1), 154.0, 0.0));

    mat2D_free(a);
    mat2D_free(b);
    mat2D_free(c);
}

static void test_det_and_minor_det_agree_3x3(void)
{
    Mat2D a = mat2D_alloc(3, 3);

    // Classic example det = -306
    // [  6   1   1
    //    4  -2   5
    //    2   8   7 ]
    MAT2D_AT(a, 0, 0) = 6;
    MAT2D_AT(a, 0, 1) = 1;
    MAT2D_AT(a, 0, 2) = 1;
    MAT2D_AT(a, 1, 0) = 4;
    MAT2D_AT(a, 1, 1) = -2;
    MAT2D_AT(a, 1, 2) = 5;
    MAT2D_AT(a, 2, 0) = 2;
    MAT2D_AT(a, 2, 1) = 8;
    MAT2D_AT(a, 2, 2) = 7;

    double det_gauss = mat2D_det(a);
    double det_minor = det_by_minors_first_col(a);

    MAT2D_ASSERT(nearly_equal(det_gauss, -306.0, MAT2D_EPS));
    MAT2D_ASSERT(nearly_equal(det_minor, -306.0, MAT2D_EPS));
    MAT2D_ASSERT(nearly_equal(det_minor, det_gauss, MAT2D_EPS));

    mat2D_free(a);
}

static void test_invert(void)
{
    /*
     * Adversarial inversion set:
     *  - a “normal” matrix
     *  - a matrix with a zero leading pivot (forces pivoting / row swaps)
     *  - an ill-conditioned-ish matrix (still invertible) to stress Gauss-Jordan
     */
    struct {
        const char *name;
        double eps;
        double data[9];
    } cases[] = {
        {
            "baseline det=3",
            MAT2D_EPS,
            {4, 7, 2, 3, 6, 1, 2, 5, 1},
        },
        {
            "forces row-swap pivoting (zero leading pivot), det=2",
            MAT2D_EPS,
            {0, 1, 1, 1, 0, 1, 1, 1, 0},
        },
        {
            "hilbert-3x3 (ill-conditioned-ish)",
            1e-10,
            {1.0, 1.0 / 2.0, 1.0 / 3.0, 1.0 / 2.0, 1.0 / 3.0, 1.0 / 4.0, 1.0 / 3.0, 1.0 / 4.0, 1.0 / 5.0},
        },
    };

    for (size_t k = 0; k < sizeof(cases) / sizeof(cases[0]); k++) {
        Mat2D a = mat2D_alloc(3, 3);
        fill_mat_from_array(a, cases[k].data);

        /* basic sanity: det should be finite and not ~0 for these cases */
        double d = mat2D_det(a);
        MAT2D_ASSERT(isfinite(d));
        MAT2D_ASSERT(fabs(d) > MAT2D_EPS);

        assert_inverse_identity_both_sides(a, cases[k].eps);
        mat2D_free(a);
    }
}

static void test_LUP_decomposition_identity_P_no_swap_case(void)
{
    const double eps = MAT2D_EPS;

    Mat2D a = mat2D_alloc(3, 3);
    Mat2D l = mat2D_alloc(3, 3);
    Mat2D p = mat2D_alloc(3, 3);
    Mat2D u = mat2D_alloc(3, 3);

    Mat2D pa = mat2D_alloc(3, 3);
    Mat2D lu = mat2D_alloc(3, 3);

    // Same matrix as invert test (should not need swaps in typical flow)
    MAT2D_AT(a, 0, 0) = 4;
    MAT2D_AT(a, 0, 1) = 7;
    MAT2D_AT(a, 0, 2) = 2;
    MAT2D_AT(a, 1, 0) = 3;
    MAT2D_AT(a, 1, 1) = 6;
    MAT2D_AT(a, 1, 2) = 1;
    MAT2D_AT(a, 2, 0) = 2;
    MAT2D_AT(a, 2, 1) = 5;
    MAT2D_AT(a, 2, 2) = 1;

    mat2D_LUP_decomposition_with_swap(a, l, p, u);

    mat2D_dot(pa, p, a);
    mat2D_dot(lu, l, u);

    assert_mat_close(pa, lu, eps);

    mat2D_free(a);
    mat2D_free(l);
    mat2D_free(p);
    mat2D_free(u);
    mat2D_free(pa);
    mat2D_free(lu);
}

static void test_LUP_decomposition_swap_required_case(void)
{
    const double eps = MAT2D_EPS;

    Mat2D a = mat2D_alloc(3, 3);
    Mat2D l = mat2D_alloc(3, 3);
    Mat2D p = mat2D_alloc(3, 3);
    Mat2D u = mat2D_alloc(3, 3);

    Mat2D pa = mat2D_alloc(3, 3);
    Mat2D lu = mat2D_alloc(3, 3);

    /*
     * This matrix is invertible but has a guaranteed zero pivot at (0,0),
     * so LUP must swap rows to proceed.
     * A =
     * [0 1 1
     *  1 0 1
     *  1 1 0]
     */
    double data[9] = {0, 1, 1, 1, 0, 1, 1, 1, 0};
    fill_mat_from_array(a, data);

    mat2D_LUP_decomposition_with_swap(a, l, p, u);
    assert_permutation_matrix(p);

    mat2D_dot(pa, p, a);
    mat2D_dot(lu, l, u);
    assert_mat_close(pa, lu, eps);

    mat2D_free(a);
    mat2D_free(l);
    mat2D_free(p);
    mat2D_free(u);
    mat2D_free(pa);
    mat2D_free(lu);
}

static void test_uint32_alloc_fill_and_at(void)
{
    Mat2D_uint32 m = mat2D_alloc_uint32(2, 3);
    mat2D_fill_uint32(m, 42);
    MAT2D_AT_UINT32(m, 0, 1) = 7;

    MAT2D_ASSERT(MAT2D_AT_UINT32(m, 0, 0) == 42);
    MAT2D_ASSERT(MAT2D_AT_UINT32(m, 0, 1) == 7);
    MAT2D_ASSERT(MAT2D_AT_UINT32(m, 1, 2) == 42);

    mat2D_free_uint32(m);
}

static void test_offset2d_and_stride(void)
{
    Mat2D a = mat2D_alloc(3, 4);
    mat2D_fill_sequence(a, 0.0, 1.0); // a[i,j] = offset(i,j)

    MAT2D_ASSERT(mat2D_offset2d(a, 0, 0) == 0);
    MAT2D_ASSERT(mat2D_offset2d(a, 0, 3) == 3);
    MAT2D_ASSERT(mat2D_offset2d(a, 1, 0) == 4);
    MAT2D_ASSERT(mat2D_offset2d(a, 2, 3) == 11);

    MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, 2, 3), 11.0, 0.0));

    mat2D_free(a);
}

static void test_copy_windows(void)
{
    Mat2D des = mat2D_alloc(4, 5);
    Mat2D src = mat2D_alloc(2, 2);
    Mat2D got = mat2D_alloc(2, 2);

    mat2D_fill(des, -1.0);
    MAT2D_AT(src, 0, 0) = 1.0;
    MAT2D_AT(src, 0, 1) = 2.0;
    MAT2D_AT(src, 1, 0) = 3.0;
    MAT2D_AT(src, 1, 1) = 4.0;

    // Copy src into des window rows [1..2], cols [2..3]
    mat2D_copy_src_to_des_window(des, src, 1, 2, 2, 3);

    // Check placement and untouched area
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(des, 0, 0), -1.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(des, 1, 2), 1.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(des, 1, 3), 2.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(des, 2, 2), 3.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(des, 2, 3), 4.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(des, 3, 4), -1.0, 0.0));

    // Copy the same window back out into got and compare to src
    mat2D_copy_src_window_to_des(got, des, 1, 2, 2, 3);
    assert_mat_close(got, src, 0.0);

    mat2D_free(des);
    mat2D_free(src);
    mat2D_free(got);
}
 
static void test_non_contiguous_stride_views(void)
{
    /*
     * Many bugs only show up when stride_r != cols.
     * We build a padded (non-contiguous) view and make sure core ops use stride correctly.
     */
    const size_t rows = 2;
    const size_t cols = 2;
    const size_t stride = 7; /* intentional padding */

    double *buf = (double *)MAT2D_MALLOC(sizeof(double) * rows * stride);
    MAT2D_ASSERT(buf != NULL);

    /* poison padding so accidental reads are obvious */
    for (size_t i = 0; i < rows * stride; i++) {
        buf[i] = 1234567.0;
    }

    Mat2D a = {.rows = rows, .cols = cols, .stride_r = stride, .elements = buf};

    MAT2D_AT(a, 0, 0) = 1.0;
    MAT2D_AT(a, 0, 1) = 2.0;
    MAT2D_AT(a, 1, 0) = 3.0;
    MAT2D_AT(a, 1, 1) = 4.0;

    MAT2D_ASSERT(mat2D_offset2d(a, 1, 1) == 1 * stride + 1);

    Mat2D c = mat2D_alloc(2, 2);
    Mat2D t = mat2D_alloc(2, 2);
    Mat2D prod = mat2D_alloc(2, 2);
    Mat2D id = mat2D_alloc(2, 2);

    mat2D_copy(c, a);
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(c, 1, 1), 4.0, 0.0));

    mat2D_transpose(t, a);
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(t, 0, 1), 3.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(t, 1, 0), 2.0, 0.0));

    mat2D_set_identity(id);
    mat2D_dot(prod, a, id);
    assert_mat_close(prod, c, 0.0);

    mat2D_free(c);
    mat2D_free(t);
    mat2D_free(prod);
    mat2D_free(id);
    MAT2D_FREE(buf);
}

static void test_row_col_ops_and_scaling(void)
{
    Mat2D a = mat2D_alloc(3, 3);
    mat2D_fill_sequence(a, 1.0, 1.0);
    // a =
    // [1 2 3
    //  4 5 6
    //  7 8 9]

    // add_row_to_row: row0 += row1 => [5 7 9]
    mat2D_add_row_to_row(a, 0, a, 1);
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, 0, 0), 5.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, 0, 1), 7.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, 0, 2), 9.0, 0.0));

    // sub_row_to_row: row2 -= row1 => [3 3 3]
    mat2D_sub_row_to_row(a, 2, a, 1);
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, 2, 0), 3.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, 2, 1), 3.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, 2, 2), 3.0, 0.0));

    // add_col_to_col: col1 += col2 (operate on current a)
    mat2D_add_col_to_col(a, 1, a, 2);
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, 1, 1), 5.0 + 6.0, 0.0));

    // sub_col_to_col: col1 -= col2 => restore original col1 for that row
    mat2D_sub_col_to_col(a, 1, a, 2);
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, 1, 1), 5.0, 0.0));

    // Row op: row1 += 2 * row2  (row2 currently [3 3 3])
    mat2D_add_row_time_factor_to_row(a, 1, 2, 2.0);
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, 1, 0), 4.0 + 2.0 * 3.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, 1, 1), 5.0 + 2.0 * 3.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, 1, 2), 6.0 + 2.0 * 3.0, 0.0));

    // Row scale
    mat2D_mult_row(a, 2, -1.0);
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, 2, 0), -3.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, 2, 1), -3.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, 2, 2), -3.0, 0.0));

    mat2D_free(a);
}

static void test_shift_and_identity(void)
{
    Mat2D a = mat2D_alloc(4, 4);
    mat2D_set_identity(a);
    mat2D_shift(a, 2.0); // diag becomes 3

    for (size_t i = 0; i < a.rows; i++) {
        for (size_t j = 0; j < a.cols; j++) {
            double expected = (i == j) ? 3.0 : 0.0;
            MAT2D_ASSERT(nearly_equal(MAT2D_AT(a, i, j), expected, 0.0));
        }
    }

    mat2D_free(a);
}

static void test_norms_and_normalize(void)
{
    const double eps = MAT2D_EPS;

    Mat2D v = mat2D_alloc(2, 1);
    MAT2D_AT(v, 0, 0) = 3.0;
    MAT2D_AT(v, 1, 0) = 4.0;

    MAT2D_ASSERT(nearly_equal(mat2D_calc_norma(v), 5.0, eps));
    MAT2D_ASSERT(nearly_equal(mat2D_calc_norma_inf(v), 4.0, eps));
    MAT2D_ASSERT(nearly_equal(mat2D_inner_product(v), 25.0, eps));

    mat2D_normalize(v);
    MAT2D_ASSERT(nearly_equal(mat2D_calc_norma(v), 1.0, MAT2D_EPS));

    mat2D_free(v);
}

static void test_outer_product_and_cross(void)
{
    const double eps = MAT2D_EPS;

    Mat2D v = mat2D_alloc(3, 1);
    Mat2D out = mat2D_alloc(3, 3);

    MAT2D_AT(v, 0, 0) = 1.0;
    MAT2D_AT(v, 1, 0) = 2.0;
    MAT2D_AT(v, 2, 0) = 3.0;

    mat2D_outer_product(out, v);
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(out, 0, 0), 1.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(out, 0, 1), 2.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(out, 0, 2), 3.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(out, 1, 0), 2.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(out, 1, 1), 4.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(out, 1, 2), 6.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(out, 2, 2), 9.0, eps));

    // Cross: i x j = k
    Mat2D i = mat2D_alloc(3, 1);
    Mat2D j = mat2D_alloc(3, 1);
    Mat2D k = mat2D_alloc(3, 1);
    mat2D_fill(i, 0.0);
    mat2D_fill(j, 0.0);
    mat2D_fill(k, 0.0);
    MAT2D_AT(i, 0, 0) = 1.0;
    MAT2D_AT(j, 1, 0) = 1.0;

    mat2D_cross(k, i, j);
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(k, 0, 0), 0.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(k, 1, 0), 0.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(k, 2, 0), 1.0, eps));

    mat2D_free(v);
    mat2D_free(out);
    mat2D_free(i);
    mat2D_free(j);
    mat2D_free(k);
}

static void test_det_2x2_and_upper_triangulate_sign(void)
{
    Mat2D a = mat2D_alloc(2, 2);
    MAT2D_AT(a, 0, 0) = 0.0;
    MAT2D_AT(a, 0, 1) = 1.0;
    MAT2D_AT(a, 1, 0) = 2.0;
    MAT2D_AT(a, 1, 1) = 3.0;

    // det should be -2
    MAT2D_ASSERT(nearly_equal(mat2D_det_2x2_mat(a), -2.0, 0.0));
    MAT2D_ASSERT(nearly_equal(mat2D_det(a), -2.0, MAT2D_EPS));

    // upper triangulation should have exactly one row swap => factor = -1
    Mat2D tmp = mat2D_alloc(2, 2);
    mat2D_copy(tmp, a);
    double f = mat2D_upper_triangulate(tmp, MAT2D_ROW_SWAPPING);
    MAT2D_ASSERT(nearly_equal(f, -1.0, 0.0));

    mat2D_free(a);
    mat2D_free(tmp);
}

static double det_by_minors_first_col(Mat2D a)
{
    MAT2D_ASSERT(a.rows == a.cols);
    size_t n = a.rows;

    if (n == 1) {
        return MAT2D_AT(a, 0, 0);
    }
    if (n == 2) {
        return mat2D_det_2x2_mat(a);
    }

    double det = 0.0;
    size_t j = 0;
    for (size_t i = 0; i < n; i++) {
        double aij = MAT2D_AT(a, i, j);
        if (aij == 0.0) {
            continue;
        }

        Mat2D_Minor mm = mat2D_minor_alloc_fill_from_mat(a, i, j);
        double minor_det = 0.0;
        if (mm.rows == 2 && mm.cols == 2) {
            minor_det = mat2D_det_2x2_mat_minor(mm);
        } else {
            minor_det = mat2D_minor_det(mm);
        }
        mat2D_minor_free(mm);

        double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
        det += aij * sign * minor_det;
    }

    return det;
}

static void test_minor_det_matches_gauss_4x4_known(void)
{
    const double eps = 1e-9;

    Mat2D a = mat2D_alloc(4, 4);
    // det = 72
    // [1 2 3 4
    //  5 6 7 8
    //  2 6 4 8
    //  3 1 1 2]
    double data[16] = {
        1, 2, 3, 4, //
        5, 6, 7, 8, //
        2, 6, 4, 8, //
        3, 1, 1, 2  //
    };
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            MAT2D_AT(a, i, j) = data[i * 4 + j];
        }
    }

    double det_gauss = mat2D_det(a);
    double det_minor = det_by_minors_first_col(a);

    MAT2D_ASSERT(nearly_equal(det_gauss, 72.0, eps));
    MAT2D_ASSERT(nearly_equal(det_minor, 72.0, eps));
    MAT2D_ASSERT(nearly_equal(det_minor, det_gauss, eps));

    mat2D_free(a);
}

static void test_reduce_rank(void)
{
    Mat2D a = mat2D_alloc(3, 3);
    // rank 2:
    // [1 2 3
    //  2 4 6
    //  1 1 1]
    MAT2D_AT(a, 0, 0) = 1;
    MAT2D_AT(a, 0, 1) = 2;
    MAT2D_AT(a, 0, 2) = 3;
    MAT2D_AT(a, 1, 0) = 2;
    MAT2D_AT(a, 1, 1) = 4;
    MAT2D_AT(a, 1, 2) = 6;
    MAT2D_AT(a, 2, 0) = 1;
    MAT2D_AT(a, 2, 1) = 1;
    MAT2D_AT(a, 2, 2) = 1;

    size_t r = mat2D_reduce(a);
    MAT2D_ASSERT(r == 2);

    mat2D_free(a);
}

static void test_rotation_matrices_orthonormal(void)
{
    const double eps = 1e-7;

    Mat2D r = mat2D_alloc(3, 3);
    Mat2D rt = mat2D_alloc(3, 3);
    Mat2D prod = mat2D_alloc(3, 3);

    // Z rotation 90deg, per library's convention:
    // [ 0  1  0
    //  -1  0  0
    //   0  0  1 ]
    mat2D_set_rot_mat_z(r, 90.0f);
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(r, 0, 0), 0.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(r, 0, 1), 1.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(r, 1, 0), -1.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(r, 1, 1), 0.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(r, 2, 2), 1.0, eps));

    mat2D_transpose(rt, r);
    mat2D_dot(prod, rt, r);
    assert_identity_close(prod, MAT2D_EPS);
    MAT2D_ASSERT(nearly_equal(mat2D_det(r), 1.0, MAT2D_EPS));

    // X rotation 90deg should also be orthonormal with det ~ 1
    mat2D_set_rot_mat_x(r, 90.0f);
    mat2D_transpose(rt, r);
    mat2D_dot(prod, rt, r);
    assert_identity_close(prod, MAT2D_EPS);
    MAT2D_ASSERT(nearly_equal(mat2D_det(r), 1.0, MAT2D_EPS));

    // Y rotation 90deg orthonormal with det ~ 1
    mat2D_set_rot_mat_y(r, 90.0f);
    mat2D_transpose(rt, r);
    mat2D_dot(prod, rt, r);
    assert_identity_close(prod, MAT2D_EPS);
    MAT2D_ASSERT(nearly_equal(mat2D_det(r), 1.0, MAT2D_EPS));

    mat2D_free(r);
    mat2D_free(rt);
    mat2D_free(prod);
}

static void test_DCM_zyx_matches_product(void)
{
    const double eps = MAT2D_EPS;

    Mat2D dcm = mat2D_alloc(3, 3);
    Mat2D rz = mat2D_alloc(3, 3);
    Mat2D ry = mat2D_alloc(3, 3);
    Mat2D rx = mat2D_alloc(3, 3);
    Mat2D tmp = mat2D_alloc(3, 3);
    Mat2D expected = mat2D_alloc(3, 3);

    float yaw = 30.0f;
    float pitch = 20.0f;
    float roll = 10.0f;

    mat2D_set_DCM_zyx(dcm, yaw, pitch, roll);

    mat2D_set_rot_mat_z(rz, yaw);
    mat2D_set_rot_mat_y(ry, pitch);
    mat2D_set_rot_mat_x(rx, roll);
    mat2D_dot(tmp, ry, rz);
    mat2D_dot(expected, rx, tmp);

    assert_mat_close(dcm, expected, eps);

    mat2D_free(dcm);
    mat2D_free(rz);
    mat2D_free(ry);
    mat2D_free(rx);
    mat2D_free(tmp);
    mat2D_free(expected);
}

static void test_solve_linear_system_LUP(void)
{
    const double eps = MAT2D_EPS;

    Mat2D a = mat2D_alloc(3, 3);
    Mat2D x = mat2D_alloc(3, 1);
    Mat2D b = mat2D_alloc(3, 1);
    Mat2D ax = mat2D_alloc(3, 1);

    // A =
    // [3 0  2
    //  2 0 -2
    //  0 1  1]
    MAT2D_AT(a, 0, 0) = 3;
    MAT2D_AT(a, 0, 1) = 0;
    MAT2D_AT(a, 0, 2) = 2;
    MAT2D_AT(a, 1, 0) = 2;
    MAT2D_AT(a, 1, 1) = 0;
    MAT2D_AT(a, 1, 2) = -2;
    MAT2D_AT(a, 2, 0) = 0;
    MAT2D_AT(a, 2, 1) = 1;
    MAT2D_AT(a, 2, 2) = 1;

    // Choose x_true = [1,2,3]^T, so b = A*x_true = [9, -4, 5]^T
    MAT2D_AT(b, 0, 0) = 9;
    MAT2D_AT(b, 1, 0) = -4;
    MAT2D_AT(b, 2, 0) = 5;

    mat2D_solve_linear_sys_LUP_decomposition(a, x, b);
    mat2D_dot(ax, a, x);

    MAT2D_ASSERT(nearly_equal(MAT2D_AT(ax, 0, 0), 9.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(ax, 1, 0), -4.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(ax, 2, 0), 5.0, eps));

    mat2D_free(a);
    mat2D_free(x);
    mat2D_free(b);
    mat2D_free(ax);
}

static void test_rand_range(void)
{
    Mat2D a = mat2D_alloc(4, 4);
    srand(1);
    mat2D_rand(a, -2.0, 5.0);

    for (size_t i = 0; i < a.rows; i++) {
        for (size_t j = 0; j < a.cols; j++) {
            double v = MAT2D_AT(a, i, j);
            MAT2D_ASSERT(v >= -2.0);
            MAT2D_ASSERT(v <= 5.0);
        }
    }

    mat2D_free(a);
}

static void test_copy_row_and_col_helpers(void)
{
    Mat2D src = mat2D_alloc(3, 3);
    Mat2D des = mat2D_alloc(3, 3);

    /*
     * src =
     * [1 2 3
     *  4 5 6
     *  7 8 9]
     */
    mat2D_fill_sequence(src, 1.0, 1.0);
    mat2D_fill(des, 0.0);

    /* copy row 1 => des[1,:] = src[1,:] */
    mat2D_copy_row_from_src_to_des(des, 1, src, 1);
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(des, 1, 0), 4.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(des, 1, 1), 5.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(des, 1, 2), 6.0, 0.0));

    /* copy col 2 => des[:,2] = src[:,2] */
    mat2D_copy_col_from_src_to_des(des, 2, src, 2);
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(des, 0, 2), 3.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(des, 1, 2), 6.0, 0.0));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(des, 2, 2), 9.0, 0.0));

    mat2D_free(src);
    mat2D_free(des);
}

static void test_dot_product_and_vector_variants(void)
{
    const double eps = 1e-12;

    /* Column vectors */
    Mat2D a = mat2D_alloc(3, 1);
    Mat2D b = mat2D_alloc(3, 1);
    MAT2D_AT(a, 0, 0) = 1.0;
    MAT2D_AT(a, 1, 0) = 2.0;
    MAT2D_AT(a, 2, 0) = 3.0;
    MAT2D_AT(b, 0, 0) = 4.0;
    MAT2D_AT(b, 1, 0) = 5.0;
    MAT2D_AT(b, 2, 0) = 6.0;

    MAT2D_ASSERT(nearly_equal(mat2D_dot_product(a, b), 32.0, eps));
    MAT2D_ASSERT(nearly_equal(mat2D_inner_product(a), 14.0, eps));

    /* Row vectors (exercise the other branch in dot_product/inner_product) */
    Mat2D ar = mat2D_alloc(1, 3);
    Mat2D br = mat2D_alloc(1, 3);
    MAT2D_AT(ar, 0, 0) = 1.0;
    MAT2D_AT(ar, 0, 1) = 2.0;
    MAT2D_AT(ar, 0, 2) = 3.0;
    MAT2D_AT(br, 0, 0) = 4.0;
    MAT2D_AT(br, 0, 1) = 5.0;
    MAT2D_AT(br, 0, 2) = 6.0;

    MAT2D_ASSERT(nearly_equal(mat2D_dot_product(ar, br), 32.0, eps));
    MAT2D_ASSERT(nearly_equal(mat2D_inner_product(ar), 14.0, eps));

    /* Normalize-by-inf macro (smoke) */
    MAT2D_ASSERT(nearly_equal(mat2D_calc_norma_inf(ar), 3.0, eps));
    mat2D_normalize_inf(ar);
    MAT2D_ASSERT(nearly_equal(mat2D_calc_norma_inf(ar), 1.0, 1e-12));

    mat2D_free(a);
    mat2D_free(b);
    mat2D_free(ar);
    mat2D_free(br);
}

static void test_outer_product_row_vector_path(void)
{
    const double eps = 1e-12;

    Mat2D v = mat2D_alloc(1, 3);   /* row-vector form */
    Mat2D out = mat2D_alloc(3, 3);

    MAT2D_AT(v, 0, 0) = 1.0;
    MAT2D_AT(v, 0, 1) = 2.0;
    MAT2D_AT(v, 0, 2) = 3.0;

    mat2D_outer_product(out, v);

    /* out[i,j] = v[i]*v[j] (with row-vector indexing) */
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(out, 0, 0), 1.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(out, 0, 1), 2.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(out, 0, 2), 3.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(out, 1, 0), 2.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(out, 1, 1), 4.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(out, 1, 2), 6.0, eps));
    MAT2D_ASSERT(nearly_equal(MAT2D_AT(out, 2, 2), 9.0, eps));

    mat2D_free(v);
    mat2D_free(out);
}

static void test_det_early_zero_row_and_zero_col_paths(void)
{
    /* Exercise mat2D_det() early return when it finds an all-zero row/col */

    Mat2D zr = mat2D_alloc(2, 2);
    /* row0 all zeros => det 0 */
    MAT2D_AT(zr, 0, 0) = 0.0;
    MAT2D_AT(zr, 0, 1) = 0.0;
    MAT2D_AT(zr, 1, 0) = 1.0;
    MAT2D_AT(zr, 1, 1) = 2.0;
    MAT2D_ASSERT(mat2D_row_is_all_digit(zr, 0.0, 0));
    MAT2D_ASSERT(nearly_equal(mat2D_det(zr), 0.0, 0.0));

    Mat2D zc = mat2D_alloc(2, 2);
    /* col0 all zeros but no all-zero row => det 0 */
    MAT2D_AT(zc, 0, 0) = 0.0;
    MAT2D_AT(zc, 0, 1) = 1.0;
    MAT2D_AT(zc, 1, 0) = 0.0;
    MAT2D_AT(zc, 1, 1) = 2.0;
    MAT2D_ASSERT(mat2D_col_is_all_digit(zc, 0.0, 0));
    MAT2D_ASSERT(nearly_equal(mat2D_det(zc), 0.0, 0.0));

    mat2D_free(zr);
    mat2D_free(zc);
}

static void test_mat_is_all_digit(void)
{
    Mat2D m = mat2D_alloc(2, 3);
    mat2D_fill(m, 7.0);
    MAT2D_ASSERT(mat2D_mat_is_all_digit(m, 7.0));
    MAT2D_ASSERT(!mat2D_mat_is_all_digit(m, 8.0));
    MAT2D_AT(m, 1, 2) = 8.0;
    MAT2D_ASSERT(!mat2D_mat_is_all_digit(m, 7.0));
    mat2D_free(m);
}

static void test_power_iterate_and_eig_helpers(void)
{
    /*
     * Use a diagonal matrix with distinct positive eigenvalues:
     * A = diag(5,3,1)
     * Power iteration should find lambda=5 with eigenvector ~ e1.
     * eig_power_iteration should recover (5,3,1) and eig_check residual small.
     */
    const double eps = 1e-7;

    Mat2D A = mat2D_alloc(3, 3);
    mat2D_fill(A, 0.0);
    MAT2D_AT(A, 0, 0) = 5.0;
    MAT2D_AT(A, 1, 1) = 3.0;
    MAT2D_AT(A, 2, 2) = 1.0;

    /* mat2D_power_iterate */
    Mat2D v = mat2D_alloc(3, 1);
    MAT2D_AT(v, 0, 0) = 1.0;
    MAT2D_AT(v, 1, 0) = 1.0;
    MAT2D_AT(v, 2, 0) = 1.0;
    double lambda = 0.0;
    int rc = mat2D_power_iterate(A, v, &lambda, 0.0, true);
    MAT2D_ASSERT(rc == 0);
    MAT2D_ASSERT(close_rel_abs(lambda, 5.0, 1e-6, 1e-6));
    /* dominant component should be index 0 */
    MAT2D_ASSERT(fabs(MAT2D_AT(v, 0, 0)) > fabs(MAT2D_AT(v, 1, 0)));
    MAT2D_ASSERT(fabs(MAT2D_AT(v, 0, 0)) > fabs(MAT2D_AT(v, 2, 0)));

    /* mat2D_eig_power_iteration + mat2D_eig_check */
    Mat2D evals = mat2D_alloc(3, 3);
    Mat2D evecs = mat2D_alloc(3, 3);
    Mat2D res = mat2D_alloc(3, 3);

    /* init vector must be non-zero */
    Mat2D init = mat2D_alloc(3, 1);
    MAT2D_AT(init, 0, 0) = 1.0;
    MAT2D_AT(init, 1, 0) = 1.0;
    MAT2D_AT(init, 2, 0) = 1.0;

    mat2D_eig_power_iteration(A, evals, evecs, init, true);
    MAT2D_ASSERT(close_rel_abs(MAT2D_AT(evals, 0, 0), 5.0, 1e-5, 1e-5));
    MAT2D_ASSERT(close_rel_abs(MAT2D_AT(evals, 1, 1), 3.0, 1e-5, 1e-5));
    MAT2D_ASSERT(close_rel_abs(MAT2D_AT(evals, 2, 2), 1.0, 1e-5, 1e-5));

    mat2D_eig_check(A, evals, evecs, res);
    MAT2D_ASSERT(mat2D_calc_norma_inf(res) < eps);

    mat2D_free(A);
    mat2D_free(v);
    mat2D_free(evals);
    mat2D_free(evecs);
    mat2D_free(res);
    mat2D_free(init);
}

static void test_deterministic_fuzz_loop(void)
{
    /*
     * Deterministic fuzz:
     *  - generate many random (but well-conditioned) square matrices
     *  - reject near-singular ones (fabs(det) too small / non-finite)
     *  - assert invariants:
     *      inv(A) exists and A*inv(A)~I and inv(A)*A~I
     *      det(A^T)~det(A)
     *      det(A)*det(inv(A))~1
     *      rank(A)==N (via mat2D_reduce)
     */

    const size_t iters = 500;
    const double inv_eps = 1e-7;
    const double det_min = 1e-8;
    const double det_abs_eps = 1e-6;
    const double det_rel_eps = 1e-6;

    uint64_t rng = 0x9e3779b97f4a7c15ULL; /* fixed seed */
    size_t tested = 0;
    size_t skipped = 0;

    for (size_t t = 0; t < iters; t++) {
        size_t n = 2 + (size_t)(xorshift64star(&rng) % 49); /* 2..50 */

        Mat2D a = mat2D_alloc(n, n);
        Mat2D inv = mat2D_alloc(n, n);
        Mat2D prod1 = mat2D_alloc(n, n);
        Mat2D prod2 = mat2D_alloc(n, n);
        Mat2D at = mat2D_alloc(n, n);
        Mat2D tmp = mat2D_alloc(n, n);

        fill_strictly_diag_dominant(a, &rng);

        double det_a = mat2D_det(a);
        if (!isfinite(det_a) || fabs(det_a) < det_min) {
            skipped++;
            mat2D_free(a);
            mat2D_free(inv);
            mat2D_free(prod1);
            mat2D_free(prod2);
            mat2D_free(at);
            mat2D_free(tmp);
            continue;
        }

        /* Inversion invariants */
        mat2D_invert(inv, a);
        mat2D_dot(prod1, a, inv);
        assert_identity_close(prod1, inv_eps);
        mat2D_dot(prod2, inv, a);
        assert_identity_close(prod2, inv_eps);

        /* det(A^T) == det(A) */
        mat2D_transpose(at, a);
        double det_at = mat2D_det(at);
        MAT2D_ASSERT(
            close_rel_abs(det_at, det_a, det_abs_eps, det_rel_eps));

        /* det(A) * det(inv(A)) == 1 */
        double det_inv = mat2D_det(inv);
        MAT2D_ASSERT(isfinite(det_inv));
        MAT2D_ASSERT(close_rel_abs(
            det_a * det_inv,
            1.0,
            1e-5,   /* a bit looser: det() is numerically touchy */
            1e-5));

        /* rank(A) == N */
        mat2D_copy(tmp, a);
        size_t r = mat2D_reduce(tmp);
        MAT2D_ASSERT(r == n);

        tested++;

        mat2D_free(a);
        mat2D_free(inv);
        mat2D_free(prod1);
        mat2D_free(prod2);
        mat2D_free(at);
        mat2D_free(tmp);
    }

    /* Make sure the loop actually exercised enough cases. */
    MAT2D_ASSERT(tested >= iters / 2);
    printf("[INFO] fuzz summary: tested=%zu skipped=%zu\n", tested, skipped);
}

int main(void)
{
    #define RUN_TEST(fn)         \
        do {                    \
            fn();               \
            printf("[INFO] passed | %s\n", \
                   #fn);        \
            fflush(stdout);     \
        } while (0)

    RUN_TEST(test_uint32_alloc_fill_and_at);
    RUN_TEST(test_offset2d_and_stride);
    RUN_TEST(test_alloc_fill_copy_add_sub);
    RUN_TEST(test_transpose);
    RUN_TEST(test_copy_windows);
    RUN_TEST(test_dot_product_matrix_multiply);
    RUN_TEST(test_det_and_minor_det_agree_3x3);
    RUN_TEST(test_det_2x2_and_upper_triangulate_sign);
    RUN_TEST(test_minor_det_matches_gauss_4x4_known);
    RUN_TEST(test_invert);
    RUN_TEST(test_LUP_decomposition_identity_P_no_swap_case);
    RUN_TEST(test_LUP_decomposition_swap_required_case);
    RUN_TEST(test_reduce_rank);
    RUN_TEST(test_norms_and_normalize);
    RUN_TEST(test_outer_product_and_cross);
    RUN_TEST(test_shift_and_identity);
    RUN_TEST(test_rotation_matrices_orthonormal);
    RUN_TEST(test_DCM_zyx_matches_product);
    RUN_TEST(test_solve_linear_system_LUP);
    RUN_TEST(test_non_contiguous_stride_views);
    RUN_TEST(test_row_col_ops_and_scaling);
    RUN_TEST(test_rand_range);
    RUN_TEST(test_copy_row_and_col_helpers);
    RUN_TEST(test_deterministic_fuzz_loop);
    RUN_TEST(test_dot_product_and_vector_variants);
    RUN_TEST(test_outer_product_row_vector_path);
    RUN_TEST(test_det_early_zero_row_and_zero_col_paths);
    RUN_TEST(test_mat_is_all_digit);
    RUN_TEST(test_power_iterate_and_eig_helpers);

    #undef RUN_TEST

    printf("\n [INFO] Matrix2D tests passed\n");
    return 0;
}