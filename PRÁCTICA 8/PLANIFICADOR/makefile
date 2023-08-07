all: fragmenta compilaEncolador ejecuta
fragmenta: fragmenta.c
	gcc -c fragmenta.c
compilaEncolador: encolador.c fragmenta.c
	gcc -g -Wall encolador.c fragmenta.c -o ejercicio
clean:
	rm ejercicio fragmenta.o
ejecuta:
	./ejercicio
