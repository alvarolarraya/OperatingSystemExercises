//Este programa crea una zona de memoria compartida de un entero y la inicializa en el proceso principal
//crea dos hijos que aumentan el entero y el propio proceso padre tambn la aumenta
//el proceso principal acaba despidiendose y mostrando el entero de la memoria compartida
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
  union semun
  {
    int val;
    struct semid_ds *buf;
    ushort *array;
  };
  key_t key = ftok("/tmp",100);
  if(key == -1)
    perror("no se ha creado bien la clave");
  int shmid = shmget(key,sizeof(int),IPC_CREAT|0660);
  if (shmid < 0)
    perror("no se ha creado bien la memoria compartida\n");
  int* enteroCompartido = (int*) shmat(shmid,0,0);
  if(enteroCompartido == (int*) -1)
  {
    perror("no se ha vinculado bien con la memoria compartida\n");
  }
  (*enteroCompartido) = 0;
  int semid = semget(key,1,IPC_CREAT);
  union semun arg;
  arg.val = 1;
  semctl(semid,0,SETVAL,arg);
  int pidHijo1 = fork();
  if(pidHijo1 == -1)
    perror("no se ha creado bien el hijo1\n");
  else if(pidHijo1 == 0)
  {
    struct sembuf accion;
    accion.sem_num = 0;
    accion.sem_op = -1;
    accion.sem_flg = 0;
    semop(semid,&accion,1);
    (*enteroCompartido) ++;
    accion.sem_op = 1;
    semop(semid,&accion,1);
    kill(getpid(),SIGKILL);
  }
  else
  {
    int pidHijo2 = fork();
    if(pidHijo2 == -1)
      perror("no se ha creado bien el hijo2\n");
    else if(pidHijo2 == 0)
    {
      struct sembuf accion;
      accion.sem_num = 0;
      accion.sem_op = -1;
      accion.sem_flg = 0;
      semop(semid,&accion,1);
      (*enteroCompartido) ++;
      accion.sem_op = 1;
      semop(semid,&accion,1);
      kill(getpid(),SIGKILL);
    }
    else
    {
      int statusHijo1,statusHijo2;
      waitpid(pidHijo1,&statusHijo1,0);
      waitpid(pidHijo2,&statusHijo2,0);
      struct sembuf accion;
      accion.sem_num = 0;
      accion.sem_op = -1;
      accion.sem_flg = 0;
      semop(semid,&accion,1);
      (*enteroCompartido) ++;
      accion.sem_op = 1;
      semop(semid,&accion,1);
    }
  }
  printf("la memoria compartida tiene valor %d\n", *enteroCompartido);
  semctl(semid, 0, IPC_RMID);
  shmdt(enteroCompartido);
  shmctl(shmid, IPC_RMID, NULL);
  printf("he eliminado el semaforo y la memoria compartida\n");
  printf("ciao\n");
  return 0;
}
