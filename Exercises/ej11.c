#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char*argv[]){
    pid_t pid;
    int status;

    if(argc != 2){
        printf("Error. Directory name must be introduced.");
        exit(EXIT_FAILURE);
    }
    
    if((pid = fork()) < 0) {
        fprintf(stderr, "Error when creating the child process\n");
        exit(EXIT_FAILURE);
    } else if(pid == 0) {
        execl("./filesDeletion", NULL, argv[1], NULL);
        fprintf(stderr,"Error in filesDeletion execution\n");
        exit(EXIT_FAILURE);
    }

    waitpid(pid, &status, 0);

    return 0;
}