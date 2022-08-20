#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <netdb.h> 
#include <unistd.h>

#define MAXDATASIZE 1024 // max buffer size 

int client_socket(char *hostname, char *port)
// execute socket() and connect() and return the socket ID
{
	struct addrinfo hints = {0};
    struct addrinfo* result;

	hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

	getaddrinfo(hostname, port, &hints, &result);

	int sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	connect(sockfd, result->ai_addr, result->ai_addrlen);

	freeaddrinfo(result);

	return sockfd;
}


int main(int argc, char *argv[]) 
{ 
    char buffer[MAXDATASIZE]; //buffer contains data coming in from read() system call 

    if (argc != 3) { // user didn't provide the client's hostname and/or port number 
            fprintf(stderr,"usage: threeClient hostname port-number\n"); 
            exit(1); 
    } 

	int sockfd = client_socket(argv[1], argv[2]); // create client socket and connect to a server/host

    int numbytes = read(sockfd, buffer, MAXDATASIZE-1); // receive data from the host/server 

    buffer[numbytes] = '\0'; 
    printf("\nReceived from server: %s\n\n",buffer); //print out what you received 

    close(sockfd); //close socket

    return 0; 
} 
