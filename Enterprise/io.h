/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Enterprise
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
#include "dades.h"

// Declaració de constants
#define ERROR_FILE1 "Error! No s'ha pogut obrir el fitxer especificat.\n"
#define ERROR_FILE2 "Error! El fitxer especificat està buit o no compleix el format.\n"

// Variables globals externes
extern Enterprise enterprise;

/******************************************************************************/
/**************************** FUNCIONS DE LECTURA *****************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     readConfig
* @Purpose  Funció llegirà el fitxer de configuració i ens donarà les seves dades
* @Param    In: path        Path del fitxer a llegir
*           Out: enterprise Enterprise on guardarem les dades del fitxer
* @return   Retorna un enter que indica els possibles errors a l'hora de llegir
*           el fitxer
*
*******************************************************************************/
int readConfig(char* path, Enterprise* enterprise);

/*******************************************************************************
*
* @Name     readMenu
* @Purpose  Funció llegirà el fitxer del Menu i ens donarà les seves dades
* @Param    In: path    Path del fitxer a llegir
*           Out: menu   Menu on guardarem les dades del fitxer
* @return   Retorna un enter que indica els possibles errors a l'hora de llegir
*           el fitxer
*
*******************************************************************************/
int readMenu(char* path, Menu* menu);

/*******************************************************************************
*
* @Name     readUntil
* @Purpose  Funció llegeix fins a un caràcter donat
* @Param    In: fd  File Descriptor del qual llegirem
                cFi Caràcter fins el qual llegirem
*           Out: -
* @return   Retorna tots els caràcters llegits fins a arribar al cFi
*
*******************************************************************************/
char* readUntil(int fd, char cFi);

/*******************************************************************************
*
* @Name     readKB
* @Purpose  Funció llegeix el teclat
* @Param    In:  -
*           Out: -
* @return   Retorna els caràcters que l'usuari ha entrat pel teclat.
*
*******************************************************************************/
char* readKB();

/*******************************************************************************
*
* @Name     inputFlush
* @Purpose  Funció que neteja els caràcters no llegits del teclat
* @Param    In:  -
*           Out: -
* @return   Retorna el nombre de caràcters netejats.
*
*******************************************************************************/
int inputFlush();

/******************************************************************************/
/************************** FUNCIONS D'ESCRIPTURA *****************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     printWelcome
* @Purpose  Funció que mostrarà per pantalla el missatge de benvinguda
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void printWelcome();

#endif
