#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX 256

int main(int argc, char **argv) {
	key_t key;
	int shmid;

	char *buffer;

	if(argc != 2) {
		printf("usage: %s <msg>\n", argv[0]);
		return -1;
	}

	/*构造一个共享内存的键值*/
	key = ftok(".", 0x3);
	if(key == -1) {
		printf("ftok error!\n");
		return -1;
	}

	/*创建一块共享内存*/
	shmid = shmget(key, MAX, IPC_CREAT | 0666);
	if(shmid == -1) {
		printf("shmget error!\n");
		return -1;
	}

	/*将共享内存映射到当前进程空间*/
	buffer = (unsigned char*)shmat(shmid, 0, 0);
	if(buffer == (void *) - 1) {
		printf("shmat error!\n");
		return -1;
	}
	
	/*将传入的内容写入到共享内存当中*/
	strncpy(buffer, argv[1], strlen(argv[1]));
	
	/*撤销共享内存内存在当前进程的映射*/
	shmdt(buffer);

	return 0;
}
