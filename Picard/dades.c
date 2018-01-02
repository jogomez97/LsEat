/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Picard
* Curs 2017-2018
*
* @File     dades.c
* @Purpose  Modul que conté les funcions relacionades amb la gestió de la informació
*           del Picard
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
********************************************************************************/

// Llibreries pròpies
#include "dades.h"

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
char* getPicardInfo(Picard picard) {
    int length = strlen(picard.nom) + sizeof(picard.saldo) + sizeof(char);
    // S'haura de alliberar somewhere
    char* buffer = (char*)malloc(sizeof(char) * length);
    sprintf(buffer, "%s&%d", picard.nom, picard.saldo);
    return buffer;
}


char* getInfoComanda(char* plat, char* units) {
    int length = strlen(plat) + strlen(units) + sizeof(char);
    // S'haura de alliberar somewhere
    char* buffer = (char*)malloc(sizeof(char) * length);
    sprintf(buffer, "%s&%s", plat, units);
    return buffer;
}
