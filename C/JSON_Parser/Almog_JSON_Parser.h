/**
 * @file
 * @brief Minimal, token-walking JSON parsing helpers (single-header library).
 *
 * This module implements a small “parser” layer on top of the token stream
 * produced by @ref al_lex_entire_file (see "Almog_Lexer.h").
 *
 * The API is intentionally low-level: it does not build an AST. Instead, it
 * provides small utilities that:
 *  - check/consume structural tokens (`{`, `}`, `[`, `]`, `,`)
 *  - step through object members
 *  - parse primitive values (string, int, bool)
 *
 * Usage pattern (typical):
 *  1. Lex a JSON file into `struct Tokens`.
 *  2. Call `ajp_array_begin` / `ajp_object_begin`.
 *  3. Iterate with `ajp_array_has_items` / `ajp_object_next_member`.
 *  4. For each key/value, call a parse function (string/int/bool) and fill
 *     your own structs.
 *
 * Single-header usage:
 *  - In exactly one translation unit, define
 *    `ALMOG_JSON_PARSER_IMPLEMENTATION` before including this header to compile
 *    the implementation.
 *  - In other translation units include the header normally for declarations.
 *
 * Important notes / limitations (implementation-defined behavior):
 *  - The parser operates on `struct Tokens` from the lexer and mutates
 *    `tokens->current_token` as it consumes tokens.
 *  - Some functions rely on @ref AJP_ASSERT for bounds checks. If assertions
 *    are disabled, out-of-bounds reads become possible on malformed input.
 *  - This parser assumes a certain tokenization of JSON by the lexer.
 *    (E.g., keys are expected as `TOKEN_STRING_LIT` tokens.)
 *  - Error reporting uses @ref ajp_dprintERROR (prints to stderr).
 *
 * @note Inspired by Tsoding’s JSON parser approach:
 *       https://youtu.be/FBpgdSjJ6nQ
 */

#ifndef ALMOG_JSON_PARSER_H_
#define ALMOG_JSON_PARSER_H_

#include "Almog_Lexer.h"

/**
 * @def AJP_ASSERT
 * @brief Assertion macro used by the JSON parser (defaults to @c assert()).
 *
 * Override by defining `AJP_ASSERT(expr)` before including this header.
 *
 * @note This library uses assertions mainly for token-stream bounds checks
 *       during parsing helpers. If you compile with `NDEBUG`, these checks may
 *       compile out depending on your assert implementation.
 */
#ifndef AJP_ASSERT
#include <assert.h>
#define AJP_ASSERT assert
#endif /* AJP_ASSERT */ 

/**
 * @def ajp_dprintERROR
 * @brief Print a formatted parser error to stderr with file/line/function info.
 *
 * The printed message format is:
 *  - `__FILE__`, `__LINE__`, `__func__` of the call site,
 *  - then the user-provided formatted message.
 *
 * Example:
 * @code
 * ajp_dprintERROR("%s:%zu:%zu: expected %s", path, line, col, what);
 * @endcode
 *
 * @warning This macro always forwards `__VA_ARGS__`. Calling it without any
 *          variadic arguments (i.e., only a format string) will not compile.
 */
#define ajp_dprintERROR(fmt, ...) \
    fprintf(stderr, "\n%s:%d:\n[Error] in function '%s':\n        " \
    fmt "\n\n", __FILE__, __LINE__, __func__, __VA_ARGS__)


bool ajp_array_begin(struct Tokens *tokens);
bool ajp_array_end(struct Tokens *tokens);
bool ajp_array_has_items(struct Tokens *tokens);
bool ajp_expect_token(struct Tokens *tokens, enum Token_Kind token_kind);
bool ajp_get_and_expect_token(struct Tokens *tokens, enum Token_Kind token_kind);
bool ajp_object_begin(struct Tokens *tokens);
bool ajp_object_end(struct Tokens *tokens);
bool ajp_object_next_member(struct Tokens *tokens);
bool ajp_parse_bool(struct Tokens *tokens, bool *boolean);
bool ajp_parse_int(struct Tokens *tokens, int *number);
bool ajp_parse_string(struct Tokens *tokens, const char **string);
void ajp_unknown_key(struct Tokens *tokens);


#endif /*ALMOG_JSON_PARSER_H_*/

#ifdef ALMOG_JSON_PARSER_IMPLEMENTATION
#undef ALMOG_JSON_PARSER_IMPLEMENTATION

/**
 * @brief Consume a JSON array opening bracket `'['`.
 *
 * Equivalent to expecting the current token to be @ref TOKEN_LBRACKET and
 * advancing `tokens->current_token` by 1 on success.
 *
 * @param tokens Token stream (mutated).
 * @return true on success, false on mismatch (and prints an error).
 *
 * @pre `tokens` is non-NULL.
 * @pre `tokens->current_token` points to the next token to be consumed.
 */
bool ajp_array_begin(struct Tokens *tokens)
{
    AJP_ASSERT(tokens->current_token < tokens->length - 1);

    return ajp_get_and_expect_token(tokens, TOKEN_LBRACKET);
}

/**
 * @brief Consume a JSON array closing bracket `']'`.
 *
 * Equivalent to expecting the current token to be @ref TOKEN_RBRACKET and
 * advancing `tokens->current_token` by 1 on success.
 *
 * @param tokens Token stream (mutated).
 * @return true on success, false on mismatch (and prints an error).
 */
bool ajp_array_end(struct Tokens *tokens)
{
    AJP_ASSERT(tokens->current_token < tokens->length - 1);

    return ajp_get_and_expect_token(tokens, TOKEN_RBRACKET);
}

/**
 * @brief Decide whether an array has more items to parse, handling commas.
 *
 * This helper inspects the current token (without consuming an item value):
 *  - If the token is a comma (`TOKEN_COMMA`), it consumes the comma and returns
 *    true (meaning “there is another item”).
 *  - If the token is `TOKEN_RBRACKET`, returns false (meaning “end of array”).
 *  - Otherwise returns true (meaning “an item should follow now”).
 *
 * Intended loop:
 * @code
 * ajp_array_begin(tokens);
 * while (ajp_array_has_items(tokens)) {
 *     ... parse one value ...
 * }
 * ajp_array_end(tokens);
 * @endcode
 *
 * @param tokens Token stream (mutated when consuming commas).
 * @return false when the next token is `]`, true otherwise.
 *
 * @warning This function treats “anything other than `]`” as “has items”.
 *          That means malformed arrays can still return true and later fail
 *          in the value parser.
 */
bool ajp_array_has_items(struct Tokens *tokens)
{
    AJP_ASSERT(tokens->current_token < tokens->length - 1);

    struct Token current = tokens->elements[tokens->current_token];
    if (current.kind == TOKEN_COMMA) {
        tokens->current_token++;
        return true;
    } else if (current.kind == TOKEN_RBRACKET) {
        return false;
    }

    return true;
}

/**
 * @brief Check whether the current token has the expected kind (no consume).
 *
 * If the current token kind differs, an error is printed via
 * @ref ajp_dprintERROR and false is returned.
 *
 * @param tokens Token stream (not advanced).
 * @param token_kind Expected token kind.
 * @return true if the current token kind matches, false otherwise.
 *
 * @pre `tokens->current_token < tokens->length`.
 */
bool ajp_expect_token(struct Tokens *tokens, enum Token_Kind token_kind)
{
    AJP_ASSERT(tokens->current_token < tokens->length);

    struct Token current = tokens->elements[tokens->current_token];
    if (current.kind != token_kind) {
        ajp_dprintERROR("%s:%zu:%zu: expected %s, but got %s.", tokens->file_path, current.location.line_num, current.location.col, al_token_kind_name(token_kind), al_token_kind_name(current.kind));
        return false;
    }
    return true;
}

/**
 * @brief Expect a token kind and advance by one token (consume).
 *
 * Semantics:
 *  1. Checks current token kind equals @p token_kind (prints error if not).
 *  2. Advances `tokens->current_token++` unconditionally after the check.
 *
 * @param tokens Token stream (mutated).
 * @param token_kind Expected token kind.
 * @return true if the token matched, false otherwise.
 *
 * @warning Even on mismatch, this function still advances the token cursor by
 *          one. This makes error recovery “skip one token” style.
 */
bool ajp_get_and_expect_token(struct Tokens *tokens, enum Token_Kind token_kind)
{
    AJP_ASSERT(tokens->current_token < tokens->length - 1);

    if (tokens->current_token >= tokens->length) {
        return false;
    }
    bool res = ajp_expect_token(tokens, token_kind);
    tokens->current_token++;
    return res;
}

/**
 * @brief Consume a JSON object opening brace `'{'`.
 *
 * Equivalent to expecting the current token to be @ref TOKEN_LBRACE and
 * advancing `tokens->current_token` by 1 on success.
 *
 * @param tokens Token stream (mutated).
 * @return true on success, false on mismatch (and prints an error).
 */
bool ajp_object_begin(struct Tokens *tokens)
{
    AJP_ASSERT(tokens->current_token < tokens->length - 1);

    return ajp_get_and_expect_token(tokens, TOKEN_LBRACE);
}

/**
 * @brief Consume a JSON object closing brace `'}'`.
 *
 * Equivalent to expecting the current token to be @ref TOKEN_RBRACE and
 * advancing `tokens->current_token` by 1 on success.
 *
 * @param tokens Token stream (mutated).
 * @return true on success, false on mismatch (and prints an error).
 */
bool ajp_object_end(struct Tokens *tokens)
{
    AJP_ASSERT(tokens->current_token < tokens->length - 1);

    return ajp_get_and_expect_token(tokens, TOKEN_RBRACE);
}

/**
 * @brief Advance to the next object member and expose its key in `tokens`.
 *
 * Intended JSON object shape:
 * @code
 * { "key": value, "key2": value2 }
 * @endcode
 *
 * Behavior (as implemented):
 *  - If the current token is `TOKEN_COMMA`, it consumes it.
 *  - If the current token is `TOKEN_RBRACE`, returns false (no more members).
 *  - Otherwise it assumes the current token is the key token and:
 *      - sets `tokens->current_key` / `tokens->current_key_len` from that token
 *      - advances `tokens->current_token += 2`
 *
 * The `+= 2` is meant to skip:
 *  - the key token, and
 *  - the colon token
 *
 * so that after this function returns true, `tokens->current_token` should
 * point at the value token for the member.
 *
 * Usage:
 * @code
 * ajp_object_begin(tokens);
 * while (ajp_object_next_member(tokens)) {
 *     if (asm_strncmp(tokens->current_key, "name", 4)) {
 *         ajp_parse_string(tokens, &out->name);
 *     } else {
 *         ajp_unknown_key(tokens);
 *         return false;
 *     }
 * }
 * ajp_object_end(tokens);
 * @endcode
 *
 * @param tokens Token stream (mutated).
 * @return true if a next member is available and the cursor was advanced to its
 *         value, false if the next token is `}`.
 *
 * @warning This function does not validate that the key is a string literal
 *          or that the following token is a colon. If the token stream is not
 *          in the expected shape, `current_key` may be nonsensical and the
 *          cursor may skip incorrectly.
 */
bool ajp_object_next_member(struct Tokens *tokens)
{
    AJP_ASSERT(tokens->current_token < tokens->length - 2);

    struct Token current = tokens->elements[tokens->current_token];
    if (current.kind == TOKEN_COMMA) {
        tokens->current_token += 1;
        current = tokens->elements[tokens->current_token];
    } else if (current.kind == TOKEN_RBRACE) {
        return false;
    }

    tokens->current_key = current.text;
    tokens->current_key_len = current.text_len;
    tokens->current_token += 2;
    return true;
}

/**
 * @brief Parse a boolean value into @p boolean.
 *
 * Expected token kind (implementation detail): `TOKEN_STRING_LIT`.
 * The token text is compared against `"true"` and `"false"` (case-sensitive).
 *
 * @param tokens Token stream (mutated).
 * @param boolean Output pointer; set to true/false on success.
 * @return true on success, false on mismatch/unrecognized spelling.
 *
 * @warning As implemented, this function expects the boolean to appear as a
 *          string-literal token, not as an identifier token. Ensure your lexer
 *          produces `TOKEN_STRING_LIT` for your boolean representation, or
 *          adjust the implementation.
 */
bool ajp_parse_bool(struct Tokens *tokens, bool *boolean)
{
    AJP_ASSERT(tokens->current_token < tokens->length - 1);

    if (!ajp_get_and_expect_token(tokens, TOKEN_STRING_LIT)) return false;
    struct Token current = tokens->elements[tokens->current_token-1];
    if (asm_strncmp(current.text, "true", current.text_len)) {
        *boolean = true;
    } else if (asm_strncmp(current.text, "false", current.text_len)) {
        *boolean = false;
    } else {
        return false;
    }

    return true;
}

/**
 * @brief Parse an integer value into @p number.
 *
 * Expected shapes (implementation detail):
 *  - `TOKEN_INT_LIT_DEC`
 *  - or a leading sign token (`TOKEN_PLUS` or `TOKEN_MINUS`) in front of an
 *    integer literal
 *
 * The implementation consumes tokens and converts text using
 * `asm_str2int(..., 10)`.
 *
 * @param tokens Token stream (mutated).
 * @param number Output pointer; written on success.
 * @return true on success, false on mismatch.
 *
 * @warning The current implementation advances the token cursor in a way that
 *          can skip extra tokens around signs. Documented here for correctness,
 *          but consider revisiting the implementation if you observe misparses.
 */
bool ajp_parse_int(struct Tokens *tokens, int *number)
{
    AJP_ASSERT(tokens->current_token < tokens->length - 1);

    struct Token current = tokens->elements[tokens->current_token];
    tokens->current_token++;
    switch (current.kind) {
        case TOKEN_PLUS:
            tokens->current_token++;
            /* fall through */
        case TOKEN_MINUS:
            tokens->current_token++;
            /* fall through */
        case TOKEN_INT_LIT_DEC:
            *number = asm_str2int(current.text, NULL, 10);
            break;
        default:
            return false;
    }
    return true;
}

/**
 * @brief Parse a JSON string into a newly allocated C string.
 *
 * Expects a token of kind `TOKEN_STRING_LIT` and duplicates its bytes using
 * `asm_strdup(token.text, token.text_len)`.
 *
 * @param tokens Token stream (mutated).
 * @param string Output pointer; receives a heap-allocated, null-terminated
 *               string on success.
 * @return true on success, false otherwise.
 *
 * @post On success, the caller owns `*string` and must free it using the
 *       allocator compatible with `ASM_MALLOC` (used by `asm_strdup`).
 */
bool ajp_parse_string(struct Tokens *tokens, const char **string)
{
    AJP_ASSERT(tokens->current_token < tokens->length - 1);

    if (!ajp_get_and_expect_token(tokens, TOKEN_STRING_LIT)) return false;
    struct Token current = tokens->elements[tokens->current_token-1];
    *string = asm_strdup(current.text, current.text_len);

    return true;
}

/**
 * @brief Report an “unknown key” error for the current object member.
 *
 * This helper prints an error using the token at `tokens->current_token - 2`,
 * which is expected (by this parser’s object iteration scheme) to be the key
 * token of the current member.
 *
 * @param tokens Token stream (not advanced).
 *
 * @note This function is typically called from user code when the key does not
 *       match any expected field.
 */
void ajp_unknown_key(struct Tokens *tokens) 
{
    struct Token current = tokens->elements[tokens->current_token-2];
    ajp_dprintERROR("%s:%zu:%zu: Unexpected filed '%.*s' ", tokens->file_path, current.location.line_num, current.location.col, (int)current.text_len, current.text);
}




#endif /*ALMOG_JSON_PARSER_IMPLEMENTATION*/

