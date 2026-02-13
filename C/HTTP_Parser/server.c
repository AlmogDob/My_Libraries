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
    struct String message_content;
    ada_init_array(char, message_content);
    
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

    // Handle client (blocking)
    send(client, sender_buffer, sender_buffer_len, 0);

    printf("Received!\n");
    // printf("------------------------------\n");
    int n = recv(client, receive_buffer, receive_buffer_len - 1, 0);
    while (n > 0) {
        receive_buffer[n] = '\0';
        // printf("%s\n", receive_buffer);
        for (int i = 0; i < n; i++) {
            ada_appand(char, message_content, receive_buffer[i]);
        }
        n = recv(client, receive_buffer, receive_buffer_len - 1, 0);
    }
    ada_appand(char, message_content, '\0');
    // printf("------------------------------\n");
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

    printf("the message:\n%.*s\n", (int)message_content.length, message_content.elements);

    struct Ahp_HTTP_Message msg;
    msg.content = message_content.elements;
    msg.content_len = message_content.length;
    (void)message_content;


    if (ahp_HTTP_message_parse(&msg) != 0) {
        free((void *)msg.HTTP_head.field_lines.elements);
        free((void *)msg.content);
        return 1;
    }

    ahp_HTTP_request_line_print(msg.HTTP_request_line);
    for (size_t i = 0; i < msg.HTTP_head.field_lines.length; i++) {
        ahp_HTTP_field_line_print(msg.HTTP_head.field_lines.elements[i]);
    }
    printf("%.*s\n", (int)msg.HTTP_body.content_len, msg.HTTP_body.content);




    free((void *)msg.HTTP_head.field_lines.elements);
    free((void *)msg.content);

    return 0;
}