#include <stdio.h>
#include <unistd.h>

#define RED  "\x1b[31m"
#define COLOR_RESET  "\x1b[0m"

int main()
{
    printf(RED"Primera iteracion"COLOR_RESET);
    printf("\n");
    sleep(6);
    printf(RED"Segunda iteracion"COLOR_RESET);
    printf("\n");
    sleep(6);
    printf(RED"Tercera iteracion"COLOR_RESET);
    printf("\n");
}
