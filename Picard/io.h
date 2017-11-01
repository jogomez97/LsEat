#ifndef   _IO_H_
#define   _IO_H_

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "dades.h"

#define ERROR_FILE1 "Error! No s'ha pogut obrir el fitxer especificat.\n"
#define ERROR_FILE2 "Error! El fitxer especificat està buit o no compleix el format.\n"

//Funcions de lectura
int readFile(char* path, Picard* picard);
char* readUntil(int fd, char cFi);
char* readKB();
int inputFlush();

//Funcions d'escriptura
void printWelcome(char* name);
void printMoney(int money);
void printShell(char* name);


#endif
