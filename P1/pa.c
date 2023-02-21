#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAXIMUM_LINE_SIZE 100
#define MAIN_DIRECTORY_NAME "/estudiantes/"

void create_directory(char *name){
    DIR *dir;
    char *directory_name = strcat(MAIN_DIRECTORY_NAME, name);

    if((dir = opendir(directory_name)) == NULL){
        mkdir(directory_name, 0777);
    }
}

int main(int argc, char*argv[]){
    FILE *file;
    char line[MAXIMUM_LINE_SIZE];

    /*It is checked that we have all the arguments we need. File named must be an argument*/
    if(argc != 2){
        fprintf(stderr,"Error. File name must be introduced\n");
        exit(-1);
    }

    /*We open the file*/
    if((file = fopen(argv[1], "r")) == NULL){
        fprintf(stderr,"Error. File could not be open\n");
        exit(-1);
    }

    /*We create the main directory which will have every other directory that will be created*/
    create_directory(MAIN_DIRECTORY_NAME);

    /*File reading by lines*/
    while (fgets(line, MAXIMUM_LINE_SIZE, file))
    {
        create_directory(strtok(line, ""));
    }

    fclose(file);
    return 0;
}