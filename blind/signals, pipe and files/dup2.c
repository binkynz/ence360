#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int ctc[2];
    int ctp[2];

    pipe(ctc);
    pipe(ctp);

    if (!fork()) {
        dup2(ctc[1], STDOUT_FILENO);
        close(ctc[0]); close(ctc[1]);
        close(ctp[0]); close(ctp[1]);
        execl("/bin/ls", "ls", "-l", NULL);
    } else if (!fork()) {
        dup2(ctc[0], STDIN_FILENO);
        dup2(ctp[1], STDOUT_FILENO);
        close(ctc[0]); close(ctc[1]);
        close(ctp[0]); close(ctp[1]);
        execl("/usr/bin/sort", "sort", "-k", "+9", NULL);
    } else {
        dup2(ctp[0], STDIN_FILENO);
        close(ctc[0]); close(ctc[1]);
        close(ctp[0]); close(ctp[1]);
        execl("/usr/bin/head", "head", "-5", NULL);
    }
}