/*
*
* Practica Sistemes Operatius - LsEat - Package Picard
* Curs 2017-2018
*
* @file dades.c
* @purpose  Modul que conté les funcions relacionades amb la gestió de la informació
            del Picard
* @author Jordi Malé Carbonell  (jordi.male.2015)
* @author Juan Gómez Gómez  (juan.gomez.2015)
*
*/

#include "dades.h"

char* getPicardInfo(Picard picard) {
    int length = strlen(picard.nom) + sizeof(picard.saldo) + sizeof(char);
    // S'haura de alliberar somewhere
    char* buffer = (char*)malloc(sizeof(char) * length);
    sprintf(buffer, "%s&%d", picard.nom, picard.saldo);
    return buffer;
}
