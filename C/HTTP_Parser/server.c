#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#define ALMOG_HTTP_PARSER_IMPLEMENTATION
#include "Almog_HTTP_Parser.h"

struct String {
    size_t length;
    size_t capacity;
    char *elements;
};

int main(void)
{
    struct Ahp_HTTP_Message msg;
    struct String message_content;
    ada_init_array(char, message_content);
    bool read_head = false;
    
    printf("----------TCP SERVER----------\n\n");

    WSADATA winsock_data;
    SOCKET TCP_server_socket;
    struct sockaddr_in TCP_server_addres;
    struct sockaddr_in TCP_client_addres;
    int TCP_client_addres_len = sizeof(TCP_client_addres);
    char sender_buffer[512] = "Hello from server!";
    int sender_buffer_len = sizeof(sender_buffer) / sizeof(sender_buffer[0]);
    char receive_buffer[512] = {0};
    int receive_buffer_len = sizeof(receive_buffer) / sizeof(receive_buffer[0]);

    if (WSAStartup(MAKEWORD(2,2), &winsock_data)) {
        fprintf(stderr, "WSAStartup failed\n");
        return 1;
    }

    TCP_server_addres.sin_family = AF_INET;
    if (InetPtonA(AF_INET, "127.0.0.1", &TCP_server_addres.sin_addr) != 1) {
        fprintf(stderr, "InetPtonA failed\n");
        return 1;
    }
    TCP_server_addres.sin_port = htons(42069);
    TCP_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (TCP_server_socket == INVALID_SOCKET) {
        fprintf(stderr, "TCP server socket creation failed. %d\n", WSAGetLastError());
        return 1;
    }
    printf("TCP server socket creation success\n");

    if (bind(TCP_server_socket, (SOCKADDR*)&TCP_server_addres, sizeof(TCP_server_addres)) == SOCKET_ERROR) {
        fprintf(stderr, "Binding failed. %d\n", WSAGetLastError());
        return 1;
    }
    printf("Binding success\n");

    if (listen(TCP_server_socket, 2) == SOCKET_ERROR) {
        fprintf(stderr, "Listening failed. %d\n", WSAGetLastError());
        return 1;
    }
    printf("Listening success\n");

    SOCKET client = accept( TCP_server_socket, (SOCKADDR *)&TCP_client_addres, &TCP_client_addres_len);
    printf("Waiting for response form the client...\n");

    if (client == INVALID_SOCKET) {
        fprintf(stderr, "accept failed. %d\n", WSAGetLastError());
        return 1; // or break if you want to stop the server
    }


    printf("Received!\n");
    // printf("------------------------------\n");
    int n = recv(client, receive_buffer, receive_buffer_len - 1, 0);
    while (n > 0) {
        for (int i = 0; i < n; i++) {
            ada_appand(char, message_content, receive_buffer[i]);
            /* checking if finished getting the head */
            if (message_content.length < 4) {
                continue;
            } else {
                if (message_content.elements[message_content.length-1] == '\n' &&
                    message_content.elements[message_content.length-2] == '\r' &&
                    message_content.elements[message_content.length-3] == '\n' &&
                    message_content.elements[message_content.length-4] == '\r') {
                        read_head = true;
                        break;
                    }
            }
        }

        if (read_head == false) {
            n = recv(client, receive_buffer, receive_buffer_len - 1, 0);
        } else {
            /* checking if there is a body */
            int body_len = (int)ahp_HTTP_body_len_get_from_head_no_parsing(message_content.elements, message_content.length);

            if (body_len == 0) {
                /* no body */
                /* parsing request line and head */
                msg.content = message_content.elements;
                msg.content_len = message_content.length;
                enum Ahp_Return_Types rt;
                
                rt = ahp_HTTP_request_line_and_head_parse(&msg);
                if (rt != AHP_SUCCESS) {
                    asm_dprintERROR("Unable to parse request line and head. Request line and head parser returned with: %d", rt);
                    return 1;
                }

                if (msg.HTTP_request_line.HTTP_method == AHP_GET) {
                    /* TODO: send OK and break */
                    send(client, sender_buffer, sender_buffer_len, 0);
                    break;
                } else if (msg.HTTP_request_line.HTTP_method == AHP_POST) {
                    /* TODO: send NOT OK and break */
                    send(client, sender_buffer, sender_buffer_len, 0);
                    break;
                }
            } else {
                /* reading only the body length regardless of the buffer size */
                if (n - (message_content.length % receive_buffer_len) > body_len) {
                    for (int i = (message_content.length % receive_buffer_len); i < n; i++) {
                        ada_appand(char, message_content, receive_buffer[i]);
                    }
                } else {
                    for (int i = (message_content.length % receive_buffer_len); i < n; i++) {
                        ada_appand(char, message_content, receive_buffer[i]);
                    }
                    body_len -= (message_content.length % receive_buffer_len);

                    n = recv(client, receive_buffer, (int)fmin((float)body_len, (float)receive_buffer_len)-1, 0);
                    while (body_len > 0) {
                        for (int i = 0; i < n; i++) {
                            ada_appand(char, message_content, receive_buffer[i]);
                        }
                        body_len -= n;
                        n = recv(client, receive_buffer, (int)fmin((float)body_len, (float)receive_buffer_len)-1, 0);
                    }
                }

                /* now it should be safe to parse the message */
                msg.content = message_content.elements;
                msg.content_len = message_content.length;
                enum Ahp_Return_Types rt;

                rt = ahp_HTTP_message_parse(&msg);
                if (rt != AHP_SUCCESS) {
                    asm_dprintERROR("Unable to parse message. Message parser returned with: %d", rt);
                    return 1;
                }

                /* TODO: send OK and break */
                send(client, sender_buffer, sender_buffer_len, 0);
                break;
            }
        }
    }
    if (n < 0) {
        fprintf(stderr, "Receiving form client failed. %d\n", WSAGetLastError());
        return 1;
    }

    if (closesocket(client) == SOCKET_ERROR) {
        fprintf(stderr, "Closing client socket failed. %d\n", WSAGetLastError());
        return 1;
    }

    if (closesocket(TCP_server_socket) == SOCKET_ERROR) {
        fprintf(stderr, "Closing sever socket failed. %d\n", WSAGetLastError());
        return 1;
    }
    printf("Closing success\n");

    if (WSACleanup() == SOCKET_ERROR) {
        fprintf(stderr, "Cleanup failed. %d\n", WSAGetLastError());
        return 1;
    }
    printf("Cleanup success\n");


    asm_print_many_times("-", 40);

    // msg.content = message_content.elements;
    // msg.content_len = message_content.length;
    // (void)message_content;


    // if (ahp_HTTP_message_parse(&msg) != 0) {
    //     free((void *)msg.HTTP_head.field_lines.elements);
    //     free((void *)msg.content);
    //     return 1;
    // }

    printf("rl\t-> ");
    ahp_HTTP_request_line_print(msg.HTTP_request_line);
    for (size_t i = 0; i < msg.HTTP_head.field_lines.length; i++) {
        printf("fl%zu\t-> ", i);
        ahp_HTTP_field_line_print(msg.HTTP_head.field_lines.elements[i]);
    }
    printf("body\t-> %.*s\n", (int)msg.HTTP_body.content_len, msg.HTTP_body.content);




    free((void *)msg.HTTP_head.field_lines.elements);
    free((void *)msg.content);

    return 0;
}