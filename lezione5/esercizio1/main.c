#include<unistd.h>
#include<errno.h>
#include<stdio.h>

int main(int argc, char *argv[])
{
	int tot,x=0,y=0;
	while(x != -1)
	{
		if(read(STDIN_FILENO,&x,sizeof(int)) == -1)
		{
			perror("read x");
			return 0;
		}
		if(read(STDIN_FILENO,&y,sizeof(int)) == -1)
		{
			perror("read y");
			return 0;
		}
		tot = x + y;
		write(STDOUT_FILENO,&tot,sizeof(int));
	}
	return 1;
}
