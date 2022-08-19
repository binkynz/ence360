#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void async_wait(int sig_num) {
    int return_code;
    wait(&return_code);

    printf("child process exited with code: %d\n", return_code);
}

int main() {
    signal(SIGCHLD, async_wait);

    if (!fork()) {
        ; // fuck u kids
    } else {
        sleep(1); // taking too damn long
    }

    return 0;
}