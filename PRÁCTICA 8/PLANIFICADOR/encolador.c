#include "fragmenta.h"
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>

bool algoEnEjecucion,haTerminado,noMasInput;
int pidEnEjecucion,cuantosEnCola,pidManejador;

typedef struct infoProceso
{
	char teclado[100];
    int pid;
}infoProceso;

struct proceso
{
    long prioridad;
    infoProceso info;
}proceso;

void encolador(struct proceso);
void hacerExec(char** comando);
void termina (int signum);
void liberaColaMensajes(int signum);
void aumentaCuantosEnCola (int signum);
void alarmaRoundRobin(int signum);
void cierraElTeclado(int signum);

int main(int argc,char** argv)
{
    bool hayFichero;
    key_t clave;
    int pid_liberar,idColaProcesos;
    struct sigaction avisameSoloCuandoTermine;
    if(argc == 1)
		hayFichero = false;
	else
		hayFichero = true;
	noMasInput = false;
	algoEnEjecucion = false;
	cuantosEnCola = 0;
	pidEnEjecucion = 0;
    haTerminado = false;
    clave = ftok ("/tmp", 1);
    if (clave == (key_t) -1)
        perror("La clave no se ha creado bien\n");
    idColaProcesos = msgget(clave, 0666 | IPC_CREAT);
    if (idColaProcesos == -1) 
        perror("La cola de mensajes se ha creado mal\n");
    printf("Introduce el comando a ejecutar: \n");
    pid_liberar = fork();
    if(pid_liberar == -1)
        perror("no se ha creado bien el proceso hijo");
    else if(pid_liberar == 0)
    {
		int pid_teclado;
		char ** datos;
		pid_teclado = fork();
		while(1)
		{
			if(pid_teclado == -1)
				perror("Se ha formado mal el proceso hijo");
			else if(pid_teclado == 0)
			{
				if(hayFichero)
				{
					FILE* fichero;
					printf("argv[1] es %s", argv[1]);
					fichero = fopen(argv[1],"r");
					fgets(proceso.info.teclado,80,fichero);
				}
				else
				{
					if(fgets(proceso.info.teclado,80,stdin) == 0)
					{
						signal(SIGUSR1,cierraElTeclado);
						printf("Has pulsado Ctrl-D: ya no se aceptarán más comandos.\n");
						printf("Pulsa Ctrl-C cuando quieras que se acabe el programa\n");
						kill(getppid(),SIGUSR1);
						kill(getpid(),SIGKILL);
					}
				}
				datos = fragmenta(proceso.info.teclado);
				proceso.prioridad = atoi(datos[0]);
				proceso.info.pid = 0;
				encolador(proceso);
				kill(getppid(),SIGUSR2);
			}
			else
			{
				signal(SIGUSR2,aumentaCuantosEnCola);
				sigemptyset(&avisameSoloCuandoTermine.sa_mask);
				avisameSoloCuandoTermine.sa_handler = termina;
				avisameSoloCuandoTermine.sa_flags = SA_NOCLDSTOP;
				if(sigaction(SIGCHLD,&avisameSoloCuandoTermine,NULL) == -1)
					perror("no se ha podido manejar la señal SIGCHLD con exito");
				if (cuantosEnCola == 0)
					pause();
				else
				{
					if(-1 == msgrcv (idColaProcesos, &proceso, (sizeof(struct proceso)-sizeof(long)),1,IPC_NOWAIT|0666))
						msgrcv (idColaProcesos, &proceso, (sizeof(struct proceso)-sizeof(long)),2,IPC_NOWAIT|0666);
					char **comando;
					if((comando = malloc(100*sizeof(char*))) == NULL)
						perror("No hay suficiente memoria para reservar el comando");
					datos = fragmenta(proceso.info.teclado);
					int i = 1;
					while(datos[i] != NULL)
					{
						comando[i-1] = datos[i];
						i++;
					}
					comando[i-1] = datos[i];
					if (proceso.prioridad == 1)
					{
						if (algoEnEjecucion)
						{
							int statusEnEjecucion;
							waitpid(pidEnEjecucion,&statusEnEjecucion,0);
						}
						int pidFifo,statusFifo;
						pidFifo = fork();
						if(pidFifo == -1)
							perror("no se ha creado bien el proceso hijo de fifo");
						else if(pidFifo == 0)
							hacerExec(comando);
						else
						{
							waitpid(pidFifo,&statusFifo,0);
							borrarg(datos);
							borrarg(comando);
						}
					}
					else if (proceso.prioridad == 2)
					{
						int pidRoundRobin;
						signal(SIGALRM,alarmaRoundRobin);
						bool primeraVez;
						if(proceso.info.pid == 0)
							primeraVez = true;
						else
							primeraVez = false;
						alarm(5);
						if(primeraVez)
						{
							pidRoundRobin = fork();
							if (pidRoundRobin == -1)
								perror("no se ha creado bien el hijo\n");
							else if (pidRoundRobin == 0)
								hacerExec(comando);
							else
							{
								proceso.info.pid = pidRoundRobin;
								pidManejador = getpid();
								pause();
								kill(pidRoundRobin,SIGSTOP);
							}
						}
						else
						{
							pidManejador = getpid();
							kill(proceso.info.pid,SIGCONT);
							pause();
							kill(proceso.info.pid,SIGSTOP);
						}
						if(!haTerminado)
							encolador(proceso);
						else
							haTerminado = false;
						borrarg(datos);
						borrarg(comando);
					}
					else
					{
						perror("no se ha pasado bien la prioridad del proceso");
						cuantosEnCola--;
					}
				}
			}
		}
	}
	else
    {
        signal(SIGINT,liberaColaMensajes);
        pause();
        printf("\nSe ha liberado la cola de mensajes\n");
		msgctl(idColaProcesos,IPC_RMID,NULL);
        kill(pid_liberar,SIGKILL);
        //HAY QUE PRINTEAR LOS DATOS DE SALIDA
    }
}

void hacerExec(char** comando)
{
    if(-1 == execvp(comando[0],comando))
    {
        perror("No se ha podido ejecutar el comando correctamente (error en el exec)");
        while(1)
        {
            exit(-1);
        }
    }
}

void encolador(struct proceso procesoAEncolar)
{
    key_t clave;
	int idColaProcesos;
	clave = ftok ("/tmp", 1);
    if (clave == (key_t) -1)
        perror("La clave no se ha creado bien\n");
    idColaProcesos = msgget(clave,0666);
    if (idColaProcesos == -1) 
        perror("La cola de mensajes no se ha obtenido bien\n");
    msgsnd(idColaProcesos, &procesoAEncolar, (sizeof(struct proceso)-sizeof(long)),IPC_NOWAIT);
}
void termina (int signum)
{
    alarm(0);
    if(!noMasInput)
    {
		printf("El programa ha terminado\n");
		cuantosEnCola--;
		algoEnEjecucion = false;
		haTerminado = true;
	}
	noMasInput = false;
}

void liberaColaMensajes(int signum){}

void aumentaCuantosEnCola (int signum)
{
	cuantosEnCola++;
}

void alarmaRoundRobin(int signum)
{
	printf("he cortado al hijo\n");
	kill(pidManejador,SIGCONT);
}

void cierraElTeclado(int signum)
{
	noMasInput = true;
}
