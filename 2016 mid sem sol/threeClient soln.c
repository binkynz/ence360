#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <netdb.h> 
#include <unistd.h>

#define MAXDATASIZE 1024 // max buffer size 

int client_socket(char *hostname, char *port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct addrinfo their_addrinfo; // server address info
	struct addrinfo *their_addr = NULL; // connector's address information
	memset(&their_addrinfo, 0, sizeof(struct addrinfo));
	their_addrinfo.ai_family = AF_INET;        /* communicate using internet address */
	their_addrinfo.ai_socktype = SOCK_STREAM;  /* use TCP rather than datagram */
	getaddrinfo( hostname, port, &their_addrinfo, &their_addr); /* get IP address info for the hostname and port */

	connect(sockfd, their_addr->ai_addr, their_addr->ai_addrlen); //connect to the host/server

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
