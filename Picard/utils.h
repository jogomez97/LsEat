#ifndef   _UTILS_H_
#define   _UTILS_H_

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "communication.h"
#include "io.h"

extern char* comanda;
extern int   connectat;
extern int   sockfd;

void stringToUpper(char* string);
int gestionaShell();

#endif
