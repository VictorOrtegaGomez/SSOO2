#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NOMBRE_FICHERO "estudiantes.txt"

int main(int argc, char*agrv[]){
    pid_t pidA;
    int statusA;

    if((pidA = fork()) < 0) {
        printf("Error al crear el proceso hijo A\n");
        exit(-1);
    } else if(pidA == 0) {
        execvp("./pa", NOMBRE_FICHERO);
    }

    printf("Proceso hijo A creado correctamente\n");
    waitpid(pidA, &statusA, 0);
}