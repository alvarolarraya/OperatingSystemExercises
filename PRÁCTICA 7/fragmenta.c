#include "fragmenta.h"

char** fragmenta(const char* cadena)
{
    //cuento cuantos caracteres tiene la cadena de entrada e inicializo
    //la variable que cuenta el numero de palabras a cero
    //ya que tal y como esta implementado se cuenta NULL como palabra
    //así que para compensar no cuento la primera palabra de la cadena
    int cuantosCaracteres = strlen(cadena),cuantasPalabras = 0,i = 0;
    char* cadenaAuxiliar;
    while(i<cuantosCaracteres)
    {
        if(cadena[i] == ' ')
        {
            //Salto todos los caracteres que sean espacio en blanco
            while(cadena[i] == ' ')
                i++;
            //aumento el contador de palabras
            cuantasPalabras++;
        }
        i++;
    }
    //como el puntero a la cadena de entrada es una constante no puedo manipularla
    //reservo memoria para la cadena auxiliar con la que voy a operar
    if ((cadenaAuxiliar = malloc((cuantosCaracteres)*sizeof(char))) == NULL)
        perror("no hay memoria para reservar la cadena auxiliar en fragmenta\n");
    //copio la cadena en otra auxiliar pero sin el \n
    i = 0;
    while(cadena[i] != '\n')
    {
        cadenaAuxiliar[i] = cadena[i];
        i++;
    }
    cadenaAuxiliar[i] = ' ';
    char** vectorDeCadenas;
    //reservo memoria para la cola que devuelve la funcion
    if ((vectorDeCadenas = malloc(cuantasPalabras*sizeof(char*))) == NULL)
        perror("no hay memoria para reservar el vector de cadenas en fragmenta\n");
    //Declaro una variable apuntador a caracter para comprobar la salida de strtok
    char* argumento;
    //reservo memoria para la variable con la que compruebo la salida de strtok
    if ((argumento = malloc(sizeof(char*))) == NULL)
        perror("no hay memoria para reservar el argumento en fragmenta\n");
    //si argumento es NULL significa que ya ha recorrido la cadena
    argumento = strtok(cadenaAuxiliar," ");
    vectorDeCadenas[0] = argumento;
    i = 1;
    while(argumento != NULL)
    {
        argumento = strtok(NULL," ");
        vectorDeCadenas[i] = argumento;
        i++;
    }
    //libero la memoria de la variable que he utilizado para pasar los string
    //de la cadena auxiliar a la cola de cadenas
    free(argumento);
    //al terminar el vector de cadenas esta formado por los argumentos
    //separados por el lugar donde anteriormente habia espacios en blanco
    return vectorDeCadenas;
}
void borrarg(char **arg)
{
    free(arg);
}
