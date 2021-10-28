#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define MAX 256

struct msgbuf {
	long mtype;
	char mtext[MAX];
};

int main(int argc, char **argv) {
	key_t key;
	int msgid;
	int ret;

	struct msgbuf msg;

	if(argc != 2) {
		printf("usage: %s <msg>\n", argv[0]);
		return -1;
	}

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

	memset(msg.mtext, 0, MAX);

	msg.mtype = getpid();
	strcpy(msg.mtext,argv[1]);

	ret = msgsnd(msgid, &msg, strlen(msg.mtext), 0);
	if(ret == -1) {
		printf("msgsnd error!\n");
		return -1;
	}

	printf("pid: %ld, message: %s\n", msg.mtype, msg.mtext);

	return 0;
}
