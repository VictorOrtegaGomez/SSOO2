#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define READING 0
#define WRITING 1
#define BUFFER_SIZE 100
#define PIPES_TO_CHAR 256
#define NUM_CHILD_PROCESSES 2
#define FATHER_MESSAGE "Food ready"

int main(int argc, char* argv[]){
    pid_t pids[NUM_CHILD_PROCESSES];
    int childrenPipe[2];
    int childFatherPipe[2];
    char bufferReading[BUFFER_SIZE];
    char childrenPipeWr[PIPES_TO_CHAR];
    char childrenPipeRd[PIPES_TO_CHAR];
    char childFatherPipeWr[PIPES_TO_CHAR];
    char childFatherPipeRd[PIPES_TO_CHAR];

    /*We create both pipes we'll be using*/
    pipe(childrenPipe);
    pipe(childFatherPipe);

    /*Pipes' pointers transformation into char*/
    sprintf(childrenPipeWr, "%d", childrenPipe[WRITING]);
    sprintf(childrenPipeRd, "%d", childrenPipe[READING]);
    sprintf(childFatherPipeRd, "%d", childFatherPipe[READING]);
    sprintf(childFatherPipeWr, "%d", childFatherPipe[WRITING]);

    if(argc != 1){
        fprintf(stdout, "Error. No arguments are needed\n");
    }

    /*Child processes' creation*/
    if((pids[0] = fork()) == -1){
        fprintf(stderr, "Error. Fork failed\n");
        exit(EXIT_FAILURE);
    }else if(pids[0] == 0){
        /*1st Child process*/
        execl("./ej15Child1", childFatherPipeRd, childrenPipeWr, NULL);
        fprintf(stderr, "Error. Exec failed\n");
        exit(EXIT_FAILURE);
    }

    if((pids[1] = fork()) == -1){
        fprintf(stderr, "Error. Fork failed\n");
        exit(EXIT_FAILURE);
    }else if(pids[1] == 0){
        /*2nd Child process*/
        execl("./ej15Child2", childFatherPipeWr, childrenPipeRd, NULL);
        fprintf(stderr, "Error. Exec failed\n");
        exit(EXIT_FAILURE);
    }

    /*Father will tell 1st child that food is ready. Afterwards it will sleep so he doesn't read its own message and then read 2nd child's message*/
    write(childFatherPipe[WRITING], FATHER_MESSAGE, sizeof(FATHER_MESSAGE));
    sleep(1);
    read(childFatherPipe[READING], bufferReading, BUFFER_SIZE);
    fprintf(stdout, "[FATHER] Children ready to eat\n");
    
    /*We close pipes' pointers*/
    close(childrenPipe[WRITING]);
    close(childrenPipe[READING]);
    close(childFatherPipe[WRITING]);
    close(childFatherPipe[READING]);

    exit(EXIT_SUCCESS);

}