#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <netdb.h> 
#include <unistd.h>
#include <pthread.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <sys/socket.h>


#define MAXDATASIZE 1024 // max buffer size 
#define SERVER_PORT 2001

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

    memset(&their_addrinfo, 0, sizeof(their_addrinfo));

    their_addrinfo.ai_family = AF_INET;
    their_addrinfo.ai_socktype = SOCK_STREAM;

    int info = getaddrinfo(hostname, port, &their_addrinfo, &their_addr);
    if (info != 0) {
        printf("ERROR: Could not get addr info\n");
        exit(EXIT_FAILURE);
    }

    struct addrinfo* addr = NULL;
    for (addr = their_addr;  addr != NULL; addr = addr->ai_next) {
        sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sockfd == -1)
            continue;

        if (connect(sockfd, addr->ai_addr, addr->ai_addrlen) != -1)
            break;

        close(sockfd);
    }

    freeaddrinfo(their_addr);

    if (addr == NULL) {
        printf("ERROR: Could not connect to socket\n");
        exit(EXIT_FAILURE);
    }

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
