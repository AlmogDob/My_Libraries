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

bool parse_person(struct Tokens *tokens, struct Person *p)
{
    if (!ajp_object_begin(tokens)) return false;

    for (;ajp_object_next_member(tokens);) {
        if (asm_strncmp(tokens->current_key, "name", 4) == true) {
            if (!ajp_parse_string(tokens, &(p->name))) return false;
        } else if (asm_strncmp(tokens->current_key, "age", 3) == true) {
            if (!ajp_parse_int(tokens, &(p->age))) return false;
        } else if (asm_strncmp(tokens->current_key, "location", 8) == true) {
            if (!ajp_parse_string(tokens, &(p->location))) return false;
        } else if (asm_strncmp(tokens->current_key, "body_count", 10) == true) {
            if (!ajp_parse_int(tokens, &(p->body_count))) return false;
        } else {
            ajp_unknown_key(tokens);
            return false;
        }
    }

    if (!ajp_object_end(tokens)) return false;

    return true;
}

bool parse_people(struct Tokens *tokens, struct People *people)
{
    if (!ajp_array_begin(tokens)) return false;

    for (;ajp_array_has_items(tokens);) {
        struct Person p = {0};
        if (!parse_person(tokens, &p)) {
            free_person(p);
            return false;
        }
        ada_appand(struct Person, *people, p);
    }

    if (!ajp_array_end(tokens)) return false;

    return true;
}

int main(void)
{
    struct Tokens tokens = al_lex_entire_file("profiles.json");

    struct People people = {0};
    ada_init_array(struct Person, people);

    if (!parse_people(&tokens, &people)) {
        asm_dprintSIZE_T(people.length);
        return 1;
    }

    for (size_t i = 0; i < people.length; i++) {
        print_person(people.elements[i]);
    }


    al_tokens_free(tokens);
    free_people(people);

    return 0;
}
