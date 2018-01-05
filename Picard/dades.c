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
    char* buffer;

    asprintf(&buffer, "%s&%d", picard.nom, picard.saldo);
    return buffer;
}

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
char* getInfoComanda(char* plat, char* units) {
    char* buffer;

    asprintf(&buffer, "%s&%s", plat, units);
    return buffer;
}


/*******************************************************************************
*
* @Name     searchForDish
* @Purpose  Funció que consultarà si un plat ja s'havia demanat amb anterioritat
* @Param    In:     name    nom del plat amb el que volem comparar
*           Out:    -
* @return   retorna -1 si el plat no es troba encara reservat, 0 altrament
*
*******************************************************************************/
int searchForDish(char* name) {
    int i;
    int n = picard.nPlats;

    for (i = 0; i < n; i++) {
        if (strcmp(picard.plats[i].nom, name) == 0) {
            return i;
        }
    }

    return -1;

}

/*******************************************************************************
*
* @Name     addDish
* @Purpose  Funció que afegirà un plat
* @Param    In:     p       Plat que volem afegir a la reserva
*           Out:    -
* @return   -
*
*******************************************************************************/
void addDish(Plat p) {
    int where = searchForDish(p.nom);

    //Si el plat encara no havia estat demanat
    if (where == -1) {
        if (picard.nPlats == 0) {
            picard.plats = (Plat*) malloc(sizeof(Plat));
        } else {
            picard.plats = (Plat*) realloc(picard.plats,
                sizeof(Plat) * (picard.nPlats + 1));
        }

        picard.plats[picard.nPlats].nom = strdup(p.nom);
        picard.plats[picard.nPlats].quants = p.quants;

        picard.nPlats++;
    } else {
        //Si el plat no és nou
        picard.plats[where].quants += p.quants;
    }

}


/*******************************************************************************
*
* @Name     removeDish
* @Purpose  Funció que eliminara el plat
* @Param    In:     p       Plat que volem eliminar
*           Out:    -
* @return   retorna un int en control d'errors (-1), 0 altrament
*
*******************************************************************************/
int removeDish(Plat p) {
    if (p.quants < 0) {
        return -1;
    }

    int where = searchForDish(p.nom);

    //Si el plat encara no està reservat
    if (where == -1) {
        return -1;
    }

    if (p.quants <= picard.plats[where].quants) {
        picard.plats[where].quants -= p.quants;
        return 0;
    }

    return -1;

}

void eliminaReserva() {
    int i;
    int n = picard.nPlats;

    for (i = 0; i < n; i++) {
        picard.plats[i].quants = 0;
        free(picard.plats[i].nom);
    }

    if (n > 0) {
        free(picard.plats);
    }
    picard.nPlats = 0;

}

/*******************************************************************************
*
* @Name     printDishes
* @Purpose  Funció pinta tots els plats reservats (DEBUG)
* @Param    In:     -
*           Out:    -
* @return   -
*
*******************************************************************************/
void printDishes() {
    int i;
    int n = picard.nPlats;

    if (n == 0) {
        write(1, "No hi ha plats reservats\n", strlen("No hi ha plats reservats\n"));
    } else {
        char buff[100];
        for (i = 0; i < n; i++) {
            sprintf(buff, "%s (%d unitats)\n", picard.plats[i].nom, picard.plats[i].quants);
            write(1, buff, strlen(buff));
        }
    }

}
