// AUTOR: Eugen Hamuraru
// FECHA: 12 - 12 - 2020
/* DESCRIPCION PROGRAMA: El programa gestor es el programa principal encargado de general el semaforo, la cola de mensajes y la memoria
compartida, además sera el proceso encargado de generar los anillos y para cada anillo su numero de nodos y el coordinador de ese anillo
para ello recibira los datos necesarios por parametro desde la terminal y tambien enviara como parametros los datos necesarios a los
procesos que vaya ejecutando.  */

#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int vectorPids[100];
int posicionVector = 0;
int idCola, idMemoria, idSemaforo, pidInformador, pidNodos;

void finEjecucion(int signum);

int main(int argc, char **argv){
  union semun arg;
	signal(SIGINT, finEjecucion);
	char stringIdAnillo[10];
	char stringNumNodos[10];
	int claveArgumento = atoi(argv[1]);
	int numAnillos = atoi(argv[2]);
	int numNodos = atoi(argv[3]);
	int clave = ftok("/tmp",claveArgumento);
	idCola = msgget(clave, 0666|IPC_CREAT);
	idMemoria = shmget(clave, sizeof(int), 0777|IPC_CREAT);
	idSemaforo = semget(clave, 1, 0777|IPC_CREAT);
	arg.val = 1;
	semctl( idSemaforo, 0, SETVAL, arg );
	sprintf(stringNumNodos,"%d",numNodos);
	pidInformador = fork();
  if(pidInformador == -1)
    perror("no se ha creado bien el proceso");
	if ( pidInformador == 0)
		execlp("./informador","./informador",argv[1],NULL);
	vectorPids[posicionVector] = pidInformador;
  printf(ANSI_COLOR_CYAN"               ANALISIS PROCESOS EN CURSO\n");
  printf(ANSI_COLOR_YELLOW"PROCESO INFORMADOR [ %d ]\n",vectorPids[0]);
	int arrayPipes[numAnillos * numNodos][2];
	int i, j;
	for ( i = 0; i < numAnillos *numNodos; i ++)
	{
			pipe(arrayPipes[i]);
	}
	pidNodos = fork();
  if(pidNodos == -1)
    perror("no se ha creado bien el proceso hijo");
	if (pidNodos == 0)
	{
		for (i = 0; i < numAnillos; i++)
		{
			for (j = 0; j < numNodos; j++)
			{
				sprintf(stringIdAnillo,"%d",i);
				pidNodos = fork();
				if ( pidNodos == 0)
				{
          //si el nodo es el ultimo es el coordinador y escribe en la entrada estandar del primer nodo
					if ( j == numNodos-1)
					{
            dup2(arrayPipes[(i *numNodos) + j][0], STDIN_FILENO);
						dup2(arrayPipes[ numNodos * i][1], STDOUT_FILENO);
						execlp("./coordinador","./coordinador",argv[1], stringIdAnillo ,stringNumNodos, argv[4] ,NULL);
					}
					else
					{
            dup2(arrayPipes[ (numNodos * i) + j][0], STDIN_FILENO);
						dup2(arrayPipes[ (numNodos * i) + j +1][1], STDOUT_FILENO);
						execlp("./nodo","./nodo",argv[4],NULL);
					}
				}
				vectorPids[1 +((i*numNodos)+j)] = pidNodos;
				if ( j == 0 )
				{
					printf(ANSI_COLOR_RED"\tSE HA CREADO EL ANILLO[ %d ]\n",i);
				}
        if ( j == numNodos-1 )
					printf(ANSI_COLOR_BLUE "PROCESO COORDINADOR [ %d ]\n",vectorPids[(i*numNodos) + j+1]);
				else
					printf(ANSI_COLOR_GREEN "PROCESO NODO [ %d ]\n",vectorPids[(i*numNodos) + j+1]);
			}
		}
	}
  else
  {
    int status;
    waitpid(pidNodos,&status,0);
    pause();
    printf(ANSI_COLOR_RESET ANSI_COLOR_YELLOW"\nTERMINO\n");
  	return 0;
  }
}

void finEjecucion(int signum){
	msgctl (idCola, IPC_RMID, 0);
	shmctl(idMemoria, IPC_RMID, NULL);
	semctl( idSemaforo, 0, IPC_RMID);
	for(int i = 0; i < 100; i++){
		if (vectorPids[i] == 0)
      break;
		kill(vectorPids[i],SIGKILL);
	}
	return;
}
