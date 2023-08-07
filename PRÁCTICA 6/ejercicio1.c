//argumentos productor   clave,tipo,numeroMensajes
//argumentos consumidor clave,tipo

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf {
    long tipo;
    char * m;   
};

int main(int argc,char * argv[])
{
    int pid,idColaMensajes,numeroMensajes,i;
    key_t clave; 
    struct msgbuf msgBuffer;
    numeroMensajes = atoi(argv[2]);
    clave = ftok ("/tmp", atoi(argv[1]));
    if (clave == (key_t) -1)
        perror("La clave no se ha creado bien\n");
    idColaMensajes = msgget (clave, 0600 | IPC_CREAT);
    if (idColaMensajes == -1) 
        perror("La cola de mensajes se ha creado mal\n");
    msgBuffer.tipo = 1;
    pid = fork();
    if (pid == 0)
    {
        for(i = 0;i<numeroMensajes; i++)
        {
            printf("%d) ", i+1);
            msgBuffer.m = "esto es el mensaje";
            msgsnd (idColaMensajes, &msgBuffer, sizeof(struct msgbuf)-sizeof(long), IPC_NOWAIT);
            printf("El productor ha mandado el mensaje: ");
            printf("%s\n", msgBuffer.m);
        }
    }
    else if (pid > 0)
    {
        for(i = 0;i<numeroMensajes; i++)
        {
            msgrcv (idColaMensajes, &msgBuffer, sizeof(struct msgbuf) -sizeof(long),1,0);
        }
    }
    else
        perror("El hijo se ha creado mal\n");
    return 0;
}
