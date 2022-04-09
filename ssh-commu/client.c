#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>


int main() {
	// socket
	int network_socket;
	network_socket = socket(AF_INET , SOCK_STREAM ,0);
	
	// address for socket
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5050);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	int connect_stat = connect(network_socket,(struct sockaddr *)&server_addr, sizeof(server_addr));
	if(connect_stat == -1) {
	    printf("error\n");   
	}
	
	char server_response[256];
	recv(network_socket,&server_response, sizeof(server_response),0);
	
	printf("server sent : %s\n",server_response);

	close(network_socket);

	return 0;
}

