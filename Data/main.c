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

Data d;
extern  List flota;

void alliberaMemoria() {

    //Allibera flota
    eraseList(&flota);

    //Allibera Data
    free(d.ip);

}

void intHandler() {
    alliberaMemoria();

    write(1, "\n", sizeof(char));
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

        //pthread_t tEnterprise;
        creaThread();
        connectPicard();

        return EXIT_SUCCESS;
    }
}
