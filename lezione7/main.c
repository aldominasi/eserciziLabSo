#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

int fibonacci(int);

int main(int argc, char *argv[])
{
	pid_t pid[2];
	int n, status_primo_figlio, status_secondo_figlio;
	int risultato_primo_figlio;
	do
	{
		printf("Inserisci n: ");
		scanf("%d",&n);
	}while(n < 0 && n > 12);
	if((pid[0] = fork()) < 0)
	{
		perror("Errore fork  primo figlio");
		exit(0);
	}
	if(pid[0] == 0) //figlio
	{
		status_primo_figlio = fibonacci(n);
		printf("Sono il processo figlio, il risultato di fibonacci(%d) è: %d\n",n,status_primo_figlio);
		exit(status_primo_figlio);
	}
	//padre
	if(wait(&status_primo_figlio) < 0)
		perror("Errore in attesa del primo figlio");
	else
	{
		if(WIFEXITED(status_primo_figlio))
			risultato_primo_figlio = WEXITSTATUS(status_primo_figlio);
		if(risultato_primo_figlio < 50) //crea un processo figlio che esegue ls -al
		{
			if((pid[1] = fork()) < 0)
			{
				perror("Errore fork secondo figlio");
				exit(0);
			}
			if(pid[1] == 0) //secondo figlio
			{
				if(execlp("ls","ls","-al","a.out",NULL) < 0)
					perror("Errore ls");
				exit(0);
			}
			if(wait(&status_secondo_figlio) < 0)
				perror("Errore in attesa del secondo figlio");
		}
		printf("Termino\n");
	}
	exit(0);
}

int fibonacci(int n)
{
	if(n < 0)
		return -1;
	if(n == 0)
		return 0;
	if( n == 1)
		return 1;
	return fibonacci(n-1) + fibonacci(n-2);
}
