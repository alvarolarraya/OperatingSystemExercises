#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Se te ha olvidado adjuntar el fichero o has adjuntado mas de uno\n");
        exit(1);
    }
    FILE* f;
    f = fopen(argv[1],"r+");
    while(feof(f) == 0)
    {
        char c;
        c = fgetc(f);
        printf("%c", toupper(c));
    }
}
