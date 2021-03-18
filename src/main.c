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

int main (int argc, char **argv) {
    int debug = 0;
    char *port = NULL;
    char *host = NULL;
    int index;
    int c;

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

    //----------------------- Client Socket Setup -----------------------

    //-------------------- Client Connect to Server ---------------------

    //--------------------- Start Client Interface ----------------------

    return 0;
}
