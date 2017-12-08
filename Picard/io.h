/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Picard
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
#define ERROR_ARG       "Error! No s'ha passat el nombre de paràmetres que pertoca.\n"
#define INTRO_COMAND    "Introdueixi comandes...\n"
#define DIS_MSG         "Gràcies per fer servir LsEat. Fins la propera.\n"
#define ERROR_COMAND    "Comanda no reconeguda\n"
#define ERROR_CONN      "Error! Ja estàs connectat.\n"
#define ERROR_NCONN     "Error! No t'has connectat.\n"
#define ERROR_CONNECT   "Error de connexió amb el servidor.\n"
#define ERROR_SOCK      "Error en crear el socket.\n"
#define CONNECTED_D     "[PICARD] Connexió establerta amb Data\n"
#define DISCONNECTED_D  "[PICARD] Desconnexió amb data\n"
#define CONNECTED_E     "[PICARD] Connexió establerta amb enterprise\n"
#define DISCONNECTED_E  "[PICARD] Desconnexió amb enterprise\n"
#define ERROR_DISCON_E  "[PICARD] Error en la desconnexió amb enterprise\n"
#define ERROR_DATA      "[PICARD] Error amb la connexió amb Data\n"
#define ERROR_ESTABLISHED "[PICARD] Connexió amb Enterprise ja realitzada!\n"

#define CONNECT         "CONNECTA"
#define SHOW            "MOSTRA"
#define MENU            "MENU"
#define ORDER           "DEMANA"
#define DELETE          "ELIMINA"
#define PAY             "PAGAR"
#define DISCONNECT      "DESCONNECTA"

#define COMANDA_OK      "[Comanda OK]\n"

#define ERROR_FILE1   "Error! No s'ha pogut obrir el fitxer especificat.\n"
#define ERROR_FILE2   "Error! El fitxer especificat està buit o no compleix el format.\n"

// Variables globals
extern Picard picard;

/******************************************************************************/
/**************************** FUNCIONS DE LECTURA *****************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     readFile
* @Purpose  Funció llegirà el fitxer de configuració i ens donarà les seves dades
* @Param    In: path    Path del fitxer a llegir
*           Out: picard Picard on guardarem les dades del fitxer
* @return   Retorna un enter que indica els possibles errors a l'hora de llegir
*           el fitxer
*
*******************************************************************************/
int readFile(char* path, Picard* picard);

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

/*******************************************************************************
*
* @Name     printMoney
* @Purpose  Funció que mostrarà per pantalla els diners disponibles del Picard
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void printMoney();

/*******************************************************************************
*
* @Name     printShell
* @Purpose  Funció que mostrarà per pantalla la Shell de Picard
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void printShell();

#endif
