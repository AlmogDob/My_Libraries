/* written by AI */
/* test_almog_string_manipulation.c */

#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#define NO_ERRORS
#include "Almog_String_Manipulation.h"

/* ---------------- Test harness ---------------- */

static int g_tests_run = 0;
static int g_tests_failed = 0;
static int g_tests_warned = 0;

#define TEST_CASE(expr)                                                      \
    do {                                                                     \
        g_tests_run++;                                                       \
        if (!(expr)) {                                                       \
            g_tests_failed++;                                                \
            fprintf(stderr, "[FAIL] %s:%d: %s\n", __FILE__, __LINE__, #expr); \
        }                                                                    \
    } while (0)

#define TEST_WARN(expr, msg)                                                 \
    do {                                                                     \
        g_tests_run++;                                                       \
        if (!(expr)) {                                                       \
            g_tests_warned++;                                                \
            fprintf(stderr, "[WARN] %s:%d: %s | %s\n", __FILE__, __LINE__,    \
                    #expr, msg);                                             \
        }                                                                    \
    } while (0)

#define TEST_EQ_INT(a, b) TEST_CASE((a) == (b))
#define TEST_EQ_SIZE(a, b) TEST_CASE((a) == (b))
#define TEST_EQ_STR(a, b) TEST_CASE(strcmp((a), (b)) == 0)
#define TEST_NE_STR(a, b) TEST_CASE(strcmp((a), (b)) != 0)

static void fill_sentinel(unsigned char *buf, size_t n, unsigned char v)
{
    for (size_t i = 0; i < n; i++) buf[i] = v;
}

static bool is_nul_terminated_within(const char *s, size_t cap)
{
    for (size_t i = 0; i < cap; i++) {
        if (s[i] == '\0') return true;
    }
    return false;
}

/* Simple deterministic RNG for fuzz-ish tests */
static uint32_t rng_state = 0xC0FFEE01u;
static uint32_t xorshift32(void)
{
    uint32_t x = rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    rng_state = x;
    return x;
}

static char rand_ascii_printable(void)
{
    /* printable ASCII range 32..126 */
    return (char)(32 + (xorshift32() % 95));
}

/* ---------------- Coverage checks ----------------
 * We can’t reliably “assert all symbols exist” at runtime, but we can at least
 * ensure we have tests for every IMPLEMENTED function by calling it at least
 * once in this file.
 */

/* ---------------- Tests: ASCII classification ---------------- */

static void test_ascii_classification_exhaustive_ranges(void)
{
    /* Check key boundaries and a few midpoints for each function. */
    TEST_CASE(asm_isdigit('0'));
    TEST_CASE(asm_isdigit('9'));
    TEST_CASE(!asm_isdigit('/'));
    TEST_CASE(!asm_isdigit(':'));

    TEST_CASE(asm_isupper('A'));
    TEST_CASE(asm_isupper('Z'));
    TEST_CASE(!asm_isupper('@'));
    TEST_CASE(!asm_isupper('['));

    TEST_CASE(asm_islower('a'));
    TEST_CASE(asm_islower('z'));
    TEST_CASE(!asm_islower('`'));
    TEST_CASE(!asm_islower('{'));

    TEST_CASE(asm_isalpha('A'));
    TEST_CASE(asm_isalpha('z'));
    TEST_CASE(!asm_isalpha('0'));

    TEST_CASE(asm_isalnum('A'));
    TEST_CASE(asm_isalnum('9'));
    TEST_CASE(!asm_isalnum('_'));
    TEST_CASE(!asm_isalnum(' '));

    TEST_CASE(asm_isspace(' '));
    TEST_CASE(asm_isspace('\n'));
    TEST_CASE(asm_isspace('\t'));
    TEST_CASE(asm_isspace('\r'));
    TEST_CASE(asm_isspace('\v'));
    TEST_CASE(asm_isspace('\f'));
    TEST_CASE(!asm_isspace('X'));

    TEST_CASE(asm_isgraph('!'));
    TEST_CASE(asm_isgraph('~'));
    TEST_CASE(!asm_isgraph(' '));

    TEST_CASE(asm_isprint(' '));
    TEST_CASE(asm_isprint('!'));
    TEST_CASE(!asm_isprint('\n'));

    TEST_CASE(asm_ispunct('!'));
    TEST_CASE(asm_ispunct('/'));
    TEST_CASE(asm_ispunct(':'));
    TEST_CASE(!asm_ispunct('A'));
    TEST_CASE(!asm_ispunct('0'));
    TEST_CASE(!asm_ispunct(' '));

    TEST_CASE(asm_iscntrl('\0'));
    TEST_CASE(asm_iscntrl('\n'));
    TEST_CASE(asm_iscntrl(127));
    TEST_CASE(!asm_iscntrl('A'));

    /* Hex digit helpers (your impl splits by case) */
    TEST_CASE(asm_isxdigit('0'));
    TEST_CASE(asm_isxdigit('9'));
    TEST_CASE(asm_isxdigit('a'));
    TEST_CASE(asm_isxdigit('f'));
    TEST_CASE(!asm_isxdigit('g'));
    TEST_CASE(!asm_isxdigit('A'));

    TEST_CASE(asm_isXdigit('0'));
    TEST_CASE(asm_isXdigit('9'));
    TEST_CASE(asm_isXdigit('A'));
    TEST_CASE(asm_isXdigit('F'));
    TEST_CASE(!asm_isXdigit('G'));
    TEST_CASE(!asm_isXdigit('a'));
}

static void test_ascii_classification_full_scan_0_127(void)
{
    /* Property checks over ASCII 0..127. */
    for (int c = 0; c <= 127; c++) {
        char ch = (char)c;

        /* isalnum == isalpha || isdigit */
        TEST_CASE(asm_isalnum(ch) == (asm_isalpha(ch) || asm_isdigit(ch)));

        /* isprint == isgraph || ' ' */
        TEST_CASE(asm_isprint(ch) == (asm_isgraph(ch) || ch == ' '));

        /* isalpha implies not digit */
        if (asm_isalpha(ch)) {
            TEST_CASE(!asm_isdigit(ch));
        }

        /* upper and lower are disjoint */
        if (asm_isupper(ch)) TEST_CASE(!asm_islower(ch));
        if (asm_islower(ch)) TEST_CASE(!asm_isupper(ch));

        /* graph implies print */
        if (asm_isgraph(ch)) TEST_CASE(asm_isprint(ch));
    }
}

/* ---------------- Tests: case conversion ---------------- */

static void test_case_conversion_roundtrip(void)
{
    for (int i = 0; i < 200; i++) {
        char s[128];
        char a[128];
        char b[128];

        /* random printable string length 0..40 */
        size_t n = (size_t)(xorshift32() % 41);
        for (size_t j = 0; j < n; j++) s[j] = rand_ascii_printable();
        s[n] = '\0';

        strcpy(a, s);
        strcpy(b, s);

        asm_tolower(a);
        asm_toupper(a);
        asm_toupper(b);
        asm_tolower(b);

        /* Not equal generally, but must still be valid strings and stable */
        TEST_CASE(is_nul_terminated_within(a, sizeof(a)));
        TEST_CASE(is_nul_terminated_within(b, sizeof(b)));

        /* toupper(toupper(x)) == toupper(x) */
        char u1[128], u2[128];
        strcpy(u1, s);
        strcpy(u2, s);
        asm_toupper(u1);
        asm_toupper(u2);
        asm_toupper(u2);
        TEST_EQ_STR(u1, u2);

        /* tolower(tolower(x)) == tolower(x) */
        char l1[128], l2[128];
        strcpy(l1, s);
        strcpy(l2, s);
        asm_tolower(l1);
        asm_tolower(l2);
        asm_tolower(l2);
        TEST_EQ_STR(l1, l2);
    }
}

/* ---------------- Tests: asm_length ---------------- */

static void test_length_matches_strlen_small(void)
{
    for (int i = 0; i < 200; i++) {
        char s[256];
        size_t n = (size_t)(xorshift32() % 200);
        for (size_t j = 0; j < n; j++) s[j] = rand_ascii_printable();
        s[n] = '\0';

        TEST_EQ_SIZE(asm_length(s), strlen(s));
    }
}

/* ---------------- Tests: asm_memset ---------------- */

static void test_memset_basic_and_edges(void)
{
    unsigned char buf[32];
    fill_sentinel(buf, sizeof(buf), 0xCC);

    void *ret = asm_memset(buf, 0xAB, sizeof(buf));
    TEST_CASE(ret == buf);
    for (size_t i = 0; i < sizeof(buf); i++) TEST_CASE(buf[i] == 0xAB);

    fill_sentinel(buf, sizeof(buf), 0xCC);
    asm_memset(buf, 0xAB, 0);
    for (size_t i = 0; i < sizeof(buf); i++) TEST_CASE(buf[i] == 0xCC);
}

/* ---------------- Tests: asm_copy_array_by_indexes ---------------- */

static void test_copy_array_by_indexes_behavior_and_bounds(void)
{
    const char *src = "abcdef";
    char out[16];

    asm_copy_array_by_indexes(out, 1, 3, src); /* inclusive end in impl */
    TEST_EQ_STR(out, "bcd");

    asm_copy_array_by_indexes(out, 0, 0, src);
    TEST_EQ_STR(out, "a");

    asm_copy_array_by_indexes(out, 5, 5, src);
    TEST_EQ_STR(out, "f");

    asm_copy_array_by_indexes(out, 0, 6, src); /* copies '\0' too */
    TEST_EQ_STR(out, "abcdef");

    /* Sentinel around output buffer to detect overwrite beyond out[16] */
    struct {
        unsigned char pre[8];
        char out2[8];
        unsigned char post[8];
    } box;

    fill_sentinel(box.pre, sizeof(box.pre), 0xA5);
    fill_sentinel((unsigned char *)box.out2, sizeof(box.out2), 0xCC);
    fill_sentinel(box.post, sizeof(box.post), 0x5A);

    /* copy "ab" plus '\0' => should fit exactly */
    asm_copy_array_by_indexes(box.out2, 0, 1, "ab");
    TEST_EQ_STR(box.out2, "ab");
    for (size_t i = 0; i < sizeof(box.pre); i++) TEST_CASE(box.pre[i] == 0xA5);
    for (size_t i = 0; i < sizeof(box.post); i++) TEST_CASE(box.post[i] == 0x5A);
}

/* ---------------- Tests: shifting/padding ---------------- */

static void test_left_shift_edges(void)
{
    char s[64];

    strcpy(s, "abcdef");
    asm_shift_left(s, 0);
    TEST_EQ_STR(s, "abcdef");

    strcpy(s, "abcdef");
    asm_shift_left(s, 1);
    TEST_EQ_STR(s, "bcdef");

    strcpy(s, "abcdef");
    asm_shift_left(s, 5);
    TEST_EQ_STR(s, "f");

    strcpy(s, "abcdef");
    asm_shift_left(s, 6);
    TEST_EQ_STR(s, "");

    strcpy(s, "abcdef");
    asm_shift_left(s, 1000);
    TEST_EQ_STR(s, "");
}

static void test_left_pad_edges_and_sentinel(void)
{
    {
        char s[64] = "abc";
        asm_pad_left(s, 0, ' ');
        TEST_EQ_STR(s, "abc");
    }
    {
        char s[64] = "abc";
        asm_pad_left(s, 4, ' ');
        TEST_EQ_STR(s, "    abc");
    }
    {
        char s[64] = "";
        asm_pad_left(s, 3, '_');
        TEST_EQ_STR(s, "___");
    }

    /* Sentinel structure: ensure we don't write before start */
    struct {
        unsigned char pre[8];
        char s[32];
        unsigned char post[8];
    } box;

    fill_sentinel(box.pre, sizeof(box.pre), 0x11);
    fill_sentinel((unsigned char *)box.s, sizeof(box.s), 0xCC);
    fill_sentinel(box.post, sizeof(box.post), 0x22);

    strcpy(box.s, "x");
    asm_pad_left(box.s, 5, '0');
    TEST_EQ_STR(box.s, "00000x");

    for (size_t i = 0; i < sizeof(box.pre); i++) TEST_CASE(box.pre[i] == 0x11);
    for (size_t i = 0; i < sizeof(box.post); i++) TEST_CASE(box.post[i] == 0x22);
}

/* ---------------- Tests: remove/strip/whitespace ---------------- */

static void test_remove_char_form_string_edges(void)
{
    char s[64];

    strcpy(s, "abcd");
    asm_remove_char_from_string(s, 1);
    TEST_EQ_STR(s, "acd");

    strcpy(s, "abcd");
    asm_remove_char_from_string(s, 0);
    TEST_EQ_STR(s, "bcd");

    strcpy(s, "abcd");
    asm_remove_char_from_string(s, 3);
    TEST_EQ_STR(s, "abc");

    strcpy(s, "a");
    asm_remove_char_from_string(s, 0);
    TEST_EQ_STR(s, "");

    strcpy(s, "");
    asm_remove_char_from_string(s, 0);
    TEST_EQ_STR(s, "");

    strcpy(s, "abcd");
    asm_remove_char_from_string(s, 999);
    TEST_EQ_STR(s, "abcd");
}

static void test_strip_whitespace_properties(void)
{
    char s[128];

    strcpy(s, "  a \t b\nc  ");
    asm_strip_whitespace(s);
    TEST_EQ_STR(s, "abc");

    strcpy(s, "no_spaces");
    asm_strip_whitespace(s);
    TEST_EQ_STR(s, "no_spaces");

    strcpy(s, " \t\r\n");
    asm_strip_whitespace(s);
    TEST_EQ_STR(s, "");

    /* Property: result has no whitespace chars */
    for (int i = 0; i < 100; i++) {
        size_t n = (size_t)(xorshift32() % 60);
        for (size_t j = 0; j < n; j++) {
            /* mix whitespace and printable */
            uint32_t r = xorshift32() % 10;
            if (r == 0) s[j] = ' ';
            else if (r == 1) s[j] = '\n';
            else if (r == 2) s[j] = '\t';
            else s[j] = rand_ascii_printable();
        }
        s[n] = '\0';

        asm_strip_whitespace(s);
        for (size_t k = 0; s[k] != '\0'; k++) {
            TEST_CASE(!asm_isspace(s[k]));
        }
    }
}

static void test_str_is_whitespace_edges(void)
{
    TEST_CASE(asm_str_is_whitespace(" \t\r\n") == true);
    TEST_CASE(asm_str_is_whitespace("") == true); /* current behavior */
    TEST_CASE(asm_str_is_whitespace(" x ") == false);
}

/* ---------------- Tests: asm_strncmp (boolean) ---------------- */

static void test_strncmp_boolean_edges(void)
{
    TEST_CASE(asm_strncmp("abc", "abc", 3) == 1);
    TEST_CASE(asm_strncmp("abc", "abd", 3) == 0);
    TEST_CASE(asm_strncmp("ab", "abc", 3) == 0);
    TEST_CASE(asm_strncmp("abc", "ab", 3) == 0);

    TEST_CASE(asm_strncmp("abc", "XYZ", 0) == 1);

    TEST_CASE(asm_strncmp("", "", 5) == 1);
    TEST_CASE(asm_strncmp("", "a", 1) == 0);
    TEST_CASE(asm_strncmp("a", "", 1) == 0);
}

/* ---------------- Tests: asm_str_in_str ---------------- */

static void test_str_in_str_overlap_and_edges(void)
{
    TEST_EQ_INT(asm_str_in_str("aaaa", "aa"), 3);
    TEST_EQ_INT(asm_str_in_str("hello world", "lo"), 1);
    TEST_EQ_INT(asm_str_in_str("abc", "abcd"), 0);
    TEST_EQ_INT(asm_str_in_str("abababa", "aba"), 3);

    /* Do not pass empty needle: undefined-ish for your implementation. */
}

/* ---------------- Tests: base digit helpers ---------------- */

static void test_base_digit_helpers(void)
{
    TEST_CASE(asm_check_char_belong_to_base('0', 2) == true);
    TEST_CASE(asm_check_char_belong_to_base('1', 2) == true);
    TEST_CASE(asm_check_char_belong_to_base('2', 2) == false);

    TEST_CASE(asm_check_char_belong_to_base('9', 10) == true);
    TEST_CASE(asm_check_char_belong_to_base('a', 10) == false);

    TEST_CASE(asm_check_char_belong_to_base('a', 16) == true);
    TEST_CASE(asm_check_char_belong_to_base('f', 16) == true);
    TEST_CASE(asm_check_char_belong_to_base('g', 16) == false);
    TEST_CASE(asm_check_char_belong_to_base('A', 16) == true);
    TEST_CASE(asm_check_char_belong_to_base('F', 16) == true);
    TEST_CASE(asm_check_char_belong_to_base('G', 16) == false);

    TEST_CASE(asm_check_char_belong_to_base('z', 36) == true);
    TEST_CASE(asm_check_char_belong_to_base('Z', 36) == true);

    TEST_EQ_INT(asm_get_char_value_in_base('0', 10), 0);
    TEST_EQ_INT(asm_get_char_value_in_base('9', 10), 9);
    TEST_EQ_INT(asm_get_char_value_in_base('A', 16), 10);
    TEST_EQ_INT(asm_get_char_value_in_base('f', 16), 15);
    TEST_EQ_INT(asm_get_char_value_in_base('Z', 36), 35);

    TEST_EQ_INT(asm_get_char_value_in_base('g', 16), -1);

    /* base validity errors should return false / -1 */
    TEST_CASE(asm_check_char_belong_to_base('0', 1) == false);
    TEST_CASE(asm_check_char_belong_to_base('0', 37) == false);
    TEST_EQ_INT(asm_get_char_value_in_base('0', 1), -1);
}

/* ---------------- Tests: str2int/size_t/float/double ---------------- */

static void test_str2int(void)
{
    const char *end = NULL;

    {
        char s[] = "  -1011zzz";
        int v = asm_str2int(s, &end, 2);
        TEST_CASE(v == -11);
        TEST_CASE(*end == 'z');
    }
    {
        char s[] = "+7fff!";
        int v = asm_str2int(s, &end, 16);
        TEST_CASE(v == 0x7fff);
        TEST_CASE(*end == '!');
    }
    {
        char s[] = "   +0";
        int v = asm_str2int(s, &end, 10);
        TEST_CASE(v == 0);
        TEST_CASE(*end == '\0');
    }
    {
        char s[] = "xyz";
        int v = asm_str2int(s, &end, 10);
        TEST_CASE(v == 0);
        TEST_CASE(*end == 'x');
    }
    {
        char s[] = "123";
        int v = asm_str2int(s, &end, 1);
        TEST_CASE(v == 0);
        TEST_CASE(end == s);
    }
}

static void test_str2size_t(void)
{
    const char *end = NULL;

    {
        char s[] = " +1f!";
        size_t v = asm_str2size_t(s, &end, 16);
        TEST_CASE(v == 31u);
        TEST_CASE(*end == '!');
    }
    {
        char s[] = "  -1";
        size_t v = asm_str2size_t(s, &end, 10);
        TEST_CASE(v == 0);
        TEST_CASE(end == s);
    }
    {
        char s[] = "  +0009x";
        size_t v = asm_str2size_t(s, &end, 10);
        TEST_CASE(v == 9u);
        TEST_CASE(*end == 'x');
    }
    {
        char s[] = "  123";
        size_t v = asm_str2size_t(s, &end, 37);
        TEST_CASE(v == 0);
        /* current implementation sets *end = s+num_of_whitespace on invalid base */
        TEST_CASE(end == s + 2);
    }
}

static void test_str2float_double(void)
{
    const char *end = NULL;

    {
        char s[] = " 10.5x";
        float v = asm_str2float(s, &end, 10);
        TEST_CASE(v > 10.49f && v < 10.51f);
        TEST_CASE(*end == 'x');
    }
    {
        char s[] = "-a.bQ";
        double v = asm_str2double(s, &end, 16);
        TEST_CASE(v < -10.68 && v > -10.70);
        TEST_CASE(*end == 'Q');
    }
    {
        char s[] = "  123.";
        double v = asm_str2double(s, &end, 10);
        TEST_CASE(v > 122.99 && v < 123.01);
        TEST_CASE(*end == '\0');
    }
    {
        char s[] = "  .5";
        double v = asm_str2double(s, &end, 10);
        TEST_CASE(v > 0.49 && v < 0.51);
        TEST_CASE(*end == '\0');
    }
    {
        char s[] = "  -.";
        double v = asm_str2double(s, &end, 10);
        TEST_CASE(v == 0.0);
        TEST_CASE(*end == '\0');
    }
    {
        char s[] = "12.3";
        double v = asm_str2double(s, &end, 37);
        TEST_CASE(v == 0.0);
        TEST_CASE(end == s);
    }
}

/* ---------------- Tests: tokenization helpers ---------------- */

static void test_get_next_word_from_line_current_behavior(void)
{
    /* Your implementation:
     * - does NOT skip whitespace
     * - stops only on delimiter or '\0'
     * - returns length (j), not consumed index
     */
    {
        char src[] = "abc,def";
        char w[64] = {0};
        int r = asm_get_next_token_from_str(w, src, ',');
        TEST_EQ_INT(r, 3);
        TEST_EQ_STR(w, "abc");
    }
    {
        char src[] = ",def";
        char w[64] = {0};
        int r = asm_get_next_token_from_str(w, src, ',');
        TEST_EQ_INT(r, 0);
        TEST_EQ_STR(w, "");
    }
    {
        char src[] = "  abc,def";
        char w[64] = {0};
        int r = asm_get_next_token_from_str(w, src, ',');
        TEST_EQ_INT(r, 5);
        TEST_EQ_STR(w, "  abc");
    }
    {
        char src[] = "abc\ndef";
        char w[64] = {0};
        int r = asm_get_next_token_from_str(w, src, ',');
        TEST_EQ_INT(r, (int)strlen(src));
        TEST_EQ_STR(w, "abc\ndef");
    }

    /* Doc mismatch detection (warn, not fail) */
    {
        char src[] = "  abc,def";
        char w[64] = {0};
        asm_get_next_token_from_str(w, src, ',');
        TEST_CASE(strcmp(w, "  abc") == 0);
    }
}

static void test_get_word_and_cut_edges(void)
{
    {
        char src[64] = "abc,def";
        char w[64] = {0};
        int ok = asm_get_token_and_cut(w, src, ',', true);
        TEST_CASE(ok == 1);
        TEST_EQ_STR(w, "abc");
        TEST_EQ_STR(src, ",def");
    }
    {
        char src[64] = "abc,def";
        char w[64] = {0};
        int ok = asm_get_token_and_cut(w, src, ',', false);
        TEST_CASE(ok == 1);
        TEST_EQ_STR(w, "abc");
        TEST_EQ_STR(src, "def");
    }
    {
        char src[64] = ",def";
        char w[64] = {0};
        int ok = asm_get_token_and_cut(w, src, ',', true);
        TEST_CASE(ok == 0);
        TEST_EQ_STR(w, "");
        TEST_EQ_STR(src, ",def");
    }
    {
        char src[64] = "nodelem";
        char w[64] = {0};
        int ok = asm_get_token_and_cut(w, src, ',', false);
        TEST_CASE(ok == 1);
        TEST_EQ_STR(w, "nodelem");
        TEST_EQ_STR(src, "");
    }
}

/* ---------------- Tests: asm_get_line ---------------- */

static void test_get_line_tmpfile(void)
{
    FILE *fp = tmpfile();
    if (!fp) {
        fprintf(stderr,
                "[WARN] tmpfile() unavailable; skipping asm_get_line tests\n");
        g_tests_warned++;
        return;
    }

    fputs("hello\n", fp);
    fputs("\n", fp);
    fputs("world", fp);
    rewind(fp);

    {
        char line[ASM_MAX_LEN + 1];
        int n = asm_get_line(fp, line);
        TEST_EQ_INT(n, 5);
        TEST_EQ_STR(line, "hello");
        TEST_CASE(is_nul_terminated_within(line, sizeof(line)));
    }
    {
        char line[ASM_MAX_LEN + 1];
        int n = asm_get_line(fp, line);
        TEST_EQ_INT(n, 0);
        TEST_EQ_STR(line, "");
    }
    {
        char line[ASM_MAX_LEN + 1];
        int n = asm_get_line(fp, line);
        TEST_EQ_INT(n, 5);
        TEST_EQ_STR(line, "world");
    }
    {
        char line[ASM_MAX_LEN + 1];
        int n = asm_get_line(fp, line);
        TEST_EQ_INT(n, -1);
    }

    fclose(fp);
}

/* Optional: test overflow condition using ASM_MAX_LEN+1 chars before '\n' */
static void test_get_line_too_long(void)
{
    FILE *fp = tmpfile();
    if (!fp) {
        fprintf(stderr,
                "[WARN] tmpfile() unavailable; skipping long-line test\n");
        g_tests_warned++;
        return;
    }

    for (int i = 0; i < ASM_MAX_LEN + 5; i++) fputc('a', fp);
    fputc('\n', fp);
    rewind(fp);

    char line[ASM_MAX_LEN + 1];
    fill_sentinel((unsigned char *)line, sizeof(line), 0xCC);

    int n = asm_get_line(fp, line);
    TEST_EQ_INT(n, -1);

    /* On error, your docs say not guaranteed NUL terminated. We only ensure
       we didn’t write past buffer size (can’t directly prove; but at least
       array exists). */
    fclose(fp);
}

/* ---------------- Tests: asm_strncat ---------------- */

static void test_strncat_current_behavior_and_sentinel(void)
{
    /* Current impl does NOT append '\0' (bug-like).
       We test both:
       - it copies correct bytes
       - it does not clobber past allowed region
     */
    struct {
        unsigned char pre[8];
        char s1[16];
        unsigned char post[8];
    } box;

    fill_sentinel(box.pre, sizeof(box.pre), 0xAA);
    fill_sentinel((unsigned char *)box.s1, sizeof(box.s1), 0xCC);
    fill_sentinel(box.post, sizeof(box.post), 0xBB);

    strcpy(box.s1, "abc");

    int n = asm_strncat(box.s1, "DEF", 3);
    TEST_EQ_INT(n, 3);

    TEST_EQ_STR(box.s1, "abcDEF");

    for (size_t i = 0; i < sizeof(box.pre); i++) TEST_CASE(box.pre[i] == 0xAA);
    for (size_t i = 0; i < sizeof(box.post); i++) TEST_CASE(box.post[i] == 0xBB);
}

/* ---------------- Main ---------------- */

int main(void)
{
    test_ascii_classification_exhaustive_ranges();
    test_ascii_classification_full_scan_0_127();

    test_case_conversion_roundtrip();

    test_length_matches_strlen_small();

    test_memset_basic_and_edges();

    test_copy_array_by_indexes_behavior_and_bounds();

    test_left_shift_edges();
    test_left_pad_edges_and_sentinel();

    test_remove_char_form_string_edges();
    test_strip_whitespace_properties();
    test_str_is_whitespace_edges();

    test_strncmp_boolean_edges();
    test_str_in_str_overlap_and_edges();

    test_base_digit_helpers();
    test_str2int();
    test_str2size_t();
    test_str2float_double();

    test_get_next_word_from_line_current_behavior();
    test_get_word_and_cut_edges();

    test_get_line_tmpfile();
    test_get_line_too_long();

    test_strncat_current_behavior_and_sentinel();

    if (g_tests_failed == 0) {
        if (g_tests_warned == 0) {
            printf("[OK] %d tests passed\n", g_tests_run);
        } else {
            printf("[OK] %d tests passed, %d warnings\n", g_tests_run,
                   g_tests_warned);
        }
        return 0;
    }

    fprintf(stderr, "[FAIL] %d/%d tests failed (%d warnings)\n", g_tests_failed,
            g_tests_run, g_tests_warned);
    return 1;
}