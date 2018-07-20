#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<errno.h>
#include<unistd.h>
int main(int argc, char *argv[])
{
	pid_t pid;
	if((pid = fork()) < 0)
	{
		perror("Errore fork");
		exit(0);
	}
	if(pid == 0)
		exit(1);
	if(execlp("ps","ps",NULL) < 0)
		printf("Errore execlp\n");
	exit(0);
}
