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
    int     preu;
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

#endif
