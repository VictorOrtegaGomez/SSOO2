#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void factorial(int n){
    int i;
    long double factorial;
    factorial = 1;

    for(i = 1; i <= n; i++) {
        factorial *= i;
    }

    printf("El factorial de %d es %0.Lf\n", n, factorial);
}

void primos(int n){
    int i, j, primo;

    for(i = 3; i <= n; i++){
        
        primo = 1;

        for(j = 2; j <= 9 && primo == 1; j++){
            if(i != j && i % j == 0) primo = 0;
        } 

        if(primo == 1) printf("El número %d es primo\n", i);

    }

}

int main(int argc, char *argv[]) {
    int n, status;
    pid_t pid;

    if(argc != 2) {
        printf("Error. Debe introducir el valor");
        exit(-1);
    }

    if((n = atoi(argv[1])) < 1) {
        printf("Error. Debe introducir un valor numérico positivo");
        exit(-1);
    }

    if((pid = fork()) < 0) {
        printf("Error al crear el proceso hijo");
        exit(-1);
    } else if(pid == 0) {
        factorial(n);
        exit(0);
    }

    if((pid = fork()) < 0) {
        printf("Error al crear el proceso hijo");
        exit(-1);
    } else if(pid == 0) {
        primos(n);
        exit(0);
    }

    wait(&status);

    return 0;
}