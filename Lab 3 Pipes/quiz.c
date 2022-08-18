#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define WRITE_PIPE 1
#define READ_PIPE 0

int file_desc[2];

void close_pipes(void) {
    close(file_desc[WRITE_PIPE]);
    close(file_desc[READ_PIPE]);
}

int main(void) {
    pipe(file_desc);

    switch (fork()) {
        case 0:
            /*
             * child process redirects stdout to point to the write
             * end of the pipe. thus, the execl output is wrote to the pipe.
             * (here)--->
             */
            dup2(file_desc[WRITE_PIPE], STDOUT_FILENO);
            close_pipes();

            execl("/usr/bin/sort", "sort", NULL);
            break;
        default:
            /*
             * parents process redirects stdin to point to the read
             * end of the pipe. thus, the execl uses the input from the pipe.
             * --->(here)
             */
            dup2(file_desc[READ_PIPE], STDIN_FILENO);
            close_pipes();

            int child_status = 0;
            wait(&child_status);

            /*
             * without the wait, the execl will hang until the child process
             * has wrote to the pipe (stdout)
             */
            execl("/usr/bin/head", "head", "-2", NULL);
            break;
    }

    return 0;
}