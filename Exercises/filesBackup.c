#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

#define MAX_PATH 4098
#define MAX_DATE 70

int getCurrentTime (char* fecha){

    time_t currentTime;
    struct tm *localTime;

    if((currentTime = time(NULL)) == -1){
        fprintf(stderr, "Error. Current time could not be obtained\n");
        return(-1);
    }

    if((localTime = localtime(&currentTime)) == NULL){
        fprintf(stderr, "Error. Local time could not be obtained\n");
        return(-1);
    }

    sprintf(fecha, "%d-%d-%d", localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900);

    return 1;
}

void backup (const char *path, const char *backupDirectory){
	DIR *dp;
	struct dirent *node;
	struct stat statBuf, statBufAux;
	char name[MAX_PATH];
    char newName[MAX_PATH];

	if(stat(path, &statBuf) == -1){
		fprintf(stderr, "Error. %s could not be open\n", path);
		exit(EXIT_FAILURE);
	}

	if ((dp = opendir(path)) == NULL) {
		fprintf(stderr, "Error. Directory %s could not be open\n", path);
		exit(EXIT_FAILURE);
	}

	while ((node = readdir(dp)) != NULL){
		sprintf(name, "%s/%s", path, node->d_name);
        sprintf(newName, "%s/%s", backupDirectory, node->d_name);

		if(strcmp(node->d_name, ".") == 0 || strcmp(node->d_name, "..") == 0) continue;

		if(stat(name, &statBufAux) == -1) fprintf(stderr, "Error. %s could not be open\n", path);

		if(S_ISDIR(statBufAux.st_mode)){
            mkdir(newName, statBufAux.st_mode);
            backup(name, newName);
        } 
		else{
			if(link(name, newName) == -1){
				fprintf(stderr, "Error. File %s could not be copied\n", name);
				exit(EXIT_FAILURE);
			}
			
		}		
	}

	closedir(dp);
}

int main(int argc, char*argv[]){
    
    struct stat statBuf;
    char date[MAX_DATE];

    if(argc != 2){
        printf("Error. Only directory name must be introduced. Number of arguments: %d\n", argc);
		printf("%s\n%s\n%s\n", argv[0], argv[1], argv[2]);
        exit(EXIT_FAILURE);
    }

    if(stat(argv[1], &statBuf) == -1){
        fprintf(stderr, "Error. %s could not be open\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if(!S_ISDIR(statBuf.st_mode)){
        fprintf(stderr, "Error. %s is not a directory\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if(getCurrentTime(date) == -1){
        fprintf(stderr, "Error. Current date could not be obtained\n");
        exit(EXIT_FAILURE);
    }

    if(mkdir(date, 0777) == -1){
        fprintf(stderr, "Error. Directory %s could not be created\n", argv[1]);
        exit(EXIT_FAILURE);
    }

	backup(argv[1], date);
	
	exit(EXIT_SUCCESS);
}