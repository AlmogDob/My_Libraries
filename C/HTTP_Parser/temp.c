#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#define ALMOG_HTTP_PARSER_IMPLEMENTATION
#include "Almog_HTTP_Parser.h"


int main(void) 
{
    // char str[] = "GET /coffee HTTP/1.1\r\nHost: localhost:42069\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\n\r\nbody";
    char str[] = "POST /coffee HTTP/1.1\r\nHost: localhost:42069\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\nContent-Type: application/json\r\nContent-Length: 22\r\n\r\n{\"flavor\":\"dark mode\"}";
    struct Ahp_HTTP_Message msg;
    msg.content = str;
    msg.content_len = asm_length(str);

    if (ahp_HTTP_message_parse(&msg) != AHP_SUCCESS) {
        return 1;
    }

    ahp_HTTP_message_debug_print(&msg);


    free(msg.HTTP_head.field_lines.elements);

    return 0;
}


