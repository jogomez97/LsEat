#ifndef   _DADES_H_
#define   _DADES_H_

#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    char*   nom;
    int     saldo;
    char*   ip;
    int     port;
} Picard;

typedef struct {
    char* nom;
    int   port;
    char* ip;
} Enterprise;

char* getPicardInfo(Picard picard);

#endif
