//NOTA: muy importante en los argumentos del main primero poner el argc y luego el argv, sino argc toma valores random.


#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    int status, funciona, pid, i;
    if (argc < 2)
    {
        printf("Se le ha olvidado darme el nombre del programa\n");
        exit(-1);
    }
    else
    {
        pid = fork();
        if (pid == -1)
        {
            printf("Ha habido un problema en la creacion del proceso hijo\n");
            exit(-1);
        }
        else if(pid == 0)
        {
            printf("Vamos a ejecutar el programa %s\n", argv[1]);
            if (argc>2)
            {
                char *argumentos[argc-2];
                for (i = 0; i<argc; i++)
                    argumentos[i] = argv[i+2];
                funciona = execv(argv[1], argumentos);
            }
            else
                funciona = execv(argv[1], argv);
            if (funciona == -1)
                printf("Ha habido un problema en la ejecucion del programa\n");
            exit(1);
        }
        else
            waitpid(pid, &status, 0);
    }
    exit(0);
}
