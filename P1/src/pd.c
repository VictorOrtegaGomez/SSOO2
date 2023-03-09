#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "constants.h"

void deleteFile (const char *path);

int main(int argc, char* argv[]){

    if(argc != 2){
        fprintf(stderr, "Error. Only file name must be introduced\n");
        exit(EXIT_FAILURE);
    }

    deleteFile(argv[1]);

    exit(EXIT_SUCCESS);
}

/*Function that will delete the file introduced. If path is a directory it will delete every file from it and then said directory*/

void deleteFile (const char *path){
	DIR *dp;
	struct dirent *node;
	struct stat statBuf, statBufAux;
	char name[MAX_PATH];

    /*First we'll see if the path is a directory*/

	if(stat(path, &statBuf) == -1){
		fprintf(stderr, "Error. %s could not be open\n", path);
		exit(EXIT_FAILURE);
	}

    /*If we're dealing with a file and not a directory we'll delete it*/

	if(S_ISREG(statBuf.st_mode)){
		if(remove(path) == -1){
			fprintf(stderr, "Error. File %s could not be deleted\n", path);
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}

    /*If it is a directory we'll read what it contains. If the node we're reading is a file it'll be deleted, otherwise we'll call this funtion again*/

	if ((dp = opendir(path)) == NULL) {
		fprintf(stderr, "Error. Directory %s could not be open\n", path);
		exit(EXIT_FAILURE);
	}

	while ((node = readdir(dp)) != NULL){
		sprintf(name, "%s/%s", path, node->d_name);

		if(strcmp(node->d_name, ".") == 0 || strcmp(node->d_name, "..") == 0) continue;

		if(stat(name, &statBufAux) == -1) fprintf(stderr, "Unable to open %s It could be a symbolic link, if so deletion is granted\n", name);

		if(S_ISDIR(statBufAux.st_mode)) deleteFile(name);
		else{
			if(remove(name) == -1){
				fprintf(stderr, "Error. File %s could not be deleted\n", name);
				exit(EXIT_FAILURE);
			}
			
		}		
		
	}

    /*Once it is over deleting everything from the directory we'll delete the own directory*/

	if(rmdir(path) == -1){
		fprintf(stderr, "Error. Directory %s could not be deleted\n", path);
		closedir(dp);
		exit(EXIT_FAILURE);
	}

	closedir(dp);
}