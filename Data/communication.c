/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Data
* Curs 2017-2018
*
* @File     communication.c
* @Purpose  Modul que conté les funcions relacionades amb les diferents connexions
*           de Enterprise a Data i de Picard a Data
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
*******************************************************************************/

// Llibreries pròpies
#include "communication.h"

/******************************************************************************/
/***************************** FUNCIONS DE PICARD *****************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     connectPicard
* @Purpose  Funció que realitzarà la connexió amb Picard per a poder després
*           gestionar-lo en forma de servidor dedicat
* @Param    In:  -
*           Out: -
* @return   Retorna -1 si hi ha hagut algun problema en engegar el servidor.
*
*******************************************************************************/
int connectPicard() {

    /* Obrir servidor */
    //Creació socket
    sockfdPicard = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfdPicard < 0) {
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
    }

    if (bind(sockfdPicard, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
        write(1, ERROR_BIND, strlen(ERROR_BIND));
        return -1;
    }

    //Listen
    listen(sockfdPicard, NCONN);

    //Accept
    socklen_t len = sizeof(s_addr);

    while (1) {
        write(1, WAIT_CONNECTP, strlen(WAIT_CONNECTP));
        clientfdPicard = accept(sockfdPicard, (struct sockaddr*) &s_addr, &len);
        if (clientfdPicard < 0) {
            write(1, ERROR_ACCEPT, strlen(ERROR_ACCEPT));
        } else {
            gestionaPicard(clientfdPicard);
        }
    }
    return 0;
}

/*******************************************************************************
*
* @Name     gestionaPicard
* @Purpose  Funció que gestionarà la interacció Data-Picard mitjançant sockets
* @Param    In:  clientfd   File Descriptor associat al Picard
*           Out: -
* @return   -
*
*******************************************************************************/
void gestionaPicard() {
    int error = 0;

    write(1, CONNECTED_P, strlen(CONNECTED_P));

    memset(&tramaPicard, 0, sizeof(tramaPicard));
    tramaPicard = readTrama(clientfdPicard, &error);


    if (error <= 0) {
        write(1, ERROR_DISCONNECTED, strlen(ERROR_DISCONNECTED));
        close(clientfdPicard);
    }
    switch (tramaPicard.type) {
        case 0x01:
            if (!isEmpty(&flota)) {
                char* data = getEnterprise();
                writeTrama(clientfdPicard, 0x01, ENT_INF, data);
                //free(data);
                //data = NULL;
                pthread_mutex_lock(&mtx);
                sortFirstNode(&flota);
                if (DEBUG_LIST) {
                    write(1, "SORTED:\n", 8);
                    printList(&flota);
                }
                pthread_mutex_unlock(&mtx);
            } else {
                writeTrama(clientfdPicard, 0x01, CONKOb, "");
            }
            write(1, DISCONNECTED_P, strlen(DISCONNECTED_P));
            break;
        default:
            write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
            break;
    }
    free(tramaPicard.data);
    tramaPicard.data = NULL;
    close(clientfdPicard);
}

/******************************************************************************/
/*************************** FUNCIONS DE ENTERPRISE ***************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     getEnterpriseFromTrama
* @Purpose  Funció que genera una Enterprise a partir de les seves dades
* @Param    In:  data Dades de l'enterprise
*           Out: -
* @return   Enterprise amb les dades introduides per paràmetre
*
*******************************************************************************/
Enterprise getEnterpriseFromTrama(char* data) {
    Enterprise e;
    char* port = strtok(data, "&");
    char* aux = strtok(NULL, "&");
    if ((port != NULL) & (aux != NULL)) {

        e.port = atoi(port);
        e.nConnections = atoi(aux);

        return e;
    }
    e.nConnections = -1;
    return e;
}

/*******************************************************************************
*
* @Name     getPortFromTrama
* @Purpose  Funció que retorna el port d'un Enterprise partir de les dades d'una trama
* @Param    In:  data  dada del port de l'enterprise
*           Out: -
* @return   Port de l'enterprise
*
*******************************************************************************/
int getPortFromTrama(char* data) {
    char* port = strtok(data, "[");
    int   nPort = -1;
    if (port != NULL) {
        nPort = atoi(port);
        return nPort;
    }
    return nPort;
}

/*******************************************************************************
*
* @Name     connectEnterprise
* @Purpose  Funció que realitzarà la connexió estable amb Enterprise per a poder
*           després gestionar-la.
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void connectEnterprise() {
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
    s_addr.sin_port = htons(d.portEnterprise);

    int error = inet_aton(d.ip, &s_addr.sin_addr);

    if (error < 0) {
        write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
        raise(SIGINT);
    }

    if (bind(sockfd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
        write(1, ERROR_BIND, strlen(ERROR_BIND));
        raise(SIGINT);
    }

    //Com s'ha pogut engegar el servidor Enterprise, també podrem intentar engegar
    //el de Picards
    pthread_barrier_wait(&barrier);

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
            gestionaEnterprise();
        }
    }
    close(sockfd);
}

/*******************************************************************************
*
* @Name     threadFunc
* @Purpose  Funció que ens realitzarà el thread, encarregada de connectar una
*           Enterprise a Data.
* @Param    In: arg Paràmetre que sempre demana les funcions associades a Threads
*           Out: -
* @return   -
*
*******************************************************************************/
void * threadFunc(void * arg) {
    connectEnterprise();
    return arg;
}

/*******************************************************************************
*
* @Name     creaThread
* @Purpose  Funció que crea un thread amb la nostra funció assignada a els Threads
*           de Data.
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void creaThread() {
    pthread_create(&threadEnterprise, NULL, threadFunc, NULL);
}

/*******************************************************************************
*
* @Name     gestionaEnterprise
* @Purpose  Funció que ens gestionarà tota la connexió relacionada amb una Enterprise,
*           escoltant el que envia i responent-li com demana el protocol.
* @Param    In: clientfd    File Descriptor del Enterprise al que hem de tractar
*           Out: -
* @return   -
*
*******************************************************************************/
void gestionaEnterprise() {
    int error;

    write(1, CONNECTED_E, strlen(CONNECTED_E));

    memset(&trama, 0, sizeof(trama));
    error = 0;
    trama = readTrama(clientfd, &error);

    if (error <= 0) {
        write(1, ERROR_DISCONNECTED, strlen(ERROR_DISCONNECTED));
        close(clientfd);
    } else {
        //Si no hi ha hagut cap error gestionem l'única trama de Enterprise,
        //ja que la connexió sempre és efímera
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
                    int port = getPortFromTrama(trama.data);
                    if (port != -1) {
                        pthread_mutex_lock(&mtx);
                        deleteNode(&flota, port);
                        if (DEBUG_LIST) {
                            write(1, "DELETED:\n", strlen("DELETED:\n"));
                            printList(&flota);
                        }
                        pthread_mutex_unlock(&mtx);
                        writeTrama(clientfd, 0x02, CONOKb, "");
                    } else {
                        writeTrama(clientfd, 0x02, CONKOb, "");
                    }
                } else {
                    writeTrama(clientfd, 0x02, CONKOb, "");
                }
                break;
            case 0x07:
                if (strcmp(trama.header, UPDATE) == 0) {
                    Enterprise e;
                    e = getEnterpriseFromTrama(trama.data);

                    if (e.nConnections == -1) {
                        writeTrama(clientfd, 0x07, UPDATEKO, "");
                    }
                    pthread_mutex_lock(&mtx);
                    updateNode(&flota, e);
                    pthread_mutex_unlock(&mtx);
                    if (DEBUG_LIST) {
                        write(1, "UPDATED:\n", 9);
                        printList(&flota);
                    }
                    writeTrama(clientfd, 0x07, UPDATEOK, "");
                } else {
                    writeTrama(clientfd, 0x07, UPDATEKO, "");
                }
                write(1, DISCONNECTED_E, strlen(DISCONNECTED_E));
                writeTrama(clientfd, 0x07, UPDATEOK, "");
                break;
            default:
                write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                break;
        }
        free(trama.data);
        trama.data = NULL;
        close(clientfd);
    }


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
    Trama tramaRead;
    memset(&tramaRead, 0, sizeof(tramaRead));

    *error = read(clientfd, &tramaRead.type, sizeof(tramaRead.type));
    if (*error < 0) {
        return tramaRead;
    }
    read(clientfd, &tramaRead.header, sizeof(tramaRead.header));

    char aux[3];
    read(clientfd, &aux, sizeof(tramaRead.length));

    aux[2] = '\0';

    tramaRead.length = (uint16_t)atoi(aux);

    trama.data = (char*) malloc(sizeof(char) * trama.length + 1);
    read(clientfd, trama.data, sizeof(char) * trama.length);
    trama.data[trama.length] = '\0';


    return tramaRead;
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
    Trama tramaWrite;
    int length;
    int i;

    memset(&tramaWrite, 0, sizeof(tramaWrite));

    //Afegim les dades a trama
    tramaWrite.type = type;
    strcpy(tramaWrite.header, header);
    tramaWrite.data = data;
    tramaWrite.length = strlen(tramaWrite.data);

    //Enviem Trama
    length = sizeof(tramaWrite.type) + sizeof(tramaWrite.header)
            + sizeof(tramaWrite.length) + strlen(tramaWrite.data);

    char* buffer2 = (char*) malloc(sizeof(char) * length);
    sprintf(buffer2, "%c%-10s%-2u%s", tramaWrite.type, tramaWrite.header,
            tramaWrite.length, tramaWrite.data);
    //Plenem el que falta de header amb '\0'
    for (i = 1; i < 11; i++) {
        if (buffer2[i] == ' ') {
            buffer2[i] = '\0';
        }
    }
    write(clientfd, buffer2, length);
    free(buffer2);

}
