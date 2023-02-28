#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define CHILD_FATHER_WRITING_PIPE 0
#define CHILDREN_READING_PIPE 1
#define BUFFER_SIZE 100

/*2nd child will read from 1st child thaat food is ready, then tell his father*/

int main(int argc, char *argv[]){

    char bufferReading[BUFFER_SIZE];

    read(atoi(argv[CHILDREN_READING_PIPE]), bufferReading, BUFFER_SIZE);
    fprintf(stdout, "[CHILD 2] Message read from children pipe: %s\n", bufferReading);
    write(atoi(argv[CHILD_FATHER_WRITING_PIPE]), bufferReading, BUFFER_SIZE);
    fprintf(stdout, "[CHILD 2] Message sent through children-father pipe: %s\n", bufferReading);

    exit(EXIT_SUCCESS);
}