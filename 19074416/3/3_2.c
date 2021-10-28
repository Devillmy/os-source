#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(int argc,char **argv)
{

	pid_t pid;//子进程
	pid = fork();
	if(pid < 0)
	{
		printf("创建子进程失败\n");
		exit(1);	
	}
	else if(pid == 0)
	{
		/*Child1*/
		int result;
		int i = 0;
		char *cmd[20];
		
		for(i=0;i<20;i++)
		{
			cmd[i] = (char *)malloc(sizeof(char));
		}
		

		int p = 0;
		for(i=1;i<argc;i++)
		{
			strcpy(cmd[p++],argv[i]);
			printf("%s\n",cmd[i]);
		}
		
		cmd[p++] = (char *)NULL;

		printf("In child process. PID: %d, PPID: %d.\n\n",getpid(),getppid());	
		
		result = execvp(cmd[0],cmd);
		
		if(result == -1)
		{
			perror("In child process,failed to exec a program");
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

	return (EXIT_SUCCESS);
		
}

