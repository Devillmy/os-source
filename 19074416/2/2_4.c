#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int pid_1, pid_2;

int main() {
	int fd[2];
	char outpipe[100], inpipe[100];
	pipe(fd);
	while((pid_1 = fork()) == -1);
	if(pid_1 == 0) {
		lockf(fd[1], 1, 0);
		sprintf(outpipe, "Child 1 is sending a message!");
		write(fd[1], outpipe, 50);
		sleep(5);
		lockf(fd[1], 0, 0);
		exit(0);
	}
	else {
		while((pid_2 = fork()) == -1);
		if(pid_2 == 0) {
			lockf(fd[1], 1, 0);
			sprintf(outpipe, "Child 2 is sending a message!");
			write(fd[1], outpipe, 50);
			sleep(5);
			lockf(fd[1], 0, 0);
			exit(0);
		}
		else {
			wait(0);
			read(fd[0], inpipe, 50);
			printf("%s\n", inpipe);
			wait(0);
			read(fd[0], inpipe, 50);
			printf("%s\n", inpipe);
			exit(0);
		}
	}
}
