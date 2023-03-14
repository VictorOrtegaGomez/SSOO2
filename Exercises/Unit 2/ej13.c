#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define READING 0
#define WRITING 1
#define BUFFER_SIZE 100

int main(int argc, char*argv[]){
    pid_t pid;
    int childPipe[2];
    int fatherPipe[2];
    char bufferReading[BUFFER_SIZE];

    pipe(childPipe);
    pipe(fatherPipe);

    if(argc != 2){
        fprintf(stderr, "Error. The number of arguments must be 2\n");
        exit(EXIT_FAILURE);
    }

    if((pid = fork()) == -1){
        fprintf(stderr, "Error creating child process\n");
        exit(EXIT_FAILURE);
    }else if(pid == 0){
        int i;
        close(childPipe[READING]);
        close(fatherPipe[WRITING]);

        read(fatherPipe[READING], bufferReading, sizeof(bufferReading));

        for (i = 0; bufferReading[i] != '\0'; ++i){
		    bufferReading[i] = toupper(bufferReading[i]);
	    }

        write(childPipe[WRITING], bufferReading, (strlen(bufferReading)+1));

        exit(EXIT_SUCCESS);
    }else{
        close(childPipe[WRITING]);
        close(fatherPipe[READING]);

        write(fatherPipe[WRITING], argv[1], (strlen(argv[1])+1));

        read(childPipe[READING], bufferReading, sizeof(bufferReading));

        printf("El texto en mayúsculas es: %s\n", bufferReading);

        exit(EXIT_SUCCESS);
    }

}