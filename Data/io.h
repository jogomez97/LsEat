/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Data
* Curs 2017-2018
*
* @File     io.h
* @Purpose  Modul que conté les funcions relacionades amb la gestió de fitxers i
*           interacció amb l'usuari.
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
********************************************************************************/

#ifndef   _IO_H_
#define   _IO_H_

// Llibreries del sistema
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// Llibreries pròpies
#include "data.h"

// Declaració de constants
#define ERROR_FILE1 "Error! No s'ha pogut obrir el fitxer especificat.\n"
#define ERROR_FILE2 "Error! El fitxer especificat està buit o no compleix el format.\n"

// Variables globals externes
extern Data d;

/******************************************************************************/
/**************************** FUNCIONS DE LECTURA *****************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     readFile
* @Purpose  Funció llegirà el fitxer de configuració i ens donarà les seves dades
* @Param    In: path    Path del fitxer a llegir
*           Out: d      Data on guardarem les dades del fitxer
* @return   Retorna un enter que indica els possibles errors a l'hora de llegir
*           el fitxer
*
*******************************************************************************/
int readFile(char* path, Data* d);

/*******************************************************************************
*
* @Name     readUntil
* @Purpose  Funció llegeix fins a un caràcter donat
* @Param    In: fd  File Descriptor del qual llegirem
*               cFi Caràcter fins el qual llegirem
*           Out: -
* @return   Retorna tots els caràcters llegits fins a arribar al cFi
*
*******************************************************************************/
char* readUntil(int fd, char cFi);

/******************************************************************************/
/************************** FUNCIONS D'ESCRIPTURA *****************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     printConnection
* @Purpose  Funció que printa una connexió per pantalla
* @Param    In: name  Nom de l'usuari que s'ha connectat a Data
*           Out: -
* @return   -
*
*******************************************************************************/
void printConnection(char* name);

/*******************************************************************************
*
* @Name     printDisconnection
* @Purpose  Funció que printa una desconnexió per pantalla
* @Param    In: name  Nom de l'usuari que s'ha desconnectat de Data
*           Out: -
* @return   -
*
*******************************************************************************/
void printDisconnection(char* name);

#endif
