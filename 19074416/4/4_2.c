#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
//两个线程，传入两个参数并打印

void *getzhengshu(void *p);//要在线程中打印？
void *getzifu(void *p);


int main(int argc,char **argv) {
	int i;
    	long zhengshuzhi;
    	char zifu;
    	int rc;
    	pthread_t tid[2];

    	printf("请输入一个整数值和一个字符，形式为 a b\n");
    	scanf("%ld %c",&zhengshuzhi,&zifu);

    	printf("In main:creating thread 1\n");
    	rc = pthread_create(&tid[0],NULL,getzhengshu,(void *)zhengshuzhi);
    	if(rc) {
        	perror("Failed in calling pthread_create");
        	exit(-1);
    	}

    	printf("In main:creating thread 2\n");
    	rc = pthread_create(&tid[1],NULL,getzifu,(void *)zifu);
    	if(rc) {
        perror("Failed in calling pthread_create");
        exit(-1);
    }

    	pthread_exit(NULL);
}

void *getzhengshu(void *p) {
    	int *q;
    
   	q = (int *)p;
    	printf("在线程中输出整数 %d\n",q);
    	pthread_exit(NULL);
}

void *getzifu(void *p) {
    	char *q;

    	q = (char *)p;
    	printf("在线程中输出字符 %c\n",q);
    	pthread_exit(NULL);
}
