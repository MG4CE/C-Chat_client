#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socket_client.h"

#define COMMAND_PREFIX "/"
#define INPUT_USER_LEN 22

void print_help();
void clear_window();
void get_username(char *username, size_t max_len);
void parse_username(char *command_line, char *username);
void trim_newline(char *line);
void remove_spaces(char* s);
void remove_spaces_end(char* s);
void print_message(message_t *message);
#endif