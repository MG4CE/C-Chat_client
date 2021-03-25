#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H
#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netdb.h>

#define IPV4 AF_INET
#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM
#define IP_PROTOCOL 0

typedef enum {
    SET_USERNAME,
    GET_USERS,
    SEND_PUBLIC,
    SEND_PRIVATE,
    DISCONNECT,
    ERROR,
}request;

typedef struct {
    int socket_descriptor;
    char *hostname;
    int port;
    char server_ip[100];
}client_socket_t;

typedef struct message {
    request command;
    char * message;
    char * selected_user;
}message_t;

int create_socket(client_socket_t *server_info);
int start_connection(client_socket_t *server_info);
int send_message(client_socket_t *server_info, message_t *message, size_t message_size);
int fetch_message(client_socket_t *server_info, message_t *message, size_t message_size);
int close_connection(client_socket_t *server_info);
int resolve_hostname(char *hostname, char *ip);
#endif
