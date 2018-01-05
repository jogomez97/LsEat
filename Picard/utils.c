/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Picard
* Curs 2017-2018
*
* @File     utils.c
* @Purpose  Modul que conté funcions genèriques emprades al programa
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
********************************************************************************/

// Llibreries pròpies
#include "utils.h"

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
* @Name     isAllSpaces
* @Purpose  Funció que mira si tots els caracters d'una cadena són espais
* @Param    In:  string Cadena de caràcters a comprovar
*           Out: -
* @return   retorna -1 si són tot espais, 0 altrament
*
*******************************************************************************/
int isAllSpaces(char* string) {
    int i;
    int n = strlen(string);

    for (i = 0; i < n; i++) {
        if (string[i] != ' ') {
            return 0;
        }
    }

    return -1;

}

char* deleteExtraSpaces(char* string) {
    char* origin = string;
    char* aux;
    char* aux2;
    char* aux3 = NULL;

    aux = strtok(string, " ");
    if (aux != NULL) {
        asprintf(&aux2, "%s", aux);
    } else {
        return origin;
    }

    while (1) {
        aux = strtok(NULL, " ");
        if (aux != NULL) {
            aux3 = strdup(aux2);
            free(aux2);
            aux2 = NULL;
            asprintf(&aux2, "%s %s", aux3, aux);
            free(aux3);
            aux3 = NULL;
        } else {
            return aux2;
        }
    }


}
