#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct node {
	int number;
	char c;
};

void *getstruct(void *jiegou);

int main(int argc, char **argv) {
	struct node* jg;
	jg = (struct node *)malloc(sizeof(struct node));
	pthread_t tid; //threadid
	int rc;

	printf("请输入一个整数值和一个字符，形式为a b\n");
	scanf("%d %c", &jg -> number, &jg -> c);

	rc = pthread_create(&tid, NULL, getstruct, jg);
	if(rc) {
		printf("Faild in calling pthread_create");
		exit(-1);
	}

	struct node* retval;
	pthread_join(tid, (void **)&retval);
	printf("主函数中读取返回值： %d_%c\n", retval->number, retval->c);
	pthread_exit(NULL);
}

void *getstruct(void *jiegou) {
	struct node *tempnode;
	
	tempnode = jiegou;
	printf("线程内部：传入线程中的结构体参数为%d_%c\n", tempnode->number, tempnode->c);
	pthread_exit(tempnode);
}

