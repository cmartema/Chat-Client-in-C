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
	// part 0: my id is 1057; random number = 24; 1000*24+057 = 24057
	
	// part 1 - cmd line arg checking
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

	// part 2 - prompt for username 
	bool validUsername = false;
	while(!validUsername){
		printf("Enter a username: "); 
		if(scanf("%s", username) < 1){
			perror("scanf()");
		}	
		// user length is 0 or they just enter an enter
		if(strlen(username) == 0 || strlen(username) == 1){
			continue;
		}
		else if (strlen(username) > MAX_NAME_LEN){
			printf("Sorry, limit your username to %d characters.\n", MAX_NAME_LEN);
			continue;
		}
		else {
			validUsername = true;
		}
	}
	char *eoln = strchr(username, '\n'); 
	if(eoln != NULL){
		*eoln = '\0';
	}
	printf("Hello, %s. Let's try to connect to the server.\n", username); 
	
	// part 3 
	// create tcp socket
	/* 
	int client_socket, bytes_recvd, ip_conversion, retval = EXIT_SUCCESS; 
	struct sockaddr_in server_addr; 
	socklen_t addrlen = sizeof(struct sockaddr_in); 
	char buf[BUFLEN]; 
	char *addr_str = argv[1]; 

	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr, "Error: Failed to create socket. %s.\n", strerror(errno)); 
		ret_val = EXIT_FAILURE; 
		goto EXIIT; 
	}
*/	
	// connect to the server 
	
	// receive welcome message from server - with error checking
	
	// print new line, welcome message, and two more lines
	
	// send username to server 
	

	// part 4
	// use fd_set and select to loop forever
	// use STDIN_FILENO to prompt for user input 
	
	// check activity on client socket and store in inbuf; if "bye", exit
	// close socket and terminate 
}
