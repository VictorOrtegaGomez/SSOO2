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
    char backupPath[MAX_PATH + MAX_DATE];
    char aux[MAX_PATH];
    char *currentDirectory;

    currentDirectory = getcwd(NULL, 0); /*We get the path of the current directory which is where the project will be located*/

    if(stat(currentDirectory, &statBuf) == -1){
        fprintf(stderr, "Error. %s could not be open\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if(!S_ISDIR(statBuf.st_mode)){
        fprintf(stderr, "Error. %s is not a directory\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if(getCurrentTime(date) == -1){ /*We get current time*/
        fprintf(stderr, "Error. Current date could not be obtained\n");
        exit(EXIT_FAILURE);
    }

    sprintf(aux, "%s", getenv("HOME"));
    sprintf(backupPath, "%s/%s", aux, date); /*We create the path of the backup directory which will be named the date we've just got*/

    if(mkdir(backupPath, 0777) == -1){
        fprintf(stderr, "Error. Directory %s could not be created\n", argv[1]);
        exit(EXIT_FAILURE);
    }

	backup(currentDirectory, backupPath);
	
	exit(EXIT_SUCCESS);
}