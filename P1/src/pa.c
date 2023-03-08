#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "constants.h"

void createDirectories(const char *fileName){

    FILE *file;
    char line[MAXIMUM_LINE_SIZE];
    char directoryName[MAXIMUM_NAME_SIZE];

    /*We open the file*/
    if((file = fopen(fileName, "r")) == NULL){
        fprintf(stderr,"Error. File could not be open\n");
        exit(EXIT_FAILURE);
    }

    /*File reading by lines*/
    while (fgets(line, MAXIMUM_LINE_SIZE, file))
    {
        /*We create the directories with the first word from the lines we read*/
        sprintf(directoryName, "%s/%s", MAIN_DIRECTORY_NAME, strtok(line, " "));

        if(mkdir(directoryName, 0777) == -1){
            fprintf(stderr, "Error. Directory %s could not be created. Perhaps it is an already existing one\n", line);
        }
    }

    fclose(file);
}

int main(int argc, char*argv[]){

    DIR *dp;

    /*It is checked that we have all the arguments we need. File name must be an argument*/
    if(argc != 2){
        fprintf(stderr,"Error. Only file name must be introduced\n");
        exit(EXIT_FAILURE);
    }

    if ((dp = opendir(MAIN_DIRECTORY_NAME)) == NULL) {
		/*We create the main directory which will have every other directory that will be created*/
        if(mkdir(MAIN_DIRECTORY_NAME, 0777) == -1){
            fprintf(stderr, "Error. Directory %s could not be created\n", MAIN_DIRECTORY_NAME);
            exit(EXIT_FAILURE);
        }
	}

    createDirectories(argv[1]);

    exit(EXIT_SUCCESS);    
}