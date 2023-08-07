//no termina de ir: la fifo que comunica el proceso 1 con el 3 no va bien no sé porque

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

void terminar(int signum);

typedef struct mensaje
{
  int numero;
}mensaje1;

int procesoPrincipal;

int main()
{
  procesoPrincipal = getpid();
  printf("La sucesión de Fibonacci es:\n");
  mkfifo("fifo12.txt",S_IFIFO|0660);
  mkfifo("fifo23.txt",S_IFIFO|0660);
  mkfifo("fifo13.txt",S_IFIFO|0660);
  int fifo12,fifo23,fifo13;
  if((fifo12 = open("fifo12.txt",O_RDWR)) == -1)
    perror("no se ha abierto bien la fifo12");
  if((fifo13 = open("fifo13.txt",O_RDWR)) == -1)
    perror("no se ha abierto bien la fifo13");
  if((fifo23 = open("fifo23.txt",O_RDWR)) == -1)
    perror("no se ha abierto bien la fifo23");
  int pid1 = fork();
  if(pid1 == -1)
    perror("no se ha creado bien el hijo");
  else if(pid1 == 0)
  {
    //proceso 1
    struct mensaje m;
    m.numero = 0;
    while(1)
    {
      if(write(fifo13,&m,sizeof(struct mensaje)) == -1)
        perror("no se ha escrito bien en la tuberia");
      if(read(fifo12,&m,sizeof(struct mensaje)) == -1)
        perror("error en la lectura de la fifo");
    }
  }
  else
  {
    int pid2 = fork();
    if(pid2 == -1)
      perror("no se ha creado bien el hijo");
    else if(pid2 == 0)
    {
      //proceso 2
      struct mensaje m;
      m.numero = 1;
      while(1)
      {
        if(write(fifo12,&m,sizeof(struct mensaje)) == -1)
          perror("no se ha escrito bien en la tuberia");
        if(write(fifo23,&m,sizeof(struct mensaje)) == -1)
          perror("no se ha escrito bien en la tuberia");
        if(read(fifo23,&m,sizeof(struct mensaje)) == -1)
          perror("error en la lectura de la fifo");
      }
    }
    else
    {
      //proceso 3
      struct mensaje m;
      signal(SIGINT,terminar);
      int entero1, entero2,suma;
      while(1)
      {
        if(read(fifo13,&m,sizeof(struct mensaje)) == -1)
          perror("error en la lectura de la fifo");
        entero1 = m.numero;
        printf("el primer entero es %d\n",entero1);
        if(read(fifo23,&m,sizeof(struct mensaje)) == -1)
          perror("error en la lectura de la fifo");
        entero2 = m.numero;
        printf("el segundo entero es %d\n",entero2);
        suma = entero1+entero2;
        sleep(2);
        printf("%d\n", suma);
        m.numero = suma;
        if(write(fifo23,&m,sizeof(struct mensaje)) == -1)
          perror("no se ha escrito bien en la tuberia");
      }
    }
  }
}

void terminar(int signum)
{
  remove("fifo12.txt");
  remove("fifo23.txt");
  remove("fifo13.txt");
  printf("\ncierro las fifo\n");
  kill(procesoPrincipal,SIGKILL);
}
