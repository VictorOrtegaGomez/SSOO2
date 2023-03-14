#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char*argv[]){
    int i;

    for (i = 0; argv[0][i] != '\0'; ++i){
		    fprintf(stdout,"%c",toupper(argv[0][i]));
    }

    exit(EXIT_SUCCESS);
}