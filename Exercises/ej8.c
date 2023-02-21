#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *fichero;

    if(argc != 2) {
        printf("Error. Debe introducir el nombre del fichero.");
        exit(-1);
    }

    if((fichero = fopen(argv[1], "r")) == NULL) {
        printf("Error al abrir el fichero. Probablemente no existe.");
        exit(-1);
    }else{
        printf("Fichero abierto correctamente. El fichero existe.");
        fclose(fichero);
        exit(0);
    }

}