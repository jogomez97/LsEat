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

        switch (trama.type) {
            case 0x01:
                if (strcmp(trama.header, PIC_INF) == 0) {
                    writeTrama(*picardfd, 0x01, CONOKb, "");

                    char* nom = strtok(trama.data, "&");
                    int length = strlen(nom) + strlen("Connectat\n");
                    char buff[length];
                    sprintf(buff, "Connectat %s\n", nom);
                    write(1, buff, strlen(buff));
                    free(nom);

                } else {
                    writeTrama(*picardfd, 0x01, CONKOb, "");
                }
                break;
            case 0x02:
                if (strcmp(trama.header, PIC_NAME) == 0) {
                    writeTrama(*picardfd, 0x02, CONOKb, "");

                    char* nom = strtok(trama.data, "\n");
                    int length = strlen(nom) + strlen("Desconnectat\n");
                    char buff[length];
                    sprintf(buff, "Desconnectat %s\n", nom);
                    write(1, buff, strlen(buff));
                    free(nom);

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
