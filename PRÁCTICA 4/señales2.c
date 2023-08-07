#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>

void parar (int signum);
void reiniciar(int signum);
bool parado = false;
int segundos = 0,minutos = 0;


int main()
{
    int pid,teclado;
    pid = fork();
    if (pid > 0)
    {
        while(1)
        {
            scanf("%d",&teclado);
            if (teclado == 0)
            {
                kill(pid,SIGKILL);
                printf("Proceso contador terminado\n");
                printf("Proceso gestor va a terminar\n");
                exit(1);
            }
            else if(teclado == 1)
                kill(pid,SIGUSR1);
            else if(teclado == 2)
                kill(pid,SIGUSR2);
            else
                printf("No hay señal asignada a esa tecla pruebe con otra\n");
        }
    }
    else if (pid == 0)
    {
        while (1)
        {
            if(!parado)
            {
                signal(SIGUSR1,parar);
                signal(SIGUSR2,reiniciar);
                sleep(1);
                segundos++;
                if (segundos == 60)
                {
                    minutos++;
                    segundos = 0;
                }
                printf("%d:%d\n", minutos,segundos);
            }
            else
                pause();
        }
    }
    return 0;
}


void parar (int signum)
{
    if (!parado)
        parado = true;
    else
        parado = false;
    printf("El contador ahora mismo va %d minutos y %d segundos\n", minutos, segundos);
    return;
}
void reiniciar(int signum)
{
    minutos = 0;
    segundos = 0;
    return;
}
