#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXIMUM_LINE_SIZE 100
#define MAXIMUM_NAME_SIZE 4098
#define MAIN_DIRECTORY_NAME "estudiantes"
#define TXT_NAME "result.txt"
#define RESULT_FILE_PERMISSIONS 0644
#define RESULT_SENTENCE "La nota que debes obtener en este nuevo examen para superar la prueba es "
#define READING 0
#define WRITING 1

void writeResultFiles(int writingPipePointer, const char *fileName){

    DIR *dp;
    FILE *file;
    int resultFilePointer, i;
    char line[MAXIMUM_LINE_SIZE];
    char directoryName[MAXIMUM_NAME_SIZE];
    char resultFileName[MAXIMUM_NAME_SIZE];
    char resultSentence[sizeof(RESULT_SENTENCE)];
    int newMark;
    char newMarkChar[2];
    char *token;

    /*We open the file*/
    if((file = fopen(fileName, "r")) == NULL){
        fprintf(stderr,"Error. File could not be open\n");
        exit(EXIT_FAILURE);
    }

    /*File reading by lines*/
    while (fgets(line, MAXIMUM_LINE_SIZE, file))
    {
        /*We open the directories from the students with the first word from the lines we read*/
        token = strtok(line, " ");
        sprintf(directoryName, "%s/%s", MAIN_DIRECTORY_NAME, token);

        /*We check that the folders exist*/
        if ((dp = opendir(directoryName)) == NULL) {
            fprintf(stderr, "Error. Directory %s does not exist\n", MAIN_DIRECTORY_NAME);
        }else{
            /*We get the mark*/
            for(i = 0; i < 2; i++) token = strtok(NULL, " ");

            /*We comunicate the old mark to the manager process*/
            if(write(writingPipePointer, token, sizeof(token)) != sizeof(token)){
                fprintf(stderr, "Error. Pipe could not be written\n");
            }

            /*We calculate the mark is needed*/
            newMark = 10 - atoi(token);
            printf("mark%d\n",newMark);

            /*We create the file where the mark needed will be written*/
            strcpy(resultFileName, directoryName);
            strcat(resultFileName, "/");
            strcat(resultFileName, TXT_NAME);

            if((resultFilePointer = open(resultFileName, O_WRONLY | O_CREAT | O_TRUNC, RESULT_FILE_PERMISSIONS)) == -1){
                fprintf(stderr, "Error. File %s could not be created\n", resultFileName);
            }

            /*We write on the file*/
            sprintf(newMarkChar, "%d", newMark);
            strcpy(resultSentence, RESULT_SENTENCE);
            strcat(resultSentence, newMarkChar);
            
            if(write(resultFilePointer, resultSentence, sizeof(resultSentence)) != sizeof(resultSentence)){
                fprintf(stderr, "Error. File %s could not be written\n", resultFileName);
            }
        }
        
    }

    fclose(file);
    closedir(dp);
    close(resultFilePointer);
}

int main(int argc, char* argv[]){

    /*It is checked that we have all the arguments we need. File name must be an argument*/
    if(argc != 2){
        fprintf(stderr,"Error. Only file name must be introduced\n");
        exit(EXIT_FAILURE);
    }

    writeResultFiles(atoi(argv[0]),argv[1]);

    exit(EXIT_SUCCESS);
}