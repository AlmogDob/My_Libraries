#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#define ALMOG_HTTP_PARSER_IMPLEMENTATION
#include "Almog_HTTP_Parser.h"


int main(void) 
{
    char str[] = "GET /coffee /HTTP/1.1\r\nHost: localhost:42069\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\n\r\n";
    struct Ahp_HTTP_Message msg;
    msg.content = str;

    if (ahp_HTTP_message_parse(&msg) != 0) {
        return 1;
    }

    ahp_HTTP_request_line_print(msg.HTTP_request_line);

    return 0;
}


