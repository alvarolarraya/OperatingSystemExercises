#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    int pipe_a[2],pipe_b[2];
    typedef struct mensaje
    {
        int secuencia,pidEmisor;
    }mensaje;
    mensaje m;
    int pid,status;
    m.secuencia = 0;
    pipe(pipe_a);
    pipe(pipe_b);
    pid = fork();
    if (pid == -1)
    {
        printf("El proceso hijo se ha creado mal\n");
        exit(-1);
    }
    while(m.secuencia<10)
    {
        if (pid > 0)
        {
            m.secuencia = m.secuencia+1;
            m.pidEmisor = getpid();
            write(pipe_a[1], &m, sizeof(m));
            printf("%d) Padre con id: %d ha mandado mensaje al hijo\n", m.secuencia, m.pidEmisor);
            read(pipe_b[0], &m, sizeof(m));
            printf("   Padre ha recibido mensaje del hijo con id: %d\n", m.pidEmisor);
        }
        else if (pid == 0)
        {
            read(pipe_a[0], &m, sizeof(m));
            printf("   Hijo ha recibido mensaje del padre con id: %d\n", m.pidEmisor);
            m.secuencia = m.secuencia+1;
            m.pidEmisor = getpid();
            write(pipe_b[1], &m, sizeof(m));
            printf("%d) Hijo con id: %d ha mandado mensaje al padre\n", m.secuencia, m.pidEmisor);
        }
    }
    close(*pipe_a);
    close(*pipe_b);
    if (pid > 0)
    {
        waitpid(pid, &status, 0);
        printf("\nEl hijo ha terminado\n");
        printf("El padre ha terminado\n");
    }
    return 0;
}
//SOLUCIÓN EJER2->   https://bit.ly/3m2qEHW
