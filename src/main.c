//single threaded client:
//parse args (ip/hostname, port)
//connect
//input username
//select user to chat with
//chat
//allow for chat commands using /<commands>
//rejoin avaliable to chat list until disconnect
//clean up on close/crash

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "../include/interface.h"
#include "../lib/socket_client/socket_client.h"

int main (int argc, char **argv) {
    int debug = 0;
    char *port = NULL;
    char *host = NULL;
    int index;
    int c;
    int username[22];
    client_socket_t connection_info;

    //---------------------------- Parse Args ----------------------------
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

    while(1){
        get_username(username, USERNAME_LEN);
        if (username[USERNAME_LEN - 1] != '\0'){
            puts("Username length exceeded please try again!");
        } else {
            break;
        }
    }

    //-------------------- Create / Connect Socket --------------------
    connection_info.hostname = host;
    if (create_socket(&connection_info) == -1){
        fprintf (stderr, "ERROR: %c\n", errno);
        exit(EXIT_FAILURE);
    }

    if (start_connection(&connection_info)== -1){
        fprintf (stderr, "ERROR: %c\n", errno);
        exit(EXIT_FAILURE);
    }


    return 0;
}
