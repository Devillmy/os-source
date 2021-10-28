#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char **argv) {
	char command[256];
	char *p;
	while(1) {
		printf(">");
		command[0] = '\0';
		p = fgets(command,256,stdin);
		if(p==NULL)
		{
			perror("Error in fgets()");
			exit(-1);
		}
		command[strlen(command)-1] = '\0';

		if(!strcmp(command,"quit"))
		{
			break;
		}

		printf("p = %s\n",p);
		
		pid_t pid;
		pid = fork();
		
		if(pid<0)
		{
			perror("Faild while calling fork");
			exit(-1);
		}
		else if(pid == 0)
		{
			//用来存放命令的空间
			int start = 0;
			char *arg[20];
			int i = 0;			
			int result;				
			for(i=0;i<20;i++)
			{
				arg[i] = (char *)malloc(sizeof(char));
			}
		
			char *temp;
			temp = (char *)malloc(sizeof(char));
			int tempstart = 0;


			for(i=0;i<strlen(p)+1;i++)
			{
				if(p[i]=='\0' || p[i] == '\t' || p[i] == ' ')
				{
					temp[tempstart] = '\0';				
					strcpy(arg[start++],temp);
					tempstart = 0;
					temp[0] = '\0';	
					continue;
				}
			
				temp[tempstart++] = p[i];
			}
			//strcpy(arg[start++],temp);
		
			char *argt[20];			
			for(i=0;i<20;i++)
			{
				argt[i] = (char *)malloc(sizeof(char));
			}	
			int jilu = start;
			start = 0;	
			//对命令进行再次的一轮筛选
			for(i=0;i<jilu;i++)
			{
				if (arg[i][0] =='\0' || arg[i][0] =='\t' || arg[i][0] == ' ')
				{
					continue;
				}
				strcpy(argt[start++],arg[i]);
			}

			for(i=0;i<start;i++)
			{
				printf("argt[%d] = %s\n",i,argt[i]);		
			}

			argt[start++] = (char *)NULL;
			

			result = execvp(argt[0],argt);
		
			for(i=0;i<start;i++)
			{
				//printf("argt[%d] = %s\n",i,argt[i]);		
				perror(argt[i]);			
				exit(result);			
			}
			exit(0);
			
		}
		else
		{
			//这里是父进程的后续执行区域
			int status;
			printf("In parent process, PID = %d, PPID = %d.\n",getpid(),getppid());		
			printf("Parent process is waiting...\n");		
			wait(&status);
			printf("\n In parent process, status = 0x%x, WEXITSTATUS(status) = %d (i.e. 0x%x)\n",status,WEXITSTATUS(status),WEXITSTATUS(status));
		}


		

				
		
		

		

	}
}

