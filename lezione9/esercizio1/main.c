#include<stdio.h>
#include<signal.h>

void signal_handler(int);

int main(int argc, char *argv[])
{
	signal(SIGINT,signal_handler);
	while(1);
	return 1;
}

void signal_handler(int signo)
{
	printf("Non voglio terminare\n");
}
