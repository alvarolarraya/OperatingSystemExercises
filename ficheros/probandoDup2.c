#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

//despues del dup2 SOLO escribe en el fichero que se le dice

int main()
{
  int descriptorFichero;
  descriptorFichero = open("escribeAqui.txt",O_WRONLY|O_CREAT);
  dup2(descriptorFichero, STDOUT_FILENO);
  if(dup2(descriptorFichero,STDOUT_FILENO) == -1)
  {
      perror("error al redireccionar la salida estandar");
      exit(-1);
  }
  printf("esto se escribe en el fichero solo");
  return 0;
}
