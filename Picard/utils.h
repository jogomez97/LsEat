/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Picard
* Curs 2017-2018
*
* @File     utils.h
* @Purpose  Modul que conté funcions genèriques emprades al programa
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
********************************************************************************/

#ifndef   _UTILS_H_
#define   _UTILS_H_

// Llibreries del sistema
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Llibreries pròpies
#include "communication.h"
#include "io.h"

// Variables globals
extern char* comanda;
extern int   connectat;
extern int   sockfd;

/*******************************************************************************
*
* @Name     stringToUpper
* @Purpose  Funció que passa una String a majúscules
* @Param    In:  string Cadena de caràcters a passar a majúscules
*           Out: -
* @return   -
*
*******************************************************************************/
void stringToUpper(char* string);

/*******************************************************************************
*
* @Name     gestionaShell
* @Purpose  Funció gestionarà el Picard i actuarà en funció del que hagi introduit
*           l'usuari per teclat
* @Param    In:  -
*           Out: -
* @return   Enter que error o èxit
*
*******************************************************************************/
int gestionaShell();

#endif
