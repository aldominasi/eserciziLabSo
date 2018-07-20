#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char * argv[])
{
	int fd_in, fd_out;
	int nread, nwrite;
	char buff;
	const char* inputfile = "testfile.txt";
	const char* outputfile = "output.txt";
	if((fd_in = open(inputfile,O_RDONLY)) == -1)
	{
		perror("open testfile.txt");
		return 1;
	}
	if((fd_out = open(outputfile,O_WRONLY | O_TRUNC)) == -1)
	{
		perror("open outputfile.txt");
		return 1;
	}
	if(dup2(fd_in,0) == -1)
	{
		perror("dup2 fd_in");
		return 1;
	}
	if(dup2(fd_out,1) == -1)
	{
		perror("dup2 fd_out");
		return 1;
	}
	while((nread = read(0,&buff,sizeof(char))) != 0)
	{
		if((nwrite = write(1,&buff,sizeof(char))) == -1)
		{
			perror("write");
			return 1;
		}
	}
	close(0);
	close(1);
	return 0;
}
