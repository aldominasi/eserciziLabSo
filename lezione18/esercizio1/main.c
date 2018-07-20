#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>
#include<errno.h>
#include<fcntl.h>

sem_t mutex_lettura, mutex_scrittura;
char file[]  = "prova.txt";
int n;

void *function_thread(void *);
void uscita();

int main(int argc, char *argv[])
{
	int i,fd;
	pthread_t thread;
	char dato_scritto[10];
	int number;
	if(argc != 2)
	{
		printf("Utilizzo: %s <number>",argv[0]);
		exit(1);
	}
	n = atoi(argv[1]);
	if(sem_init(&mutex_scrittura,0,1) == -1)
	{
		perror("Errore sem_init");
		exit(0);
	}
	if(sem_init(&mutex_lettura,0,0) == -1)
	{
		perror("Errore sem_init");
		exit(0);
	}
	atexit(uscita);
	if((fd = open(file,O_WRONLY)) == -1)
	{
		perror("Errore open");
		exit(0);
	}
	if(pthread_create(&thread,NULL,function_thread,NULL))
	{
		printf("Errore pthread_create\n");
		exit(0);
	}
	for(i=0;i<n;i++)
	{
		sem_wait(&mutex_scrittura);
		write(fd,&i,sizeof(int));
		lseek(fd,0L,SEEK_SET);
		sem_post(&mutex_lettura);
	}
	close(fd);
	if(pthread_join(thread,NULL))
	{
		printf("Errore pthread_join\n");
		exit(0);
	}
	exit(1);
}

void *function_thread(void *arg)
{
	int i;
	int fd,number;
	if((fd = open(file,O_RDONLY)) == -1)
		pthread_exit(NULL);
	for(i=0;i<n;i++)
	{
		sem_wait(&mutex_lettura);
		while(read(fd,&number,sizeof(int)) > 0);
		printf("%d\n",number);
		lseek(fd,0L,SEEK_SET);
		sem_post(&mutex_scrittura);
	}
	close(fd);
	pthread_exit(NULL);
}

void uscita()
{
	sem_destroy(&mutex_lettura);
	sem_destroy(&mutex_scrittura);
}
