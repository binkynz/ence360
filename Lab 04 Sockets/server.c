#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/socket.h>

#include "list.h"

#define MAXDATASIZE 1024 // max buffer size 
#define SERVER_PORT 2001

static node_t* list = NULL;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

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
    int sockfd;
    struct sockaddr_in client_addr;
    socklen_t client_len;

    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = INADDR_ANY;

    client_len = sizeof(client_addr);

    sockfd = accept(s, (struct sockaddr*)&client_addr, &client_len);
    if (sockfd == -1) {
        perror("accept");
        exit(1);
    }

    printf("Accepted: %d\n", sockfd);

    return sockfd;
}

void handle_request(int msgsock) {
    char buffer[MAXDATASIZE];
    int num_read = 0;

    do {
        num_read = read(msgsock, buffer, MAXDATASIZE - 1);
        buffer[num_read] = '\0';

        write(msgsock, buffer, num_read);

        printf("client: %s\n", buffer);
    } while (num_read > 0);

    close(msgsock);
}

void handle_fork(int msgsock) {
    // Be very careful to close all sockets used, 
    // and exit any processes or threads which aren't used
    // Note that sockets open BEFORE a fork() are open in BOTH parent/child

    ///////////////////////////////////////////

    if (fork() == 0) {
        handle_request(msgsock);
    }
    else {
        close(msgsock);
    }
}

void* handle_thread_request(void* arg) {
    char buffer[MAXDATASIZE];
    int num_read = 0;
    int msgsock = *(int*)arg;

    do {
        num_read = read(msgsock, buffer, MAXDATASIZE - 1);
        buffer[num_read] = '\0';

        pthread_mutex_lock(&lock);
        for (node_t* item = list; item != NULL; item = item->next) {
            write(item->value, buffer, num_read);
            printf("client %d: %s\n", item->value, buffer);
        }
        pthread_mutex_unlock(&lock);
    } while (num_read > 0);

    pthread_mutex_lock(&lock);
    list_remove(&list, msgsock);
    close(msgsock);
    pthread_mutex_unlock(&lock);

    return NULL;
}

void handle_thread(int msgsock) {
    pthread_t id;

    pthread_mutex_lock(&lock);
    if (list == NULL)
        list = list_new(msgsock);
    else
        list_add(list, msgsock);
    pthread_mutex_unlock(&lock);

    if (pthread_create(&id, NULL, handle_thread_request, (void*)&msgsock) != 0) {
        perror("create");
        exit(1);
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

    list_free(list);
    close(s);
    exit(0);
}

