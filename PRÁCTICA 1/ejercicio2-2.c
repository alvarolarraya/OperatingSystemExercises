#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char* argr[])
{
    if (argc != 2)
        exit(1);
    int f;
    if ((f = open(argr[1],O_RDONLY)) == -1)
    {
        exit(2);
    }
    char buffer;
    int size = 1;
    while(read(f,&buffer,size)!=0)
    {
        printf("%c", buffer);
    }
    close(f);
    exit(0);
}
