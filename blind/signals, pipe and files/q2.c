#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

static char count = 0;
void siquit_handler(int sig_num) {
    signal(SIGQUIT, siquit_handler);

    write(1, "SIGQUIT\n", 8);
    if (++count >= 2)
        exit(0);
}

void install_handler() {
    signal(SIGQUIT, siquit_handler);
}

int main() {
    int child_pid = 0;
    if (!(child_pid = fork())) {
        install_handler();
        for(;;);
    } else {
        sleep(1);
        kill(child_pid, SIGQUIT);
        sleep(1);
        kill(child_pid, SIGQUIT);
        sleep(1);
        kill(child_pid, SIGQUIT);
    }

    int rc;
    wait(&rc);

    printf("finished\n");

    return 0;
}