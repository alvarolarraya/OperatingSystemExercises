//crea un fichero con el nombre que se le pasa como argumento o usa un que ya exista y escribe
//al final el pid del proceso y lo que esté en la variable texto

#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
  char texto[100] = "Hola mundo\n";
  FILE* fichero;
  char* nombreFichero = argv[1];
  if((fichero = fopen(nombreFichero,"a")) == NULL)
    perror("no se ha abierto bien el fichero");
  fprintf(fichero,"%d\t",getpid());
  fputs(texto,fichero);
  printf("ya se ha escrito en el fichero\n");
  fclose(fichero);
  return 0;
}
