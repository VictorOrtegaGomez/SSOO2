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
#define LOG_FILE_OPEN_MODE "a"
#define FIRST_LOG_LINE "******** System log ********\n"
#define LAST_LOG_LINE "\nEND OF EXECUTION\n"
#define PROCESS_A_SUCCESS "Directory creation finished.\n"
#define PROCESS_A_FAILURE "Error. Process A finished with errors\n"
#define PROCESS_B_SUCCESS "Exam models copy finished.\n"
#define PROCESS_B_FAILURE "Error. Process B finished with errors\n"
#define PROCESS_C_SUCCESS "Files with minimum mark creation finished\n"
#define PROCESS_C_FAILURE "Error. Process C finished with errors\n"
#define MEAN_MESSAGE "The mean of the class is: "
#define CTRL_C_MESSAGE "Ctrl+C pressed. Processes killed and files deleted\n"
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
void writeLogFile(char *message);
int meanCalculation(int readingPipePointer);
void writeMean (int rdPipe);

pid_t glbPids[NUMBER_OF_PROCESSES];

int main(int argc, char*argv[]){

    pid_t pidBC;
    int statusA, statusBC, i;
    int pipePointers[2];
    char wrPipe[WRITING_PIPE_SIZE];

    /*We write the first line of the log file*/

    writeLogFile(FIRST_LOG_LINE);
    
    /*We install the signal handler for SIGINT signals*/

    if(signal(SIGINT, signalHandler) == SIG_ERR){
        writeLogFile("Error. Signal handler could not be set\nTerminating program\n");
        exit(EXIT_FAILURE);
    }

    /*Creation of the child process A*/

    if((glbPids[PROCESS_A_ARRAY_POSITION] = fork()) < 0) {
        writeLogFile("Error. The child process A could not be created\nTerminating program\n");
        exit(EXIT_FAILURE);
    } else if(glbPids[PROCESS_A_ARRAY_POSITION] == 0) {
        execl("./exec/pa", argv[0], FILE_NAME, NULL);
        writeLogFile("Error. The child process A could not be executed\nTerminating program\n");
        exit(EXIT_FAILURE);
    }

    /*We wait for the child process A to finish*/

    if(waitpid(glbPids[PROCESS_A_ARRAY_POSITION], &statusA, 0) == -1){
        writeLogFile("Error. Waiting for child proccess A failed\nTerminating program\n");
        exit(EXIT_FAILURE);
    }
    glbPids[PROCESS_A_ARRAY_POSITION] = 0;

    /*We write the results of process A execution*/

    if(statusA == EXIT_SUCCESS){
        writeLogFile(PROCESS_A_SUCCESS);
    } else {
        writeLogFile(PROCESS_A_FAILURE);
    }

    /*Creation of child process B*/

    if((glbPids[PROCESS_B_ARRAY_POSITION] = fork()) < 0) {
        writeLogFile("Error. The child process B could not be created\nTerminating program\n");
        exit(EXIT_FAILURE);
    } else if(glbPids[PROCESS_B_ARRAY_POSITION] == 0) {
        execl("./exec/pb", argv[0], FILE_NAME, NULL);
        writeLogFile("Error. The child process B could not be executed\nTerminating program\n");
        exit(EXIT_FAILURE);
    }

    /*Comunication Pipe between manager and C proccess*/

    if(pipe(pipePointers) == -1){
        writeLogFile("Error. Pipe could not be created\nTerminating program\n");
        exit(EXIT_FAILURE);
    }
    sprintf(wrPipe, "%d", pipePointers[WRITING]);

    /*Creation of child process C*/

    if((glbPids[PROCESS_C_ARRAY_POSITION] = fork()) < 0) {
        writeLogFile("Error. The child process C could not be created\nTerminating program\n");
        exit(EXIT_FAILURE);
    } else if(glbPids[PROCESS_C_ARRAY_POSITION] == 0) {
        execl("./exec/pc", wrPipe, FILE_NAME, NULL);
        writeLogFile("Error. The child process C could not be executed\nTerminating program\n");
        exit(EXIT_FAILURE);
    }
    
    /*We wait for the child processes B and C to finish*/

    for(i = 1; i < NUMBER_OF_PROCESSES; i++){     

        if((pidBC = wait(&statusBC)) == -1){
            writeLogFile("Error. Waiting for child proccesses B and C failed\nTerminating program\n");
            exit(EXIT_FAILURE);
        }

        if(pidBC == glbPids[PROCESS_B_ARRAY_POSITION]){
            if(statusBC == EXIT_SUCCESS) writeLogFile(PROCESS_B_SUCCESS);
            else writeLogFile(PROCESS_B_FAILURE);
            glbPids[PROCESS_B_ARRAY_POSITION] = 0;
        }else if(pidBC == glbPids[PROCESS_C_ARRAY_POSITION]){
            if(statusBC == EXIT_SUCCESS){
                writeLogFile(PROCESS_C_SUCCESS);
                writeMean(pipePointers[READING]);
            } 
            else writeLogFile(PROCESS_C_FAILURE);
            glbPids[PROCESS_C_ARRAY_POSITION] = 0;
        }
    }

    writeLogFile(LAST_LOG_LINE);

    exit(EXIT_SUCCESS);
}

/*Function that will write in the log file the students' mean*/

void writeMean (int rdPipe){
    char message[sizeof(MEAN_MESSAGE)];
    char meanChar [2];
    int mean = 0;

    /*The calculated mean from the child process will be read through a pipe*/

    if(read(rdPipe, &mean, sizeof(int)) == -1) writeLogFile("Error. Mean could not be read\nTerminating program\n");

    sprintf(meanChar, "%d", mean);
    strcpy(message, MEAN_MESSAGE);
    strcat(message, meanChar);

    writeLogFile(message);
    
}

/*Function that will delete the file introduced. If path is a directory it will delete every file from it and then said directory*/

void deleteFile (const char *path){
	DIR *dp;
	struct dirent *node;
	struct stat statBuf, statBufAux;
	char name[MAX_PATH];

    /*First we'll see if the path is a directory*/

	if(stat(path, &statBuf) == -1){
		fprintf(stderr, "Error. %s could not be open\n", path);
		exit(EXIT_FAILURE);
	}

    /*If we're dealing with a file and not a directory we'll delete it*/

	if(S_ISREG(statBuf.st_mode)){
		if(remove(path) == -1){
			fprintf(stderr, "Error. File %s could not be deleted\n", path);
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}

    /*If it is a directory we'll read what it contains. If the node we're reading is a file it'll be deleted, otherwise we'll call this funtion again*/

	if ((dp = opendir(path)) == NULL) {
		fprintf(stderr, "Error. Directory %s could not be open\n", path);
		exit(EXIT_FAILURE);
	}

	while ((node = readdir(dp)) != NULL){
		sprintf(name, "%s/%s", path, node->d_name);

		if(strcmp(node->d_name, ".") == 0 || strcmp(node->d_name, "..") == 0) continue;

		if(stat(name, &statBufAux) == -1) fprintf(stderr, "Unable to open %s It could be a symbolic link, if so deletion is granted\n", name);

		if(S_ISDIR(statBufAux.st_mode)) deleteFile(name);
		else{
			if(remove(name) == -1){
				fprintf(stderr, "Error. File %s could not be deleted\n", name);
				exit(EXIT_FAILURE);
			}
			
		}		
		
	}

    /*Once it is over deleting everything from the directory we'll delete the own directory*/

	if(rmdir(path) == -1){
		fprintf(stderr, "Error. Directory %s could not be deleted\n", path);
		closedir(dp);
		exit(EXIT_FAILURE);
	}

	closedir(dp);
}

/*Function that will send a SIGINT signal to all processes in order to stop them*/

void endProcesses(pid_t glbPids[]){
    int i;

    for(i = 0; i < NUMBER_OF_PROCESSES; i++){
        if(glbPids[i] != 0){
            if(kill(glbPids[i], SIGINT) == -1){
                writeLogFile("Error. Processes could not be killed\n");
            }
        }
    }
}

/*Function that will handle SIGINT signals*/

void signalHandler(int signal){

    pid_t pidD;

    /*First we'll end every process*/

    endProcesses(glbPids);

    /*Second we'll relay on a child process that will handle the deletion of the generated files*/

    if((pidD = fork()) == 0){
        deleteFile(MAIN_DIRECTORY_NAME);
        exit(EXIT_SUCCESS);
    }

    /*Third we'll write in the log file that CTRL + C signal was received and we'll wait for the previous process to end*/

    writeLogFile(CTRL_C_MESSAGE);
    writeLogFile(LAST_LOG_LINE);

    waitpid(pidD, NULL, 0);

    exit(EXIT_SUCCESS);
}

/*Function that will write the message in the log file. If the file does not exist it'll be created*/

void writeLogFile(char *message){

    FILE *logFile;

    if((logFile = fopen(LOG_FILE_NAME, LOG_FILE_OPEN_MODE)) == NULL){
        fprintf(stderr, "Error. File %s could not be open\n", LOG_FILE_NAME);
        exit(EXIT_FAILURE);
    }

    fprintf(logFile, "%s", message);

    fclose(logFile);
}