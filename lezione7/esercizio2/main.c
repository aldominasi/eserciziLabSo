#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>

int main(int argc, char *argv[])
{
	pid_t pid[2];
	int seconds = 5, status_pid_uno, status_pid_due;
	if((pid[0] = fork()) < 0)
	{
		perror("Errore fork 1");
		exit(0);
	}
	if(pid[0] == 0) //figlio
	{
		if(execlp("ls","ls","-l",NULL) < 0)
			perror("Errore exec");
		exit(0);
	}
	//padre
	printf("Sono il padre e sono in attesa\n");
	if(wait(&status_pid_uno) < 0)
		printf("Errore in attesa del primo figlio\n");
	printf("Nuovo figlio\n");
	if((pid[1] = fork()) < 0)
	{
		perror("Errore fork 2");
		exit(0);
	}
	if(pid[1] == 0) //figlio
	{
		sleep(seconds);
		printf("Sono il processo %d, mio padre è il processo %d e sto terminando l'esecuzione\n",getpid(),getppid());
		exit(1);
	}
	//padre
	if(wait(&status_pid_due) < 0)
		printf("Errore in attesa del secondo figlio\n");
	printf("Il processo appena terminato è %d\n",pid[1]);
	exit(1);
}
