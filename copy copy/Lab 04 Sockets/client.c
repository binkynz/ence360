#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <netdb.h> 
#include <unistd.h>

#include <pthread.h>

#include <readline/readline.h>
#include <readline/history.h>


#define MAXDATASIZE 1024 // max buffer size 
#define SERVER_PORT 2000

int client_socket(char *hostname)
{
    char port[20];
    struct addrinfo their_addrinfo; // server address info
    struct addrinfo *their_addr = NULL; // connector's address information  
    int sockfd;

    int n = snprintf(port, 20, "%d", SERVER_PORT); // Make a string out of the port number
    if ((n < 0) || (n >= 20))
    {
        printf("ERROR: Malformed Port\n");
        exit(EXIT_FAILURE);
    }

    /////////////////////////

    //TODO: 
    // 1) initialise socket using 'socket'
    // 2) initialise 'their_addrinfo' and use 'getaddrinfo' to lookup the IP from host name
    // 3) connect to remote host using 'connect'

    ///////////////////////////////

    if ((sockfd = socket(AF_INET , SOCK_STREAM , 0)) < 0) {
        perror("Socket");
        exit(0);
    }

    memset(&their_addrinfo, 0, sizeof(their_addrinfo));

    their_addrinfo.ai_family = AF_INET;
    their_addrinfo.ai_socktype = SOCK_STREAM ;

    if (getaddrinfo(hostname, port, &their_addrinfo, &their_addr) != 0) {
        perror("Addr info");
        exit(0);
    }

    if (connect(sockfd, their_addr->ai_addr, their_addr->ai_addrlen) < 0) {
        perror("Connect");
        exit(0);
    }

    freeaddrinfo(their_addr);

    return sockfd;
}

void* recieve_msg(void* arg) {
    char buffer[MAXDATASIZE];
    int numbytes = 0;

    do {
        numbytes = read(*(int*)arg, buffer, MAXDATASIZE - 1);
        buffer[numbytes] = '\0';

        printf("server: %s\n", buffer);
    } while (numbytes > 0);

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    int sockfd = client_socket(argv[1]);

    pthread_t id;
    if (pthread_create(&id, NULL, recieve_msg, (void*)&sockfd) != 0) {
        perror("create");
        exit(1);
    }

    char *line;
    do {
        line = readline("");
        write(sockfd, line, strlen(line));
    } while (1);

    free(line);
    close(sockfd);

    return 0;
}
