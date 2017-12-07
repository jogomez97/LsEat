/*∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗
   @Autor: Juan Gómez Gómez - juan.gomez.2015
           Jordi Malé Carbonell - jordi.male.2015
           PORTS: 8180-8189 / 8260-8269
∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "io.h"
#include "dades.h"
#include "communication.h"

#define NPARAM          3
#define ERROR_ARG       "Error! No s'ha passat el nombre de paràmetres que pertoca.\n"
#define C_MENU          "Carregat Menu!\n"
#define WAIT_CLIENT     "Esperant clients...\n"
#define ERROR_DATA      "Error en connectar amb Data\n"

Enterprise enterprise;
Menu menu;
int connectionFlag;

void alliberaMemoria() {
    int i;

    //Allibera plats
    for (i = 0; i < menu.nPlats; i++) {
        free(menu.plats[i].nom);
    }
    free(menu.plats);

    //Allibera enterprise
    free(enterprise.nom);
    free(enterprise.ipData);
    free(enterprise.ipPicard);

}

void intHandler() {
    alliberaMemoria();
    //També s'han de tancar totes les connexions de Picar existents (Fase més endavant)

    write(1, "\n", sizeof(char));
    exit(0);
}

void alarmSignal() {
    connectionFlag = 1;
    signal(SIGALRM, alarmSignal);
    alarm(enterprise.seg);
}

int main(int argc, char const *argv[]) {

    if (argc != NPARAM) {
        write(1, ERROR_ARG, strlen(ERROR_ARG));
        return EXIT_FAILURE;
    } else {
        int error;

        error = readConfig((char*)argv[1], &enterprise);
        enterprise.nConnections = 0;


        if (error) {
            return EXIT_FAILURE;
        } else {
            connectionFlag = 0;
            signal(SIGINT, intHandler);

            printWelcome();
            error = readMenu((char*)argv[2], &menu);

            if (error) {
                return EXIT_FAILURE;
            } else {

                write(1, C_MENU, strlen(C_MENU));
                gestionaConnexioData(NEW_CONN);

                //Engeguem el thread del servidor de Picards
                pthread_t id;
                pthread_create(&id, NULL, engegaServidor, NULL);

                //Reprogramem l'alarma per anar-nos conenctant amb Data periodicament
                signal(SIGALRM, alarmSignal);
                alarm(enterprise.seg);
                while(1) {
                    pause();
                    gestionaConnexioData(!NEW_CONN);
                }
            }

        }

    }
    return EXIT_SUCCESS;
}
