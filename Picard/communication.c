#include "communication.h"

int connectaServidor(int connectat, Picard picard, int mode, Enterprise* e) {
    if (mode == DATA) {
        if (!connectat) {
            //Aquí s'intentarà connectar amb Data, de moment suposarem que
            //es fa bé (return 1)
            //Connect
            int sockfd;

            sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            if (sockfd < 0) {
                write(1, ERROR_SOCK, strlen(ERROR_SOCK));
                return -1;
            }

            struct sockaddr_in s_addr;
            memset(&s_addr, 0, sizeof (s_addr));
            s_addr.sin_family = AF_INET;
            s_addr.sin_port = htons(picard.port);

            int error = inet_aton(picard.ip, &s_addr.sin_addr);

            if (error < 0) {
                write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
                return -1;
            };

            if (connect(sockfd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
                write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
                return -1;
            }
            write(1, COMANDA_OK, strlen(COMANDA_OK));


            writeTrama(sockfd, 0x01, PIC_NAME, picard.nom);

            error = 0;

            Trama t = readTrama(sockfd, &error);

            if (error <= 0) {
                write(1, ERROR_DATA, strlen(ERROR_DATA));
                close(sockfd);
                return -1;
            }

            error = gestionaTrama(t, DATA);

            if (error == 1) {
                close(sockfd);
                return -1;
            }

            return 1;
        } else {
            write(1, ERROR_CONN, strlen(ERROR_CONN));
        }
    } else {
        int sockfd;

        sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (sockfd < 0) {
            write(1, ERROR_SOCK, strlen(ERROR_SOCK));
            return -1;
        }

        struct sockaddr_in s_addr;
        memset(&s_addr, 0, sizeof (s_addr));
        s_addr.sin_family = AF_INET;
        s_addr.sin_port = htons(e->port);

        int error = inet_aton(e->ip, &s_addr.sin_addr);

        if (error < 0) {
            write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
            return -1;
        };

        if (connect(sockfd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
            write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
            return -1;
        }
        write(1, COMANDA_OK, strlen(COMANDA_OK));


        writeTrama(sockfd, 0x01, PIC_INF, getPicardInfo(picard));

        return 1;
    }
    return 1;
}

/* FUNCIONS DE LES OPCIONS */

void show(int connectat) {

    if (connectat) {
        //Fem tot el pertinent per mostrar el menú
        write(1, COMANDA_OK, strlen(COMANDA_OK));
    } else {
        write(1, ERROR_NCONN, strlen(ERROR_NCONN));
    }

}

void order(int connectat) {

    if (connectat) {
        //Fem tot el pertinent per fer una comanda
        write(1, COMANDA_OK, strlen(COMANDA_OK));
    } else {
        write(1, ERROR_NCONN, strlen(ERROR_NCONN));
    }
}

void delete(int connectat) {

    if (connectat) {
        //Fem tot el pertinent esborrar una comanda
        write(1, COMANDA_OK, strlen(COMANDA_OK));
    } else {
        write(1, ERROR_NCONN, strlen(ERROR_NCONN));
    }

}

void pay(int connectat) {

    if (connectat) {
        //Fem tot el pertinent per a pagar
        write(1, COMANDA_OK, strlen(COMANDA_OK));
    } else {
        write(1, ERROR_NCONN, strlen(ERROR_NCONN));
    }
}

void disconnect(int connectat) {
    if (connectat) {
        //Ens desconnectem
    }
    write(1, DIS_MSG, strlen(DIS_MSG));
}

int gestionaTrama(Trama t, int mode) {
    if (mode == DATA) {
        if (strcmp(t.header, ENT_INF) == 0) {
            write(1, DISCONNECTED_D, strlen(DISCONNECTED_D));

            Enterprise e;

            printf("DATAAAAA: %s\n", t.data);


            connectaServidor(0, picard, ENTERPRISE, &e);
            return 1;
        } else {
            write(1, ERROR_DATA, strlen(ERROR_DATA));
            return -1;
        }
    }
    return 1;
}


/* FUNCIONS GENÈRIQUES */

Trama readTrama(int clientfd, int* error) {
    Trama trama;
    memset(&trama, 0, sizeof(trama));

    *error = read(clientfd, &trama.type, sizeof(trama.type));
    read(clientfd, &trama.header, sizeof(trama.header));
    char aux[3];
    read(clientfd, &aux, sizeof(trama.length));
    aux[2] = '\0';

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
    sprintf(buffer2, "%c%-10s%-2u%s", trama.type, trama.header, trama.length, trama.data);
    //Plenem el que falta de header amb '\0'
    for (i = 1; i < 11; i++) {
        if (buffer2[i] == ' ') {
            buffer2[i] = '\0';
        }
    }
    write(clientfd, buffer2, length);
}
