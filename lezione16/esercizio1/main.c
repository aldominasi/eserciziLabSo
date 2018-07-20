#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#define ELEM_DA_TROVARE 221
#define N 4

int matrice[N][N] = {{2,5,3,6},{4,12,32,221},{1,90,9,0},{400,43,54,65}};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int trovato;

void *thread_function(void *);

int main(int argc, char * argv[])
{
	int i,index[N];
	pthread_t threads[N];
	trovato = 0;
	for(i=0;i<N;i++)
	{
		index[i] = i;
		if(pthread_create(&threads[i],NULL,thread_function,&index[i]))
		{
			printf("Errore pthread_create\n");
			exit(0);
		}
	}
	for(i=0;i<N;i++)
	{
		if(pthread_join(threads[i],NULL))
		{
			printf("Errore pthread_join\n");
			exit(0);
		}
	}
	exit(1);
}

void *thread_function(void *arg)
{
	int index;
	int j;
	index = *(int*)arg;
	printf("%d\n",index);
	j=0;
	while(j<N && trovato == 0)
	{
		pthread_mutex_lock(&mutex);
		if(matrice[index][j] == ELEM_DA_TROVARE)
		{
			trovato == 1;
			printf("L'elemento è stato trovato in riga %d colonna %d\n",index+1,j+1);
		}
		pthread_mutex_unlock(&mutex);
		j++;
	}
	pthread_exit(NULL);
}
