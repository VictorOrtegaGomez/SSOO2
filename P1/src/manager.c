#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define FILE_NAME "estudiantes.txt"

int main(int argc, char*argv[]){
    
    pid_t pidA;
    int statusA;

    /*Creation of the child process A*/
    if((pidA = fork()) < 0) {
        fprintf(stderr, "Error. The child process A could not be created\nTerminating program\n");
        exit(EXIT_FAILURE);
    } else if(pidA == 0) {
        execl("./exec/pa", argv[0], FILE_NAME, NULL);
        fprintf(stderr, "Error. The child process A could not be executed\nTerminating program\n");
        exit(EXIT_FAILURE);
    }

    /*We wait for the child process A to finish*/
    if(waitpid(pidA, &statusA, 0) == -1){
        fprintf(stderr, "Error. Waiting for child proccess A failed\nTerminating program\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}