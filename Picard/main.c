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

#define DSC_ENTERP  2

void alliberaMemoria();
void intHandler();

/* VARIABLES GLOBALS PER A PODER FER EL INTHANDLER */
Picard picard;
int connectat;
char* bufferKB;
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

            connectat = 0;
            int end = 0;

            while (!end) {
                end = gestionaShell();
                free(comanda);
            }
            alliberaMemoria();
            return EXIT_SUCCESS;
        }

    }
    return EXIT_SUCCESS;
}

void alliberaMemoria() {
    free(picard.nom);
    free(picard.ip);
    if (bufferKB == comanda) {
        free(comanda);
    } else {
        free(comanda);
        free(bufferKB);
    }
}

void intHandler() {
    write(1, "\n", strlen("\n"));
    write(1, DIS_MSG, strlen(DIS_MSG));
    if (connectat) {
        write(1, "HOLA1\n", 6);
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
