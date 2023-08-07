#include "fragmenta.h"
#include <signal.h>
#include <fcntl.h>

void pasarFiltro(char* fichero, char* filtro);
void copiarFichero(char* origen, char* destino);

int main(int argc, char** argv)
{
  //intentar ver si hay fichero y si eso leer de él
  char quiereSalir;
  char* filtroUno;
  char* filtroDos;
  char* filtroTres;
  char* fichero;
  int descriptorFichero,pid,status;
  filtroUno = argv[1];
  filtroDos = argv[2];
  filtroTres = argv[3];
  if (argc > 4)
    fichero = argv[4];
  else
    perror("se le ha olvidado  adjuntarme el fichero");
  pasarFiltro(fichero,filtroUno);
  pasarFiltro("ficheroFiltrado.txt",filtroDos);
  pid = fork();
  if(pid == -1)
  {
    fprintf(stderr, "no se ha creado bien el proceso hijo");
    exit(-1);
  }
  else if(pid == 0)
  {
    printf("hola");
    fflush(stdout);
    execlp("grep", "grep", filtroTres,"ficheroFiltrado.txt", NULL);
  }
  else
    waitpid(pid,&status, 0);
  //remove("ficheroFiltrado.txt");
}

void pasarFiltro(char* fichero, char* filtro)
{
  int pid,status,descriptorFichero;
  descriptorFichero = open("ficheroAuxiliar.txt",O_RDWR|O_CREAT,777);
  pid = fork();
  if(pid == -1)
  {
    fprintf(stderr, "no se ha creado bien el proceso hijo");
    exit(-1);
  }
  else if(pid == 0)
  {
    if(dup2(descriptorFichero,STDOUT_FILENO) == -1)
    {
        perror("error al redireccionar la salida estandar");
        exit(-1);
    }
    execlp("grep", "grep", filtro,fichero, NULL);
  }
  else
    waitpid(pid,&status, 0);
  copiarFichero("ficheroAuxiliar.txt","ficheroFiltrado.txt");
  remove("ficheroAuxiliar.txt");
}

void copiarFichero(char* origen, char* destino)
{
  char texto[100];
  FILE *ficheroOrigen, *ficheroDestino;
  ficheroOrigen = fopen(origen, "r");
  ficheroDestino = fopen(destino,"w+");
  if(ficheroOrigen == NULL)
    perror("Error abriendo el fichero origen");
  if(ficheroDestino == NULL)
    perror("Error abriendo el fichero destino");
  while(feof(ficheroOrigen) == 0)
  {
    fgets(texto,100,ficheroOrigen);
    if(feof(ficheroOrigen) == 0)
      fputs(texto,ficheroDestino);
  }
}
