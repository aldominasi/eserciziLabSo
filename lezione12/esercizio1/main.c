#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>
#include<fcntl.h>
#include<sys/stat.h>

int somma=0, fd[2];

void signal_handler(int);

int main(int argc, char *argv[])
{
	int i,n, numero, status;
	pid_t pid[2];
	char pathdispari[] = "fifodispari", pathpari[] = "fifopari";
	if(mkfifo(pathdispari,0777) != 0)
	{
		if(errno != EEXIST)
		{
			perror("Padre Errore creazione fifo");
			exit(0);
		}
	}
	if(mkfifo(pathpari,0777) != 0)
	{
		if(errno  != EEXIST)
		{
			perror("Padre Errore creazione fifo");
			kill(0,SIGKILL);
			exit(0);
		}
	}
	for(i=0;i<2;i++)
	{
		if((pid[i] = fork()) < 0)
		{
			perror("Errore fork");
			exit(0);
		}
		if(pid[i] == 0) //figli
		{
			signal(SIGUSR1,signal_handler);
			if(i==0) //primo figlio dispari
			{
				if((fd[0] = open(pathdispari,O_RDONLY)) < 0)
				{
					perror("Padre Errore open");
					kill(0,SIGKILL);
					exit(0);
				}
			}
			else
			{
				if((fd[0] = open(pathpari,O_RDONLY)) < 0)
				{
					perror("Padre Errore open");
					kill(0,SIGKILL);
					exit(0);
				}
			}
			for(;;)
			{
				read(fd[0],&numero,sizeof(int));
					somma += numero;
			}
		}
	}
	//padre
	if((fd[0] = open(pathdispari,O_WRONLY)) < 0)
	{
		perror("Padre Errore open");
		kill(0,SIGKILL);
		exit(0);
	}
	if((fd[1] = open(pathpari,O_WRONLY)) < 0)
	{
		perror("Padre Errore open");
		kill(0,SIGKILL);
		exit(0);
	}
	do
	{
		printf("Quanti numeri vuoi inserire? ");
		scanf("%d",&n);
	} while(n <= 0);
	for(i=0;i<n;i++)
	{
		printf("Inserisci il numero: ");
		scanf("%d",&numero);
		if((numero % 2) == 0)
			write(fd[1],&numero,sizeof(int));
		else
			write(fd[0],&numero,sizeof(int));
	}
	for(i=0;i<2;i++)
	{
		kill(pid[i],SIGUSR1);
		wait(&status);
		if(WIFEXITED(status))
			somma += WEXITSTATUS(status);
	}
	close(fd[0]);
	close(fd[1]);
	printf("La somma totale è: %d\n",somma);
	exit(1);
}

void signal_handler(int signo)
{
	close(fd[0]);
	exit(somma);
}
