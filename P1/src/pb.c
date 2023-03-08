#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "constants.h"

void copyExamFiles(const char *fileName){

    DIR *dp;
    FILE *file;
    char line[MAXIMUM_LINE_SIZE];
    char directoryName[MAXIMUM_NAME_SIZE];
    char examName[MAXIMUM_NAME_SIZE];
    char *token;

    /*We open the file*/
    if((file = fopen(fileName, "r")) == NULL){
        fprintf(stderr,"Error. File could not be open\n");
        exit(EXIT_FAILURE);
    }

    /*File reading by lines*/
    while (fgets(line, MAXIMUM_LINE_SIZE, file))
    {
        /*We create the directories with the first word from the lines we read*/
        token = strtok(line, " ");
        sprintf(directoryName, "%s/%s", MAIN_DIRECTORY_NAME, token);

        /*We check that the folders exist*/
        if ((dp = opendir(directoryName)) == NULL) {
            fprintf(stderr, "Error. Directory %s does not exist\n", MAIN_DIRECTORY_NAME);
        }else{
            /*We copy the exam files*/
            token = strtok(NULL, " ");

            switch (*token){
                case 'A':
                    strcpy(examName, directoryName);
                    strcat(examName, "/");
                    strcat(examName, EXAM_A_NAME);

                    if(symlink(EXAM_A_PATH, examName) == -1){
                        fprintf(stderr, "Error. File %s could not be copied\n", EXAM_A_NAME);
                        exit(EXIT_FAILURE);
                    }

                    break;

                case 'B':
                    strcpy(examName, directoryName);
                    strcat(examName, "/");
                    strcat(examName, EXAM_B_NAME);

                    if(symlink(EXAM_B_PATH, examName) == -1){
                        fprintf(stderr, "Error. File %s could not be copied\n", EXAM_B_NAME);
                        exit(EXIT_FAILURE);
                    }
                    break;

                case 'C':
                    strcpy(examName, directoryName);
                    strcat(examName, "/");
                    strcat(examName, EXAM_C_NAME);

                    if(symlink(EXAM_C_PATH, examName) == -1){
                        fprintf(stderr, "Error. File %s could not be copied\n", EXAM_C_NAME);
                        exit(EXIT_FAILURE);
                    }
                    break;

                default:
                    fprintf(stderr, "Error. Exam type %s is not valid\n", token);
                    break;
            }            
        }
        
    }

    fclose(file);
    closedir(dp);
}

int main(int argc, char*argv[]){

    /*It is checked that we have all the arguments we need. File name must be an argument*/
    if(argc != 2){
        fprintf(stderr,"Error. Only file name must be introduced\n");
        exit(EXIT_FAILURE);
    }

    copyExamFiles(argv[1]);

    exit(EXIT_SUCCESS);    
}