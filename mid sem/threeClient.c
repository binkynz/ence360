#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <netdb.h> 
#include <unistd.h>

#define MAXDATASIZE 1024  // create a definition for the max amount of data to send/recieve

int main(int argc, char *argv[]) 
{ 
	// check for the correct number of arguments
    if (argc != 3) {  
		// print to stderr
        fprintf(stderr,"usage: threeClient hostname port-number\n"); 
        exit(1); // exit with error code
    } 

	// create a new internet tcp socket and store the sockets file descriptor
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct addrinfo their_addrinfo; // server address info
	struct addrinfo *their_addr = NULL; // connector's address information
	memset(&their_addrinfo, 0, sizeof(struct addrinfo)); // clear the memory of their_addrinfo
	their_addrinfo.ai_family = AF_INET;        /* communicate using internet address */
	their_addrinfo.ai_socktype = SOCK_STREAM;  /* use TCP rather than datagram */
	getaddrinfo(argv[1], argv[2], &their_addrinfo, &their_addr); /* get IP address info for the hostname (argv[1]) and port (argv[2]) */

	// connect to the ip address recieved from getaddrinfo
	connect(sockfd, their_addr->ai_addr, their_addr->ai_addrlen);

	// free the linked list allocated to their_addr in getaddrinfo
	freeaddrinfo(their_addr);

	char buffer[MAXDATASIZE]; //buffer contains data from/to server
	int numbytes; // number of bytes of data read from socket
	// get data from the server:
	
	// read from the socket into the buffer and store the number of bytes recieved
	numbytes = read(sockfd, buffer, MAXDATASIZE - 1);

	// loop until we reach an EOF or the connection is interrupted
	while (numbytes > 0)
	{
		// remove one character from the end of the buffer by replacing it with a null character
		buffer[numbytes-1] = '\0';
		printf("%s\n", buffer); //print out what you received 

		// send data to the server and then get data back from the server:
		
		// write the buffer to the server
		write(sockfd, buffer, strlen(buffer));

		// re-read from the server and store in the buffer
		numbytes = read(sockfd, buffer, MAXDATASIZE - 1);
	}
	
	// close the socket file descriptor
	close(sockfd);

	// exit
    return 0; 
} 



/* The expected output is listed below - very briefly describe why the output appears like this:



congrats you successfully connected to the server
congrats you successfully connected to the serv
congrats you successfully connected to the se
congrats you successfully connected to the
congrats you successfully connected to th
congrats you successfully connected to
congrats you successfully connected t
congrats you successfully connected
congrats you successfully connect
congrats you successfully conne
congrats you successfully con
congrats you successfully c
congrats you successfully
congrats you successful
congrats you successf
congrats you succes
congrats you succ
congrats you su
congrats you
congrats yo
congrats
congrat
congr
con
c

*/

