#include "fragmenta.h"

char** fragmenta(const char* cadena)
{
    int cuantosCaracteres = strlen(cadena),cuantasPalabras = 0,i = 0;
    char* cadenaAuxiliar;
    while(i<cuantosCaracteres)
    {
        if(cadena[i] == ' ')
        {
            while(cadena[i] == ' ')
                i++;
            cuantasPalabras++;
        }
        i++;
    }
    if ((cadenaAuxiliar = malloc((cuantosCaracteres)*sizeof(char))) == NULL)
        perror("no hay memoria para reservar la cadena auxiliar en fragmenta\n");
    i = 0;
    while(cadena[i] != '\n')
    {
        cadenaAuxiliar[i] = cadena[i];
        i++;
    }
    cadenaAuxiliar[i] = ' ';
    char** vectorDeCadenas;
    if ((vectorDeCadenas = malloc(cuantasPalabras*sizeof(char*))) == NULL)
        perror("no hay memoria para reservar el vector de cadenas en fragmenta\n");
    char* argumento;
    if ((argumento = malloc(sizeof(char*))) == NULL)
        perror("no hay memoria para reservar el argumento en fragmenta\n");
    argumento = strtok(cadenaAuxiliar," ");
    vectorDeCadenas[0] = argumento;
    i = 1;
    while(argumento != NULL)
    {
        argumento = strtok(NULL," ");
        vectorDeCadenas[i] = argumento;
        i++;
    }
    free(argumento);
    return vectorDeCadenas;
}
void borrarg(char **arg)
{
    free(arg);
}
