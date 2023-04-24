#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "util.h"

int client_socket = -1;
char username[MAX_NAME_LEN + 1];
char inbuf[BUFLEN + 1];
char outbuf[MAX_MSG_LEN + 1];

int handle_stdin() {
    /* TODO */
	return 0; 
}

int handle_client_socket() {
    /* TODO */
	return 0; 
}

// no signal handling in the client 
int main(int argc, char **argv) {
    /* TODO */
	// check for right number of command line args 
	if(argc == 1 || argc > 3){
		fprintf(stderr, "Usage: %s <server IP> <port>\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	// check for valid IP address using inet_pton()
	struct in_addr inaddr; 
	int rval; 
	if((rval = inet_pton(AF_INET, argv[1], &inaddr)) == 0){
		fprintf(stderr, "IPv4 address %s is invalid.\n", argv[1]);
	       return EXIT_FAILURE;
	}	       
	else if(rval == -1){
		perror("inet_pton"); 
		return EXIT_FAILURE; 
	}

	// chack that port number is in range 
	int port = atoi(argv[2]);
	if(port < 1024 || port > 65535){
		fprintf(stderr, "Error: Port %s is out of range. Port must be in range [1024, 65535].\n", argv[2]); 
		return EXIT_FAILURE; 
	}
}
