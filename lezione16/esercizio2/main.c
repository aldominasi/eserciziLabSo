#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<time.h>

#define N 3
#define THOUSAND 1000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int count = 0;

void *sum_thousand_number(void *);
void *wait_thousand_sum(void *);

int main()
{
	int i;
	pthread_t threads[N];
	srand((unsigned)time(NULL));
	for(i=0;i<N-1;i++)
	{
		if(pthread_create(&threads[i],NULL,sum_thousand_number,NULL))
		{
			printf("Errore pthread_create\n");
			exit(0);
		}
	}
	if(pthread_create(&threads[2],NULL,wait_thousand_sum,NULL))
	{
		printf("Errore pthread_create\n");
		exit(0);
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

void *sum_thousand_number(void *arg)
{
	int a=0;
	int i;
	for(i=0;i<THOUSAND;i++)
	{
		pthread_mutex_lock(&mutex);
		a += rand()%500;
		count++;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

void *wait_thousand_sum(void *arg)
{
	pthread_mutex_lock(&mutex);
	while(count < (THOUSAND + THOUSAND))
	{
		printf("count: %d\n",count);
		pthread_cond_wait(&cond,&mutex);
	}
	pthread_mutex_unlock(&mutex);
	printf("count: %d\n",count);
	pthread_exit(NULL);
}
