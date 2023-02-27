#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>
#include <string.h>

#define FILE_NAME "estudiantes.txt"
#define MAIN_DIRECTORY_NAME "estudiantes"
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
#define MEAN_MESSAGE "La nota media de la clase es: "
#define READING 0
#define WRITING 1
#define WRITING_PIPE_SIZE 256
#define NUMBER_OF_PROCESSES 3
#define PROCESS_A_ARRAY_POSITION 0
#define PROCESS_B_ARRAY_POSITION 1
#define PROCESS_C_ARRAY_POSITION 2
#define MAX_PATH 4098

void deleteFiles(const char *path);
void endProcesses();
void signalHandler(int signal);
void writeFile(int filePointer, char *message);
int meanCalculation(int readingPipePointer);
void writeMean (int rdPipe, int logFilePointer);

pid_t pids[NUMBER_OF_PROCESSES];
int logFilePointer;

int main(int argc, char*argv[]){

    pid_t pidBC;
    int statusA, statusBC, i;
    int pipePointers[2];
    char wrPipe[WRITING_PIPE_SIZE];

    /*Creating and opening of Log file*/
    if((logFilePointer = open(LOG_FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, LOG_FILE_PERMISSIONS)) == -1){
        fprintf(stderr, "Error. Log file could not be created\nTerminating program\n");
        exit(EXIT_FAILURE);
    }

    /*The first line of the log file is written*/
    writeFile(logFilePointer, FIRST_LOG_LINE);

    if(signal(SIGINT, signalHandler) == SIG_ERR){
        writeFile(logFilePointer, "Error. Signal handler could not be set\nTerminating program\n");
        exit(EXIT_FAILURE);
    }

    /*Creation of the child process A*/
    if((pids[PROCESS_A_ARRAY_POSITION] = fork()) < 0) {
        writeFile(logFilePointer, "Error. The child process A could not be created\nTerminating program\n");
        exit(EXIT_FAILURE);
    } else if(pids[PROCESS_A_ARRAY_POSITION] == 0) {
        execl("./exec/pa", argv[0], FILE_NAME, NULL);
        writeFile(logFilePointer, "Error. The child process A could not be executed\nTerminating program\n");
        exit(EXIT_FAILURE);
    }

    /*We wait for the child process A to finish*/
    if(waitpid(pids[PROCESS_A_ARRAY_POSITION], &statusA, 0) == -1){
        writeFile(logFilePointer, "Error. Waiting for child proccess A failed\nTerminating program\n");
        exit(EXIT_FAILURE);
    }
    pids[PROCESS_A_ARRAY_POSITION] = 0;

    /*We write the results of process A execution*/
    if(statusA == EXIT_SUCCESS){
        writeFile(logFilePointer, PROCESS_A_SUCCESS);
    } else {
        writeFile(logFilePointer, PROCESS_A_FAILURE);
    }

    /*Creation of child process B*/
    if((pids[PROCESS_B_ARRAY_POSITION] = fork()) < 0) {
        writeFile(logFilePointer, "Error. The child process B could not be created\nTerminating program\n");
        exit(EXIT_FAILURE);
    } else if(pids[PROCESS_B_ARRAY_POSITION] == 0) {
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
    if((pids[PROCESS_C_ARRAY_POSITION] = fork()) < 0) {
        writeFile(logFilePointer, "Error. The child process C could not be created\nTerminating program\n");
        exit(EXIT_FAILURE);
    } else if(pids[PROCESS_C_ARRAY_POSITION] == 0) {
        execl("./exec/pc", wrPipe, FILE_NAME, NULL);
        writeFile(logFilePointer, "Error. The child process C could not be executed\nTerminating program\n");
        exit(EXIT_FAILURE);
    }
    
    /*We wait for the child processes B and C to finish*/
    for(i = 1; i < NUMBER_OF_PROCESSES; i++){     

        if((pidBC = wait(&statusBC)) == -1){
            writeFile(logFilePointer, "Error. Waiting for child proccesses B and C failed\nTerminating program\n");
            exit(EXIT_FAILURE);
        }

        if(pidBC == pids[PROCESS_B_ARRAY_POSITION]){
            if(statusBC == EXIT_SUCCESS) writeFile(logFilePointer, PROCESS_B_SUCCESS);
            else writeFile(logFilePointer, PROCESS_B_FAILURE);
            pids[PROCESS_B_ARRAY_POSITION] = 0;
        }else if(pidBC == pids[PROCESS_C_ARRAY_POSITION]){
            if(statusBC == EXIT_SUCCESS){
                writeFile(logFilePointer, PROCESS_C_SUCCESS);
                writeMean(pipePointers[READING], logFilePointer);
            } 
            else writeFile(logFilePointer, PROCESS_C_FAILURE);
            pids[PROCESS_C_ARRAY_POSITION] = 0;
        }
    }

    writeFile(logFilePointer, LAST_LOG_LINE);

    exit(EXIT_SUCCESS);
}

void writeMean (int rdPipe, int logFilePointer){
    char message[sizeof(MEAN_MESSAGE)];
    char meanChar [2];
    int mean = 0;

    if(read(rdPipe, &mean, sizeof(int)) == -1) writeFile(logFilePointer, "Error. Mean could not be read\nTerminating program\n");

    sprintf(meanChar, "%d", mean);
    strcpy(message, MEAN_MESSAGE);
    strcat(message, meanChar);

    writeFile(logFilePointer, message);
    
}

void deleteFile (const char *path){
	DIR *dp;
	struct dirent *node;
	struct stat statBuf, statBufAux;
	char name[MAX_PATH];

	if(stat(path, &statBuf) == -1){
		fprintf(stderr, "Error. %s could not be open\n", path);
		exit(EXIT_FAILURE);
	}

	if(S_ISREG(statBuf.st_mode)){
		if(remove(path) == -1){
			fprintf(stderr, "Error. File %s could not be deleted\n", path);
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}

	if ((dp = opendir(path)) == NULL) {
		fprintf(stderr, "Error. Directory %s could not be open\n", path);
		exit(EXIT_FAILURE);
	}

	while ((node = readdir(dp)) != NULL){
		sprintf(name, "%s/%s", path, node->d_name);

		if(strcmp(node->d_name, ".") == 0 || strcmp(node->d_name, "..") == 0) continue;

		if(stat(name, &statBufAux) == -1) fprintf(stderr, "Error. %s could not be open\n", path);

		if(S_ISDIR(statBufAux.st_mode)) deleteFile(name);
		else{
			if(remove(name) == -1){
				fprintf(stderr, "Error. File %s could not be deleted\n", name);
				exit(EXIT_FAILURE);
			}
			
		}		
		
	}

	if(rmdir(path) == -1){
		fprintf(stderr, "Error. Directory %s could not be deleted\n", path);
		closedir(dp);
		exit(EXIT_FAILURE);
	}

	closedir(dp);
}

void endProcesses(pid_t pids[], int logFilePointer){

    int i;

    for(i = 0; i < NUMBER_OF_PROCESSES; i++){
        if(pids[i] != 0){
            if(kill(pids[i], SIGINT) == -1){
                writeFile(logFilePointer, "Error. Processes could not be killed\n");
            }
        }
    }
}

void signalHandler(int signal){

    pid_t pidD;

    endProcesses(pids, logFilePointer);

    if((pidD = fork()) == 0){
        deleteFile(MAIN_DIRECTORY_NAME);
    }else{
        waitpid(pidD, NULL, 0);
    }

    writeFile(logFilePointer, "Ctrl+C pressed. Processes killed and files deleted\n");
    writeFile(logFilePointer, LAST_LOG_LINE);
    exit(EXIT_SUCCESS);
}

void writeFile(int filePointer, char *message){

    if(write(filePointer, message, sizeof(message)) != sizeof(message)){
        fprintf(stderr, "Error. File could not be written\n");
        exit(EXIT_FAILURE);
    }
    
}