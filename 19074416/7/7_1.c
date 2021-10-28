#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

static void sig_usr(int signo);

int main(int argc, char **argv) {
	pid_t pid;
	
	pid = fork();
	if(pid == 0) {
		/*In Child Process*/
		printf("Child's pid = %d\n", getpid());
		
		if(signal(SIGUSR1, sig_usr) == SIG_ERR) {
			printf("Can't catch SIGUSR_1\n");
			exit(1);
		}

		if(signal(SIGUSR2, sig_usr) == SIG_ERR) {
			printf("Can't catch SIGUSR_2\n");
			exit(1);
		}
		
		signal(SIGINT, sig_usr);
		for(;;) pause;
	}

	else {
		/*In Parnt Process*/
		printf("Parents's pid = %d\n", getpid());

		if(signal(SIGUSR1, sig_usr) == SIG_ERR) {
			printf("Can't catch SIGUSR_1\n");
			exit(1);
		}
		signal(SIGINT, sig_usr);
		for(;;) pause;
	}
}

static void sig_usr(int signo) {
	if(signo == SIGUSR1) {
		printf("Received signal %d\t", signo);
		printf("Received SIGUSR1\n");
	}
	else if(signo == SIGUSR2) {
		printf("Received signal %d\t", signo);
		printf("Received SIGUSR2\n");
	}
	else {
		printf("Received signal %d\n", signo);
		int pid = getpid();
		printf("\tpid = %d\n", pid);
		exit(1);
	}

}
