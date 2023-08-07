/*Lo que hace este programa es abrir un fichero de texto esperar 5 segundos y escribir Hola mundo en el*/
#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
    if (argc != 2)
        printf("Ha olvidado llamar al fichero\n");
        exit(1); //0 para cuando ha terminado bien como parametro en exit, cualquier otro si no
    FILE* f;
    f = fopen(argv[1],"w");//dentro de argv 1 para fichero.txt/ 0 para nombre de fichero
    sleep(5);
    fputs("Hola mundo",f);
    fclose(f);
    exit(0);
}
//Para compilar: gcc ejemplo.c -o ejemplo
//Para ejecutar: ./ejemplo hola
//Siendo ejemplo el programa este y hola un fichero de texto (se puede crear con kate, haciendo un nuevo archivo y guardando simplemente)
