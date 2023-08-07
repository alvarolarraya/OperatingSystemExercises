//este programa abre un fichero con el nombre que se le pasa como parametro y imprime el contenido por pantalla

#include <stdio.h>

int main(int argc,char** argv)
{
  FILE* fichero;
  char texto[100];
  char* nombreFichero = argv[1];
  if((fichero = fopen(nombreFichero,"r")) == NULL)
    perror("ha habido un fallo abriendo el fichero");
  fgets(texto,100,fichero);
  while(feof(fichero) == 0)
  {
    printf("%s", texto);
    fgets(texto,100,fichero);
  }
  fclose(fichero);
  return 0;
}
