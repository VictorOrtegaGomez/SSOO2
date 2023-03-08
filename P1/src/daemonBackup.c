#define _POSIX_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include "constants.h"

int getCurrentTime (char *bufDate);
int backup (const char *path, const char *backupDirectory);
void signalHandler (int sig);

int main(int argc, char* argv[]){

    signal(SIGALRM, signalHandler);
    while (1){
        alarm(TIME_FOR_BACKUP);
        pause();
    }
}

int getCurrentTime (char* bufDate){

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

    sprintf(bufDate, "%d-%d-%d-%d:%d:%d", localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

    return 1;
}

int backup (const char *path, const char *backupDirectory){
	DIR *dp;
	struct dirent *node;
	struct stat statBuf, statBufAux;
	char name[MAX_PATH];
    char newName[MAX_PATH];

	if(stat(path, &statBuf) == -1){
		fprintf(stderr, "Error. %s could not be open\n", path);
        return 0;
	}

	if ((dp = opendir(path)) == NULL) {
		fprintf(stderr, "Error. Directory %s could not be open\n", path);
        return 0;
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
                return 0;   
			}
			
		}		
	}

	closedir(dp);
    return 1;
}

void signalHandler(int sig){
    
    struct stat statBuf;
    char date[MAX_DATE];
    char backupPath[MAX_PATH + MAX_DATE];
    char aux[MAX_PATH];
    char *currentDirectory;

    signal(SIGALRM, signalHandler);

    currentDirectory = getcwd(NULL, 0); /*We get the path of the current directory which is where the project will be located*/

    if(stat(currentDirectory, &statBuf) == -1){
        fprintf(stderr, "Error. %s could not be open\n", currentDirectory);
    }

    if(!S_ISDIR(statBuf.st_mode)){
        fprintf(stderr, "Error. %s is not a directory\n", currentDirectory);
    }

    if(getCurrentTime(date) == -1){ /*We get current time*/
        fprintf(stderr, "Error. Current date could not be obtained\n");
    }

    sprintf(aux, "%s", getenv("HOME"));
    sprintf(backupPath, "%s/%s", aux, date); /*We create the path of the backup directory which will be named the date we've just got*/

    if(mkdir(backupPath, 0777) == -1){
        fprintf(stderr, "Error. Directory %s could not be created\n", backupPath);
    }

    fprintf(stdout, "Starting backup procedure...\n");

	if(backup(currentDirectory, backupPath) == 0) fprintf(stderr, "Error. Backup procedure could not be completed\n");
    else fprintf(stdout, "Backup procedure completed\n");
    
}