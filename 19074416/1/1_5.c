#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>



int main(int argc,char **argv)
{
	if(argc != 3)
	{
		printf("命令行参数个数有误，应为3个参数，分别对应程序，源文件名称，目标文件名称");
	}

	char *s1;
	s1 = (char *)malloc(sizeof(char));
	char *s2;
	s2 = (char *)malloc(sizeof(char));

	s1 = argv[1];
	s2 = argv[2];	

	char *t1;
	t1 = "cp";


	int kan1 = strlen(s1);
	int kan2 = strlen(s2);
	
	int flag = 1;

	while(kan1>=0 && kan2 >=0 )
	{
		if(s1[kan1] == '.' || s2[kan2] == '.')
		{
			break;
		}
		if(s1[kan1--]!=s2[kan2--])
		{
			flag = 0;
			break;
		}			
	}

	if(flag==0)
	{
		printf("请检查目的文件格式与源文件格式是否相同！\n");
		return 0;
	}

	printf("命令成功执行！\n");
	int result;	
	result = execlp(t1,t1,s1,s2,(char *)NULL);
	
	return 0;	

	//似乎已经进行了自动的错误处理
}

