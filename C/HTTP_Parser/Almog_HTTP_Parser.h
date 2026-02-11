#ifndef ALMOG_HTTP_PARSER_H_
#define ALMOG_HTTP_PARSER_H_

#include "Almog_String_Manipulation.h"

#ifndef AHP_ASSERT
    #include <assert.h>
    #define AHP_ASSERT assert
#endif

enum Ahp_HTTP_Methods {
    AHP_GET,
    AHP_POST,
    AHP_NUM_OF_METHODS
};
    
struct Ahp_HTTP_Request_Line {
    char HTTP_version[4];
    const char *request_target;
    size_t request_target_len;
    enum Ahp_HTTP_Methods method;
};

struct Ahp_HTTP_Head {
    const char *content;
    size_t content_len;
};

struct Ahp_HTTP_Body {
    const char *content;
    size_t content_len;
};

struct Ahp_HTTP_Message {
    const char *content;
    size_t cursor;
    size_t content_len;

    struct Ahp_HTTP_Request_Line HTTP_request_line;

    struct Ahp_HTTP_Head HTTP_head;

    struct Ahp_HTTP_Body HTTP_body;

};

int     ahp_GET_parse(struct Ahp_HTTP_Message *msg);
void    ahp_HTTP_message_cursor_advenc(struct Ahp_HTTP_Message *msg, size_t count);
int     ahp_HTTP_message_cursor_expect_advenc(struct Ahp_HTTP_Message *msg, char c);
int     ahp_HTTP_message_parse(struct Ahp_HTTP_Message *msg);
char *  ahp_HTTP_message_method_name(enum Ahp_HTTP_Methods method);
void    ahp_HTTP_request_line_print(struct Ahp_HTTP_Request_Line rl);

#endif /* ALMOG_HTTP_PARSER_H_*/

#ifdef ALMOG_HTTP_PARSER_IMPLEMENTATION
#undef ALMOG_HTTP_PARSER_IMPLEMENTATION

int ahp_GET_parse(struct Ahp_HTTP_Message *msg)
{
    char current_word[ASM_MAX_LEN] = {0xFe};
    /* parse method */
    msg->cursor += asm_get_next_token_from_str(current_word, &(msg->content[msg->cursor]), ' ');
    if (!asm_strncmp(current_word, "GET", 0)) {
        asm_dprintERROR("GET parser got a non GET message. Method of message: '%s'", current_word);
        return 1;
    }
    msg->HTTP_request_line.method = AHP_GET;
    ahp_HTTP_message_cursor_advenc(msg, 1); /* skip ' ' */
    /* parse request target */
    if (ahp_HTTP_message_cursor_expect_advenc(msg, '/')) return 1; /* skip '/' */
    msg->HTTP_request_line.request_target = &(msg->content[msg->cursor]);
    if (msg->content[msg->cursor] == ' ') {
        msg->HTTP_request_line.request_target_len = 0;
    } else {
        int rc = asm_get_next_token_from_str(current_word, &(msg->content[msg->cursor]), ' ');
        ahp_HTTP_message_cursor_advenc(msg, rc);
        msg->HTTP_request_line.request_target_len = &(msg->content[msg->cursor]) - msg->HTTP_request_line.request_target;
    }
    ahp_HTTP_message_cursor_advenc(msg, 1); /* skip ' ' */
    /* parse HTTP version */
    if (ahp_HTTP_message_cursor_expect_advenc(msg, '/')) return 1; /* skip '/' */
    msg->cursor += asm_get_next_token_from_str(current_word, &(msg->content[msg->cursor]), '/');
    if (!asm_strncmp(current_word, "HTTP", 0)) {
        asm_dprintERROR("expected HTTP befor HTTP version but got: '%s'", current_word);
        return 1;
    }
    if (ahp_HTTP_message_cursor_expect_advenc(msg, '/')) return 1; /* skip '/' */
    msg->cursor += asm_get_next_token_from_str(current_word, &(msg->content[msg->cursor]), '\r');
    if (!asm_strncmp(current_word, "1.1", 0)) {
        asm_dprintERROR("unsupported HTTP version '%s'. Supports only version 1.1", current_word);
        return 1;
    }
    asm_strncpy(msg->HTTP_request_line.HTTP_version, current_word, 4);
    ahp_HTTP_message_cursor_advenc(msg, 2); /* skip '\r\n' */

    // asm_dprintSTRING(current_word);
    // asm_dprintSTRING(&(msg->content[msg->cursor]));

    return 0;
}   

void ahp_HTTP_message_cursor_advenc(struct Ahp_HTTP_Message *msg, size_t count)
{
    AHP_ASSERT(msg->cursor < msg->content_len);

    while (count--) {
        msg->cursor++;
    }
}

int ahp_HTTP_message_cursor_expect_advenc(struct Ahp_HTTP_Message *msg, char c)
{
    if (msg->content[msg->cursor] != c) {
        asm_dprintERROR("expected '%c' of request target, but got '%c'", c, msg->content[msg->cursor]);
        return 1;
    }
    ahp_HTTP_message_cursor_advenc(msg, 1); /* skip 'c' */
    return 0;
}

int ahp_HTTP_message_parse(struct Ahp_HTTP_Message *msg)
{
    msg->cursor = 0;

    char current_word[ASM_MAX_LEN] = {0xFe};
    asm_get_next_token_from_str(current_word, msg->content, ' ');
    if (asm_strncmp(current_word, "GET", 0)) {
        int rc = ahp_GET_parse(msg);
        if (rc != 0) {
            asm_dprintERROR("unable to parse GET message. GET parser returned with: %d", rc);
            return 1;
        }
    } else {
        asm_dprintERROR("unknown message method '%s'", current_word);
        return 1;
    }

    return 0;
}

char * ahp_HTTP_message_method_name(enum Ahp_HTTP_Methods method) 
{
    switch (method) {
        case AHP_GET:
            return ("GET");
        case AHP_POST:
            return ("POST");
        default:
            AHP_ASSERT(0 && "Unknown method");
            return "UNREACHABLE";
    }
}

void ahp_HTTP_request_line_print(struct Ahp_HTTP_Request_Line rl)
{
    printf("%s /%.*s /HTTP/%s\n", ahp_HTTP_message_method_name(rl.method), (int)rl.request_target_len, rl.request_target, rl.HTTP_version);
}

#endif /* ALMOG_HTTP_PARSER_IMPLEMENTATION*/