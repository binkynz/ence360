/* Title: pipedup2.c
 * Description: ENCE360 Example code - dup2.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define INP 1
#define OUTP 0

int ctc_fd[2];
int ctp_fd[2];

void close_the_fds_man(void) {
    close(ctc_fd[0]);
    close(ctc_fd[1]);
    close(ctp_fd[0]);
    close(ctp_fd[1]);
}

int main(void) {
    pid_t childpid;

    pipe(ctc_fd);
    pipe(ctp_fd);


    if ((childpid = fork()) == 0) { /* Child code: Runs ls */
        dup2(ctc_fd[INP], STDOUT_FILENO);
        close_the_fds_man();
        execl("/bin/ls", "ls", "-l", NULL);
        perror("The exec of ls failed");
    }
    else if ((childpid = fork()) == 0) { /* Child code: Runs sort */
        dup2(ctc_fd[OUTP], STDIN_FILENO);
        dup2(ctp_fd[INP], STDOUT_FILENO);
        close_the_fds_man();
        execl("/usr/bin/sort", "sort", "-r", "-k", "+8", NULL);
        /* Note: The location of sort depends on your distribution.
         * Use 'which sort' to find the correct location */
        perror("The exec of sort failed");
    }
    else { /* Parent code: Runs head */
        dup2(ctp_fd[OUTP], STDIN_FILENO);
        close_the_fds_man();
        execl("/usr/bin/head", "head", "-5", NULL);
        perror("The exec of head failed");
    }

    exit(0);
}
