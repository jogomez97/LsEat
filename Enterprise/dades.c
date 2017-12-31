/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Enterprise
* Curs 2017-2018
*
* @File     dades.c
* @Purpose  Inclou els structs que implementen les dades del programa.
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
********************************************************************************/

// Llibreries pròpies
#include "dades.h"

/*******************************************************************************
*
* @Name     createPicard
* @Purpose  Funció que inicialitza un Picard només amb el seu file descriptor
* @Param    In:     int fd associat al Picard
*           Out: -
* @return   Retorna un Picard inicialitzat
*
*******************************************************************************/
Picard createPicard(int fd) {
    Picard p;
    p.nom = NULL;
    p.fd = fd;
    p.compte = 0;
    p.nPlats = 0;
    p.plats = NULL;

    return p;
}

/*******************************************************************************
*
* @Name     getDishInFormat
* @Purpose  Funció que retorna un plat en el format de dades que necessita la trama
* @Param    In:     i  posició del menú on està el plat que volem enviar
*           Out: -
* @return   Retorna la cadena que enviarem com a data
*
*******************************************************************************/
char* getDishInFormat(int i) {
    Plat p = menu.plats[i];
    char* data = (char*) malloc(strlen(p.nom) + sizeof(int) * 2 + 4);

    sprintf(data, "%s&%d&%d", p.nom, p.preu, p.quants);
    return data;
}
