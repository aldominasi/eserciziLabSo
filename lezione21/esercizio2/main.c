#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>
#include<errno.h>

int trovato = 0, **matrice, n, numero;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *function_thread(void *);
void visualizza_matrice();

int main(int argc, char *argv[])
{
	int i,j;
	pthread_t *threads;
	int *argument;
	if(argc != 2)
	{
		printf("Utilizzo: %s <number>\n",argv[0]);
		exit(1);
	}
	n = atoi(argv[1]);
	srand((unsigned)time(NULL));
	printf("Inserisci il numero da trovare (0-19): ");
	scanf("%d",&numero);
	if((matrice = (int **)malloc(n*sizeof(int *))) == NULL)
	{
		perror("Errore malloc 1");
		exit(0);
	}
	for(i=0;i<n;i++)
	{
		if((matrice[i] = (int *)malloc(n*sizeof(int))) == NULL)
		{
			perror("Errore malloc 2");
			exit(0);
		}
		for(j=0;j<n;j++)
			matrice[i][j] = rand()%20;
	}
	visualizza_matrice();
	if((argument  = (int *)malloc(n*sizeof(int))) == NULL)
	{
		perror("Errore malloc 3");
		exit(0);
	}
	if((threads = (pthread_t *)malloc(n*sizeof(pthread_t))) == NULL)
	{
		perror("Errore malloc 4");
		exit(0);
	}
	for(i=0;i<n;i++)
	{
		argument[i] = i;
		if(pthread_create(&threads[i],NULL,function_thread,&argument[i]))
		{
			printf("Errore pthread_create\n");
			exit(0);
		}
	}
	for(i=0;i<n;i++)
	{
		if(pthread_join(threads[i],NULL))
		{
			printf("Errore pthread_join\n");
			exit(0);
		}
	}
	free(argument);
	free(threads);
	for(i=0;i<n;i++)
		free(matrice[i]);
	free(matrice);
	exit(1);
}

void *function_thread(void *arg)
{
	int *riga, col;
	riga = (int*)arg;
	for(col=0;col<n && trovato == 0;col++)
	{
		pthread_mutex_lock(&mutex);
		if(matrice[*riga][col] == numero && trovato == 0)
		{
			trovato = 1;
			printf("Il numero è stato trovato in riga %d colonna %d\n",(*riga)+1,col+1);
		}
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

void visualizza_matrice()
{
	int i,j;
	for(i=0;i<n;i++)
	{
		printf("[");
		for(j=0;j<n;j++)
			printf("\t%d",matrice[i][j]);
		printf("]\n");
	}
}
