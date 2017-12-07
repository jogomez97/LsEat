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

#include "utils.h"

void stringToUpper(char* string) {
    int i;
    int fi = strlen(string);

    for (i = 0; i < fi; i++) {
        string[i] = toupper(string[i]);
    }

}

int gestionaShell() {
    char* split;
    char* num;
    int   quantitat;
    char* plat;

    printShell(picard.nom);
    comanda = readKB();
    stringToUpper(comanda);

    split = strtok(comanda, " ");
    if (strcmp(CONNECT, split) == 0 && strtok(NULL, " ") == NULL) {
        if (!connectat) {
            int a = connectaServidor(connectat, picard, DATA, NULL);
            if (a >= 1) {
                sockfd = a;
                connectat = 1;
            }
        } else {
            write(1, ERROR_ESTABLISHED, strlen(ERROR_ESTABLISHED));
        }
    } else if (strcmp(SHOW, split) == 0) {
        split = strtok(NULL, " ");
        if (split != NULL && strtok(NULL, " ") == NULL) {
            if (strcmp(split, MENU) == 0) {
                show(connectat);
            } else {
                write(1, ERROR_COMAND, strlen(ERROR_COMAND));
            }
        } else {
            write(1, ERROR_COMAND, strlen(ERROR_COMAND));
        }
    } else if (strcmp(ORDER, split) == 0) {
        num = strtok(NULL, " ");
        plat = strtok(NULL, "\n");
        if (num != NULL && plat != NULL) {
            quantitat = atoi(num);
            if (quantitat > 0) {
                order(connectat);
            } else {
                write(1, ERROR_COMAND, strlen(ERROR_COMAND));
            }
        } else {
            write(1, ERROR_COMAND, strlen(ERROR_COMAND));
        }
    } else if (strcmp(DELETE, split) == 0) {
        num = strtok(NULL, " ");
        plat = strtok(NULL, "\n");
        if (num != NULL && plat != NULL) {
            quantitat = atoi(num);
            if (quantitat > 0) {
                delete(connectat);
            } else {
                write(1, ERROR_COMAND, strlen(ERROR_COMAND));
            }
        } else {
            write(1, ERROR_COMAND, strlen(ERROR_COMAND));
        }
    } else if (strcmp(PAY, split) == 0 && strtok(NULL, " ") == NULL) {
        pay(connectat);
    } else if (strcmp(DISCONNECT, split) == 0 && strtok(NULL, " ") == NULL) {
        disconnect(connectat, sockfd);
        return 1;
    } else {
        write(1, ERROR_COMAND, strlen(ERROR_COMAND));
    }
    return 0;
}
