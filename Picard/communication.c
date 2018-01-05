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
        }

        if (connect(sockfd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
            write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
            return -1;
        }

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
            free(t.data);
            t.data = NULL;
            close(sockfd);
            return -1;
        }
        free(t.data);
        t.data = NULL;

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

            free(e->ip);
            free(e->nom);

            if (error < 0) {
                write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
                return -1;
            }

            if (connect(sockfd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
                write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
                return -1;
            }

            char* aux = getPicardInfo(picard);
            writeTrama(sockfd, 0x01, PIC_INF, aux);
            free(aux);

            t = readTrama(sockfd, &error);

            if (error <= 0) {
                write(1, ERROR_DATA, strlen(ERROR_DATA));
                close(sockfd);
                return -1;
            }

            if (gestionaTrama(t, ENTERPRISE)) {
                // Connexió correcta, retornem el fd associat al socket
                free(t.data);
                t.data = NULL;
                return sockfd;
            }
            free(t.data);
            t.data = NULL;

            return -1;
        } else {
            write(1, ERROR_CONN, strlen(ERROR_CONN));
        }
    }
    return 1;
}


/*******************************************************************************
*
* @Name     showDishFromTrama
* @Purpose  Funció que mostrarà un plat per pantalla que li ha enviat enterprise
* @Param    In: data  informació de la trama que ha enviat Enterprise
*           Out: -
* @return   Retorna 0 si la trama era correcta, -1 altrament
*
*******************************************************************************/
int showDishFromTrama(char* data) {

    char* name  =  strtok(data, "&");
    char* price = strtok(NULL, "&");
    char* stock = strtok(NULL, "");
    if ((name != NULL) & (price != NULL) & (stock != NULL)) {
        if (atoi(stock) > 0) {
            char* line;
            asprintf(&line, "· %s (%s euros)\n", name, price);
            write(1, line, strlen(line));
            free(line);
            line = NULL;
            return 0;
        }
        return 0;
    }
    return -1;
}


void show() {
    if (connectat) {
        //Fem tot el pertinent per mostrar el menú
        writeTrama(sockfd, M_MENU, SHW_MENU, "");

        Trama trama;
        int error;
        write(1, MENU_DISP, strlen(MENU_DISP));
        while (1) {
            trama = readTrama(sockfd, &error);
            if (error <= 0) {
                write(1, REPEAT, strlen(REPEAT));
                break;
            } else {
                if (trama.type == M_MENU) {
                    if (strcmp(trama.header, DISH) == 0) {
                        error = showDishFromTrama(trama.data);
                        if (error < 0) {
                            write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                            free(trama.data);
                            trama.data = NULL;
                            break;
                        }
                    } else {
                        free(trama.data);
                        trama.data = NULL;
                        break;
                    }
                } else {
                    write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                    free(trama.data);
                    trama.data = NULL;
                    break;
                }
                free(trama.data);
                trama.data = NULL;
            }
        }
    } else {
        write(1, ERROR_NCONN, strlen(ERROR_NCONN));
    }

}

void order(char* plat, char* units) {
    if (connectat) {
        char* aux = getInfoComanda(plat, units);
        writeTrama(sockfd, DEMANA, NEW_ORD, aux);
        free(aux);
        aux = NULL;
        int error;
        Trama trama = readTrama(sockfd, &error);
        if (error <= 0) {
            write(1, REPEAT, strlen(REPEAT));
        } else {
            if (strcmp(trama.header, ORDOK) == 0) {
                Plat p;
                p.nom =  plat;
                p.quants = atoi(units);
                addDish(p);
                write(1, ORD_CORRECT, strlen(ORD_CORRECT));
            } else if (strcmp(trama.header, ORDKO) == 0) {
                write(1, ORD_INCORRECT, strlen(ORD_INCORRECT));
                write(1, ORD_KO, strlen(ORD_KO));
            } else if (strcmp(trama.header, ORDKO2) == 0) {
                write(1, ORD_INCORRECT, strlen(ORD_INCORRECT));
                write(1, ORD_KO2, strlen(ORD_KO2));
            } else if (strcmp(trama.header, ORDKO3) == 0) {
                write(1, ORD_INCORRECT, strlen(ORD_INCORRECT));
                write(1, ORD_KO3, strlen(ORD_KO3));
            } else {
                write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
            }
            free(trama.data);
            trama.data = NULL;
        }
    } else {
        write(1, ERROR_NCONN, strlen(ERROR_NCONN));
    }
}

void delete(char* plat, char* units) {

    if (connectat) {
        char* aux = getInfoComanda(plat, units);
        writeTrama(sockfd, ELIMINA, DEL_ORD, aux);
        free(aux);
        aux = NULL;
        int error;
        Trama trama = readTrama(sockfd, &error);
        if (error <= 0) {
            write(1, REPEAT, strlen(REPEAT));
        } else {
            if (strcmp(trama.header, ORDOK) == 0) {
                Plat p;
                p.nom =  plat;
                p.quants = atoi(units);
                removeDish(p);
                write(1, DEL_CORRECT, strlen(DEL_CORRECT));
            } else if (strcmp(trama.header, ORDKO) == 0) {
                write(1, ORD_INCORRECT, strlen(ORD_INCORRECT));
                write(1, DEL_KO, strlen(DEL_KO));
            } else if (strcmp(trama.header, ORDKO2) == 0) {
                write(1, ORD_INCORRECT, strlen(ORD_INCORRECT));
                write(1, DEL_KO2, strlen(DEL_KO2));
            } else {
                write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
            }
            free(trama.data);
            trama.data = NULL;
        }
    } else {
        write(1, ERROR_NCONN, strlen(ERROR_NCONN));
    }

}

void pay() {

    if (connectat) {
        //Fem tot el pertinent per a pagar

        writeTrama(sockfd, PAGAR, PAY_C, "");
        int error;
        Trama trama = readTrama(sockfd, &error);
        if (error <= 0) {
            write(1, REPEAT, strlen(REPEAT));
        } else {
            if (strcmp(trama.header, PAYOK) == 0) {
                char* p = strtok(trama.data, "");
                if (p != NULL) {
                    int compte = atoi(p);
                    picard.saldo -= compte;
                    printBill(compte);
                    eliminaReserva();
                } else {
                    write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                }
            } else if (strcmp(trama.header, PAYKO) == 0) {
                write(1, PAY_KO, strlen(PAY_KO));
            } else {
                write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
            }
            free(trama.data);
            trama.data = NULL;
        }
    } else {
        write(1, ERROR_NCONN, strlen(ERROR_NCONN));
    }
}

void enviaTotsElsPlats() {
    int i;
    int n = picard.nPlats;
    char* aux;

    for (i = 0; i < n; i++) {
        if (picard.plats[i].quants > 0) {
            asprintf(&aux, "Recuperant %d unitats del plat: %s\n", picard.plats[i].quants,
                    picard.plats[i].nom);
            write(1, aux, strlen(aux));
            free(aux);
            aux = NULL;

            asprintf(&aux, "%d", picard.plats[i].quants);
            order(picard.plats[i].nom, aux);
            free(aux);
            aux = NULL;
        }
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
        } else if (gestionaTrama(t, DSC_ENTERP)) {
            free(t.data);
            t.data = NULL;
            write(1, DISCONNECTED_E, strlen(DISCONNECTED_E));
            close(sockfd);
        } else {
            free(t.data);
            t.data = NULL;
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
            e.nom = strdup(split);


            split = strtok(NULL, "&");
            e.port = atoi(split);

            split = strtok(NULL, "");

            e.ip = strdup(split);

            return connectaServidor(0, picard, ENTERPRISE, &e);
        } else {
            if (strcmp(t.header, CONKO) == 0) {
                write(1, ERROR_NO_E_AVAIL, strlen(ERROR_NO_E_AVAIL));
                return -1;
            }
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
    if (*error <= 0) {
        if (connectat) {
            connectat = 0;
            close(sockfd);
            write(1, ERROR_E_DOWN, strlen(ERROR_E_DOWN));
            write(1, CONNECTING, strlen(CONNECTING));
            int a = connectaServidor(connectat, picard, DATA, NULL);
            if (a >= 1) {
                sockfd = a;
                write(1, CONNECTED, strlen(CONNECTED));
                connectat = 1;
                enviaTotsElsPlats();
            }
        }
        return trama;
    }

    read(clientfd, &trama.header, sizeof(trama.header));

    char aux[3];
    read(clientfd, &aux, sizeof(trama.length));
    aux[2] = '\0';

    trama.length = (uint16_t)atoi(aux);

    trama.data = (char*) malloc(sizeof(char) * trama.length + 1);
    read(clientfd, trama.data, sizeof(char) * trama.length);
    trama.data[trama.length] = '\0';

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

    char* buffer2;
    asprintf(&buffer2, "%c%-10s%-2u%s", trama.type, trama.header, trama.length, trama.data);
    //Plenem el que falta de header amb '\0'
    for (i = 1; i < 11; i++) {
        if (buffer2[i] == ' ') {
            buffer2[i] = '\0';
        }
    }
    write(clientfd, buffer2, length);
    free(buffer2);
    buffer2 = NULL;
}
