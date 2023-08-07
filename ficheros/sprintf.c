#include <stdio.h>

int main()
{
  char mensaje[100];
  sprintf(mensaje,"%d, %d, %d", 42,35,62);
  sprintf(mensaje,"hola me llamo alvaro");
  printf("%s", mensaje);
}
