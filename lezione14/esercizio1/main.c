#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

int number;
pid_t pid[2];

void cubo(int);
void saluta(int);
void reciproco(int);

int main(int argc, char *argv[])
{
	int i,status[2];
	if(argc != 2)
	{
		printf("Utilizzo: %s <number>\n",argv[0]);
		exit(1);
	}
	number = atoi(argv[1]);
	for(i=0;i<2;i++)
	{
		if((pid[i] = fork()) < 0)
		{
			perror("Errore fork");
			kill(0,SIGKILL);
			exit(0);
		}
		if(pid[i] == 0) //figli
		{
			if(i == 0) //figlio A
			{
				signal(SIGUSR1,cubo);
				signal(SIGUSR2,saluta);
			}
			else //figlio B
			{
				signal(SIGUSR1,saluta);
				signal(SIGUSR2,reciproco);
			}
			for(;;){}
		}
	}
	signal(SIGUSR1,SIG_IGN);
	signal(SIGUSR2,SIG_IGN);
	sleep(2);
	if((number%2) == 0) //pari
		kill(0,SIGUSR1);
	else
		kill(0,SIGUSR2);
	for(i=0;i<2;i++)
		if(wait(&status[i]) < 0)
			perror("Errore wait");
	exit(1);
}

void cubo(int signo)
{
	int tot;
	printf("cubo\n");
	tot = number * number * number;
	printf("Sono il figlio A il cubo di %d è %d\n",number,tot);
	exit(0);
}

void saluta(int signo)
{
	if(signo == SIGUSR1)
	{
		printf("Ciao\n");
		exit(1);
	}
	else
	{
		signal(SIGUSR1,saluta);
	}
}

void reciproco(int signo)
{
	int tot;
	tot = -number;
	printf("Sono il figlio B l'opposto di %d è: %d\n",number,tot);
	sleep(number*2);
	kill(pid[0],SIGUSR1);
	exit(1);
}
