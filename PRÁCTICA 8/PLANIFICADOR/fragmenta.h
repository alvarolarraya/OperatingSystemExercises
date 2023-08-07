/*
 *	MODULO: fragmenta
 *	FICHERO: fragmenta.h
 *	Creado por Álvaro Larraya Conejo el 21/10/20.
 *  DESCRIPCION: Este modulo exporta las funcionalidades necesarias para trocear una string
 *  en una cadena de strings.
 */
#ifndef ALC_FRAGMENTA_H
#define ALC_FRAGMENTA_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 *  FUNCION: fragmenta
 *  ENTRADA: cadena de caracteres
 *  REQUISITOS: Ninguno
 *  SALIDA: un array de strings compuesto por los caracteres de la cadena de entrada sin los 
 *  espacios en blanco
 */
char** fragmenta(const char*);

/*
 *  ACCION: borrarg
 *  ENTRADA: un array de strings
 *  REQUISITOS: Ninguno
 *  SALIDA: la memoria anteriormente reservada para crear la estructura de entrada es liberada
 */
 void borrarg(char **);

#endif