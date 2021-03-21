#include "../include/interface.h"

void print_help(){
    puts("/help 				- shows available commands");
    puts("/list 				- return all connected users and thier status");
    puts("/public 			- connect to public chat");
    puts("/private <username> 		- sends private message to user");
    puts("/private-session <username> 	- request to start private session with user");
    puts("/accept 			- accept private session request");
    puts("/decline 			- decline private session request");
    puts("/leave 				- leave current session");
    puts("/exit 				- disconnect and exit");
}

void clear(){
    system("clear");
}

void get_username(char *username, size_t max_len){
    printf("Please enter your username:");
    fgets(username, max_len, stdin);
    trim_newline(username);
}

void trim_newline(char *line){
    int index = strlen(line) - 1;
    if (line[index] == '\n') {
        line[index] = '\0';
    }
}

command_t * parse_commands(char *line) {
    command_t * command = malloc(sizeof(command_t));
    char selected_username[USERNAME_LEN];
    if (strcmp(line, "/help")) {
        command->command = HELP;
    } else if (strcmp(line, "/list")){
        command->command = LIST;
    } else if (strcmp(line, "/public")){
        command->command = PUBLIC;
    } else if (strstr(line, "/private ")){
        command->command = PRIVATE;
        parse_username(line, selected_username);
        command->selected_user = selected_username;
    } else if (strstr(line, "/private-session ") != NULL){
        command->command = PRIVATE_SESSION;
        parse_username(line, selected_username);
        command->selected_user = selected_username;
    } else if (strcmp(line, "/accept")){
        command->command = ACCEPT;
    } else if (strcmp(line, "/decline")){
        command->command = DECLINE;
    } else if (strcmp(line, "/leave")){
        command->command = LEAVE;
    } else if (strcmp(line, "/exit")){
        command->command = EXIT;
    } else {
        command->command = INVALID_COMMAND;
    }
    return command;
}

void parse_username(char *command_line, char *username){
    //TODO add pre size check for username
    int can_parse = 0;
    int username_index = 0;
    for (int i = 0; i < strlen(command_line); i++){
        if (command_line[i] == ' ' && !can_parse){
            can_parse = 1;
        } else if (command_line[i] == '\0'){
            can_parse = 0;
            break;
        }
        if (can_parse){
            if (username_index < USERNAME_LEN - 2){
                username[username_index] = command_line[i];
                username_index++;
            }
        }
    }
    username[username_index] = '\0';
}

void remove_spaces(char* s) {
    const char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}

void remove_spaces_end(char* s) {
    for (int i = strlen(s) - 1; i > 0; i--){
        if (s[i] != ' ' && s[i] != '\0'){
            break;
        } else if (s[i] == ' ') {
            s[i] = '/0';
        }
    }
}