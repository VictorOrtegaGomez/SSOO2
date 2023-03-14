#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX_PATH 4098

void deleteFile (const char *path){
	DIR *dp;
	struct dirent *node;
	struct stat statBuf, statBufAux;
	char name[MAX_PATH];

	if(stat(path, &statBuf) == -1){
		fprintf(stderr, "Error. %s could not be open\n", path);
		exit(EXIT_FAILURE);
	}

	if(S_ISREG(statBuf.st_mode)){
		if(remove(path) == -1){
			fprintf(stderr, "Error. File %s could not be deleted\n", path);
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}

	if ((dp = opendir(path)) == NULL) {
		fprintf(stderr, "Error. Directory %s could not be open\n", path);
		exit(EXIT_FAILURE);
	}

	while ((node = readdir(dp)) != NULL){
		sprintf(name, "%s/%s", path, node->d_name);

		if(strcmp(node->d_name, ".") == 0 || strcmp(node->d_name, "..") == 0) continue;

		if(stat(name, &statBufAux) == -1) fprintf(stderr, "Error. %s could not be open\n", path);

		if(S_ISDIR(statBufAux.st_mode)) deleteFile(name);
		else{
			if(remove(name) == -1){
				fprintf(stderr, "Error. File %s could not be deleted\n", name);
				exit(EXIT_FAILURE);
			}
			
		}		
		
	}

	if(rmdir(path) == -1){
		fprintf(stderr, "Error. Directory %s could not be deleted\n", path);
		closedir(dp);
		exit(EXIT_FAILURE);
	}

	closedir(dp);
}

int main(int argc, char*argv[]){

    if(argc != 2){
        printf("Error. Only directory or file name must be introduced. Number of arguments: %d\n", argc);
		printf("%s\n%s\n%s\n", argv[0], argv[1], argv[2]);
        exit(EXIT_FAILURE);
    }

	deleteFile(argv[1]);
	
	exit(EXIT_SUCCESS);
}