/*∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗
   @Autor: Juan Gómez Gómez - juan.gomez.2015
           Jordi Malé Carbonell - jordi.male.2015
           PORTS: 8180-8189 / 8260-8269
∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗*/

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

#include "io.h"
#include "list.h"
#include "data.h"
#include "communication.h"

#define DATA_EXEC         "Executant Data\n"
#define WAITING_CLIENTS   "Esperant clients...\n"

#define CONFIGFILE  "Data.dat"

/* Variables globals per poder alliberar memoria */
Data d;
extern  List flota;
pthread_t threadEnterprise;
int sockfd;
int clientfd;
int sockfdPicard;
int clientfdPicard;
Trama tramaPicard;
Trama trama;

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


void intHandler() {
    alliberaMemoria();

    //Tanquem tots els fd
    close(sockfd);
    close(clientfd);
    close(sockfdPicard);
    close(clientfdPicard);

    write(1, "\n", sizeof(char));


    //Matem el thread de Enterprise
    /*
    if (threadEnterprise > 0) {
        pthread_cancel(threadEnterprise);
        pthread_join(threadEnterprise, NULL);
    }
    */

    exit(0);
}


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
