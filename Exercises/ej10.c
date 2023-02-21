#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int hijo(){
    int numero;

    srand(getpid()); /*Semilla*/
    numero = rand() % (3-1+1) + 1; /*El número aleatorio está entre M y N (N-M+1) + M*/

    exit(numero);
}

int main(int argc, char*argv[]){
    pid_t pid;
    int status, causa;

    if((pid = fork()) < 0) {
        printf("Error al crear el proceso hijo");
        exit(-1);
    } else if(pid == 0) {
        hijo();
    }

    waitpid(pid, &status, 0);
    causa = WEXITSTATUS(status);
    
    if(causa == 1) printf("El hijo ha dicho 1\n");
    else if(causa == 2) printf("El hijo ha dicho 2\n");
    else if(causa == 3) printf("El hijo ha dicho 3\n");

    return 0;
}