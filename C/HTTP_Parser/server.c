#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>

#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#define ALMOG_HTTP_PARSER_IMPLEMENTATION
#include "Almog_HTTP_Parser.h"

void parse_body(const char *body, int *n1, int *n2) 
{
    char current_word[ASM_MAX_LEN];
    char *str = (char *)body;
    str += asm_get_next_token_from_str(current_word, str, '=');
    str++;
    *n1 = asm_str2int(str, &str, 10);
    str += asm_get_next_token_from_str(current_word, str, '=');
    str++;
    *n2 = asm_str2int(str, &str, 10);
}

struct String {
    size_t length;
    size_t capacity;
    char *elements;
};

struct TCP_Server {
    WSADATA winsock_data;
    SOCKET TCP_client_socket;
    SOCKET TCP_server_socket;
    ADDRESS_FAMILY family;
    struct sockaddr_in TCP_server_addres;
    struct sockaddr_in TCP_client_addres;
    int TCP_client_addres_len;
    char send_buffer[ASM_MAX_LEN];
    size_t  send_buffer_max_len;
    char recv_buffer[ASM_MAX_LEN];
    size_t  recv_buffer_max_len;
    char server_ip[ASM_MAX_LEN];
    size_t  server_ip_len;
    char client_ip[ASM_MAX_LEN];
    size_t  client_ip_max_len;
    u_short port;
    int backlog;
};

struct TCP_Server setup_TCP_server(ADDRESS_FAMILY family, char ip[], u_short port, int backlog)
{
    struct TCP_Server server = {
        .TCP_client_addres_len = sizeof(server.TCP_client_addres),
        .send_buffer_max_len   = sizeof(server.send_buffer) / sizeof(server.send_buffer[0]),
        .recv_buffer_max_len   = sizeof(server.recv_buffer) / sizeof(server.recv_buffer[0]),
        .family = family,
        .port = port,
        .backlog = backlog,
        .client_ip_max_len = ASM_MAX_LEN,
    };
    asm_strncpy(server.server_ip, ip, asm_length(ip));
    server.server_ip_len = asm_length(ip);

    if (WSAStartup(MAKEWORD(2,2), &server.winsock_data)) {
        fprintf(stderr, "WSAStartup failed\n");
        exit(1);
    }

    server.TCP_server_addres.sin_family = server.family;
    if (InetPtonA(server.family, server.server_ip, &server.TCP_server_addres.sin_addr) != 1) {
        fprintf(stderr, "InetPtonA failed\n");
        exit(1);
    }
    server.TCP_server_addres.sin_port = htons(server.port);
    server.TCP_server_socket = socket(server.family, SOCK_STREAM, IPPROTO_TCP);
    if (server.TCP_server_socket == INVALID_SOCKET) {
        fprintf(stderr, "TCP server socket creation failed. %d\n", WSAGetLastError());
        exit(1);
    }
    printf("TCP server socket creation success\n");

    if (bind(server.TCP_server_socket, (SOCKADDR*)&server.TCP_server_addres, sizeof(server.TCP_server_addres)) == SOCKET_ERROR) {
        fprintf(stderr, "Binding failed. %d\n", WSAGetLastError());
        exit(1);
    }
    printf("Binding success\n");

    if (listen(server.TCP_server_socket, server.backlog) == SOCKET_ERROR) {
        fprintf(stderr, "Listening failed. %d\n", WSAGetLastError());
        exit(1);
    }
    printf("Listening success\n");

    return server;
}

u_short client_ip_get(struct TCP_Server *server)
{
    InetNtopA(server->family, &server->TCP_client_addres.sin_addr, server->client_ip, server->client_ip_max_len);
    return ntohs(server->TCP_client_addres.sin_port);
}

bool CRLF_at_the_end(char *str, size_t len)
{
    return str[len-1] == '\n' && str[len-2] == '\r' && str[len-3] == '\n' && str[len-4] == '\r';
}

void close_server(struct TCP_Server *server)
{
    if (closesocket(server->TCP_server_socket) == SOCKET_ERROR) {
        fprintf(stderr, "Closing sever socket failed. %d\n", WSAGetLastError());
        exit(1);
    }
    printf("Closing success\n");

    if (WSACleanup() == SOCKET_ERROR) {
        fprintf(stderr, "Cleanup failed. %d\n", WSAGetLastError());
        exit(1);
    }
    printf("Cleanup success\n");
}

const char default_HTTP_OK_message[] = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Length: 18\r\n"
    "Connection: close\r\n"
    "\r\n"
    "Hello from server!";

int main(void)
{
    
    printf("----------TCP SERVER----------\n\n");

    struct TCP_Server server = setup_TCP_server(AF_INET, "0.0.0.0", 42069, 2);

    for (;;) {
        struct Ahp_HTTP_Request msg = {.content = NULL};
        struct String message_content;
        ada_init_array(char, message_content);
        bool read_head = false;
        int body_start_in_recv = -1;

        server.TCP_client_socket = accept(server.TCP_server_socket, (SOCKADDR *)&server.TCP_client_addres, &server.TCP_client_addres_len);
        if (server.TCP_client_socket == INVALID_SOCKET) {
            fprintf(stderr, "accept failed. %d\n", WSAGetLastError());
            free((void *)msg.HTTP_header.field_lines.elements);
            free((void *)msg.content);
            break; // or break if you want to stop the server
        }
        u_short client_port = client_ip_get(&server);
        printf("Client connected from %s:%u\n", server.client_ip, client_port);
        DWORD timeout_ms = 2000;
        setsockopt( server.TCP_client_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout_ms, sizeof(timeout_ms));

        /* ------------------------------------------------------------------------------- */

        int n = recv(server.TCP_client_socket, server.recv_buffer, (int)server.recv_buffer_max_len - 1, 0);
        if (n == 0) {
            closesocket(server.TCP_client_socket);
        } else 
        if (n < 0) {
            fprintf(stderr, "recv failed. %d\n", WSAGetLastError());
            closesocket(server.TCP_client_socket);
        }
        for (;n > 0;) {
            body_start_in_recv = -1;
            for (int i = 0; i < n; i++) {
                ada_appand(char, message_content, server.recv_buffer[i]);
                /* checking if finished getting the head */
                if (message_content.length < 4) {
                    continue;
                } else if (CRLF_at_the_end(message_content.elements, message_content.length)) {
                    read_head = true;
                    body_start_in_recv = i + 1; /* next byte in THIS recv buffer is body start */
                    break;
                }
            }
            if (read_head == false) {
                n = recv(server.TCP_client_socket, server.recv_buffer, (int)server.recv_buffer_max_len - 1, 0);
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
                    goto clean_client_and_break;
                }

                send(server.TCP_client_socket, default_HTTP_OK_message, (int)asm_length(default_HTTP_OK_message), 0);
                if (closesocket(server.TCP_client_socket) == SOCKET_ERROR) {
                    fprintf(stderr, "Closing client socket failed. %d\n", WSAGetLastError());
                    goto clean_server_and_exit;
                }

                ahp_HTTP_request_debug_print(&msg);

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
                ada_appand(char, message_content, server.recv_buffer[body_start_in_recv + (int)i]);
            }
            body_len -= take_now;

            /* Receive the remaining body bytes */
            while (body_len > 0) {
                int want = (int)asm_min(body_len, server.recv_buffer_max_len-1);

                n = recv(server.TCP_client_socket, server.recv_buffer, want, 0);
                if (n == 0) {
                    // client closed without sending a request
                    closesocket(server.TCP_client_socket);
                    break;
                }
                if (n < 0) {
                    fprintf(stderr, "recv failed. %d\n", WSAGetLastError());
                    closesocket(server.TCP_client_socket);
                    break;
                }
                if (n <= 0) {
                    fprintf(stderr, "Receiving body from client failed. %d\n", WSAGetLastError());
                    goto clean_client_and_break;
                }

                for (int i = 0; i < n; i++) {
                    ada_appand(char, message_content, server.recv_buffer[i]);
                }
                body_len -= (size_t)n;
            }

            /* Now safe to parse full message */
            msg.content = message_content.elements;
            msg.content_len = message_content.length;

            if (ahp_HTTP_request_parse(&msg) != AHP_SUCCESS) {
                asm_dprintERROR("%s", "Failed to parse HTTP message");
                goto clean_client_and_break;
            }

            char body_str[ASM_MAX_LEN];
            int n1, n2;
            parse_body(msg.HTTP_body.content, &n1, &n2);

            sprintf_s(body_str, ASM_MAX_LEN, 
                "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<style>"
                "body {"
                "    font-family: Gabriel;"
                "font-size: 30px;"
                "    margin: -1;"
                "    padding: 23px;"
                "    background: #181818;"
                "    color: #fff;"
                "}"
                "</style>"
                "</head>"
                "<body>%d + %d = %d</body>"
                "</html>", n1, n2, n1 + n2);
            
            sprintf_s(server.send_buffer, ASM_MAX_LEN,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %zu\r\n"
                "Connection: close\r\n"
                "\r\n"
                "%s"
                , asm_length(body_str), body_str);

            send(server.TCP_client_socket, server.send_buffer, (int)asm_length(server.send_buffer), 0);

            ahp_HTTP_request_debug_print(&msg);

            break;
        }

        clean_client_and_break:
            free((void *)msg.HTTP_header.field_lines.elements);
            free((void *)msg.content);
            if (closesocket(server.TCP_client_socket) == SOCKET_ERROR) {
                fprintf(stderr, "Closing client socket failed. %d\n", WSAGetLastError());
                goto clean_server_and_exit;
            }
    }

    clean_server_and_exit:
        close_server(&server);

    return 0;
}