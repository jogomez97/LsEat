/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Picard
* Curs 2017-2018
*
* @File     dades.h
* @Purpose  Modul que conté les funcions relacionades amb la gestió de la informació
*           del Picard.
*           Inclou els structs que implementen les dades del programa.
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
********************************************************************************/

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
