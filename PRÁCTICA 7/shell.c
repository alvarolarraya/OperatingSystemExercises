#include "fragmenta.h"
#include <stdbool.h>
#include <fcntl.h>
#include <signal.h>


//strcount  compara strings
//dup2      cambia salida de fichero
//execvp       ejecuta los cadenas
//strtok        corta los strings
//calloc(numeroLetras+1,sizeof(tipoquesereserva))        reserva memoria
//strcpy        copiar strings
// remove pathFichero  para borrar ficheros

void hacerExec(char** comando);
int buscarOperador(char** comando);
void buscarDosOperadores(char ** comando, int * posicionOperadorUno, int * posicioOperadorDos);
void ejecutarUnOperando(char ** comando);
int contarOperadores(char ** comando);
#define RED  "\x1b[31m"
#define COLOR_RESET  "\x1b[0m"

int main()
{
    int pid;
    char* cadena;
    char** comando;
    while(1)
    {
        //printeo el prompt del programa en rojo
        printf(RED"minishell""\\""> "COLOR_RESET);
        //reservo memoria donde poder guardar el comando
        if ((cadena = malloc(sizeof(char*))) == NULL)
            perror("no hay memoria para reservar el comando en el programa principal\n");
        //pido al usuario que introduzca una cadena de caracteres
        fgets(cadena,100,stdin);
        //si el comando es exit termino el programa
        //para comprobar si el comando es exit utilizo strcmp ya que con operadores lógicos
        //lo que comparo es si los punteros apuntan a la misma direccion de memoria
        if(strcmp(cadena, "exit\n") == 0)
        {
            printf("Se cerró la minishell\n");
            exit(0);
        }
        //troceo la cadena por espacios en blanco
        comando = fragmenta(cadena);
        //creo un proceso hijo para ejecutar el comando
        //ya que execvp se apropia del proceso (lo mata)
        pid = fork();
        if(pid == -1)
        {
            perror("No se ha creado bien el proceso hijo del programa principal\n");
            exit(-1);
        }
        else if(pid == 0)
        {
            //proceso hijo
            //analizo la instruccion y la ejecuto
            int numeroOperadores;
            //contar el numero de operadores
            //la funcion devuelve 0,1 o 2
            numeroOperadores = contarOperadores(comando);
            //si hay un operador
            if(numeroOperadores == 1)
            {
                ejecutarUnOperando(comando);
            }
            else if(numeroOperadores == 2)
            {
                int posicionOperadorUno,posicionOperadorDos,i,descriptorFichero,pid_2,status2;
                buscarDosOperadores(comando,&posicionOperadorUno,&posicionOperadorDos);
                //cuento el numero de palabras en comando
                printf("El fichero que se quiere abrir es: %s", comando[posicionOperadorDos+1]);
                descriptorFichero = open(comando[posicionOperadorDos+1],O_WRONLY); 
                if(descriptorFichero == -1)
                    perror("no se ha abierto bien el fichero");
                if(dup2(descriptorFichero,STDOUT_FILENO) == -1)
                {
                    perror("error al redireccionar la salida estandar");
                    exit(-1);
                }
                char ** comandoIzquierda;
                pid_2 = fork();
                if(pid_2 == -1)
                    perror("ha habido un problema con el fork()");
                if ((comandoIzquierda = malloc((posicionOperadorUno+2)*sizeof(char*))) == NULL)
                    perror("no hay memoria para reservar el operando izquierdo\n");
                else if(pid_2 == 0)
                {
                    //doy valor a la variable de recorrido para copiar el comando hasta el primer 
                    //operador en comandoIzquierda
                    i = 0;
                    //copio el operando de la izquierda en comandoIzquierda
                    while(i < posicionOperadorUno)
                    {
                        comandoIzquierda[i] = comando[i];
                        i++;
                    }
                    comandoIzquierda[i] = comando[posicionOperadorUno+1];
                    comandoIzquierda[i+1] = NULL;
                    hacerExec(comandoIzquierda);
                }
                else
                {
                    waitpid(pid_2,&status2,0);
                    borrarg(comandoIzquierda);
                    close(descriptorFichero);
                    kill(getpid(),SIGKILL);
                }
            }
            //si no hay operador
            else if(numeroOperadores == 0)
                hacerExec(comando);
            else
                perror("La sintaxis del comando no es correcta");
        }
        else
        {
            int status;
            //proceso padre
            //espero a que termine el hijo para liberar la memoria usada y
            //esperar a la siguiente orden
            waitpid(pid,&status,0);
            //libero la memoria utilizada para almacenar el comando
            borrarg(comando);
            //libero la memoria de la cadena
            free(cadena);
        }
    }
}

/*
 *  FUNCION: contarOperadores
 *  ENTRADA: cola de cadena de caracteres
 *  REQUISITOS: Ninguno
 *  SALIDA: devuelve el numero de operadores que hay en la cola (0,1 o 2)
 */
int contarOperadores(char ** comando)
{
    int numeroOperadores = 0,i = 0;
    while(comando[i] != NULL)
    {
        if((strcmp(comando[i], ">") == 0) || (strcmp(comando[i], "<") == 0) || (strcmp(comando[i], ">>") == 0) || (strcmp(comando[i], "|") == 0))
            numeroOperadores++;
        i++;
    }
    return numeroOperadores;
}

/*
 *  FUNCION: buscarOperador
 *  ENTRADA: cola de cadena de caracteres
 *  REQUISITOS: Ninguno
 *  SALIDA: si hay un operador devuelve la posición en la que se encuentra, sino
 *  devuelve un -1
 */
int buscarOperador(char** comando)
{
    int i = 0;
    //recorro el comando hasta que se acabe o encuentre un operando
    while((comando[i] != NULL) && (strcmp(comando[i], ">") != 0) && (strcmp(comando[i], "<") != 0) && (strcmp(comando[i], ">>") != 0) && (strcmp(comando[i], "|") != 0))
        i++;
    //compruebo si se ha encontrado un operando
    //si no ha encontrado un operador devuelvo -1
    if (comando[i] == NULL)
          return -1;
    //si ha encontrado un operador devuelvo su posición
    else
          return i;
}

/*
 *  FUNCION: buscarOperador
 *  ENTRADA: cola de cadena de caracteres
 *  REQUISITOS: Ninguno
 *  SALIDA: si hay un operador devuelve la posición en la que se encuentra, sino
 *  devuelve un -1
 */
void buscarDosOperadores(char ** comando, int * posicionOperadorUno, int * posicionOperadorDos)
{
    int i = 0;
    //recorro el comando hasta que se acabe o encuentre un operando
    while((comando[i] != NULL) && (strcmp(comando[i], ">") != 0) && (strcmp(comando[i], "<") != 0) && (strcmp(comando[i], ">>") != 0) && (strcmp(comando[i], "|") != 0))
        i++;
    *posicionOperadorUno = i;
    i++;
    while((comando[i] != NULL) && (strcmp(comando[i], ">") != 0) && (strcmp(comando[i], "<") != 0) && (strcmp(comando[i], ">>") != 0) && (strcmp(comando[i], "|") != 0))
        i++;
    *posicionOperadorDos = i;
}

/*
 *  ACCIÓN: hacerExec
 *  ENTRADA: cola de cadena de caracteres
 *  REQUISITOS: Ninguno
 *  SALIDA: ejecuta el comando de entrada
 */
void hacerExec(char** comando)
{
    if(-1 == execvp(comando[0],comando))
    {
        perror("No se ha podido ejecutar el comando correctamente (error en el exec)");
        while(1)
        {
            exit(-1);
        }
    }
}

/*
 *  ACCIÓN: ejecutarUnOperando
 *  ENTRADA: cola de cadena de caracteres
 *  REQUISITOS: Ninguno
 *  SALIDA: analiza y ejecuta el comando de entrada
 */
void ejecutarUnOperando(char ** comando)
{
    //le pido a la funcion que me devuelva la posicion en la que se encuentra el operador
    //si es que hay operador
    int posicionOperador;
    posicionOperador = buscarOperador(comando);
    if(posicionOperador == -1)
        perror("ha fallado la función que cuenta operadores");
    char ** comandoIzquierda;
    char ** comandoDerecha;
    int i = posicionOperador,j = 0,numeroArgumentos = 0;
    //cuento cuantos string necesito reservar para el operando de la derecha
    while(comando[i] != NULL)
    {
        numeroArgumentos++;
        i++;
    }
    //reservo memoria para los operandos
    if ((comandoIzquierda = malloc((posicionOperador+1)*sizeof(char*))) == NULL)
        perror("no hay memoria para reservar el operando izquierdo\n");
    if ((comandoDerecha = malloc((numeroArgumentos+1)*sizeof(char*))) == NULL)
        perror("no hay memoria para reservar el operando derecho\n");
    //copio el comando a partir del operador en comandoDerecha
    i = posicionOperador+1;
    while(comando[i] != NULL)
    {
        comandoDerecha[j] = comando[i];
        i++;
        j++;
    }
    comandoDerecha[j] = NULL;
    //guardo la ultima posicion
    int ultimaPosicion = j;
    //doy valor a las variables de recorrido para copiar el comando hasta el operador
    //en comandoIzquierda
    i = 0;
    //copio el operando de la izquierda en comandoIzquierda
    while(i < posicionOperador)
    {
        comandoIzquierda[i] = comando[i];
        i++;
    }
    comandoIzquierda[i] = NULL;
    //creo un proceso con el que liberar la memoria de comandoIzquierda y comandoDerecha
    //una vez ejecutados
    int pid_2,status2;
    pid_2 = fork();
    if(pid_2 == -1)
    {
        perror("No se ha creado bien el proceso hijo del programa principal\n");
        exit(-1);   
    }
    if(pid_2 == 0)
    {
        int descriptorFichero;
        if(strcmp(comando[posicionOperador], "|") == 0)
        {
            //creo el fichero tuberia
            //le doy permiso a todo el mundo a leer,escribir y ejecutar tuberia
            descriptorFichero = open("tuberia",O_RDWR|O_CREAT|O_EXCL,777);
            //compruebo si se ha abierto correctamente
            if(descriptorFichero == -1)
            {
                perror("fallo en el open");
                exit(-1);
            }
            int pid_3,status3;
            //creo otro proceso para ejecutar un operando por proceso
            pid_3 = fork();
            if(pid_3 == -1)
                perror("Se ha creado mal el hijo");
            else if(pid_3 == 0)
            {
                //proceso que ejecuta comandoIzquierda
                //redirecciono la salida estandar a la tuberia
                if(dup2(descriptorFichero,STDOUT_FILENO) == -1)
                {
                    perror("error al redireccionar la salida estandar");
                    exit(-1);
                }
                //ejecuto el operando de la izquierda
                hacerExec(comandoIzquierda);
            }
            else
            {
                //proceso que ejecuta comandoDerecha
                //espero a que termine de ejecutarse el hijo
                waitpid(pid_3,&status3,0);
                //ejecuto el comando a partir del operador
                //con la tuberia como argumento
                comandoDerecha[ultimaPosicion] = "tuberia";
                comandoDerecha[ultimaPosicion+1] = NULL;
                //ejecuto el comando de la derecha
                hacerExec(comandoDerecha);
            }
        }
        else if(strcmp(comando[posicionOperador], ">") == 0)
        {
            //ejecuto comando de la izquierda y lo escribo en el fichero
            //que me indica comandoDerecha
            descriptorFichero = open(comandoDerecha[0],O_WRONLY);
            //compruebo si se ha abierto correctamente
            if(descriptorFichero == -1)
            {
                perror("fallo en el open");
                exit(-1);
            }
            //redirecciono la salida estandar al fichero
            if(dup2(descriptorFichero,STDOUT_FILENO) == -1)
            {
                perror("error al redireccionar la salida estandar");
                exit(-1);
            }
            //ejecuto el operando de la izquierda
            hacerExec(comandoIzquierda);
        }
        else if(strcmp(comando[posicionOperador], ">>") == 0)
        {
            //ejecuto comando de la izquierda y lo escribo en el fichero
            //que me indica comandoDerecha
            descriptorFichero = open(comandoDerecha[0],O_WRONLY|O_APPEND);
            //compruebo si se ha abierto correctamente
            if(descriptorFichero == -1)
            {
                perror("fallo en el open");
                exit(-1);
            }
            //redirecciono la salida estandar al fichero
            if(dup2(descriptorFichero,STDOUT_FILENO) == -1)
            {
                perror("error al redireccionar la salida estandar");
                exit(-1);
            }
            //ejecuto el operando de la izquierda
            hacerExec(comandoIzquierda);
        }
        else if(strcmp(comando[posicionOperador], "<") == 0)
        {
            descriptorFichero = open(comandoDerecha[0],O_RDONLY);
            if(descriptorFichero == -1)
            {
                perror("fallo en el open");
                exit(-1);
            }
            if(dup2(descriptorFichero,STDIN_FILENO) == -1)
            {
                perror("error al redireccionar la salida estandar");
                exit(-1);
            }
            hacerExec(comandoIzquierda);
        }
    }
    else
    {
        waitpid(pid_2,&status2,0);
        borrarg(comandoDerecha);
        borrarg(comandoIzquierda);
        if(strcmp(comando[posicionOperador], "|") == 0)
        {
            //borro la tuberia
            if(-1 == remove("tuberia"))
            {
                perror("No se ha podido borrar la tuberia correctamente");
                while(1)
                    exit(-1);
            }   
        }
        kill(getpid(),SIGKILL);
    }
}
