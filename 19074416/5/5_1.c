/*
利用posix信号量实现线程间同步，实现生产者消费者问题
命名的 可以用于进程之间同步
未命名的 只能用于同一进程内线程的同步————本实验用这种

一个信号量——初始化，P，V

int sem_init(sem_t *sem,int pshared,unsigned int value)成功调用返回基本都是0

int sem_wait(sem_t *sem);//P操作
int sem_post(sem_t *sem);//V操作


本实验中的简化生产者消费者问题：
｀创建２个生产者线程
　－分别从文件中读取１０个整数，并放入环形缓冲区
｀创建２个消费者线程
　－从唤醒缓冲区中读取１０个整数，每读取２个数据，一个线程将两个整数相加输出结果，一个线程将两个整数相乘输出结果
	
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>

#define MAX_LEN 5

int buf[MAX_LEN];//共享区长度
int in,out;//头尾数组标记下标

sem_t empty,full,mutex;


void *produce(void *p);//两个生产者执行的都是这个，通过ＰＶ搞定
void *consume(void *p);
void init_semaphore(sem_t *sem,int init_value,char *sem_name);




int main(int argc,char **argv)
{
	pthread_t tid[4];
	pthread_attr_t attr;//这个是初始化问题了，貌似不用也行，是一些奇怪的结构体
	int i;
	int result;
	int consumer_id[2] = {0,1};
	
	if(argc != 3)
	{
		printf("usage: %s file1 file2\n",argv[0]);
		exit(1);	
	}

	//信号量初始化部分
	init_semaphore(&empty,MAX_LEN,"empty");
	init_semaphore(&full,0,"full");
	init_semaphore(&mutex,1,"mutex");

	in = 0;
	out = 0;
	pthread_attr_init(&attr);

	//create producer threads
	for(i=0;i<2;i++)
	{
		result = pthread_create(&tid[i],&attr,produce,(void *)argv[1+i]);//把写到哪的文件地址作为值传入线程
		if(result)//(!=0)
		{
			perror("Error - creating producer");
			exit(1);
		}
	}

	//create consumer threads
	for(i=0;i<2;i++)
	{
		result = pthread_create(&tid[i+2],&attr,consume,(void *)&consumer_id[i]);//把消费者编号(消费者有区分)作为数组传入
		if(result)//(!=0)
		{
			perror("Error - creating consumer");
			exit(1);
		}
	}

	for(i=0;i<4;i++)
	{
		pthread_join(tid[i],(void **)NULL);//这个放在这里的意思是要等所有线程执行完毕
	}

	return 0;
}

void init_semaphore(sem_t *sem_p,int init_value,char *sem_name)
{
	int result;
	result = sem_init(sem_p,0,init_value);
	if(result)
	{
		perror("Init error mutex");
		printf("Semaphore name:%s\n",sem_name);
		exit(1);
	}

}

void *produce(void *p)
{
	FILE *fp;
	int i;
	int value;
	int result;

	printf("File Name:%s\n",(char *)p);
	
	fp = fopen((char *)p,"r");
	if(fp == NULL)
	{
		perror("Can Not open file");
		printf("File name:%s\n",(char *)p);
		exit(1);
	}
	
	for(i=0;i<10;i++)//读10个整形的数据
	{
		result = fscanf(fp,"%d",&value);
		if(result != 1)
		{
			printf("Read Error.\n");
			exit(1);
		}
		printf("Reading %d\n",value);

		sem_wait(&empty);
		sem_wait(&mutex);
		buf[in] = value;
		in = (in + 1)%MAX_LEN;
		sem_post(&mutex);
		sem_post(&full);
	}

	fclose(fp);
	pthread_exit((void *)NULL);//从线程中退出

}




void *consume(void *p)
{
	int i;
	int j;
	int values[2];
	int result;
	char op;
	for(i=0;i<5;i++)
	{
		for(j=0;j<2;j++)
		{
			sem_wait(&full);
			sem_wait(&mutex);
			values[j] = buf[out];
			out = (out+1)%MAX_LEN;
			sem_post(&mutex);
			sem_post(&empty);
		}
		if(*((int *)p) == 0 )
		{
			op = '+';
			result = values[0] + values[1];
		}
		else
		{
			op = '*';
			result = values[0] * values[1];
		}
		
		printf("%d %c %d = %d\n",values[0],op,values[1],result);
	}
	pthread_exit((void *)NULL);
}
