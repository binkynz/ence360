#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <errno.h>
#include <unistd.h>

int server_socket(char *port)
{
	
	int s = socket(AF_INET,SOCK_STREAM,0);	

	struct sockaddr_in sa, caller;  
	sa.sin_family= AF_INET;          /* communicate using internet address */
	sa.sin_addr.s_addr = INADDR_ANY; /* accept all calls                   */
    sa.sin_port = htons(atoi(port)); /* this is port number                */
        
    bind(s,(struct sockaddr *)&sa,sizeof(sa)); /* bind address to socket   */	

	listen (s, 5); /* listen for connections on a socket */
	
	socklen_t length = sizeof(caller);

	int msgsock = accept(s,(struct sockaddr *)&caller,&length); /* wait for call to socket, and return new socket */
    close (s);

	return msgsock;
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

