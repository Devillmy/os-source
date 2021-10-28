#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#define MAX 256

struct msgbuf {
	long mtype;
	char mtext[MAX];
};

int main(void) {
	key_t key;
	int msgid;
	int ret;

	struct msgbuf msg;

	key = ftok(".", 0x03);
	if(key == -1) {
		printf("ftok error!\n");
		return -1;
	}

	msgid = msgget(key, 0666 | IPC_CREAT);
	if(msgid == -1) {
		printf("msgget error!\n");
		return -1;
	}

	while(1) {
		memset(msg.mtext, 0, MAX);

		ret = msgrcv(msgid, &msg, MAX, 0, 0);
		if(ret == -1) {
			printf("msgrcv error!\n");
			return -1;
		}

		printf("message form: %ld, message: %s\n", msg.mtype, msg.mtext);

		if(strncmp(msg.mtext, "quit", 4) == 0) {
			printf("quit!!!\n");
			goto out;
		}
	}
	
out:
	ret = msgctl(msgid, IPC_RMID, NULL);
	if(ret == -1) {
		printf("msgctl error!\n");
		return -1;
	}

	return 0;
}
