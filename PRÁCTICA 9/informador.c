// AUTOR: Eugen Hamuraru
// FECHA: 12 - 12 - 2020
/* DESCRIPCION PROGRAMA: El programa informador es un sencillo programa que de forma indefinida lee mensajes de una cola previamente
creada y los va mostrando por pantalla  */


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
#define FONDO_BLANCO "\x1b[47m"
#define NEGRO "\x1b[30m"

typedef struct msgbuf{
	long idMensaje;
	int idAnillo;
	int numVueltas;
	int nodosVisitados;
	int ordenLlegada;
}msgbuf;

int main(int argc, char **argv)
{
	msgbuf msg;
	int idCola;
	int claveArgumento = atoi(argv[1]);
	key_t clave = ftok("/tmp",claveArgumento);
	idCola = msgget(clave, 0666);
	while(1)
	{
		msgrcv(idCola, &msg, sizeof(msg) - sizeof(long), 0, 0);
		printf(FONDO_BLANCO NEGRO "ordenLlegada: %d , idAnillo: %d, numVueltas: %d, nodosVisitados: %d \n",msg.ordenLlegada, msg.idAnillo, msg.numVueltas, msg.nodosVisitados);
	}
}
