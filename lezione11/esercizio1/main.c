#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<stdlib.h>
#include<signal.h>
#define WRITE 1
#define READ 0

int main(int argc, char *argv[])
{
	pid_t pid[2];
	char buff[50];
	int fd[2],i,status;
	if(argc != 2)
	{
		printf("utilizzare il programma in questo modo: %s <path>\n",argv[0]);
		exit(0);
	}
	if(pipe(fd) < 0)
	{
		perror("Errore pipe");
		exit(0);
	}
	for(i=0;i<2;i++)
	{
		if((pid[i] = fork()) < 0)
		{
			close(fd[READ]);
			close(fd[WRITE]);
			kill(0,SIGKILL);
			exit(0);
		}
		else if(pid[i] == 0) //figli
		{
			if(i == 0) //primo figlio esegue ls -R
			{
				close(fd[READ]);
				if(dup2(fd[WRITE],1) < 0)
				{
					perror("Errore primo figlio dup2");
					exit(0);
				}
				close(fd[WRITE]);
				if(execlp("ls","ls","-R",NULL) < 0)
					perror("Errore primo figlio execlp");
				exit(0);
			}
			else //secondo figlio esegue grep argv
			{
				close(fd[WRITE]);
				if(dup2(fd[READ],0) < 0)
				{
					perror("Errore secondo figlio dup2");
					exit(0);
				}
				close(fd[READ]);
				if(execlp("grep","grep",argv[1],NULL) < 0)
					perror("Errore secondo figlio execlp");
				exit(0);
			}
		}
	}
	close(fd[READ]);
	close(fd[WRITE]);
	for(i = 0; i < 2; i++)
		wait(&status);
	exit(1);
}
