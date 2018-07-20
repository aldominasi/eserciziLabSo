#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

int m,n,p;
int **matrice_a,**matrice_b,**matrice_c;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void  *calculate_matrix(void *);
void libera_memoria();

int main()
{
	int i,j;
	pthread_t *threads;
	int *dati_threads;
	atexit(libera_memoria);
	srand((unsigned)time(NULL));
	printf("Inserisci il numero di righe della prima matrice: ");
	scanf("%d",&m);
	printf("Inserisci il numero di colonne della prima matrice: ");
	scanf("%d",&n);
	printf("Inserisci il numero di colonne della seconda matrice che ha %d numero di righe: ",n);
	scanf("%d",&p);
	matrice_a = (int **)malloc(m * sizeof(int*));
	for(i=0;i<m;i++)
	{
		matrice_a[i] = (int *)malloc(n * sizeof(int));
		for(j=0;j<n;j++)
			matrice_a[i][j] = rand()%20;
	}
	matrice_b = (int **)malloc(n * sizeof(int*));
	for(i=0;i<n;i++)
	{
		matrice_b[i] = (int *)malloc(p * sizeof(int));
		for(j=0;j<p;j++)
			matrice_b[i][j] = rand()%20;
	}
	matrice_c = (int **) malloc(m * sizeof(int*));
	for(i=0;i<m;i++)
	{
		matrice_c[i] = (int *) malloc(p * sizeof(int));
		for(j=0;j<p;j++)
			matrice_c[i][j] = 0;
	}
	threads = (pthread_t *)malloc(m * sizeof(pthread_t));
	dati_threads = (int *)malloc(m * sizeof(int));
	for(i=0;i<m;i++)
	{
		dati_threads[i] = i;
		if(pthread_create(&threads[i],NULL,calculate_matrix,&dati_threads[i]))
		{
			printf("Errore pthread_create\n");
			free(threads);
			free(dati_threads);
			exit(0);
		}
	}
	for(i=0;i<m;i++)
	{
		if(pthread_join(threads[i],NULL))
		{
			printf("Errore pthread_join\n");
			free(threads);
			free(dati_threads);
			exit(0);
		}
	}
	printf("***** MATRICE A *****\n");
	for(i=0;i<m;i++)
	{
		printf("[ ");
		for(j=0;j<n;j++)
			printf("%d ",matrice_a[i][j]);
		printf("]\n");
	}
	printf("***** MATRICE B *****\n");
	for(i=0;i<n;i++)
	{
		printf("[ ");
		for(j=0;j<p;j++)
			printf("%d ",matrice_b[i][j]);
		printf("]\n");
	}
	sleep(2);
	printf("***** MATRICE C *****\n");
	for(i=0;i<m;i++)
	{
		printf("[ ");
		for(j=0;j<p;j++)
		{
			printf("%d ",matrice_c[i][j]);
		}
		printf("]\n");
	}
	free(threads);
	free(dati_threads);
	exit(1);
}

void *calculate_matrix(void *arg)
{
	int *index,i,j;
	index = (int *)arg;
	for(j=0;j<p;j++)
		for(i=0;i<n;i++)
			matrice_c[*index][j] += matrice_a[*index][i] * matrice_b[i][j];
	pthread_exit(NULL);
}

void libera_memoria()
{
	int i;
	for(i=0;i<m;i++)
	{
		free(matrice_a[i]);
		free(matrice_c[i]);
	}
	free(matrice_a);
	free(matrice_c);
	for(i=0;i<n;i++)
		free(matrice_b[i]);
	free(matrice_b);
}
