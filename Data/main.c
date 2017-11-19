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
#include "data.h"
#include "communication.h"

#define DATA_EXEC         "Executant Data\n"
#define WAITING_CLIENTS   "Esperant clients...\n"

#define CONFIGFILE  "Data.dat"

Fleet flota;
Data d;

int main () {

    int error;

    flota.quants = 0;

    error = readFile(CONFIGFILE, &d);
    if (error) {
        //Com la funció readFile ja ha mostrat l'error específic, només cal
        //acabar l'execució
        return -1;
    } else {

        //pthread_t tEnterprise;
        creaThread();
        connectPicard();

        return 0;
    }
}
