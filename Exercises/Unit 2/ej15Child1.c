#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define CHILD_FATHER_READING_PIPE 0
#define CHILDREN_WRITING_PIPE 1
#define BUFFER_SIZE 100

/*1st child will read from the father that food is ready, then he will tell to the 2nd child*/

int main(int argc, char *argv[]){

    char bufferReading[BUFFER_SIZE];

    read(atoi(argv[CHILD_FATHER_READING_PIPE]), bufferReading, BUFFER_SIZE);
    fprintf(stdout, "[CHILD 1] Message read from children-father pipe: %s\n", bufferReading);
    write(atoi(argv[CHILDREN_WRITING_PIPE]), bufferReading, BUFFER_SIZE);
    fprintf(stdout, "[CHILD 1] Message sent through children pipe: %s\n", bufferReading);

    exit(EXIT_SUCCESS);
}