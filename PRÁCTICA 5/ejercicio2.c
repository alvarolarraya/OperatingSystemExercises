//Este programa se vincula a la memoria que crea el ejercicio 1 y modifica el entero en la posicion indicada como argumento dandole el valor que diga el usuario como argumento

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{   
    int posicion,clave,valor,shmid,tamano,;
    int *posicionMemoriaPrimerEntero,posicionMemoriaInteres;
    //Obtengo como argumento el tamaño de la memoria compartida
    tamano = atoi(argv[2]);
    //Obtengo la clave de la memoria compartida
    clave = ftok("/tmp",atoi(argv[1]));
    //Obtengo el id de la memoria compartida
    shmid = shmget(clave, tamano,  SHM_W);
    //Me vinculo a la memoria compartida
    posicionMemoriaPrimerEntero = (int *)shmat(shmid, NULL, 0);
    if (posicionMemoriaPrimerEntero == (int *)-1) {
        perror(“Obteniendo dirección de memoria compartida”);
        return -1;
    }
    //Como quiero modificar la posicion de memoria del iesimo entero y tengo la del primero => avanzo lo que ocupa un entero posicion veces, para tener la direccion de memoria que quiero
    posicionMemoriaInteres = posicionMemoriaPrimerEntero + posicion*sizeof(int);
}
