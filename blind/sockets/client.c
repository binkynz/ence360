#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("usage: host, port\n");
        exit(0);
    }

    struct addrinfo hints = {0};
    struct addrinfo* result;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(argv[1], argv[2], &hints, &result) < 0) {
        perror("get addr info");
        exit(0);
    }

    int sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sockfd < 0) {
        perror("socket");
        exit(0);
    }

    if (connect(sockfd, result->ai_addr, result->ai_addrlen) < 0) {
        perror("connect");
        exit(0);
    }

    freeaddrinfo(result);

    if (!fork()) {
        while (1) {
            char* line = readline("");
            write(sockfd, line, strlen(line));
        }
        close(sockfd);
    } else {
        char buffer[1024];
        int num_bytes;
        do {
            num_bytes = read(sockfd, buffer, 1023);
            buffer[num_bytes] = '\0';
            printf("recieved: %s\n", buffer);
        } while(num_bytes > 0);
        close(sockfd);
    }

    return 0;
}