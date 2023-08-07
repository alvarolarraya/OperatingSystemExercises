//Este programa crea 5 programas hijo con el mismo padre
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main () {
    int i, pid;
    printf("Este es el proceso padre con ID %ld\n",
            (long)getpid());
    for (i=0; i < 5; i++){
        pid = fork();

        if (pid == -1) {
            printf("Error en la creación del proceso\n");
            exit (-1);
        }
        if (pid == 0) { /* Proceso hijo */

            printf("Este es el proceso hijo de ID %d, cuyo padre es %ld\n", getpid(),
            (long)getppid());
            exit(999);
        }
    }
    return (0);
} 
