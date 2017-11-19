#include "communication.h"

void gestionaNovaConnexio() {
    int sockfd = connectaData();
    if (sockfd < 0) {
        exit(0);
    } else {
        int error = enviaNovaConnexio(sockfd);
        if (error) {
            write(1, ERROR_1STDATA, strlen(ERROR_1STDATA));
            exit(0);
        }
    }
}

int connectaData() {
    //Creació socket
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        write(1, ERROR_SOCK, strlen(ERROR_SOCK));
        return -1;
    }

    //Connect
    struct sockaddr_in s_addr;
    memset(&s_addr, 0, sizeof (s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(enterprise.portData);

    int error = inet_aton(enterprise.ipData, &s_addr.sin_addr);
    if (error < 0) {
        write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
        return -1;
    }

    if (connect(sockfd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
        write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
        return -1;
    }

    return sockfd;
}

int enviaNovaConnexio(int sockfd) {
    int length;
    Trama trama;

    length = strlen(enterprise.nom) + strlen(enterprise.ipData)
            + sizeof(enterprise.portData) + 2 * sizeof(char);
    char buffer[length];
    sprintf(buffer, "%s&%d&%s", enterprise.nom, enterprise.portData, enterprise.ipData);

    writeTrama(sockfd, 0x01, ENT_INF, buffer);
    trama = readTrama(sockfd);
    close(sockfd);

    switch(trama.type) {
        case 0x01:
            if (strcmp(trama.header, CONOK) == 0) {
                return 0;
            } else if (strcmp(trama.header, CONKO) == 0) {
                return -1;
            } else {
                return -1;
            }
            break;
        default:
            write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
            return -1;
    }

    return 0;
}

Trama readTrama(int sockfd) {
    Trama trama;
    memset(&trama, 0, sizeof(trama));

    read(sockfd, &trama.type, sizeof(trama.type));
    read(sockfd, &trama.header, sizeof(trama.header));
    char aux[2];
    read(sockfd, &aux, sizeof(trama.length));

    trama.length = (uint16_t)atoi(aux);

    trama.data = (char*) malloc(sizeof(char) * trama.length);
    read(sockfd, trama.data, sizeof(char) * trama.length);

    return trama;
}

void writeTrama(int sockfd, char type, char header[10], char* data) {
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
    write(sockfd, buffer2, length);

}
