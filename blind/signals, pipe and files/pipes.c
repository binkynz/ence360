#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SIZE 256

int ctp[2];
int ptc[2];

void child() {
    close(ctp[0]);
    close(ptc[1]);

    char message[BUF_SIZE];
    int bytes = read(ptc[0], message, BUF_SIZE - 1);
    message[bytes] = '\0';

    for (int i = 0; i < bytes; i++)
        message[i] = toupper(message[i]);
    write(ctp[1], message, bytes);

    close(ctp[1]);
    close(ptc[0]);
}

void parent(char* message) {
    close(ctp[1]);
    close(ptc[0]);

    write(ptc[1], message, strlen(message));
    int bytes = read(ctp[0], message, BUF_SIZE - 1);
    message[bytes] = '\0';

    printf("recieved: %s\n", message);

    int rc;
    wait(&rc);

    close(ctp[0]);
    close(ptc[1]);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: ./pipes message\n");
        exit(0);
    }

    pipe(ctp);
    pipe(ptc);

    char* message = argv[1];
    if (!fork()) 
        child();
    else
        parent(message);

    return 0;
}