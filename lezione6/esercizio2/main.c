#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[]){
	off_t max = 0;
	DIR *dp;
	struct dirent *dirp;
	struct stat infofile,infolink;
	char path[100],buff[50];
	if(getcwd(path,sizeof(path)) == NULL)
	{
		printf("Errore getcwd\n");
		return 0;
	}
	if((dp = opendir(path)) == NULL)
	{
		printf("Errore opendir\n");
		return 0;
	}
	while((dirp = readdir(dp)) != NULL)
	{
		if(lstat(dirp->d_name,&infolink) < 0)
		{
			printf("Errore lstat\n");
		}
		else
		{
			if(S_ISLNK(infolink.st_mode))
			{
				if(stat(dirp->d_name,&infofile) < 0)
					printf("Errore stat\n");
				else if(max < infofile.st_size)
				{
					max = infofile.st_size;
					strcpy(buff,dirp->d_name);
					printf("%ld\n",infofile.st_size);
				}
			}
		}
	}
	if(max > 0)
		printf("%s\n",buff);
	closedir(dp);
	return 1;
}
