#include <stdio.h>
#include <ctype.h>

void copiarFichero(char* origen, char* destino);

int main(int argc,char** argv)
{
  FILE* fichero;
  FILE* auxiliar;
  char texto[100];
  char* nombreFichero = argv[1];
  char caracter;
  int i;
  if((fichero = fopen(nombreFichero,"r")) == NULL)
    perror("ha habido un fallo abriendo el fichero");
  if((auxiliar = fopen("auxiliar.txt","a")) == NULL)
    perror("ha habido un fallo abriendo el fichero");
  fgets(texto,100,fichero);
  while(feof(fichero) == 0)
  {
    i = 1;
    caracter = toupper(texto[0]);
    while(texto[i] != '\n')
    {
      fprintf(auxiliar,"%c",caracter);
      caracter = toupper(texto[i]);
      i++;
    }
    fprintf(auxiliar,"%c\n",caracter);
    fgets(texto,100,fichero);
  }
  fclose(fichero);
  fclose(auxiliar);
  remove(nombreFichero);
  copiarFichero("auxiliar.txt",nombreFichero);
  remove("auxiliar.txt");
  return 0;
}

void copiarFichero(char* origen, char* destino)
{
  char texto[100];
  FILE *ficheroOrigen, *ficheroDestino;
  ficheroOrigen = fopen(origen, "r");
  ficheroDestino = fopen(destino,"a");
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
