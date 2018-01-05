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

#define   _GNU_SOURCE 1

// Llibreries del sistema
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Llibreries pròpies
#include "io.h"

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
* @Name     isAllSpaces
* @Purpose  Funció que mira si tots els caracters d'una cadena són espais
* @Param    In:  string Cadena de caràcters a comprovar
*           Out: -
* @return   retorna -1 si són tot espais, 0 altrament
*
*******************************************************************************/
int isAllSpaces(char* string);

char* deleteExtraSpaces(char* string);

#endif
