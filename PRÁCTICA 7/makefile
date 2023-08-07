all: fragmenta prueba ejecuta
fragmenta: fragmenta.c
	gcc -Wall -c fragmenta.c
prueba: shell.c fragmenta.o
	gcc -Wall -o shell shell.c fragmenta.o
clean:
	rm shell fragmenta.o
ejecuta:
	./shell
