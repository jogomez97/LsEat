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


void stringToUpper(char* string) {
    int i;
    int fi = strlen(string);

    for (i = 0; i < fi; i++) {
        string[i] = toupper(string[i]);
    }
}

/******************************************************************************/
/************************ FUNCIONS CONNEXIÓ AMB DATA **************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     gestionaConnexioData
* @Purpose  Funció que gestiona tota la primera connexió amb data
* @Param    In: new     indica si és la primera connexió (1) o no (0)
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
            raise(SIGINT);
        }
        while (!done) {
            done = desconnecta(sockfd, 0);
            if (done) {
                write(1, DISCONNECTED_D, strlen(DISCONNECTED_D));
            }
        }
    }
}

/*******************************************************************************
*
* @Name     desconnecta
* @Purpose  Funció que ens permet desconnectar-nos de Data de dues maneres:
*               - enviant la trama de desconnexió si és la primera connexió
*               - tancant el socket directament si és un update
* @Param    In: sockfd    Socket de Data del qual ens volem desconnectar
*               dead      indica si hem de communicar-nos amb Data (1) o no (0)
*           Out: -
* @return  Retorna 1 si s'ha tancat el socket, 0 altrament
*
*******************************************************************************/
int desconnecta(int sockfd, int dead) {

    if (dead) {
        char* data = (char*) malloc(sizeof(int) + 1);
        sprintf(data, "%d", enterprise.portPicard);
        writeTrama(sockfd, 0x02, ENT_INF, data);
        free(data);

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
                    free(trama.data);
                    trama.data = NULL;
                    return 1;
                }
                free(trama.data);
                trama.data = NULL;
                return 0;
                break;
            default:
                write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                free(trama.data);
                trama.data = NULL;
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
* @Purpose  Funció que ens permet establir connexió amb Data mitjançant un Socket
* @Param    In:  -
*           Out: -
* @return   Retorna el valor del fd del Socket si s'ha pogut establir connexió,
*           -1 altrament
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
* @Purpose  Funció que permet gestiona l'enviament i tractament de les
*           diferents Trames de Data, depenent de si és el primer cop que ens
*           connectem o no.
* @Param    In: sockfd   Socket de Data al qual estem connectats
*               new      indica si és la primera connexió (1) o no (0)
*           Out: -
* @return   Retorna 0 si la communicació ha resultat satisfactòria, -1 altrament.
*
*******************************************************************************/
int enviaNovaConnexio(int sockfd, int new) {
    int length;
    Trama trama;
    int error;

    if (new) {
        length = strlen(enterprise.nom) + strlen(enterprise.ipData)
                + sizeof(enterprise.portPicard) + 2 * sizeof(char);
        char* buffer = (char*)malloc(sizeof(char) * length);
        sprintf(buffer, "%s&%d&%s", enterprise.nom, enterprise.portPicard, enterprise.ipPicard);

        writeTrama(sockfd, 0x01, ENT_INF, buffer);
        free(buffer);
        trama = readTrama(sockfd, &error);
        if (error <= 0) {
            write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
            close(sockfd);
            return -1;
        }

        switch(trama.type) {
            case 0x01:
                if (strcmp(trama.header, CONOK) == 0) {
                    free(trama.data);
                    trama.data = NULL;
                    return 0;
                } else if (strcmp(trama.header, CONKO) == 0) {
                    free(trama.data);
                    trama.data = NULL;
                    return -1;
                } else {
                    free(trama.data);
                    trama.data = NULL;
                    return -1;
                }
                break;
            default:
                free(trama.data);
                trama.data = NULL;
                write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                return -1;
        }
    } else {
        length = sizeof(int) + sizeof(char) + sizeof(enterprise.portPicard);
        char* buffer = (char*)malloc(sizeof(char) * length);
        sprintf(buffer, "%d&%d", enterprise.portPicard, enterprise.nConnections);

        writeTrama(sockfd, 0x07, UPDATE, buffer);
        free(buffer);
        trama = readTrama(sockfd, &error);
        if (error <= 0) {
            write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
            close(sockfd);
            return -1;
        }

        switch(trama.type) {
            case 0x07:
                if (strcmp(trama.header, UPDATEOK) == 0) {
                    free(trama.data);
                    trama.data = NULL;
                    return 0;
                } else if (strcmp(trama.header, UPDATEKO) == 0) {
                    free(trama.data);
                    trama.data = NULL;
                    return -1;
                } else {
                    free(trama.data);
                    trama.data = NULL;
                    return -1;
                }
                break;
            default:
                free(trama.data);
                trama.data = NULL;
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
* @Purpose  Thread corresponent al servidor general que atén les noves
*           peticions de connexió de Picards, tot creant un servidor dedicat
*           per cadascun d'ells.
* @Param    In: arg    Argument void* per prototip
*           Out: -
* @return   Retorna un void* per prototip
*
*******************************************************************************/
void* engegaServidor(void* arg) {
    int sockfd;

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


        int picardfd = accept(sockfd, (struct sockaddr*) &s_addr, &len);
        if (picardfd < 0) {
            write(1, ERROR_ACCEPT, strlen(ERROR_ACCEPT));
        } else {

            Picard p = createPicard(picardfd);
            pthread_t id;

            pthread_mutex_lock(&mtx);

            enterprise.nConnections++;
            insertNode(&clients, p);
            int* fd = checkLastElementFd(&clients);
            pthread_create(&id, NULL, threadPicard, fd);

            pthread_mutex_unlock(&mtx);
        }
    }

    return arg;
}

/*******************************************************************************
*
* @Name     threadPicard
* @Purpose  Thread en funció de servidor dedicat que gestiona la communicació
*           amb un sol Picard
* @Param    In: arg     Argument void* per prototip, es fa servir per
*                       obtindre el fd del Socket del Picard
*           Out: -
* @return   Retorna un void* per prototip
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
                    pthread_mutex_lock(&mtx);
                    addNameToElement(&clients, *picardfd, nom);
                    if (DEBUG_LIST) {
                        write(1, "ADDED:\n", 7);
                        printList(&clients);
                    }
                    pthread_mutex_unlock(&mtx);
                    int length = strlen(nom) + strlen("Connectat\n");
                    char* buff = (char*)malloc(sizeof(char) * length);
                    sprintf(buff, "Connectat %s\n", nom);
                    write(1, buff, strlen(buff));
                    free(buff);

                } else {
                    writeTrama(*picardfd, 0x01, CONKOb, "");
                }
                break;
            case 0x02:
                if (strcmp(trama.header, PIC_NAME) == 0) {
                    writeTrama(*picardfd, 0x02, CONOKb, "");

                    char* nom = strtok(trama.data, "\n");
                    int length = strlen(nom) + strlen("Desconnectat\n");
                    char* buff = (char*)malloc(sizeof(char) * length);
                    sprintf(buff, "Desconnectat %s\n", nom);
                    write(1, buff, strlen(buff));
                    free(buff);
                } else {
                    writeTrama(*picardfd, 0x02, CONKOb, "");
                }
                end = 1;
                close(*picardfd);
                pthread_mutex_lock(&mtx);
                enterprise.nConnections--;
                deleteNode(&clients, *picardfd);
                if (DEBUG_LIST) {
                    write(1, "DELETED:\n", 9);
                    printList(&clients);
                }
                pthread_mutex_unlock(&mtx);
                break;
            case 0x03:
                if (strcmp(trama.header, SHW_MENU) == 0) {
                    int i;
                    char* aux;
                    for (i = 0; i < menu.nPlats; i++) {
                        aux = getDishInFormat(i);
                        writeTrama(*picardfd, 0x03, DISH, aux);
                        free(aux);
                        aux = NULL;
                    }
                    writeTrama(*picardfd, 0x03, END_MENU, "");
                } else {
                    write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                    break;
                }
                break;
            case 0x04:
                if (strcmp(trama.header, NEW_ORD) == 0) {
                    int error = 1;
                    int i;
                    int quants;
                    char* nom = strtok(trama.data, "&");
                    for (i = 0; i < menu.nPlats; i++) {
                        char* aux = (char*)malloc(strlen(menu.plats[i].nom));
                        strcpy(aux, menu.plats[i].nom);
                        stringToUpper(aux);
                        if (strcmp(aux, nom) == 0) {
                            nom = strtok(NULL, "");
                            quants = atoi(nom);
                            pthread_mutex_lock(&mtxMenu);
                            if (menu.plats[i].quants >= quants) {
                                // APUNTAR AQUÍ LA RESERVA

                                menu.plats[i].quants = menu.plats[i].quants - quants;

                                pthread_mutex_unlock(&mtxMenu);
                                writeTrama(*picardfd, 0x04, ORDOK, "");
                                error = 0;
                                break;
                            } else {
                                pthread_mutex_unlock(&mtxMenu);
                                error = 2;
                                break;
                            }
                        }
                        free(aux);
                    }
                    if (error == 1) {

                        writeTrama(*picardfd, 0x04, ORDKO, "");
                    } else if (error == 2) {
    
                        writeTrama(*picardfd, 0x04, ORDKO2, "");
                    }
                    break;
                } else {
                    write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                    break;
                }
                break;
            case 0x05:
                if (strcmp(trama.header, DEL_ORD) == 0) {
                    /**
                        BORRAR
                    */
                    writeTrama(*picardfd, 0x05, ORDOK, "");
                } else {
                    write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                    break;
                }
                break;
            default:
                write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                break;
        }
        free(trama.data);
        trama.data = NULL;
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


    char* buffer2 = (char*)malloc(sizeof(char) * length);
    sprintf(buffer2, "%c%-10s%-2u%s", trama.type, trama.header, trama.length, trama.data);
    //Plenem el que falta de header amb '\0'
    for (i = 1; i < 11; i++) {
        if (buffer2[i] == ' ') {
            buffer2[i] = '\0';
        }
    }
    write(sockfd, buffer2, length);
    free(buffer2);

}
