/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Enterprise
* Curs 2017-2018
*
* @File     communication.c
* @Purpose  Modul que conté les funcions relacionades amb les diferents connexions
*           de Enterprise a Data i de Picard a Enterprise
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
*******************************************************************************/

// Llibreries pròpies
#include "communication.h"

/******************************************************************************/
/************************ FUNCIONS CONNEXIÓ AMB DATA **************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     readTrama
* @Purpose  ????????
* @Param    In: new    ?????
*           Out: -
* @return   -
*
*******************************************************************************/
void gestionaConnexioData(int new) {
    int done = 0;

    int sockfd = connectaData();
    if (sockfd < 0) {
        raise(SIGINT);
    } else {
        write(1, CONNECTED_D, strlen(CONNECTED_D));

        int error = enviaNovaConnexio(sockfd, new);
        if (error) {
            desconnecta(sockfd, new);
            raise(SIGINT);
        }
        while (!done) {
            done = desconnecta(sockfd, new);
            if (done) {
                write(1, DISCONNECTED_D, strlen(DISCONNECTED_D));
            }
        }
    }
}

/*******************************************************************************
*
* @Name     desconnecta
* @Purpose  ??????????
* @Param    In: sockfd    ???????
*               new       ??????
*           Out: -
* @return   ?????????????
*
*******************************************************************************/
int desconnecta(int sockfd, int new) {

    if (new) {
        char data[sizeof(int) + 1];
        sprintf(data, "%d", enterprise.portData);
        writeTrama(sockfd, 0x02, ENT_INF, data);

        int read = 0;

        Trama trama = readTrama(sockfd, &read);
        if (read <= 0) {
            write(1, ERROR_DISCONNECTED, strlen(ERROR_DISCONNECTED));
            close(sockfd);
            return 1;
        }

        switch (trama.type) {
            case 0x02:
                if (strcmp(trama.header, CONOKb) == 0) {
                    close(sockfd);
                    return 1;
                }
                return 0;
                break;
            default:
                write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                return 0;
        }
    } else {
        close(sockfd);
        return 1;
    }

}

/*******************************************************************************
*
* @Name     connectaData
* @Purpose  ???????????????
* @Param    In:  -
*           Out: -
* @return   ??????????????
*
*******************************************************************************/
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

/*******************************************************************************
*
* @Name     enviaNovaConnexio
* @Purpose  ????????????
* @Param    In: sockfd   ?????????????
*               new      ????????????
*           Out: -
* @return   ??????????
*
*******************************************************************************/
int enviaNovaConnexio(int sockfd, int new) {
    int length;
    Trama trama;
    int error;

    if (new) {
        length = strlen(enterprise.nom) + strlen(enterprise.ipData)
                + sizeof(enterprise.portPicard) + 2 * sizeof(char);
        char buffer[length];
        sprintf(buffer, "%s&%d&%s", enterprise.nom, enterprise.portPicard, enterprise.ipPicard);

        writeTrama(sockfd, 0x01, ENT_INF, buffer);
        trama = readTrama(sockfd, &error);
        if (error <= 0) {
            write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
            close(sockfd);
            return -1;
        }

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
    } else {
        length = sizeof(int) + sizeof(char) + sizeof(enterprise.portPicard);
        char buffer[length];
        sprintf(buffer, "%d&%d", enterprise.portPicard, enterprise.nConnections);

        writeTrama(sockfd, 0x07, UPDATE, buffer);
        trama = readTrama(sockfd, &error);
        if (error <= 0) {
            write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
            close(sockfd);
            return -1;
        }

        switch(trama.type) {
            case 0x07:
                if (strcmp(trama.header, UPDATEOK) == 0) {
                    return 0;
                } else if (strcmp(trama.header, UPDATEKO) == 0) {
                    return -1;
                } else {
                    return -1;
                }
                break;
            default:
                write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                return -1;
        }

    }


    return 0;
}

/******************************************************************************/
/************************ FUNCIONS CONNEXIÓ DE PICARDS ************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     engegaServidor
* @Purpose  ????????
* @Param    In: arg    ?????
*           Out: -
* @return   ?????????
*
*******************************************************************************/
void* engegaServidor(void* arg) {
    int sockfd;
    int* picardfds;

    /* Obrir servidor */
    //Creació socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        write(1, ERROR_SOCK, strlen(ERROR_SOCK));
        raise(SIGINT);
    }

    //Bind
    struct sockaddr_in s_addr;
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(enterprise.portPicard);

    int error = inet_aton(enterprise.ipPicard, &s_addr.sin_addr);

    if (error < 0) {
        write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
        raise(SIGINT);
    }

    if (bind(sockfd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
        write(1, ERROR_BIND, strlen(ERROR_BIND));
        raise(SIGINT);
    }

    //Listen
    listen(sockfd, NCONN);

    //Accept
    socklen_t len = sizeof(s_addr);


    while (1) {
        write(1, WAITING, strlen(WAITING));
        if (enterprise.nConnections == 0) {
            picardfds = (int*) malloc(sizeof(int));
        } else {
            picardfds = (int*) realloc(picardfds, sizeof(int) * (enterprise.nConnections + 1));
        }
        int picardfd = accept(sockfd, (struct sockaddr*) &s_addr, &len);
        if (picardfd < 0) {
            write(1, ERROR_ACCEPT, strlen(ERROR_ACCEPT));
        } else {
            picardfds[enterprise.nConnections] = picardfd;
            enterprise.nConnections++;
            write(1, CONNECTED_P, strlen(CONNECTED_P));

            pthread_t id;

            pthread_create(&id, NULL, threadPicard, &picardfds[enterprise.nConnections - 1]);
        }
    }

    return arg;
}

/*******************************************************************************
*
* @Name     threadPicard
* @Purpose  ????????
* @Param    In: arg    ?????
*           Out: -
* @return   ??????????
*
*******************************************************************************/
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
                pthread_mutex_lock(&mtx);
                enterprise.nConnections--;
                pthread_mutex_unlock(&mtx);
                break;
            default:
                write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                break;
        }
    }
    return arg;
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
    read(clientfd, &trama.header, sizeof(trama.header));
    if (*error < 0) {
        return trama;
    }
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
    sprintf(buffer2, "%c%-10s%-2u%s", trama.type, trama.header, trama.length, trama.data);
    //Plenem el que falta de header amb '\0'
    for (i = 1; i < 11; i++) {
        if (buffer2[i] == ' ') {
            buffer2[i] = '\0';
        }
    }
    write(sockfd, buffer2, length);

}
