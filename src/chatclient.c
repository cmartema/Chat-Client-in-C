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

int handle_stdin(int client_socket){
		if(fgets(outbuf, sizeof(outbuf), stdin) == NULL){
			perror("fgets()");
		}
		char *eoln = strchr(outbuf, '\n'); 
		if(eoln != NULL){
			*eoln = '\0';
		}
		if(strlen(outbuf) > MAX_MSG_LEN){
			fprintf(stderr, "Sorry, limit your message to 1 line of at most %d characters.\n", MAX_MSG_LEN);
			/* Clears stdin */
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}
		}

		if(!strcmp(outbuf, "bye")){
			printf("Goodbye.\n");
			return 1; 
		}
		int ret = send(client_socket, outbuf, strlen(outbuf) + 1, 0);
		if(ret < 0) {
			fprintf(stderr, "Error: Failed to send message to server.\n");
			return -1;
		}
		//printf("[%s]: ", username); 

	return 0; 
}

int handle_client_socket(int client_socket){
	 memset(inbuf, 0, BUFLEN);
	 int bytes_recvd; 
         if ((bytes_recvd = recv(client_socket, inbuf, BUFLEN-1, 0)) < 0) {
		 if(errno != EINTR){
			 fprintf(stderr, "Warning: Failed to receive incoming message.\n");
		 }
	 } else if (bytes_recvd == 0) {
		 fprintf(stderr, "\nConnection to server has been lost.\n");
	 } else {
		 if(!strcmp(inbuf, "bye")){
			 printf("\nServer initiated shutdown.\n");
			 return -1;
		 }
		 else{
			 printf("%s\n", inbuf); 
		 }
	 }

	return 0; 
}

// no signal handling in the client 
int main(int argc, char **argv) {
    /* TODO */
	
	// part 0: my id is 1057: random number = 24: 1000*24+057 = 24057 - Anita
	// part 0: my id is 1234: random number = 24: 1000*24+234 = 24234 - Chris
	
	 
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
			fprintf(stderr, "Sorry, limit your username to %d characters.\n", MAX_NAME_LEN);
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
	// TODO - only one client is connecting at a time 	
	// create tcp socket
	
	int client_socket, bytes_recvd, ip_conversion, retval = EXIT_SUCCESS; 
	struct sockaddr_in server_addr; 
	socklen_t addrlen = sizeof(struct sockaddr_in); 
	char buf[BUFLEN]; 
	char *addr_str = argv[1]; 

	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr, "Error: Failed to create socket. %s.\n", strerror(errno)); 
		retval = EXIT_FAILURE; 
		goto EXIT; 
	}

	memset(&server_addr, 0, addrlen); //Zero out structure and prevents bugs 	
	// connect to the server 
	ip_conversion = inet_pton(AF_INET, addr_str, &server_addr.sin_addr);
	if (ip_conversion == 0) {
		fprintf(stderr, "Error: Invalid IP address '%s'.\n", addr_str);
		retval = EXIT_FAILURE;
		goto EXIT;
	} else if (ip_conversion < 0) {
		fprintf(stderr, "Error: Failed to convert IP address. %s.\n", strerror(errno));
		retval = EXIT_FAILURE;
		goto EXIT;
	}

	server_addr.sin_family = AF_INET; // Internet adress family
	int PORT = atoi(argv[2]);
	server_addr.sin_port = htons(PORT); // im using my port check in case
	//server_addr.sin_addr.s_addr = ip_conversion;
	
	if (connect(client_socket, (struct sockaddr *)&server_addr, addrlen) < 0) {
		fprintf(stderr, "Error: Failed to connect to server. %s.\n",
				strerror(errno));
		retval = EXIT_FAILURE;
		goto EXIT;
	}

	// receive welcome message from server - with error checking
	//memset(inbuf, 0, BUFLEN);
	if ((bytes_recvd = recv(client_socket, buf, BUFLEN-1, 0)) < 0) {
		fprintf(stderr, "Warning: Failed to receive incoming message. %s.\n", strerror(errno));
		retval = EXIT_FAILURE;
		goto EXIT;
	} else if (bytes_recvd == 0) {
		fprintf(stderr, "Connection to server has been lost\n");
		retval = EXIT_FAILURE;
		goto EXIT;
	} else {
		// print new line, welcome message, and two more lines 
		buf[bytes_recvd] = '\0';
		printf("\n%s\n\n", buf);
		
		if (send(client_socket, username, strlen(username) + 1, 0) < 0) {
			fprintf(stderr, "Error: Failed to send username to server. %s.\n",
					strerror(errno));
			retval = EXIT_FAILURE;
			goto EXIT;
		}
	}
		
	// part 4
	// use fd_set and select to loop forever
	// use STDIN_FILENO to prompt for user input 
	// check activity on client socket and store in inbuf; if "bye", exit
	fd_set read_fds;
	bool running = true;
	while(running){
		printf("[%s]: ", username);
	       	fflush(stdin); 	
		FD_ZERO(&read_fds);
		FD_SET(client_socket, &read_fds);
		FD_SET(STDIN_FILENO, &read_fds);
		if (select(client_socket + 1, &read_fds, NULL, NULL, NULL) < 0 && errno != EINTR ) {
			fprintf(stderr, "Error: Failed to select on file decriptors. %s.\n",
					strerror(errno));
			retval = EXIT_FAILURE;	 	
			goto EXIT;
		}

		if (FD_ISSET(client_socket, &read_fds) && running) { 	
			printf("\n"); 
			if (handle_client_socket(client_socket) < 0) {
				retval = EXIT_FAILURE;	 	
				goto EXIT;
			}
		}

		if (FD_ISSET(STDIN_FILENO, &read_fds) && running) {	
			int ret = handle_stdin(client_socket);
			if (ret < 0) {
				retval = EXIT_FAILURE;	 	
				goto EXIT;
			}
			else if (ret > 0){
				goto EXIT;
			}
		}

			
	}


EXIT:
	// if client socket has a file descriptor, close it.
	if (fcntl(client_socket, F_GETFD) != -1) {
		close(client_socket);
	}
	return retval;
}
