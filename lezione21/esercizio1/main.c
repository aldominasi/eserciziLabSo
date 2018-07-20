#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>

int **matrice, *somme_parziali, n;

void *function_thread(void *);

int main(int argc, char *argv[])
{
	pthread_t *threads;
	int *argument, i, j, min;
	void *number;
	if(argc != 2)
	{
		printf("Utilizzo %s <number>\n",argv[0]);
		exit(1);
	}
	n = atoi(argv[1]);
	srand((unsigned)time(NULL));
	if((matrice = (int **)malloc(n * sizeof(int *))) == NULL)
	{
		perror("Errore malloc 1");
		exit(0);
	}
	for(i=0;i<n;i++)
	{
		matrice[i] = (int *)malloc(n*sizeof(int));
		for(j=0;j<n;j++)
			matrice[i][j] = rand()%256;
	}
	if((somme_parziali = (int *)malloc(n*sizeof(int))) == NULL)
	{
		perror("Errore malloc 2");
		exit(0);
	}
	if((threads = (pthread_t *)malloc(n * sizeof(pthread_t))) == NULL)
	{
		perror("Errore malloc 3");
		exit(0);
	}
	if((argument = (int *)malloc(n*sizeof(int))) == NULL)
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
	min =  somme_parziali[0];
	for(i=1;i<n;i++)
	{
		if(min > somme_parziali[i])
			min = somme_parziali[i];
	}
	printf("Il minimo della matrice è: %d\n",min);
	free(threads);
	free(somme_parziali);
	free(argument);
	for(i=0;i<n;i++)
		free(matrice[i]);
	free(matrice);
	exit(1);
}

void *function_thread(void *arg)
{
	int *riga, tot=0, col;
	riga = (int *)arg;
	if(*riga % 2 == 0) //pari
		for(col=0;col<n;col+=2)
			tot += matrice[*riga][col];
	else
		for(col=1;col<n;col+=2)
			tot += matrice[*riga][col];
	somme_parziali[*riga] = tot;
	printf("tot: %d\n",tot);
	pthread_exit(NULL);
}
