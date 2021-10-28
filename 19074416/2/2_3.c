#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main() {

	printf("PID = %d, PPID = %d, Original\n", getpid(), getppid());

	pid_t result1;
	pid_t result2;

	result1 = fork();

	if(result1 < 0) {
		perror("Failed to create process after <A>");
		exit(1);
	}
	else if(result1 == 0) {
		printf("PID = %d, PPID = %d, Child1\n", getpid(), getppid());
		exit(0);
	}

	wait(NULL);
	printf("PID = %d, PPID = %d, Parent\n", getpid(), getppid());
	
	result2 = fork();

	if(result2 < 0) {
		perror("Failed to create process after <B>");
		exit(1);
	}
	else if(result2 == 0) {
		printf("PID = %d, PPID = %d, Child2\n", getpid(), getppid());
		exit(0);
	}
	wait(NULL);
	return (EXIT_SUCCESS);
}
	
