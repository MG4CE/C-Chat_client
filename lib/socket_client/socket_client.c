#include "socket_client.h"

int create_socket(client_socket_t *server_info) {
    if (resolve_hostname(server_info->hostname, server_info->server_ip) == -1){
        return -1;
    }
    int socket_desc = socket(IPV4, TCP, IP_PROTOCOL);
    if(socket_desc == -1){
        return -1;
    }
    return 0;
}

int start_connection(client_socket_t *server_info) {
    struct sockaddr_in server;

    server.sin_addr.s_addr = inet_addr(server_info->server_ip); //inet_addr converts string addr to unsigned long
    server.sin_port = htons(server_info->port); //Host TO Network Short
    server.sin_family = IPV4;

    return connect(server_info->socket_descriptor, (struct sockaddr *) &server, sizeof(server));
}

int send_message(client_socket_t *server_info, char *message, size_t message_size) {
    return send(server_info->socket_descriptor , message, message_size, 0);
}

int fetch_message(client_socket_t *server_info, char *message, size_t message_size) {
    return recv(server_info->socket_descriptor, message, message_size, 0);
}

int close_connection(client_socket_t *server_info) {
    return close(server_info->socket_descriptor);
}

int resolve_hostname(char *hostname, char *ip) {
	struct hostent *he;
	struct in_addr **addr_list;

    he = gethostbyname(hostname);

	if (he == NULL) 
	{
		herror("gethostbyname");
        return -1;
	}
	addr_list = (struct in_addr **) he->h_addr_list;
	
	for(int i = 0; addr_list[i] != NULL; i++) 
	{
		strcpy(ip , inet_ntoa(*addr_list[i]));
	}
    return 1;
}
