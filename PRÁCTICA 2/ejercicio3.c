//Este algoritmo crea un arbol de procesos binario completo de altura tres
//NOTA: acuerdate de que al hacer un fork en una funcion/accion, este se queda ahi no vuelve al main.
//Se puede mejorar la salida a pantalla con una pila, para que salgan los nombres de los nodos por orden de nivel y no de creacion.


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


typedef struct nodo
{
    int nivel;
    int pid;
    int ppid;
    char letra;
}nodo;


void resumen(nodo p);
void duplicate(nodo raiz, char letras[], int cuantasLetras);
void eliminaLetras(char letras[], int cuantasLetras);


int main()
{
    nodo raiz,A,B,C,D,E,F;
    char letras[6] = "ABCDEF";
    int cuantasLetras = sizeof(letras) / sizeof(letras[0]);
    raiz.letra = '/';
    raiz.pid = getpid();
    raiz.ppid = getppid();
    raiz.nivel = 1;
    duplicate(raiz,letras,cuantasLetras);
    return 0;
}


void resumen(nodo p)
{
        printf("Se ha creado el proceso %c en el nivel %d, con pid: %d y ppid: %d\n", p.letra, p.nivel, p.pid, p.ppid);
        return;
}


void duplicate(nodo raiz, char letras[], int cuantasLetras)
{
    int i, status, pid;
    nodo A,B;
    resumen(raiz);
    if (raiz.nivel<3 && cuantasLetras>0)
    {
        A.nivel = raiz.nivel+1;
        B.nivel = raiz.nivel+1;
        pid = fork();
        if (pid == -1) 
        {
            printf("Un proceso se ha creado mal\n");
            exit (-1);
        }
        else if (pid == 0)
        {
            A.pid = getpid();
            A.ppid = getppid();
            A.letra = letras[0];
            cuantasLetras = cuantasLetras-1;
            eliminaLetras(letras, cuantasLetras);
            cuantasLetras = cuantasLetras-1;
            eliminaLetras(letras, cuantasLetras);
            duplicate(A,letras,cuantasLetras);
            exit(1);
        }
        else
        {
            waitpid(pid, &status, 0);
            pid = fork();
            if (pid == -1) 
            {
                printf("Un proceso se ha creado mal\n");
                exit (-1);
            }
            else if (pid == 0)
            {
                cuantasLetras = cuantasLetras-1;
                eliminaLetras(letras, cuantasLetras);
                B.pid = getpid();
                B.ppid = getppid();
                B.letra = letras[0];
                cuantasLetras = cuantasLetras-1;
                eliminaLetras(letras, cuantasLetras);
                cuantasLetras = cuantasLetras-1;
                eliminaLetras(letras, cuantasLetras);
                duplicate(B,letras,cuantasLetras);
                exit(2);
            }
            else
                waitpid(pid, &status, 0);
        }
    }
    return;
}


void eliminaLetras(char letras[], int cuantasLetras)
{
	for(int i = 0; i < cuantasLetras; i ++)
    {
		letras[i] = letras[i + 1];
	}
	return;
}
