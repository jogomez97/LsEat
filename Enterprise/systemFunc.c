#include "systemFunc.h"

void alarmSignal() {
    connectionFlag = 1;
    signal(SIGALRM, alarmSignal);
    alarm(enterprise.seg);
}

void * threadFunc(void * arg) {

    signal(SIGALRM, alarmSignal);
    alarm(enterprise.seg);
    while (1) {
        if (connectionFlag) {
            connectionFlag = 0;
            gestionaConnexioData(!NEW_CONN);
        }
    }
    return arg;
}

void creaThread() {
    pthread_t id;
    pthread_create(&id, NULL, threadFunc, NULL);
}

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
    //També s'han de tancar totes les connexions de Picar existents

    write(1, "\n", sizeof(char));
    exit(0);
}
