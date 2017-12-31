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

// Llibreries pròpies
#include "list.h"

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
    int     nPlats;
    Plat*   plats;
} Menu;

// Variables globals externes
extern Menu menu;

/*******************************************************************************
*
* @Name     createPicard
* @Purpose  Funció que inicialitza un Picard només amb el seu file descriptor
* @Param    In:     int fd associat al Picard
*           Out: -
* @return   Retorna un Picard inicialitzat
*
*******************************************************************************/
Picard createPicard(int fd);

/*******************************************************************************
*
* @Name     getDishInFormat
* @Purpose  Funció que retorna un plat en el format de dades que necessita la trama
* @Param    In:     i  posició del menú on està el plat que volem enviar
*           Out: -
* @return   Retorna la cadena que enviarem com a data
*
*******************************************************************************/
char* getDishInFormat(int i);

#endif
