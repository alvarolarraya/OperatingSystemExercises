#include <stdio.h>
#include <unistd.h>

#define RED  "\x1b[31m"
#define COLOR_RESET  "\x1b[0m"

int main()
{
    printf(RED"Se ha ejecutado el programa1"COLOR_RESET);
    printf("\n");
}
