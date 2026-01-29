/** 
 * Written by AI
 * 
 * test_almog_lexer.c
 * Simple, self-contained tests for Almog_Lexer.h (single-header).
*/

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Compile implementations in THIS translation unit. */
#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#define ALMOG_LEXER_IMPLEMENTATION
#include "Almog_Lexer.h"

static const char *kind_name(enum Token_Kind k)
{
    switch (k) {
        case TOKEN_EOF: return "TOKEN_EOF";
        case TOKEN_INVALID: return "TOKEN_INVALID";
        case TOKEN_PP_DIRECTIVE: return "TOKEN_PP_DIRECTIVE";
        case TOKEN_COMMENT: return "TOKEN_COMMENT";
        case TOKEN_STRING_LIT: return "TOKEN_STRING_LIT";
        case TOKEN_CHAR_LIT: return "TOKEN_CHAR_LIT";
        case TOKEN_NUMBER: return "TOKEN_NUMBER";
        case TOKEN_KEYWORD: return "TOKEN_KEYWORD";
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";

        case TOKEN_LPAREN: return "TOKEN_LPAREN";
        case TOKEN_RPAREN: return "TOKEN_RPAREN";
        case TOKEN_LBRACKET: return "TOKEN_LBRACKET";
        case TOKEN_RBRACKET: return "TOKEN_RBRACKET";
        case TOKEN_LBRACE: return "TOKEN_LBRACE";
        case TOKEN_RBRACE: return "TOKEN_RBRACE";

        case TOKEN_DOT: return "TOKEN_DOT";
        case TOKEN_COMMA: return "TOKEN_COMMA";
        case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
        case TOKEN_BSLASH: return "TOKEN_BSLASH";
        case TOKEN_HASH: return "TOKEN_HASH";

        case TOKEN_QUESTION: return "TOKEN_QUESTION";
        case TOKEN_COLON: return "TOKEN_COLON";

        case TOKEN_EQ: return "TOKEN_EQ";
        case TOKEN_EQEQ: return "TOKEN_EQEQ";
        case TOKEN_NE: return "TOKEN_NE";
        case TOKEN_BANG: return "TOKEN_BANG";

        case TOKEN_LT: return "TOKEN_LT";
        case TOKEN_GT: return "TOKEN_GT";
        case TOKEN_LE: return "TOKEN_LE";
        case TOKEN_GE: return "TOKEN_GE";

        case TOKEN_BITAND: return "TOKEN_BITAND";
        case TOKEN_ANDAND: return "TOKEN_ANDAND";
        case TOKEN_BITOR: return "TOKEN_BITOR";
        case TOKEN_OROR: return "TOKEN_OROR";
        case TOKEN_CARET: return "TOKEN_CARET";
        case TOKEN_TILDE: return "TOKEN_TILDE";

        case TOKEN_LSHIFT: return "TOKEN_LSHIFT";
        case TOKEN_RSHIFT: return "TOKEN_RSHIFT";

        case TOKEN_PLUSPLUS: return "TOKEN_PLUSPLUS";
        case TOKEN_MINUSMINUS: return "TOKEN_MINUSMINUS";

        case TOKEN_PLUS: return "TOKEN_PLUS";
        case TOKEN_MINUS: return "TOKEN_MINUS";
        case TOKEN_STAR: return "TOKEN_STAR";
        case TOKEN_SLASH: return "TOKEN_SLASH";
        case TOKEN_PERCENT: return "TOKEN_PERCENT";

        case TOKEN_PLUSEQ: return "TOKEN_PLUSEQ";
        case TOKEN_MINUSEQ: return "TOKEN_MINUSEQ";
        case TOKEN_STAREQ: return "TOKEN_STAREQ";
        case TOKEN_SLASHEQ: return "TOKEN_SLASHEQ";
        case TOKEN_PERCENTEQ: return "TOKEN_PERCENTEQ";
        case TOKEN_ANDEQ: return "TOKEN_ANDEQ";
        case TOKEN_OREQ: return "TOKEN_OREQ";
        case TOKEN_XOREQ: return "TOKEN_XOREQ";
        case TOKEN_LSHIFTEQ: return "TOKEN_LSHIFTEQ";
        case TOKEN_RSHIFTEQ: return "TOKEN_RSHIFTEQ";

        case TOKEN_ARROW: return "TOKEN_ARROW";
        case TOKEN_ELLIPSIS: return "TOKEN_ELLIPSIS";
    }
    return "TOKEN_<unknown>";
}

static void fail_token(
    const char *test_name,
    struct Token got,
    enum Token_Kind exp_kind,
    const char *exp_text,
    size_t exp_line,
    size_t exp_col
)
{
    fprintf(stderr, "\n[FAIL] %s\n", test_name);
    fprintf(stderr, "  expected: kind=%s", kind_name(exp_kind));
    if (exp_text) {
        fprintf(stderr, ", text=\"%s\" (len=%zu)", exp_text, strlen(exp_text));
    }
    if (exp_line) fprintf(stderr, ", line=%zu", exp_line);
    if (exp_col) fprintf(stderr, ", col=%zu", exp_col);
    fprintf(stderr, "\n");

    fprintf(stderr, "  got:      kind=%s, text_len=%zu, line=%zu, col=%zu, text=\"%.*s\"\n",
            kind_name(got.kind),
            got.text_len,
            got.location.line_num,
            got.location.col,
            (int)got.text_len,
            got.text ? got.text : "");
    exit(1);
}

/* If exp_text == NULL => don't check text. If exp_line/col == 0 => don't check. */
static void expect_tok(
    const char *test_name,
    struct Lexer *l,
    enum Token_Kind exp_kind,
    const char *exp_text,
    size_t exp_line,
    size_t exp_col
)
{
    struct Token t = al_lexer_next_token(l);

    if (t.kind != exp_kind) {
        fail_token(test_name, t, exp_kind, exp_text, exp_line, exp_col);
    }

    if (exp_text) {
        size_t n = strlen(exp_text);
        if (t.text_len != n || memcmp(t.text, exp_text, n) != 0) {
            fail_token(test_name, t, exp_kind, exp_text, exp_line, exp_col);
        }
    }

    if (exp_line && t.location.line_num != exp_line) {
        fail_token(test_name, t, exp_kind, exp_text, exp_line, exp_col);
    }

    if (exp_col && t.location.col != exp_col) {
        fail_token(test_name, t, exp_kind, exp_text, exp_line, exp_col);
    }
}

static void test_basic_program(void)
{
    const char *name = "basic_program";
    const char *src = "int main() { return 0; }";
    struct Lexer l = al_lexer_alloc(src, strlen(src));

    expect_tok(name, &l, TOKEN_KEYWORD, "int", 0, 0);
    expect_tok(name, &l, TOKEN_IDENTIFIER, "main", 0, 0);
    expect_tok(name, &l, TOKEN_LPAREN, "(", 0, 0);
    expect_tok(name, &l, TOKEN_RPAREN, ")", 0, 0);
    expect_tok(name, &l, TOKEN_LBRACE, "{", 0, 0);
    expect_tok(name, &l, TOKEN_KEYWORD, "return", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, "0", 0, 0);
    expect_tok(name, &l, TOKEN_SEMICOLON, ";", 0, 0);
    expect_tok(name, &l, TOKEN_RBRACE, "}", 0, 0);
    expect_tok(name, &l, TOKEN_EOF, NULL, 0, 0);
}

static void test_pp_directive_and_locations(void)
{
    const char *name = "pp_directive_and_locations";
    const char *src = "#include <stdio.h>\nint x;\n";
    struct Lexer l = al_lexer_alloc(src, strlen(src));

    /* PP directive is only recognized at col==1 and includes the newline. */
    expect_tok(name, &l, TOKEN_PP_DIRECTIVE, "#include <stdio.h>\n", 1, 1);

    expect_tok(name, &l, TOKEN_KEYWORD, "int", 2, 1);
    expect_tok(name, &l, TOKEN_IDENTIFIER, "x", 2, 5);
    expect_tok(name, &l, TOKEN_SEMICOLON, ";", 2, 6);

    expect_tok(name, &l, TOKEN_EOF, NULL, 0, 0);
}

static void test_whitespace_location_math(void)
{
    const char *name = "whitespace_location";
    const char *src = "a\n  b";
    struct Lexer l = al_lexer_alloc(src, strlen(src));

    expect_tok(name, &l, TOKEN_IDENTIFIER, "a", 1, 1);
    expect_tok(name, &l, TOKEN_IDENTIFIER, "b", 2, 3);
    expect_tok(name, &l, TOKEN_EOF, NULL, 0, 0);
}

static void test_comments(void)
{
    {
        const char *name = "line_comment_includes_newline";
        const char *src = "// hello\nx";
        struct Lexer l = al_lexer_alloc(src, strlen(src));

        expect_tok(name, &l, TOKEN_COMMENT, "// hello\n", 1, 1);
        expect_tok(name, &l, TOKEN_IDENTIFIER, "x", 2, 1);
        expect_tok(name, &l, TOKEN_EOF, NULL, 0, 0);
    }

    {
        const char *name = "block_comment_updates_line_col";
        const char *src = "/*x\ny*/z";
        struct Lexer l = al_lexer_alloc(src, strlen(src));

        expect_tok(name, &l, TOKEN_COMMENT, "/*x\ny*/", 1, 1);
        /* After the newline inside the comment, 'z' should be on line 2. */
        expect_tok(name, &l, TOKEN_IDENTIFIER, "z", 2, 4);
        expect_tok(name, &l, TOKEN_EOF, NULL, 0, 0);
    }
}

static void test_string_and_char_literals(void)
{
    const char *name = "string_and_char_literals";
    const char *src = "\"abc\" 'x' \"unterminated\n";
    struct Lexer l = al_lexer_alloc(src, strlen(src));

    expect_tok(name, &l, TOKEN_STRING_LIT, "\"abc\"", 0, 0);
    expect_tok(name, &l, TOKEN_CHAR_LIT, "'x'", 0, 0);

    /* Lexer stops string literal on '\n' if not closed. Still TOKEN_STRING_LIT. */
    expect_tok(name, &l, TOKEN_STRING_LIT, "\"unterminated", 0, 0);
    expect_tok(name, &l, TOKEN_EOF, NULL, 0, 0);
}

static void test_literal_operators_longest_match(void)
{
    const char *name = "literal_operators_longest_match";
    const char *src =
        "( ) [ ] { } ... . , ? : == != = ! ; \\ -> "
        "> >= < <= <<= >>= ++ -- << >> "
        "+= -= *= /= %= &= |= ^= || && | & ^ ~ "
        "+ - * / %";
    struct Lexer l = al_lexer_alloc(src, strlen(src));

    expect_tok(name, &l, TOKEN_LPAREN, "(", 0, 0);
    expect_tok(name, &l, TOKEN_RPAREN, ")", 0, 0);
    expect_tok(name, &l, TOKEN_LBRACKET, "[", 0, 0);
    expect_tok(name, &l, TOKEN_RBRACKET, "]", 0, 0);
    expect_tok(name, &l, TOKEN_LBRACE, "{", 0, 0);
    expect_tok(name, &l, TOKEN_RBRACE, "}", 0, 0);

    expect_tok(name, &l, TOKEN_ELLIPSIS, "...", 0, 0);
    expect_tok(name, &l, TOKEN_DOT, ".", 0, 0);
    expect_tok(name, &l, TOKEN_COMMA, ",", 0, 0);
    expect_tok(name, &l, TOKEN_QUESTION, "?", 0, 0);
    expect_tok(name, &l, TOKEN_COLON, ":", 0, 0);

    expect_tok(name, &l, TOKEN_EQEQ, "==", 0, 0);
    expect_tok(name, &l, TOKEN_NE, "!=", 0, 0);
    expect_tok(name, &l, TOKEN_EQ, "=", 0, 0);
    expect_tok(name, &l, TOKEN_BANG, "!", 0, 0);
    expect_tok(name, &l, TOKEN_SEMICOLON, ";", 0, 0);
    expect_tok(name, &l, TOKEN_BSLASH, "\\", 0, 0);
    expect_tok(name, &l, TOKEN_ARROW, "->", 0, 0);

    expect_tok(name, &l, TOKEN_GT, ">", 0, 0);
    expect_tok(name, &l, TOKEN_GE, ">=", 0, 0);
    expect_tok(name, &l, TOKEN_LT, "<", 0, 0);
    expect_tok(name, &l, TOKEN_LE, "<=", 0, 0);

    expect_tok(name, &l, TOKEN_LSHIFTEQ, "<<=", 0, 0);
    expect_tok(name, &l, TOKEN_RSHIFTEQ, ">>=", 0, 0);

    expect_tok(name, &l, TOKEN_PLUSPLUS, "++", 0, 0);
    expect_tok(name, &l, TOKEN_MINUSMINUS, "--", 0, 0);
    expect_tok(name, &l, TOKEN_LSHIFT, "<<", 0, 0);
    expect_tok(name, &l, TOKEN_RSHIFT, ">>", 0, 0);

    expect_tok(name, &l, TOKEN_PLUSEQ, "+=", 0, 0);
    expect_tok(name, &l, TOKEN_MINUSEQ, "-=", 0, 0);
    expect_tok(name, &l, TOKEN_STAREQ, "*=", 0, 0);
    expect_tok(name, &l, TOKEN_SLASHEQ, "/=", 0, 0);
    expect_tok(name, &l, TOKEN_PERCENTEQ, "%=", 0, 0);
    expect_tok(name, &l, TOKEN_ANDEQ, "&=", 0, 0);
    expect_tok(name, &l, TOKEN_OREQ, "|=", 0, 0);
    expect_tok(name, &l, TOKEN_XOREQ, "^=", 0, 0);

    expect_tok(name, &l, TOKEN_OROR, "||", 0, 0);
    expect_tok(name, &l, TOKEN_ANDAND, "&&", 0, 0);
    expect_tok(name, &l, TOKEN_BITOR, "|", 0, 0);
    expect_tok(name, &l, TOKEN_BITAND, "&", 0, 0);
    expect_tok(name, &l, TOKEN_CARET, "^", 0, 0);
    expect_tok(name, &l, TOKEN_TILDE, "~", 0, 0);

    expect_tok(name, &l, TOKEN_PLUS, "+", 0, 0);
    expect_tok(name, &l, TOKEN_MINUS, "-", 0, 0);
    expect_tok(name, &l, TOKEN_STAR, "*", 0, 0);
    expect_tok(name, &l, TOKEN_SLASH, "/", 0, 0);
    expect_tok(name, &l, TOKEN_PERCENT, "%", 0, 0);

    expect_tok(name, &l, TOKEN_EOF, NULL, 0, 0);
}

static void test_numbers_valid_and_invalid(void)
{
    const char *name = "numbers_valid_and_invalid";
    const char *src =
        "0 123 1. .5 1.5 "
        "1e3 1e+3 1e-3 1e 1e+ "
        "0xFF 0x1.fp3 0x1.fp 0x "
        "0b1011 0b "
        "0o77 0o "
        "42u 42ULL "
        "3.14f 2.0L "
        ". .0";
    struct Lexer l = al_lexer_alloc(src, strlen(src));

    expect_tok(name, &l, TOKEN_NUMBER, "0", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, "123", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, "1.", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, ".5", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, "1.5", 0, 0);

    expect_tok(name, &l, TOKEN_NUMBER, "1e3", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, "1e+3", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, "1e-3", 0, 0);
    expect_tok(name, &l, TOKEN_INVALID, "1e", 0, 0);
    expect_tok(name, &l, TOKEN_INVALID, "1e+", 0, 0);

    expect_tok(name, &l, TOKEN_NUMBER, "0xFF", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, "0x1.fp3", 0, 0);
    expect_tok(name, &l, TOKEN_INVALID, "0x1.fp", 0, 0);
    expect_tok(name, &l, TOKEN_INVALID, "0x", 0, 0);

    expect_tok(name, &l, TOKEN_NUMBER, "0b1011", 0, 0);
    expect_tok(name, &l, TOKEN_INVALID, "0b", 0, 0);

    expect_tok(name, &l, TOKEN_NUMBER, "0o77", 0, 0);
    expect_tok(name, &l, TOKEN_INVALID, "0o", 0, 0);

    expect_tok(name, &l, TOKEN_NUMBER, "42u", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, "42ULL", 0, 0);

    expect_tok(name, &l, TOKEN_NUMBER, "3.14f", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, "2.0L", 0, 0);

    /* '.' alone should be DOT, but '.0' should be NUMBER. */
    expect_tok(name, &l, TOKEN_DOT, ".", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, ".0", 0, 0);

    expect_tok(name, &l, TOKEN_EOF, NULL, 0, 0);
}

static void test_invalid_single_char(void)
{
    const char *name = "invalid_single_char";
    const char *src = "@";
    struct Lexer l = al_lexer_alloc(src, strlen(src));

    expect_tok(name, &l, TOKEN_INVALID, "@", 1, 1);
    expect_tok(name, &l, TOKEN_EOF, NULL, 0, 0);
}

static void test_keyword_vs_identifier_prefix(void)
{
    const char *name = "keyword_vs_identifier_prefix";
    const char *src =
        "int intensity return return_ goto goto1 _x x_1 __ __9 a9 _9";
    struct Lexer l = al_lexer_alloc(src, strlen(src));

    expect_tok(name, &l, TOKEN_KEYWORD, "int", 0, 0);
    expect_tok(name, &l, TOKEN_IDENTIFIER, "intensity", 0, 0);

    expect_tok(name, &l, TOKEN_KEYWORD, "return", 0, 0);
    expect_tok(name, &l, TOKEN_IDENTIFIER, "return_", 0, 0);

    expect_tok(name, &l, TOKEN_KEYWORD, "goto", 0, 0);
    expect_tok(name, &l, TOKEN_IDENTIFIER, "goto1", 0, 0);

    expect_tok(name, &l, TOKEN_IDENTIFIER, "_x", 0, 0);
    expect_tok(name, &l, TOKEN_IDENTIFIER, "x_1", 0, 0);
    expect_tok(name, &l, TOKEN_IDENTIFIER, "__", 0, 0);
    expect_tok(name, &l, TOKEN_IDENTIFIER, "__9", 0, 0);
    expect_tok(name, &l, TOKEN_IDENTIFIER, "a9", 0, 0);
    expect_tok(name, &l, TOKEN_IDENTIFIER, "_9", 0, 0);

    expect_tok(name, &l, TOKEN_EOF, NULL, 0, 0);
}

static void test_hash_not_pp_directive_when_not_column1(void)
{
    const char *name = "hash_not_pp_directive_when_not_column1";
    const char *src = "  #define X 1\n#define Y 2\n";
    struct Lexer l = al_lexer_alloc(src, strlen(src));

    /* Because of leading spaces, '#' is not at col 1 => NOT a PP directive. */
    expect_tok(name, &l, TOKEN_HASH, "#", 1, 3);
    expect_tok(name, &l, TOKEN_IDENTIFIER, "define", 1, 4);
    expect_tok(name, &l, TOKEN_IDENTIFIER, "X", 1, 11);
    expect_tok(name, &l, TOKEN_NUMBER, "1", 1, 13);

    /* This one is at col 1 and should be treated as a directive (includes '\n'). */
    expect_tok(name, &l, TOKEN_PP_DIRECTIVE, "#define Y 2\n", 2, 1);
    expect_tok(name, &l, TOKEN_EOF, NULL, 0, 0);
}

static void test_unterminated_block_comment(void)
{
    const char *name = "unterminated_block_comment";
    const char *src = "/* unterminated";
    struct Lexer l = al_lexer_alloc(src, strlen(src));

    /* Lexer consumes to EOF and still returns TOKEN_COMMENT. */
    expect_tok(name, &l, TOKEN_COMMENT, "/* unterminated", 1, 1);
    expect_tok(name, &l, TOKEN_EOF, NULL, 0, 0);
}

static void test_hex_float_variants(void)
{
    const char *name = "hex_float_variants";
    const char *src =
        "0x1p2 0x1p+2 0x1p-2 0x.1p1 0x.p1 0xp1 0x1.0p0 0x1.0 0x1";
    struct Lexer l = al_lexer_alloc(src, strlen(src));

    expect_tok(name, &l, TOKEN_NUMBER, "0x1p2", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, "0x1p+2", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, "0x1p-2", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, "0x.1p1", 0, 0);

    /* Invalid: dot in hex mantissa but no digits before/after the dot */
    expect_tok(name, &l, TOKEN_INVALID, "0x.p1", 0, 0);
    /* Invalid: no mantissa digits (even though exponent is present) */
    expect_tok(name, &l, TOKEN_INVALID, "0xp1", 0, 0);

    expect_tok(name, &l, TOKEN_NUMBER, "0x1.0p0", 0, 0);
    /* Invalid: '.' in hex mantissa requires p/P exponent in this lexer */
    expect_tok(name, &l, TOKEN_INVALID, "0x1.0", 0, 0);
    /* Plain hex integer is valid */
    expect_tok(name, &l, TOKEN_NUMBER, "0x1", 0, 0);

    expect_tok(name, &l, TOKEN_EOF, NULL, 0, 0);
}

static void test_number_stops_on_invalid_digit_in_base(void)
{
    const char *name = "number_stops_on_invalid_digit_in_base";
    const char *src = "0b102 0o78";
    struct Lexer l = al_lexer_alloc(src, strlen(src));

    /*
        Current behavior: it tokenizes the longest valid prefix for the base.
        This test documents that behavior (rather than forcing it to be invalid).
    */
    expect_tok(name, &l, TOKEN_NUMBER, "0b10", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, "2", 0, 0);

    expect_tok(name, &l, TOKEN_NUMBER, "0o7", 0, 0);
    expect_tok(name, &l, TOKEN_NUMBER, "8", 0, 0);

    expect_tok(name, &l, TOKEN_EOF, NULL, 0, 0);
}

static void test_helpers_direct(void)
{
    const char *name = "helpers_direct";
    AL_UNUSED(name);

    /* al_is_identifier / al_is_identifier_start */
    assert(al_is_identifier_start('_'));
    assert(al_is_identifier('_'));
    assert(al_is_identifier('a'));
    assert(al_is_identifier('Z'));
    assert(al_is_identifier('9'));
    assert(!al_is_identifier_start('9'));

    /* al_lexer_start_with: empty prefix path */
    {
        struct Lexer l = al_lexer_alloc("abc123", 6);
        assert(al_lexer_start_with(&l, ""));
        assert(al_lexer_start_with(&l, "ab"));
        assert(!al_lexer_start_with(&l, "abcd"));
    }

    /* al_lexer_chop_while + al_lexer_peek */
    {
        struct Lexer l = al_lexer_alloc("abc123", 6);
        al_lexer_chop_while(&l, asm_isalpha);
        assert(l.cursor == 3);
        assert(al_lexer_peek(&l, 0) == '1');
        assert(al_lexer_peek(&l, 100) == '\0');
    }

    /* al_lexer_chop_char newline bookkeeping */
    {
        struct Lexer l = al_lexer_alloc("x\ny", 3);
        assert(l.line_num == 0);
        assert(l.begining_of_line == 0);
        (void)al_lexer_chop_char(&l); /* 'x' */
        (void)al_lexer_chop_char(&l); /* '\n' */
        assert(l.line_num == 1);
        assert(l.begining_of_line == 2);
    }
}

int main(void)
{
    test_basic_program();
    test_pp_directive_and_locations();
    test_whitespace_location_math();
    test_comments();
    test_string_and_char_literals();
    test_literal_operators_longest_match();
    test_numbers_valid_and_invalid();
    test_keyword_vs_identifier_prefix();
    test_hash_not_pp_directive_when_not_column1();
    test_unterminated_block_comment();
    test_hex_float_variants();
    test_number_stops_on_invalid_digit_in_base();
    test_helpers_direct();
    test_invalid_single_char();

    printf("All lexer tests passed.\n");
    return 0;
}