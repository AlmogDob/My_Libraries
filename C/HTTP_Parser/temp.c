#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#define ALMOG_HTTP_PARSER_IMPLEMENTATION
#include "Almog_HTTP_Parser.h"


int main(void) 
{
    char str[] = "GET /coffee HTTP/1.1\r\nHost: localhost:42069\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\n\r\nbody";
    // char str[] = "POST /coffee HTTP/1.1\r\nHost: localhost:42069\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\nContent-Type: application/json\r\nContent-Length: 22\r\n\r\n{\"flavor\":\"dark mode\"}";
    struct Ahp_HTTP_Message msg;
    msg.content = str;

    if (ahp_HTTP_message_parse(&msg) != 0) {
        return 1;
    }

    printf("rl\t-> ");
    ahp_HTTP_request_line_print(msg.HTTP_request_line);
    for (size_t i = 0; i < msg.HTTP_head.field_lines.length; i++) {
        printf("fl%zu\t-> ", i);
        ahp_HTTP_field_line_print(msg.HTTP_head.field_lines.elements[i]);
    }
    printf("body\t-> %.*s\n", (int)msg.HTTP_body.content_len, msg.HTTP_body.content);




    free(msg.HTTP_head.field_lines.elements);

    return 0;
}


