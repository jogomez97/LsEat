/*∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗
   @Autor: Juan Gómez Gómez - juan.gomez.2015
           Jordi Malé Carbonell - jordi.male.2015
           PORTS: 8180-8189 / 8260-8269
∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗*/


/*  TODO:
+ - Trobar un lloc per posar la funció stringToUpper, ara està al main
* - Modul propi per a les funcions del shell? shell.c?
*
* Preguntes:
*   - La classe dades en pot juntar amb el main? (struct)
*   - Es connectaran sempre primer o poden fer una altra tasca?
*
*/

#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "io.h"
#include "dades.h"

#define ERROR_ARG       "Error! No s'ha passat el nombre de paràmetres que pertoca.\n"
#define INTRO_COMAND    "Introdueixi comandes...\n"
#define DIS_MSG         "Gràcies per fer servir LsEat. Fins la propera.\n"
#define ERROR_COMAND    "Comanda no reconeguda\n"
#define ERROR_CONN      "Error! Ja estàs connectat.\n"
#define ERROR_NCONN     "Error! No t'has connectat.\n"

//S'haurà de borrar per les següents fases
#define COMANDA_OK      "[Comanda OK]\n"

#define CONNECT         "CONNECTA"
#define SHOW            "MOSTRA"
#define MENU            "MENU"
#define ORDER           "DEMANA"
#define DELETE          "ELIMINA"
#define PAY             "PAGAR"
#define DISCONNECT      "DESCONNECTA"

void stringToUpper(char* string);
int connect();
void show();
void order();
void delete();
void pay();
void disconnect();
void alliberaMemoria();
void intHandler();

/* VARIABLES GLOBALS PER A PODER FER EL INTHANDLER */
Picard picard;
int connectat;
char* comanda;

int main(int argc, char const *argv[]) {
    int a;

    //Comprovem que només s'hagi passat un fitxer
    if (argc != 2) {
        write(1, ERROR_ARG, strlen(ERROR_ARG));
        return -1;
    } else {
        int error;

        error = readFile((char*)argv[1], &picard);
        if (error) {
            //Com la funció readFile ja ha mostrat l'error específic, només cal
            //acabar l'execució
            return -1;
        } else {
            signal(SIGINT, intHandler);
            //Printar informació de benvinguda
            printWelcome(picard.nom);
            printMoney(picard.saldo);
            write(1, INTRO_COMAND, strlen(INTRO_COMAND));

            //Variables pròpies de l'execució
            connectat = 0;
            char* split;
            char* num;
            int   quantitat;
            char* plat;
            while (1) {
                printShell(picard.nom);
                comanda = readKB();
                stringToUpper(comanda);

                split = strtok(comanda, " ");
                if (strcmp(CONNECT, split) == 0 && strtok(NULL, " ") == NULL) {
                    connectat = connect();
                } else if (strcmp(SHOW, split) == 0) {
                    split = strtok(NULL, " ");
                    if (split != NULL && strtok(NULL, " ") == NULL) {
                        if (strcmp(split, MENU) == 0) {
                            show();
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
                            order();
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
                            delete();
                        } else {
                            write(1, ERROR_COMAND, strlen(ERROR_COMAND));
                        }
                    } else {
                        write(1, ERROR_COMAND, strlen(ERROR_COMAND));
                    }
                } else if (strcmp(PAY, split) == 0 && strtok(NULL, " ") == NULL) {
                    pay(connectat);
                } else if (strcmp(DISCONNECT, split) == 0 && strtok(NULL, " ") == NULL) {
                    alliberaMemoria();
                    disconnect();
                    return 0;
                } else {
                    write(1, ERROR_COMAND, strlen(ERROR_COMAND));
                }
            }
            free(comanda);
            comanda = NULL;
        }

    }
    return 0;
}

void stringToUpper(char* string) {
    int i;
    int fi = strlen(string);

    for (i = 0; i < fi; i++) {
        string[i] = toupper(string[i]);
    }

}

void alliberaMemoria() {
    free(picard.nom);
    picard.nom = NULL;
    free(picard.ip);
    picard.ip = NULL;
    free(comanda);
    comanda = NULL;
}

void intHandler() {
    alliberaMemoria();
    if (connectat) {
        //Aquí es desconnectarà de enterprise amb el protocol adient
    }
    write(1, "\n", sizeof(char));
    exit(0);
}

/* FUNCIONS DE LES OPCIONS */

int connect() {

    if (!connectat) {
        //Aquí s'intentarà connectar amb Data, de moment suposarem que
        //es fa bé (return 1)
        write(1, COMANDA_OK, strlen(COMANDA_OK));
        return 1;
    } else {
        write(1, ERROR_CONN, strlen(ERROR_CONN));
    }

    return 1;
}

void show() {

    if (connectat) {
        //Fem tot el pertinent per mostrar el menú
        write(1, COMANDA_OK, strlen(COMANDA_OK));
    } else {
        write(1, ERROR_NCONN, strlen(ERROR_NCONN));
    }

}

void order() {

    if (connectat) {
        //Fem tot el pertinent per fer una comanda
        write(1, COMANDA_OK, strlen(COMANDA_OK));
    } else {
        write(1, ERROR_NCONN, strlen(ERROR_NCONN));
    }
}

void delete() {

    if (connectat) {
        //Fem tot el pertinent esborrar una comanda
        write(1, COMANDA_OK, strlen(COMANDA_OK));
    } else {
        write(1, ERROR_NCONN, strlen(ERROR_NCONN));
    }

}

void pay() {

    if (connectat) {
        //Fem tot el pertinent per a pagar
        write(1, COMANDA_OK, strlen(COMANDA_OK));
    } else {
        write(1, ERROR_NCONN, strlen(ERROR_NCONN));
    }
}

void disconnect() {
    if (connectat) {
        //Ens desconnectem
    }
    write(1, DIS_MSG, strlen(DIS_MSG));
}
