/** This single header library is insapired by
 * Tsoding's C-lexer implementation: https://youtu.be/AqyZztKlSGQ
 */
#ifndef ALMOG_LEXER_H_
#define ALMOG_LEXER_H_

#include "Almog_String_Manipulation.h"

#ifndef AL_ASSERT
#include <assert.h>
#define AL_ASSERT assert
#endif /* AL_ASSERT */ 

enum Token_Kind {
    TOKEN_END,
    TOKEN_PREPROC,
    TOKEN_SYMBOL,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACK,
    TOKEN_RBRACK,
    TOKEN_SEMICOL,
    TOKEN_GREATER,
    TOKEN_LESSER,
    TOKEN_GE,
    TOKEN_LE,
    TOKEN_BINOR,
    TOKEN_BOOLOR,
    TOKEN_BINAND,
    TOKEN_BOOLAND,
    TOKEN_KEYWORD,
    TOKEN_COMMENT,
    TOKEN_STRING,
    TOKEN_UNKNOWN,
};

struct Literal_Token {
    enum Token_Kind kind;
    const char * const text;
};

struct Literal_Token literal_tokens[] = {
    {.text = "(" , .kind = TOKEN_LPAREN}, 
    {.text = ")" , .kind = TOKEN_RPAREN},
    {.text = "{" , .kind = TOKEN_LBRACK}, 
    {.text = "}" , .kind = TOKEN_RBRACK},
    {.text = ";" , .kind = TOKEN_SEMICOL},
    {.text = ">" , .kind = TOKEN_GREATER},
    {.text = ">=", .kind = TOKEN_GE},
    {.text = "<" , .kind = TOKEN_LESSER},
    {.text = "<=", .kind = TOKEN_LE},
    {.text = "|" , .kind = TOKEN_BINOR},
    {.text = "||", .kind = TOKEN_BOOLOR},
    {.text = "&" , .kind = TOKEN_BINAND},
    {.text = "&&", .kind = TOKEN_BOOLAND},
};
#define literal_tokens_count (sizeof(literal_tokens) / sizeof(literal_tokens[0]))

const char * const keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double",
    "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register",
    "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
    "union", "unsigned", "void", "volatile", "while", "alignas", "alignof", "and",
    "and_eq", "asm", "atomic_cancel", "atomic_commit", "atomic_noexcept", "bitand",
    "bitor", "bool", "catch", "char16_t", "char32_t", "char8_t", "class", "co_await",
    "co_return", "co_yield", "compl", "concept", "const_cast", "consteval", "constexpr",
    "constinit", "decltype", "delete", "dynamic_cast", "explicit", "export", "false",
    "friend", "inline", "mutable", "namespace", "new", "noexcept", "not", "not_eq",
    "nullptr", "operator", "or", "or_eq", "private", "protected", "public", "reflexpr",
    "reinterpret_cast", "requires", "static_assert", "static_cast", "synchronized",
    "template", "this", "thread_local", "throw", "true", "try", "typeid", "typename",
    "using", "virtual", "wchar_t", "xor", "xor_eq",
};
#define keywords_count (sizeof(keywords) / sizeof(keywords[0]))

struct Location {
    size_t line_num;
    size_t col;
};

struct Token {
    enum Token_Kind kind;
    const char *text;
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

bool            al_is_symbol(char c);
bool            al_is_symbol_start(char c);
struct Lexer    al_lexer_alloc(const char *content, size_t len);
char            al_lexer_chop_char(struct Lexer *l);
struct Token    al_lexer_next_token(struct Lexer *l);
bool            al_lexer_start_with(struct Lexer *l, const char *prefix);
void            al_lexer_trim_left(struct Lexer *l);
void            al_token_print(struct Token tok);
const char *    al_token_kind_name(enum Token_Kind kind);

#endif /*ALMOG_LEXER_H_*/

#ifdef ALMOG_LEXER_IMPLEMENTATION
#undef ALMOG_LEXER_IMPLEMENTATION

#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "Almog_String_Manipulation.h"

bool al_is_symbol(char c)
{
    return asm_isalnum(c) || c == '_';
}

bool al_is_symbol_start(char c)
{
    return asm_isalpha(c) || c == '_';
}

struct Lexer al_lexer_alloc(const char *content, size_t len)
{
    struct Lexer l = {0};
    l.content = content;
    l.content_len = len;
    return l;
}

char al_lexer_chop_char(struct Lexer *l)
{
    AL_ASSERT(l->cursor < l->content_len);
    char c = l->content[l->cursor++]; 
    if (c == '\n') {
        l->line_num++;
        l->begining_of_line = l->cursor;
    }
    return c;
}

struct Token al_lexer_next_token(struct Lexer *l)
{
    al_lexer_trim_left(l);

    struct Token token = {
        .kind = TOKEN_UNKNOWN,
        .text = &(l->content[l->cursor]),
        .text_len = 0,
        .location.line_num = l->line_num,
        .location.col = l->cursor - l->begining_of_line,
    };

    if (l->cursor >= l->content_len) {
        token.kind = TOKEN_END;
        token.text_len = 0;
    } else if (l->content[l->cursor] == '#') {
        token.kind = TOKEN_PREPROC;
        for (;l->cursor < l->content_len && l->content[l->cursor] != '\n';) {
            al_lexer_chop_char(l);
            token.text_len++;
        }
        if (l->cursor < l->content_len) {
            al_lexer_chop_char(l);
        }
    } else if (al_is_symbol_start(l->content[l->cursor])) {
        token.kind = TOKEN_SYMBOL;
        for ( ; l->cursor < l->content_len && al_is_symbol(l->content[l->cursor]); ) {
            token.text_len++;
            al_lexer_chop_char(l);
        }
        for (size_t i = 0; i < keywords_count; i++) {
            if (asm_strncmp(token.text, keywords[i], asm_length(keywords[i]))) {
                token.kind = TOKEN_KEYWORD;
                break;
            }
        }
    } else if (l->content[l->cursor] == '"') {
        token.kind = TOKEN_STRING;
        token.text_len++;
        al_lexer_chop_char(l);
        for ( ; (l->cursor < l->content_len) && (l->content[l->cursor] != '"') && (l->content[l->cursor] != '\n'); ) {
            token.text_len++;
            al_lexer_chop_char(l);
        }
        token.text_len++;
        if (l->cursor < l->content_len) {
            al_lexer_chop_char(l);
        }
    } else if (al_lexer_start_with(l, "//")) {
        token.kind = TOKEN_COMMENT;
        for (;l->cursor < l->content_len && l->content[l->cursor] != '\n';) {
            al_lexer_chop_char(l);
            token.text_len++;
        }
        if (l->cursor < l->content_len) {
            al_lexer_chop_char(l);
        }
    } else if (al_lexer_start_with(l, "/*")) {
        token.kind = TOKEN_COMMENT;
        for ( ; l->cursor < l->content_len; ) {
            token.text_len++;
            if ((l->content[l->cursor-1] == '*') && (l->content[l->cursor] == '/')) {
                al_lexer_chop_char(l);
                break;
            }
            al_lexer_chop_char(l);
        }
    } else {
        size_t longest_matching_token = 0;
        for (size_t i = 0; i < literal_tokens_count; i++) {
            if (al_lexer_start_with(l, literal_tokens[i].text)) {
                /* NOTE: assumes that literal_tokens[i].text does not have any '\n' */
                size_t text_len = asm_length(literal_tokens[i].text);
                if (text_len > longest_matching_token) {
                    longest_matching_token = text_len;
                    token.kind = literal_tokens[i].kind;
                    token.text_len = text_len;
                }
            }
        }
        if (longest_matching_token == 0) {
            token.text_len = 1;
        }
        for (size_t i = 0; i < token.text_len; i++) {
            al_lexer_chop_char(l);
        }
    }

    return token;
}

bool al_lexer_start_with(struct Lexer *l, const char *prefix)
{
    size_t prefix_len = asm_length(prefix); 
    if (prefix_len == 0) {
        return true;
    }
    if (l->cursor + prefix_len-1 >= l->content_len) {
        return false;
    }
    for (size_t i = 0; i < prefix_len; i++) {
        if (prefix[i] != l->content[l->cursor + i]) {
            return false;
        }
    }
    return true;
}

void al_lexer_trim_left(struct Lexer *l)
{
    for (;l->cursor < l->content_len;) {
        if (!asm_isspace(l->content[l->cursor])) {
            break;
        }
        al_lexer_chop_char(l);
    }
}

void al_token_print(struct Token tok)
{
    printf("%-3zu:%-2zu:\t(%s)\t-> \"%.*s\"\n", tok.location.line_num, tok.location.col, al_token_kind_name(tok.kind), (int)tok.text_len, tok.text);
}

const char *al_token_kind_name(enum Token_Kind kind)
{
    switch (kind) {
        case TOKEN_END:   
            return ("TOKEN_END");
        case TOKEN_PREPROC: 
            return ("TOKEN_PREPROC");
        case TOKEN_SYMBOL:
            return ("TOKEN_SYMBOL");
        case TOKEN_LPAREN:
            return ("TOKEN_LPAREN");
        case TOKEN_RPAREN:
            return ("TOKEN_RPAREN");
        case TOKEN_LBRACK:
            return ("TOKEN_LBRACK");
        case TOKEN_RBRACK:
            return ("TOKEN_RBRACK");
        case TOKEN_UNKNOWN:
            return ("TOKEN_UNKNOWN");
        case TOKEN_SEMICOL:
            return ("TOKEN_SEMICOL");
        case TOKEN_GREATER:
            return ("TOKEN_GREATER");
        case TOKEN_LESSER:
            return ("TOKEN_LESSER");
        case TOKEN_GE:
            return ("TOKEN_GE");
        case TOKEN_LE:
            return ("TOKEN_LE");
        case TOKEN_BINOR:
            return ("TOKEN_BINOR");
        case TOKEN_BOOLOR:
            return ("TOKEN_BOOLOR");
        case TOKEN_BINAND:
            return ("TOKEN_BINAND");
        case TOKEN_BOOLAND:
            return ("TOKEN_BOOLAND");
        case TOKEN_KEYWORD:
            return ("TOKEN_KEYWORD");
        case TOKEN_COMMENT:
            return ("TOKEN_COMMENT");
        case TOKEN_STRING:
            return ("TOKEN_STRING");
        default:
            AL_ASSERT(0 && "Unknown kind");
    }
    return NULL;
}

#endif /*ALMOG_LEXER_IMPLEMENTATION*/

