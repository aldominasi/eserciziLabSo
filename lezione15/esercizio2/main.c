#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<pthread.h>
#include<fcntl.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/stat.h>
#include<string.h>
struct dati
{
	char file[10];
	int n;
	off_t nbytes;
	int i;
};

void *thread_function(void *);

int main(int argc, char *argv[])
{
	struct dati dati_thread;
	char file[] = "testo.txt";
	int i,count=0,fd;
	struct stat info_file;
	pthread_t *threads;
	void *result;
	if(argc != 2)
	{
		printf("Utilizzo: %s <number>\n",argv[0]);
		exit(1);
	}
	strcpy(dati_thread.file,file);
	dati_thread.n = atoi(argv[1]);
	if((fd = open(file,O_RDONLY)) == -1)
	{
		perror("open");
		exit(0);
	}
	if(fstat(fd,&info_file) == -1)
	{
		perror("fstat");
		close(fd);
		exit(0);
	}
	dati_thread.nbytes = info_file.st_size;
	close(fd);
	if((threads = (pthread_t *) malloc(dati_thread.n * sizeof(pthread_t))) == NULL)
	{
		perror("malloc");
		exit(0);
	}
	for(i=0;i<dati_thread.n;i++)
	{
		dati_thread.i = i;
		if(pthread_create(&threads[i],NULL,thread_function,&dati_thread))
		{
			printf("Errore pthread_create\n");
			exit(0);
		}
	}
	for(i=0;i<dati_thread.n;i++)
	{
		if(pthread_join(threads[i],&result))
		{
			printf("Errore join\n");
			exit(0);
		}
		count += (int)result;
		printf("%d\n",(int)result);
	}
	printf("Il numero di parole è: %d\n",count);
	exit(0);
}

void *thread_function(void *arg)
{
	int fd,num_parole=0,da_leggere,i;
	char parola;
	off_t seek;
	struct dati *argument = (struct dati *) arg;
	da_leggere = argument->nbytes / argument->n;
	fd = open(argument->file,O_RDONLY);
	lseek(fd,da_leggere * argument->i, SEEK_SET);
	if(argument->i == (argument->n-1))
		while(read(fd,&parola,1) > 0)
			num_parole++;
	else
		while(read(fd,&parola,1) < da_leggere)
			num_parole++;
	pthread_exit((void *)num_parole);
}
