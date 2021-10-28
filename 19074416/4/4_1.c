#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_THREADS 2

void *PrintHello(void *threadid) {
	long tid;
	tid = (long) threadid;

	printf("Hello World! It's me, thread#%ld\n", tid);
	pthread_exit(NULL);
}

void *PrintHello2() {
	printf("19074416LMY\n");

	pthread_exit(NULL);
}

int main(int argc, char **argv) {
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;

	printf("In main: Creating thread %d\n", 0);
	rc = pthread_create(&threads[0], NULL, PrintHello, (void *)t);
	if(rc) {
		perror("Failed in calling pthread_create");
		exit(-1);
	}
	
	printf("In main: Creating thread %d\n", 1);
	rc = pthread_create(&threads[1], NULL, PrintHello2, NULL);
	if(rc) {
		perror("Failed in caling pthread_create");
		exit(-1);
	}

	pthread_exit(NULL);
}
