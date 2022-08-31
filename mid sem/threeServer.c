#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <errno.h>
#include <unistd.h>

#define MAXDATASIZE 1024  // create a definition for the max amount of data to send/recieve

int main(int argc, char *argv[])  {
	// check for the correct number of arguments
    if (argc != 2) {  
		// write to stderror
        fprintf(stderr,"usage: threeServer port-number\n"); 
        exit(1); // exit with error code
    } 

	// print the pid and port of the server
	printf("\nThis is the server with pid %d listening on port %s\n", getpid(), argv[1]);
	
	// create a ipv4, tcp socket and store the socket's file descriptor in sockfd
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// create variables to define the nature of the server (and store the callee's infromation on accept)
	struct sockaddr_in sa, caller; 
	sa.sin_family = AF_INET; // ipv4 socket  
	sa.sin_addr.s_addr = INADDR_ANY; // accept all ip addresses from clients
	sa.sin_port = htons(atoi(argv[1])); // turn the port number into an interger, and make sure its in network byte order

	// bind socket using sa (the nature of the server)
	bind(sockfd, (struct sockaddr*)&sa, sizeof(sa));

	// listen for incoming connections with backlog of 5
	listen(sockfd, 5);

	// calculate the length of the caller
	socklen_t length = sizeof(caller);
	
	// accept a connection and store the new socket fd and fill caller variable with information
	// about the caller
	int clientfd = accept(sockfd, (struct sockaddr* )&caller, &length);

	// close the old file descriptor
	if (clientfd != sockfd)
		close(sockfd);

	// create a variable to store a message to send to the client
	char message[MAXDATASIZE] = "congrats you successfully connected to the server!";
	// loop while the message variable is not empty
	while (strlen(message) > 0)
	{
		int numbytes; // number of bytes of data read from socket

		// send data to the client and then get data back from the client:
		
		// write the message to the client
		write(clientfd, message, strlen(message));

		// read a message from the client
		numbytes = read(clientfd, message, MAXDATASIZE - 1);

		// remove one character from the end of the message by replacing it with a null character
		message[numbytes - 1] = '\0';
	}

	// close the clients file descriptor
	close(clientfd);

	// exit
    exit(0);
}

