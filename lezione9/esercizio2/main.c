#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

int a,b;

void signal_handler(int);

int main(int argc, char *argv[])
{
	signal(SIGUSR1,signal_handler);
	signal(SIGUSR2,signal_handler);
	signal(SIGINT,signal_handler);
	signal(SIGALRM,signal_handler);
	srand((unsigned)time(NULL));
	do
	{
		a=0;
		b=0;
		printf("Inserisci a: ");
		scanf("%d",&a);
		printf("Inserisci b: ");
		scanf("%d",&b);
		alarm(2);
		pause();
	}while(1);
	exit(0);
}

void signal_handler(int signo)
{
	int i;
	if(signo == SIGINT)
	{
		printf("\n");
		exit(1);
	}
	if(signo == SIGUSR1)
		printf("Il risultato di %d - %d è: %d\n",a,b,a-b);
	else if(signo == SIGUSR2)
		printf("Il risultato di %d + %d è: %d\n",a,b,a+b);
	else
	{
		i = rand() % 2;
		if(i == 0)
			raise(SIGUSR1);
		else if(i == 1)
			raise(SIGUSR2);
	}
}
