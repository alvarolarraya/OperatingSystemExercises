// AUTOR: Eugen Hamuraru
// FECHA: 12 - 12 - 2020
/* DESCRIPCION PROGRAMA: Para cada uno de nuestros anillos se ira generando un programa coordinador cuyo trabajo es actualizar el mensaje,
encolarlo para que el proceso informador pueda acceder a el, acceder a la memoria compartida para obtener el orden de llegada y para ello
hare uso de un semaforo para evitar daños en la memoria compartida*/

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

typedef struct msgbuf{
	long idMensaje;
	int idAnillo;
	int numVueltas;
	int nodosVisitados;
	int ordenLlegada;
}msgbuf;
msgbuf msg;

int main(int argc, char **argv){
	int *punteroAMemoria;
	int claveArgumento = atoi(argv[1]);
	char mensajeLlegada[10];
	int idCola, idSemaforo, idMemoria;
	int idAnillo = atoi(argv[2]);
	int numNodos = atoi(argv[3]);
	int periodo = atoi(argv[4]);
	int clave = ftok("/tmp",claveArgumento);
	idCola = msgget(clave, 0666);
	idSemaforo = semget(clave, 1, 0666);
	idMemoria = shmget(clave, sizeof(int), 0777|IPC_CREAT);
	semctl( idSemaforo, 0, SETVAL, 1 );
	punteroAMemoria =(int *)shmat(idMemoria,NULL, 0);
	(*punteroAMemoria) = 0;
	msg.idMensaje = 1;
	msg.idAnillo = idAnillo;
	msg.numVueltas = 0;
	msg.nodosVisitados = 0;
	msg.ordenLlegada = 0;
	struct sembuf p = { 0, -1, 0};
	struct sembuf v = { 0, +1, 0};
	sleep(periodo);
	sprintf(mensajeLlegada,"%ld %d %d %d %d",msg.idMensaje,msg.idAnillo,msg.numVueltas,msg.nodosVisitados,msg.ordenLlegada);
	dprintf(STDOUT_FILENO,"%s", mensajeLlegada);		//aqui si utilizas fprintf o printf (cualquier funcion que utilice FILE*) no funciona: tiene que utilizar un entero como descriptor de fichero por el dup2
	while(1)
	{
		read(STDIN_FILENO,mensajeLlegada,10);		//aqui si utilizas el fgets fscanf o lo que sea que utilice FILE* no funciona (será por el dup2): tiene que utilizar un entero como descriptor de fichero
		msg.numVueltas++;
		msg.nodosVisitados = msg.nodosVisitados + numNodos;
		if (semop(idSemaforo, &p, 1) == -1)
		{
			perror("Error bloqueo de semaforo\n");
			exit(-1);
		}
		(*punteroAMemoria)++;
		msg.ordenLlegada = (*punteroAMemoria);
		if (semop(idSemaforo, &v, 1) < 0 )
		{
			perror("Error liberacion de  semaforo\n");
			exit(-1);
		}
		msgsnd(idCola, &msg, sizeof(msg) - sizeof(long), IPC_NOWAIT);
		sprintf(mensajeLlegada,"%ld %d %d %d %d",msg.idMensaje,msg.idAnillo,msg.numVueltas,msg.nodosVisitados,msg.ordenLlegada);
		sleep(periodo);
		dprintf(STDOUT_FILENO,"%s", mensajeLlegada);
	}
}
