/*∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗
   @Autor: Juan Gómez Gómez - juan.gomez.2015
           Jordi Malé Carbonell - jordi.male.2015
           PORTS: 8180-8189 / 8260-8269
∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗*/

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "io.h"
#include "dades.h"
#include "communication.h"
#include "utils.h"

void alliberaMemoria();
void intHandler();

/* VARIABLES GLOBALS PER A PODER FER EL INTHANDLER */
Picard picard;
int connectat;
char* comanda;
int sockfd;

int main(int argc, char const *argv[]) {

    //Comprovem que només s'hagi passat un fitxer
    if (argc != 2) {
        write(1, ERROR_ARG, strlen(ERROR_ARG));
        return EXIT_FAILURE;
    } else {
        int error;

        error = readFile((char*)argv[1], &picard);
        if (error) {
            //Com la funció readFile ja ha mostrat l'error específic, només cal
            //acabar l'execució
            return EXIT_FAILURE;
        } else {
            signal(SIGINT, intHandler);
            //Printar informació de benvinguda
            printWelcome();
            printMoney();
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
                    alliberaMemoria();
                    exit(0);
                    return 0;
                } else {
                    write(1, ERROR_COMAND, strlen(ERROR_COMAND));
                }
            }
            free(comanda);
            comanda = NULL;
        }

    }
    return EXIT_SUCCESS;
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
    if (connectat) {

        writeTrama(sockfd, 0x02, PIC_NAME, picard.nom);

        int error = 0;
        Trama t  = readTrama(sockfd, &error);

        if (error <= 0) {
            write(1, ERROR_DATA, strlen(ERROR_DATA));
            close(sockfd);
        }
        if (gestionaTrama(t, DSC_ENTERP)) {
            write(1, DISCONNECTED_E, strlen(DISCONNECTED_E));
            close(sockfd);
        } else {
            write(1, ERROR_DISCON_E, strlen(ERROR_DISCON_E));
            close(sockfd);
        }
    }

    alliberaMemoria();
    exit(0);
}
