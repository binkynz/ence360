#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <errno.h>
#include <unistd.h>

int server_socket(char *port) 
// execute socket(), bind(), listen(), accept() and return the new socket from accept()
{
	struct addrinfo hints = {0};
	struct addrinfo* result;

	hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // INADDR_ANY

	getaddrinfo(NULL, port, &hints, &result);

	int sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(sockfd, result->ai_addr, result->ai_addrlen);

	freeaddrinfo(result);

	listen(sockfd, 5);

	int clientfd = accept(sockfd, NULL, NULL);
	close(sockfd); // dont forget to close me!

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

