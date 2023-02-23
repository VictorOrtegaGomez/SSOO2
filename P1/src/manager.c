#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILE_NAME "estudiantes.txt"
#define LOG_FILE_NAME "log.txt"
#define LOG_FILE_PERMISSIONS 0644
#define FIRST_LOG_LINE "******** Log del sistema ********"
#define LAST_LOG_LINE "FIN DE PROGRAMA"
#define PROCESS_A_SUCCESS "Creación de directorios finalizada."
#define PROCESS_A_FAILURE "Error. Process A finished with errors\nFIN DE PROGRAMA"
#define PROCESS_B_SUCCESS "Copia de modelos de examen, finalizada."
#define PROCESS_B_FAILURE "Error. Process B finished with errors\nFIN DE PROGRAMA"
#define PROCESS_C_SUCCESS "Creación de archivos con nota necesaria para alcanzar la nota de corte, finalizada."
#define PROCESS_C_FAILURE "Error. Process C finished with errors\nFIN DE PROGRAMA"
#define READING 0
#define WRITING 1
#define WRITING_PIPE_SIZE 256

void writeFile(int filePointer, char *message){
    if(write(filePointer, message, sizeof(message)) != sizeof(message)){
        fprintf(stderr, "Error. File could not be written\n");
        exit(EXIT_FAILURE);
    }
}

int meanCalculation(int readingPipePointer){
    
    char mark[1];
    int mean = 0;
    int counter = 0;

    /*We read process C marks and calculate the mean*/

    do{
        if(read(readingPipePointer, mark, sizeof(mark)) < 0){
                fprintf(stderr, "Error. Pipe could not be read\nTerminating program\n");
                return(EXIT_FAILURE);
        }
        mean += atoi(mark);
        counter++;
    }while (*mark != EOF);

    return mean/counter;
}

int main(int argc, char*argv[]){

    pid_t pidA, pidB, pidC, pidBC;
    int statusA, statusBC, logFilePointer, mean, i;
    int pipePointers[2];
    char wrPipe[WRITING_PIPE_SIZE];

    /*Creating and opening of Log file*/
    if((logFilePointer = open(LOG_FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, LOG_FILE_PERMISSIONS)) == -1){
        fprintf(stderr, "Error. Log file could not be created\nTerminating program\n");
        exit(EXIT_FAILURE);
    }

    /*The first line of the log file is written*/
    writeFile(logFilePointer, FIRST_LOG_LINE);

    /*Creation of the child process A*/
    if((pidA = fork()) < 0) {
        writeFile(logFilePointer, "Error. The child process A could not be created\nTerminating program\n");
        exit(EXIT_FAILURE);
    } else if(pidA == 0) {
        execl("./exec/pa", argv[0], FILE_NAME, NULL);
        writeFile(logFilePointer, "Error. The child process A could not be executed\nTerminating program\n");
        exit(EXIT_FAILURE);
    }

    /*We wait for the child process A to finish*/
    if(waitpid(pidA, &statusA, 0) == -1){
        writeFile(logFilePointer, "Error. Waiting for child proccess A failed\nTerminating program\n");
        exit(EXIT_FAILURE);
    }

    /*We write the results of process A execution*/
    if(statusA == EXIT_SUCCESS){
        writeFile(logFilePointer, PROCESS_A_SUCCESS);
    } else {
        writeFile(logFilePointer, PROCESS_A_FAILURE);
    }

    /*Creation of child process B*/
    if((pidB = fork()) < 0) {
        writeFile(logFilePointer, "Error. The child process B could not be created\nTerminating program\n");
        exit(EXIT_FAILURE);
    } else if(pidB == 0) {
        execl("./exec/pb", argv[0], FILE_NAME, NULL);
        writeFile(logFilePointer, "Error. The child process B could not be executed\nTerminating program\n");
        exit(EXIT_FAILURE);
    }

    /*Comunication Pipe between manager and C proccess*/
    if(pipe(pipePointers) == -1){
        writeFile(logFilePointer, "Error. Pipe could not be created\nTerminating program\n");
        exit(EXIT_FAILURE);
    }
    sprintf(wrPipe, "%d", pipePointers[WRITING]);

    /*Creation of child process C*/
    if((pidC = fork()) < 0) {
        writeFile(logFilePointer, "Error. The child process C could not be created\nTerminating program\n");
        exit(EXIT_FAILURE);
    } else if(pidC == 0) {
        execl("./exec/pc", wrPipe, FILE_NAME, NULL);
        writeFile(logFilePointer, "Error. The child process C could not be executed\nTerminating program\n");
        exit(EXIT_FAILURE);
    }

    /*Mean calculation*/
    printf("Calculating mean...\n");
    if((mean = meanCalculation(pipePointers[READING])) == EXIT_FAILURE){
        writeFile(logFilePointer, "Error. Mean calculation failed\nTerminating program\n");
        exit(EXIT_FAILURE);
    }
    printf("Mean: %d\n", mean);
    
    /*We wait for the child processes B and C to finish*/
    for(i = 0; i < 2; i++){     

        if((pidBC = wait(&statusBC)) == -1){
            writeFile(logFilePointer, "Error. Waiting for child proccesses B and C failed\nTerminating program\n");
            exit(EXIT_FAILURE);
        }

        if(pidBC == pidB){
            if(statusBC == EXIT_SUCCESS) writeFile(logFilePointer, PROCESS_B_SUCCESS);
            else writeFile(logFilePointer, PROCESS_B_FAILURE);
        }else if(pidBC == pidC){
            if(statusBC == EXIT_SUCCESS) writeFile(logFilePointer, PROCESS_C_SUCCESS);
            else writeFile(logFilePointer, PROCESS_C_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}