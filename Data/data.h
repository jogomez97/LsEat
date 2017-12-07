#ifndef   _DATA_H_
#define   _DATA_H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

typedef struct {
    char* ip;
    int portPicard;
    int portEnterprise;
} Data;


int gestionaFlota(char* data);
char* getEnterprise();

#endif
