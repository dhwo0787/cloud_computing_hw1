#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main() {
    
    char server_msg[256] = "Success connect to Server";

    int socket_s;

    socket_s = socket(AF_INET, SOCK_STREAM , 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5050);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to our environment
    bind(socket_s, (struct sockaddr *) &server_addr, sizeof(server_addr));

    listen(socket_s, 10);

    int client_socket;
    client_socket = accept(socket_s,NULL,NULL);

    send(client_socket,server_msg,sizeof(server_msg),0);
   
    close(socket_s); 
    return 0;
}
