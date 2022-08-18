/* Title: pipedup2.c
 * Description: ENCE360 Example code - dup2.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
    int ctc_fd[2];
    int ctp_fd[2];

    pipe(ctc_fd);
    pipe(ctp_fd);

    if (!fork()) {
        dup2(ctc_fd[1], STDOUT_FILENO);
        close(ctc_fd[0]); close(ctc_fd[1]);
        close(ctp_fd[0]); close(ctp_fd[1]);
        execl("/bin/ls", "ls", "-l", NULL);
    } else if (!fork()) {
        dup2(ctc_fd[0], STDIN_FILENO);
        dup2(ctp_fd[1], STDOUT_FILENO);
        close(ctc_fd[0]); close(ctc_fd[1]);
        close(ctp_fd[0]); close(ctp_fd[1]);
        execl("/bin/sort", "sort", "-k", "+9", NULL);
    } else {
        dup2(ctp_fd[0], STDIN_FILENO);
        close(ctc_fd[0]); close(ctc_fd[1]);
        close(ctp_fd[0]); close(ctp_fd[1]);
        execl("/usr/bin/head", "head", "-5", NULL);
    }

    exit(0);
}
