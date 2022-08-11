#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

void q1(void) {
	int fd[2];
	pipe(fd);

	if (fork() == 0) {
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]); close(fd[1]);
		execl("/usr/bin/sort", "sort", NULL);
	}
	else {
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]); close(fd[1]);
		execl("/usr/bin/head", "head", "-2", NULL);
	}
}

char count = 0;
void sigquit_handler(int sig_num) {
	signal(SIGQUIT, sigquit_handler);

	write(1, "SIGQUIT\n", 8);

	if (++count == 2)
		exit(0);
}

void install_handler(void) {
	signal(SIGQUIT, sigquit_handler);
}

void q2(void) {
	int childpid = fork();
	if (childpid == 0) {
		install_handler();
		for (; ; sleep(1));
	}
	else {
		sleep(1);
		kill(childpid, SIGQUIT);
		sleep(1);
		kill(childpid, SIGQUIT);
		sleep(1);
		kill(childpid, SIGQUIT);

		int exit_code = 0;
		wait(&exit_code);
	}
}

int main() {
	q2();

	return 0;
}