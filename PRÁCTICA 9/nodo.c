// AUTOR: Eugen Hamuraru
// FECHA: 12 - 12 - 2020
/* DESCRIPCION PROGRAMA: El programa nodo es un sencillo programa lee un mensaje de la entrada estandar espera un periodo indicado como
argumento y lo escribe en la salida estandar, tanto la entrada como la salida estandard estan gestionadas desde el gestor */


#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>


int main(int argc, char **argv)
{
	char mensajeLlegada[10];
	int periodo = atoi(argv[1]);
	while(1)
	{
		read(STDIN_FILENO,mensajeLlegada,10);
		sleep(periodo);
		dprintf(STDOUT_FILENO,"%s", mensajeLlegada);
	}
	return 0;
}
