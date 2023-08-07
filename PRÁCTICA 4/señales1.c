#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
int contador = 0; //variable global para que la funcion pueda acceder
void manejadorPadre(int signum);
void manejadorHijo(int signum);
int main()
{
    int pid,status;
    pid = fork();
    while (contador < 10)
    {
        if (pid > 0)
        {
            signal(SIGUSR1,manejadorPadre);
            sleep(3);
            kill(pid,SIGUSR2);
            pause();
            printf("ITERACION: %d\n", contador);
        }
        else if(pid == 0)
        {
            signal(SIGUSR2,manejadorHijo);
            sleep(1);
            pause();
            kill(getppid(),SIGUSR1);
            printf("ITERACION: %d\n", contador);
        }
    }
    if (pid > 0)
    {
        waitpid(pid,&status,0);
        printf("El hijo ha terminado\n");
        printf("El padre ha terminado\n");
    }
    return 0;
}
void manejadorPadre(int signum) //numero que identifica la señal
{
    contador++;
}
void manejadorHijo(int signum) //numero que identifica la señal
{
    contador++;
}
