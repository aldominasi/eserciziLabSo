#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

int glob = 0;

void *printglob(void *);

int main(int argc, char *argv[])
{
	int i,n;
	pthread_t *threads;
	srand((unsigned)time(NULL));
	if(argc != 2)
	{
		printf("Utilizzo: %s <number>\n",argv[0]);
		exit(1);
	}
	n = atoi(argv[1]);
	if((threads = (pthread_t *)malloc(n * sizeof(pthread_t))) == NULL)
	{
		printf("Errore allocazione memoria\n");
		exit(0);
	}
	for(i=0;i<n;i++)
	{
		if(pthread_create(&threads[i],NULL,printglob,NULL) != 0)
		{
			printf("Errore durante la creazione del thread %d\n",i+1);
			exit(0);
		}
	}
	for(i=0;i<n;i++)
		pthread_join(threads[i],NULL);
	exit(1);
}

void *printglob(void *arg)
{
	int seconds;
	seconds = rand()%10 + 1;
	sleep(seconds);
	glob++;
	printf("Sono il thread %ld e glob vale %d\n",pthread_self(),glob);
	pthread_exit(NULL);
}
