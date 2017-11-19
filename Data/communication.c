#include "communication.h"

int connectPicard(Data d) {
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
        return -1;
    };

    if (bind(sockfd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
        write(1, ERROR_BIND, strlen(ERROR_BIND));
        return -1;
    }

    //Listen
    listen(sockfd, NCONN);

    //Accept
    socklen_t len = sizeof(s_addr);

    while (1) {
        clientfd = accept(sockfd, (struct sockaddr*) &s_addr, &len);
        if (clientfd < 0) {
            write(1, ERROR_ACCEPT, strlen(ERROR_ACCEPT));
            return -1;
        } else {
            gestionaPicard(clientfd);
        }
        return 0;
    }
}

void gestionaPicard(int clientfd) {
    Trama trama;

    write(1, CONNECTEDP, strlen(CONNECTEDP));

    memset(&trama, 0, sizeof(trama));
    trama = readTrama(clientfd);

    switch (trama.type) {
        case 0x01:
            //està a data.
            if (flota.quants == 0) {
                writeTrama(clientfd, 0x01, ENT_INF, getFlota());
                free(getFlota());
            } else {
                writeTrama(clientfd, 0x01, CONOK, "");
            }
            break;
        default:
            write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
            break;
    }
    close(clientfd);
}

/* FUNCIONS ENTERPRISE */

int connectEnterprise(Data d) {
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
    s_addr.sin_port = htons(d.portEnterprise);

    int error = inet_aton(d.ip, &s_addr.sin_addr);

    if (error < 0) {
        write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
        return -1;
    };

    if (bind(sockfd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
        write(1, ERROR_BIND, strlen(ERROR_BIND));
        return -1;
    }

    //Listen
    listen(sockfd, NCONN);

    //Accept
    socklen_t len = sizeof(s_addr);

    while (1) {
        write(1, WAIT_CONNECT, strlen(WAIT_CONNECT));
        clientfd = accept(sockfd, (struct sockaddr*) &s_addr, &len);
        if (clientfd < 0) {
            write(1, ERROR_ACCEPT, strlen(ERROR_ACCEPT));
            return -1;
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

    write(1, CONNECTEDE, strlen(CONNECTEDE));

    memset(&trama, 0, sizeof(trama));
    trama = readTrama(clientfd);

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
        default:
            write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
            break;
    }
    close(clientfd);
}


Trama readTrama(int clientfd) {
    Trama trama;
    memset(&trama, 0, sizeof(trama));

    read(clientfd, &trama.type, sizeof(trama.type));
    read(clientfd, &trama.header, sizeof(trama.header));
    char aux[2];
    read(clientfd, &aux, sizeof(trama.length));

    trama.length = (uint16_t)atoi(aux);

    trama.data = (char*) malloc(sizeof(char) * trama.length);
    read(clientfd, trama.data, sizeof(char) * trama.length);

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
    sprintf(buffer2, "%c%-10s%u%s", trama.type, trama.header, trama.length, trama.data);
    //Plenem el que falta de header amb '\0'
    for (i = 1; i < 11; i++) {
        if (buffer2[i] == ' ') {
            buffer2[i] = '\0';
        }
    }
    write(clientfd, buffer2, length);
}
