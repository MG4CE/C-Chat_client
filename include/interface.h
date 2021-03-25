#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define COMMAND_PREFIX "/"
#define USERNAME_LEN 22

typedef enum{
    HELP,
    LIST,
    PRIVATE,
    LEAVE,
    EXIT,
    INVALID_COMMAND,
}commands;

typedef struct command{
    commands command;
    char *selected_user;
}command_t;

void print_help();
void clear_window();
void get_username(char *username, size_t max_len);
void trim_newline(char *line);
command_t * parse_commands(char *line);
void parse_username(char *command_line, char *username);
void remove_spaces(char* s);
void remove_spaces_end(char* s);
#endif