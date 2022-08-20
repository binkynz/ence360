#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <errno.h>
#include <unistd.h>

int server_socket(char *port) 
// execute socket(), bind(), listen(), accept() and return the new socket from accept()
{
	// var to setup the server
	struct sockaddr_in hints = {0};

	hints.sin_family = AF_INET; // ipv4
	hints.sin_port = htons(atoi(port)); // define port
	hints.sin_addr.s_addr = 0; // INADDR_ANY - bind socket to all available interfaces

	// create a ipv4, tcp socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// bind socket using hints
	bind(sockfd, (struct sockaddr*)&hints, sizeof(hints));

	// listen for incoming connections with backlog of 5
	listen(sockfd, 5);

	// accept a connection and store the new socket fd
	int clientfd = accept(sockfd, NULL, NULL);

	// return the clients fd
	return clientfd;
}


int main(int argc, char *argv[])  {

    if (argc != 2) { //if user doesn't provide the clients hostname and port number 
            fprintf(stderr,"usage: threeServer port-number\n"); 
            exit(1); 
    } 

	printf("\nThis is the server with pid %d listening on port %s\n", getpid(), argv[1]);
	
	int sockfd = server_socket(argv[1]);
	
	char* message = "congrats you successfully connected to the server!\n";
	write (sockfd, message, strlen(message));

    close(sockfd);

    exit (0);
}

