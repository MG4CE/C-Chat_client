#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "../include/interface.h"
#include "../include/socket_client.h"

#define MAXEVENT_FDS 2
#define MAX_MESSAGE_LEN 3950

//TODO add timeout function as sperate thread

void exit_errno(client_socket_t *connection_info);
void exit_error(client_socket_t *connection_info, char *message);
void connect_to_server(client_socket_t *connection_info, char *host, int port);
void input_username(char *username);
void set_username(client_socket_t *connection_info, char *username);
void process_user_command(client_socket_t *connection_info, char *line);
void process_server_message(client_socket_t *connection_info);
void process_user_input(client_socket_t *connection_info);

int main (int argc, char **argv) {
    char *host = NULL;
    int port = 0;
    int index;
    int c;
    char username[USERNAME_LEN];
    client_socket_t *connection_info = malloc(sizeof(client_socket_t));

    opterr = 0;

    while ((c = getopt (argc, argv, "p:h:")) != -1) {
        switch (c) {
            case 'd':
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'h':
                host = optarg;
                break;
            case '?':
                if (optopt == 'p' || optopt == 'h') {
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                } else if (isprint (optopt)) {
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                }
                return 1;
            default:
                abort ();
        }
    } 

    for (index = optind; index < argc; index++) {
        printf ("Non-option argument %s\n", argv[index]);
    }

    input_username(username);

    connect_to_server(connection_info, host, port);

    set_username(connection_info, username);

    fd_set event_fds, ready_fds;
    FD_ZERO(&event_fds);
    FD_SET(connection_info->socket_descriptor, &event_fds);
    FD_SET(STDIN_FILENO, &event_fds);
    fflush(stdin);

    while (1){
        ready_fds = event_fds;

        if (select(MAXEVENT_FDS, &ready_fds, NULL, NULL, NULL) < 0) {
            close_connection(connection_info);
            exit_errno(connection_info);
        }

        if (FD_ISSET(connection_info->socket_descriptor, &ready_fds)) {
            process_server_message(connection_info);
        }

        if (FD_ISSET(STDIN_FILENO, &ready_fds)) {
            process_user_input(connection_info);
        }
    }

    close_connection(connection_info);
    free(connection_info);
    return EXIT_SUCCESS;
}

void exit_errno(client_socket_t *connection_info){
    fprintf (stderr, "ERROR: %c\n", errno);
    close_connection(connection_info);
    free(connection_info);
    exit(EXIT_FAILURE);
}

void exit_error(client_socket_t *connection_info, char *message) {
    fprintf (stderr, "ERROR: %s\n", message);
    close_connection(connection_info);
    free(connection_info);
    exit(EXIT_FAILURE);
}

void connect_to_server(client_socket_t *connection_info, char *host, int port){
    connection_info->hostname = host;
    connection_info->port = port;

    if (create_socket(connection_info) == -1){
        exit_errno(connection_info);
    }

    if (start_connection(connection_info)== -1){
        exit_error(connection_info, "Unable to connect to server!");
    }
    puts("Connected!");
}

void input_username(char *username){
        while(1){
        get_username(username, INPUT_USER_LEN);
        if (strlen(username) > INPUT_USER_LEN){
            puts("Username length exceeded please try again!");
        } else {
            break;
        }
    }
}

void set_username(client_socket_t *connection_info, char *username){
    message_t *set_username = malloc(sizeof(message_t));
    set_username->command = SET_USERNAME;
    strcpy(set_username->message, username);

    if (send_message(connection_info, set_username, sizeof(message_t)) == -1) {
        exit_errno(connection_info);
    }

    if (fetch_message(connection_info, set_username, sizeof(message_t)) == -1) {
        exit_errno(connection_info);
    }

    if (set_username->command == ERROR){
        exit_error(connection_info, set_username->message);
    }

    free(set_username);
}

void process_user_input(client_socket_t *connection_info) {
    char str[MAX_MESSAGE_LEN];
    fgets(str, MAX_MESSAGE_LEN, stdin); //check return?

    if (strlen(str) <= 1) {
        return;
    }

    if (str[0] == '/'){
        process_user_command(connection_info, str);
        return;
    }

    message_t public_message;
    public_message.command = SEND_PUBLIC;
    strcpy(public_message.message, str);

    if (send_message(connection_info, &public_message, sizeof(message_t))){
        exit_errno(connection_info);
    }
}

void process_user_command(client_socket_t *connection_info, char *line) {
    if (strcmp(line, "/help")) {
        print_help();
        return;
    } 

    message_t *message = malloc(sizeof(message_t));

    if (strcmp(line, "/list")){
        message->command = GET_USERS;
        if (send_message(connection_info, message, sizeof(message_t))){
            exit_errno(connection_info);
        }
    } else if (strstr(line, "/private ")){
        char selected_username[USERNAME_LEN];
        parse_username(line, selected_username);
        message->command = SEND_PRIVATE;
        strcpy(message->username, selected_username);
        if (send_message(connection_info, message, sizeof(message_t))){
            exit_errno(connection_info);
        }
    } else if (strcmp(line, "/exit")){
        message->command = DISCONNECT;
        if (send_message(connection_info, message, sizeof(message_t))){
            exit_errno(connection_info);
        }
    } else {
        puts("Unknown command, use /help for list of commands");
    }

    free(message);
}

void process_server_message(client_socket_t *connection_info) {
    message_t *message = malloc(sizeof(message_t));

    if (fetch_message(connection_info, message, sizeof(message_t))){
        exit_errno(connection_info);
    }

    if (message->command == SEND_PUBLIC) {
        printf("%s: %s", message->username, message->message);
    } else if (message->command == SEND_PRIVATE) {
        printf("%s -> You: %s", message->username, message->message);
    } else if (message->command == GET_USERS) {
        printf("Connected Users: %s", message->message);
    } else if (message->command == ERROR) {
        printf("ERROR: %s", message->message);
    } else {
        puts("Unknown command received, message ignored!");
    }

    free(message);
}
