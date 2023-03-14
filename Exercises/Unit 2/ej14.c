#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define READING 0
#define WRITING 1
#define DIRECTORY_TO_DELETE 1
#define DIRECTORY_TO_BACKUP 2
#define NUM_CHILD_PROCESSES 2
#define PIPES_TO_CHAR 256
#define MAX_PATH 4098

int main(int argc, char* argv[]){
    pid_t pids[NUM_CHILD_PROCESSES];
    int deletionPipe[2];
    int backupPipe[2];
    /*
    char deletionPipeRr[PIPES_TO_CHAR];
    char backupPipeRd[PIPES_TO_CHAR]; */

    /*We create both pipes we'll be using*/
    pipe(deletionPipe);
    pipe(backupPipe);

    /*Pipes' pointers transformation into char
    sprintf(deletionPipeRr, "%d", deletionPipe[WRITING]);
    sprintf(backupPipeRd, "%d", backupPipe[WRITING]);
    */

    if(argc != 3){
        fprintf(stderr, "Error. 1st argument introduced must be the directory we want to delete and 2nd the directory we want to backup\n");
        exit(EXIT_FAILURE);
    }

    /*Child processes' creation*/
    if((pids[0] = fork()) == -1){
        fprintf(stderr, "Error. Fork failed\n");
        exit(EXIT_FAILURE);
    }else if(pids[0] == 0){
        /*1st Child process*/
        char directory[MAX_PATH];
        read(deletionPipe[READING], directory, MAX_PATH);
        execl("./filesDeletion", argv[0], directory, NULL);
        fprintf(stderr, "Error. Exec failed\n");
        exit(EXIT_FAILURE);
    }

    if((pids[1] = fork()) == -1){
        fprintf(stderr, "Error. Fork failed\n");
        exit(EXIT_FAILURE);
    }else if(pids[1] == 0){
        /*2nd Child process*/
        char directory[MAX_PATH];
        read(backupPipe[READING], directory, MAX_PATH);
        execl("./filesBackup", argv[0], directory, NULL);
        fprintf(stderr, "Error. Exec failed\n");
        exit(EXIT_FAILURE);
    }

    write(deletionPipe[WRITING], argv[DIRECTORY_TO_DELETE], sizeof(argv[DIRECTORY_TO_DELETE]));
    write(backupPipe[WRITING], argv[DIRECTORY_TO_BACKUP], sizeof(argv[DIRECTORY_TO_BACKUP]));

    exit(EXIT_SUCCESS);

}