#ifndef   _IO_H_
#define   _IO_H_


#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "data.h"

#define ERROR_FILE1 "Error! No s'ha pogut obrir el fitxer especificat.\n"
#define ERROR_FILE2 "Error! El fitxer especificat està buit o no compleix el format.\n"


int readFile(char* path, Data* data);
char* readUntil(int fd, char cFi);
void printConnection(char* name);
void printDisconnection(char* name);

#endif
