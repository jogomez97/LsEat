/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Picard
* Curs 2017-2018
*
* @File     main.c
* @Purpose  Procediment principal de Picard
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
* Note: PORTS: 8180-8189 / 8260-8269
********************************************************************************/

// Llibreries del sistema
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Llibreries pròpies
#include "io.h"
#include "dades.h"
#include "communication.h"
#include "utils.h"


// Headers de funcions
void alliberaMemoria();
void intHandler();

// Variables globals
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
            signal(SIGTERM, intHandler);
            //Printar informació de benvinguda
            printWelcome();
            printMoney();
            write(1, INTRO_COMAND, strlen(INTRO_COMAND));

            picard.nPlats = 0;
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

/******************************************************************************/
/***************************** FUNCIONS AUXILIARS *****************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     alliberaMemoria
* @Purpose  Funció allibera la memòria emprada pel programa
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void alliberaMemoria() {
    free(picard.nom);
    free(picard.ip);
    if (bufferKB == comanda) {
        free(comanda);
    } else {
        free(comanda);
        free(bufferKB);
    }

    int i;
    int n = picard.nPlats;
    if (n > 0) {
        for (i = 0; i < n; i++) {
            free(picard.plats[i].nom);
        }
        free(picard.plats);
        picard.nPlats = 0;
    }

}

/*******************************************************************************
*
* @Name     intHandler
* @Purpose  Funció a executar quan el programa rebi el signal SIGINT.
*           Picard haurà de desconnectar-se de Enterprise (en cas d'estar connectat)
*           i haurà d'alliberar la memòria emprada
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void intHandler() {
    write(1, "\n", strlen("\n"));
    write(1, DIS_MSG, strlen(DIS_MSG));
    if (connectat) {
        writeTrama(sockfd, 0x02, PIC_NAME, picard.nom);

        int error = 0;

        Trama t  = readTrama(sockfd, &error);

        if (error <= 0) {
            write(1, ERROR_DISCON_E, strlen(ERROR_DISCON_E));
            close(sockfd);
        } else if (gestionaTrama(t, DSC_ENTERP)) {
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
