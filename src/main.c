#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "../include/interface.h"
#include "../include/socket_client.h"

#define MAXEVENT_FDS 2

void exit_errno(client_socket_t *connection_info);
void exit_server_error(client_socket_t *connection_info, char *message);
void connect_to_server(client_socket_t *connection_info, char *host);
void input_username(char *username);
void set_username(client_socket_t *connection_info, char *username);
void process_user_input();
void process_server_message();

int main (int argc, char **argv) {
    int debug = 0;
    char *port = NULL;
    char *host = NULL;
    int index;
    int c;
    int username[USERNAME_LEN];
    int quit = 0;
    client_socket_t *connection_info = malloc(sizeof(client_socket_t));

    if (argc != 2) {
          fprintf (stderr, "Usage -p <port> -h <hostname>\n", argv[0]);
          exit (EXIT_FAILURE);
    }

    opterr = 0;

    while ((c = getopt (argc, argv, "dp:h:")) != -1) {
        switch (c) {
            case 'd':
                debug = 1;
                break;
            case 'p':
                port = optarg;
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

    connect_to_server(connection_info, host);

    set_username(connection_info, username);

    fd_set event_fds, ready_fds;
    FD_ZERO(&event_fds);
    FD_SET(connection_info->socket_descriptor, &event_fds);
    FD_SET(STDIN_FILENO, &event_fds);
    flush(stdin);

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

void exit_server_error(client_socket_t *connection_info, char *message){
    fprintf (stderr, "SERVER ERROR: %c\n", message);
    close_connection(connection_info);
    free(connection_info);
    exit(EXIT_FAILURE);
}

void connect_to_server(client_socket_t *connection_info, char *host){
    connection_info->hostname = host;

    if (create_socket(connection_info) == -1){
        exit_errno(connection_info);
    }

    if (start_connection(connection_info)== -1){
        exit_errno(connection_info);
    }
    puts("Connected!");
}

void input_username(char *username){
        while(1){
        get_username(username, USERNAME_LEN);
        if (username[USERNAME_LEN - 1] != '\0'){
            puts("Username length exceeded please try again!");
        } else {
            break;
        }
    }
}

void set_username(client_socket_t *connection_info, char *username){
    message_t *set_username = malloc(sizeof(message_t));
    set_username->command = SET_USERNAME;
    set_username->message = username;

    if (send_message(&connection_info, set_username, sizeof(message_t)) == -1) {
        exit_errno(connection_info);
    }

    if (fetch_message(&connection_info, set_username, sizeof(message_t)) == -1) {
        exit_errno(connection_info);
    }

    if (set_username->command == ERROR){
        exit_server_error(connection_info, set_username->message);
    }

    free(set_username);
}

void process_user_input(client_socket_t *connection_info){

}

void process_server_message(client_socket_t *connection_info){
    
}