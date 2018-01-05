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

#define   _GNU_SOURCE 1

// Llibreries del sistema
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Definició de tipus propis
typedef struct {
    char*   nom;
    int     quants;
} Plat;

typedef struct {
    char*   nom;
    int     saldo;
    char*   ip;
    int     port;
    int     nPlats;
    Plat*   plats;
} Picard;

typedef struct {
    char* nom;
    int   port;
    char* ip;
} Enterprise;

// Variables globals
extern Picard picard;

/*******************************************************************************
*
* @Name     getPicardInfo
* @Purpose  Funció ens retornarà l'info de Picard en el format desitjat
* @Param    In: picard  Picard del que obtindrem la info
*           Out: -
* @return   Retorna la informació de picard en el format necessari pel protocol
*           de comunicació emprat.
*
*******************************************************************************/
char* getPicardInfo(Picard picard);

/*******************************************************************************
*
* @Name     getInfoComanda
* @Purpose  Funció ens retornarà l'info d'una comanda en el format desitjat
* @Param    In: plat    char* nom del plat que volem demanar
                units   char* nombre d'unitats que volem demanar
*           Out: -
* @return   Retorna la informació d'una comanda en el format necessari pel protocol
*           de comunicació emprat.
*
*******************************************************************************/
char* getInfoComanda(char* plat, char* units);

/*******************************************************************************
*
* @Name     searchForDish
* @Purpose  Funció que consultarà si un plat ja s'havia demanat amb anterioritat
* @Param    In:     name    nom del plat amb el que volem comparar
*           Out:    -
* @return   retorna -1 si el plat no es troba encara reservat, 0 altrament
*
*******************************************************************************/
int searchForDish(char* name);

/*******************************************************************************
*
* @Name     addDish
* @Purpose  Funció que afegirà un plat
* @Param    In:     p       Plat que volem afegir a la reserva
*           Out:    -
* @return   -
*
*******************************************************************************/
void addDish(Plat p);

/*******************************************************************************
*
* @Name     removeDish
* @Purpose  Funció que eliminara el plat
* @Param    In:     p       Plat que volem eliminar
*           Out:    -
* @return   retorna un int en control d'errors (-1), 0 altrament
*
*******************************************************************************/
int removeDish(Plat p);

/*******************************************************************************
*
* @Name     eliminaReserva
* @Purpose  Funció que eliminara tots els plats ja que s'ha pagat
* @Param    In:     -
*           Out:    -
* @return   -
*
*******************************************************************************/
void eliminaReserva();

/*******************************************************************************
*
* @Name     printDishes
* @Purpose  Funció pinta tots els plats reservats (DEBUG)
* @Param    In:     -
*           Out:    -
* @return   -
*
*******************************************************************************/
void printDishes();


#endif
