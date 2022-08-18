#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "list.h"

#define MAXDATASIZE 1024 // max buffer size 
#define SERVER_PORT 2000

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static list_t* sockets = NULL;

int listen_on(int port)
{

    int s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sa;
    sa.sin_family = AF_INET;          /* communicate using internet address */
    sa.sin_addr.s_addr = INADDR_ANY; /* accept all calls                   */
    sa.sin_port = htons(port); /* this is port number                */

    int rc = bind(s, (struct sockaddr *)&sa, sizeof(sa)); /* bind address to socket   */
    if (rc == -1) { // Check for errors
        perror("bind");
        exit(1);
    }

    rc = listen(s, 5); /* listen for connections on a socket */
    if (rc == -1) { // Check for errors
        perror("listen");
        exit(1);
    }

    return s;
}


int accept_connection(int s) {


    /////////////////////////////////////////////
    // TODO: Implement in terms of 'accept'

    /////////////////////////////////////////////  

    struct sockaddr_in sa;
    socklen_t sa_size = sizeof(sa);

    int sockfd = accept(s, (struct sockaddr*)&sa, &sa_size);
    if (sockfd == -1) {
        perror("accept");
        exit(0);
    }

    return sockfd;
}


void handle_request(int msgsock) {
    ///////////////////

      // This initial code reads a single message (and ignores it!)
    char buffer[MAXDATASIZE];
    int num_read = 0;

    do {
        num_read = read(msgsock, buffer, MAXDATASIZE - 1);
        buffer[num_read] = '\0';

        printf("client (%d): %s\n", msgsock, buffer);

        write(msgsock, buffer, strlen(buffer));
    } while (num_read > 0);
}


// handle request by forking a new process
void handle_fork(int msgsock) {

    //TODO: run this line inside a forked child process

    // Be very careful to close all sockets used, 
    // and exit any processes or threads which aren't used
      // Note that sockets open BEFORE a fork() are open in BOTH parent/child

    if (!fork()) {
        handle_request(msgsock);
        close(msgsock);
    } else {
        close(msgsock);
    }

    ///////////////////////////////////////////
}

void handle_request_mul(void* msgsock) {
    char buffer[MAXDATASIZE];
    int num_read = 0;

    int sockfd = *(int*)msgsock;

    do {
        num_read = read(sockfd, buffer, MAXDATASIZE - 1);
        buffer[num_read] = '\0';

        pthread_mutex_lock(&lock);
        for (list_t* node = sockets; node != NULL; node = node->next) {
            write(node->value, buffer, strlen(buffer));
            printf("echo to client (%d): %s\n", node->value, buffer);
        }
        pthread_mutex_unlock(&lock);
    } while (num_read > 0);

    pthread_mutex_lock(&lock);
    list_remove(&sockets, sockfd);
    pthread_mutex_unlock(&lock);
}

void handle_thread(int msgsock) {
    pthread_t id;

    pthread_mutex_lock(&lock);
    list_append(&sockets, msgsock);
    pthread_mutex_unlock(&lock);

    if (pthread_create(&id, NULL, (void*)&handle_request_mul, &msgsock) != 0) {
        perror("thread");
        close(msgsock);
    }
}

int main(int argc, char *argv[]) {
    printf("\nThis is the server with pid %d listening on port %d\n", getpid(), SERVER_PORT);

    // setup the server to bind and listen on a port
    int s = listen_on(SERVER_PORT);

    while (1) { // forever
        int msgsock = accept_connection(s); // wait for a client to connect
        printf("Got connection from client!\n");

        // handle the request with a new process
        // handle_fork(msgsock);
        handle_thread(msgsock);
    }

    close(s);
    exit(0);
}

