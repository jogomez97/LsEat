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
    char* data;

    asprintf(&data, "%s&%d&%d", p.nom, p.preu, p.quants);
    return data;
}

/*******************************************************************************
*
* @Name     stringToUpper
* @Purpose  Funció que passa una String a majúscules
* @Param    In:  string Cadena de caràcters a passar a majúscules
*           Out: -
* @return   -
*
*******************************************************************************/
void stringToUpper(char* string) {
    int i;
    int fi = strlen(string);

    for (i = 0; i < fi; i++) {
        string[i] = toupper(string[i]);
    }
}

/*******************************************************************************
*
* @Name     reestableixMenu
* @Purpose  Funció que torna al menú totes les unitats que un Picard no ha comprat
* @Param    In:     fd      file descriptor propi del Picard
*           Out:    -
* @return   -
*
*******************************************************************************/
void reestableixMenu(int fd) {
    int i, j;

    Plats p = getDishInfo(&clients, fd);
    Plat* plats = p.plats;
    if (p.nPlats != 0) {
        for (i = 0; i < p.nPlats; i++) {
            for (j = 0; j < menu.nPlats; j++) {
                char* aux = strdup(menu.plats[j].nom);
                stringToUpper(aux);
                if (strcmp(plats[i].nom, aux) == 0) {
                    pthread_mutex_lock(&mtxMenu);
                    menu.plats[j].quants += plats[i].quants;
                    pthread_mutex_unlock(&mtxMenu);
                    free(aux);
                    aux = NULL;
                    break;
                }
                free(aux);
                aux = NULL;
            }
        }
    }

}
