/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Picard
* Curs 2017-2018
*
* @File     communication.c
* @Purpose  Modul que conté les funcions relacionades amb les diferents connexions
*           de Picard a Data i Enterprise
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
*******************************************************************************/

// Llibreries pròpies
#include "communication.h"

/*******************************************************************************
*
* @Name     connectaServidor
* @Purpose  Funció que connectarà Picard a Enterprise o Data
* @Param    In: connectat   Indica si el Picard està o no connectat
*               picard      Picard a connectar
*               mode        Indica si hem de connectar a Data o Enterprise
*               e           Enterprise a la que ens hem de connectar
*           Out: -
* @return   Retorna diferents valors, indicant error o èxit en la connexió.
*           En cas d'èxit, l'enter retornat és el fd a emprar del Picard.
*
*******************************************************************************/
int connectaServidor(int connectat, Picard picard, int mode, Enterprise* e) {
    Trama t;
    if (mode == DATA) {
        //Connexió amb Data amb el respectiu control d'errors

        int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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

        t = readTrama(sockfd, &error);

        if (error <= 0) {
            write(1, ERROR_DATA, strlen(ERROR_DATA));
            close(sockfd);
            return -1;
        }

        // Gestió de l'intent de connexió amb DATA

        error = gestionaTrama(t, DATA);

        if (error < 1) {
            close(sockfd);
            return -1;
        }

        return error;
    } else {
        if (!connectat) {
            // Connexió amb Enterprise
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

            t = readTrama(sockfd, &error);

            if (error <= 0) {
                write(1, ERROR_DATA, strlen(ERROR_DATA));
                close(sockfd);
                return -1;
            }

            if (gestionaTrama(t, ENTERPRISE)) {
                // Connexió correcta, retornem el fd associat al socket
                return sockfd;
            }

            return -1;
        } else {
            write(1, ERROR_CONN, strlen(ERROR_CONN));
        }
    }
    return 1;
}


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

/*******************************************************************************
*
* @Name     disconnect
* @Purpose  Funció que desconnectarà Picard d'Enterprise (amb el protocol corresponent)
* @Param    In: connectat   Indica si el Picard està o no connectat
*               sockfd      File descriptor del que ens hem de desconnectar
*           Out: -
* @return   -
*
*******************************************************************************/
void disconnect(int connectat, int sockfd) {
    write(1, DIS_MSG, strlen(DIS_MSG));
    if (connectat) {
        writeTrama(sockfd, 0x02, PIC_NAME, picard.nom);

        int error = 0;
        Trama t  = readTrama(sockfd, &error);

        if (error <= 0) {
            write(1, ERROR_DATA, strlen(ERROR_DATA));
            close(sockfd);
        }
        if (gestionaTrama(t, DSC_ENTERP)) {
            write(1, DISCONNECTED_E, strlen(DISCONNECTED_E));
            close(sockfd);
        } else {
            write(1, ERROR_DISCON_E, strlen(ERROR_DISCON_E));
            close(sockfd);
        }
    }
}

/*******************************************************************************
*
* @Name     gestionaTrama
* @Purpose  Funció interpretarà les trames i actuarà en funció del que calgui realitzar
* @Param    In: t       Trama a gestionar
*               mode    Mode de gestió de la trama
*           Out: -
* @return   Retorna un enter que indica error o èxit
*
*******************************************************************************/
int gestionaTrama(Trama t, int mode) {
    if (mode == DATA) {
        if (strcmp(t.header, ENT_INF) == 0) {
            write(1, CONNECTED_D, strlen(CONNECTED_D));
            write(1, DISCONNECTED_D, strlen(DISCONNECTED_D));

            Enterprise e;

            char * split = strtok(t.data, "&");
            e.nom = (char*) malloc(sizeof(char) * strlen(split));
            strcpy(e.nom, split);

            split = strtok(NULL, "&");
            e.port = atoi(split);

            split = strtok(NULL, "&");

            e.ip = (char*) malloc(sizeof(char) * strlen(split));
            strcpy(e.ip, split);

            //falta alliberar tot aixo

            return connectaServidor(0, picard, ENTERPRISE, &e);
        } else {
            write(1, ERROR_DATA, strlen(ERROR_DATA));
            return -1;
        }
    } else {
        if (strcmp(t.header, CONOK) == 0 && mode == ENTERPRISE) {
            write(1, CONNECTED_E, strlen(CONNECTED_E));
            return 1;
        } else if (strcmp(t.header, CONOK) != 0){
            write(1, ERROR_DATA, strlen(ERROR_DATA));
            return -1;
        }
    }
    return 1;
}

/******************************************************************************/
/**************************** FUNCIONS GENÈRIQUES *****************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     readTrama
* @Purpose  Funció llegirà una Trama donat un fd associat a un socket
* @Param    In: clientfd    Socket del que rebrem la trama
*           Out: error      Variable de control d'errors
* @return   Retorna la Trama llegida en cas de no haver-hi errors
*
*******************************************************************************/
Trama readTrama(int clientfd, int* error) {
    Trama trama;
    memset(&trama, 0, sizeof(trama));

    *error = read(clientfd, &trama.type, sizeof(trama.type));
    if (*error < 0) {
        return trama;
    }

    read(clientfd, &trama.header, sizeof(trama.header));

    char aux[3];
    read(clientfd, &aux, sizeof(trama.length));
    aux[2] = '\0';

    trama.length = (uint16_t)atoi(aux);

    trama.data = (char*) malloc(sizeof(char) * trama.length);
    read(clientfd, trama.data, sizeof(char) * trama.length);

    return trama;
}

/*******************************************************************************
*
* @Name     writeTrama
* @Purpose  Funció escriurà una Trama donat un fd associat a un socket
* @Param    In: clientfd    Socket al que escriurem la trama
*               type        Type de la trama a enviar
*               header      Header de la trama a enviar
*               data        Data de la trama a enviar
*           Out: -
* @return   Retorna la Trama llegida en cas de no haver-hi errors
*
*******************************************************************************/
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
