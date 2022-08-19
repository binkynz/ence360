#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "list.h"

static list_t* clientfds = NULL;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* handle_request(void* arg) {
    int clientfd = *(int*)arg;

    char buffer[1024];
    int num_bytes;
    do {
        num_bytes = read(clientfd, buffer, 1023);
        buffer[num_bytes] = '\0';

        pthread_mutex_lock(&lock);
        for (list_t* fd = clientfds; fd != NULL; fd = fd->next) {
            write(fd->value, buffer, strlen(buffer));
            printf("echoed to (%d): %s\n", fd->value, buffer);
        }
        pthread_mutex_unlock(&lock);
    } while(num_bytes > 0);

    pthread_mutex_lock(&lock);
    list_remove(&clientfds, clientfd);
    pthread_mutex_unlock(&lock);

    close(clientfd);

    return NULL;
}

int main(int argc, char *argv[]) {
    struct sockaddr_in my_addr = {0};
    struct sockaddr_in their_addr;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(0);
    }

    my_addr.sin_family = AF_INET; 
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(2000);

    if (bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) {
        perror("bind");
        exit(0);
    }

    if (listen(sockfd, 5) < 0) {
        perror("listen");
        exit(0);
    }

    socklen_t their_addr_len = sizeof(their_addr);

    while (1) {
        int clientfd = accept(sockfd, (struct sockaddr*)&their_addr, &their_addr_len);
        if (clientfd < 0) {
            perror("accept");
            exit(0);
        }

        pthread_mutex_lock(&lock);
        list_append(&clientfds, clientfd);
        pthread_mutex_unlock(&lock);

        pthread_t id;
        if (pthread_create(&id, NULL, handle_request, &clientfd) < 0) {
            perror("thread");
            exit(0);
        }
    }

    return 0;
}