#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fd[2];
    pipe(fd);

    if (!fork()) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]); close(fd[1]);
        execl("/usr/bin/sort", "sort", NULL);
    } else {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]); close(fd[1]);
        execl("/usr/bin/head", "head", "-2", NULL);
    }

    return 0;
}