/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Enterprise
* Curs 2017-2018
*
* @File     main.c
* @Purpose  Procediment principal de Enterprise
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
* Note: PORTS: 8180-8189 / 8260-8269
********************************************************************************/

// Llibreries del sistema
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

// Llibreries pròpies
#include "io.h"
#include "dades.h"
#include "communication.h"

// Declaració de constants
#define NPARAM          3
#define ERROR_ARG       "Error! No s'ha passat el nombre de paràmetres que pertoca.\n"
#define C_MENU          "Carregat Menu!\n"
#define WAIT_CLIENT     "Esperant clients...\n"

// Headers de funcions
void alliberaMemoria();
void intHandler();
void alarmSignal();

// Variables globals
Enterprise enterprise;
Menu menu;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char const *argv[]) {

    if (argc != NPARAM) {
        write(1, ERROR_ARG, strlen(ERROR_ARG));
        return EXIT_FAILURE;
    } else {
        signal(SIGINT, intHandler);
        int error;

        error = readConfig((char*)argv[1], &enterprise);
        enterprise.nConnections = 0;

        if (error) {
            raise(SIGINT);
        } else {

            printWelcome();
            error = readMenu((char*)argv[2], &menu);

            if (error) {
                raise(SIGINT);
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

/*******************************************************************************
*
* @Name     intHandler
* @Purpose  Funció a executar quan el programa rebi el signal SIGINT.
*           Enterprise haurà de tancar totes les connexions i haurà d'alliberar
*           la memòria emprada
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void intHandler() {
    alliberaMemoria();
    //També s'han de tancar totes les connexions de Picar existents (Fase més endavant)

    pthread_mutex_destroy(&mtx);

    write(1, "\n", sizeof(char));
    exit(0);
}

/*******************************************************************************
*
* @Name     alarmSignal
* @Purpose  Funció a executar quan el programa rebi el signal SIGALRM.
*           S'haurà de tornar a programar un alarm per a que torni a saltar el
*           signal i Enterprise es vagi connectant a Data de forma periòdica.
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void alarmSignal() {
    signal(SIGALRM, alarmSignal);
    alarm(enterprise.seg);
}
