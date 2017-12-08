/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Data
* Curs 2017-2018
*
* @File     main.c
* @Purpose  Procediment principal de Data
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
* Note: PORTS: 8180-8189 / 8260-8269
********************************************************************************/

// Llibreries del sistema
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Llibreries pròpies
#include "io.h"
#include "list.h"
#include "data.h"
#include "communication.h"

// Declaració de constants
#define DATA_EXEC         "Executant Data\n"
#define WAITING_CLIENTS   "Esperant clients...\n"

#define CONFIGFILE  "Data.dat"

// Headers de funcions
void alliberaMemoria();
void intHandler();

// Variables global
Data d;
extern  List flota;
pthread_t threadEnterprise;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
int sockfd;
int clientfd;
int sockfdPicard;
int clientfdPicard;
Trama tramaPicard;
Trama trama;

int main () {

    int error;

    flota = createList();
    error = readFile(CONFIGFILE, &d);
    if (error) {
        //Com la funció readFile ja ha mostrat l'error específic, només cal
        //acabar l'execució
        return EXIT_FAILURE;
    } else {
        signal(SIGINT, intHandler);
        threadEnterprise = 0;

        //Server Enterprise
        creaThread();
        //Server Picards
        int eServer = connectPicard();

        //Només s'executarà si no s'ha pogut crear el servidor de Picards
        if (eServer < 0) {
            raise(SIGINT);
        }

        return EXIT_SUCCESS;
    }
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
    //Allibera flota
    eraseList(&flota);

    //Allibera Data
    free(d.ip);

    if (tramaPicard.data != NULL) {
        free(tramaPicard.data);
    }

    if (trama.data != NULL) {
        free(trama.data);
    }
}

/*******************************************************************************
*
* @Name     intHandler
* @Purpose  Funció a executar quan el programa rebi el signal SIGINT.
*           Data haurà de tancar totes les connexions i haurà d'alliberar la
*           memòria emprada
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void intHandler() {
    alliberaMemoria();

    //Tanquem tots els fd
    close(sockfd);
    close(clientfd);
    close(sockfdPicard);
    close(clientfdPicard);

    write(1, "\n", sizeof(char));

    pthread_mutex_destroy(&mtx);

    //Matem el thread de Enterprise
    /*
    if (threadEnterprise > 0) {
        pthread_cancel(threadEnterprise);
        pthread_join(threadEnterprise, NULL);
    }
    */

    exit(0);
}
