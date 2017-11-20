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

void * threadPicard(void * arg) {

    //A partir d'aquí ha de ser un thread per cada picard
    Trama trama;
    int error;
    int end = 0;
    int* picardfd = (int*) arg;

    while (!end) {
        trama = readTrama(*picardfd, &error);
        if (error <= 0) {
            write(1, ERROR_DISCONNECTEDP, strlen(ERROR_DISCONNECTEDP));
            break;
        }

        char    a[500];
        sprintf(a, "%X/%s/%u/%s\n", trama.type, trama.header, trama.length, trama.data);
        write(1, a, strlen(a));

        switch (trama.type) {
            case 0x01:
                if (strcmp(trama.header, PIC_INF) == 0) {
                    writeTrama(*picardfd, 0x01, CONOKb, "");
                } else {
                    writeTrama(*picardfd, 0x01, CONKOb, "");
                }
                break;
            case 0x02:
                if (strcmp(trama.header, PIC_NAME) == 0) {
                    writeTrama(*picardfd, 0x02, CONOKb, "");
                } else {
                    writeTrama(*picardfd, 0x02, CONKOb, "");
                }
                end = 1;
                close(*picardfd);
                break;
            default:
                write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                break;
        }
    }
    return arg;
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
