#ifndef ALMOG_HTTP_PARSER_H_
#define ALMOG_HTTP_PARSER_H_

#include "Almog_String_Manipulation.h"
#include "Almog_Dynamic_Array.h"

#ifndef AHP_ASSERT
    #include <assert.h>
    #define AHP_ASSERT assert
#endif

enum Ahp_Return_Types {
    AHP_SUCCESS,
    AHP_FAIL,
};

enum Ahp_HTTP_Methods {
    AHP_GET,
    AHP_POST,
    AHP_NUM_OF_METHODS
};
    
struct Ahp_HTTP_Request_Line {
    char HTTP_version[4];
    char HTTP_method_word[5];
    const char *request_target;
    size_t request_target_len;
    enum Ahp_HTTP_Methods HTTP_method;
};

struct Ahp_HTTP_Field_Line {
    const char *field_name;
    size_t field_name_len;
    const char *field_value;
    size_t field_value_len;
};

struct Ahp_HTTP_Head {
    const char *content;
    size_t content_len;
    struct {
        size_t length;
        size_t capacity;
        struct Ahp_HTTP_Field_Line *elements;
    } field_lines;
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

enum Ahp_Return_Types   ahp_GET_message_parse(struct Ahp_HTTP_Message *msg);
enum Ahp_Return_Types   ahp_HTTP_field_line_parse(struct Ahp_HTTP_Message *msg, struct Ahp_HTTP_Field_Line *fl);
void                    ahp_HTTP_field_line_print(struct Ahp_HTTP_Field_Line fl);
void                    ahp_HTTP_message_cursor_advenc(struct Ahp_HTTP_Message *msg, size_t count);
enum Ahp_Return_Types   ahp_HTTP_message_cursor_check_ahead(struct Ahp_HTTP_Message *msg, const char * const str);
enum Ahp_Return_Types   ahp_HTTP_message_cursor_expect_and_advenc(struct Ahp_HTTP_Message *msg, char c);
enum Ahp_Return_Types   ahp_HTTP_message_parse(struct Ahp_HTTP_Message *msg);
char *                  ahp_HTTP_message_method_name(enum Ahp_HTTP_Methods method);
enum Ahp_Return_Types   ahp_HTTP_request_line_parse(struct Ahp_HTTP_Message *msg);
void                    ahp_HTTP_request_line_print(struct Ahp_HTTP_Request_Line rl);
bool                    ahp_is_valid_field_name(const char * const name, const size_t len);
bool                    ahp_is_valid_field_name_char(const char c);
enum Ahp_Return_Types   ahp_POST_message_parse(struct Ahp_HTTP_Message *msg);

#endif /* ALMOG_HTTP_PARSER_H_*/

#ifdef ALMOG_HTTP_PARSER_IMPLEMENTATION
#undef ALMOG_HTTP_PARSER_IMPLEMENTATION

enum Ahp_Return_Types ahp_GET_message_parse(struct Ahp_HTTP_Message *msg)
{
    enum Ahp_Return_Types rt;

    msg->HTTP_head.content = &(msg->content[msg->cursor]);
    while (AHP_SUCCESS != ahp_HTTP_message_cursor_check_ahead(msg, "\r\n")) {
        struct Ahp_HTTP_Field_Line fl = {0};
        rt = ahp_HTTP_field_line_parse(msg, &fl);
        if (AHP_SUCCESS != rt) {
            asm_dprintERROR("failed to parse field line. Field line parser returned with '%d'", rt);
            return AHP_FAIL;
        }
        ada_appand(struct Ahp_HTTP_Field_Line, msg->HTTP_head.field_lines, fl);
        msg->HTTP_head.content_len = &(msg->content[msg->cursor]) - msg->HTTP_head.content;
    }
    if (ahp_HTTP_message_cursor_expect_and_advenc(msg, '\r')) return AHP_FAIL; /* skip '\r' */
    if (ahp_HTTP_message_cursor_expect_and_advenc(msg, '\n')) return AHP_FAIL; /* skip '\n' */

    #if 1
    msg->HTTP_body.content = &(msg->content[msg->cursor]);
    msg->HTTP_body.content_len = 0;
    #else
    if (msg->cursor >= msg->content_len) {
        msg->HTTP_body.content_len = 0;
    } else {
        msg->HTTP_body.content_len = msg->content_len - msg->cursor;
    }
    #endif

    return AHP_SUCCESS;
}

enum Ahp_Return_Types ahp_HTTP_field_line_parse(struct Ahp_HTTP_Message *msg, struct Ahp_HTTP_Field_Line *fl)
{
    char current_word[ASM_MAX_LEN] = {0xFe};

    fl->field_name = &(msg->content[msg->cursor]);
    msg->cursor += asm_get_next_token_from_str(current_word, &(msg->content[msg->cursor]), ':');
    if (asm_isspace(current_word[asm_length(current_word) - 1])) { /* there should be no space between the filed name and the colon */
        asm_dprintERROR("%s", "invalid field line. detected space between field name and colon");
        return AHP_FAIL;
    }
    fl->field_name_len = &(msg->content[msg->cursor]) - fl->field_name;
    if (!ahp_is_valid_field_name(fl->field_name, fl->field_name_len)) {
        asm_dprintERROR("invalid field name '%.*s'", (int)fl->field_name_len, fl->field_name);
        return AHP_FAIL;
    }
    if (ahp_HTTP_message_cursor_expect_and_advenc(msg, ':')) return AHP_FAIL; /* skip ':' */

    fl->field_value = &(msg->content[msg->cursor]);
    msg->cursor += asm_get_next_token_from_str(current_word, &(msg->content[msg->cursor]), '\r');
    fl->field_value_len = &(msg->content[msg->cursor]) - fl->field_value;
    if (ahp_HTTP_message_cursor_expect_and_advenc(msg, '\r')) return AHP_FAIL; /* skip '\r' */
    if (ahp_HTTP_message_cursor_expect_and_advenc(msg, '\n')) return AHP_FAIL; /* skip '\n' */

    return AHP_SUCCESS;
}

void ahp_HTTP_field_line_print(struct Ahp_HTTP_Field_Line fl)
{
    printf("%.*s:%.*s\n", (int)fl.field_name_len, fl.field_name, (int)fl.field_value_len, fl.field_value);
}


void ahp_HTTP_message_cursor_advenc(struct Ahp_HTTP_Message *msg, size_t count)
{
    AHP_ASSERT(msg->cursor < msg->content_len);

    while (count--) {
        msg->cursor++;
    }
}

enum Ahp_Return_Types ahp_HTTP_message_cursor_check_ahead(struct Ahp_HTTP_Message *msg, const char * const str)
{
    size_t str_len = asm_length(str);
    if (msg->cursor + str_len >= msg->content_len) {
        return AHP_FAIL;
    }
    for (size_t i = 0; i < str_len; i++) {
        if (msg->content[msg->cursor + i] != str[i]) {
            return AHP_FAIL;
        }
    }

    return AHP_SUCCESS;
}

enum Ahp_Return_Types ahp_HTTP_message_cursor_expect_and_advenc(struct Ahp_HTTP_Message *msg, char c)
{
    if (msg->content[msg->cursor] != c) {
        asm_dprintERROR("expected '%c' of request target, but got '%c'", c, msg->content[msg->cursor]);
        return AHP_FAIL;
    }
    ahp_HTTP_message_cursor_advenc(msg, 1); /* skip 'c' */
    return AHP_SUCCESS;
}

enum Ahp_Return_Types ahp_HTTP_message_parse(struct Ahp_HTTP_Message *msg)
{
    msg->cursor = 0;
    ada_init_array(struct Ahp_HTTP_Field_Line, msg->HTTP_head.field_lines);
    enum Ahp_Return_Types rt;

    rt = ahp_HTTP_request_line_parse(msg);
    if (rt != AHP_SUCCESS) {
        asm_dprintERROR("unable to parse request line. request line parser returned with: %d", rt);
        return AHP_FAIL;
    }

    if (asm_strncmp(msg->HTTP_request_line.HTTP_method_word, "GET", 0)) {
        msg->HTTP_request_line.HTTP_method = AHP_GET;
        rt = ahp_GET_message_parse(msg);
        if (rt != AHP_SUCCESS) {
            asm_dprintERROR("unable to parse GET message. GET parser returned with: %d", rt);
            return AHP_FAIL;
        }
    } else if (asm_strncmp(msg->HTTP_request_line.HTTP_method_word, "POST", 0)) {
        msg->HTTP_request_line.HTTP_method = AHP_POST;
        rt = ahp_POST_message_parse(msg);
        if (rt != AHP_SUCCESS) {
            asm_dprintERROR("unable to parse POST message. POST parser returned with: %d", rt);
            return AHP_FAIL;
        }
    } else {
        asm_dprintERROR("unknown message method '%s'", msg->HTTP_request_line.HTTP_method_word);
        return AHP_FAIL;
    }

    return AHP_SUCCESS;
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

enum Ahp_Return_Types ahp_HTTP_request_line_parse(struct Ahp_HTTP_Message *msg)
{
    char current_word[ASM_MAX_LEN] = {0xFe};

    /* parse method */
    msg->cursor += asm_get_next_token_from_str(current_word, &(msg->content[msg->cursor]), ' ');
    asm_strncpy(msg->HTTP_request_line.HTTP_method_word, current_word, asm_length(current_word));
    if (ahp_HTTP_message_cursor_expect_and_advenc(msg, ' ')) return AHP_FAIL; /* skip ' ' */

    /* parse request target */
    if (ahp_HTTP_message_cursor_expect_and_advenc(msg, '/')) return AHP_FAIL; /* skip '/' */
    msg->HTTP_request_line.request_target = &(msg->content[msg->cursor]);
    if (msg->content[msg->cursor] == ' ') {
        msg->HTTP_request_line.request_target_len = 0;
    } else {
        enum Ahp_Return_Types rt = asm_get_next_token_from_str(current_word, &(msg->content[msg->cursor]), ' ');
        ahp_HTTP_message_cursor_advenc(msg, rt);
        msg->HTTP_request_line.request_target_len = &(msg->content[msg->cursor]) - msg->HTTP_request_line.request_target;
    }
    if (ahp_HTTP_message_cursor_expect_and_advenc(msg, ' ')) return AHP_FAIL; /* skip ' ' */

    /* parse HTTP version */
    msg->cursor += asm_get_next_token_from_str(current_word, &(msg->content[msg->cursor]), '/');
    if (!asm_strncmp(current_word, "HTTP", 0)) {
        asm_dprintERROR("expected 'HTTP' befor HTTP-version but got: '%s'", current_word);
        return AHP_FAIL;
    }
    if (ahp_HTTP_message_cursor_expect_and_advenc(msg, '/')) return AHP_FAIL; /* skip '/' */
    msg->cursor += asm_get_next_token_from_str(current_word, &(msg->content[msg->cursor]), '\r');
    if (!asm_strncmp(current_word, "1.1", 0)) {
        asm_dprintERROR("unsupported HTTP-version '%s'. Supports only version 1.1", current_word);
        return AHP_FAIL;
    }
    asm_strncpy(msg->HTTP_request_line.HTTP_version, current_word, 4);
    if (ahp_HTTP_message_cursor_expect_and_advenc(msg, '\r')) return AHP_FAIL; /* skip '\r' */
    if (ahp_HTTP_message_cursor_expect_and_advenc(msg, '\n')) return AHP_FAIL; /* skip '\n' */

    // asm_dprintSTRING(&(msg->content[msg->cursor]));

    return AHP_SUCCESS;
}

void ahp_HTTP_request_line_print(struct Ahp_HTTP_Request_Line rl)
{
    printf("%s /%.*s HTTP/%s\n", ahp_HTTP_message_method_name(rl.HTTP_method), (int)rl.request_target_len, rl.request_target, rl.HTTP_version);
}

bool ahp_is_valid_field_name(const char * const name, const size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (!ahp_is_valid_field_name_char(name[i])) {
            return false;
        }
    }

    return true;
}

bool ahp_is_valid_field_name_char(const char c)
{
    if (asm_isalnum(c)) return true;
    switch (c) {
    case '!': case '#': case '$': case '%': case '&': case '\'':
    case '*': case '+': case '-': case '.': case '^': case '_':
    case '`': case '|': case '~':
        return true;
    default:
        return false;
    }
}

enum Ahp_Return_Types ahp_POST_message_parse(struct Ahp_HTTP_Message *msg)
{
    (void)msg;

    return AHP_FAIL;
}

#endif /* ALMOG_HTTP_PARSER_IMPLEMENTATION*/