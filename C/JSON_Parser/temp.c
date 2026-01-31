#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#define ALMOG_LEXER_IMPLEMENTATION
#define ALMOG_JSON_PARSER_IMPLEMENTATION
#include "Almog_JSON_Parser.h"

struct Person {
    const char *name;
    int age;
    const char *location;
    int body_count;
};

struct People {
    size_t length;
    size_t capacity;
    struct Person* elements;
};

bool ajp_expect_token(struct Tokens *tokens, enum Token_Kind token_kind) {
    if (tokens->current_token >= tokens->length) {
        return false;
    }
    struct Token current = tokens->elements[tokens->current_token];
    if (current.kind != token_kind) {
        ajp_dprintERROR("%s:%zu:%zu: expected %s, but got %s.", tokens->file_path, current.location.line_num, current.location.col, al_token_kind_name(token_kind), al_token_kind_name(current.kind));
        return false;
    }
    return true;
}

bool ajp_get_and_expect_token(struct Tokens *tokens, enum Token_Kind token_kind) {
    if (tokens->current_token >= tokens->length) {
        return false;
    }
    bool res = ajp_expect_token(tokens, token_kind);
    tokens->current_token++;
    return res;
}

void print_person(struct Person p) 
{
    asm_print_many_times("-", 20);
    asm_dprintSTRING(p.name);
    asm_dprintINT(p.age);
    asm_dprintSTRING(p.location);
    asm_dprintINT(p.body_count);
}

void free_person(struct Person p)
{
    free((char *)p.name);
    free((char *)p.location);
}

void free_people(struct People people)
{
    for (size_t i = 0; i < people.length; i++) {
        free_person(people.elements[i]);
    }
    free(people.elements);
}

bool parse_int(struct Tokens *tokens, int *number)
{
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

bool parse_string(struct Tokens *tokens, const char **string)
{
    if (!ajp_get_and_expect_token(tokens, TOKEN_STRING_LIT)) return false;
    struct Token current = tokens->elements[tokens->current_token-1];
    *string = asm_strdup(current.text, current.text_len);

    return true;
}

bool parse_bool(struct Tokens *tokens, bool *boolean)
{
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

bool parse_person(struct Tokens *tokens, struct Person *p)
{
    if (!ajp_get_and_expect_token(tokens, TOKEN_LBRACE)) return false;

    struct Token current = tokens->elements[tokens->current_token-1];
    if (tokens->elements[tokens->current_token].kind == TOKEN_RBRACE) {
        return true;
    }

    for (;;) {
        if (!ajp_get_and_expect_token(tokens, TOKEN_STRING_LIT)) return false;
        current = tokens->elements[tokens->current_token-1];
        if (asm_strncmp(current.text, "name", current.text_len) == true) {
            if (!ajp_get_and_expect_token(tokens, TOKEN_COLON)) return false;
            if (!parse_string(tokens, &(p->name))) return false;
        } else if (asm_strncmp(current.text, "age", current.text_len) == true) {
            if (!ajp_get_and_expect_token(tokens, TOKEN_COLON)) return false;
            if (!parse_int(tokens, &(p->age))) return false;
        } else if (asm_strncmp(current.text, "location", current.text_len) == true) {
            if (!ajp_get_and_expect_token(tokens, TOKEN_COLON)) return false;
            if (!parse_string(tokens, &(p->location))) return false;
        } else if (asm_strncmp(current.text, "body_count", current.text_len) == true) {
            if (!ajp_get_and_expect_token(tokens, TOKEN_COLON)) return false;
            if (!parse_int(tokens, &(p->body_count))) return false;
        } else {
            ajp_dprintERROR("%s:%zu:%zu: Unexpected filed '%.*s' ", tokens->file_path, current.location.line_num, current.location.col, (int)current.text_len, current.text);
            return false;
        }
        current = tokens->elements[tokens->current_token];
        if (current.kind != TOKEN_COMMA) break;
        tokens->current_token++;
    }

    if (!ajp_get_and_expect_token(tokens, TOKEN_RBRACE)) return false;

    return true;
}

bool parse_people(struct Tokens *tokens, struct People *people)
{
    if (!ajp_get_and_expect_token(tokens, TOKEN_LBRACKET)) return false;

    struct Token current = tokens->elements[tokens->current_token-1];
    if (tokens->elements[tokens->current_token].kind == TOKEN_RBRACE) {
        return true;
    }

    for (;;) {
        struct Person p = {0};
        if (!parse_person(tokens, &p)) return false;
        ada_appand(struct Person, *people, p);

        current = tokens->elements[(tokens->current_token)];
        if (current.kind != TOKEN_COMMA) break;
        tokens->current_token++;
    }

    if (!ajp_expect_token(tokens, TOKEN_RBRACKET)) return false;

    return true;
}

int main(void)
{
    struct Tokens tokens = al_lex_entire_file("profile.json");

    struct People people = {0};
    ada_init_array(struct Person, people);

    parse_people(&tokens, &people);

    for (size_t i = 0; i < people.length; i++) {
        print_person(people.elements[i]);
    }


    al_tokens_free(tokens);
    free_people(people);

    return 0;
}
