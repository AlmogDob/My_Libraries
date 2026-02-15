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

    for (;;) {
    struct Ahp_HTTP_Message msg = {.content = NULL};
    struct String message_content;
    ada_init_array(char, message_content);
    bool read_head = false;
    int body_start_in_recv = -1;

    SOCKET client = accept( TCP_server_socket, (SOCKADDR *)&TCP_client_addres, &TCP_client_addres_len);
    if (client == INVALID_SOCKET) {
        fprintf(stderr, "accept failed. %d\n", WSAGetLastError());
        return 1; // or break if you want to stop the server
    }
    printf("Connection accepted\n");
    DWORD timeout_ms = 2000;
    setsockopt(
        client,
        SOL_SOCKET,
        SO_RCVTIMEO,
        (const char *)&timeout_ms,
        sizeof(timeout_ms)
    );

    /* ------------------------------------------------------------------------------- */

    int n = recv(client, receive_buffer, receive_buffer_len - 1, 0);
    if (n == 0) {
        // client closed without sending a request
        closesocket(client);
    } else 
    if (n < 0) {
        fprintf(stderr, "recv failed. %d\n", WSAGetLastError());
        closesocket(client);
    }
    for (;n > 0;) {
        body_start_in_recv = -1;
        for (int i = 0; i < n; i++) {
            ada_appand(char, message_content, receive_buffer[i]);
            /* checking if finished getting the head */
            if (message_content.length < 4) {
                continue;
            } else if (message_content.elements[message_content.length-1] == '\n' &&
                       message_content.elements[message_content.length-2] == '\r' &&
                       message_content.elements[message_content.length-3] == '\n' &&
                       message_content.elements[message_content.length-4] == '\r') {
                read_head = true;
                body_start_in_recv = i + 1; /* next byte in THIS recv buffer is body start */
                break;
            }
        }
        if (read_head == false) {
            n = recv(client, receive_buffer, receive_buffer_len - 1, 0);
            continue;
        }

        ada_appand(char, message_content, '\0');

        size_t body_len = ahp_HTTP_body_len_get_from_head_no_parsing(message_content.elements, message_content.length);

        message_content.length--;

        if (body_len == 0) {
            /* No body: parse only request-line+head */
            msg.content = message_content.elements;
            msg.content_len = message_content.length;

            if (ahp_HTTP_request_line_and_head_parse(&msg) != AHP_SUCCESS) {
                asm_dprintERROR("%s", "Failed to parse request-line+head");
                return 1;
            }

            const char *resp =
                "HTTP/1.1 200 OK\r\n"
                "Content-Length: 18\r\n"
                "Connection: close\r\n"
                "\r\n"
                "Hello from server!";
            send(client, resp, (int)asm_length(resp), 0);
            if (closesocket(client) == SOCKET_ERROR) {
                fprintf(stderr, "Closing client socket failed. %d\n", WSAGetLastError());
                return 1;
            }

            ahp_HTTP_message_debug_print(&msg);

            break;
        }

        /* Append ONLY Content-Length bytes from this recv buffer. */
        if (body_start_in_recv < 0) {
            body_start_in_recv = n; /* should not happen, but safe */
        }

        size_t already_in_recv = 0;
        if (body_start_in_recv < n) {
            already_in_recv = (size_t)(n - body_start_in_recv);
        }

        size_t take_now = already_in_recv > body_len ? body_len : already_in_recv;
        for (size_t i = 0; i < take_now; i++) {
            ada_appand(char, message_content, receive_buffer[body_start_in_recv + (int)i]);
        }
        body_len -= take_now;

        /* Receive the remaining body bytes */
        while (body_len > 0) {
            int want = (int)body_len;
            if (want > receive_buffer_len - 1) {
                want = receive_buffer_len - 1;
            }

            n = recv(client, receive_buffer, want, 0);
            if (n == 0) {
                // client closed without sending a request
                closesocket(client);
                break;
            }
            if (n < 0) {
                fprintf(stderr, "recv failed. %d\n", WSAGetLastError());
                closesocket(client);
                break;
            }
            if (n <= 0) {
                fprintf(stderr, "Receiving body from client failed. %d\n", WSAGetLastError());
                return 1;
            }

            for (int i = 0; i < n; i++) {
                ada_appand(char, message_content, receive_buffer[i]);
            }
            body_len -= (size_t)n;
        }

        /* Now safe to parse full message */
        msg.content = message_content.elements;
        msg.content_len = message_content.length;

        if (ahp_HTTP_message_parse(&msg) != AHP_SUCCESS) {
            asm_dprintERROR("%s", "Failed to parse HTTP message");
            return 1;
        }

        const char *resp =
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 18\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Hello from server!";
        send(client, resp, (int)asm_length(resp), 0);
        if (closesocket(client) == SOCKET_ERROR) {
            fprintf(stderr, "Closing client socket failed. %d\n", WSAGetLastError());
            return 1;
        }

        ahp_HTTP_message_debug_print(&msg);

        break;
    }


    free((void *)msg.HTTP_head.field_lines.elements);
    free((void *)msg.content);
    /* ------------------------------------------------------------------------------- */

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





    return 0;
}