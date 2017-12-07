#include "communication.h"

int connectPicard() {
    int sockfd;
    int clientfd;

    /* Obrir servidor */
    //Creació socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        write(1, ERROR_SOCK, strlen(ERROR_SOCK));
        return -1;
    }

    //Bind
    struct sockaddr_in s_addr;
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(d.portPicard);

    int error = inet_aton(d.ip, &s_addr.sin_addr);

    if (error < 0) {
        write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
        exit(0);
    }

    if (bind(sockfd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
        write(1, ERROR_BIND, strlen(ERROR_BIND));
        exit(0);
    }

    //Listen
    listen(sockfd, NCONN);

    //Accept
    socklen_t len = sizeof(s_addr);

    while (1) {
        write(1, WAIT_CONNECTP, strlen(WAIT_CONNECTP));
        clientfd = accept(sockfd, (struct sockaddr*) &s_addr, &len);
        if (clientfd < 0) {
            write(1, ERROR_ACCEPT, strlen(ERROR_ACCEPT));
        } else {
            gestionaPicard(clientfd);
        }
    }
}

void gestionaPicard(int clientfd) {
    Trama trama;
    int error = 0;


    write(1, CONNECTED_P, strlen(CONNECTED_P));

    memset(&trama, 0, sizeof(trama));
    trama = readTrama(clientfd, &error);


    if (error <= 0) {
        write(1, ERROR_DISCONNECTED, strlen(ERROR_DISCONNECTED));
        close(clientfd);
    }
    switch (trama.type) {
        case 0x01:
            //està a data.

            if (!isEmpty(&flota)) {
                char* data = getEnterprise();
                writeTrama(clientfd, 0x01, ENT_INF, getEnterprise());
                free(data);
                sortFirstNode(&flota);
            } else {
                writeTrama(clientfd, 0x01, CONKO, "");
            }
            write(1, DISCONNECTED_P, strlen(DISCONNECTED_P));
            break;
        default:
            printf("1\n");
            write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
            break;
    }
    close(clientfd);
}

/* FUNCIONS ENTERPRISE */

void * threadFunc(void * arg) {

    connectEnterprise();
    return arg;
}

void creaThread() {
    pthread_t id;
    pthread_create(&id, NULL, threadFunc, NULL);
}

int connectEnterprise() {
    int sockfd;
    int clientfd;

    /* Obrir servidor */
    //Creació socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        write(1, ERROR_SOCK, strlen(ERROR_SOCK));
        exit(0);
    }

    //Bind
    struct sockaddr_in s_addr;
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(d.portEnterprise);

    int error = inet_aton(d.ip, &s_addr.sin_addr);

    if (error < 0) {
        write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
        exit(0);
    };

    if (bind(sockfd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
        write(1, ERROR_BIND, strlen(ERROR_BIND));
        exit(0);
    }

    //Listen
    listen(sockfd, NCONN);

    //Accept
    socklen_t len = sizeof(s_addr);

    while (1) {
        write(1, WAIT_CONNECTE, strlen(WAIT_CONNECTE));
        clientfd = accept(sockfd, (struct sockaddr*) &s_addr, &len);
        if (clientfd < 0) {
            write(1, ERROR_ACCEPT, strlen(ERROR_ACCEPT));
        } else {
            gestionaEnterprise(clientfd);
        }
    }
    close(sockfd);
    return 0;
}

void gestionaEnterprise(int clientfd) {
    Trama trama;
    int error;
    //s'hauria de dir !end, porta a confusió
    int end = 1;

    write(1, CONNECTED_E, strlen(CONNECTED_E));

    while (end) {
        memset(&trama, 0, sizeof(trama));
        error = 0;
        trama = readTrama(clientfd, &error);

        if (error <= 0) {
            write(1, ERROR_DISCONNECTED, strlen(ERROR_DISCONNECTED));
            close(clientfd);
            break;
        }

        switch (trama.type) {
            case 0x01:
                //està a data.c
                error = gestionaFlota(trama.data);
                if (!error) {
                    writeTrama(clientfd, 0x01, CONOK, "");
                } else {
                    writeTrama(clientfd, 0x01, CONKO, "");
                }
                break;
            case 0x02:
                if (strcmp(trama.header, ENT_INF) == 0) {
                    write(1, DISCONNECTED_E, strlen(DISCONNECTED_E));
                    writeTrama(clientfd, 0x02, CONOKb, "");
                    close(clientfd);
                    end = 0;
                } else {
                    writeTrama(clientfd, 0x02, CONKOb, "");
                }
                break;
            case 0x07:
                if (strcmp(trama.header, UPDATE) == 0) {
                    //S'haurà de fer gestió de nConnectats per fer el balanceig
                    writeTrama(clientfd, 0x07, UPDATEOK, "");
                } else {
                    writeTrama(clientfd, 0x07, UPDATEKO, "");
                }
                write(1, DISCONNECTED_E, strlen(DISCONNECTED_E));
                writeTrama(clientfd, 0x07, UPDATEOK, "");
                close(clientfd);
                end = 0;
                break;
            default:
                write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                break;
        }
    }

}

/* FUNCIONS GENÈRIQUES */

Trama readTrama(int clientfd, int* error) {
    Trama trama;
    memset(&trama, 0, sizeof(trama));

    *error = read(clientfd, &trama.type, sizeof(trama.type));
    if (*error < 0) {
        return trama;
    }
    *error = read(clientfd, &trama.header, sizeof(trama.header));
    if (*error < 0) {
        return trama;
    }
    char aux[3];
    *error = read(clientfd, &aux, sizeof(trama.length));
    if (*error < 0) {
        return trama;
    }
    aux[2] = '\0';

    trama.length = (uint16_t)atoi(aux);

    trama.data = (char*) malloc(sizeof(char) * trama.length);
    *error = read(clientfd, trama.data, sizeof(char) * trama.length);
    if (*error < 0) {
        return trama;
    }

    return trama;
}

void writeTrama(int clientfd, char type, char header[10], char* data) {
    Trama trama;
    int length;
    int i;

    memset(&trama, 0, sizeof(trama));

    //Afegim les dades a trama
    trama.type = type;
    strcpy(trama.header, header);
    trama.data = data;
    trama.length = strlen(trama.data);

    //Enviem Trama
    length = sizeof(trama.type) + sizeof(trama.header)
            + sizeof(trama.length) + strlen(trama.data);

    char buffer2[length];
    sprintf(buffer2, "%c%-10s%-2u%s", trama.type, trama.header, trama.length, trama.data);
    //Plenem el que falta de header amb '\0'
    for (i = 1; i < 11; i++) {
        if (buffer2[i] == ' ') {
            buffer2[i] = '\0';
        }
    }
    write(clientfd, buffer2, length);
}
