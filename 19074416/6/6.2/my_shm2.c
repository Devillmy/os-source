#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX 256

int main(void) {
	key_t key;
	int shmid;
	int ret;
	char *buffer;

	/*构造访问共享内存的键值*/
	key = ftok(".", 0x03);
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

	/*将共享内存中的内容读取出来*/
	printf("The msg is %s\n", buffer);

	/*撤销共享内存在当前进程的映射*/
	shmdt(buffer);

	/*删除这块共享内存*/
	ret = shmctl(shmid, IPC_RMID, NULL);
	if(ret == -1) {
		printf("shmctl error!\n");
		return -1;
	}

	return 0;
}
