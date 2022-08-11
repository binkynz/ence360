#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

char count = 0;

void sigquit_handler() {
    signal(SIGQUIT, sigquit_handler);

    write(1, "SIGQUIT\n", 8);

    count += 1;
    if (count == 2)
        exit(0);
}

void install_handler() {
    signal(SIGQUIT, sigquit_handler);
}

int main(void) {
    pid_t pid = fork();

    switch (pid) {
        case 0:
            install_handler();

            for (;; sleep(1));
        default:
            sleep(1);
            kill(pid, SIGQUIT);
            sleep(1);
            kill(pid, SIGQUIT);
            sleep(1);
            kill(pid, SIGQUIT);
            sleep(1);
            kill(pid, SIGQUIT);
            sleep(1);
            break;
    }

    return 0;
}