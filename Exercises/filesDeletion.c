#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX_PATH 256

void deleteFile (const char *path){
	DIR *dp;
	struct dirent *node;
	struct stat statBuf;
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

	while (statBuf.st_nlink != 0 && (node = readdir(dp)) != NULL){
		sprintf(name, "%s/%s", path, node->d_name);

		if(strcmp(node->d_name, ".") == 0 || strcmp(node->d_name, "..") == 0) continue;

		if(S_ISDIR(statBuf.st_mode)) deleteFile(name);

		if(remove(name) == -1){
			fprintf(stderr, "Error. File %s could not be deleted\n", name);
			exit(EXIT_FAILURE);
		}

		statBuf.st_nlink--;		
	}
}

int main(int argc, char*argv[]){

    if(argc != 2){
        printf("Error. Directory or file name must be introduced.");
        exit(EXIT_FAILURE);
    }

	deleteFile(argv[1]);
	
	exit(EXIT_SUCCESS);
}