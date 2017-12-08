/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Enterprise
* Curs 2017-2018
*
* @File     dades.h
* @Purpose  Inclou els structs que implementen les dades del programa.
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
********************************************************************************/

#ifndef   _DADES_H_
#define   _DADES_H_

// Definició de tipus propis
typedef struct {
    char*   nom;
    int     seg;
    char*   ipData;
    int     portData;
    char*   ipPicard;
    int     portPicard;
    int     nConnections;
} Enterprise;

typedef struct {
    char*   nom;
    int     quants;
    int     preu;
} Plat;

typedef struct {
    int     nPlats;
    Plat*   plats;
} Menu;


#endif
