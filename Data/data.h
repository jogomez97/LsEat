#ifndef   _DATA_H_
#define   _DATA_H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char* ip;
    int portPicard;
    int portEnterprise;
} Data;

typedef struct {
    char* nom;
    int   port;
    char* ip;
} Enterprise;

typedef struct {
    int quants;
    Enterprise* enterprises;
} Fleet;

extern Fleet flota;

int gestionaFlota(char* data);
void printaFlota();
char* getFlota();

#endif
