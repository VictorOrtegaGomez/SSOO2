#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>

#define READING 0
#define WRITING 1
#define BUFFER_SIZE 100

int main(int argc, char*argv[]){
    pid_t pid;
    int fd[2];
    char bufferReading[BUFFER_SIZE];

    pipe(fd);

    if(argc != 2){
        fprintf(stderr, "Error. The number of arguments must be 2\n");
        exit(EXIT_FAILURE);
    }

    if((pid = fork()) == -1){
        fprintf(stderr, "Error creating child process\n");
        exit(EXIT_FAILURE);
    }else if(pid == 0){
        dup2(fd[WRITING], STDOUT_FILENO);
        if(execl("./ej13hijo", argv[1], NULL)==-1){
            fprintf(stderr, "Error in execl\n");
            exit(EXIT_FAILURE);
        }
    }else{
        read(fd[READING], bufferReading, sizeof(bufferReading));

        printf("El texto en mayúsculas es: %s\n", bufferReading);

        exit(EXIT_SUCCESS);
    }

}