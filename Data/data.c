/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Data
* Curs 2017-2018
*
* @File     data.c
* @Purpose  Inclou les funcions i structs que gestionen i implementen les dades
*            del programa.
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
********************************************************************************/

// Llibreries pròpies
#include "data.h"

// Variables globals
List flota;

int gestionaFlota(char* data) {
    char* nom = strtok(data, "&");
    char* aux = strtok(NULL, "&");
    char* ip = strtok(NULL, "");
    if ((nom != NULL) & (aux != NULL) & (ip != NULL)) {
        Enterprise e;
        e.nom = strdup(nom);
        e.ip = strdup(ip);
        e.port = atoi(aux);
        e.nConnections = 0;

        insertNode(&flota, e);
        //printList(&flota);
        return 0;
    }
    return -1;
}


/*******************************************************************************
*
* @Name     getEnterprise
* @Purpose  Funció que retornarà una Enterprise per a que Picard s'hi connecti
* @Param    In:  -
*           Out: -
* @return   Retorna el codi de l'Enterprise on s'haurà de connectar Picard
*
*******************************************************************************/
char* getEnterprise() {
    Enterprise e = checkFirstElement(&flota);

    int length = strlen(e.nom) + strlen(e.ip)
            + sizeof(e.port) + 2 * sizeof(char);

    char* buffer = (char*)malloc(sizeof(char) * length);
    sprintf(buffer, "%s&%d&%s", e.nom, e.port, e.ip);
    return buffer;
}
