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
    /* Sentinel / unknown */
    TOKEN_EOF,
    TOKEN_INVALID,

    /* High-level / multi-char / "word-like" */
    TOKEN_PP_DIRECTIVE,
    TOKEN_COMMENT,
    TOKEN_STRING_LIT,
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,

    /* Grouping / separators */
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_LBRACE,
    TOKEN_RBRACE,

    /* Punctuation */
    TOKEN_DOT,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_BSLASH,

    /* Ternary */
    TOKEN_QUESTION,
    TOKEN_COLON,

    /* Assignment / equality */
    TOKEN_EQ,
    TOKEN_EQEQ,
    TOKEN_NE,
    TOKEN_BANG,

    /* Relational */
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_LE,
    TOKEN_GE,

    /* Bitwise / boolean */
    TOKEN_BITAND,
    TOKEN_ANDAND,
    TOKEN_BITOR,
    TOKEN_OROR,
    /* Bitwise unary */
    TOKEN_CARET,
    TOKEN_TILDE,

    /* Shifts */
    TOKEN_LSHIFT,
    TOKEN_RSHIFT,

    /* Inc / dec */
    TOKEN_PLUSPLUS,
    TOKEN_MINUSMINUS,

    /* Arithmetic */
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_PERCENT,

    /* Compound assignment */
    TOKEN_PLUSEQ,
    TOKEN_MINUSEQ,
    TOKEN_STAREQ,
    TOKEN_SLASHEQ,
    TOKEN_PERCENTEQ,
    TOKEN_ANDEQ,
    TOKEN_OREQ,
    TOKEN_XOREQ,
    TOKEN_LSHIFTEQ,
    TOKEN_RSHIFTEQ,

    /* Member access / varargs */
    TOKEN_ARROW,
    TOKEN_ELLIPSIS,
};

struct Literal_Token {
    enum Token_Kind kind;
    const char * const text;
};

static struct Literal_Token literal_tokens[] = {
    {.text = "("  , .kind = TOKEN_LPAREN}, 
    {.text = ")"  , .kind = TOKEN_RPAREN},
    {.text = "["  , .kind = TOKEN_LBRACKET}, 
    {.text = "]"  , .kind = TOKEN_RBRACKET},
    {.text = "{"  , .kind = TOKEN_LBRACE}, 
    {.text = "}"  , .kind = TOKEN_RBRACE},
    {.text = "...", .kind = TOKEN_ELLIPSIS},
    {.text = "."  , .kind = TOKEN_DOT},
    {.text = ","  , .kind = TOKEN_COMMA},
    {.text = "?"  , .kind = TOKEN_QUESTION},
    {.text = ":"  , .kind = TOKEN_COLON},
    {.text = "==" , .kind = TOKEN_EQEQ},
    {.text = "!=" , .kind = TOKEN_NE},
    {.text = "="  , .kind = TOKEN_EQ},
    {.text = "!"  , .kind = TOKEN_BANG},
    {.text = ";"  , .kind = TOKEN_SEMICOLON},
    {.text = "\\" , .kind = TOKEN_BSLASH},
    {.text = "->" , .kind = TOKEN_ARROW},
    {.text = ">"  , .kind = TOKEN_GT},
    {.text = ">=" , .kind = TOKEN_GE},
    {.text = "<"  , .kind = TOKEN_LT},
    {.text = "<=" , .kind = TOKEN_LE},
    {.text = "<<=", .kind = TOKEN_LSHIFTEQ},
    {.text = ">>=", .kind = TOKEN_RSHIFTEQ},
    {.text = "++" , .kind = TOKEN_PLUSPLUS},
    {.text = "--" , .kind = TOKEN_MINUSMINUS},
    {.text = "<<" , .kind = TOKEN_LSHIFT},
    {.text = ">>" , .kind = TOKEN_RSHIFT},
    {.text = "+=", .kind = TOKEN_PLUSEQ},
    {.text = "-=", .kind = TOKEN_MINUSEQ},
    {.text = "*=", .kind = TOKEN_STAREQ},
    {.text = "/=", .kind = TOKEN_SLASHEQ},
    {.text = "%=", .kind = TOKEN_PERCENTEQ},
    {.text = "&=", .kind = TOKEN_ANDEQ},
    {.text = "|=", .kind = TOKEN_OREQ},
    {.text = "^=", .kind = TOKEN_XOREQ},
    {.text = "||", .kind = TOKEN_OROR},
    {.text = "&&", .kind = TOKEN_ANDAND},
    {.text = "|" , .kind = TOKEN_BITOR},
    {.text = "&" , .kind = TOKEN_BITAND},
    {.text = "^" , .kind = TOKEN_CARET},
    {.text = "~" , .kind = TOKEN_TILDE},
    {.text = "+" , .kind = TOKEN_PLUS},
    {.text = "-" , .kind = TOKEN_MINUS},
    {.text = "*" , .kind = TOKEN_STAR},
    {.text = "/" , .kind = TOKEN_SLASH},
    {.text = "%" , .kind = TOKEN_PERCENT},
};
#define literal_tokens_count (sizeof(literal_tokens) / sizeof(literal_tokens[0]))

static const char * const keywords[] = {
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
        .kind = TOKEN_INVALID,
        .text = &(l->content[l->cursor]),
        .text_len = 0,
        .location.line_num = l->line_num,
        .location.col = l->cursor - l->begining_of_line,
    };

    if (l->cursor >= l->content_len) {
        token.kind = TOKEN_EOF;
        token.text_len = 0;
    } else if (l->content[l->cursor] == '#' && token.location.col == 0) {
        token.kind = TOKEN_PP_DIRECTIVE;
        for (;l->cursor < l->content_len && l->content[l->cursor] != '\n';) {
            al_lexer_chop_char(l);
            token.text_len++;
        }
        if (l->cursor < l->content_len) {
            al_lexer_chop_char(l);
        }
    } else if (al_is_symbol_start(l->content[l->cursor])) {
        token.kind = TOKEN_IDENTIFIER;
        for ( ; l->cursor < l->content_len && al_is_symbol(l->content[l->cursor]); ) {
            token.text_len++;
            al_lexer_chop_char(l);
        }
        for (size_t i = 0; i < keywords_count; i++) {
            if ((token.text_len == asm_length(keywords[i])) && (asm_strncmp(token.text, keywords[i], asm_length(keywords[i])))) {
                token.kind = TOKEN_KEYWORD;
                break;
            }
        }
    } else if (l->content[l->cursor] == '"') {
        token.kind = TOKEN_STRING_LIT;
        token.text_len++;
        al_lexer_chop_char(l);
        for ( ; (l->cursor < l->content_len) && (l->content[l->cursor] != '"') && (l->content[l->cursor] != '\n'); ) {
            token.text_len++;
            al_lexer_chop_char(l);
        }
        if ((l->cursor < l->content_len) && (l->content[l->cursor] == '"')) {
            token.text_len++;
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
        token.text_len = 2;
        al_lexer_chop_char(l);
        al_lexer_chop_char(l);
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
    if (l->cursor + prefix_len > l->content_len) {
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
    printf("%4zu:%-3zu:(%-18s) -> \"%.*s\"\n", tok.location.line_num, tok.location.col, al_token_kind_name(tok.kind), (int)tok.text_len, tok.text);
}

const char *al_token_kind_name(enum Token_Kind kind)
{
    switch (kind) {
        case TOKEN_EOF:
            return ("TOKEN_EOF");
        case TOKEN_INVALID:
            return ("TOKEN_INVALID");
        case TOKEN_PP_DIRECTIVE:
            return ("TOKEN_PP_DIRECTIVE");
        case TOKEN_IDENTIFIER:
            return ("TOKEN_IDENTIFIER");
        case TOKEN_LPAREN:
            return ("TOKEN_LPAREN");
        case TOKEN_RPAREN:
            return ("TOKEN_RPAREN");
        case TOKEN_LBRACKET:
            return ("TOKEN_LBRACKET");
        case TOKEN_RBRACKET:
            return ("TOKEN_RBRACKET");
        case TOKEN_LBRACE:
            return ("TOKEN_LBRACE");
        case TOKEN_RBRACE:
            return ("TOKEN_RBRACE");
        case TOKEN_DOT:
            return ("TOKEN_DOT");
        case TOKEN_COMMA:
            return ("TOKEN_COMMA");
        case TOKEN_SEMICOLON:
            return ("TOKEN_SEMICOLON");
        case TOKEN_BSLASH:
            return ("TOKEN_BSLASH");
        case TOKEN_QUESTION:
            return ("TOKEN_QUESTION");
        case TOKEN_COLON:
            return ("TOKEN_COLON");
        case TOKEN_LT:
            return ("TOKEN_LT");
        case TOKEN_GT:
            return ("TOKEN_GT");
        case TOKEN_GE:
            return ("TOKEN_GE");
        case TOKEN_LE:
            return ("TOKEN_LE");
        case TOKEN_KEYWORD:
            return ("TOKEN_KEYWORD");
        case TOKEN_COMMENT:
            return ("TOKEN_COMMENT");
        case TOKEN_STRING_LIT:
            return ("TOKEN_STRING_LIT");
        case TOKEN_EQ:
            return ("TOKEN_EQ");
        case TOKEN_EQEQ:
            return ("TOKEN_EQEQ");
        case TOKEN_NE:
            return ("TOKEN_NE");
        case TOKEN_BANG:
            return ("TOKEN_BANG");
        case TOKEN_BITAND:
            return ("TOKEN_BITAND");
        case TOKEN_ANDAND:
            return ("TOKEN_ANDAND");
        case TOKEN_BITOR:
            return ("TOKEN_BITOR");
        case TOKEN_OROR:
            return ("TOKEN_OROR");
        case TOKEN_CARET:
            return ("TOKEN_CARET");
        case TOKEN_TILDE:
            return ("TOKEN_TILDE");
        case TOKEN_PLUSPLUS:
            return ("TOKEN_PLUSPLUS");
        case TOKEN_MINUSMINUS:
            return ("TOKEN_MINUSMINUS");
        case TOKEN_LSHIFT:
            return ("TOKEN_LSHIFT");
        case TOKEN_RSHIFT:
            return ("TOKEN_RSHIFT");
        case TOKEN_PLUS:
            return ("TOKEN_PLUS");
        case TOKEN_MINUS:
            return ("TOKEN_MINUS");
        case TOKEN_STAR:
            return ("TOKEN_STAR");
        case TOKEN_SLASH:
            return ("TOKEN_SLASH");
        case TOKEN_PERCENT:
            return ("TOKEN_PERCENT");
        case TOKEN_PLUSEQ:
            return ("TOKEN_PLUSEQ");
        case TOKEN_MINUSEQ:
            return ("TOKEN_MINUSEQ");
        case TOKEN_STAREQ:
            return ("TOKEN_STAREQ");
        case TOKEN_SLASHEQ:
            return ("TOKEN_SLASHEQ");
        case TOKEN_PERCENTEQ:
            return ("TOKEN_PERCENTEQ");
        case TOKEN_ANDEQ:
            return ("TOKEN_ANDEQ");
        case TOKEN_OREQ:
            return ("TOKEN_OREQ");
        case TOKEN_XOREQ:
            return ("TOKEN_XOREQ");
        case TOKEN_LSHIFTEQ:
            return ("TOKEN_LSHIFTEQ");
        case TOKEN_RSHIFTEQ:
            return ("TOKEN_RSHIFTEQ");
        case TOKEN_ARROW:
            return ("TOKEN_ARROW");
        case TOKEN_ELLIPSIS:
            return ("TOKEN_ELLIPSIS");
        default:
            AL_ASSERT(0 && "Unknown kind");
    }
    return NULL;
}

#endif /*ALMOG_LEXER_IMPLEMENTATION*/

