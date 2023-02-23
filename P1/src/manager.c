#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILE_NAME "estudiantes.txt"
#define LOG_FILE_NAME "log.txt"
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

int main(int argc, char*argv[]){

    pid_t pidA, pidB, pidC, pidBC;
    int statusA, statusBC, mean, counter, logFilePointer, i;
    int pipePointers[2];
    char wrPipe[WRITING_PIPE_SIZE];
    char *mark;

    /*Creating and opening of Log file*/
    if((logFilePointer = open(LOG_FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1){
        fprintf(stderr, "Error. Log file could not be created\nTerminating program\n");
        exit(EXIT_FAILURE);
    }

    /*The first line of the log file is written*/
    if(write(logFilePointer, FIRST_LOG_LINE, sizeof(FIRST_LOG_LINE)) != sizeof(FIRST_LOG_LINE)){
        fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
        exit(EXIT_FAILURE);
    }

    /*Creation of the child process A*/
    if((pidA = fork()) < 0) {
        fprintf(stderr, "Error. The child process A could not be created\nTerminating program\n");

        if(write(logFilePointer, stderr, sizeof(stderr)) != sizeof(stderr)){
            fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_FAILURE);
    } else if(pidA == 0) {
        execl("./exec/pa", argv[0], FILE_NAME, NULL);
        fprintf(stderr, "Error. The child process A could not be executed\nTerminating program\n");

        if(write(logFilePointer, stderr, sizeof(stderr)) != sizeof(stderr)){
            fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_FAILURE);
    }

    /*We wait for the child process A to finish*/
    if(waitpid(pidA, &statusA, 0) == -1){
        fprintf(stderr, "Error. Waiting for child proccess A failed\nTerminating program\n");

        if(write(logFilePointer, stderr, sizeof(stderr)) != sizeof(stderr)){
            fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_FAILURE);
    }

    /*We write the results of process A execution*/
    if(statusA == EXIT_SUCCESS){
        if(write(logFilePointer, PROCESS_A_SUCCESS, sizeof(PROCESS_A_SUCCESS)) != sizeof(PROCESS_A_SUCCESS)){
            fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
            exit(EXIT_FAILURE);
        }
    } else {
        if(write(logFilePointer, PROCESS_A_FAILURE, sizeof(PROCESS_A_FAILURE)) != sizeof(PROCESS_A_FAILURE)){
            fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
            exit(EXIT_FAILURE);
        }
    }

    /*Creation of child process B*/
    if((pidB = fork()) < 0) {
        fprintf(stderr, "Error. The child process B could not be created\nTerminating program\n");

        if(write(logFilePointer, stderr, sizeof(stderr)) != sizeof(stderr)){
            fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_FAILURE);
    } else if(pidB == 0) {
        execl("./exec/pb", argv[0], FILE_NAME, NULL);
        fprintf(stderr, "Error. The child process B could not be executed\nTerminating program\n");

        if(write(logFilePointer, stderr, sizeof(stderr)) != sizeof(stderr)){
            fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_FAILURE);
    }

    /*Comunication Pipe between manager and C proccess*/
    if(pipe(pipePointers) == -1){
        fprintf(stderr, "Error. Pipe could not be created\nTerminating program\n");

        if(write(logFilePointer, stderr, sizeof(stderr)) != sizeof(stderr)){
            fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_FAILURE);
    }

    sprintf(wrPipe, "%d", pipePointers[WRITING]);

    /*Creation of child process C*/
    if((pidC = fork()) < 0) {
        fprintf(stderr, "Error. The child process C could not be created\nTerminating program\n");

        if(write(logFilePointer, stderr, sizeof(stderr)) != sizeof(stderr)){
            fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_FAILURE);
    } else if(pidC == 0) {
        execl("./exec/pc", wrPipe, FILE_NAME, NULL);
        fprintf(stderr, "Error. The child process C could not be executed\nTerminating program\n");

        if(write(logFilePointer, stderr, sizeof(stderr)) != sizeof(stderr)){
            fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_FAILURE);
    }

    /*We read process C marks and calculate the mean*/
    counter = 0;
    mean = 0;
    mark = 0;

    while(*mark != EOF){
        if(read(pipePointers[READING], mark, sizeof(mark)) < 0){
                fprintf(stderr, "Error. Pipe could not be read\nTerminating program\n");

                if(write(logFilePointer, stderr, sizeof(stderr)) != sizeof(stderr)){
                    fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
                    exit(EXIT_FAILURE);
                }

                exit(EXIT_FAILURE);
        }
        mean += atoi(mark);
        counter++;
    }

    mean = mean/counter;
    
    /*We wait for the child processes B and C to finish*/
    for(i = 0; i < 2; i++){     
        if((pidBC = wait(&statusBC)) == -1){
            if(pidBC == pidB){
                if(statusBC == EXIT_SUCCESS){
                    if(write(logFilePointer, PROCESS_B_SUCCESS, sizeof(PROCESS_B_SUCCESS)) != sizeof(PROCESS_B_SUCCESS)){
                        fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
                        exit(EXIT_FAILURE);
                    }
                }else{
                    if(write(logFilePointer, PROCESS_B_FAILURE, sizeof(PROCESS_B_FAILURE)) != sizeof(PROCESS_B_FAILURE)){
                        fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
                        exit(EXIT_FAILURE);
                    }
                }
            } else if(pidBC == pidC){
                if(statusBC == EXIT_SUCCESS){
                    if(write(logFilePointer, PROCESS_C_SUCCESS, sizeof(PROCESS_C_SUCCESS)) != sizeof(PROCESS_C_SUCCESS)){
                        fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
                        exit(EXIT_FAILURE);
                    }
                }else{
                    if(write(logFilePointer, PROCESS_C_FAILURE, sizeof(PROCESS_C_FAILURE)) != sizeof(PROCESS_C_FAILURE)){
                        fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
                        exit(EXIT_FAILURE);
                    }
                }

            }else if(pidBC == -1){
                fprintf(stderr, "Error. Waiting for child proccesses B and C failed\nTerminating program\n");

                if(write(logFilePointer, stderr, sizeof(stderr)) != sizeof(stderr)){
                    fprintf(stderr, "Error. Log file could not be written\nTerminating program\n");
                    exit(EXIT_FAILURE);
                }
            } 
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}