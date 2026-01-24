/** This single header library is insapired by
 * Tsoding's C-lexer implementation: https://youtu.be/AqyZztKlSGQ
 */
#ifndef ALMOG_LEXER_H_
#define ALMOG_LEXER_H_

#include "Almog_String_Manipulation.h"

enum Token_Kind {
    TOKEN_UNKNOWN,
    TOKEN_POUND,
    TOKEN_SYMBOL,
    TOKEN_END,
};

struct Location {
    size_t line_num;
    size_t col;
};

struct Token {
    enum Token_Kind kind;
    const char * const text;
    size_t text_len;
    struct Location location;
};

struct Lexer {
    const char * content;
    size_t content_len;
    size_t cursor;
    size_t line_num;
    size_t begining_of_line;
};

#define AL_UNUSED(x) (void)x

struct Lexer    al_lexer_alloc(const char *content, size_t len);
void            al_lexer_trim_left(struct Lexer *l);
struct Token    al_lexer_next_token(struct Lexer *l);

#endif /*ALMOG_LEXER_H_*/

#ifdef ALMOG_LEXER_IMPLEMENTATION
#undef ALMOG_LEXER_IMPLEMENTATION

#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "Almog_String_Manipulation.h"

struct Lexer al_lexer_alloc(const char *content, size_t len)
{
    struct Lexer l = {0};
    l.content = content;
    l.content_len = len;
    return l;
}

void al_lexer_trim_left(struct Lexer *l)
{
    for (;l->cursor < l->content_len;) {
        if (!asm_isspace(l->content[l->cursor])) {
            break;
        }
        l->cursor++;
    }
}

struct Token al_lexer_next_token(struct Lexer *l)
{
    al_lexer_trim_left(l);

    struct Token token = {
        .text = &(l->content[l->cursor])
    };

    if (l->cursor >= l->content_len) return token;

    if (l->content[l->cursor] == '#') {
        l->cursor += 1;
        token.kind = TOKEN_POUND;
        token.text_len = 1;
        token.location.line_num = l->line_num;
        return token;
    }
    

    // UNIMPLEMENTED("al_lexer_next");
    AL_UNUSED(l);

    return token;
}




#endif /*ALMOG_LEXER_IMPLEMENTATION*/

