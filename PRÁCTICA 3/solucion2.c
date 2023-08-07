#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


#define RED  "\x1b[31m"
#define GREEN  "\x1b[32m"
#define COLOR_RESET  "\x1b[0m"

#define FIFONAME_A "P03_02_FIFO_A"
#define FIFONAME_B "P03_02_FIFO_B"

int main (){
    int bytes, fp_a, fp_b,pid;

    struct message {
        int count, pid;
    } message;

    message.count = 1;
    message.pid = getpid();

    mkfifo(FIFONAME_A, S_IFIFO|0660);
    mkfifo(FIFONAME_B, S_IFIFO|0660);
    pid = fork();

    if(pid > 0)
    {
        if((fp_a = open(FIFONAME_A, O_WRONLY)) == -1) {
            printf(RED"[2A - %d] ERROR opening write FIFO: %s\n"\
                COLOR_RESET, getpid(), strerror(errno));
            exit(-1);
        }
        else {
            printf(RED"[2A - %d] FIFO OPEN!\n"COLOR_RESET, getpid());
        }
        if((fp_b = open(FIFONAME_B, O_RDONLY)) == -1){
            printf(RED"[2A - %d] ERROR opening read FIFO: %s\n"\
                COLOR_RESET, getpid(), strerror(errno));
            exit(-1);
        }else{
            printf(RED"[2A - %d] FIFO OPEN!\n"COLOR_RESET, getpid());
        }

        while (message.count < 10){

        // WRITE
        printf(RED"[2A - %d] Writing[%d, %d] \n"\
                COLOR_RESET, getpid(), message.count, message.pid);
        if ((bytes = write(fp_a, &message, sizeof(message))) == -1){
            printf(RED"[2A - %d] ERROR writting in FIFO: %s\n"\
                    COLOR_RESET, getpid(), strerror(errno));
            exit(-1);
        }

        // READ
        if ((bytes = read(fp_b, &message, sizeof(message))) == -1){
            printf(RED"[2A - %d] ERROR reading FIFO: %s\n"COLOR_RESET,getpid(), strerror(errno));
            exit(-1);
        }
        printf(RED"[2A - %d] Reading from FIFO pipe [%d, %d] (%d bytes)\n"\
                COLOR_RESET, getpid(), message.count, message.pid, bytes);
        message.count++;
        message.pid = getpid();

        }
    }
    else if (pid == 0)
    {
        if((fp_a = open(FIFONAME_A, O_RDONLY)) == -1) {
        printf(GREEN"[2A - %d] ERROR opening read FIFO: %s\n"\
               COLOR_RESET, getpid(), strerror(errno));
        exit(-1);
        }
        else {
            printf(GREEN"[2A - %d] FIFO OPEN!\n"COLOR_RESET, getpid());
        }
        if((fp_b = open(FIFONAME_B, O_WRONLY)) == -1){
            printf(GREEN"[2A - %d] ERROR opening write FIFO: %s\n"\
                COLOR_RESET, getpid(), strerror(errno));
            exit(-1);
        }else{
            printf(GREEN"[2A - %d] FIFO OPEN!\n"COLOR_RESET, getpid());
        }

        while (message.count < 10){
        
            // READ
            if ((bytes = read(fp_a, &message, sizeof(message))) == -1){
                printf(GREEN"[2A - %d] ERROR reading FIFO: %s\n"COLOR_RESET,getpid(), strerror(errno));
                exit(-1);
            }
            printf(GREEN"[2A - %d] Reading from FIFO pipe [%d, %d] (%d bytes)\n"\
                COLOR_RESET, getpid(), message.count, message.pid, bytes);
      
            message.count++;
            message.pid = getpid();
            
            //WRITE
            printf(GREEN"[2A - %d] Writing[%d, %d] \n"\
             COLOR_RESET, getpid(), message.count, message.pid);
            if ((bytes = write(fp_b, &message, sizeof(message))) == -1){
                printf(GREEN"[2A - %d] ERROR writting in FIFO: %s\n"\
                 COLOR_RESET, getpid(), strerror(errno));
                exit(-1);
            }
        }
        exit(0);
    }
    else
        perror("El proceso hijo se ha creado mal\n");
    close(fp_a);
    close(fp_b);
    remove(FIFONAME_A);
    remove(FIFONAME_B);
    exit(0);
}
