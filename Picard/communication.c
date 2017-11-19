#include "communication.h"

int connectaData(int connectat, Picard picard) {
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
        return 1;
    } else {
        write(1, ERROR_CONN, strlen(ERROR_CONN));
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
