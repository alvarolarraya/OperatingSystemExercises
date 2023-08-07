#include "fragmenta.h"
#include <signal.h>
#include <fcntl.h>

void copiarFichero(char* origen, char* destino);

int main()
{
  copiarFichero("ficheroAuxiliar.txt","ficheroFiltrado.txt");
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
  fclose(ficheroOrigen);
  fclose(ficheroDestino);
}
