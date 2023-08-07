//diapositivas de iñigo:    tinyurl.com/SSOO2020
// ftok("\tmp",int)   tmp es una carpeta temporal, luego va a ir siempre (si no funciona puede ser \tmp/)

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int shmid;
void liberar(int signum);


int main(int argc, char * argv[])
{   
    int tamano,errorClave;
    key_t clave;
    //Espero la señal control C y la trato con liberar
    signal(SIGINT,liberar);
    if(argc != 3)
        printf("Ha olvidado la clave y el tamano\n");
    //atoi sirve para obtener el entero correspondiente a un caracter
    tamano = atoi(argv[2]);
    //Creo la clave para la memoria compartida (directorio donde guardo la memoria compartida, argumento para crear la clave)
    //Dependiendo de si uso un argumento u otro la clave que me devuelva sera distinta
    clave = ftok("/tmp",atoi(argv[1]));
    //Creo la memoria compartida
    shmid = shmget(clave, tamano, IPC_CREAT | IPC_EXCL);
    if (shmid == -1)
        perror("Error al crear la memoria compartida.");
    else
        printf("Se ha creado una memoria compartida\n");
    //Espero a que me entre la señal control c por teclado
    pause();
    printf("Se va a terminar el programa\n");
    return 0;
}

void liberar(int signum)
{
    //Libero la memoria compartida
    int r = shmctl(shmid, IPC_RMID, NULL);
    if (r == -1)
        perror("\nError eliminando memoria compartida\n");
    printf("\nSe ha liberado la memoria\n");
}
